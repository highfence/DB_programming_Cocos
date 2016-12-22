#pragma once
using namespace ui;

// Query를 수행하며 계정 정보를 받아오기 위한 구조체.
typedef struct account
{
	bool accountExist;
	int battleTag;
	char name[20];
	char email[50];
	char region[10];
	int blockedDay;
};

// Query를 수행하며 계정에 속한 캐릭터 정보를 받아오기 위한 구조체.
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
	WindowRenderer에 소속되어 계정 관리 정보를 조회하고 보여주는 클래스.
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

