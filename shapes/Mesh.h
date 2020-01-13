//
//  Mesh.h
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 7/1/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#ifndef __OpenGLOtherTest__Mesh__
#define __OpenGLOtherTest__Mesh__

#include "Shape.h"

class Mesh : public Shape
{
public:
    
    Mesh() {}
    Mesh( const GLchar * path );
    virtual ~Mesh()
    {
        
    }
    
    virtual void transform( glm::mat4 m_transform );
    
    virtual void draw( glm::mat4 m_transform );
    
protected:
    
    bool isTriangleFaces = true;
    
    GLsizei * indicesCount = NULL;
};


#endif /* defined(__OpenGLOtherTest__Mesh__) */
