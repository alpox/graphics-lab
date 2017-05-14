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

#include "IdManager.h"

// Maybe we use a registry later
const unsigned short COMPONENT_NONE = 1;

typedef unsigned short COMPONENT_MASK;

class Component {
public:
private:
    virtual void setType() {
        type = COMPONENT_NONE;
    }
    
    COMPONENT_MASK type;
};



/*
namespace component {
    typedef Component& (*CreateComponentFunc)();
    typedef std::map<std::string, CreateComponentFunc> ComponentRegistry;
    
    ComponentRegistry& getComponentRegistry() {
        static ComponentRegistry reg;
        return reg;
    }
    
    template<typename T>
    Component createComponent() {
        return T();
    }
    
    
}*/

#endif /* Component_hpp */
