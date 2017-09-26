#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofDisableSmoothing();
    
    colorValue = 0;
    //lily.load("lily.png");
    lily.load("speechless.jpg");
    
//------ sound
    sound.load("tibetan_bell.aiff");
    playSound = true;
    detectCountour = false;
    
//------- contour Finder
    // Load a video.
    video.load("Shadows.mov");
    
    // Start the video playing.
    video.play();
    
    // Set the blob minimum size.
    contourFinder.setMinAreaRadius(80);
    
    // Set the blob maximum size.
    contourFinder.setMaxAreaRadius(1000);
    
    // Q. What does this do?
    contourFinder.setSimplify(true);
    
    setupGui();
    
    guiGroup.setName("gui");
    
    gui.setup(guiGroup);
    
    gui.add(opacitySpeedController.set("opacitySpeed", 2.275, 0.0, 5.0));
    gui.add(timeScaleController.set("timeScale", 0.1, 0.0, 5.0));
    gui.add(ageSpeedController.set("ageSpeed", 4.0, 0.0, 10.0));
    gui.add(smooth.set("smooth", 4.0, 0.0, 32.0));
    gui.add(simplify.set("simplify", 20.0, 0.0, 40.0));
    gui.add(rSpeed.set("rSpeed", 0.05, 0.00, 0.50));



}

//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(0);
    ofSoundUpdate();

    totalParticles += addParticles;
    if (totalParticles > 5000){
        addParticles = 0;
    }
    
    video.update();
    
    if (video.isFrameNew())
    {
        
        ofPixels & pix = video.getPixels();
        int numPixels = pix.size();
        rect.set(100,100,400,400);
        
        // Region of Interest
        for(std::size_t y= 0; y < video.getHeight(); y++){
            for(std::size_t x = 0; x < video.getWidth(); x++) {
                if (!rect.inside(x, y))
                {
                    pix.setColor(x, y, 255);
                }
            }
        }
        
        contourFinder.setTargetColor(finderTargetColor, finderTrackHueSaturation ? ofxCv::TRACK_COLOR_HS : ofxCv::TRACK_COLOR_RGB);
        
        contourFinder.findContours(video);
    }
    
    
    for (auto contourIndex = 0; contourIndex < contourFinder.size(); ++contourIndex)
    {
        
        ofPolyline contour = contourFinder.getPolylines()[contourIndex];
        
        ConvexHull convexHull(contour, hullMinumumDefectDepth);
        
        ofPolyline convexHullSmoothed = convexHull.convexHull();
        
        convexHullSmoothed.simplify(simplify);
        
        //draw blue rectangles on the smoothed convexHulls
        for (auto point: convexHullSmoothed){
            ofSetColor(0,0,255, 50);
            ofDrawRectangle(point.x, point.y, 100, 100);
            
        }
        
        //draw SphereParticles
        if (convexHullSmoothed.size() > 2)
        {
            while (particles.size() <= maxParticles)
            {
                createSphereParticle();
                createCubeParticle();
            }
        }
    }
    
    // play sound first detect a contour
    if (!detectCountour && contourFinder.size() > 0)
    {
        sound.play();
        
    }
    
    if (contourFinder.size() > 0)
    {
        
        detectCountour = true;
        
        ofPolyline ourBiggestContour = contourFinder.getPolyline(0);
        
        
        if (!isfin)
        {
            for (auto& p: ps.particles)
            {
                if (!ourBiggestContour.inside(p->position.x, p->position.y))
                {
                    p->ageSpeed = 10;
                    p->opcDecreaseSpeed = 3.0;
                    //p->age = 95;//std::numeric_limits<uint64_t>::max();
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
                    p->opcDecreaseSpeed = opacitySpeedController;
                    //p->age = 95;//std::numeric_limits<uint64_t>::max();
                }
            }
        }
        
        if (ps.particles.size() < totalParticles)
        {
            std::size_t numNeeded = totalParticles - ps.particles.size();
            
            std::vector<glm::vec3> newParticlesA = ofApp::getRandomPositionInsideOfPolyline(ourBiggestContour, numNeeded * 0.35);
            
            std::vector<glm::vec3> newParticlesB = ofApp::getRandomPositionInsideTips(ourBiggestContour, numNeeded * 0.20);
            
            std::vector<glm::vec3> newParticles = newParticlesA;
            newParticles.insert(newParticles.end(), newParticlesB.begin(), newParticlesB.end());
            
            
            for (auto position: newParticles)
            {
                glm::vec3 velocity;
                velocity.x = ofRandom(-1, 1);
                velocity.y = ofRandom(-1, 1);
                velocity.z = ofRandom(-1, 1);
                
                
                ps.particles.push_back(std::make_unique<Particle>(position, velocity));
            }
        }
    }else{
        detectCountour = false;
    }
    
    
    ps.update();

    for (std::size_t i = 0; i < particles.size(); i++)
    {
        // Update the particle.
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
    
    
//    while (particles.size() <= maxParticles)
//    {
//        createSphereParticle();
//        createCubeParticle();
//    }
    
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
    
    cam.begin();
    ofRotateDeg(sin(ofGetElapsedTimef() * rSpeed)* 5, 0, 1, 0);

    
    ofNoFill();
    ofPushMatrix();
    ofTranslate(video.getWidth(), 0);
    ofSetColor(255);
    video.draw(0, 0);
    ofPopMatrix();
    

    //draw the particle system
    ofPushMatrix();
    ofTranslate(0, 0);
    ps.draw();
    ofPopMatrix();
    

    
    // draw all baseParticle & children
    for (std::size_t i = 0; i < particles.size(); i++)
    {
        // Update the particle.
        particles[i]->draw();
    }
    
    cam.end();
    
    // draw gui
    ofPushMatrix();
    ofTranslate(video.getWidth(), video.getHeight() + 100);
    gui.draw();
    ofPopMatrix();

    drawGui();
    //ofSetColor(255);
    lily.draw(video.getWidth() + 200, video.getHeight() + 100);


}

