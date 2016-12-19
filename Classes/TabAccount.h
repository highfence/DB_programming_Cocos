#pragma once
using namespace ui;

typedef struct account
{
	int battleTag;
	char name[20];
	char email[50];
	char region[10];
	int blockedDay;
};

class TabAccount : public Node
{
public:
	CREATE_FUNC(TabAccount);
	virtual bool init();

	bool drawComponent();
	bool drawData();

	EditBox* m_pBattleTagBox;
	CC_SYNTHESIZE(MenuItemImage*, m_pFindButton, FindButton);
	CC_SYNTHESIZE(Menu*, m_pButtonMenu, ButtonMenu);

	std::string BattleTag;
	std::string AccountName;

	void GetTextFromEditBox(Ref* pSendor);
	int Query();

	account* my_account;
};

