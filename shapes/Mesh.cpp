//
//  Mesh.cpp
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 7/1/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#include "Mesh.h"
#include "../meshloader/ObjMeshLoader.h"
#include "../meshloader/ObjMeshLoader2Passes.h"

static void print( const glm::vec3 * array, const GLint length, const std::string str )
{
    for ( int i = 0; i < length; i++ )
        std::cout << str << " x " << array[i].x << ", y " << array[i].y << ", z " << array[i].z << std::endl;
}

static void print( const GLuint * array, const GLint length, const std::string str )
{
    for ( int i = 0; i < length; i++ )
    {
        std::cout << str << " " << array[i] << std::endl;
    }
}

static void print( const GLfloat * array, const GLint length, const std::string str )
{
    printf( "size %d\n\n", length );
    
    for ( int i = 0; i < length; i+=6 )
    {
        GLuint vIndex = i/6;
        std::cout << str << vIndex << " v " << array[i] << " " << array[i + 1] << " " << array[i + 2] << std::endl;
        
        std::cout << str << vIndex << " vn " << array[i + 3] << " " << array[i + 4] << " " << array[i + 5] << std::endl;
    }
}

Mesh::Mesh( const GLchar * path )
{
    std::string str_path( path );
    std::string extension( str_path.substr( str_path.find_last_of( "." ) + 1 ) );
    
    //printf( "before loading mesh file. extension %s\n", extension.c_str() );
    
    MeshLoader * meshLoader = NULL;
    if ( extension == "obj" )
    {
        //printf("went inside the loader\n");
        meshLoader =  new ObjMeshLoader2Passes( path );//new ObjMeshLoader( path );
        
        groups = meshLoader->getGroups();
        
        //        nr_vertices = (GLuint)meshLoader->getVertices()->size();
        //        GLuint nr_faces = (GLuint)meshLoader->getFaces()->size();
        //        nr_normals = (GLuint)meshLoader->getNormals()->size();
        nr_groups = (GLuint)groups->size();
        //        drawCount = (GLuint)meshLoader->getVertexOrder()->size();
        
        
        //printf("# vertices %d\n", nr_vertices);
        //printf("# normals %d\n", nr_normals);
        //        printf("# faces %d\n", nr_faces);
        //printf("# groups %d\n", nr_groups );
        //        SIZE = nr_vertices * 6;
        
        vertices = new GLfloat[ meshLoader->getVertices()->size() * 6 ];
        verticesOrder = new GLuint * [ groups->size() ];
        //        groupIndexOffsets = new GLuint[ meshLoader->getStartOfGroups()->size() ];
        
        printf("# groups %zd\n", nr_groups);
        printf("# vertices %zd\n", meshLoader->getVertices()->size() );
        printf("# normals %zd\n", meshLoader->getNormals()->size() );
        
        Group group;
        for ( int i = 0; i < groups->size(); i++ )
        {
            //printf( "before copying\n" );
            group = groups->at(i);
            //printf( "after copying\n" );
            
            //printf("# indices %zd\n", group.getIndices()->size() );
            
            
            
            
            //std::cout << " group vertices size " << size << std::endl;
            
            
            
            
            verticesOrder[ i ] = new GLuint[ group.getIndices()->size() ];
            
            // vertices...
            size_t size = meshLoader->getVertices()->size();
            SIZE = size * 6;
//            for ( int j = 0; j < size; j++ )
//            {
//                GLuint row = j * 6;
//                
//                glm::vec3 vec = meshLoader->getVertices()->at(j);
//                //vertices[ row ] = vec.x;
//                //vertices[ row + 1 ] = vec.y;
//                //vertices[ row + 2 ] = vec.z;
//            }
            
            // vertices...
            for ( int j = 0; j < group.getIndices()->size(); j++ )
            {
                GLuint index = group.getIndices()->at(j);
                verticesOrder[ i ][ j ] = index;
                
                glm::vec3 vertex = meshLoader->getVertices()->at( index );
                vertices[ index * 6 ] = vertex.x;
                vertices[ index * 6 + 1 ] = vertex.y;
                vertices[ index * 6 + 2 ] = vertex.z;
            }
            
            //faces...
            size = group.getFaces()->size();
            for ( int j = 0; j < size; j++ )
            {
                Face face = group.getFaces()->at(j);
                for ( int p = 0; p < 3; p++ )
                {
                    GLuint row = face.vertexIndices[p] * 6;
                    
                    vertices[ row + 3 ] = face.normal.x;
                    vertices[ row + 4 ] = face.normal.y;
                    vertices[ row + 5 ] = face.normal.z;
                    
                    //printf("index %d, row %d, normal z component %f\n", face.vertexIndices[p], row, vertices[ row + 5]);
                }
            }
            
            //std::cout << "pseudo draw count " << group.getIndices()->size() << std::endl;
            drawCount += group.getIndices()->size();
        }
        
        //        glm::vec3 vec;
        //        for ( int i = 0; i < nr_vertices; i++ )
        //        {
        //            GLuint row = i * 6;
        ////            vec = meshLoader->getVertices()->at(i);
        //            vertices[ row ] = vec.x;
        //            vertices[ row + 1 ] = vec.y;
        //            vertices[ row + 2 ] = vec.z;
        //        }
        
        //        Face face;
        //printf( "faces #%d\n", nr_faces );
        //        for ( int i = 0; i < nr_faces; i++ )
        //        {
        //            face = meshLoader->getFaces()->at(i);
        //            for ( int j = 0; j < 3; j++ )
        //            {
        //                GLuint row = face.vertexIndices[j] * 6;
        //
        //                vertices[ row + 3 ] = face.normal.x;
        //                vertices[ row + 4 ] = face.normal.y;
        //                vertices[ row + 5 ] = face.normal.z;
        //
        //                //printf( "%d face v %f %f %f\n", i, vertices[ row ], vertices[ row + 1 ], vertices[ row + 2 ] );
        //                //printf( "%d face vn %f %f %f\n\n", i, vertices[ row + 3 ], vertices[ row + 4 ], vertices[ row + 5 ] );
        //            }
        //        }
        
        //print( vertices, SIZE, "mesh " );
        
        //        int i = 0;
        //        for ( std::vector<GLuint>::iterator itr = meshLoader->getVertexOrder()->begin(); itr != meshLoader->getVertexOrder()->end(); itr++ )
        //            verticesOrder[ i++ ] = *itr;
        //        i = 0;
        //        for ( std::vector<GLuint>::iterator itr = meshLoader->getStartOfGroups()->begin(); itr != meshLoader->getStartOfGroups()->end(); itr++ )
        //            groupIndexOffsets[ i++ ] = *itr;
        
        isTriangleFaces = meshLoader->getIsTriangleFaces();
    }
    
    if ( !meshLoader )
    {
        std::cout<< "Could not create mesh loader object! " << std::endl;
        return;
    }
    
    if ( !shader->bindShader("../shaders/shader.vs", GL_VERTEX_SHADER) )
    {
        return;
    }
    
    if ( !shader->bindShader("../shaders/shader.fs", GL_FRAGMENT_SHADER) )
    {
        return;
    }
    
    delete meshLoader;
    
    shader->linkProgram();
    
    glGenBuffers(TOTAL_BUFFERS, VBO);
    //nr_groups = 2;
    VAO = new GLuint[ nr_groups ];
    glGenVertexArrays( nr_groups, VAO);
    
    // position
    glBindBuffer(GL_ARRAY_BUFFER, VBO[POSITIONS_BUFF]);
    
    std::cout << "total number of elements " << SIZE << std::endl;
    std::cout << "total size buffer " << (SIZE*sizeof(GLfloat)) << std::endl;
    std::cout << "drawCount " << drawCount << std::endl;
    
//    printf("STARTING vertices printout\n");
//    for ( int i = 0; i < SIZE; i+=6 )
//    {
//        printf("%d v  %f ,%f ,%f\n",i/6, vertices[i], vertices[i + 1], vertices[i + 2]);
//        printf("%d vn %f ,%f ,%f\n", i/6, vertices[i+3], vertices[i + 4], vertices[i + 5]);
//    }
//    printf("END vertices printout\n");
    
    //float * vertexPtr = &vertices[0][0];
    
    //glBufferData(GL_ARRAY_BUFFER, SIZE*sizeof(GLfloat), vertices[0], GL_STATIC_DRAW);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, SIZE*sizeof(GLfloat), vertices[0]);
    glBufferData(GL_ARRAY_BUFFER, SIZE*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
//    
//    GLuint offset = 0;
//    for ( int i = 0; i < nr_groups; i++ )
//    {
//        Group group = groups->at(i);
//        //offset += (GLuint)group.getVertices()->size()*6*sizeof(GLfloat);
//        //std::cout << "offset of buffer " << offset << std::endl;
//        
//        //std::cout << "count of vertices " << group.getVertices()->size() << std::endl;
//        //std::cout << "size of buffer " << group.getVertices()->size()*6*sizeof(GLfloat) << std::endl;
//    
//        glBufferSubData(GL_ARRAY_BUFFER, offset, SIZE*sizeof(GLfloat), vertices );
//        
//        glBindVertexArray(VAO[i]);
//        
//        positionLoc = shader->getAttribute( "position" );
//        glEnableVertexAttribArray(positionLoc);
//        
//        glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)offset );
//        //glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
//        
//        // normals
//        normalLoc = shader->getAttribute( "normal" );
//        glEnableVertexAttribArray(normalLoc);
//        glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)(offset + (3*sizeof(GLfloat))));
//        
//        offset += (GLuint)group.getVertices()->size()*6*sizeof(GLfloat);
//    }
    
//    glBindVertexArray(VAO[0]);
//    
//    positionLoc = shader->getAttribute( "position" );
//    glEnableVertexAttribArray(positionLoc);
//    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), 0);
//    //glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
//    
//    // normals
//    normalLoc = shader->getAttribute( "normal" );
//    glEnableVertexAttribArray(normalLoc);
//    glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    GLuint offset = 0;
    for ( int i = 0; i < nr_groups; i++ )
    {
        glBindVertexArray(VAO[i]);
        
        positionLoc = shader->getAttribute( "position" );
        glEnableVertexAttribArray(positionLoc);
        glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)offset);
        
        // normals
        normalLoc = shader->getAttribute( "normal" );
        glEnableVertexAttribArray(normalLoc);
        glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid*)(offset + 3*sizeof(GLfloat)));
        
        offset += groups->at(i).getIndices()->size()*6*sizeof(GLfloat);
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX_BUFF]);
    
    //std::cout << "total size of index buff " << drawCount*sizeof(GLuint) << std::endl;
    
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, drawCount*sizeof(GLuint), verticesOrder[0], GL_STATIC_DRAW);
    //glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, drawCount*sizeof(GLuint), verticesOrder[0]);
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, drawCount*sizeof(GLuint), 0, GL_STATIC_DRAW);
    
    offset = 0;
    indicesCount = new GLsizei[ nr_groups ];
    for ( int i = 0; i < nr_groups; i++ )
    {
//        std::cout << "offset " << offset << std::endl;
    
            Group group = groups->at(i);
    
//            std::cout << "size of index buffer " << (group.getIndices()->size()*sizeof(GLuint)) << std::endl;
//            std::cout << "number of indices " << group.getIndices()->size() << std::endl;
//    
//            for ( int j = 0; j < group.getIndices()->size(); j++ )
//            {
//                std::cout << verticesOrder[i][j] << ", ";
//            }
//        
//        for ( int j = 0; j < group.getVertices()->size(); j++ )
//            std::cout << group.getVertices()->at(j).x << group.getVertices()->at(j).y << group.getVertices()->at(j).z << std::endl;
//
//            std::cout << std::endl;
    
        indicesCount[i] = (GLsizei)group.getIndices()->size();
        
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, group.getIndices()->size()*sizeof(GLuint), verticesOrder[i]);
        
        offset += (GLuint)group.getIndices()->size()*sizeof(GLuint);
    }
    
    
