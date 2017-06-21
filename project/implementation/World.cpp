//
//  World.cpp
//  bRenderer_ios
//
//  Created by Elias on 14.05.17.
//
//

#include "World.h"

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

void renderSun(Renderer& renderer) {
    
    ShaderPtr sunShader = renderer.getObjects()->loadShaderFile("colorShader", 0, false, false, false, false, false);
    CameraPtr camera = renderer.getObjects()->getCamera("camera");
    
    // Load model if necessary
    if(renderer.getObjects()->getModel("sphere") == nullptr)
        renderer.getObjects()->loadObjModel_o("sphere.obj", sunShader);
    
    // Set camera position to 0
    vmml::Vector3f cameraPosition = camera->getPosition();
    camera->setPosition({0.0,0.0,0.0});
    
    // Orient model
    vmml::Matrix4f modelMatrix = vmml::create_translation(vmml::Vector3f::BACKWARD * 20.f) *
        vmml::create_scaling(vmml::Vector3f(0.5f));
    
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

void renderSkyCube(Renderer &renderer) {
    
    ObjectManagerPtr objectManager = renderer.getObjects();
    
    CameraPtr camera = objectManager->getCamera("camera");
    
    // Load model if necessary
    CubeMapPtr skyBox;
    ShaderPtr skyShader = objectManager->loadShaderFile("skyBox", 0, false, false, false, false, false);
    
    // Load cubemap texture
	//cloudy sky
	/*
    if((skyBox = objectManager->getCubeMap("skyCube")) == nullptr) {
        TextureData skyLeft("WhiteCloudLeft.jpg");
        TextureData skyRight("WhiteCloudRight.jpg");
        TextureData skyUp("WhiteCloudUp.jpg");
        TextureData skyFront("WhiteCloudFront.jpg");
        TextureData skyBack("WhiteCloudBack.jpg");
        TextureData skyDown("WhiteCloudDown.jpg");
        
        skyBox = objectManager->createCubeMap("skyCube", { skyLeft, skyRight, skyUp, skyDown, skyFront, skyBack});
    }
	*/
	//desert
	if ((skyBox = objectManager->getCubeMap("skyCube")) == nullptr) {
		TextureData skyLeft("sahara_rt.jpg");
		TextureData skyRight("sahara_lf.jpg");
		TextureData skyUp("sahara_up.jpg");
		TextureData skyFront("sahara_ft.jpg");
		TextureData skyBack("sahara_bk.jpg");
		TextureData skyDown("sahara_dn.jpg");

		skyBox = objectManager->createCubeMap("skyCube", { skyLeft, skyRight, skyUp, skyDown, skyFront, skyBack });
	}
    
    if(renderer.getObjects()->getModel("cube") == nullptr)
        renderer.getObjects()->loadObjModel_o("cube.obj", skyShader, FLIP_T);
    
    
    // Set camera position to 0
    vmml::Vector3f cameraPosition = camera->getPosition();
    camera->setPosition({0.0,0.0,0.0});
    
    // Orient model
    vmml::Matrix4f modelMatrix = vmml::create_scaling(vmml::Vector3f(3.f));
    
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

void World::render(const double &deltaTime) {
    // Apply all systems before rendering
    applySystems(deltaTime);
    
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
    //renderSun(renderer);
    std::for_each(entities.begin(), entities.end(), [&deltaTime](const auto& entity) {
        entity->render(deltaTime, FIRST_PASS);
    });
    
    framebuffer->unbind(defaultFBO); // Restore default fbo
    
    // Second pass
    glClear(GL_DEPTH_BUFFER_BIT);
    
    renderSkyCube(renderer);
    std::for_each(entities.begin(), entities.end(), [&deltaTime](const auto& entity) {
        entity->render(deltaTime, SECOND_PASS);
    });
    
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
    godrayShader->setUniform("density", (GLfloat)1.0);
    godrayShader->setUniform("weight", (GLfloat)0.01);
    
    CameraPtr camera = renderer.getObjects()->getCamera("camera");
    vmml::Matrix4f view = camera->getViewMatrix();
    vmml::Matrix4f projection = camera->getProjectionMatrix();
    
    // Transform sun position to screen position
    vmml::Vector4f sunPosition4f = vmml::Vector4f(-camera->getPosition() + vmml::Vector3f::BACKWARD * 20.f);
    
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
