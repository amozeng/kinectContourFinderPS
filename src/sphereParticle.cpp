#include "sphereParticle.h"

sphereParticle::sphereParticle(){}

void sphereParticle::setup() {
    bool bLoaded = lilyInSphereParticle.load("lily.png");
    cout << bLoaded << endl; //false if not in testApp.cpp
    lilyInSphereParticle.load("lily.png");
}


void sphereParticle::draw(){
    
    ofFill();
    float normalizedAge = ofMap(getAge(), 0, maximumAge, 1, 0);
    
    
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

void cubeParticle::draw() {
    ofFill();
    
    float normalizedAge = ofMap(getAge(), 0, maximumAge, 1, 0);
    
    ofSetColor(255, 255, 0, 30);
    
    ofPushMatrix();
    ofTranslate(position);
    
    // float angle =
    // ofQuaternion quat;
    
    // quat.makeRotate(velocity, ofPoint::zero());//position);
    
    float angle = position.angle(velocity);
    
    ofPoint direction = position - velocity;
    
    // position.getAngle(
    
    // quat.getRotate(angle, direction);
    
    ofRotate(angle, direction.x, direction.y, direction.z);
    
    //ofDrawCone(0, 0, 0, 40 * normalizedAge, 60 * normalizedAge);
    
    float randomW = ofRandom(3,6);
    float randomH = ofRandom(10,12);
    
    
    //    ofDrawCone(0, 0, 0, 40 * normalizedAge, 60 * normalizedAge);
    ofDrawPlane(0, 0, 0, randomW * normalizedAge, randomH * normalizedAge);
    
    
    ofPopMatrix();
    
}
