//
//  Component.hpp
//  bRenderer_ios
//
//  Created by Elias on 14.05.17.
//
//

#ifndef Component_hpp
#define Component_hpp

#include <map>
#include <string>
#include <memory>

#include "IdManager.h"

typedef unsigned short COMPONENT_MASK;

// Maybe we use a registry later
const COMPONENT_MASK COMPONENT_NONE = 1;
const COMPONENT_MASK COMPONENT_TRANSFORM = 1 << 1;
const COMPONENT_MASK COMPONENT_RENDERER = 1 << 2;
const COMPONENT_MASK COMPONENT_STREAM = 1 << 3;

class Component {
public:
    virtual ~Component() {}
    
    COMPONENT_MASK type;
};

typedef std::shared_ptr<Component> ComponentPtr;

#endif /* Component_hpp */
