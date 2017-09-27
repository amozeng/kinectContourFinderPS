#pragma once

#include "ofTypes.h"
#include "ofUtils.h"
#include "ofMain.h"
#include "stdio.h"



class baseParticle
{
public:
    baseParticle();

    virtual ~baseParticle();
    
    //virtual void reset();
    virtual void update();
    virtual void draw() const;
    
    uint64_t getAge();
    
    
    /// \brief This represents the mass of the particle.
    ///
    /// This is not normally used, but could be usef for more sophisticated
    /// systems.
    float mass;
    
    /// \brief The size of the particle in the x, y and z direction.
    ///
    /// The size of the particle and the position constitute a 3D bounding cube.
    ofPoint size;
    
    /// \brief Acceleration in the x, y and z direction.
    ofPoint position;
    
    /// \brief Velocity in the x, y and z direction.
    ofPoint velocity;
    
    /// \brief Acceleration in the x, y and z direction.
    ofPoint acceleration;
    
    /// \brief Drag in the x, y and z direction.
    ///
    /// Usually the drag is equal in all dimensions.
    ofPoint drag;
    
    /// \brief This is the "bounciness" of the particle.
    ///
    /// When a particle collides with an obstacle, this determines how much of
    /// its velocity is lost in the bounce.
    ofPoint restitution;
    
    /// \brief A default color for the particle.
    ofColor color;
    
    /// \brief A record of the system frame count when the particle was created.
    ///
    /// This can be useful for deciding when a particle is "dead" or can be
    /// mapped to various effects.
    uint64_t createdFrameCount;
    
    
    uint64_t maximumAge;

};


