//
//  Scene.h
//  OpenGLOtherTest
//
//  Created by Abraham-mac on 7/1/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#ifndef OpenGLOtherTest_Scene_h
#define OpenGLOtherTest_Scene_h

#include "../common/declarations.h"
#include "../shapes/Shape.h"

class Scene
{
public:
    
    Scene()
    {

    }
    
    virtual void prepareScene() = 0;
    
    virtual std::vector<Shape*> getObjects() const
    {
        return objects;
    }
    
    virtual ~Scene()
    {
        printf("calling Scene destructor\n");
        for ( std::vector<Shape*>::iterator it = objects.begin(); it != objects.end(); it++ )
        {
            delete *it;
        }
        printf("done with scene\n");
    }
    
protected:
    
    std::vector<Shape*> objects;
};

#endif
