#include "stdafx.h"
#include "TabAccount.h"


bool TabAccount::init()
{
	if (!Node::init())
	{
		return false;
	}
	
	my_account = new account;
	drawComponent();

	return true;
}


bool TabAccount::drawComponent()
{
	m_pBattleTagBox = EditBox::create(Size(254, 58), "box.png");
	m_pBattleTagBox->setPosition(Vec2(500, 500));
	addChild(m_pBattleTagBox);

	m_pFindButton = MenuItemImage::create("FindAccount_Normal.png", "FindAccount_Selected", CC_CALLBACK_1(TabAccount::GetTextFromEditBox, this));
	m_pButtonMenu = Menu::create(m_pFindButton, NULL);
	m_pButtonMenu->setPosition(Vec2(500, 400));
	addChild(m_pButtonMenu);

	return true;
}



void TabAccount::GetTextFromEditBox(Ref* pSendor)
{
	BattleTag = m_pBattleTagBox->getText();
	Query();

	if (my_account->battleTag)
	{
		drawData();
	}

	return;
}


int TabAccount::Query()
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
			
				break;
			}
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
	sprintf(inputStr, "%d", my_account->battleTag);

	auto battleTagLabel = Label::createWithTTF(inputStr, "NotoSansCJKkr-Bold.otf", 20);
	battleTagLabel->setPosition(Vec2(200, 400));
	addChild(battleTagLabel);

	return true;

}