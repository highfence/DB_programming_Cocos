
#include "stdafx.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "windowRenderer.h"


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	m_pWindowRenderer = WindowRenderer::create();
	m_pWindowRenderer->DrawBasicWindow();
	addChild(m_pWindowRenderer);

    return true;
}

