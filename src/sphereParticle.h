#pragma once

#include "stdio.h"
#include "ofMain.h"
#include "baseParticle.h"


class sphereParticle: public baseParticle {
public:
    sphereParticle();
    
    virtual void setup();
    virtual void draw();
    ofImage lilyInSphereParticle;
    


    
};


class cubeParticle: public baseParticle {
public:
    virtual void draw();
        
};




