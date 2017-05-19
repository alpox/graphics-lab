//
//  System.cpp
//  bRenderer_ios
//
//  Created by Elias on 14.05.17.
//
//

#include "System.h"


void System::apply(std::vector<EntityPtr> entities) const {
    
    // Apply the system to all entities
    std::for_each(entities.begin(), entities.end(), [&](EntityPtr entity) {
        if(entity->hasComponents(requirements_mask))
            apply(entity);
    });
}
