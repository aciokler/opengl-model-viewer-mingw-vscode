//
//  declarations.h
//  ModernOpenGLSemiAdvanced
//
//  Created by Abraham-mac on 1/20/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#ifndef _declarations_h
#define _declarations_h

// open gl define
//#define GLFW_INCLUDE_GLCOREARB

//#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>
#include <set>
#include <map>
#include <stdexcept>
#include <algorithm>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"

class Vertex
{
public:
    
    Vertex()
    {
    }
    
    Vertex( glm::vec3 pos, glm::vec3 norml )
    {
        position = pos; normal = norml;
    }
    
    void setPosition( const glm::vec3 pos )
    {
        position = pos;
    }
    
    void setNormal( const glm::vec3 norml )
    {
        normal = norml;
    }
    
    glm::vec3 getPosition() const
    {
        return position;
    }
    
    glm::vec3 getNormal() const
    {
        return normal;
    }
    
protected:
    glm::vec3 position;
    glm::vec3 normal;
};

typedef struct Face
{
    GLuint vertexIndices[3];
    GLuint vertexNormalIndices[3];
    GLuint normalInd;
    glm::vec3 normal;
    GLuint normalWeight = 1;
    
    Face()
    {
    }
    
    Face( const Face & face )
    {
        if ( this != &face )
            swap(face);
    }
    
    Face & operator=( const Face & face )
    {
        if (  this != &face )
            swap(face);
        return *this;
    }
    
    void swap( const Face & face )
    {
        vertexIndices[0] = face.vertexIndices[0];
        vertexIndices[1] = face.vertexIndices[1];
        vertexIndices[2] = face.vertexIndices[2];
        
        vertexNormalIndices[0] = face.vertexNormalIndices[0];
        vertexNormalIndices[1] = face.vertexNormalIndices[1];
        vertexNormalIndices[2] = face.vertexNormalIndices[2];
        
        normalInd = face.normalInd;
        normal = face.normal;
        normalWeight = face.normalWeight;
    }
    
    bool operator==(const Face &right)
    {
        return (vertexIndices[0] == right.vertexIndices[0] &&
                vertexIndices[1] == right.vertexIndices[1] &&
                vertexIndices[2] == right.vertexIndices[2] &&
                
                vertexNormalIndices[0] == right.vertexNormalIndices[0] &&
                vertexNormalIndices[1] == right.vertexNormalIndices[1] &&
                vertexNormalIndices[2] == right.vertexNormalIndices[2] &&
                
                normal == right.normal &&
                normalInd == right.normalInd &&
                normalWeight == right.normalWeight);
    }
} Face;

class Group
{
public:
    
    Group()
    {
        indices = new std::vector<GLuint>();
        normals = new std::vector<glm::vec3>();
        faces = new std::vector<Face>();
    }
    
    Group( const Group & group )
    {
        //printf( "calling the copy constructor\n" );
        indices = new std::vector<GLuint>();
        normals = new std::vector<glm::vec3>();
        faces = new std::vector<Face>();
        
        swap( group );
        
        //printf( "after copy\n" );
    }
    
    ~Group()
    {
        printf("deleting group\n");
        delete indices;
        delete normals;
        delete faces;
        printf("finished deleting group\n");
    }
    
    std::vector<GLuint> * getIndices() const
    {
        return indices;
    }
    
    std::vector<glm::vec3> * getNormals() const
    {
        return normals;
    }
    
    std::vector<Face> * getFaces() const
    {
        return faces;
    }
    
    Group & operator=( const Group & rhs )
    {
        if ( this != &rhs )
        {
            indices->clear();
            normals->clear();
            faces->clear();
            
            //printf( "before swap\n" );
            swap( rhs );
            //printf( "returning the this\n" );
        }
        return *this;
    }
    
private:
    
    void swap( const Group & group )
    {
        for ( std::vector<glm::vec3>::iterator vecItr = group.getNormals()->begin(); vecItr != group.getNormals()->end(); vecItr++ )
        {
            normals->push_back(*vecItr);
        }
        
        for ( std::vector<GLuint>::iterator vecItr = group.getIndices()->begin(); vecItr != group.getIndices()->end(); vecItr++ )
        {
            indices->push_back(*vecItr);
        }
        
        for ( std::vector<Face>::iterator vecItr = group.getFaces()->begin(); vecItr != group.getFaces()->end(); vecItr++ )
        {
            faces->push_back(*vecItr);
        }
        
        //printf( "after swap\n" );
    }
    
    std::vector<glm::vec3> * vertices = NULL;
    std::vector<GLuint> * indices = NULL;
    std::vector<glm::vec3> * normals = NULL;
    std::vector<Face> * faces = NULL;
    
    size_t bufferSize;
};

class Object
{
public:
    
    Object()
    {
        vertices = new std::vector<glm::vec3>();
        normals = new std::vector<glm::vec3>();
    }
    
    Object( const Object & obj )
    {
        vertices = new std::vector<glm::vec3>();
        normals = new std::vector<glm::vec3>();
        swap( obj );
    }
    
    ~Object()
    {
        printf("deleting object\n");
        delete vertices;
    }
    
    std::vector<glm::vec3> * getVertices() const
    {
        return vertices;
    }
    
    std::vector<glm::vec3> * getNormals() const
    {
        return normals;
    }
    
    std::vector<Group> * getGroups() const
    {
        return groups;
    }
    
    Object & operator=( const Object & rhs )
    {
        if ( this != &rhs )
        {
            vertices->clear();
            normals->clear();
            groups->clear();
            swap( rhs );
        }
        return *this;
    }
    
private:
    
    void swap( const Object & obj )
    {
        //printf("before the swap %lu\n", group.getVertices()->size() );
        for ( std::vector<glm::vec3>::iterator vecItr = obj.getVertices()->begin(); vecItr != obj.getVertices()->end(); vecItr++ )
        {
            //printf( "copying values x=%f y=%f z=%f\n", vecItr->x, vecItr->y, vecItr->z );
            vertices->push_back(*vecItr);
        }
        
        for ( std::vector<glm::vec3>::iterator vecItr = obj.getNormals()->begin(); vecItr != obj.getNormals()->end(); vecItr++ )
        {
            normals->push_back(*vecItr);
        }
        
        for ( std::vector<Group>::iterator vecItr = obj.getGroups()->begin(); vecItr != obj.getGroups()->end(); vecItr++ )
        {
            groups->push_back(*vecItr);
        }
    }
    
    std::vector<glm::vec3> * vertices = NULL;
    std::vector<glm::vec3> * normals = NULL;
    std::vector<Group> * groups = NULL;
};

#endif
