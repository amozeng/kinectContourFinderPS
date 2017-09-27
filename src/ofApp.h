#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "CvUtils.h"
#include "ParticleSystem.h"

#include "baseParticle.h"
#include "sphereParticle.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();
    
        void drawPointCloud();
        void createCubeParticle();
        void createSphereParticle();


		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    //--------------- uncategorized stuff
    float colorValue;
    bool isfin  = false;
    bool turnWhite = false;
    bool drawGrayImage = false;
    bool drawAllImage = false;

    
    //--------------- baseParticle
    std::vector<std::shared_ptr<baseParticle> > particles;
    int maxParticles = 300;
    
    //--------------- sound
    ofSoundPlayer sound;
    bool playSound;
    bool detectCountour;
    
    //--------- Contour Finder
    
    std::vector<glm::vec2> getRandomPositionInsideOfPolyline(const ofPolyline& polyline, std::size_t howMany);
    std::vector<glm::vec2> getRandomPositionInsideTips(const ofPolyline& polyline, std::size_t howMany);

    
    /// \brief The ContourFinder object.
    ofxCv::ContourFinder contourFinderOfxCv;

    ofRectangle rect;
    //---------- kinect
    
    ofxKinect kinect;
    
    ofxCvColorImage colorImg;
    
    ofxCvGrayscaleImage grayImage; // grayscale depth image
    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    
    bool bThreshWithOpenCV;
    bool bDrawPointCloud;
    
    int nearThreshold;
    int farThreshold;
    
    int angle;
    
    //----------- Particle System
    std::size_t addParticles = 100;
    std::size_t totalParticles = 5000;
    
    ParticleSystem ps;
    
    
    // used for viewing the point cloud
    ofEasyCam easyCam;
    
    //--------------- assignment 01
    float hullMinumumDefectDepth = 10;
    ofPolyline getPreparedContour(std::size_t i) const;
    
    //--------------- gui
    ofxPanel gui;
    ofxFloatSlider smooth;
    ofxFloatSlider simplify;
    ofxFloatSlider x;
    ofxFloatSlider y;
    ofxFloatSlider w;
    ofxFloatSlider h;
    ofxFloatSlider nearThresholds;
    ofxFloatSlider farThresholds;
    ofxFloatSlider posX;
    ofxFloatSlider posY;
	
};
