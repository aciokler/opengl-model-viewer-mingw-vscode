//
//  ObjMeshExperiment.cpp
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 6/29/15.
//  Copyright (c) 2015 Abraham-mac. All rights reserved.
//

#include "ObjMeshExperiment.h"
#include "../meshloader/ObjMeshLoaderExperiment.h"
#include "../meshloader/ObjMeshLoader2Passes.h"
#include "../meshloader/ObjMeshLoaderFasterExperiment.h"
#include "../meshloader/ObjMeshLoaderSmoothNormalsExperiment.h"
#include "../meshloader/ObjMeshLoaderQuadsExperiment.h"
#include "../meshloader/ObjMeshLoaderLatestFixingBugs.h"
#include "../meshloader/ObjMeshLoaderMilestone1.h"

void ObjMeshExperiment::initWithObjects( const std::vector<glm::vec3> * meshVertices, const std::vector<glm::vec3> * meshNormals )
{
    Object obj;
    Group group;
    
    for ( int j = 0; j < objects->size(); j++ )
    {
        obj = objects->at(j);
        
        nr_groups = (GLuint)groups->size();
        
        vertices = new GLfloat[ meshVertices->size() * BUFFER_OFFSET_FACTOR ];
        verticesOrder = new GLuint * [ obj.getGroups()->size() ];
        
        for ( int i = 0; i < obj.getGroups()->size(); i++ )
        {
            group = obj.getGroups()->at(i);
            //verticesOrder[ i ] = new GLuint[ group.getIndices()->size() ];
            
            // vertices...
            for ( int j = 0; j < group.getIndices()->size(); j++ )
            {
                GLuint index = group.getIndices()->at(j);
                verticesOrder[ i ][ j ] = index;
                
                glm::vec3 vertex = meshVertices->at( index );
                vertices[ index * BUFFER_OFFSET_FACTOR ] = vertex.x;
                vertices[ index * BUFFER_OFFSET_FACTOR + 1 ] = vertex.y;
                vertices[ index * BUFFER_OFFSET_FACTOR + 2 ] = vertex.z;
            }
            
            //faces...
            size_t size = group.getFaces()->size();
            for ( int j = 0; j < size; j++ )
            {
                Face face = group.getFaces()->at(j);
                for ( int p = 0; p < 3; p++ )
                {
                    GLuint row = face.vertexIndices[p] * BUFFER_OFFSET_FACTOR;
                    
                    vertices[ row + 3 ] = face.normal.x;
                    vertices[ row + 4 ] = face.normal.y;
                    vertices[ row + 5 ] = face.normal.z;
                }
            }
            
            drawCount += group.getIndices()->size();
        }
    }
}

