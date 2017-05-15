//
//  Stream.h
//  bRenderer_ios
//
//  Created by Elias on 15.05.17.
//
//

#ifndef Stream_h
#define Stream_h

#include "Component.h"

class Stream: public Component {
public:
    Stream(float& offset): offset(offset) {
        type = COMPONENT_STREAM;
    }
    float& offset;
};

typedef std::shared_ptr<Stream> StreamPtr;

#endif /* Stream_h */
