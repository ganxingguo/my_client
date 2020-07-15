#include "stdafx.h"
#include "MyClient.h"
#include "MyClientDlg.h"
#include "iphlpapi.h"	
#include <stdarg.h>

// GetAdaptersInfo 这个函数在这个里面实现
#pragma comment(lib,"iphlpapi.lib")

// 静态-----------------------------------
CListBox* CMyClientDlg::s_pListBoxLog = NULL;
CMyClientDlg* CMyClientDlg::s_pMainDlg = NULL;

char* GetTimeStr()
{
	static char tmp[64];
	static time_t timep = 0;
	time(&timep);
	strftime(tmp, sizeof(tmp), "%m-%d %H:%M:%S > ", localtime(&timep));
	return tmp;
}

CMyClientDlg* CMyClientDlg::CreateInstance(CWnd* pParent/*=NULL*/)
{
	if (!s_pMainDlg) {
		s_pMainDlg = new CMyClientDlg(pParent);
	}
	return s_pMainDlg;
}

void CMyClientDlg::AddLog(const char* szLog, ...)
{
	va_list ap;
	va_start(ap, szLog);

	std::string strLog = GetTimeStr();
	char tmpLog[512] = {0};	
	vsprintf(tmpLog, szLog, ap);
	strLog += tmpLog;

	s_pListBoxLog->AddString(strLog.c_str());
	s_pListBoxLog->SetCurSel(CMyClientDlg::s_pListBoxLog->GetCount() - 1);
	s_pMainDlg->UpdateData(TRUE);
	va_end(ap);
}

// -----------------------------------------
CMyClientDlg::CMyClientDlg(CWnd * pParent)
	: CDialog(IDD_DIALOG)
	, m_CtrlGoalMoney(0)
	, m_CtrlLevel(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_iEnterKindID = 3;
	m_iEnterNameID = 0;
	m_iEnterRoomID = 0;

	::memset(&m_PlaceUserInfo, 0, sizeof(m_PlaceUserInfo));

	m_bAutoDesk = true;
	m_bRandomSitDesk = true;
	m_pTCPSocket = new CTCPClientSocket(this);
}

CMyClientDlg::~CMyClientDlg()
{
	auto itorMap = m_mapRoomItem.begin();
	for (; itorMap != m_mapRoomItem.end(); ++itorMap)
	{
		itorMap->second->DestroyWindow();
		delete itorMap->second;
	}

	::WSACleanup();
}

void CMyClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_ListLog);
	DDX_Text(pDX, IDC_EDIT_GOBLCTRL, m_CtrlGoalMoney);
	DDV_MinMaxInt(pDX, m_CtrlGoalMoney, 0, 100000000);
	DDX_Text(pDX, IDC_EDIT_CTRL_LEVEL, m_CtrlLevel);
	DDV_MinMaxInt(pDX, m_CtrlLevel, 0, 100);
}

BEGIN_MESSAGE_MAP(CMyClientDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_SIZE()//////////
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SYSCOMMAND()
	ON_MESSAGE(IDM_CLOSE_GAME_ROOM, OnCloseGameRoom)
	ON_MESSAGE(IDM_LOGON_SUCESS, OnLogonSucess)
	ON_MESSAGE(IDM_LOGON_FAIL, OnLogonFail)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_LOG, &CMyClientDlg::OnBnClickedButtonClearLog)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BTN_START_ROLL, &CMyClientDlg::OnBnClickedBtnStartRoll)
	ON_BN_CLICKED(IDC_BTN_SEND_CTRL, &CMyClientDlg::OnBnClickedBtnSendCtrl)
END_MESSAGE_MAP()


