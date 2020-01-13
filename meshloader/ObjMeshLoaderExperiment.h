//
//  ObjMeshLoaderExperiment.h
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 6/29/15.
//  Copyright (c) 2015 Abraham-mac. All rights reserved.
//

#ifndef __OpenGLOtherTest__ObjMeshLoaderExperiment__
#define __OpenGLOtherTest__ObjMeshLoaderExperiment__

#include "MeshLoaderExperiment.h"

class ObjMeshLoaderExperiment : public MeshLoader
{
public:
    
    ObjMeshLoaderExperiment( const GLchar * path )
    {
        loadFromFile( path );
    }
    
    ~ObjMeshLoaderExperiment()
    {
    }
    
    bool isSplitVertex( const GLuint & index, const GLuint & normalIndex, std::map<GLuint,std::vector<GLuint>*> * normalIndexMap );
    
    void loadFromFile( const GLchar * path );
};


#endif /* defined(__OpenGLOtherTest__ObjMeshLoaderExperiment__) */
