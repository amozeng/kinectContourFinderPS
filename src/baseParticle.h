#pragma once

#include "ofMain.h"
#include "stdio.h"



/// \brief This class defines the basic properties of a simple 3D particle.
class baseParticle
{
public:
    /// \brief The default constructor for the BaseParticle.
    baseParticle();
    
    /// \brief The BaseParticle's draw method.
    virtual ~baseParticle();
    
    /// \brief The BaseParticle's update method.
    virtual void update();
    
    /// \brief The BaseParticle's draw method.
    virtual void draw();
    
    /// \brief This method calculates the age of the particle.
    /// \returns The age of the particle in number of frames.
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