BOOL CMyClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetClassLong(m_hWnd, GCL_HBRBACKGROUND, NULL);

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图

	SetDlgItemText(IDC_EDIT_KINDID, "1");
	SetDlgItemInt(IDC_EDIT_GAME_NAMEID, NAME_ID);
	SetDlgItemInt(IDC_EDIT_ROOMID, ROOM_ID);

	m_CtrlGoalMoney = 1000;
	m_CtrlLevel = 30;

	s_pListBoxLog = &m_ListLog;
	StartLogonHall();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CMyClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMyClientDlg::OnBnClickedButtonClearLog()
{
	m_ListLog.ResetContent();
}

void CMyClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TIME_CHECK_SOCKET_ERROR:		// socket错误检查
	{
		if (m_bLogoning && m_pTCPSocket->GetSocketState() == NO_CONNECT)
		{
			ADD_LOG("socket连接异常！");
		}
		break;
	}
	case TIME_DELAY_LOGON:			// 延时登陆
	{
		KillTimer(nIDEvent);
		ConnectToGateServer();
		break;
	}
	case TIMER_CHECK_GATECONNECT:
	{
		KillTimer(TIMER_CHECK_GATECONNECT);
		MessageBox("Gate服务器连接超时");
		break;
	}
	}
	return __super::OnTimer(nIDEvent);
}


void CMyClientDlg::StartLogonHall()
{
	UpdateData(false);

	char sz[200] = { 0 };
	GetDlgItemText(IDC_EDIT_KINDID, sz, sizeof(sz));
	m_iEnterKindID = atoi(sz);

	GetDlgItemText(IDC_EDIT_GAME_NAMEID, sz, sizeof(sz));
	m_iEnterNameID = atoi(sz);

	GetDlgItemText(IDC_EDIT_ROOMID, sz, sizeof(sz));
	m_iEnterRoomID = atoi(sz);

	// 网络连接错误检查
	KillTimer(TIME_CHECK_SOCKET_ERROR);
	SetTimer(TIME_CHECK_SOCKET_ERROR, TIMEROUT_CHECK_SOCKET_ERROR, NULL);
	SetTimer(TIME_DELAY_LOGON, TIMEROUT_DELAY_LOGON, NULL);
}

//连接登陆服务器
void CMyClientDlg::ConnectToGateServer()
{
	if (!m_pTCPSocket->Connect(GATE_IP, GATE_PORT1)) {
		MessageBox("Gate服务器连接失败");
	}
	else {
		SetTimer(TIMER_CHECK_GATECONNECT, TIMEROUT_CHECK_GATECONNECT, NULL);
	}
}

//发送网络消息函数
int CMyClientDlg::SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	return m_pTCPSocket->SendData(bMainID, bAssistantID, bHandleCode);//处理主类型
}

//发送网络消息函数
int CMyClientDlg::SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	return m_pTCPSocket->SendData(pData, uBufLen, bMainID, bAssistantID, bHandleCode);
}

//网络读取消息
bool CMyClientDlg::OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{

	switch (pNetHead->bMainID)
	{
	case MDM_CONNECT:	//连接消息 
	{
		return OnConnectMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	case MDM_GP_LOGON:	//登陆信息
	{
		return OnLogonMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	case MDM_GP_LIST:	//游戏列表
	{
		return OnListMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	case MDM_GP_MESSAGE://房间的消息 新闻，小喇叭
	{
		return true;
	}
	}
	return true;
}

//网络连接消息
bool CMyClientDlg::OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket)
{
	KillTimer(TIMER_CHECK_GATECONNECT);
	int i = 0;
	m_pTCPSocket->SendData(&i, sizeof(int), MDM_CONNECT, ASS_NET_GET, NOENCRYPTION);
	return true;
}

//网络关闭消息
bool CMyClientDlg::OnSocketCloseEvent()
{
	return true;
}

//连接消息处理
bool CMyClientDlg::OnConnectMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bAssistantID == ASS_NET_TEST)	//网络测试信息
	{
		return true;
	}
	else if ((pNetHead->bAssistantID == ASS_CONNECT_SUCCESS))	//连接成功
	{
		MSG_S_ConnectSuccess * pData = (MSG_S_ConnectSuccess *)pNetData;
		if (pData != NULL) {
			pClientSocket->SetCheckCode(pData->i64CheckCode, SECRET_KEY);
		}

		ADD_LOG("Gate服务器连接成功");
		UserLogonHall();
		return true;
	}
	return false;
}

