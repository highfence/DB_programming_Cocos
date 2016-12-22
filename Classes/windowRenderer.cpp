#include "stdafx.h"
#include "windowRenderer.h"
#include "TabAccount.h"
#include "TabCharacter.h"


bool WindowRenderer::init()
{
	if (!Node::init())
	{
		return false;
	}

	setCurrentTabNum(TAB_NUM::NONE);
	DrawBasicWindow();
	DrawTabs();
	DrawCurrentTab();

	// Tab setting
	m_pTabAccount = TabAccount::create();
	m_pTabAccount->setVisible(false);
	addChild(m_pTabAccount);

	m_pTabCharacter = TabCharacter::create();
	m_pTabCharacter->setVisible(false);
	addChild(m_pTabCharacter);

	scheduleUpdate();
	return true;
}

void WindowRenderer::update(const float dt)
{
	if (CheckCallFunctions())
	{
		char buf[10];
		sprintf(buf, "%d", m_pTabAccount->getCallCharacterId());
		m_pTabCharacter->Character_id = buf;
		m_pTabCharacter->setVisible(true);
		m_pTabCharacter->setIsCharacterTabCalled(true);
		m_pTabCharacter->GetTextFromEditBox(m_pTabCharacter);
		m_pTabAccount->setCallFlag(false);
	}
	return;
}

bool WindowRenderer::DrawBasicWindow()
{
	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const auto origin = Director::getInstance()->getVisibleOrigin();

	// backgrond
	auto Background = Sprite::create("Background.png");
	Background->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.5f));
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

	return true;
}

bool WindowRenderer::DrawTabs()
{
	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const auto origin = Director::getInstance()->getVisibleOrigin();
	m_pAccountTab = MenuItemImage::create("Tabs/Account_Normal.png", "Tabs/Account_Selected.png", CC_CALLBACK_1(WindowRenderer::changeToAccountTab, this));
	m_pCharacterTab = MenuItemImage::create("Tabs/Character_Normal.png", "Tabs/Character_Selected.png", CC_CALLBACK_1(WindowRenderer::changeToCharacterTab, this));
	m_pItemTab = MenuItemImage::create("Tabs/Item_Normal.png", "Tabs/Item_Selected.png", CC_CALLBACK_1(WindowRenderer::changeToItemTab, this));
	m_pDungeonTab = MenuItemImage::create("Tabs/Dungeon_Normal.png", "Tabs/Dungeon_Selected.png", CC_CALLBACK_1(WindowRenderer::changeToDungeonTab, this));

	float h = visibleSize.height - 71;
	float TabX = origin.x + m_pAccountTab->getContentSize().width / 2;

	m_pAccountTab->setPosition(Vec2(TabX, h));
	m_pCharacterTab->setPosition(Vec2(m_pAccountTab->getPosition().x + 163, h));
	m_pItemTab->setPosition(Vec2(m_pCharacterTab->getPosition().x + 163, h));
	m_pDungeonTab->setPosition(Vec2(m_pItemTab->getPosition().x + 163, h));

	m_pTabMenu = Menu::create(m_pAccountTab, m_pCharacterTab, m_pItemTab, m_pDungeonTab,  NULL);
	m_pTabMenu->setPosition(Vec2::ZERO);
	addChild(m_pTabMenu, 1);

	return true;
}

bool WindowRenderer::DrawCurrentTab()
{
	if (getCurrentTabNum() == TAB_NUM::ACCOUNT)
	{
		m_pTabAccount->setVisible(true);
	}
	if (getCurrentTabNum() == TAB_NUM::CHARACTER)
	{
		m_pTabCharacter->setVisible(true);
	}

	return true;
}

void WindowRenderer::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
}

void WindowRenderer::changeToAccountTab(Ref * pSender)
{
	m_pTabAccount->setVisible(false);
	m_pTabCharacter->setVisible(false);
	setCurrentTabNum(TAB_NUM::ACCOUNT);
	DrawCurrentTab();
}

void WindowRenderer::changeToCharacterTab(Ref * pSender)
{
	m_pTabAccount->setVisible(false);
	m_pTabCharacter->setVisible(false);
	setCurrentTabNum(TAB_NUM::CHARACTER);
	DrawCurrentTab();
}

void WindowRenderer::changeToItemTab(Ref * pSender)
{
	setCurrentTabNum(TAB_NUM::ITEM);
	DrawCurrentTab();
}

void WindowRenderer::changeToDungeonTab(Ref * pSender)
{
	setCurrentTabNum(TAB_NUM::DUNGEON);
	DrawCurrentTab();
}

bool WindowRenderer::CheckCallFunctions()
{
	if (m_pTabAccount->getCallFlag())
	{
		return true;
	}

	return false;
}
