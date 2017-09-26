//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#pragma once


#include "ofTypes.h"
#include "ofUtils.h"



class Particle
{
public:
    Particle();
    Particle(const glm::vec3& position);
    Particle(const glm::vec3& position, const glm::vec3& velocity);

    virtual ~Particle();

    virtual void reset();
    virtual void update();
    virtual void draw() const;

    virtual bool isDead() const;

    bool dead = false;
    
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 gravity;
    glm::vec3 impulse;
    float drag = 1;

    uint64_t age = 0;
    float ageSpeed = 0;
    
    float opcDecreaseSpeed = 0;
    float opacity = 200;
    
    //------------ ofSignedNoise setup ----------//
    float offsetX;
    float offsetY;
    
    glm::vec3 timeOffset;
    
    std::size_t time;
    std::size_t timeScale;
    std::size_t displacementScale;
    
    //CGImage lily;
    
    
};

