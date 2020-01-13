//
//  ObjMeshLoaderQuadsExperiment.h
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 6/29/16.
//  Copyright © 2016 Abraham-mac. All rights reserved.
//

#ifndef ObjMeshLoaderQuadsExperiment_h
#define ObjMeshLoaderQuadsExperiment_h

#include "MeshLoader.h"

class ObjMeshLoaderQuadsExperiment : public MeshLoader
{
public:
    
    ObjMeshLoaderQuadsExperiment( const GLchar * path )
    {
        loadFromFile( path );
    }
    
    ~ObjMeshLoaderQuadsExperiment()
    {
    }
    
    bool isSplitVertex( const GLuint & index, const GLuint & normalIndex, std::map<GLuint,std::vector<GLuint>*> * normalIndexMap );
    
    void loadFromFile( const GLchar * path );
};

#endif /* ObjMeshLoaderQuadsExperiment_h */
