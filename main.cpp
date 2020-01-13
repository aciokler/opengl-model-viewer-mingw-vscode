//
//  main.cpp
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 6/29/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#include "common/declarations.h"
#include "Window.h"
#include "scenes/TestScene.h"

int main(int argc, const char * argv[])
{
    Window* window = new Window( 800, 640, "Title test" );
    window->setScene(new TestScene());
    window->runRenderLoop();
    //TestScene* scene = new TestScene();
    //delete scene;
    delete window;
    
    return 0;
}
