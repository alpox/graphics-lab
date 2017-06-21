//
//  World.cpp
//  bRenderer_ios
//
//  Created by Elias on 14.05.17.
//
//

#include "World.h"
#include <vmmlib/frustum.hpp>


vmml::AABBf transformBoundingBox(vmml::AABBf boundingBox, vmml::Matrix4f m) {
    vmml::Matrix3f dimensions(m);
    
    vmml::Vector3f xa = dimensions.get_row(0) * boundingBox.getMin().x();
    vmml::Vector3f xb = dimensions.get_row(0) * boundingBox.getMax().x();
    
    vmml::Vector3f ya = dimensions.get_row(1) * boundingBox.getMin().y();
    vmml::Vector3f yb = dimensions.get_row(1) * boundingBox.getMax().y();
    
    vmml::Vector3f za = dimensions.get_row(2) * boundingBox.getMin().z();
    vmml::Vector3f zb = dimensions.get_row(2) * boundingBox.getMax().z();
    
    vmml::Vector3f translation(m.get_column(3));
    
    return vmml::AABBf(
                       min(xa, xb) + min(ya, yb) + min(za, zb) + translation,
                       max(xa, xb) + max(ya, yb) + max(za, zb) + translation
                       );
}

vmml::Vector3f min(vmml::Vector3f vec1, vmml::Vector3f vec2) {
    return vmml::Vector3f(
                          std::min(vec1.x(), vec2.x()),
                          std::min(vec1.y(), vec2.y()),
                          std::min(vec1.z(), vec2.z())
                          );
}

vmml::Vector3f max(vmml::Vector3f vec1, vmml::Vector3f vec2) {
    return vmml::Vector3f(
                          std::max(vec1.x(), vec2.x()),
                          std::max(vec1.y(), vec2.y()),
                          std::max(vec1.z(), vec2.z())
                          );
}

vmml::Vector3f getSunPosition() {
    return vmml::normalize(vmml::Vector3f(0.f, 0.f, 1.f)) * 1000.f;
}

vmml::AABBf World::getFullBoundingBox() {
    vmml::AABBf &boundingBox = _fullBoundingBox;
    if(boundingBox != vmml::AABBf()) return boundingBox;
    
    for(EntityArray::iterator it = entities.begin(); it != entities.end(); it++) {
        EntityPtr entity = *it;
        ModelPtr model = entity->model();
        vmml::AABBf aabb = model->getBoundingBoxObjectSpace();
        
        auto entityTransform = entity->getComponent<Transform>(COMPONENT_TRANSFORM);
        
        if(entityTransform == nullptr) continue;
        
        boundingBox.merge(transformBoundingBox(aabb, entityTransform->modelMatrix));
    }
    
    return boundingBox;
}

vmml::Matrix4f World::getSunViewMatrix() {
    CameraPtr camera = renderer.getObjects()->getCamera("camera");
    vmml::Vector3f sunPosition = getSunPosition() / 1000.f;
    return camera->lookAt(sunPosition, vmml::Vector3f(0.0, 0.0, 0.0), vmml::Vector3f::UP);
}

vmml::Matrix4f World::getSunProjectionMatrix() {
    vmml::Matrix4f sunViewMatrix = getSunViewMatrix();
    vmml::AABBf fullBoundingBox = getFullBoundingBox();
    
    vmml::AABBf boundingBoxLightDirection = transformBoundingBox(fullBoundingBox, sunViewMatrix);
    
    vmml::Frustumf orthoFrustum(boundingBoxLightDirection.getMin().x(), // left
                                boundingBoxLightDirection.getMax().x(), // right
                                boundingBoxLightDirection.getMin().y(), // bottom
                                boundingBoxLightDirection.getMax().y(), // top
                                boundingBoxLightDirection.getMin().z(), // near
                                boundingBoxLightDirection.getMax().z()); // far
    
    return orthoFrustum.compute_ortho_matrix();
}

DepthMapPtr World::renderSunShadowMap(const double &deltaTime) {
    FramebufferPtr framebuffer;
    if((framebuffer = renderer.getObjects()->getFramebuffer("shadowMapBuffer")) == nullptr)
        framebuffer = renderer.getObjects()->createFramebuffer("shadowMapBuffer");
    
    // Store default fbo to reuse it later
    GLint defaultFBO = Framebuffer::getCurrentFramebuffer();
    
    // create texture to bind to the fbo
    DepthMapPtr fboTexture;
    if((fboTexture = renderer.getObjects()->getDepthMap("shadowMap")) == nullptr)
        fboTexture = renderer.getObjects()->createDepthMap("shadowMap",
                                                           renderer.getView()->getViewportWidth(), renderer.getView()->getViewportHeight());
    
    framebuffer->bindDepthMap(fboTexture, true);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    for(EntityArray::iterator it = entities.begin(); it != entities.end(); it++) {
        EntityPtr entity = *it;
        
        entity->render(deltaTime, LIGHT_PASS, getSunViewMatrix(), getSunProjectionMatrix(), getSunPosition());
    }
    
    framebuffer->unbind(defaultFBO); // Restore default fbo
    
    return fboTexture;
}

