//
//  ObjMeshLoaderSmoothNormalsExperiment.hpp
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 3/15/16.
//  Copyright © 2016 Abraham-mac. All rights reserved.
//

#ifndef ObjMeshLoaderSmoothNormalsExperiment_h
#define ObjMeshLoaderSmoothNormalsExperiment_h

#include "MeshLoader.h"

class ObjMeshLoaderSmoothNormalsExperiment : public MeshLoader
{
public:
    
    ObjMeshLoaderSmoothNormalsExperiment( const GLchar * path )
    {
        loadFromFile( path );
    }
    
    ~ObjMeshLoaderSmoothNormalsExperiment()
    {
    }
    
    bool isSplitVertex( const GLuint & index, const GLuint & normalIndex, std::map<GLuint,std::vector<GLuint>*> * normalIndexMap );
    
    void loadFromFile( const GLchar * path );
};

#endif /* ObjMeshLoaderSmoothNormalsExperiment_h */