CString CMyClientDlg::GetMacAddress()
{
	CString retMac = "";
	IP_ADAPTER_INFO* pAdapterInfo = new IP_ADAPTER_INFO; 
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS)
	{
		delete pAdapterInfo;
		pAdapterInfo = (IP_ADAPTER_INFO*)new char(ulOutBufLen);
	}

	DWORD dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
	if (dwRetVal  == NO_ERROR)
	{
		IP_ADAPTER_INFO* pAdapter = pAdapterInfo;
		CString temp;
		while (pAdapter)
		{
			//pAdapter->Description中包含"PCI"为：物理网卡//pAdapter->Type是71为：无线网卡
			if (pAdapter->Type == 6)
			{
				for (UINT i = 0; i < pAdapter->AddressLength; i++)
				{
					temp.Format("%02X", pAdapter->Address[i]);
					retMac += temp;
				}
				break;
			}
			pAdapter = pAdapter->Next;
		}
	}

	if (pAdapterInfo) {
		delete pAdapterInfo;
	}

	return retMac;
}

//获取机器码相关函数 
CString CMyClientDlg::GetMachineCode()
{
	CString strRet = "";
	CString macAddress = GetMacAddress();
	if (macAddress == "") {
		return strRet;
	}

	CString s = "";
	int r = 0;
	int l = macAddress.GetLength();
	for (int i = 0; i < l; i += 2)
	{
		int r1 = 0, r2 = 0;
		s = macAddress.Mid(i, 1);
		if (s >= "0" && s <= "9")
			r1 = atoi(s);
		if (s == "A" || s == "a")	
			r1 = 10;
		if (s == "B" || s == "b")	
			r1 = 11;
		if (s == "C" || s == "c")	
			r1 = 12;
		if (s == "D" || s == "d")	
			r1 = 13;
		if (s == "E" || s == "e")	
			r1 = 14;
		if (s == "F" || s == "f")	
			r1 = 15;
		s = macAddress.Mid(i + 1, 1);
		if (s >= "0" && s <= "9")
			r2 = atoi(s);

		if (s == "A" || s == "a")	
			r2 = 10;
		if (s == "B" || s == "b")	
			r2 = 11;
		if (s == "C" || s == "c")	
			r2 = 12;
		if (s == "D" || s == "d")	
			r2 = 13;
		if (s == "E" || s == "e")	
			r2 = 14;
		if (s == "F" || s == "f")	
			r2 = 15;

		r += r1 * 16 + r2;
		srand(r);

		CString t;
		t.Format("%s%04X%s-", macAddress.Mid(l - i - 1, 1), rand(), macAddress.Mid(l - i - 2, 1));
		strRet += t;
	}
	if (strRet.Right(1) == "-") {
		strRet = strRet.Left(strRet.GetLength() - 1);
	}

	return strRet;
}

//发送玩家消息
BOOL CMyClientDlg::UserLogonHall()
{
	int rNum = 0;
	CString TML_SN = "";
	CString allword = "0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
	for (int i = 0; i <= 120; i++)
	{
		rNum = (rand() * 1000) % 62;
		TML_SN += allword[rNum];
	}

	CString MatchineCode = GetMachineCode();

	MSG_GP_S_LogonByNameStruct LogonByName;
	::memset(&LogonByName, 0, sizeof(LogonByName));
	LogonByName.uRoomVer = GAME_PLACE_MAX_VER;
	lstrcpy(LogonByName.TML_SN, TML_SN);
	lstrcpy(LogonByName.szName, ACCOUNT);
	lstrcpy(LogonByName.szMD5Pass, PWD);
	lstrcpy(LogonByName.szMathineCode, MatchineCode);
	LogonByName.gsqPs = 100000000;
	LogonByName.szdevice_type = 100000000;
	LogonByName.uRoomVer = 100000000;
	m_pTCPSocket->SendData(&LogonByName, sizeof(LogonByName), MDM_GP_LOGON, ASS_GP_LOGON_BY_NAME, 0);

	return true;
}

