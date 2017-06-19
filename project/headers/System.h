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

#include "Entity.h"

class World;
class System {
public:
    System(World& world): world(world) {}
    
    void apply(std::vector<EntityPtr> entities, const double &deltaTime);
protected:
    virtual void apply(EntityPtr entity, const double &deltaTime) = 0;
    
    COMPONENT_MASK requirements_mask;
    
    World& world;
};

#endif /* System_hpp */
