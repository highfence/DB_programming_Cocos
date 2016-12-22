#pragma once

/*
	WindowRenderer
	기본 윈도우를 그려주고 Tab들을 가지고 관리해주는 클래스.
*/

class TabAccount;
class TabCharacter;
class TabItem;

class WindowRenderer : public cocos2d::Node
{
public : 
	CREATE_FUNC(WindowRenderer);

	/* Basic Functions */
	virtual bool init() override;
	virtual void update(const float) override;

	// Member variables
	CC_SYNTHESIZE(Sprite*, m_pBasicWindow, BasicWindow);
	CC_SYNTHESIZE(MenuItemImage*, m_pCloseButton, CloseButton);
	CC_SYNTHESIZE(Menu*, m_pCloseMenu, CloseMenu);
	CC_SYNTHESIZE(int, m_pCurrentTabNum, CurrentTabNum);

	// Tabs
	CC_SYNTHESIZE(MenuItemImage*, m_pAccountTab, AccountTab);
	CC_SYNTHESIZE(MenuItemImage*, m_pCharacterTab, CharaccterTab);
	CC_SYNTHESIZE(MenuItemImage*, m_pItemTab, ItemTab);
	CC_SYNTHESIZE(MenuItemImage*, m_pDungeonTab, DungeonTab);
	CC_SYNTHESIZE(Menu*, m_pTabMenu, TabMenu);


	// Member Functions
	bool DrawBasicWindow();
	bool DrawTabs();
	bool DrawCurrentTab();
	int CheckCallFunctions();
	void ChangeTabAccountToCharacter();
	void ChangeTabCharacterToItem();

	// Call back Functions
    void menuCloseCallback(Ref* pSender);
	void changeToAccountTab(Ref* pSender);
	void changeToCharacterTab(Ref* pSender);
	void changeToItemTab(Ref* pSender);
	void changeToDungeonTab(Ref* pSender);
	

private :
	TabAccount* m_pTabAccount;
	TabCharacter* m_pTabCharacter;
	TabItem* m_pTabItem;
};