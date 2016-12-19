#include "stdafx.h"
#include "windowRenderer.h"

bool WindowRenderer::init()
{
	if (!Node::init())
	{
		return false;
	}

	scheduleUpdate();
	return true;
}

bool WindowRenderer::DrawBasicWindow()
{

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// backgrond
	auto Background = Sprite::create("Background.png");
	Background->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f));
	addChild(Background);

	// closebutton
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseNormal.png",
		CC_CALLBACK_1(WindowRenderer::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2 - 7.5f,
		origin.y + visibleSize.height - closeItem->getContentSize().height / 2 - 7.5f));


	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return false;
}

void WindowRenderer::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
}
