//
//  Position.h
//  bRenderer_ios
//
//  Created by Elias on 14.05.17.
//
//

#ifndef Position_h
#define Position_h

#include "Component.h"
#include "vmmlib/matrix.hpp"

class Transform: public Component {
protected:
    void setType() override {
        type = COMPONENT_POSITION;
    }
    
    vmml::Matrix4f transform;
};

#endif /* Position_h */
