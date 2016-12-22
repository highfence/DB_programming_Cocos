#include "stdafx.h"
#include "TabAccount.h"

// TabAccount 초기화 함수.
bool TabAccount::init()
{
	if (!Node::init())
	{
		return false;
	}

	setIsNeedRefresh(false);

	my_account = new account;
	my_account->accountExist = false;
	my_characters = new std::vector<character*>;
	accountLabels = new cocos2d::Vector<cocos2d::Label*>;

	setCallFlag(false);

	drawSearchingBox();

	scheduleUpdate();
	return true;
}

void TabAccount::update(float dt)
{
	// TODO :: 검색창에 입력을 받은 뒤에 글자가 남아있도록.
	/*
	if (m_pBattleTagBox->)
	{
		char buffer[255];
		sprintf(buffer, "%d", m_pBattleTagBox->getText());
		m_pTypedTag = Label::createWithTTF(buffer, TTF, 35);
		m_pTypedTag->setColor(ccc3(0, 0, 0));
		m_pTypedTag->setPosition(Vec2(origin.x + m_pTypedTag->getContentSize().width / 2 + 5, 620));
		addChild(m_pTypedTag);
	}
*/
	return;
}

bool TabAccount::drawSearchingBox()
{
	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const auto origin = Director::getInstance()->getVisibleOrigin();
	// 검색창 구현
	m_pBattleTagBox = EditBox::create(Size(200, 46), "box.png");
	m_pBattleTagBox->setPosition(Vec2(origin.x + m_pBattleTagBox->getContentSize().width / 2 + 5, 620));
	addChild(m_pBattleTagBox);

	auto searchLabel = Label::createWithTTF("Find with Battle_Tag", TTF, 20);
	searchLabel->setColor(ccc3(0, 0, 0));
	searchLabel->setPosition(Vec2(origin.x + searchLabel->getContentSize().width / 2 + 5, m_pBattleTagBox->getPosition().y + 40));
	addChild(searchLabel);

	m_pFindButton = MenuItemImage::create("FindAccount_Normal.png", "FindAccount_Selected.png", CC_CALLBACK_1(TabAccount::GetTextFromEditBox, this));
	m_pButtonMenu = Menu::create(m_pFindButton, NULL);
	m_pButtonMenu->setPosition(Vec2(origin.x + m_pFindButton->getContentSize().width / 2 + 5, 570));
	addChild(m_pButtonMenu);

	return true;
}



void TabAccount::GetTextFromEditBox(Ref* pSendor)
{
	BattleTag = m_pBattleTagBox->getText();

	if (!BattleTag.empty())
	{
		if (getIsNeedRefresh())
		{
			auto temporarySaver = BattleTag;
			this->removeAllChildren();
			my_characters->clear();
			BattleTag = temporarySaver;
			drawSearchingBox();
		}
		AccountQuery();

		if (my_account->accountExist)
		{
			drawData();
		}
	}

		return;
}


int TabAccount::AccountQuery()
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
		return 1;
	}

	std::wstring addStirng;
	addStirng.assign(BattleTag.begin(), BattleTag.end());
	std::wstring inputString = L"select * from blizzard_account where battle_Tag =" + addStirng;
	const wchar_t *inputWChar = inputString.c_str();

	// 명령
	ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	ret = SQLExecDirect(hStmt, (SQLWCHAR*)inputWChar, SQL_NTS);


	if (ret == SQL_SUCCESS) {
		int iCount = 0;
		SQLLEN iNameLen, iEmailLen, iRegionLen, iBlockedLen, iTagLen;

		while (TRUE) {
			ret = SQLFetch(hStmt);
			if (ret == SQL_ERROR || ret == SQL_SUCCESS_WITH_INFO)
				printf("An error occured \n");
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
				SQLGetData(hStmt, 1, SQL_C_ULONG, &my_account->battleTag, 0, &iTagLen);
				SQLGetData(hStmt, 2, SQL_C_WCHAR, my_account->name, 20, &iNameLen);
				SQLGetData(hStmt, 3, SQL_C_WCHAR, my_account->email, 50, &iEmailLen);
				SQLGetData(hStmt, 4, SQL_C_WCHAR, my_account->region, 10, &iRegionLen);
				SQLGetData(hStmt, 5, SQL_C_ULONG, &my_account->blockedDay, 0, &iBlockedLen);

				// TODO :: NULL 문자가 들어가는 이유 찾고 해결하기. (메모리상 손해)
				deleteNulls(my_account->name, 20);
				deleteNulls(my_account->email, 50);
				deleteNulls(my_account->region, 10);

				my_account->accountExist = true;
				break;
			}
			else break;
		}
	}
	else
		printf("An error occured during excuting query!! \n");

	std::wstring addStirng2;
	addStirng2.assign(BattleTag.begin(), BattleTag.end());
	std::wstring inputString2 = L"select c.character_id, c.class_type, c.class_level, c.play_time from instance_character AS c INNER JOIN account_has_character AS a ON(a.character_id = c.character_id)	WHERE a.battle_tag =" + addStirng2;
	const wchar_t *inputWChar2 = inputString2.c_str();
	ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	ret = SQLExecDirect(hStmt, (SQLWCHAR*)inputWChar2, SQL_NTS);
	int i = 0;

	if (ret == SQL_SUCCESS) {
		int iCount = 0;
		SQLLEN iIdLen, iClassTypeLen, iLevelLen, iPlayTimeLen;

		while (TRUE) {
			character* my_character = new character;
			ret = SQLFetch(hStmt);
			if (ret == SQL_ERROR || ret == SQL_SUCCESS_WITH_INFO) {
				printf("An error occured \n");
				break;
			}
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
				SQLGetData(hStmt, 1, SQL_C_ULONG, &my_character->id, 0, &iIdLen);
				SQLGetData(hStmt, 2, SQL_C_WCHAR, my_character->classType, 40, &iClassTypeLen);
				SQLGetData(hStmt, 3, SQL_C_ULONG, &my_character->level, 0, &iLevelLen);
				SQLGetData(hStmt, 4, SQL_C_WCHAR, my_character->playTime, 40, &iPlayTimeLen);

				// TODO :: NULL 문자가 들어가는 이유 찾고 해결하기. (메모리상 손해)
				deleteNulls(my_character->classType, 40);
				deleteNulls(my_character->playTime, 40);

				my_character->characterExist = true;
				my_characters->push_back(my_character);
			}
			else break;
		}
	}
	else
		printf("An error occured during excuting query!! \n");

	// 접속 종료 및 반환
	if (hStmt) SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	if (hDbc) SQLDisconnect(hDbc);
	if (hDbc) SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

	return 0;
}

