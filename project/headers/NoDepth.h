//
//  NoDepth.h
//  bRenderer_ios
//
//  Created by Elias on 15.06.17.
//
//

#ifndef NoDepth_h
#define NoDepth_h

#include "Component.h"

class NoDepth: public Component {
public:
    NoDepth() {
        type = COMPONENT_NODEPTH;
    }
};

typedef std::shared_ptr<NoDepth> NoDepthPtr;

#endif /* NoDepth_h */
