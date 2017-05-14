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
    void apply(Entity& entity) const {
        auto renderer = entity.getComponent<Render>(COMPONENT_RENDERER);
        auto transform = entity.getComponent<Transform>(COMPONENT_TRANSFORM);
        
        //renderer->bRenderer.getObjects()->setAmbientColor(renderer->ambientColor);
        renderer->bRenderer.getModelRenderer()->queueModelInstance(renderer->modelName, renderer->instanceName,
                                                renderer->camera, transform->modelMatrix, renderer->lightNames,
                                                renderer->doFrustrumCulling, renderer->cullIndividualGeometry,
                                                renderer->isTransparent, renderer->blendSfactor,
                                                renderer->blendDfactor, renderer->customDistance);
    }
};

#endif /* Render_h */
