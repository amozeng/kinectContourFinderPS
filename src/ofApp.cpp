#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofDisableSmoothing();
    
    colorValue = 0;

    //------ sound
    sound.load("tibetan_bell.aiff");
    playSound = true;
    detectCountour = false;
    
    //--------- Contour Finder ofxCv
    
    // Set the blob minimum size.
    contourFinderOfxCv.setMinAreaRadius(80);
    
    // Set the blob maximum size.
    contourFinderOfxCv.setMaxAreaRadius(1000);
    
    // Q. What does this do?
    contourFinderOfxCv.setSimplify(true);
    
    //---------- kinect
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // enable depth->video image calibration
    kinect.setRegistration(true);
    
    kinect.init();
    //kinect.init(true); // shows infrared instead of RGB video image
    //kinect.init(false, false); // disable video image (faster fps)
    
    kinect.open();		// opens first available kinect
    //kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
    //kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
    
    // print the intrinsic IR sensor values
    if(kinect.isConnected()) {
        ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
        ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
        ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
        ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
    }
    
    
    colorImg.allocate(kinect.width, kinect.height);
    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);
    
    nearThreshold = 10;
    farThreshold = 178;
    bThreshWithOpenCV = true;
    
    ofSetFrameRate(60);
    
    // zero the tilt on startup
    angle = 10;
    kinect.setCameraTiltAngle(angle);
    
    // start from the front
    bDrawPointCloud = false;
    
    //----- gui
    gui.setup();
    gui.add(smooth.setup("smooth", 10.0, 0.0, 20));
    gui.add(simplify.setup("simplify", 30.0, 0.0, 40.0));
    gui.add(x.setup("x", 20, 0, 400));
    gui.add(y.setup("y", 0, 0, 400));
    gui.add(w.setup("w", 1500, 0, 1500));
    gui.add(h.setup("h", 700, 0, 800));
    gui.add(nearThresholds.setup("nearThresholds", 80, 0, 150));
    gui.add(farThresholds.setup("farThresholds", 160, 0, 255));
    gui.add(posX.setup("posX", -100, -500, 2000));
    gui.add(posY.setup("posY", 0, 0, 2000));





}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofBackground(0);
    ofSoundUpdate();

    
    totalParticles += addParticles;
    if (totalParticles >= 3000){
        addParticles = 0;
    }
    
    kinect.update();
    
    // there is a new frame and we are connected
    if(kinect.isFrameNew()) {
        
        // load grayscale depth image from the kinect source
        grayImage.setFromPixels(kinect.getDepthPixels());
        
        // we do two thresholds - one for the far plane and one for the near plane
        // we then do a cvAnd to get the pixels which are a union of the two thresholds
        if(false){
//            bThreshWithOpenCV) {
            grayThreshNear = grayImage;
            grayThreshFar = grayImage;
            grayThreshNear.threshold(nearThresholds, true);
            grayThreshFar.threshold(farThresholds);
            cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
        } else {
            
            // or we do it ourselves - show people how they can work with the pixels
            ofPixels & pix = grayImage.getPixels();
            int numPixels = pix.size();
            
            rect.set(x,y,w,h);
            
            // Region of Interest
            for(std::size_t y= 0; y < grayImage.getHeight(); y++){
                for(std::size_t x = 0; x < grayImage.getWidth(); x++) {
                    if (!rect.inside(x, y))
                    {
                        pix.setColor(x, y, 255);
                    }
                }
            }

            
            for(int i = 0; i < numPixels; i++) {
                if(255 - pix[i] > nearThresholds && 255 - pix[i] < farThresholds) {
                    pix[i] = 255;
                } else {
                    pix[i] = 0;
                }
            }
        }
        
        grayImage.flagImageChanged();
        
        contourFinderOfxCv.findContours(grayImage);
    }
    
    for (auto contourIndex = 0; contourIndex < contourFinderOfxCv.size(); ++contourIndex)
    {
        
        const ofPolyline contour = contourFinderOfxCv.getPolylines()[contourIndex];
        
        ConvexHull convexHull(contour, hullMinumumDefectDepth);
        
        ofPolyline convexHullSmoothed = convexHull.convexHull();
        
        convexHullSmoothed.simplify(simplify);
        
        //draw SphereParticles
        if (convexHullSmoothed.size() > 0)
        {
            while (particles.size() <= maxParticles)
            {
                createSphereParticle();
                //createCubeParticle();
            }
        }
    }
    
    // play sound first detect a contour
    if (!detectCountour && contourFinderOfxCv.size() > 0)
    {
        sound.play();
//        for (auto contourIndex = 0; contourIndex < contourFinderOfxCv.size(); ++contourIndex)
//        {
//            
//            const ofPolyline contour = contourFinderOfxCv.getPolylines()[contourIndex];
//            
//            ConvexHull convexHull(contour, hullMinumumDefectDepth);
//            
//            ofPolyline convexHullSmoothed = convexHull.convexHull();
//            
//            convexHullSmoothed.simplify(simplify);
//            
//            //draw SphereParticles
//            if (convexHullSmoothed.size() > 0)
//            {
//                while (particles.size() <= maxParticles)
//                {
//                    createSphereParticle();
//                    //createCubeParticle();
//                }
//            }
//        }

        
    }

    
    if (contourFinderOfxCv.size() > 0)
    {
        detectCountour = true;
        ofPolyline ourBiggestContour = contourFinderOfxCv.getPolyline(0);
        if (!isfin)
        {
            for (auto& p: ps.particles)
            {
                if (!ourBiggestContour.inside(p->position.x, p->position.y))
                {
                    p->ageSpeed = 10;
                    p->opcDecreaseSpeed = 3.0;
                }
                else
                {
                    p->ageSpeed = 0;
                }
            }
        }
        else
        {
            for (auto& p: ps.particles)
            {
                if (!ourBiggestContour.inside(p->position.x, p->position.y))
                {
                    p->ageSpeed = 0;
                    p->opcDecreaseSpeed = 2.275;
                }
            }
        }

//        for (auto& p: ps.particles)
//        {
//            if (!ourBiggestContour.inside(p->position.x, p->position.y))
//            {
//                p->ageSpeed = 10;
//                p->opcDecreaseSpeed = 3.0;
//            }
//            else
//            {
//                p->ageSpeed = 0;
//            }
//        }
        
        if (ps.particles.size() < totalParticles)
        {
            std::size_t numNeeded = totalParticles - ps.particles.size();
            
            std::vector<glm::vec2> newParticlesA = ofApp::getRandomPositionInsideOfPolyline(ourBiggestContour, numNeeded * 0.75);
            
            std::vector<glm::vec2> newParticlesB = ofApp::getRandomPositionInsideTips(ourBiggestContour, numNeeded * 0.30);
            
            std::vector<glm::vec2> newParticles = newParticlesA;
            newParticles.insert(newParticles.end(), newParticlesB.begin(), newParticlesB.end());
            
            
            for (auto position: newParticlesB)
            {
                glm::vec2 velocity;
                velocity.x = ofRandom(-1, 1);
                velocity.y = ofRandom(-1, 1);
                
                
                ps.particles.push_back(std::make_unique<Particle>(position, velocity));
            }
        }

    }else{
        detectCountour = false;
    }
    
    ps.update();
    
    // Update the baseParticle
    for (std::size_t i = 0; i < particles.size(); i++)
    {
        particles[i]->update();
    }
    
    // Create an iterator for the particle vector.
    std::vector<std::shared_ptr<baseParticle> >::iterator iter = particles.begin();
    
    while (iter != particles.end())
    {
        if ((*iter)->getAge() > (*iter)->maximumAge)
        {
            iter = particles.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
    
    if(turnWhite){
        if(colorValue < 255){
            colorValue += 3;
        }else{
            colorValue += 0;
        }
    }
    if(!turnWhite){
        if(colorValue > 0){
            colorValue -= 3;
        }else{
            colorValue -= 0;
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(colorValue);
    
    if(bDrawPointCloud) {
        easyCam.begin();
        drawPointCloud();
        easyCam.end();
    }
    
    if(drawGrayImage){
        ofSetColor(255);
        grayImage.draw(10, 320, 400, 300);

    }
    if(drawAllImage){
        
    
        // draw from the live kinect
        
        kinect.drawDepth(10, 10, 400, 300);
        kinect.draw(420, 10, 400, 300);
        
        grayImage.draw(10, 320, 400, 300);
    }
        //contourFinderOfxCv.draw();
    
    
    //easyCam.begin();
    //ofRotateDeg(ofGetElapsedTimef(), 0, 0, 1);

    
    //--------- Particle System
    ofPushMatrix();
    ofTranslate(posX, posY);
    //ofScale(-1.5, 1.5);
    ofScale(2.0);

    contourFinderOfxCv.draw();
    ps.draw();
    // draw all baseParticle & children
    for (std::size_t i = 0; i < particles.size(); i++)
    {
        // Update the particle.
        particles[i]->draw();
    }

    ofPopMatrix();
    
    //--------- draw rect on the convexHull
//    for (auto contourIndex = 0; contourIndex < contourFinderOfxCv.size(); ++contourIndex)
//    {
//        
////        const ofPolyline contour = contourFinderOfxCv.getPolylines()[contourIndex];
////        
////        ConvexHull convexHull(contour, hullMinumumDefectDepth);
//        
//        const ofPolyline contour = contourFinderOfxCv.getPolylines()[contourIndex];
//        
//        ConvexHull convexHull(contour, hullMinumumDefectDepth);
//        
//        ofPolyline convexHullSmoothed = convexHull.convexHull();
//        
//        convexHullSmoothed.simplify(simplify);
//        
//        for (auto point: convexHull.convexHull()){
//            ofSetColor(0,0,255, 100);
//            ofDrawRectangle(point.x, point.y, 100, 100);
//        }
//    }

    
    // draw instructions
    ofSetColor(255, 255, 255);
    stringstream reportStream;
    
    if(kinect.hasAccelControl()) {
        reportStream << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
        << ofToString(kinect.getMksAccel().y, 2) << " / "
        << ofToString(kinect.getMksAccel().z, 2) << endl;
    } else {
        reportStream << "Note: this is a newer Xbox Kinect or Kinect For Windows device," << endl
        << "motor / led / accel controls are not currently supported" << endl << endl;
    }
    
    reportStream << "press p to switch between images and point cloud, rotate the point cloud with the mouse" << endl
    << "using opencv threshold = " << bThreshWithOpenCV <<" (press spacebar)" << endl
    << "set near threshold " << nearThreshold << " (press: + -)" << endl
    << "set far threshold " << farThreshold << " (press: < >) num blobs found " << contourFinderOfxCv.size()
    << ", fps: " << ofGetFrameRate() << endl
    << "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;
    
    if(kinect.hasCamTiltControl()) {
        reportStream << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl
        << "press 1-5 & 0 to change the led mode" << endl;
    }
    
    //ofDrawBitmapString(reportStream.str(), 20, 652);
    
    
    //easyCam.end();

    //gui.draw();

}

//--------------------------------------------------------------
void ofApp::drawPointCloud() {
    int w = 640;
    int h = 480;
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_POINTS);
    
    float nt = ofMap(nearThresholds, 0, 255, kinect.getNearClipping(), kinect.getFarClipping(), true);
    float ft = ofMap(farThresholds, 0, 255, kinect.getNearClipping(), kinect.getFarClipping(), true);

    int step = 2;
    for(int y = 0; y < h; y += step) {
        for(int x = 0; x < w; x += step) {
            
            float d = kinect.getDistanceAt(x, y);
            
            if(d > 0) {
                
                if (d > nt && d < ft)
                {
                    mesh.addColor(kinect.getColorAt(x,y));
                }
                else
                {
                    mesh.addColor(ofColor::yellow);
                }
                
                mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
            }
        }
    }
    glPointSize(3);
    ofPushMatrix();
    // the projected points are 'upside down' and 'backwards'
    ofScale(1, -1, -1);
    ofTranslate(0, 0, -1000); // center the points a bit
    ofEnableDepthTest();
    mesh.drawVertices();
    

    
    ofPushStyle();
    ofSetColor(255, 0, 0, 80);
    ofDrawPlane(0, 0, nt, 5000, 5000);
    ofSetColor(0, 255, 0, 80);
    ofDrawPlane(0, 0, ft, 5000, 5000);
    ofPopStyle();
    
    ofDisableDepthTest();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::exit() {
    kinect.setCameraTiltAngle(0); // zero the tilt on exit
    kinect.close();
}

//--------------------------------------------------------------
std::vector<glm::vec2> ofApp::getRandomPositionInsideOfPolyline(const ofPolyline& polyline, std::size_t howMany)
{
    std::vector<glm::vec2> results;
    
    ofRectangle boundBox = polyline.getBoundingBox();

    
    for (std::size_t i = 0; i < howMany; ++i)
    {
        glm::vec2 v;
        
        while (!polyline.inside(v.x, v.y))
        {
            v.x = ofRandom(boundBox.getMinX(), boundBox.getMaxX());
            v.y = ofRandom(boundBox.getMinX(), boundBox.getMaxX());
        }
        
        results.push_back(v);
    }
    
    return results;
    
}

std::vector<glm::vec2> ofApp::getRandomPositionInsideTips(const ofPolyline& polyline, std::size_t howMany)
{
    std::vector<glm::vec2> results;
    
    ofRectangle boundBox = polyline.getBoundingBox();
    for (auto contourIndex = 0; contourIndex < contourFinderOfxCv.size(); ++contourIndex)
    {
        
        const ofPolyline contour = contourFinderOfxCv.getPolylines()[contourIndex];
        
        ConvexHull convexHull(contour, hullMinumumDefectDepth);

        ofPolyline convexHullSmoothed = convexHull.convexHull();

        convexHullSmoothed.simplify(simplify);
        
        for (std::size_t i = 0; i < convexHullSmoothed.size(); ++i){

            ofRectangle smallBox;
            
            smallBox.setFromCenter(convexHullSmoothed[i].x, convexHullSmoothed[i].y, 100, 100);
            ofSetColor(255,255,0,200);
            ofDrawCircle(convexHullSmoothed[i].x, convexHullSmoothed[i].y, 200);
            ofRectangle intersectionBox = boundBox.getIntersection(smallBox);
            
            
            for (std::size_t i = 0; i < howMany; ++i)
            {
                glm::vec2 v;
                
                while (!polyline.inside(v.x, v.y))
                {
                    v.x = ofRandom(intersectionBox.getMinX(), intersectionBox.getMaxX());
                    v.y = ofRandom(intersectionBox.getMinY(), intersectionBox.getMaxY());
                }
                
                results.push_back(v);
            }
        }
    }
    
    return results;
    
}

void ofApp::createCubeParticle(){
    // Create a half-sized box for calculations.
    ofPoint halfBoxSize = ofPoint(300,300,300);
    
    // Instantiate our base particle.
    std::shared_ptr<cubeParticle> aParticle = std::make_shared<cubeParticle>();
    
    // Change postion on convexHull
    for (auto contourIndex = 0; contourIndex < contourFinderOfxCv.size(); ++contourIndex)
    {
        ofPolyline contour = contourFinderOfxCv.getPolylines()[contourIndex];
        
        ConvexHull convexHull(contour, hullMinumumDefectDepth);
        
        ofPolyline convexHullSmoothed = convexHull.convexHull();
        
        convexHullSmoothed.simplify(simplify);
        
        for (auto point: convexHullSmoothed){
            
            aParticle->position = ofPoint(point.x, point.y, 0);
        }
        
    }
    
    // Set a random velocity.
    aParticle->velocity = ofPoint(ofRandom(-2, 2),
                                  ofRandom(-5, 0),
                                  ofRandom(-3, 3));
    
    // Set a random drag.
    float drag = ofRandom(0.95, 0.999);
    
    // Our drag is the same in the x, y and z directions.
    aParticle->drag = ofPoint(drag, drag, drag);
    
    // Create a random size for our particle.
    float size = 20;
    
    // Our particle size is the same in the x, y and z directions.
    aParticle->size = ofPoint(size, size, size);
    
    // Assign a random color to the particle.
    aParticle->color = ofColor(255, 200);
    
    // Add the particle to our collection.
    particles.push_back(aParticle);
    
}

void ofApp::createSphereParticle()
{
    // Create a half-sized box for calculations.
    ofPoint halfBoxSize = ofPoint(300,300,300);
    
    // Instantiate our base particle.
    std::shared_ptr<sphereParticle> aParticle = std::make_shared<sphereParticle>();
    
    // Change postion on convexHull
    for (auto contourIndex = 0; contourIndex < contourFinderOfxCv.size(); ++contourIndex)
    {
        ofPolyline contour = contourFinderOfxCv.getPolylines()[contourIndex];
        
        ConvexHull convexHull(contour, hullMinumumDefectDepth);
        
        ofPolyline convexHullSmoothed = convexHull.convexHull();
        
        convexHullSmoothed.simplify(simplify);
        
        
        
        for (auto point: convexHullSmoothed){
            
            aParticle->position = ofPoint(point.x, point.y, 0);
        }
        
    }
    
    // Set a random velocity.
    aParticle->velocity = ofPoint(ofRandom(-2, 2),
                                  ofRandom(5, 0),
                                  ofRandom(-3, 3));
    
    // Set a random drag.
    float drag = ofRandom(0.990, 0.999);
    
    // Our drag is the same in the x, y and z directions.
    aParticle->drag = ofPoint(drag, drag, drag);
    
    // Create a random size for our particle.
    float size = 3;
    
    // Our particle size is the same in the x, y and z directions.
    aParticle->size = ofPoint(size, size, size);
    
    // Assign a random color to the particle.
    aParticle->color = ofColor(255, 0, 0,127);
    
    // Add the particle to our collection.
    particles.push_back(aParticle);
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key) {
        case 'q':
            bThreshWithOpenCV = !bThreshWithOpenCV;
            break;
            
        case ' ':
            turnWhite = !turnWhite;
            break;
            
        case '7':
            drawGrayImage = !drawGrayImage;
            break;
            
        case '8':
            drawAllImage = !drawAllImage;
            break;
            
        case'p':
            bDrawPointCloud = !bDrawPointCloud;
            break;
            
        case '>':
        case '.':
            farThreshold ++;
            if (farThreshold > 255) farThreshold = 255;
            break;
            
        case '<':
        case ',':
            farThreshold --;
            if (farThreshold < 0) farThreshold = 0;
            break;
            
        case '+':
        case '=':
            nearThreshold ++;
            if (nearThreshold > 255) nearThreshold = 255;
            break;
            
        case '-':
            nearThreshold --;
            if (nearThreshold < 0) nearThreshold = 0;
            break;
            
        case 'w':
            kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
            break;
            
        case 'o':
            kinect.setCameraTiltAngle(angle); // go back to prev tilt
            kinect.open();
            break;
            
        case 'c':
            kinect.setCameraTiltAngle(0); // zero the tilt
            kinect.close();
            break;
            
        case '1':
            kinect.setLed(ofxKinect::LED_GREEN);
            break;
            
        case '2':
            kinect.setLed(ofxKinect::LED_YELLOW);
            break;
            
        case '3':
            kinect.setLed(ofxKinect::LED_RED);
            break;
            
        case '4':
            kinect.setLed(ofxKinect::LED_BLINK_GREEN);
            break;
            
        case '5':
            kinect.setLed(ofxKinect::LED_BLINK_YELLOW_RED);
            break;
            
        case '0':
            kinect.setLed(ofxKinect::LED_OFF);
            break;
            
        case OF_KEY_UP:
            angle++;
            if(angle>30) angle=30;
            kinect.setCameraTiltAngle(angle);
            break;
            
        case OF_KEY_DOWN:
            angle--;
            if(angle<-30) angle=-30;
            kinect.setCameraTiltAngle(angle);
            break;
    }


}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