void ObjMeshExperiment::initWithGroups( const std::vector<glm::vec3> * meshVertices, const std::vector<glm::vec3> * meshNormals )
{
    nr_groups = (GLuint)groups->size();
    
    vertices = new GLfloat[ meshVertices->size() * BUFFER_OFFSET_FACTOR ];
    //verticesOrder = new GLuint * [ groups->size() ];
    SIZE = (GLuint)meshVertices->size()*BUFFER_OFFSET_FACTOR;
    
    Group group;
    size_t count = 0;
    for ( int i = 0; i < groups->size(); i++ )
    {
        count += groups->at(i).getIndices()->size();
    }
    
    elements = new GLuint[count];
    
    for ( int i = 0; i < groups->size(); i++ )
    {
        group = groups->at(i);
        //verticesOrder[ i ] = new GLuint[ group.getIndices()->size() ];
        
//        // vertices...
//        for ( int j = 0; j < group.getIndices()->size(); j++ )
//        {
//            GLuint index = group.getIndices()->at(j);
//            verticesOrder[ i ][ j ] = index;
//            
//            glm::vec3 vertex = meshVertices->at( index );
//            vertices[ index * BUFFER_OFFSET_FACTOR ] = vertex.x;
//            vertices[ index * BUFFER_OFFSET_FACTOR + 1 ] = vertex.y;
//            vertices[ index * BUFFER_OFFSET_FACTOR + 2 ] = vertex.z;
//        }
        
        //std::cout << "group: " << i << std::endl;
        
        //faces...
        int vertexCount = 0;
        size_t size = group.getFaces()->size();
        for ( int j = 0; j < size; j++ )
        {
            //std::cout << "face: " << j << std::endl;
            
            Face face = group.getFaces()->at(j);
            for ( int p = 0; p < 3; p++, vertexCount++ )
            {
                GLuint row = face.vertexIndices[p] * BUFFER_OFFSET_FACTOR;
                
                //std::cout << "vertex: " << vertexCount << ", vertex component: " << p << std::endl;
                
                elements[drawCount + vertexCount] = face.vertexIndices[p];
                //verticesOrder[ i ][ vertexCount ] = face.vertexIndices[p];
                //std::cout << "size: " << meshVertices->size() << ", index: " << face.vertexIndices[p] << std::endl;
                glm::vec3 vertex = meshVertices->at( face.vertexIndices[p] );
                
                vertices[ row ] = vertex.x;
                vertices[ row + 1 ] = vertex.y;
                vertices[ row + 2 ] = vertex.z;
                
                //glm::vec3 normal = meshNormals->at(face.normalInd);
                //glm::vec3 normal = (sizeof(face.vertexNormalIndices) / sizeof(*face.vertexNormalIndices) == 1) ? meshNormals->at(0) : meshNormals->at(face.vertexNormalIndices[p]);
                glm::vec3 normal = meshNormals->at(face.vertexNormalIndices[p]);
                vertices[ row + 3 ] = normal.x;
                vertices[ row + 4 ] = normal.y;
                vertices[ row + 5 ] = normal.z;
//                vertices[ row + 3 ] = face.normal.x;
//                vertices[ row + 4 ] = face.normal.y;
//                vertices[ row + 5 ] = face.normal.z;
            }
        }
        drawCount += group.getIndices()->size();
    }
}

void ObjMeshExperiment::prepareBuffers()
{
    shader->linkProgram();
    
    glGenBuffers(TOTAL_BUFFERS, VBO);
    VAO = new GLuint[ nr_groups ];
    positionLoc2 = new GLint[nr_groups];
    normalLoc2 = new GLint[nr_groups];
    
    
    
    std::cout << "total number of elements " << SIZE << std::endl;
    std::cout << "total size buffer " << (SIZE*sizeof(GLfloat)) << std::endl;
    std::cout << "drawCount " << drawCount << std::endl;
    
    //glGenVertexArrays( nr_groups, VAO);
    glGenVertexArrays(1, VAO);
    
    // position
    glBindBuffer(GL_ARRAY_BUFFER, VBO[POSITIONS_BUFF]);
    glBufferData(GL_ARRAY_BUFFER, SIZE*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    
    GLuint offset = 0;
    for ( int i = 0; i < 1; i++ )//for ( int i = 0; i < nr_groups; i++ )
    {
        //glBindBuffer(GL_ARRAY_BUFFER, VBO[POSITIONS_BUFF]);
        glBindVertexArray(VAO[i]);
        
        // vertex
        positionLoc2[i] = shader->getAttribute( "position" );
        glEnableVertexAttribArray(positionLoc2[i]);
        glVertexAttribPointer(positionLoc2[i], 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET_FACTOR*sizeof(GLfloat), (GLvoid*)offset);
        
        // normals
        normalLoc2[i] = shader->getAttribute( "normal" );
        glEnableVertexAttribArray(normalLoc2[i]);
        glVertexAttribPointer(normalLoc2[i], 3, GL_FLOAT, GL_FALSE, BUFFER_OFFSET_FACTOR*sizeof(GLfloat), (GLvoid*)(offset + 3*sizeof(GLfloat)));
        
        offset += groups->at(i).getIndices()->size()*BUFFER_OFFSET_FACTOR*sizeof(GLfloat);
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX_BUFF]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, drawCount*sizeof(GLuint), elements, GL_STATIC_DRAW);
    
    offset = 0;
    indicesCount = new GLsizei[ nr_groups ];
    for ( int i = 0; i < nr_groups; i++ )
    {
        Group group = groups->at(i);
        indicesCount[i] = (GLsizei)group.getIndices()->size();
        
        //glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, group.getIndices()->size()*sizeof(GLuint), verticesOrder[i]);
        
        //offset += (GLuint)group.getIndices()->size()*sizeof(GLuint);
    }
    
    uniform_m_transform = shader->getUniform("m_transform");
}

