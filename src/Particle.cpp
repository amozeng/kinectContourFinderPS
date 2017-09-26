//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#include "Particle.h"
#include "ofGraphics.h"


Particle::Particle()
{
    offsetX = ofRandom(0,1000);
    offsetY = ofRandom(0,1000);    
    
}


Particle::Particle(const glm::vec3& _position): position(_position)
{
}


Particle::Particle(const glm::vec3& _position, const glm::vec3& _velocity):
    position(_position),
    velocity(_velocity)
{
}


Particle::~Particle()
{
}

void Particle::reset(){
    
}


void Particle::update()
{
    age = age + ageSpeed;
    velocity += (gravity + impulse);
    impulse *= 0.0;
    velocity *= drag;
    position += velocity;
    opacity = opacity - opcDecreaseSpeed;
    
    time = ofGetElapsedTimef();
    timeScale = 5.0;
    displacementScale = 0.75;
    timeOffset.x = offsetX;
    timeOffset.y = offsetY;

    position.x += (ofSignedNoise(time*timeScale+timeOffset.x)) * displacementScale;
    position.y += (ofSignedNoise(time*timeScale+timeOffset.y)) * displacementScale;
    
    //age = ofMap(opacity, 0, 255, 100, 0, true);

}


void Particle::draw() const
{
    ofSetColor(255, opacity);
    ofDrawCircle(position, 0.1);
    
}


bool Particle::isDead() const
{
    return age > 100 || opacity <= 0;
}
