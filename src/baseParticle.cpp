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
    
    maximumAge = ofRandom(60*0.5, 60*1);
}



/// \brief The BaseParticle's draw method.
baseParticle::~baseParticle()
{
}



/// \brief The BaseParticle's update method.
void baseParticle::update()
{
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

/// \brief The BaseParticle's draw method.
void baseParticle::draw() const
{
    ofFill();
//    ofSetColor(color);
//    
//    // Sphere radius ...
//    float sphereRadius = (size.x + size.y + size.z) / 3.0 / 2.0;
//    
//    ofDrawCone(position, sphereRadius, sphereRadius);
    float normalizedAge = ofMap(ofGetFrameNum() - createdFrameCount, 0, maximumAge, 1, 0);

    ofSetColor(255, 153, 204, 30);
    
    ofPushMatrix();
    
    ofTranslate(position);
    float randomW = ofRandom(3,6);
    float randomH = ofRandom(10,12);
    float angle = position.angle(velocity);
    ofPoint direction = position - velocity;
    ofRotate(angle, direction.x, direction.y, direction.z);
    
    ofDrawPlane(0, 0, 0, randomW * normalizedAge, randomH * normalizedAge);
    //ofDrawSphere(position, 10 * normalizedAge);
    //lilyInSphereParticle.draw(position.x, position.y);
    ofPopMatrix();

}

/// \brief This method calculates the age of the particle.
/// \returns The age of the particle in number of frames.
uint64_t baseParticle::getAge()
{
    return ofGetFrameNum() - createdFrameCount;
}
    


