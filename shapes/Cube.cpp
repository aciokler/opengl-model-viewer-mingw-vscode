//
//  Cube.cpp
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 7/1/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#include "Cube.h"

Cube::Cube()
{
    if ( !shader->bindShader("../shaders/shader.vs", GL_VERTEX_SHADER) )
    {
        return;
    }
    
    if ( !shader->bindShader("../shaders/normals.gs", GL_GEOMETRY_SHADER) )
    {
        return;
    }
    
    if ( !shader->bindShader("../shaders/shader.fs", GL_FRAGMENT_SHADER) )
    {
        return;
    }
    
    if ( glGetError() != GL_NO_ERROR )
    {
        printf( "1 error?: %d - %d", glGetError(), GL_NO_ERROR );
    }
    
    shader->linkProgram();
    
    if ( glGetError() != GL_NO_ERROR )
    {
        printf( "2 error?: %d - %d", glGetError(), GL_NO_ERROR );
    }
    

    glGenBuffers(TOTAL_BUFFERS, VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO[ POSITIONS_BUFF ] );
    
    GLfloat bufferData[] =
    {
        //  0.5, 0.5, 0.0,
        // -0.5, 0.5, 0.0,
        // -0.5,-0.5, 0.0,
        //  0.5,-0.5, 0.0
        
        // front
        -1.0, -1.0,  1.0,
        1.0, -1.0,  1.0,
        1.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,
        // back
        -1.0, -1.0, -1.0,
        1.0, -1.0, -1.0,
        1.0,  1.0, -1.0,
        -1.0,  1.0, -1.0,
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(bufferData), bufferData, GL_STATIC_DRAW);
    
    VAO = new GLuint[1];
    glGenVertexArrays(1, VAO);
    glBindVertexArray( VAO[0] );
    
    GLint positionLoc = shader->getAttribute("position");
    
    glEnableVertexAttribArray(positionLoc);
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO[COLOR_BUFF]);
    
    GLfloat cube_colors[] = {
        // front colors
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0,
        // back colors
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0,
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
    
    GLint vertexColorLoc = shader->getAttribute("v_color");
    
    glEnableVertexAttribArray(vertexColorLoc);
    glVertexAttribPointer(vertexColorLoc, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), NULL);
    
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    glBindVertexArray(NULL);
    
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX_BUFF] );
    
    GLushort indicesData[] =
    {
        //0, 1, 2,
        //0, 2, 3
        
        // front
        0, 1, 2,
        2, 3, 0,
        // top
        3, 2, 6,
        6, 7, 3,
        // back
        7, 6, 5,
        5, 4, 7,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // left
        4, 0, 3,
        3, 7, 4,
        // right
        1, 5, 6,
        6, 2, 1,
    };
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesData), indicesData, GL_STATIC_DRAW);
    
    if ( glGetError() != GL_NO_ERROR )
    {
        printf( "3 error?: %d - %d", glGetError(), GL_NO_ERROR );
    }
    
    //GLint colorLoc = shader->getUniform("color");
    //GLint uniform_m_transform = shader->getUniform("m_transform");
}

void Cube::draw( glm::mat4 m_transform )
{
    if ( glGetError() != GL_NO_ERROR )
    {
        printf( "6 error?: %d - %d", glGetError(), GL_NO_ERROR );
    }
    
    shader->use();
    
    if ( glGetError() != GL_NO_ERROR )
    {
        printf( "7 error?: %d - %d", glGetError(), GL_NO_ERROR );
    }
    
    // rotation calculation...
    float move = sinf(glfwGetTime() * (4*3.14) / 5); // -1<->+1 every 5 seconds
    float angle = glfwGetTime() * 45;  // 45° per second
    
    if ( glGetError() != GL_NO_ERROR )
    {
        printf( "7.2 error?: %d - %d", glGetError(), GL_NO_ERROR );
    }
    
    //glm::vec3 axis_y(0.0, 1.0, 0.0);
    //glm::mat4 anim = glm::translate(glm::mat4(1.0f), glm::vec3(move, 0.0, 0.0)) * glm::rotate(glm::mat4(1.0f), angle, axis_y);
    
    //glm::vec3 axis_z(0, 0, 1);
    
    //glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
    //glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
    //glm::mat4 projection = glm::perspective(45.0f, 1.0f*viewPortWidth/viewPortHeight, 0.1f, 10.0f);
    
    //glm::mat4 m_transform = glm::translate(glm::mat4(1.0f), glm::vec3(move, 0.0, 0.0)) * glm::rotate(glm::mat4(1.0f), angle, axis_z);
    
    //glm::mat4 m_transform = projection * view * model * anim;
    
    //assigning the value to the vertex shader uniform matrix...
    glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
    
    if ( glGetError() != GL_NO_ERROR )
    {
        printf( "7.2 error?: %d - %d", glGetError(), GL_NO_ERROR );
    }
    
    //glUniform4f(colorLoc, sinf(glfwGetTime()*2), 0.0, 1.0, 1.0);
    
    if ( glGetError() != GL_NO_ERROR )
    {
        printf( "7.3 error?: %d - %d", glGetError(), GL_NO_ERROR );
    }
    
    glBindVertexArray( VAO[ 0 ] );
    
    if ( glGetError() != GL_NO_ERROR )
    {
        printf( "7.4 error?: %d - %d", glGetError(), GL_NO_ERROR );
    }
    
    if ( glGetError() != GL_NO_ERROR )
    {
        printf( "7.5 error?: %d - %d", glGetError(), GL_NO_ERROR );
    }
    
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX_BUFF] );
    int size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    
    if ( glGetError() != GL_NO_ERROR )
    {
        printf( "7.7 error?: %d - %d", glGetError(), GL_NO_ERROR );
    }
    
    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
    
    if ( glGetError() != GL_NO_ERROR )
    {
        printf( "7.8 error?: %d - %d", glGetError(), GL_NO_ERROR );
    }
    
    glBindVertexArray(NULL);
    
    if ( glGetError() != GL_NO_ERROR )
    {
        printf( "8 error?: %d - %d", glGetError(), GL_NO_ERROR );
    }
}

void Cube::transform( glm::mat4 m_transform )
{
    //assigning the value to the vertex shader uniform matrix...
    glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
}
