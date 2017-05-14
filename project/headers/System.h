//
//  System.hpp
//  bRenderer_ios
//
//  Created by Elias on 14.05.17.
//
//

#ifndef System_hpp
#define System_hpp

#include <vector>
#include <algorithm>

#include "Entity.h"

class System {
public:
    void apply(std::vector<Entity> entities) const;
protected:
    virtual void apply(Entity& entity) const = 0;
    
    COMPONENT_MASK requirements_mask;
};

#endif /* System_hpp */
