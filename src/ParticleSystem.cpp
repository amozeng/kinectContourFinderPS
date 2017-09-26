//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:    MIT
//


#include "ParticleSystem.h"


ParticleSystem::ParticleSystem()
{
}


ParticleSystem::~ParticleSystem()
{
}


void ParticleSystem::update()
{
//    for (std::size_t i = 0; i < particles.size(); ++i)
//    {
//        if (particles[i]->isDead())
//        {
//            // ???
//            particles.erase(particles.begin() + i);
//        }
//    }
//    

    std::vector<std::unique_ptr<Particle>>::iterator iter = particles.begin();

    while (iter != particles.end())
    {
        (*iter)->update();

        if ((*iter)->isDead())
        {
            iter = particles.erase(iter);
        }
        else
        {
            iter = iter + 1;
        }

//        if (particle->isDead())
//        {
//            particles.erase(iter);
//        }
//        
//        
//        
//        iter = iter + 1;
// 
        
    
    
    }
}


void ParticleSystem::draw() const
{
    for (const auto& particle: particles)
    {
        particle->draw();
    }
}
