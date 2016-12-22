#pragma once

typedef struct item_s
{
	int id;
	char name[40];
	int attackPoint;
	float drop_chance;
	char wearingClass[40];
	int drop_diff;
	char drop_dungeon[40];
};


class TabItem : public cocos2d::Node
{
public :
	CREATE_FUNC(TabItem);
	virtual bool init() override;

	bool drawSearchingBox();
	bool drawData();
	void GetTextFromEditBox(Ref*);
	int ItemQuery();
	void deleteNulls(char*, int);

	CC_SYNTHESIZE(bool, m_IsItemTabCalled, IsItemTabCalled);
	CC_SYNTHESIZE(bool, m_IsNeedRefresh, IsNeedRefresh);
	
	std::string Item_id;
	ui::EditBox* m_pItemIdBox;
	MenuItemImage* m_pFindButton;
	Menu* m_pButtonMenu;

	vector<item_s*>* my_items;
};