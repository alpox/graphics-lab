//
//  constants.h
//  bRenderer_ios
//
//  Created by Elias on 15.06.17.
//
//

#ifndef constants_h
#define constants_h

typedef unsigned short COMPONENT_MASK;

// Maybe we use a registry later
const COMPONENT_MASK COMPONENT_NONE = 1;
const COMPONENT_MASK COMPONENT_TRANSFORM = 1 << 1;
const COMPONENT_MASK COMPONENT_RENDERER = 1 << 2;
const COMPONENT_MASK COMPONENT_STREAM = 1 << 3;
const COMPONENT_MASK COMPONENT_NODEPTH = 1 << 4;
const COMPONENT_MASK COMPONENT_COLLIDER = 1 << 5;

typedef unsigned short PASS;

const PASS FIRST_PASS = 1;
const PASS SECOND_PASS = 2;
const PASS THIRD_PASS = 3;
const PASS SHADOW_PASS = 4;
const PASS LIGHT_PASS = 5;

typedef unsigned short EFFECT;

const EFFECT EFFECT_CEL = 1;
const EFFECT EFFECT_BLUEVISION = 1 << 1;

const int EFFECT_DURATION = 10;

#endif /* constants_h */