void World::renderSun() {
    
    ShaderPtr sunShader = renderer.getObjects()->loadShaderFile("colorShader", 0, false, false, false, false, false);
    CameraPtr camera = renderer.getObjects()->getCamera("camera");
    
    // Load model if necessary
    if(renderer.getObjects()->getModel("sphere") == nullptr)
        renderer.getObjects()->loadObjModel_o("sphere.obj", sunShader);
    
    // Set camera position to 0
    vmml::Vector3f cameraPosition = camera->getPosition();
    camera->setPosition({0.0,0.0,0.0});
    
    // Orient model
    vmml::Matrix4f modelMatrix = vmml::create_translation(getSunPosition()) *
        vmml::create_scaling(vmml::Vector3f(60.f));
    
    /*vmml::Matrix4f modelMatrix = vmml::create_translation(vmml::Vector3f::BACKWARD * 1000.f) *
        vmml::create_scaling(vmml::Vector3f(30.f));*/

    
    // Set all model/view/projection matrizes
    sunShader->setUniform("model", modelMatrix);
    sunShader->setUniform("view", camera->getViewMatrix());
    sunShader->setUniform("projection", camera->getProjectionMatrix());
    
    glDisable(GL_DEPTH_TEST);
    
    renderer.getModelRenderer()->drawModel("sphere", "camera", modelMatrix, std::vector<std::string>({}));
    
    glEnable(GL_DEPTH_TEST);
    
    
    // Reset camera position
    camera->setPosition(cameraPosition);
}

void World::renderSkyCube() {
    
    ObjectManagerPtr objectManager = renderer.getObjects();
    
    CameraPtr camera = objectManager->getCamera("camera");
    
    // Load model if necessary
    CubeMapPtr skyBox;
    ShaderPtr skyShader = objectManager->loadShaderFile("skyBox", 0, false, false, false, false, false);
    
    // Load cubemap texture
    if((skyBox = objectManager->getCubeMap("skyCube")) == nullptr) {
        TextureData skyLeft("WhiteCloudLeft.jpg");
        TextureData skyRight("WhiteCloudRight.jpg");
        TextureData skyUp("WhiteCloudUp.jpg");
        TextureData skyFront("WhiteCloudFront.jpg");
        TextureData skyBack("WhiteCloudBack.jpg");
        TextureData skyDown("WhiteCloudDown.jpg");
        
        skyBox = objectManager->createCubeMap("skyCube", { skyLeft, skyRight, skyUp, skyDown, skyFront, skyBack});
    }
    
    if(renderer.getObjects()->getModel("cube") == nullptr)
        renderer.getObjects()->loadObjModel_o("cube.obj", skyShader, FLIP_T);
    
    
    // Set camera position to 0
    vmml::Vector3f cameraPosition = camera->getPosition();
    camera->setPosition({0.0,0.0,0.0});
    
    // Orient model
    vmml::Matrix4f modelMatrix = vmml::create_scaling(vmml::Vector3f(1000.f));
    
    // Set all model/view/projection matrizes
    skyShader->setUniform("skyCube", skyBox);
    skyShader->setUniform("model", modelMatrix);
    skyShader->setUniform("view", camera->getViewMatrix());
    skyShader->setUniform("projection", camera->getProjectionMatrix());
    
    glDisable(GL_DEPTH_TEST);
    
    renderer.getModelRenderer()->drawModel("cube", "camera", modelMatrix, std::vector<std::string>({}));
    
    glEnable(GL_DEPTH_TEST);
    
    
    // Reset camera position
    camera->setPosition(cameraPosition);
}

void World::removeEntity(int id) {
    entities.erase(std::remove_if(entities.begin(), entities.end(), [id](const auto& entity) {
        return entity->id() == id;
    }));
    idManager.removeId(id); // Make id accessible again
}

void World::applySystems(const double &deltaTime) {
    effectsBag.testEffects(deltaTime);
    
    std::for_each(systems.begin(), systems.end(), [&](const auto& system) {
        system->apply(entities, deltaTime);
    });
}