ObjMeshExperiment::ObjMeshExperiment( const GLchar * path )
{
    std::string str_path( path );
    std::string extension( str_path.substr( str_path.find_last_of( "." ) + 1 ) );
    
    MeshLoader * meshLoader = NULL;
    //ObjMeshLoaderExperiment * meshLoader = NULL;
    //ObjMeshLoader2Passes * meshLoader = NULL;
    if ( extension == "obj" )
    {
        //meshLoader =  new ObjMeshLoader2Passes( path );
        //meshLoader = new ObjMeshLoaderExperiment( path );
        //meshLoader = new ObjMeshLoaderFasterExperiment( path );
        //meshLoader = new ObjMeshLoaderSmoothNormalsExperiment( path );
        //meshLoader = new ObjMeshLoaderQuadsExperiment( path );
        //meshLoader = new ObjMeshLoaderMilestone1( path );
        meshLoader = new ObjMeshLoaderLatestFixingBugs( path );
        
        //objects = meshLoader->getObjects();
        //if ( objects )
        //    initWithObjects(meshLoader->getVertices(), meshLoader->getNormals());
        groups = meshLoader->getGroups();
        //if ( groups )
        initWithGroups(meshLoader->getVertices(), meshLoader->getNormals());
        
        printf("# groups %zd\n", nr_groups);
        printf("# vertices %zd\n", meshLoader->getTotalVertices() );
        printf("# normals %zd\n", meshLoader->getTotalNormals() );
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
    
//    if ( !shader->bindShader("../shaders/regulargeometry.gs", GL_GEOMETRY_SHADER) )
//    {
//        return;
//    }
    
    if ( !shader->bindShader("../shaders/normals.gs", GL_GEOMETRY_SHADER) )
    {
        return;
    }
    
    if ( !shader->bindShader("../shaders/shader.fs", GL_FRAGMENT_SHADER) )
    {
        return;
    }
    
    delete meshLoader;
    
    prepareBuffers();
    cleanUp();
}

void ObjMeshExperiment::draw( glm::mat4 m_transform )
{
    shader->use();
    
    //assigning the value to the vertex shader uniform matrix...
    glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
    
    if ( !shader->validateProgram() )
    {
        printf( "error with programs" );
        return;
    }
    
    GLuint error = glGetError();
    if ( glGetError() != GL_NO_ERROR )
    {
        printf( "error?: %d - %d\n", error, GL_NO_ERROR );
    }
//
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX_BUFF]);
    glBindVertexArray( VAO[0] );
    
    //glEnable(GL_PRIMITIVE_RESTART);
    
    GLuint offset = 0;
    GLuint index = 0;
    for ( int i = 0; i < nr_groups; i++ )
    {
        //glPrimitiveRestartIndex(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX_BUFF]);
        //glBindVertexArray( VAO[0] );
        
        //printf("indicesCount %d\n", indicesCount[i]);
        //glDrawElementsBaseVertex(GL_TRIANGLES, (GLuint)indicesCount[i], GL_UNSIGNED_INT, 0, offset);
        //index += indicesCount[i];
        glDrawElements(GL_TRIANGLES, (GLuint)indicesCount[i], GL_UNSIGNED_INT, (GLvoid*)offset);
        offset += sizeof(GLuint)*indicesCount[i];
    }
    
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
    //glBindVertexArray( NULL );
    
    //glDisable(GL_PRIMITIVE_RESTART);
    
    error = glGetError();
    if ( glGetError() != GL_NO_ERROR )
    {
        printf( "error?: %d - %d\n", error, GL_NO_ERROR );
    }
}

void ObjMeshExperiment::transform( glm::mat4 m_transform )
{
    
}
