//
//  Window.cpp
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 7/1/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#include "Window.h"

Window::Window( GLint width, GLint height, std::string windowTitle )
{
    GLint glfwStatus = glfwInit();
    if ( glfwStatus != GL_TRUE )
    {
        std::cout << "glfw failed! :(" << std::endl;
        throw std::runtime_error("glfwInit failed");
    }
    
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    handle = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);
    if (!handle)
    {
        std::cout << "failed to create window! :(" << std::endl;
        glfwTerminate();
        throw std::runtime_error("failed to create window!");
    }
    
    glfwMakeContextCurrent(handle);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress); // add GLAD init
    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_VENDOR) << std::endl;
    std::cout << glGetString(GL_RENDERER) << std::endl; 
    
    // disable cursor and set mouse in center view...
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(handle, 0, 0);
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_BACK);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //    render_function = func_ptr;
}

GLFWwindow * Window::getHandle() const
{
    return handle;
}

void Window::setScene( Scene * pScene )
{
    scene = pScene;
}

void Window::runRenderLoop()
{
    const float INITIAL_FOV = 45.0f;
    float mouseSpeed = 400;
    float horizontalAngle = 0;
    float verticalAngle = 0;
    float lastTime = glfwGetTime();
    float deltaTime = 0;
    float movementSpeed = 10000.0f;
    glm::vec3 position(0.0, 2.0, -10.0);
    
    while (!glfwWindowShouldClose(handle))
    {
        deltaTime = float(glfwGetTime() - lastTime);
        
        GLint viewPortWidth, viewPortHeight;
        glfwGetFramebufferSize(handle, &viewPortWidth, &viewPortHeight);
        glViewport(0, 0, viewPortWidth, viewPortHeight);
        // clear color and depth...
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        // rotation calculation...
        float move = 0; //sinf(glfwGetTime() * ((4*3.14) / 5) * 0); // -1<->+1 every 5 seconds
        float angle = glfwGetTime() * 45 * 0;  // 45° per second
        
        glm::vec3 axis_y(0.0, 1.0, 0.0);
        //glm::mat4 anim = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0)) * glm::rotate(glm::mat4(1.0f), angle, axis_y);
        
        glm::vec3 axis_z(0, 0, 1);
        
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
        
        glm::mat4 anim = glm::translate(glm::mat4(1.0f), glm::vec3(move, 0.0, 0.0)) * glm::rotate(glm::mat4(1.0f), angle, axis_y);
        
        // camera movement...
        double mousePosX, mousePosY;
        glfwGetCursorPos(handle, &mousePosX, &mousePosY);
        
        horizontalAngle += mouseSpeed * deltaTime * float(viewPortWidth/2 - mousePosX );
        verticalAngle   += mouseSpeed * deltaTime * float( viewPortHeight/2 - mousePosY );
    
        glm::vec3 direction(std::cos(verticalAngle) * std::sin(horizontalAngle),
                            std::sin(verticalAngle),
                            std::cos(verticalAngle) * std::cos(horizontalAngle));
        
        glm::vec3 right = glm::vec3(sin(horizontalAngle - 3.14f/2.0f),
                                    0,
                                    cos(horizontalAngle - 3.14f/2.0f));
        
        glm::vec3 up = glm::cross( right, direction );
        
        // reset cursor...
        glfwSetCursorPos(handle, viewPortWidth/2, viewPortHeight/2);
        
        
        // get camera positionign with keys...
        if ( glfwGetKey(handle, GLFW_KEY_W))
        {
            position += direction * deltaTime * movementSpeed;
        }
        else if ( glfwGetKey(handle, GLFW_KEY_S))
        {
            position -= direction * deltaTime * movementSpeed;
        }
        
        if ( glfwGetKey(handle, GLFW_KEY_A))
        {
            position -= right * deltaTime * movementSpeed;
        }
        else if ( glfwGetKey(handle, GLFW_KEY_D))
        {
            position += right * deltaTime * movementSpeed;
        }
        
        //glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 view = glm::lookAt(position, position+direction, up);
        
        //double xoffset, yoffset;
        float fov = INITIAL_FOV; // - 5 * glfwSetScrollCallback(handle, &xoffset, &yoffset);
        glm::mat4 projection = glm::perspective(fov, 1.0f*viewPortWidth/viewPortHeight, 0.1f, 10000.0f);
        
        //glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
        //glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
        //glm::mat4 projection = glm::perspective(45.0f, 1.0f*viewPortWidth/viewPortHeight, 0.1f, 10.0f);
        
        glm::mat4 m_transform = projection * view * model * anim;
        
        std::vector<Shape*> objects = scene->getObjects();
        for ( std::vector<Shape*>::iterator it = objects.begin(); it != objects.end(); it++ )
        {
            (*it)->draw( m_transform );
        }
        
        //printf("after the thing\n");
        
        glfwPollEvents();
        glfwSwapBuffers(handle);
        
        lastTime = glfwGetTime();
        
        //exit program if escape key is pressed
        if(glfwGetKey(handle, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(handle, GL_TRUE);
        
//        if ( glGetError() != GL_NO_ERROR )
//        {
//            printf( "10 error?: %d - %d\n", glGetError(), GL_NO_ERROR );
//        }
//        
//        if ( glGetError() == GL_INVALID_OPERATION )
//        {
//            printf("invalid operation!\n");
//        }
//        else if ( glGetError() != GL_NO_ERROR )
//        {
//            printf( "some other eroor occured: %d", glGetError() );
//        }
    }
}