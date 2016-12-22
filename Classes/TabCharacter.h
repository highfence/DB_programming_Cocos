#pragma once
using namespace ui;

typedef struct character_s
{
	bool characterExist;
	int id;
	char classType[40];
	int level;
	int strength;
	int agility;
	int intellect;
	int hp;
	int mp;
};

typedef struct item
{
	int id;
	char name[40];
	int attackPoint;
	float drop_chance;
	char wearingClass[40];
};

class TabCharacter : public Node
{
public:
	CREATE_FUNC(TabCharacter);
	virtual bool init() override;
	virtual void update(float) override;

	bool drawSearchingBox();
	bool drawData();

	EditBox* m_pCharacterIdBox;
	Label* m_pTypedTag;
	CC_SYNTHESIZE(MenuItemImage*, m_pFindButton, FindButton);
	CC_SYNTHESIZE(Menu*, m_pButtonMenu, ButtonMenu);
	CC_SYNTHESIZE(bool, m_CallFlag, CallFlag);
	CC_SYNTHESIZE(int, m_CallItemId, CallItemId);
	CC_SYNTHESIZE(bool, m_IsCharaterTabCalled, IsCharacterTabCalled);
	CC_SYNTHESIZE(bool, m_IsNeedRefresh, IsNeedRefresh);

	std::string Character_id;
	std::string CharacterName;

	void GetTextFromEditBox(Ref* pSendor);
	int CharacterQuery();
	void deleteNulls(char*, int);
	void gotoItemStatus(Ref*, int);

	character_s* my_character;
	vector<item*>* my_items;
};

