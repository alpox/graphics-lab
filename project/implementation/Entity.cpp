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
    CameraPtr camera = _renderer.getObjects()->getCamera(renderer->camera);
    
    vmml::Matrix4f viewMatrix = camera->getViewMatrix();
    vmml::Matrix4f projectionMatrix = camera->getProjectionMatrix();
    
    vmml::Vector3f eyePosition = -camera->getPosition();
    
    render(deltaTime, pass, viewMatrix, projectionMatrix, eyePosition);
}

void Entity::render(const double &deltaTime, PASS pass, vmml::Matrix4f viewMatrix, vmml::Matrix4f projectionMatrix, vmml::Vector3f eyePosition) const {
    RenderPtr renderer = getComponent<Render>(COMPONENT_RENDERER);
    TransformPtr transform = getComponent<Transform>(COMPONENT_TRANSFORM);
    NoDepthPtr noDepth = getComponent<NoDepth>(COMPONENT_NODEPTH);
    
    CameraPtr camera = _renderer.getObjects()->getCamera(renderer->camera);
    ModelPtr model = _renderer.getObjects()->getModel(_modelName);
    
    renderer->totalTime += deltaTime;
    
    if(_shader != nullptr)
        setUniforms(_shader, renderer, transform, deltaTime, pass, viewMatrix, projectionMatrix, eyePosition);
    
    if(noDepth != nullptr)
        glDisable(GL_DEPTH_TEST);
    
    if(!(pass == SECOND_PASS && noDepth != nullptr)) { // Don't draw objects without depth buffer in second pass
        _renderer.getObjects()->setAmbientColor(renderer->ambientColor);
        _renderer.getModelRenderer()->drawModel(model, transform->modelMatrix, viewMatrix, projectionMatrix, renderer->lightNames, false);
        _renderer.getObjects()->setAmbientColor(bRenderer::DEFAULT_AMBIENT_COLOR());
    }
    
    if(noDepth != nullptr)
        glEnable(GL_DEPTH_TEST);
}

void Entity::setUniforms(ShaderPtr shader, RenderPtr render, TransformPtr transform, const double &deltaTime, PASS pass,
                         vmml::Matrix4f viewMatrix, vmml::Matrix4f projectionMatrix, vmml::Vector3f eyePosition) const {
    ObjectManagerPtr objectManager = renderer().getObjects();
    
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
    
    shader->setUniform("eyePosition", eyePosition);
    
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
    
    EffectList effects = _effects.getEffects();
    
    shader->setUniform("blueShift", 0);
    shader->setUniform("celShader", 0);
    
    std::for_each(effects.begin(), effects.end(), [&shader,&deltaTime](std::pair<double, EFFECT> &effect) {
        switch(effect.second) {
            case EFFECT_BLUEVISION:
                shader->setUniform("blueShift", 1);
            case EFFECT_CEL:
                shader->setUniform("celShader", 1);
        }
    });
}
