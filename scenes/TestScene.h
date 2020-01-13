//
//  TestScene.h
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 7/1/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#ifndef __OpenGLOtherTest__TestScene__
#define __OpenGLOtherTest__TestScene__

#include "Scene.h"

class TestScene : public Scene
{
public:
    
    TestScene() {
        prepareScene();
    }
    
    void prepareScene();
    
    ~TestScene()
    {
        
    }
};

#endif /* defined(__OpenGLOtherTest__TestScene__) */
