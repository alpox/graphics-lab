//
//  StreamSystem.h
//  bRenderer_ios
//
//  Created by Elias on 15.05.17.
//
//

#ifndef StreamSystem_h
#define StreamSystem_h

class StreamSystem: public virtual System {
public:
    StreamSystem() {
        requirements_mask = (COMPONENT_RENDERER | COMPONENT_STREAM);
    }
    
protected:
    void apply(Entity& entity) const {
        auto renderer = entity.getComponent<Render>(COMPONENT_RENDERER);
        auto stream = entity.getComponent<Stream>(COMPONENT_STREAM);
        
        renderer->bRenderer.getObjects()->getProperties("streamProperties")->setScalar("offset", stream->offset);
    }
};

#endif /* StreamSystem_h */
