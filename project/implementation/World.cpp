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

void World::applySystems() {
    std::for_each(systems.begin(), systems.end(), [&](const auto& system) {
        system->apply(entities);
    });
}
