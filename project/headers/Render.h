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
    Render(
           std::vector<std::string>&&lightNames,
           bool doFrustrumCulling = true, bool cullIndividualGeometry = false, bool isTransparent = false,
           GLenum blendSfactor = GL_SRC_ALPHA, GLenum blendDfactor = GL_ONE_MINUS_SRC_ALPHA,
           GLfloat customDistance = 10000.0f):
    ambientColor(bRenderer::DEFAULT_AMBIENT_COLOR()),
    lightNames(lightNames), doFrustrumCulling(doFrustrumCulling),
    cullIndividualGeometry(cullIndividualGeometry), isTransparent(isTransparent),
    blendSfactor(blendSfactor), blendDfactor(blendDfactor), customDistance(customDistance){
        type = COMPONENT_RENDERER;
    }
    
    Render(
          vmml::Vector3f&& ambientColor, std::vector<std::string>&& lightNames,
          bool doFrustrumCulling = true, bool cullIndividualGeometry = false, bool isTransparent = false,
          GLenum blendSfactor = GL_SRC_ALPHA, GLenum blendDfactor = GL_ONE_MINUS_SRC_ALPHA,
          GLfloat customDistance = 10000.0f):
   ambientColor(ambientColor), lightNames(lightNames), doFrustrumCulling(doFrustrumCulling),
   cullIndividualGeometry(cullIndividualGeometry), isTransparent(isTransparent),
   blendSfactor(blendSfactor), blendDfactor(blendDfactor), customDistance(customDistance) {
       type = COMPONENT_RENDERER;
   }
    // Renderqueue properties
    std::string camera = "camera";
    std::vector<std::string> lightNames;
    vmml::Vector3f ambientColor;
    bool doFrustrumCulling = true;
    bool cullIndividualGeometry = false;
    bool isTransparent = false;
    GLenum blendSfactor = GL_SRC_ALPHA;
    GLenum blendDfactor = GL_ONE_MINUS_SRC_ALPHA;
    GLfloat customDistance = 10000.0f;
    
    // Model properties
    PropertiesPtr properties;
    ShaderPtr shader;
    double totalTime;
};

typedef std::shared_ptr<Render> RenderPtr;

#endif /* Renderer_h */
