#pragma once

class WindowRenderer : public Node
{
public : 
	CREATE_FUNC(WindowRenderer);

	/* Basic Functions */
	virtual bool init();

	// Member variables
	CC_SYNTHESIZE(Sprite*, m_pBasicWindow, BasicWindow);
	CC_SYNTHESIZE(MenuItemImage*, m_pCloseButton, CloseButton);
	CC_SYNTHESIZE(Menu*, m_pCloseMenu, CloseMenu);

	// Member Functions
	bool DrawBasicWindow();


    void menuCloseCallback(cocos2d::Ref* pSender);

private :
};