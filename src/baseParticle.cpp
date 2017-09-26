
#include "baseParticle.h"

baseParticle::baseParticle()
{
    mass = 1;
    position = ofPoint();
    velocity = ofPoint();
    acceleration = ofPoint();
    drag = ofPoint(0.99);
    restitution = ofPoint(0.90);
    color = ofColor(127);;
    createdFrameCount = ofGetFrameNum();
    
    maximumAge = ofRandom(60*2, 60*5);
}

/// \brief The BaseParticle's draw method.
baseParticle::~baseParticle()
{
}

void baseParticle::update() {
    // Add the acceleration to the velocity.
    // Acceleration is the change in velocity over time.
    velocity += acceleration;
    
    // Slow the velocity down by the drag term.
    velocity *= drag;
    
    // Add the velocity to the position.
    // Velocity is the change in position over time.
    position += velocity;
    
    // We reset out our acceleration each time we add it.
    acceleration.set(0, 0, 0);
}

void baseParticle::draw() {
    ofFill();
    ofSetColor(color);
    
    // Sphere radius ...
    float sphereRadius = (size.x + size.y + size.z) / 3.0 / 2.0;
    
    ofDrawSphere(position, sphereRadius);
}

uint64_t baseParticle::getAge()
{
    return ofGetFrameNum() - createdFrameCount;
}
