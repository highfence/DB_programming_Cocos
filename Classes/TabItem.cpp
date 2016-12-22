#include "stdafx.h"
#include "TabItem.h"

bool TabItem::init()
{
	if (!Node::init())
	{
		return false;
	}

	drawSearchingBox();
	my_items = new std::vector<item_s*>;


	return true;
}


bool TabItem::drawSearchingBox()
{
	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const auto origin = Director::getInstance()->getVisibleOrigin();

	// 검색창 구현
	m_pItemIdBox = ui::EditBox::create(Size(200, 46), "box.png");
	m_pItemIdBox->setPosition(Vec2(origin.x + m_pItemIdBox->getContentSize().width / 2 + 5, 620));
	addChild(m_pItemIdBox);

	auto searchLabel = Label::createWithTTF("Find with Item_id", TTF, 20);
	searchLabel->setColor(ccc3(0, 0, 0));
	searchLabel->setPosition(Vec2(origin.x + searchLabel->getContentSize().width / 2 + 5, m_pItemIdBox->getPosition().y + 40));
	addChild(searchLabel);

	m_pFindButton = MenuItemImage::create("FindItem_Normal.png", "FindItem_Selected.png", CC_CALLBACK_1(TabItem::GetTextFromEditBox, this));
	m_pButtonMenu = Menu::create(m_pFindButton, NULL);
	m_pButtonMenu->setPosition(Vec2(origin.x + m_pFindButton->getContentSize().width / 2 + 5, 570));
	addChild(m_pButtonMenu);

	return true;
}


void TabItem::GetTextFromEditBox(Ref* pSendor)
{
	if (getIsItemTabCalled())
	{
		setIsItemTabCalled(false);
	}
	else
	{
		Item_id = m_pItemIdBox->getText();
	}

	if (!Item_id.empty())
	{
		if (getIsNeedRefresh())
		{
			auto temporarySaver = Item_id;
			this->removeAllChildren();
			my_items->clear();
			Item_id = temporarySaver;
			drawSearchingBox();
		}
		if (!ItemQuery())
		{
			drawData();
		}
	}

	return;
}

void TabItem::deleteNulls(char* destChar, int maxLength)
{
	int beforeInt = 0;
	for (int i = 0; ; ++i)
	{
		if (!beforeInt && !destChar[i]) break;
		else if (!destChar[i])
		{
			for (int j = i; j < maxLength - 1; ++j)
			{
				destChar[j] = destChar[j + 1];
			}
		}

		beforeInt = destChar[i];
	}

	return;
}


int TabItem::ItemQuery()
{
	int ret;
	SQLHENV hEnv;
	SQLHDBC hDbc;
	SQLHSTMT hStmt;

	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

	ret = SQLConnect(hDbc, (SQLWCHAR*)L"LOCAL", SQL_NTS, (SQLWCHAR*)L"root", SQL_NTS, (SQLWCHAR*)L"dlrmsdnjs93", SQL_NTS);

	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
		printf("SQL Connect Ok \n");
	else {
		printf("sqlconnect error. \n");
		return -1;
	}

	std::wstring addStirng;
	addStirng.assign(Item_id.begin(), Item_id.end());
	std::wstring inputString = L"select i.item_id, i.item_name, i.attack_point, i.drop_chance, i.wearing_class, d.difficulty, d.dungeon_type from dungeon_drop_item as d \
	INNER JOIN idle_item AS i ON (i.item_id = d.item_id) WHERE i.item_id = " + addStirng;
	const wchar_t *inputWChar = inputString.c_str();

	// Item 정보를 찾아오는 작업 수행.
	ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	ret = SQLExecDirect(hStmt, (SQLWCHAR*)inputWChar, SQL_NTS);

	if (ret == SQL_SUCCESS) {
		int iCount = 0;
		SQLLEN iIdLen, iNameLen, iAtkLen, iDropLen, iClassLen, iDiffLen, iDungeonLen;

		while (TRUE) {
			ret = SQLFetch(hStmt);
			auto new_item = new item_s;
			if (ret == SQL_ERROR || ret == SQL_SUCCESS_WITH_INFO)
				printf("An error occured \n");
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
				SQLGetData(hStmt, 1, SQL_C_ULONG, &new_item->id, 0, &iIdLen);
				SQLGetData(hStmt, 2, SQL_C_WCHAR, new_item->name, 40, &iNameLen);
				SQLGetData(hStmt, 3, SQL_C_ULONG, &new_item->attackPoint, 0, &iAtkLen);
				SQLGetData(hStmt, 4, SQL_C_FLOAT, &new_item->drop_chance, 0, &iDropLen);
				SQLGetData(hStmt, 5, SQL_C_WCHAR, new_item->wearingClass, 40, &iClassLen);
				SQLGetData(hStmt, 6, SQL_C_ULONG, &new_item->drop_diff, 0, &iDiffLen);
				SQLGetData(hStmt, 7, SQL_C_WCHAR, new_item->drop_dungeon, 40, &iDungeonLen);


				// TODO :: NULL 문자가 들어가는 이유 찾고 해결하기. (메모리상 손해)
				deleteNulls(new_item->name, 40);
				deleteNulls(new_item->wearingClass, 40);
				deleteNulls(new_item->drop_dungeon, 40);

				my_items->push_back(new_item);
			}
			else break;
		}
	}
	else
	{
		printf("An error occured during excuting query!! \n");
		return -1;
	}

	// 접속 종료 및 반환
	if (hStmt) SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	if (hDbc) SQLDisconnect(hDbc);
	if (hDbc) SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

	return 0;
}

