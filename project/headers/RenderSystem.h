//
//  Render.h
//  bRenderer_ios
//
//  Created by Elias on 14.05.17.
//
//

#ifndef Render_h
#define Render_h

#include "Entity.h"
#include "Render.h"

class RenderSystem: public virtual System {
public:
    RenderSystem() {
        requirements_mask = (COMPONENT_RENDERER | COMPONENT_TRANSFORM);
    }
    
protected:
    void apply(EntityPtr entity, const double &deltaTime) const {
        RenderPtr renderer = entity->getComponent<Render>(COMPONENT_RENDERER);
        TransformPtr transform = entity->getComponent<Transform>(COMPONENT_TRANSFORM);
        
        ShaderPtr shader = entity->shader();
        
        // small hack
        if(entity->modelName() == "cube") {
            transform->modelMatrix = vmml::create_translation(vmml::Vector3f(entity->renderer().getObjects()->getLight("torchLight")->getPosition()) +
                                                              entity->renderer().getObjects()->getCamera("camera")->getForward()*20.f
                                                              ) * vmml::create_scaling(vmml::Vector3f(3.f));
        }
        
        if(shader != nullptr)
            setUniforms(entity, shader, renderer, transform, deltaTime);
        
        entity->renderer().getObjects()->setAmbientColor(renderer->ambientColor);
        entity->renderer().getModelRenderer()->drawModel(entity->modelName(),
                                                renderer->camera, transform->modelMatrix, renderer->lightNames);
        entity->renderer().getObjects()->setAmbientColor(bRenderer::DEFAULT_AMBIENT_COLOR());
    }
    
    void setUniforms(EntityPtr entity, ShaderPtr shader, RenderPtr render, TransformPtr transform, const double &deltaTime) const {
        
        ObjectManagerPtr objectManager = entity->renderer().getObjects();
        
        vmml::Matrix4f viewMatrix = objectManager->getCamera(render->camera)->getViewMatrix();
        vmml::Matrix4f projectionMatrix = objectManager->getCamera(render->camera)->getProjectionMatrix();
        
        vmml::Matrix3f normalMatrix;
        vmml::compute_inverse(vmml::transpose(vmml::Matrix3f(transform->modelMatrix)), normalMatrix);
        shader->setUniform("normalMatrix", normalMatrix);
        
        // Calculate and set inverse model matrix
        vmml::Matrix4f inverseModelMatrix;
        vmml::compute_inverse(transform->modelMatrix, inverseModelMatrix);
        shader->setUniform("inverseModel", inverseModelMatrix);
        
        // Calculate and set inverse view matrix
        vmml::Matrix4f inverseViewMatrix;
        vmml::compute_inverse(viewMatrix, inverseViewMatrix);
        shader->setUniform("inverseView", inverseViewMatrix);
        
        // Calculate and set inverse view matrix
        vmml::Matrix4f inverseProjectionMatrix;
        vmml::compute_inverse(projectionMatrix, inverseProjectionMatrix);
        shader->setUniform("inverseProjection", inverseProjectionMatrix);
        
        // Set all model/view/projection matrizes
        shader->setUniform("model", transform->modelMatrix);
        shader->setUniform("view", viewMatrix);
        shader->setUniform("projection", projectionMatrix);
        
        shader->setUniform("eyePosition", -objectManager->getCamera(render->camera)->getPosition());
        
        shader->setUniform("ambient", render->ambientColor);
        
        for(int i = 0; i < render->lightNames.size(); i++) {
            LightPtr light = objectManager->getLight(render->lightNames.at(i));
            std::string lightName = "lights[" + std::to_string(i) + "]";
            
            shader->setUniform(lightName + ".position", light->getPosition());
            shader->setUniform(lightName + ".diffuse", light->getDiffuseColor());
            shader->setUniform(lightName + ".specular", light->getSpecularColor());
            shader->setUniform(lightName + ".intensity", light->getIntensity());
            shader->setUniform(lightName + ".attenuation", light->getAttenuation());
            shader->setUniform(lightName + ".radius", light->getRadius());
        }
        
        shader->setUniform("numLights", (GLint)render->lightNames.size());
    }
};

#endif /* Render_h */
