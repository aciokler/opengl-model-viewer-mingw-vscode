//
//  Shader.cpp
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 7/1/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#include "Shader.h"

bool Shader::bindShader( const GLchar *path, GLenum shaderType )
{
    std::ifstream fin;
    fin.open(path);
    if ( !fin.is_open() )
    {
        fin.close();
        std::cout << "Error opening file: '" << path << "'!" << std::endl;
        
        return false;
    }
    
    std::string source((std::istreambuf_iterator<GLchar>(fin)), std::istreambuf_iterator<GLchar>());
    
    //std::cout << source << std::endl;
    
    fin.close();
    
    // create shader
    GLuint handle = glCreateShader(shaderType);
    
    const GLchar * shaderSource = source.c_str();
    glShaderSource(handle, 1, &shaderSource, NULL);
    
    glCompileShader(handle);
    
    GLint compileStatus;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &compileStatus);
    if ( compileStatus != GL_TRUE )
    {
        std::cout << "Failed to compile shader: '" << path << "'!" << std::endl;
        
        GLint infoLogLength;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *infoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(handle, infoLogLength + 1, NULL, infoLog);
        std::cout << infoLog << std::endl;
        delete[] infoLog;
        
        glDeleteShader(handle);
        
        return false;
    }
    
    glAttachShader(program, handle);
    
    if ( GL_VERTEX_SHADER == shaderType )
        shaders[ VERTEX_SHADER ] = handle;
    else if ( GL_FRAGMENT_SHADER == shaderType )
        shaders[ FRAGMENT_SHADER ] = handle;
    
    return true;
}

GLint Shader::getAttribute(const GLchar* attribName) const {
    if(!attribName)
        throw std::runtime_error("attribName was NULL");
    
    GLint attrib = glGetAttribLocation(program, attribName);
    if(attrib == -1)
        throw std::runtime_error(std::string("Program attribute not found: ") + attribName);
    
    return attrib;
}

GLint Shader::getUniform(const GLchar* uniformName) const {
    if(!uniformName)
        throw std::runtime_error("uniformName was NULL");
    
    GLint uniform = glGetUniformLocation(program, uniformName);
    if(uniform == -1)
        throw std::runtime_error(std::string("Program uniform not found: ") + uniformName);
    
    return uniform;
}

void Shader::linkProgram()
{
    glLinkProgram(program);
    
    // detach all shaders...
    for ( int i = 0; i < MAX_NR_SHADERS; i++)
        glDetachShader( program, shaders[ i ] );
    
    //throw exception if linking failed
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        std::string msg("Program linking failure: ");
        
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;
        
        glDeleteProgram(program); program = 0;
        throw std::runtime_error(msg);
    }
}