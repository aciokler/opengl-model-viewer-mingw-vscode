//
//  Window.h
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 7/1/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#ifndef __OpenGLOtherTest__Window__
#define __OpenGLOtherTest__Window__

#include "common/declarations.h"
#include "scenes/Scene.h"

class Window
{
public:
    
    Window( GLint width, GLint height, std::string windowTitle );
    virtual ~Window()
    {
        printf("deleting the window\n");
        glfwDestroyWindow(handle);
        glfwTerminate();
        
        delete scene;
    }
    
    void getSomething() {
        
    }
    
    GLFWwindow * getHandle() const;
    
    //    void setRenderFunction( void (*func_ptr)(void) )
    //    {
    //        render_function = func_ptr;
    //    }
    
    void runRenderLoop();
    
    void setScene( Scene * pScene );
    
    glm::mat4 getProjectionMatrix() const
    {
        return projection;
    }
    
private:
    
    //void (*render_function)( glm::mat4 ) = NULL;
    GLFWwindow * handle;
    
    Scene * scene = NULL;
    
    glm::mat4 projection;
};

#endif /* defined(__OpenGLOtherTest__Window__) */
