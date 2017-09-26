#pragma once

#include "ofMain.h"
#include "baseParticle.h"
#include "sphereParticle.h"

#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "CvUtils.h"
#include "ParticleSystem.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        void setupGui();
        void drawGui();
    
        std::vector<glm::vec3> getRandomPositionInsideOfPolyline(const ofPolyline& polyline, std::size_t howMany);
        std::vector<glm::vec3> getRandomPositionInsideTips(const ofPolyline& polyline, std::size_t howMany);


        void createCubeParticle();
        void createSphereParticle();
    

		void keyPressed(int key);
    
//--------------- uncategorized stuff
    float colorValue;
    bool isfin  = false;
    bool turnWhite = false;
    ofImage lily;
    ofEasyCam cam;


//--------------- baseParticle
    std::vector<std::shared_ptr<baseParticle> > particles;
    int maxParticles = 200;

//--------------- sound
    ofSoundPlayer sound;
    bool playSound;
    bool detectCountour;
    
//--------------- contour finder
    /// \brief The video player.
    ofVideoPlayer video;
    
    /// \brief The ContourFinder object.
    ofxCv::ContourFinder contourFinder;
    
    /// \brief A gui.
    ofxPanel guiFinder;
    
    /// \brief The contour threshold.
    ofParameter<float> finderThreshold;
    
    /// \brief True if hue / saturation should be tracked.
    ofParameter<bool> finderTrackHueSaturation;
    
    /// \brief The target color to threshold.
    ofParameter<ofColor> finderTargetColor;
    
    float hullMinumumDefectDepth = 10;
    ofRectangle rect;

    
//--------------- gui
    ofxPanel gui;
    ofParameterGroup guiGroup;
    
    ofParameter<float> opacitySpeedController;
    ofParameter<float> timeScaleController;
    ofParameter<float> ageSpeedController;
    ofParameter<float> smooth;
    ofParameter<float> simplify;
    ofParameter<float> rSpeed;

    
//---------------- Particle System
    std::size_t addParticles = 100;
    std::size_t totalParticles = 2000;
    
    ParticleSystem ps;
    

		
};
