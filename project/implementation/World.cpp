//
//  World.cpp
//  bRenderer_ios
//
//  Created by Elias on 14.05.17.
//
//

#include "World.h"

Entity& World::createEntity() {
    entities.emplace_back(idManager.next());
    return entities.back();
}

Entity& World::createRenderModel(TransformPtr transform, RenderPtr render) {
    Entity& entity = createEntity();
    
    entity.addComponent(transform);
    entity.addComponent(render);
    
    return entity;
}

void World::removeEntity(int id) {
    std::remove_if(entities.begin(), entities.end(), [id](const Entity& entity) {
        return entity.getId() == id;
    });
    idManager.removeId(id); // Make id accessible again
}

void World::applySystems() {
    std::for_each(systems.begin(), systems.end(), [&](auto& system) {
        system->apply(entities);
    });
}
