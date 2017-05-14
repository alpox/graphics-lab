//
//  World.cpp
//  bRenderer_ios
//
//  Created by Elias on 14.05.17.
//
//

#include "World.h"

template<typename ...Args>
Entity& World::createEntity(Args &&... args) {
    entities.emplace_back(idManager.next(), args...);
    return entities.back();
}

void World::removeEntity(int id) {
    std::remove_if(entities.begin(), entities.end(), [id](const Entity& entity) {
        return entity.getId() == id;
    });
    idManager.removeId(id); // Make id accessible again
}
