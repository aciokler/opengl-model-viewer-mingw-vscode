//
//  ObjMeshLoaderFasterExperiment.hpp
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 3/11/16.
//  Copyright © 2016 Abraham-mac. All rights reserved.
//

#ifndef ObjMeshLoaderFasterExperiment_hpp
#define ObjMeshLoaderFasterExperiment_hpp

#include "MeshLoader.h"

class ObjMeshLoaderFasterExperiment : public MeshLoader
{
public:
    
    ObjMeshLoaderFasterExperiment( const GLchar * path )
    {
        loadFromFile( path );
    }
    
    ~ObjMeshLoaderFasterExperiment()
    {
    }
    
    bool isSplitVertex( const GLuint & index, const GLuint & normalIndex, std::map<GLuint,std::vector<GLuint>*> * normalIndexMap );
    
    void loadFromFile( const GLchar * path );
};

#endif /* ObjMeshLoaderFasterExperiment_hpp */
