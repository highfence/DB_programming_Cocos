#include "stdafx.h"
#include "windowRenderer.h"
#include "TabAccount.h"
#include "TabCharacter.h"
#include "TabItem.h"

// 윈도우 렌더러 초기화 클래스.
bool WindowRenderer::init()
{
	if (!Node::init())
	{
		return false;
	}

	// Basic Setting And Draw.
	setCurrentTabNum(TAB_NUM::NONE);
	DrawBasicWindow();
	DrawTabs();

	// Tab setting
	m_pTabAccount = TabAccount::create();
	m_pTabAccount->setVisible(false);
	addChild(m_pTabAccount);

	m_pTabCharacter = TabCharacter::create();
	m_pTabCharacter->setVisible(false);
	addChild(m_pTabCharacter);

	m_pTabItem = TabItem::create();
	m_pTabItem->setVisible(false);
	addChild(m_pTabItem);

	DrawCurrentTab();

	scheduleUpdate();
	return true;
}


void WindowRenderer::update(const float dt)
{
	// 한 탭에서 다른 탭을 호출하기 위한 신호가 왔는지 확인해준다.
	if (int CallTabNum = CheckCallFunctions())
	{
		switch (CallTabNum)
		{
		case TAB_NUM::ACCOUNT :
			ChangeTabAccountToCharacter();
			break;
		case TAB_NUM::CHARACTER :
			ChangeTabCharacterToItem();
			break;
		}
	}
	return;
}

// Account Tab에서 Character Tab을 호출하는 신호 처리.
void WindowRenderer::ChangeTabAccountToCharacter()
{
	// Account Tab에서 선택된 Character id를 받아 Character Tab에 넘겨줌.
	char buf[10];
	sprintf(buf, "%d", m_pTabAccount->getCallCharacterId());
	m_pTabCharacter->Character_id = buf;

	// Character Tab을 화면에 띄우고 받은 id로 검색 시작.
	m_pTabCharacter->setVisible(true);
	m_pTabCharacter->setIsCharacterTabCalled(true);
	m_pTabCharacter->GetTextFromEditBox(m_pTabCharacter);

	// Account Tab의 신호를 꺼주기.
	m_pTabAccount->setCallFlag(false);
	return;
}

// Character Tab에서 Item Tab을 호출하는 신호 처리.
void WindowRenderer::ChangeTabCharacterToItem()
{
	// Character Tab에서 선택된 Item id를 받아 Item Tab에 넘겨줌.
	char buf[10];
	sprintf(buf, "%d", m_pTabCharacter->getCallItemId());
	m_pTabItem->Item_id = buf;

	// Item Tab을 화면에 띄우고 받은 id로 검색 시작.
	m_pTabItem->setVisible(true);
	m_pTabItem->setIsItemTabCalled(true);
	m_pTabItem->GetTextFromEditBox(m_pItemTab);

	// Character Tab의 신호를 꺼주기.
	m_pTabCharacter->setCallFlag(false);
	return;
}

// 기본적인 윈도우를 그려줌. 바탕화면, X버튼 등.
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

