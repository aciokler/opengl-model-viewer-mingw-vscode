//
//  MeshLoader.h
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 7/1/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#ifndef OpenGLOtherTest_MeshLoader_h
#define OpenGLOtherTest_MeshLoader_h

#include "../common/declarations.h"

class MeshLoader
{
public:
    
    virtual ~MeshLoader()
    {
        delete groups;
        
        delete vertices;
        delete normals;
        delete indices;
    }
    
    virtual void loadFromFile( const GLchar * path ) = 0;
    
    /**
     * return a group of the groups
     */
    std::vector<Group> * getGroups() const
    {
        std::vector<Group> * copy = new std::vector<Group>();
        Group * groupCopy = NULL;
        for ( std::vector<Group>::iterator itr = groups->begin(); itr != groups->end(); itr++ )
        {
            groupCopy = new Group();
            
//            for ( std::vector<glm::vec3>::iterator vecItr = itr->getVertices()->begin(); vecItr != itr->getVertices()->end(); vecItr++ )
//            {
//                groupCopy->getVertices()->push_back(*vecItr);
//                
//                //std::cout << vecItr->x << ", " << vecItr->y << ", " << vecItr->z << std::endl;
//            }
            
            //std::cout << "size of vertices after copying " << groupCopy->getVertices()->size() << std::endl;
            
//            for ( std::vector<glm::vec3>::iterator vecItr = itr->getNormals()->begin(); vecItr != itr->getNormals()->end(); vecItr++ )
//            {
//                groupCopy->getNormals()->push_back(*vecItr);
//            }
            
            for ( std::vector<GLuint>::iterator vecItr = itr->getIndices()->begin(); vecItr != itr->getIndices()->end(); vecItr++ )
            {
                groupCopy->getIndices()->push_back(*vecItr);
            }
            
            for ( std::vector<Face>::iterator vecItr = itr->getFaces()->begin(); vecItr != itr->getFaces()->end(); vecItr++ )
            {
                groupCopy->getFaces()->push_back(*vecItr);
                
                //std::cout << vecItr->vertexIndices[0] << ", " << vecItr->vertexIndices[1] << ", " << vecItr->vertexIndices[2] << std::endl;
            }
            
            copy->push_back(*groupCopy);
        }
        
        //printf( "before returing copy of groups\n" );
        return copy;
    }
    
    std::vector<Object> * getObjects() const
    {
        return objects;
    }
    
    std::vector<glm::vec3> * getVertices() const
    {
        std::vector<glm::vec3> * copy = new std::vector<glm::vec3>();
        for ( std::vector<glm::vec3>::iterator vecItr = vertices->begin(); vecItr != vertices->end(); vecItr++ )
        {
            copy->push_back(*vecItr);
        }
        
        return copy;
    }
    
    std::vector<glm::vec3> * getNormals() const
    {
        std::vector<glm::vec3> * copy = new std::vector<glm::vec3>();
        for ( std::vector<glm::vec3>::iterator vecItr = normals->begin(); vecItr != normals->end(); vecItr++ )
        {
            copy->push_back(*vecItr);
        }
        
        return copy;

    }
    
    unsigned long getTotalVertices() const
    {
        return vertices->size();
    }
    
    unsigned long getTotalNormals() const
    {
        return normals->size();
    }
    
//    std::vector<GLuint> * getIndices() const
//    {
//        return indices;
//    }
    
    bool getIsTriangleFaces() const
    {
        return isTriangleFaces;
    }
    
protected:
    
    bool isSplitVertex( const GLuint & index, const GLuint & normalIndex, std::map<GLuint,std::vector<GLuint>*> * normalIndexMap );
    
    std::vector<Object> * objects = new std::vector<Object>();
    std::vector<Group> * groups = new std::vector<Group>();
    std::vector<glm::vec3> * vertices = new std::vector<glm::vec3>();
    std::vector<glm::vec3> * normals = new std::vector<glm::vec3>();
    std::vector<GLuint> * indices = new std::vector<GLuint>();
    
    bool isTriangleFaces = true;
    
};

#endif