bool TabAccount::drawData()
{
	char inputStr[50];


	// Box 생성
	auto battleTagBox = Sprite::create("resultBox.png");
	battleTagBox->setPosition(m_pBattleTagBox->getPosition() + Vec2(250, 0));
	addChild(battleTagBox);

	auto nameBox = Sprite::create("resultBox.png");
	nameBox->setPosition(battleTagBox->getPosition() - Vec2(0, 85));
	addChild(nameBox);

	auto emailBox = Sprite::create("resultBox.png");
	emailBox->setPosition(nameBox->getPosition() - Vec2(0, 85));
	addChild(emailBox);

	auto regionBox = Sprite::create("resultBox.png");
	regionBox->setPosition(emailBox->getPosition() - Vec2(0, 85));
	addChild(regionBox);

	auto blockedDayBox = Sprite::create("resultBox.png");
	blockedDayBox->setPosition(regionBox->getPosition() - Vec2(0, 85));
	addChild(blockedDayBox);

	// 내용 생성.

	sprintf(inputStr, "%d", my_account->battleTag);
	auto battleTagLabel = Label::createWithTTF(inputStr, TTF, 20);
	battleTagLabel->setPosition(battleTagBox->getPosition());
	battleTagLabel->setColor(ccc3(0, 0, 0));
	accountLabels->pushBack(battleTagLabel);
	addChild(battleTagLabel);

	auto nameLabel = Label::createWithTTF(my_account->name, TTF, 20);
	nameLabel->setPosition(nameBox->getPosition());
	nameLabel->setColor(ccc3(0, 0, 0));
	accountLabels->pushBack(nameLabel);
	addChild(nameLabel);

	auto emailLabel = Label::createWithTTF(my_account->email, TTF, 20);
	emailLabel->setPosition(emailBox->getPosition());
	emailLabel->setColor(ccc3(0, 0, 0));
	accountLabels->pushBack(emailLabel);
	addChild(emailLabel);

	auto regionLabel = Label::createWithTTF(my_account->region, TTF, 20);
	regionLabel->setPosition(regionBox->getPosition());
	regionLabel->setColor(ccc3(0, 0, 0));
	accountLabels->pushBack(regionLabel);
	addChild(regionLabel);

	sprintf(inputStr, "%d", my_account->blockedDay);
	auto blockedDayLabel = Label::createWithTTF(inputStr, TTF, 20);
	blockedDayLabel->setPosition(blockedDayBox->getPosition());
	blockedDayLabel->setColor(ccc3(0, 0, 0));
	accountLabels->pushBack(blockedDayLabel);
	addChild(blockedDayLabel);

	// 제목 생성.
	auto battleTagTitle = Label::createWithTTF("BattleTag", TTF, 20);
	battleTagTitle->setPosition(battleTagBox->getPosition() + Vec2(-battleTagBox->getContentSize().width / 2, 35));
	battleTagTitle->setAnchorPoint(ccp(0, 0.5f));
	addChild(battleTagTitle);

	auto nameTitle = Label::createWithTTF("Name", TTF, 20);
	nameTitle->setPosition(nameBox->getPosition() + Vec2(-nameBox->getContentSize().width / 2, 35));
	nameTitle->setAnchorPoint(ccp(0, 0.5f));
	addChild(nameTitle);

	auto emailTitle = Label::createWithTTF("E-mail", TTF, 20);
	emailTitle->setPosition(emailBox->getPosition() + Vec2(-emailBox->getContentSize().width / 2, 35));
	emailTitle->setAnchorPoint(ccp(0, 0.5f));
	addChild(emailTitle);

	auto regionTitle = Label::createWithTTF("Region", TTF, 20);
	regionTitle->setPosition(regionBox->getPosition() + Vec2(-regionBox->getContentSize().width / 2, 35));
	regionTitle->setAnchorPoint(ccp(0, 0.5f));
	addChild(regionTitle);

	auto blockedDayTitle = Label::createWithTTF("Blocked_Day", TTF, 20);
	blockedDayTitle->setPosition(blockedDayBox->getPosition() + Vec2(-blockedDayBox->getContentSize().width / 2, 35));
	blockedDayTitle->setAnchorPoint(ccp(0, 0.5f));
	addChild(blockedDayTitle);

	// 보유 캐릭터 목록 생성.
	auto wideBox = Sprite::create("wideBox.png");
	wideBox->setPosition(regionBox->getPosition() + Vec2(wideBox->getContentSize().width - regionBox->getContentSize().width / 2, -regionBox->getContentSize().height / 2));
	addChild(wideBox);

	auto idTitle = Label::createWithTTF("Id", TTF, 20);
	idTitle->setPosition(Vec2(wideBox->getPosition().x - wideBox->getContentSize().width / 2 + idTitle->getContentSize().width / 2 + 20,
		wideBox->getPosition().y + wideBox->getContentSize().height / 2 - idTitle->getContentSize().height / 2 - 15));
	accountLabels->pushBack(idTitle);
	addChild(idTitle);

	auto classTitle = Label::createWithTTF("class", TTF, 20);
	classTitle->setPosition(idTitle->getPosition() + Vec2(100, 0));
	accountLabels->pushBack(classTitle);
	addChild(classTitle);

	auto playtimeTitle = Label::createWithTTF("playtime", TTF, 20);
	playtimeTitle->setPosition(classTitle->getPosition() + Vec2(150, 0));
	accountLabels->pushBack(playtimeTitle);
	addChild(playtimeTitle);

	auto levelTitle = Label::createWithTTF("level", TTF, 20);
	levelTitle->setPosition(playtimeTitle->getPosition() + Vec2(150, 0));
	accountLabels->pushBack(levelTitle);
	addChild(levelTitle);


	vector<character*>::iterator iter = my_characters->begin();
	for (int i = 0; ; ++i)
	{
		// TODO :: 캐릭터 버튼 생성.
		auto button = MenuItemImage::create("line_Normal.png", "line_Selected.png", CC_CALLBACK_1(TabAccount::gotoCharaterStatus, this, my_characters->at(i)->id));
		auto buttonMenu = Menu::create(button, NULL);
		buttonMenu->setPosition(Vec2(wideBox->getPosition().x, levelTitle->getPosition().y - 50 * (i + 1)));
		addChild(buttonMenu);

		char buf[40];
		sprintf(buf, "%d", my_characters->at(i)->id);
		auto idLabel = Label::createWithTTF(buf, TTF, 20);
		idLabel->setColor(ccc3(255, 0, 0));
		idLabel->setPosition(idTitle->getPosition() - Vec2(0, 50 * (i + 1)));
		accountLabels->pushBack(idLabel);
		addChild(idLabel);

		auto classLabel = Label::createWithTTF(my_characters->at(i)->classType, TTF, 20);
		classLabel->setColor(ccc3(255, 0, 0));
		classLabel->setPosition(classTitle->getPosition() - Vec2(0, 50 * (i + 1)));
		accountLabels->pushBack(classLabel);
		addChild(classLabel);

		auto playtimeLabel = Label::createWithTTF(my_characters->at(i)->playTime, TTF, 20);
		playtimeLabel->setColor(ccc3(255, 0, 0));
		playtimeLabel->setPosition(playtimeTitle->getPosition() - Vec2(0, 50 * (i + 1)));
		accountLabels->pushBack(playtimeLabel);
		addChild(playtimeLabel);

		sprintf(buf, "%d", my_characters->at(i)->level);
		auto levelLabel = Label::createWithTTF(buf, TTF, 20);
		levelLabel->setColor(ccc3(255, 0, 0));
		levelLabel->setPosition(levelTitle->getPosition() - Vec2(0, 50 * (i + 1)));
		accountLabels->pushBack(levelLabel);
		addChild(levelLabel);


		++iter;
		if (iter == my_characters->end()) break;
	}

	setIsNeedRefresh(true);
	return true;

}


void TabAccount::deleteNulls(char* destChar, int maxLength)
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

void TabAccount::gotoCharaterStatus(Ref* pSender, int character_id)
{
	this->setVisible(false);
	setCallFlag(true);
	setCallCharacterId(character_id);
	return;
}