//
//  World.cpp
//  bRenderer_ios
//
//  Created by Elias on 14.05.17.
//
//

#include "World.h"

void World::removeEntity(int id) {
    std::remove_if(entities.begin(), entities.end(), [id](const auto& entity) {
        return entity->id() == id;
    });
    idManager.removeId(id); // Make id accessible again
}

void World::applySystems(const double &deltaTime) const {
    std::for_each(systems.begin(), systems.end(), [&](const auto& system) {
        system->apply(entities, deltaTime);
    });
}

void World::render(const double &deltaTime) const {
    // Apply all systems before rendering
    applySystems(deltaTime);
    
    // First pass
    std::for_each(entities.begin(), entities.end(), [&deltaTime](const auto& entity) {
        entity->render(deltaTime, FIRST_PASS);
    });
    
    // Second pass
    std::for_each(entities.begin(), entities.end(), [&deltaTime](const auto& entity) {
        entity->render(deltaTime, SECOND_PASS);
    });
    
    // Third pass
    /*std::for_each(entities.begin(), entities.end(), [&deltaTime](const auto& entity) {
        entity->render(deltaTime, THIRD_PASS);
    });*/
}