// Tab들을 그려주는 함수.
bool WindowRenderer::DrawTabs()
{
	// Get Size
	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const auto origin = Director::getInstance()->getVisibleOrigin();

	// Make Tabs
	m_pAccountTab = MenuItemImage::create("Tabs/Account_Normal.png", "Tabs/Account_Selected.png", CC_CALLBACK_1(WindowRenderer::changeToAccountTab, this));
	m_pCharacterTab = MenuItemImage::create("Tabs/Character_Normal.png", "Tabs/Character_Selected.png", CC_CALLBACK_1(WindowRenderer::changeToCharacterTab, this));
	m_pItemTab = MenuItemImage::create("Tabs/Item_Normal.png", "Tabs/Item_Selected.png", CC_CALLBACK_1(WindowRenderer::changeToItemTab, this));
	m_pDungeonTab = MenuItemImage::create("Tabs/Dungeon_Normal.png", "Tabs/Dungeon_Selected.png", CC_CALLBACK_1(WindowRenderer::changeToDungeonTab, this));

	// Setting Tab Positions
	float h = visibleSize.height - 71;
	float TabX = origin.x + m_pAccountTab->getContentSize().width / 2;

	m_pAccountTab->setPosition(Vec2(TabX, h));
	m_pCharacterTab->setPosition(Vec2(m_pAccountTab->getPosition().x + 163, h));
	m_pItemTab->setPosition(Vec2(m_pCharacterTab->getPosition().x + 163, h));
	m_pDungeonTab->setPosition(Vec2(m_pItemTab->getPosition().x + 163, h));

	// Make Menu with Tabs, and ADDCHILD
	m_pTabMenu = Menu::create(m_pAccountTab, m_pCharacterTab, m_pItemTab, m_pDungeonTab, NULL);
	m_pTabMenu->setPosition(Vec2::ZERO);
	addChild(m_pTabMenu, 1);

	return true;
}

// 현재 켜진 탭이 뭔지 확인하고 그 탭을 켜주는 함수. 
bool WindowRenderer::DrawCurrentTab()
{
	// 현재 탭의 번호를 받은 뒤, 해당하는 탭의 번호를 킨다.
	if (getCurrentTabNum() == TAB_NUM::ACCOUNT)
	{
		m_pTabAccount->setVisible(true);
	}
	if (getCurrentTabNum() == TAB_NUM::CHARACTER)
	{
		m_pTabCharacter->setVisible(true);
	}
	if (getCurrentTabNum() == TAB_NUM::ITEM)
	{
		m_pTabItem->setVisible(true);
	}

	return true;
}

// X 버튼을 위한 콜백함수. 프로그램을 종료한다.
void WindowRenderer::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
}

// AccountTab을 클릭했을 경우의 콜백함수. 
// 다른 탭을 unVisible하게 만든 뒤, AccountTab을 켜준다.
void WindowRenderer::changeToAccountTab(Ref * pSender)
{
	m_pTabCharacter->setVisible(false);
	m_pTabItem->setVisible(false);
	setCurrentTabNum(TAB_NUM::ACCOUNT);
	DrawCurrentTab();
}

// CharacterTab을 클릭했을 경우의 콜백함수.
// 다른 탭을 unVisible하게 만든 뒤, CharacterTab을 켜준다.
void WindowRenderer::changeToCharacterTab(Ref * pSender)
{
	m_pTabAccount->setVisible(false);
	m_pTabItem->setVisible(false);
	setCurrentTabNum(TAB_NUM::CHARACTER);
	DrawCurrentTab();
}

// ItemTab을 클릭했을 경우의 콜백함수.
// 다른 탭을 unVisible하게 만든 뒤, ItemTab을 켜준다.
void WindowRenderer::changeToItemTab(Ref * pSender)
{
	m_pTabAccount->setVisible(false);
	m_pTabCharacter->setVisible(false);
	setCurrentTabNum(TAB_NUM::ITEM);
	DrawCurrentTab();
}

void WindowRenderer::changeToDungeonTab(Ref * pSender)
{
	setCurrentTabNum(TAB_NUM::DUNGEON);
	DrawCurrentTab();
}

// Update문 안에서 주기적으로 다른 탭으로의 전환 신호가 들어왔는지 확인하는 함수.
// 신호가 없다면 NONE, 신호가 있다면 그 신호를 보낸 Tab의 번호를 반환한다.
int WindowRenderer::CheckCallFunctions()
{
	if (m_pTabAccount->getCallFlag())
	{
		return TAB_NUM::ACCOUNT;
	}
	else if (m_pTabCharacter->getCallFlag())
	{
		return TAB_NUM::CHARACTER;
	}

	return TAB_NUM::NONE;
}
