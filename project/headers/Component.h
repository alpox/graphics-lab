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

#include "constants.h"
#include "IdManager.h"

class Component {
public:
    virtual ~Component() {}
    
    COMPONENT_MASK type;
};

typedef std::shared_ptr<Component> ComponentPtr;

#endif /* Component_hpp */
