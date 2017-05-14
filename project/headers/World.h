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
#include <algorithm>

#include "Entity.h"
#include "System.h"

#include "bRenderer.h"
#include "Transform.h"
#include "Render.h"
#include "RenderSystem.h"

class World {
public:
    using EntityArray = std::vector<Entity>;
    using SystemArray = std::vector<std::unique_ptr<System>>;
    
    World() = default;
    World(const World& world) = delete;
    World(World&& world) = delete;
    
    Entity& createEntity();
    
    void removeEntity(int id);
    
    template<typename T>
    void addSystem() {
        systems.emplace_back(new T());
    }
    
    EntityArray& getEntities() {
        return entities;
    }
    
    void applySystems();
private:
    
    EntityArray entities;
    SystemArray systems;
    
    IdManager idManager;
};

#endif /* World_hpp */
