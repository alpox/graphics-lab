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
#include "Stream.h"
#include "RenderSystem.h"
#include "StreamSystem.h"

class World {
public:
    using EntityArray = std::vector<EntityPtr>;
    using SystemArray = std::vector<std::unique_ptr<System>>;
    
    World(Renderer& renderer): renderer(renderer) {}
    
    template<typename... Args>
    EntityPtr createEntity(Args&&...args) {
        auto entity = EntityPtr(new Entity(idManager.next(), args...));
        entities.push_back(entity);
        return entities.back();
    }
    
    template<typename ...Args>
    EntityPtr createRenderModel(std::string modelName, TransformPtr transform, RenderPtr render, Args&&... args) {
        EntityPtr entity = createEntity(renderer, modelName, args...);
        
        entity->addComponent(transform);
        entity->addComponent(render);
        
        return entity;
    }
    
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
    Renderer& renderer;
    
    EntityArray entities;
    SystemArray systems;
    
    IdManager idManager;
};

#endif /* World_hpp */
