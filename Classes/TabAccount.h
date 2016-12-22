#pragma once
using namespace ui;

// Query�� �����ϸ� ���� ������ �޾ƿ��� ���� ����ü.
typedef struct account
{
	bool accountExist;
	int battleTag;
	char name[20];
	char email[50];
	char region[10];
	int blockedDay;
};

// Query�� �����ϸ� ������ ���� ĳ���� ������ �޾ƿ��� ���� ����ü.
typedef struct character
{
	bool characterExist;
	int id;
	char classType[40];
	int level;
	char playTime[40];
};


/*
	TabAccount
	WindowRenderer�� �ҼӵǾ� ���� ���� ������ ��ȸ�ϰ� �����ִ� Ŭ����.
*/

class TabAccount : public Node
{
public:
	// Cocos2d-x Basic Functions
	CREATE_FUNC(TabAccount);
	virtual bool init() override;
	virtual void update(float) override;

	// Draw Functions
	bool drawSearchingBox();
	bool drawData();

	// Member Variables.
	EditBox* m_pBattleTagBox;
	Label* m_pTypedTag;
	CC_SYNTHESIZE(MenuItemImage*, m_pFindButton, FindButton);
	CC_SYNTHESIZE(Menu*, m_pButtonMenu, ButtonMenu);
	CC_SYNTHESIZE(bool, m_CallFlag, CallFlag);
	CC_SYNTHESIZE(int, m_CallCharacterId, CallCharacterId);
	CC_SYNTHESIZE(bool, m_IsNeedRefresh, IsNeedRefresh);

	std::string BattleTag;
	std::string AccountName;

	account* my_account;
	std::vector<character*>* my_characters;
	cocos2d::Vector<cocos2d::Label*>* accountLabels;

	//  Other Functions
	void GetTextFromEditBox(Ref* pSendor);
	int AccountQuery();
	void deleteNulls(char*, int);
	void gotoCharaterStatus(Ref*, int);

};

