//
//  Entity.cpp
//  bRenderer_ios
//
//  Created by Elias on 14.05.17.
//
//

#include "Entity.h"

bool Entity::hasComponents(COMPONENT_MASK type) {
    return (mask & type) == type;
}