//登陆消息处理
bool CMyClientDlg::OnLogonMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bAssistantID == ASS_GP_LOGON_SUCCESS)		//登陆成功
	{
		//设置信息
		m_PlaceUserInfo = *((MSG_GP_R_LogonResult *)pNetData);
		lstrcpy(m_PlaceUserInfo.szMD5Pass, PWD);

		//获取游戏列表
		m_pTCPSocket->SendData(MDM_GP_LIST, ASS_GP_LIST_KIND, 0);
		return true;
	}
	else if (pNetHead->bAssistantID == ASS_GP_LOGON_ERROR)	//登陆失败
	{
		static map<int, string> mapErrMsg = {
			{ERR_GP_USER_NO_FIND, "错误帐号！"},
			{ERR_GP_USER_PASS_ERROR , "密码错误！"},
			{ERR_GP_USER_VALIDATA , "此帐号已被禁止登录！"},
			{ERR_GP_USER_IP_LIMITED, "你所在的 IP 地址被禁止登陆！"},
			{ERR_GP_USER_EXIST, "很抱歉，这个用户名已经有人注册了！"},
			{ERR_GP_PASS_LIMITED, "密码长度错误！"},
			{ERR_GP_IP_NO_ORDER, "你需要在指定的 IP 地址的系统登陆！"},
			{ERR_GP_USER_PASS_ERROR , "已经登录"},
			{ERR_GP_USERNICK_EXIST, "昵称已存在"},
		};

		auto itorErr = mapErrMsg.find(pNetHead->bHandleCode);
		if (itorErr != mapErrMsg.end()) {
			ADD_LOG("登录失败：%s", itorErr->second);
		}
		else {
			ADD_LOG("登录失败，未知错误 %d", pNetHead->bHandleCode);
		}

		//重新连接
		pClientSocket->CloseSocket(true);
		return true;
	}

	return true;
}

//列表信息
bool CMyClientDlg::OnListMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_GP_LIST_KIND:		//获取的是游戏类型
	{
		m_GameList.AddGameKind((ComKindInfo *)pNetData, uDataSize / sizeof(ComKindInfo));
		return true;
	}
	case ASS_GP_LIST_NAME:		//获取的游戏名字
	{
		//处理数据
		m_GameList.AddGameName((ComNameInfo *)pNetData, uDataSize / sizeof(ComNameInfo));

		//更新提示
		if (pNetHead->bHandleCode == ERR_GP_LIST_FINISH) //发送游戏类型列表结束标志
		{
			MSG_GP_SR_GetRoomStruct GetList;
			GetList.uKindID = m_iEnterKindID;
			GetList.uNameID = m_iEnterNameID;
			m_pTCPSocket->SendData(&GetList, sizeof(GetList), MDM_GP_LIST, ASS_GP_LIST_ROOM, 0);
		}
		return true;
	}
	case ASS_GP_LIST_ROOM:		//获取的游戏房间
	{
		//效验数据 
		if (uDataSize < sizeof(MSG_GP_SR_GetRoomStruct)) {
			return false;
		}

		MSG_GP_SR_GetRoomStruct* pNetRoomBuf = (MSG_GP_SR_GetRoomStruct *)pNetData;
		ComRoomInfo* pData = (ComRoomInfo*)((char*)pNetData + sizeof(MSG_GP_SR_GetRoomStruct));

		int nCnt = (uDataSize - sizeof(MSG_GP_SR_GetRoomStruct)) / sizeof(ComRoomInfo);
		m_GameList.AddGameRoom(pData, nCnt, pNetRoomBuf->uKindID, pNetRoomBuf->uNameID);

		if (pNetHead->bHandleCode == ERR_GP_LIST_FINISH)
		{
			EnterRoom(m_iEnterRoomID);
		}
		return true;
	}
	case ASS_GP_LIST_COUNT:		//在线人数
	{
		return true;
	}
	}
	return TRUE;
}

