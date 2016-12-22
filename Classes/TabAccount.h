#pragma once
using namespace ui;

typedef struct account
{
	bool accountExist;
	int battleTag;
	char name[20];
	char email[50];
	char region[10];
	int blockedDay;
};

typedef struct character
{
	bool characterExist;
	int id;
	char classType[40];
	int level;
	char playTime[40];
};

class TabAccount : public Node
{
public:
	CREATE_FUNC(TabAccount);
	virtual bool init() override;
	virtual void update(float) override;

	bool drawSearchingBox();
	bool drawData();

	EditBox* m_pBattleTagBox;
	Label* m_pTypedTag;
	CC_SYNTHESIZE(MenuItemImage*, m_pFindButton, FindButton);
	CC_SYNTHESIZE(Menu*, m_pButtonMenu, ButtonMenu);
	CC_SYNTHESIZE(bool, m_CallFlag, CallFlag);
	CC_SYNTHESIZE(int, m_CallCharacterId, CallCharacterId);
	CC_SYNTHESIZE(bool, m_IsNeedRefresh, IsNeedRefresh);

	std::string BattleTag;
	std::string AccountName;

	void GetTextFromEditBox(Ref* pSendor);
	int AccountQuery();
	void deleteNulls(char*, int);
	void gotoCharaterStatus(Ref*, int);

	account* my_account;
	std::vector<character*>* my_characters;
	cocos2d::Vector<cocos2d::Label*>* accountLabels;
};

