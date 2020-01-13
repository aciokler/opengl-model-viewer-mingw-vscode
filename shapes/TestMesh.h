//
//  TestMesh.h
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 7/9/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#ifndef __OpenGLOtherTest__TestMesh__
#define __OpenGLOtherTest__TestMesh__

#include "Shape.h"


class TestMesh : public Shape
{
public:
    
    TestMesh();
    
    virtual ~TestMesh()
    {
        
    }
    
    virtual void draw(glm::mat4 m_transform);
    
    virtual void transform( glm::mat4 m_transform );
    
private:
    
};

#endif /* defined(__OpenGLOtherTest__TestMesh__) */
