//
//  Shape.h
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 7/1/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#ifndef OpenGLOtherTest_Shape_h
#define OpenGLOtherTest_Shape_h

#include "../common/declarations.h"
#include "../shaders/Shader.h"

class Shape
{
public:
    
    Shape()
    {
        shader = new Shader();
        
        //glGenBuffers(1, &VBO);
        //glGenBuffers(1, &EBO);
        //glGenVertexArrays(1, &VAO);
    }
    
    virtual ~Shape()
    {
        printf("calling shape constructor\n");
        glDeleteBuffers(TOTAL_BUFFERS, VBO);
        glDeleteVertexArrays(nr_groups, VAO);
        delete [] VAO;
        
        delete shader;
        
        cleanUp();
//        printf("deleting the vertices and indices\n");
//        for ( int i = 0; i < nr_groups; i++ )
//        {
//            //delete vertices[i];
//            if ( verticesOrder != NULL && verticesOrder[i] != NULL )
//                delete verticesOrder[i];
//        }
//        
//        printf("deleting dynamic arrays\n");
//        if ( vertices )
//        {
//            printf("deleting vertices\n");
//            delete [] vertices;
//        }
//        
//        if ( verticesOrder )
//        {
//            printf("deleting verticesOrder\n");
//            delete [] verticesOrder;
//        }
//        if ( elements )
//        {
//            printf("deleting elements\n");
//            delete [] elements;
//        }
//        if ( groups )
//        {
//            printf("deleting groups\n");
//            delete groups;
//        }
//        if( objects )
//        {
//            printf("deleting objects\n");
//            delete objects;
//        }
        
        printf("done with shape\n");
    }
    
    virtual void transform( glm::mat4 m_transform ) = 0;
    virtual void draw( glm::mat4 m_transform ) = 0;
    void cleanUp()
    {
        printf("deleting the vertices and indices\n");
        for ( int i = 0; i < nr_groups; i++ )
        {
            //delete vertices[i];
            if ( verticesOrder != NULL && verticesOrder[i] != NULL )
                delete verticesOrder[i];
        }
        
        printf("deleting dynamic arrays\n");
        if ( vertices )
        {
            printf("deleting vertices\n");
            delete [] vertices;
            vertices = NULL;
        }
        
        if ( verticesOrder )
        {
            printf("deleting verticesOrder\n");
            delete [] verticesOrder;
            verticesOrder = NULL;
        }
        if ( elements )
        {
            printf("deleting elements\n");
            delete [] elements;
            elements = NULL;
        }
        if ( groups )
        {
            printf("deleting groups\n");
            delete groups;
            groups = NULL;
        }
        if( objects )
        {
            printf("deleting objects\n");
            delete objects;
            objects = NULL;
        }
    }
    
    enum
    {
        POSITIONS_BUFF,
        INDEX_BUFF,
        COLOR_BUFF,
        TOTAL_BUFFERS
    };
    
    GLuint getVBOPointer( GLuint type ) { return VBO[ type ]; }
    GLuint getVAOPointer( GLuint index ) { return VAO[index]; }
    
protected:
    
    GLfloat * vertices = NULL;   // vertices
    GLuint ** verticesOrder = NULL; // vertices order
    GLuint * elements = NULL;
    std::vector<Group> * groups = NULL;
    std::vector<Object> * objects = NULL;
    
    const GLuint BUFFER_OFFSET_FACTOR = 6;
    
    GLuint SIZE = 0;
    GLuint nr_vertices = 0;
    GLuint nr_normals = 0;
    GLuint nr_groups = 0;
    
    GLuint VBO[ Shape::TOTAL_BUFFERS ];
    GLuint * VAO = NULL;
    GLuint EBO;
    
    Shader * shader = NULL;
    
    GLint * positionLoc2 = NULL;
    GLint positionLoc;
    GLint uniform_m_transform;
    GLint colorLoc;
    GLint * normalLoc2 = NULL;
    GLint normalLoc;
    
    GLuint drawCount = 0;
};

#endif