std::vector<glm::vec3> ofApp::getRandomPositionInsideOfPolyline(const ofPolyline& polyline, std::size_t howMany)
{
    std::vector<glm::vec3> results;
    
    ofRectangle boundBox = polyline.getBoundingBox();
    
    
    for (std::size_t i = 0; i < howMany * 0.75; ++i)
    {
        glm::vec3 v;
        
        while (!polyline.inside(v.x, v.y))
        {
            v.x = ofRandom(boundBox.getMinX(), boundBox.getMaxX());
            v.y = ofRandom(boundBox.getMinY(), boundBox.getMaxY());
            v.z = ofRandom(-10,10);
        }
        
        results.push_back(v);
    }
    
    return results;
    
}

std::vector<glm::vec3> ofApp::getRandomPositionInsideTips(const ofPolyline& polyline, std::size_t howMany)
{
    std::vector<glm::vec3> results;
    
    ofRectangle boundBox = polyline.getBoundingBox();
    for (auto contourIndex = 0; contourIndex < contourFinder.size(); ++contourIndex)
    {
        
        ofPolyline contour = contourFinder.getPolylines()[contourIndex];
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
                glm::vec3 v;
                
                while (!polyline.inside(v.x, v.y))
                {
                    v.x = ofRandom(intersectionBox.getMinX(), intersectionBox.getMaxX());
                    v.y = ofRandom(intersectionBox.getMinY(), intersectionBox.getMaxY());
                    v.z = ofRandom(-10,10);

                }
                
                results.push_back(v);
            }
        }
    }
    return results;
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch(key){
            case ' ':
            turnWhite = !turnWhite;
            break;
    }

}

void ofApp::createCubeParticle(){
    // Create a half-sized box for calculations.
    ofPoint halfBoxSize = ofPoint(300,300,300);
    
    // Instantiate our base particle.
    std::shared_ptr<cubeParticle> aParticle = std::make_shared<cubeParticle>();
    
    // Change postion on convexHull
    for (auto contourIndex = 0; contourIndex < contourFinder.size(); ++contourIndex)
    {
        ofPolyline contour = contourFinder.getPolylines()[contourIndex];
        
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
    for (auto contourIndex = 0; contourIndex < contourFinder.size(); ++contourIndex)
    {
        ofPolyline contour = contourFinder.getPolylines()[contourIndex];
        
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

void ofApp::setupGui()
{
    // Set up the gui.
    guiFinder.setup();
    guiFinder.setPosition(0, video.getHeight());
    guiFinder.add(finderThreshold.set("Threshold", 60, 0, 255));
    guiFinder.add(finderTrackHueSaturation.set("Track Hue/Saturation", false));
    guiFinder.add(finderTargetColor.set("Track Color", ofColor(38, 23, 19)));
}


void ofApp::drawGui()
{
    guiFinder.draw();
}


