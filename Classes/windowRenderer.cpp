#include "stdafx.h"
#include "windowRenderer.h"
#include "TabAccount.h"
#include "TabCharacter.h"
#include "TabItem.h"

// ������ ������ �ʱ�ȭ Ŭ����.
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
	// �� �ǿ��� �ٸ� ���� ȣ���ϱ� ���� ��ȣ�� �Դ��� Ȯ�����ش�.
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

// Account Tab���� Character Tab�� ȣ���ϴ� ��ȣ ó��.
void WindowRenderer::ChangeTabAccountToCharacter()
{
	// Account Tab���� ���õ� Character id�� �޾� Character Tab�� �Ѱ���.
	char buf[10];
	sprintf(buf, "%d", m_pTabAccount->getCallCharacterId());
	m_pTabCharacter->Character_id = buf;

	// Character Tab�� ȭ�鿡 ���� ���� id�� �˻� ����.
	m_pTabCharacter->setVisible(true);
	m_pTabCharacter->setIsCharacterTabCalled(true);
	m_pTabCharacter->GetTextFromEditBox(m_pTabCharacter);

	// Account Tab�� ��ȣ�� ���ֱ�.
	m_pTabAccount->setCallFlag(false);
	return;
}

// Character Tab���� Item Tab�� ȣ���ϴ� ��ȣ ó��.
void WindowRenderer::ChangeTabCharacterToItem()
{
	// Character Tab���� ���õ� Item id�� �޾� Item Tab�� �Ѱ���.
	char buf[10];
	sprintf(buf, "%d", m_pTabCharacter->getCallItemId());
	m_pTabItem->Item_id = buf;

	// Item Tab�� ȭ�鿡 ���� ���� id�� �˻� ����.
	m_pTabItem->setVisible(true);
	m_pTabItem->setIsItemTabCalled(true);
	m_pTabItem->GetTextFromEditBox(m_pItemTab);

	// Character Tab�� ��ȣ�� ���ֱ�.
	m_pTabCharacter->setCallFlag(false);
	return;
}

// �⺻���� �����츦 �׷���. ����ȭ��, X��ư ��.
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

// Tab���� �׷��ִ� �Լ�.
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

// ���� ���� ���� ���� Ȯ���ϰ� �� ���� ���ִ� �Լ�. 
bool WindowRenderer::DrawCurrentTab()
{
	// ���� ���� ��ȣ�� ���� ��, �ش��ϴ� ���� ��ȣ�� Ų��.
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

// X ��ư�� ���� �ݹ��Լ�. ���α׷��� �����Ѵ�.
void WindowRenderer::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
}

// AccountTab�� Ŭ������ ����� �ݹ��Լ�. 
// �ٸ� ���� unVisible�ϰ� ���� ��, AccountTab�� ���ش�.
void WindowRenderer::changeToAccountTab(Ref * pSender)
{
	m_pTabCharacter->setVisible(false);
	m_pTabItem->setVisible(false);
	setCurrentTabNum(TAB_NUM::ACCOUNT);
	DrawCurrentTab();
}

// CharacterTab�� Ŭ������ ����� �ݹ��Լ�.
// �ٸ� ���� unVisible�ϰ� ���� ��, CharacterTab�� ���ش�.
void WindowRenderer::changeToCharacterTab(Ref * pSender)
{
	m_pTabAccount->setVisible(false);
	m_pTabItem->setVisible(false);
	setCurrentTabNum(TAB_NUM::CHARACTER);
	DrawCurrentTab();
}

// ItemTab�� Ŭ������ ����� �ݹ��Լ�.
// �ٸ� ���� unVisible�ϰ� ���� ��, ItemTab�� ���ش�.
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

// Update�� �ȿ��� �ֱ������� �ٸ� �������� ��ȯ ��ȣ�� ���Դ��� Ȯ���ϴ� �Լ�.
// ��ȣ�� ���ٸ� NONE, ��ȣ�� �ִٸ� �� ��ȣ�� ���� Tab�� ��ȣ�� ��ȯ�Ѵ�.
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
