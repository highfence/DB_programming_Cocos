#include "stdafx.h"
#include "TabCharacter.h"


bool TabCharacter::init()
{
	if (!Node::init())
	{
		return false;
	}

	setIsNeedRefresh(false);
	setIsCharacterTabCalled(false);
	
	my_character = new character_s;
	my_character->characterExist = false;

	my_items = new std::vector<item*>;

	drawSearchingBox();

	scheduleUpdate();
	return true;
}

void TabCharacter::update(float dt)
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

bool TabCharacter::drawSearchingBox()
{
	const auto visibleSize = Director::getInstance()->getVisibleSize();
	const auto origin = Director::getInstance()->getVisibleOrigin();

	// 검색창 구현
	m_pCharacterIdBox= EditBox::create(Size(200, 46), "box.png");
	m_pCharacterIdBox->setPosition(Vec2(origin.x + m_pCharacterIdBox->getContentSize().width / 2 + 5, 620));
	addChild(m_pCharacterIdBox);

	auto searchLabel = Label::createWithTTF("Find with Character_id", TTF, 20);
	searchLabel->setColor(ccc3(0, 0, 0));
	searchLabel->setPosition(Vec2(origin.x + searchLabel->getContentSize().width / 2 + 5, m_pCharacterIdBox->getPosition().y + 40));
	addChild(searchLabel);

	m_pFindButton = MenuItemImage::create("FindCharacter_Normal.png", "FindCharacter_Selected.png", CC_CALLBACK_1(TabCharacter::GetTextFromEditBox, this));
	m_pButtonMenu = Menu::create(m_pFindButton, NULL);
	m_pButtonMenu->setPosition(Vec2(origin.x + m_pFindButton->getContentSize().width / 2 + 5, 570));
	addChild(m_pButtonMenu);

	return true;
}



void TabCharacter::GetTextFromEditBox(Ref* pSendor)
{
	if (getIsCharacterTabCalled())
	{
		setIsCharacterTabCalled(false);
	}
	else
	{
		Character_id = m_pCharacterIdBox->getText();
	}

	if (!Character_id.empty())
	{
		if (getIsNeedRefresh())
		{
			auto temporarySaver = Character_id;
			this->removeAllChildren();
			my_items->clear();
			Character_id = temporarySaver;
			drawSearchingBox();
		}
		CharacterQuery();

		if (my_character->characterExist)
		{
			drawData();
		}
	}

	return;
}


