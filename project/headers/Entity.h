//
//  Entity.hpp
//  bRenderer_ios
//
//  Created by Elias on 14.05.17.
//
//

#ifndef Entity_hpp
#define Entity_hpp

#include "Component.h"

class Entity {
public:
    Entity(int id): id(id) {}
    
    template<typename T, typename ...Args>
    T& addComponent(Args&&... args) {
        T component(args...);
        components.push_back(component);
        mask &= component.type;
        return component;
    }
    
    bool hasComponents(COMPONENT_MASK type);
    
    int getId() const { return id; }
private:
    using ComponentArray = std::vector<Component>;
    int id;
    
    ComponentArray components;
    COMPONENT_MASK mask;
};

#endif /* Entity_hpp */
