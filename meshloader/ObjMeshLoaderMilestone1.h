//
//  ObjMeshLoaderMilestone1.hpp
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 7/8/16.
//  Copyright © 2016 Abraham-mac. All rights reserved.
//

#ifndef ObjMeshLoaderMilestone1_h
#define ObjMeshLoaderMilestone1_h

#include "MeshLoader.h"

class ObjMeshLoaderMilestone1 : public MeshLoader
{
public:
    
    ObjMeshLoaderMilestone1( const GLchar * path )
    {
        loadFromFile( path );
    }
    
    ~ObjMeshLoaderMilestone1()
    {
    }
    
    bool isSplitVertex( const GLuint & index, const GLuint & normalIndex, std::map<GLuint,std::vector<GLuint>*> * normalIndexMap );
    
    void loadFromFile( const GLchar * path );
};

#endif /* ObjMeshLoaderMilestone1_h */
