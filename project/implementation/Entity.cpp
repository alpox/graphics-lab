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

void Entity::render(const double &deltaTime, PASS pass) const {
    RenderPtr renderer = getComponent<Render>(COMPONENT_RENDERER);
    TransformPtr transform = getComponent<Transform>(COMPONENT_TRANSFORM);
    NoDepthPtr noDepth = getComponent<NoDepth>(COMPONENT_NODEPTH);
    
    if(_shader != nullptr)
        setUniforms(_shader, renderer, transform, deltaTime, pass);
    
    if(modelName() == "cube") {
        transform->modelMatrix = vmml::create_translation(-_renderer.getObjects()->getCamera(renderer->camera)->getPosition() +
                                                          vmml::Vector3f::BACKWARD * 20.f
                                                          ) * vmml::create_scaling(vmml::Vector3f(0.5f));
    }
    
    if(noDepth != nullptr)
        glDisable(GL_DEPTH_TEST);
    
    if(!(pass == SECOND_PASS && noDepth != nullptr)) { // Don't draw objects without depth buffer in second pass
        _renderer.getObjects()->setAmbientColor(renderer->ambientColor);
        _renderer.getModelRenderer()->drawModel(_modelName, renderer->camera, transform->modelMatrix, renderer->lightNames);
        _renderer.getObjects()->setAmbientColor(bRenderer::DEFAULT_AMBIENT_COLOR());
    }
    
    if(noDepth != nullptr)
        glEnable(GL_DEPTH_TEST);
}

void Entity::setUniforms(ShaderPtr shader, RenderPtr render, TransformPtr transform, const double &deltaTime, PASS pass) const {
    ObjectManagerPtr objectManager = renderer().getObjects();
    
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
    
    shader->setUniform("numPass", (GLint)pass);
}
