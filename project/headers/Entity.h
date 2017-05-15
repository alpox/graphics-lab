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
    void addComponent(ComponentPtr component) {
        components.insert(std::make_pair((*component).type, component));
        mask |= (*component).type;
    }
    
    template<typename T>
    std::shared_ptr<T> getComponent(COMPONENT_MASK maskType) const {
        return std::dynamic_pointer_cast<T>(components.at(maskType));
    }
    
    bool hasComponents(COMPONENT_MASK type);
    
    int getId() const { return id; }
private:
    using ComponentMap = std::map<COMPONENT_MASK, std::shared_ptr<Component>>;
    int id;
    
    ComponentMap components;
    COMPONENT_MASK mask;
};

#endif /* Entity_hpp */
