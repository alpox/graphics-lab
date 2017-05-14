//
//  Renderer.h
//  bRenderer_ios
//
//  Created by Elias on 14.05.17.
//
//

#ifndef Renderer_h
#define Renderer_h

#include "bRenderer.h"

class Render: public Component {
public:
    Render(Renderer& renderer, std::string modelName, std::string instanceName,
                    std::string camera, vmml::Vector3f&& ambientColor, std::vector<std::string>&& lightNames):
    bRenderer(renderer), modelName(modelName), instanceName(instanceName), camera(camera),
            ambientColor(ambientColor), lightNames(lightNames) {
                type = COMPONENT_RENDERER;
    }
    
    Renderer& bRenderer;
    std::string modelName;
    std::string instanceName;
    std::string camera;
    std::vector<std::string> lightNames;
    vmml::Vector3f& ambientColor;
    bool doFrustrumCulling = true;
    bool cullIndividualGeometry = false;
    bool isTransparent = false;
    GLenum blendSfactor = GL_SRC_ALPHA;
    GLenum blendDfactor = GL_ONE_MINUS_SRC_ALPHA;
    GLfloat customDistance = 10000.0f;
};

#endif /* Renderer_h */
