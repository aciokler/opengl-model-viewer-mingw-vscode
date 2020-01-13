//
//  Cube.h
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 7/1/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#ifndef __OpenGLOtherTest__Cube__
#define __OpenGLOtherTest__Cube__

#include "Shape.h"

class Cube : public Shape
{
public:
    Cube();
    
    void transform( glm::mat4 m_transform );
    
    void draw( glm::mat4 m_transform );
};

#endif /* defined(__OpenGLOtherTest__Cube__) */
