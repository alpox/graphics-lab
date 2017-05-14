//
//  World.hpp
//  bRenderer_ios
//
//  Created by Elias on 14.05.17.
//
//

#ifndef World_hpp
#define World_hpp

#include <vector>

#include "Entity.h"

class System;

class World {
public:
    using EntityArray = std::vector<Entity>;
    using SystemArray = std::vector<System>;
    
    World() = default;
    World(const World& world) = delete;
    World(World&& world) = delete;
    
    template<typename ...Args>
    Entity& createEntity(Args&&... args);
    
    void removeEntity(int id);
    
    EntityArray& getEntities() {
        return entities;
    }
private:
    
    EntityArray entities;
    SystemArray systems;
    
    IdManager idManager;
};

#endif /* World_hpp */
