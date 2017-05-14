//
//  System.cpp
//  bRenderer_ios
//
//  Created by Elias on 14.05.17.
//
//

#include "System.h"


void System::apply() {
    auto entities = world.getEntities();
    
    // Apply the system to all entities
    std::for_each(entities.begin(), entities.end(), [&](Entity& entity) {
        apply(entity);
    });
}
