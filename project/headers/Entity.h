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
#include "Render.h"
#include "Transform.h"
#include "NoDepth.h"
#include "EffectsBag.h"

class Entity {
public:
    typedef unsigned char OPTIONS;
    
    Entity(int id, EffectsBag& effects, Renderer& renderer, std::string& modelName,
           MaterialPtr material,
           OPTIONS options = NO_OPTION, PropertiesPtr properties = nullptr):
    _id(id), _effects(effects), _modelName(modelName), _renderer(renderer) {
        if(renderer.getObjects()->getModel(modelName) == nullptr)
            renderer.getObjects()->loadObjModel_o(modelName + ".obj", material, options, properties);
        
        _instanceName = modelName + std::to_string(id);
    }
    
    Entity(int id, EffectsBag& effects, Renderer& renderer, std::string& modelName,
           ShaderPtr& shader,
           OPTIONS options = NO_OPTION, PropertiesPtr properties = nullptr):
    _id(id), _effects(effects), _modelName(modelName), _renderer(renderer) {
        if(renderer.getObjects()->getModel(modelName) == nullptr)
            renderer.getObjects()->loadObjModel_o(modelName + ".obj", shader, options, properties);
        
        _instanceName = _modelName + std::to_string(id);
        _shader = shader;
    }
    
    Entity(int id, EffectsBag& effects, Renderer& renderer, std::string& modelName,
           GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS(),
           OPTIONS options = NO_OPTION, PropertiesPtr properties = nullptr):
    _id(id), _effects(effects), _modelName(modelName), _renderer(renderer) {
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
        auto found = components.find(maskType);
        if(found == components.end())
            return nullptr;
        return std::dynamic_pointer_cast<T>(found->second);
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
    
    ShaderPtr shader() const {
        return _shader;
    }
    
    ModelPtr model() const {
        return _renderer.getObjects()->getModel(_modelName);
    }
    
    void render(const double &deltaTime, PASS pass) const;
    
    void render(const double &deltaTime, PASS pass, vmml::Matrix4f viewMatrix, vmml::Matrix4f projectionMatrix, vmml::Vector3f eyePosition) const;
    
    void setUniforms(ShaderPtr shader, RenderPtr render, TransformPtr transform, const double &deltaTime, PASS pass,
                     vmml::Matrix4f viewMatrix, vmml::Matrix4f projectionMatrix, vmml::Vector3f eyePosition) const;
    
    EffectsBag& effects() {
        return _effects;
    }
private:
    using ComponentMap = std::map<COMPONENT_MASK, ComponentPtr>;
    int _id;
    std::string _modelName;
    std::string _instanceName;
    Renderer& _renderer;
    EffectsBag& _effects;
    ShaderPtr _shader;
    
    ComponentMap components;
    COMPONENT_MASK mask = 0;
};

typedef std::shared_ptr<Entity> EntityPtr;

#endif /* Entity_hpp */
