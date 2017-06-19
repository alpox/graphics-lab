//
//  Collider.h
//  bRenderer_ios
//
//  Created by Elias on 19.06.17.
//
//

#ifndef Collider_h
#define Collider_h

#include "Component.h"

class Collider: public Component {
public:
    Collider(EFFECT effect): effect(effect) {
        type = COMPONENT_COLLIDER;
    }
    
    EFFECT effect;
};

typedef std::shared_ptr<Collider> ColliderPtr;

#endif /* Collider_h */
