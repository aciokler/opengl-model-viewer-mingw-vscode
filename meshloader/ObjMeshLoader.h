//
//  ObjMeshLoader.h
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 7/1/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#ifndef __OpenGLOtherTest__ObjMeshLoader__
#define __OpenGLOtherTest__ObjMeshLoader__

#include "MeshLoader.h"

class ObjMeshLoader : public MeshLoader
{
public:
    
    ObjMeshLoader( const GLchar * path )
    {
        loadFromFile( path );
    }
    
    ~ObjMeshLoader()
    {
    }
    
    bool isSplitVertex( const GLuint & index, const GLuint & normalIndex, std::map<GLuint,std::vector<GLuint>*> * normalIndexMap );
    
    void loadFromFile( const GLchar * path );
};

#endif /* defined(__OpenGLOtherTest__ObjMeshLoader__) */
