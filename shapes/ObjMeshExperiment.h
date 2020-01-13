//
//  ObjMeshExperiment.h
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 6/29/15.
//  Copyright (c) 2015 Abraham-mac. All rights reserved.
//

#ifndef __OpenGLOtherTest__ObjMeshExperiment__
#define __OpenGLOtherTest__ObjMeshExperiment__

#include "../common/declarations.h"
#include "Mesh.h"

class ObjMeshExperiment : public Mesh
{
public:
    
    ObjMeshExperiment( const GLchar * path );
    virtual ~ObjMeshExperiment()
    {
        
    }
    
    void transform( glm::mat4 m_transform );
    
    void draw( glm::mat4 m_transform );
    
protected:
    
    void initWithGroups( const std::vector<glm::vec3> * meshVertices, const std::vector<glm::vec3> * meshNormals );
    void initWithObjects( const std::vector<glm::vec3> * meshVertices, const std::vector<glm::vec3> * meshNormals );
    void prepareBuffers();
    
    GLsizei * indicesCount = NULL;
};

#endif /* defined(__OpenGLOtherTest__ObjMeshExperiment__) */