//进入房间
bool CMyClientDlg::EnterRoom(int roomid)
{
	if (m_mapRoomItem.find(roomid) != m_mapRoomItem.end())
	{
		return false;
	}

	RoomInfo *pRoomInfo = m_GameList.FindRoomInfo(roomid);
	if (!pRoomInfo)
	{
		ADD_LOG("找不到房间 %d", roomid);
		return false;
	}

	return CreateGameRoom(pRoomInfo);
}

//进入游戏房间
bool CMyClientDlg::CreateGameRoom(RoomInfo* pRoomInfo)
{
	//判断是否特殊房间
	if (pRoomInfo->uComType != TY_NORMAL_GAME && 
		pRoomInfo->uComType != TY_MATCH_GAME && 
		pRoomInfo->uComType != TY_MONEY_GAME)
	{
		ADD_LOG("游戏类型错误：comType = %d", pRoomInfo->uComType);
		return false;
	}
	//判断游戏是否安装并效验版本
	TCHAR szProessName[31], szGameName[61];
	m_GameList.GetGameName(pRoomInfo->uRoomID, szGameName, sizeof(szGameName));
	m_GameList.GetProcessName(pRoomInfo->uRoomID, szProessName, sizeof(szProessName));

	try {

		CGameRoomEx* pRoomInfoItem = new CGameRoomEx(szGameName, pRoomInfo, &m_PlaceUserInfo, m_bAutoDesk, m_bRandomSitDesk);
		if (!pRoomInfoItem->Create(IDD_GAME_ROOM, this))
		{
			delete pRoomInfoItem;
			throw TEXT("CGameRoomEx create 失败");
		}

		m_mapRoomItem.insert(make_pair(pRoomInfo->uRoomID, pRoomInfoItem));
	}
	catch (...)
	{
		return false;
	}

	ADD_LOG("游戏房间创建成功 %d", pRoomInfo->uRoomID);
	ADD_LOG("大厅socket断开");
	m_pTCPSocket->CloseSocket();
	KillTimer(TIME_CHECK_SOCKET_ERROR);

	return true;
}

//登陆房间成功
LRESULT CMyClientDlg::OnLogonSucess(WPARAM wparam, LPARAM lparam)
{
	int iUserID = (int)lparam;
	char szLog[512] = { 0 };

	return 0;
}

//关闭房间请求
LRESULT CMyClientDlg::OnCloseGameRoom(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

// 登录房间失败
LRESULT CMyClientDlg::OnLogonFail(WPARAM wparam, LPARAM lparam)
{
	int iUserID = (int)lparam;
	RoomInfo* pRoomInfo = (RoomInfo*)wparam;
	if (pRoomInfo) {
		ADD_LOG("%d 登陆房间 [%d] 失败", iUserID, pRoomInfo->uRoomID);
	}
	else {
		ADD_LOG("%d 登陆房间 [] 失败", iUserID);
	}
	return 0;
}

void CMyClientDlg::OnBnClickedBtnStartRoll()
{
	auto itorMap = m_mapRoomItem.find(ROOM_ID);
	if (itorMap != m_mapRoomItem.end())
	{
		itorMap->second->PostMessageA(IDM_START_ROLL);
	}
};


void CMyClientDlg::OnBnClickedBtnSendCtrl()
{
	UpdateData(FALSE);
	auto itorMap = m_mapRoomItem.find(ROOM_ID);
	if (itorMap != m_mapRoomItem.end())
	{
		itorMap->second->PostMessageA(IDM_SEND_CTRL, m_CtrlGoalMoney, m_CtrlLevel);
	}
}
