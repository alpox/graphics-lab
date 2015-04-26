#ifndef B_LINK_TO_VIEW_H
#define B_LINK_TO_VIEW_H

#include "../../headers/OSdetect.h"
#ifdef OS_IOS

#include "../../headers/Renderer_GL.h"

/* These functions get called by the view class and serve as a bridge between the Objective-C and the c++ code */

namespace bRenderer
{
    /**	@brief Draw the scene
     */
    void draw();

} // namespace bRenderer

#endif

#endif /* defined(B_LINK_TO_VIEW_H) */