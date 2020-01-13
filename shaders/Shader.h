//
//  Shader.h
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 7/1/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#ifndef __OpenGLOtherTest__Shader__
#define __OpenGLOtherTest__Shader__

#include "../common/declarations.h"

typedef enum
{
    VERTEX_SHADER,
    GEOMETRY_SHADER,
    FRAGMENT_SHADER
} ShaderTypes;

class Shader
{
public:
    
    Shader()
    {
        // initialize the shaders to zero and create the program
        for ( int i = 0; i < MAX_NR_SHADERS; i++)
            shaders[ i ] = 0;
        
        program = glCreateProgram();
    }
    
    virtual ~Shader()
    {
        printf("deleting shader\n");
        for ( int i = 0; i < MAX_NR_SHADERS; i++)
            glDeleteShader( shaders[ i ] );
        glDeleteProgram(program);
    }
    
    bool bindShader( const GLchar *path, GLenum shaderType );
    
    void linkProgram();
    
    void use()
    {
        glUseProgram(program);
    }
    
    bool validateProgram()
    {
        glValidateProgram(program);
        GLint valid;
        glGetProgramiv(program, GL_VALIDATE_STATUS, &valid);
        
        if ( valid == GL_FALSE )
        {
            printf("found an error in progams");
            
            GLint logLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength );
            
            GLchar * infoLog = new GLchar[ logLength + 1 ];
            glGetProgramInfoLog(program, logLength, NULL, infoLog );
            
            std::string msg("Compile failure in shader:\n");
            msg += infoLog;
            
            delete[] infoLog;
            throw std::runtime_error(msg);
        }
        
        return valid;
    }
    
    GLint getAttribute(const GLchar* attribName) const;
    
    GLint getUniform(const GLchar* uniformName) const;
    
private:
    
    const int MAX_NR_SHADERS = FRAGMENT_SHADER + 1;
    GLuint * shaders = new GLuint[ MAX_NR_SHADERS ];
    GLuint program;
    
};

#endif /* defined(__OpenGLOtherTest__Shader__) */
