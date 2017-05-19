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
#include "bRenderer.h"

class Entity {
public:
    typedef unsigned char OPTIONS;
    
    Entity(int id, Renderer& renderer, std::string& modelName,
           MaterialPtr material,
           OPTIONS options = NO_OPTION, PropertiesPtr properties = nullptr):
    _id(id), _modelName(modelName), _renderer(renderer) {
        if(renderer.getObjects()->getModel(modelName) == nullptr)
            renderer.getObjects()->loadObjModel_o(modelName + ".obj", material, options, properties);
        
        _instanceName = modelName + std::to_string(id);
    }
    
    Entity(int id, Renderer& renderer, std::string& modelName,
           ShaderPtr& shader,
           OPTIONS options = NO_OPTION, PropertiesPtr properties = nullptr):
    _id(id), _modelName(modelName), _renderer(renderer) {
        if(renderer.getObjects()->getModel(modelName) == nullptr)
            renderer.getObjects()->loadObjModel_o(modelName + ".obj", shader, options, properties);
        
        _instanceName = _modelName + std::to_string(id);
    }
    
    Entity(int id, Renderer& renderer, std::string& modelName,
           GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS(),
           OPTIONS options = NO_OPTION, PropertiesPtr properties = nullptr):
    _id(id), _modelName(modelName), _renderer(renderer) {
        if(renderer.getObjects()->getModel(modelName) == nullptr)
            renderer.getObjects()->loadObjModel_o(modelName + ".obj", shaderMaxLights, options, properties);
        
        _instanceName = _modelName + std::to_string(id);
    }
    
    Entity(const Entity&) = default;
    Entity(Entity&&) = default;
    Entity& operator=(Entity&&) = default;
    
    void addComponent(ComponentPtr component) {
        components.insert(std::make_pair((*component).type, component));
        mask |= (*component).type;
    }
    
    template<typename T>
    std::shared_ptr<T> getComponent(COMPONENT_MASK maskType) const {
        return std::dynamic_pointer_cast<T>(components.at(maskType));
    }
    
    bool hasComponents(COMPONENT_MASK type);
    
    int id() const { return _id; }
    std::string modelName() const {
        return _modelName;
    }
    
    std::string instanceName() const {
        return _instanceName;
    }
    
    Renderer& renderer() const {
        return _renderer;
    }
private:
    using ComponentMap = std::map<COMPONENT_MASK, ComponentPtr>;
    int _id;
    std::string _modelName;
    std::string _instanceName;
    Renderer& _renderer;
    
    ComponentMap components;
    COMPONENT_MASK mask = 0;
};

typedef std::shared_ptr<Entity> EntityPtr;

#endif /* Entity_hpp */
