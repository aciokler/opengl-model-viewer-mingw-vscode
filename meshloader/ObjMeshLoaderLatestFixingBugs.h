//
//  ObjMeshLoaderLatestFixingBugs.hpp
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 7/7/16.
//  Copyright © 2016 Abraham-mac. All rights reserved.
//

#ifndef ObjMeshLoaderLatestFixingBugs_h
#define ObjMeshLoaderLatestFixingBugs_h

#include "MeshLoader.h"

class ObjMeshLoaderLatestFixingBugs : public MeshLoader
{
public:
    
    ObjMeshLoaderLatestFixingBugs( const GLchar * path )
    {
        loadFromFile( path );
    }
    
    ~ObjMeshLoaderLatestFixingBugs()
    {
    }
    
    bool isSplitVertex( const GLuint & index, const GLuint & normalIndex, std::map<GLuint,std::vector<GLuint>*> * normalIndexMap );
    
    void loadFromFile( const GLchar * path );
};


#endif /* ObjMeshLoaderLatestFixingBugs_h */
