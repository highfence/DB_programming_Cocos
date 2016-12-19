#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class WindowRenderer;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    
    CREATE_FUNC(HelloWorld);

	WindowRenderer* m_pWindowRenderer;
};

#endif // __HELLOWORLD_SCENE_H__
