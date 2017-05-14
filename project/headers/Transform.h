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
public:
    Transform(vmml::Matrix4f modelMatrix): modelMatrix(modelMatrix) {
        type = COMPONENT_TRANSFORM;
    }
    vmml::Matrix4f modelMatrix;
};

#endif /* Position_h */