//    
//    for ( int i = 0; i < nr_groups; i++ )
//    {
//        for ( int j = 0; j < groups->at(i).getIndices()->size(); j++ )
//            std::cout <<  "," << verticesOrder[i][j];
//        
//        std::cout << " count " << indicesCount[i] << std::endl;
//    }
    
    
    
    //std::cout << " before rendering " << std::endl;
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX_BUFF]);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, drawCount*sizeof(GLuint), verticesOrder, GL_STATIC_DRAW);
    
    //print( vertices, nr_vertices, "vertex" );
    //print( normals, nr_normals, "normal" );
    //print( verticesOrder, drawCount, "order" );
    
    //colorLoc = shader->getUniform( "color" );
    uniform_m_transform = shader->getUniform("m_transform");
    
    //printf( "draw cont %d\n", drawCount );
    
}

void Mesh::draw( glm::mat4 m_transform )
{
    //printf("commencing drawing\n");
    shader->use();
    
    //assigning the value to the vertex shader uniform matrix...
    glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
    
    //glUniform4f(colorLoc, sinf(glfwGetTime()*2), 1.0, 1.0, 1.0);
    
    //printf("before rendering\n");
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX_BUFF]);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO[POSITIONS_BUFF]);
    
    
    //printf("after VAO \n");
    //
    
    
    //glMultiDrawElements( GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, (const GLvoid * const *)verticesOrder, nr_groups );
    
    //std::cout << "cout " << indicesCount[0] << std::endl;
    
    if ( !shader->validateProgram() )
    {
        printf( "error with programs" );
        return;
    }
    
    //printf("before draw method\n");
    //glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, NULL );
    //glDrawElements(GL_TRIANGLES, 72, GL_UNSIGNED_INT, NULL);
    //glMultiDrawElements( GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, (const GLvoid * const *)verticesOrder, nr_groups );
    
    GLuint error = glGetError();
    if ( glGetError() != GL_NO_ERROR )
    {
        printf( "error?: %d - %d\n", error, GL_NO_ERROR );
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX_BUFF]);
    //glBindVertexArray( VAO[0] );
    
    GLuint offset = 0;
    //glMultiDrawElements( GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, (const GLvoid * const *)verticesOrder, nr_groups );
    
    glBindVertexArray( VAO[0] );
    //glDrawElements(GL_TRIANGLES, indicesCount[0], GL_UNSIGNED_INT, NULL );
    
    //glBindVertexArray( VAO[1] );
    //glDrawElements(GL_TRIANGLES, indicesCount[1], GL_UNSIGNED_INT, (GLvoid*)(indicesCount[1]*sizeof(GLuint)) );
    for ( int i = 0; i < nr_groups; i++ )
    {
        //glBindVertexArray( VAO[i] );
        
        //std::cout << " indices count " << indicesCount[i] << ", offset " << offset << std::endl;
        glDrawElements(GL_TRIANGLES, (GLuint)indicesCount[i], GL_UNSIGNED_INT, (GLvoid*)offset );
        
        offset += sizeof(GLuint)*indicesCount[i];
    }
    
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    error = glGetError();
    if ( glGetError() != GL_NO_ERROR )
    {
        printf( "error?: %d - %d\n", error, GL_NO_ERROR );
    }
}

void Mesh::transform( glm::mat4 m_transform )
{
    
}
