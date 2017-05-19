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
    void apply(EntityPtr entity) const {
        auto renderer = entity->getComponent<Render>(COMPONENT_RENDERER);
        auto transform = entity->getComponent<Transform>(COMPONENT_TRANSFORM);
        
        entity->renderer().getObjects()->setAmbientColor(renderer->ambientColor);
        entity->renderer().getModelRenderer()->queueModelInstance(entity->modelName(), entity->instanceName(),
                                                renderer->camera, transform->modelMatrix, renderer->lightNames,
                                                renderer->doFrustrumCulling, renderer->cullIndividualGeometry,
                                                renderer->isTransparent, renderer->blendSfactor,
                                                renderer->blendDfactor, renderer->customDistance);
        entity->renderer().getObjects()->setAmbientColor(bRenderer::DEFAULT_AMBIENT_COLOR());
    }
};

#endif /* Render_h */