void World::render(const double &deltaTime) {
    glCullFace(GL_FRONT_AND_BACK);
    
    // Apply all systems before rendering
    applySystems(deltaTime);
    
    renderSkyCube();
    
    DepthMapPtr shadowMap = renderSunShadowMap(deltaTime);
    
    FramebufferPtr framebuffer;
    if((framebuffer = renderer.getObjects()->getFramebuffer("firstPass")) == nullptr)
        framebuffer = renderer.getObjects()->createFramebuffer("firstPass");
    
    // Store default fbo to reuse it later
    GLint defaultFBO = Framebuffer::getCurrentFramebuffer();
    
    // create texture to bind to the fbo
    TexturePtr fboTexture;
    if((fboTexture = renderer.getObjects()->getTexture("fbo_godray_texture")) == nullptr)
        fboTexture = renderer.getObjects()->createTexture("fbo_godray_texture",
                                                          renderer.getView()->getWidth(), renderer.getView()->getHeight());
    glClear(GL_DEPTH_BUFFER_BIT);
    framebuffer->bindTexture(fboTexture, true);
    
    // First pass
    renderSun();
    
    for(EntityArray::iterator it = entities.begin(); it != entities.end(); it++) {
        EntityPtr entity = *it;
        entity->render(deltaTime, FIRST_PASS);
    }
    
    framebuffer->unbind(defaultFBO); // Restore default fbo
    
    // Second pass
    glClear(GL_DEPTH_BUFFER_BIT);
    
    renderer.getObjects()->getShader("scene1")->setUniform("ShadowMap", shadowMap);
    renderer.getObjects()->getShader("scene1")->setUniform("sunSpaceView", getSunViewMatrix());
    renderer.getObjects()->getShader("scene1")->setUniform("sunSpaceProjection", getSunProjectionMatrix());
    
    for(EntityArray::iterator it = entities.begin(); it != entities.end(); it++) {
        EntityPtr entity = *it;
        entity->render(deltaTime, SECOND_PASS);
    };
    
    glClear(GL_DEPTH_BUFFER_BIT);
    
    // Third pass
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    // Create godray shader
    ShaderPtr godrayShader;
    if((godrayShader = renderer.getObjects()->getShader("godrays")) == nullptr)
        godrayShader = renderer.getObjects()->loadShaderFile("godrays", 0, false, false, false, false, false);
    
    godrayShader->setUniform("firstPass", fboTexture);
    godrayShader->setUniform("exposure", (GLfloat)1.0);
    godrayShader->setUniform("decay", (GLfloat)1.0);
    godrayShader->setUniform("density", (GLfloat)1.1);
    godrayShader->setUniform("weight", (GLfloat)0.007);
    
    CameraPtr camera = renderer.getObjects()->getCamera("camera");
    vmml::Matrix4f view = camera->getViewMatrix();
    vmml::Matrix4f projection = camera->getProjectionMatrix();
    
    // Transform sun position to screen position
    vmml::Vector4f sunPosition4f = vmml::Vector4f(-camera->getPosition() + getSunPosition());
    
    sunPosition4f = projection * view * sunPosition4f;
    vmml::Vector2f sunPositionCube;
    sunPositionCube.x() = (sunPosition4f.x() / sunPosition4f.w() + 1.0) * 0.5;
    sunPositionCube.y() = (sunPosition4f.y() / sunPosition4f.w() + 1.0) * 0.5;
    
    // Set sun position to the shader
    godrayShader->setUniform("lightPositionOnScreen", sunPositionCube);
    
    // Create fullscreen geometry
    GeometryDataPtr geometryData = GeometryDataPtr(new GeometryData);
    geometryData->vboVertices.emplace_back(-1, -1, 0, 0, 0);
    geometryData->vboVertices.emplace_back(1, -1, 0, 0, 0);
    geometryData->vboVertices.emplace_back(-1, 1, 0, 0, 0);
    geometryData->vboVertices.emplace_back(-1, 1, 0, 0, 0);
    geometryData->vboVertices.emplace_back(1, -1, 0, 0, 0);
    geometryData->vboVertices.emplace_back(1, 1, 0, 0, 0);
    
    for(int i = 0; i < 6; i++)
        geometryData->vboIndices.push_back(i);
    
    MaterialPtr material;
    if((material = renderer.getObjects()->getMaterial("fullscreenMap")) == nullptr)
        material = renderer.getObjects()->createMaterial("fullscreenMap", godrayShader);
    
    GeometryPtr geometry = GeometryPtr(new Geometry);
    geometry->setMaterial(material);
    geometry->initialize(geometryData);
    
    // Bind first pass texture and draw the fullscreen geometry
    geometry->draw();
    
}

