//
//  System.hpp
//  bRenderer_ios
//
//  Created by Elias on 14.05.17.
//
//

#ifndef System_hpp
#define System_hpp

#include <vector>
#include <algorithm>

#include "World.h"

class World;

class System {
public:
    System(World& world): world(world) {}
    
    void apply();
private:
    virtual void apply(Entity& entity);
    
    virtual void setRequirements();
    
    World& world;
    COMPONENT_MASK component_mask;
};

#endif /* System_hpp */