int TabCharacter::CharacterQuery()
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
	addStirng.assign(Character_id.begin(), Character_id.end());
	std::wstring inputString = L"select c.character_id, c.class_type, c.class_level, ic.strength, ic.agility, ic.intellect, ic.hp, ic.mp from instance_character AS c\
		INNER JOIN idle_character AS ic ON(c.class_level = ic.class_level AND c.class_type = ic.class_type)\
		WHERE c.character_id = " + addStirng;
	const wchar_t *inputWChar = inputString.c_str();

	// 명령
	ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	ret = SQLExecDirect(hStmt, (SQLWCHAR*)inputWChar, SQL_NTS);


	if (ret == SQL_SUCCESS) {
		int iCount = 0;
		SQLLEN iIdLen, iClassTypeLen, iLevelLen, iStrLen, iAgiLen, iIntLen, iHpLen, iMpLen;

		while (TRUE) {
			ret = SQLFetch(hStmt);
			if (ret == SQL_ERROR || ret == SQL_SUCCESS_WITH_INFO)
				printf("An error occured \n");
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
				SQLGetData(hStmt, 1, SQL_C_ULONG, &my_character->id, 0, &iIdLen);
				SQLGetData(hStmt, 2, SQL_C_WCHAR, my_character->classType, 40, &iClassTypeLen);
				SQLGetData(hStmt, 3, SQL_C_ULONG, &my_character->level, 0, &iLevelLen);
				SQLGetData(hStmt, 4, SQL_C_ULONG, &my_character->strength, 0, &iStrLen);
				SQLGetData(hStmt, 5, SQL_C_ULONG, &my_character->agility, 0, &iAgiLen);
				SQLGetData(hStmt, 6, SQL_C_ULONG, &my_character->intellect, 0, &iIntLen);
				SQLGetData(hStmt, 7, SQL_C_ULONG, &my_character->hp, 0, &iHpLen);
				SQLGetData(hStmt, 8, SQL_C_ULONG, &my_character->mp, 0, &iMpLen);

				// TODO :: NULL 문자가 들어가는 이유 찾고 해결하기. (메모리상 손해)
				deleteNulls(my_character->classType, 40);

				my_character->characterExist = true;
				break;
			}
			else break;
		}
	}
	else
		printf("An error occured during excuting query!! \n");
	
	std::wstring addStirng2;
	addStirng2.assign(Character_id.begin(), Character_id.end());
	std::wstring inputString2 = L"select i.item_id, i.item_name, i.attack_point, i.drop_chance, i.wearing_class from idle_item AS i\
		INNER JOIN character_has_item AS c ON(c.item_id = i.item_id)\
		WHERE c.character_id = " + addStirng2;
	const wchar_t *inputWChar2 = inputString2.c_str();
	ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	ret = SQLExecDirect(hStmt, (SQLWCHAR*)inputWChar2, SQL_NTS);
	int i = 0;

	if (ret == SQL_SUCCESS) {
		int iCount = 0;
		SQLLEN iIdLen, iNameLen, iAtkLen, iDropLen, iClassLen;

		while (TRUE) {
			auto new_item = new item;
			ret = SQLFetch(hStmt);
			if (ret == SQL_ERROR || ret == SQL_SUCCESS_WITH_INFO) {
				printf("An error occured \n");
				break;
			}
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
				SQLGetData(hStmt, 1, SQL_C_ULONG, &new_item->id, 0, &iIdLen);
				SQLGetData(hStmt, 2, SQL_C_WCHAR, new_item->name, 40, &iNameLen);
				SQLGetData(hStmt, 3, SQL_C_ULONG, &new_item->attackPoint, 0, &iAtkLen);
				SQLGetData(hStmt, 4, SQL_C_FLOAT, &new_item->drop_chance, 0, &iDropLen);
				SQLGetData(hStmt, 5, SQL_C_WCHAR, &new_item->wearingClass, 40, &iClassLen);

				// TODO :: NULL 문자가 들어가는 이유 찾고 해결하기. (메모리상 손해)
				deleteNulls(new_item->name, 40);
				deleteNulls(new_item->wearingClass, 40);

				my_items->push_back(new_item);
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

bool TabCharacter::drawData()
{
	char inputStr[50];




	// Box 생성
	auto idBox = Sprite::create("resultBox.png");
	idBox->setPosition(m_pCharacterIdBox->getPosition() + Vec2(250, 0));
	addChild(idBox);

	auto classTypeBox = Sprite::create("resultBox.png");
	classTypeBox->setPosition(idBox->getPosition() - Vec2(0, 85));
	addChild(classTypeBox);

	auto levelBox = Sprite::create("resultBox.png");
	levelBox->setPosition(classTypeBox->getPosition() - Vec2(0, 85));
	addChild(levelBox);

	auto strBox = Sprite::create("resultBox.png");
	strBox->setPosition(levelBox->getPosition() - Vec2(0, 85));
	addChild(strBox);

	auto agiBox = Sprite::create("resultBox.png");
	agiBox->setPosition(strBox->getPosition() - Vec2(0, 85));
	addChild(agiBox);

	auto intBox = Sprite::create("resultBox.png");
	intBox->setPosition(agiBox->getPosition() - Vec2(0, 85));
	addChild(intBox);

	auto hpBox = Sprite::create("resultBox.png");
	hpBox->setPosition(agiBox->getPosition() - Vec2(250, 0));
	addChild(hpBox);

	auto mpBox = Sprite::create("resultBox.png");
	mpBox->setPosition(hpBox->getPosition() - Vec2(0, 85));
	addChild(mpBox);

	// 내용 생성.
	sprintf(inputStr, "%d", my_character->id);
	auto idLabel = Label::createWithTTF(inputStr, TTF, 20);
	idLabel->setPosition(idBox->getPosition());
	idLabel->setColor(ccc3(0, 0, 0));
	addChild(idLabel);

	auto classTypeLabel = Label::createWithTTF(my_character->classType, TTF, 20);
	classTypeLabel->setPosition(classTypeBox->getPosition());
	classTypeLabel->setColor(ccc3(0, 0, 0));
	addChild(classTypeLabel);

	sprintf(inputStr, "%d", my_character->level);
	auto levelLabel = Label::createWithTTF(inputStr, TTF, 20);
	levelLabel->setPosition(levelBox->getPosition());
	levelLabel->setColor(ccc3(0, 0, 0));
	addChild(levelLabel);
	
	sprintf(inputStr, "%d", my_character->strength);
	auto strLabel = Label::createWithTTF(inputStr, TTF, 20);
	strLabel->setPosition(strBox->getPosition());
	strLabel->setColor(ccc3(0, 0, 0));
	addChild(strLabel);

	sprintf(inputStr, "%d", my_character->agility);
	auto agiLabel = Label::createWithTTF(inputStr, TTF, 20);
	agiLabel->setPosition(agiBox->getPosition());
	agiLabel->setColor(ccc3(0, 0, 0));
	addChild(agiLabel);

	sprintf(inputStr, "%d", my_character->intellect);
	auto intLabel = Label::createWithTTF(inputStr, TTF, 20);
	intLabel->setPosition(intBox->getPosition());
	intLabel->setColor(ccc3(0, 0, 0));
	addChild(intLabel);

	sprintf(inputStr, "%d", my_character->hp);
	auto hpLabel = Label::createWithTTF(inputStr, TTF, 20);
	hpLabel->setPosition(hpBox->getPosition());
	hpLabel->setColor(ccc3(0, 0, 0));
	addChild(hpLabel);

	sprintf(inputStr, "%d", my_character->mp);
	auto mpLabel = Label::createWithTTF(inputStr, TTF, 20);
	mpLabel->setPosition(mpBox->getPosition());
	mpLabel->setColor(ccc3(0, 0, 0));
	addChild(mpLabel);
	
	// 제목 생성.
	
	auto idTitle = Label::createWithTTF("Character_id", TTF, 20);
	idTitle->setPosition(idBox->getPosition() + Vec2(-idBox->getContentSize().width / 2, 35));
	idTitle->setAnchorPoint(ccp(0, 0.5f));
	addChild(idTitle);
	
	auto classTypeTitle = Label::createWithTTF("ClassType", TTF, 20);
	classTypeTitle->setPosition(classTypeBox->getPosition() + Vec2(-classTypeBox->getContentSize().width / 2, 35));
	classTypeTitle->setAnchorPoint(ccp(0, 0.5f));
	addChild(classTypeTitle);
	
	auto levelTitle = Label::createWithTTF("Level", TTF, 20);
	levelTitle->setPosition(levelBox->getPosition() + Vec2(-levelBox->getContentSize().width / 2, 35));
	levelTitle->setAnchorPoint(ccp(0, 0.5f));
	addChild(levelTitle);
	
	auto strTitle = Label::createWithTTF("Strength", TTF, 20);
	strTitle->setPosition(strBox->getPosition() + Vec2(-strBox->getContentSize().width / 2, 35));
	strTitle->setAnchorPoint(ccp(0, 0.5f));
	addChild(strTitle);

	auto agiTitle = Label::createWithTTF("Agility", TTF, 20);
	agiTitle->setPosition(agiBox->getPosition() + Vec2(-agiBox->getContentSize().width / 2, 35));
	agiTitle->setAnchorPoint(ccp(0, 0.5f));
	addChild(agiTitle);

	auto intTitle = Label::createWithTTF("Intellect", TTF, 20);
	intTitle->setPosition(intBox->getPosition() + Vec2(-intBox->getContentSize().width / 2, 35));
	intTitle->setAnchorPoint(ccp(0, 0.5f));
	addChild(intTitle);

	auto hpTitle = Label::createWithTTF("Max Hp", TTF, 20);
	hpTitle->setPosition(hpBox->getPosition() + Vec2(-hpBox->getContentSize().width / 2, 35));
	hpTitle->setAnchorPoint(ccp(0, 0.5f));
	addChild(hpTitle);

	auto mpTitle = Label::createWithTTF("Max Mp", TTF, 20);
	mpTitle->setPosition(mpBox->getPosition() + Vec2(-mpBox->getContentSize().width / 2, 35));
	mpTitle->setAnchorPoint(ccp(0, 0.5f));
	addChild(mpTitle);

	// 보유 아이템 목록 생성.
	auto wideBox = Sprite::create("wideBox.png");
	wideBox->setPosition(strBox->getPosition() + Vec2(wideBox->getContentSize().width - strBox->getContentSize().width / 2, -strBox->getContentSize().height / 2));
	addChild(wideBox);

	auto nameTitle = Label::createWithTTF("Name", TTF, 20);
	nameTitle->setPosition(Vec2(wideBox->getPosition().x - wideBox->getContentSize().width / 2 + nameTitle->getContentSize().width / 2 + 20,
		wideBox->getPosition().y + wideBox->getContentSize().height / 2 - nameTitle->getContentSize().height / 2 - 15));
	addChild(nameTitle);

	auto AtkTitle = Label::createWithTTF("Atk", TTF, 20);
	AtkTitle->setPosition(nameTitle->getPosition() + Vec2(150, 0));
	addChild(AtkTitle);

	auto DropTitle = Label::createWithTTF("Drop Chance", TTF, 20);
	DropTitle->setPosition(AtkTitle->getPosition() + Vec2(115, 0));
	addChild(DropTitle);

	auto WearingTitle = Label::createWithTTF("Wearing", TTF, 20);
	WearingTitle->setPosition(DropTitle->getPosition() + Vec2(125, 0));
	addChild(WearingTitle);

	
	vector<item*>::iterator iter = my_items->begin();
	for (int i = 0; ; ++i)
	{
		// TODO :: 캐릭터 버튼 생성.
		auto button = MenuItemImage::create("line_Normal.png", "line_Selected.png", CC_CALLBACK_1(TabCharacter::gotoItemStatus, this, my_items->at(i)->id));
		auto buttonMenu = Menu::create(button, NULL);
		buttonMenu->setPosition(Vec2(wideBox->getPosition().x, nameTitle->getPosition().y - 50 * (i + 1)));
		addChild(buttonMenu);

		char buf[40];

		auto nameLabel = Label::createWithTTF(my_items->at(i)->name, TTF, 20);
		nameLabel->setColor(ccc3(255, 0, 0));
		nameLabel->setAnchorPoint(ccp(0, 0.5f));
		nameLabel->setPosition(nameTitle->getPosition() - Vec2(nameTitle->getContentSize().width/2 + 20, 50 * (i + 1)));
		addChild(nameLabel);

		sprintf(buf, "%d", my_items->at(i)->attackPoint);
		auto atkLabel = Label::createWithTTF(buf, TTF, 20);
		atkLabel->setColor(ccc3(255, 0, 0));
		atkLabel->setPosition(AtkTitle->getPosition() - Vec2(0, 50 * (i + 1)));
		addChild(atkLabel);

		sprintf(buf, "%f", my_items->at(i)->drop_chance);
		auto dropLabel = Label::createWithTTF(buf, TTF, 20);
		dropLabel->setColor(ccc3(255, 0, 0));
		dropLabel->setPosition(DropTitle->getPosition() - Vec2(0, 50 * (i + 1)));
		addChild(dropLabel);

		auto wearingLabel = Label::createWithTTF(my_items->at(i)->wearingClass, TTF, 20);
		wearingLabel->setColor(ccc3(255, 0, 0));
		wearingLabel->setPosition(WearingTitle->getPosition() - Vec2(0, 50 * (i + 1)));
		addChild(wearingLabel);


		++iter;
		if (iter == my_items->end()) break;
	}

	setIsNeedRefresh(true);

	return true;
}


void TabCharacter::deleteNulls(char* destChar, int maxLength)
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


void TabCharacter::gotoItemStatus(Ref* pSender, int item_id)
{
	this->setVisible(false);
	setCallFlag(true);
	setCallItemId(item_id);
	return;
}
