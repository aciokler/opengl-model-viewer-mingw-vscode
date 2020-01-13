//
//  ObjMeshLoader2Passes.h
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 10/29/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#ifndef OpenGLOtherTest_ObjMeshLoader2Passes_h
#define OpenGLOtherTest_ObjMeshLoader2Passes_h

#include "MeshLoader.h"

class ObjMeshLoader2Passes : public MeshLoader
{
public:
    
    ObjMeshLoader2Passes( const GLchar * path )
    {
        loadFromFile( path );
    }
    
    ~ObjMeshLoader2Passes()
    {
    }
    
    bool isSplitVertex( const GLuint & index, const GLuint & normalIndex, std::map<GLuint,std::vector<GLuint>*> * normalIndexMap );
    
    void loadFromFile( const GLchar * path );
    
private:
    void firstPass( std::ifstream & in );
    
    void secondPass( std::ifstream & in );
    
    void buildNormals();
};

#endif