bool TabItem::drawData()
{
	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const auto origin = Director::getInstance()->getVisibleOrigin();

	// 배경 생성
	auto wideBox = Sprite::create("wideBox.png");
	wideBox->setPosition(Vec2(m_pButtonMenu->getPosition().x + (visibleSize.width - m_pButtonMenu->getPosition().x) / 2 + 30, visibleSize.height / 2 - 42));
	wideBox->setScale(1.5f, 1.0f);
	addChild(wideBox);
	
	// 속성 메뉴 생성
	auto idTitle = Label::createWithTTF("Id", TTF, 20);
	idTitle->setPosition(m_pItemIdBox->getPosition() + Vec2(130, -5));
	addChild(idTitle);

	auto nameTitle = Label::createWithTTF("Name", TTF, 20);
	nameTitle->setPosition(idTitle->getPosition() + Vec2(idTitle->getContentSize().width / 2 
		+ nameTitle->getContentSize().width / 2 + 60, 0));
	addChild(nameTitle);

	auto attackPointTitle = Label::createWithTTF("Atk", TTF, 20);
	attackPointTitle->setPosition(nameTitle->getPosition() + Vec2(nameTitle->getContentSize().width / 2
		+ attackPointTitle->getContentSize().width / 2 + 60, 0));
	addChild(attackPointTitle);

	auto dropChanceTitle = Label::createWithTTF("DropChance", TTF, 20);
	dropChanceTitle->setPosition(attackPointTitle->getPosition() + Vec2(attackPointTitle->getContentSize().width / 2
		+ dropChanceTitle->getContentSize().width / 2 + 15, 0));
	addChild(dropChanceTitle);

	auto wearingClassTitle = Label::createWithTTF("Class", TTF, 20);
	wearingClassTitle->setPosition(dropChanceTitle->getPosition() + Vec2(dropChanceTitle->getContentSize().width / 2
		+ wearingClassTitle->getContentSize().width / 2 + 15, 0));
	addChild(wearingClassTitle);

	auto diffTitle = Label::createWithTTF("DropDifficult", TTF, 20);
	diffTitle->setPosition(wearingClassTitle->getPosition() + Vec2(wearingClassTitle->getContentSize().width / 2
		+ diffTitle->getContentSize().width / 2 + 15, 0));
	addChild(diffTitle);

	auto dungeonTitle = Label::createWithTTF("DropDungeon", TTF, 20);
	dungeonTitle->setPosition(diffTitle->getPosition() + Vec2(diffTitle->getContentSize().width / 2
		+ dungeonTitle->getContentSize().width / 2 + 15, 0));
	addChild(dungeonTitle);



	vector<item_s*>::iterator iter = my_items->begin();
	for (int i = 0; ; ++i)
	{
		// TODO :: 캐릭터 버튼 생성.
		auto button = MenuItemImage::create("line_Normal.png", "line_Selected.png");
		button->setScale(1.5f, 1.0f);
		auto buttonMenu = Menu::create(button, NULL);
		buttonMenu->setPosition(Vec2(wideBox->getPosition().x, diffTitle->getPosition().y - 50 * (i + 1)));
		addChild(buttonMenu);

		char buf[40];
		sprintf(buf, "%d", my_items->at(i)->id);
		auto idLabel = Label::createWithTTF(buf, TTF, 20);
		idLabel->setColor(ccc3(255, 0, 0));
		idLabel->setPosition(idTitle->getPosition() - Vec2(0, 50 * (i + 1)));
		addChild(idLabel);

		auto nameLabel = Label::createWithTTF(my_items->at(i)->name, TTF, 20);
		nameLabel->setColor(ccc3(255, 0, 0));
		nameLabel->setPosition(nameTitle->getPosition() - Vec2(0, 50 * (i + 1)));
		addChild(nameLabel);

		sprintf(buf, "%d", my_items->at(i)->attackPoint);
		auto atkLabel = Label::createWithTTF(buf, TTF, 20);
		atkLabel->setColor(ccc3(255, 0, 0));
		atkLabel->setPosition(attackPointTitle->getPosition() - Vec2(0, 50 * (i + 1)));
		addChild(atkLabel);

		sprintf(buf, "%f", my_items->at(i)->drop_chance);
		auto dropChanceLabel = Label::createWithTTF(buf, TTF, 20);
		dropChanceLabel->setColor(ccc3(255, 0, 0));
		dropChanceLabel->setPosition(dropChanceTitle->getPosition() - Vec2(0, 50 * (i + 1)));
		addChild(dropChanceLabel);

		auto classLabel = Label::createWithTTF(my_items->at(i)->wearingClass, TTF, 20);
		classLabel->setColor(ccc3(255, 0, 0));
		classLabel->setPosition(wearingClassTitle->getPosition() - Vec2(0, 50 * (i + 1)));
		addChild(classLabel);

		sprintf(buf, "%d", my_items->at(i)->drop_diff);
		auto dropDiffLabel = Label::createWithTTF(buf, TTF, 20);
		dropDiffLabel->setColor(ccc3(255, 0, 0));
		dropDiffLabel->setPosition(diffTitle->getPosition() - Vec2(0, 50 * (i + 1)));
		addChild(dropDiffLabel);

		auto dungeonLabel = Label::createWithTTF(my_items->at(i)->drop_dungeon, TTF, 20);
		dungeonLabel->setColor(ccc3(255, 0, 0));
		dungeonLabel->setPosition(dungeonTitle->getPosition() - Vec2(0, 50 * (i + 1)));
		addChild(dungeonLabel);

		++iter;
		if (iter == my_items->end()) break;
	}

	setIsNeedRefresh(true);

	return true;
}