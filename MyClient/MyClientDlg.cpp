#include "stdafx.h"
#include "MyClient.h"
#include "MyClientDlg.h"
#include "iphlpapi.h"	
#include <stdarg.h>

// GetAdaptersInfo ����������������ʵ��
#pragma comment(lib,"iphlpapi.lib")

// ��̬-----------------------------------
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ

	SetDlgItemText(IDC_EDIT_KINDID, "1");
	SetDlgItemInt(IDC_EDIT_GAME_NAMEID, NAME_ID);
	SetDlgItemInt(IDC_EDIT_ROOMID, ROOM_ID);

	m_CtrlGoalMoney = 1000;
	m_CtrlLevel = 30;

	s_pListBoxLog = &m_ListLog;
	StartLogonHall();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMyClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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
	case TIME_CHECK_SOCKET_ERROR:		// socket������
	{
		if (m_bLogoning && m_pTCPSocket->GetSocketState() == NO_CONNECT)
		{
			ADD_LOG("socket�����쳣��");
		}
		break;
	}
	case TIME_DELAY_LOGON:			// ��ʱ��½
	{
		KillTimer(nIDEvent);
		ConnectToGateServer();
		break;
	}
	case TIMER_CHECK_GATECONNECT:
	{
		KillTimer(TIMER_CHECK_GATECONNECT);
		MessageBox("Gate���������ӳ�ʱ");
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

	// �������Ӵ�����
	KillTimer(TIME_CHECK_SOCKET_ERROR);
	SetTimer(TIME_CHECK_SOCKET_ERROR, TIMEROUT_CHECK_SOCKET_ERROR, NULL);
	SetTimer(TIME_DELAY_LOGON, TIMEROUT_DELAY_LOGON, NULL);
}

//���ӵ�½������
void CMyClientDlg::ConnectToGateServer()
{
	if (!m_pTCPSocket->Connect(GATE_IP, GATE_PORT1)) {
		MessageBox("Gate����������ʧ��");
	}
	else {
		SetTimer(TIMER_CHECK_GATECONNECT, TIMEROUT_CHECK_GATECONNECT, NULL);
	}
}

//����������Ϣ����
int CMyClientDlg::SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	return m_pTCPSocket->SendData(bMainID, bAssistantID, bHandleCode);//����������
}

//����������Ϣ����
int CMyClientDlg::SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	return m_pTCPSocket->SendData(pData, uBufLen, bMainID, bAssistantID, bHandleCode);
}

//�����ȡ��Ϣ
bool CMyClientDlg::OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{

	switch (pNetHead->bMainID)
	{
	case MDM_CONNECT:	//������Ϣ 
	{
		return OnConnectMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	case MDM_GP_LOGON:	//��½��Ϣ
	{
		return OnLogonMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	case MDM_GP_LIST:	//��Ϸ�б�
	{
		return OnListMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	case MDM_GP_MESSAGE://�������Ϣ ���ţ�С����
	{
		return true;
	}
	}
	return true;
}

//����������Ϣ
bool CMyClientDlg::OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket)
{
	KillTimer(TIMER_CHECK_GATECONNECT);
	int i = 0;
	m_pTCPSocket->SendData(&i, sizeof(int), MDM_CONNECT, ASS_NET_GET, NOENCRYPTION);
	return true;
}

//����ر���Ϣ
bool CMyClientDlg::OnSocketCloseEvent()
{
	return true;
}

//������Ϣ����
bool CMyClientDlg::OnConnectMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bAssistantID == ASS_NET_TEST)	//���������Ϣ
	{
		return true;
	}
	else if ((pNetHead->bAssistantID == ASS_CONNECT_SUCCESS))	//���ӳɹ�
	{
		MSG_S_ConnectSuccess * pData = (MSG_S_ConnectSuccess *)pNetData;
		if (pData != NULL) {
			pClientSocket->SetCheckCode(pData->i64CheckCode, SECRET_KEY);
		}

		ADD_LOG("Gate���������ӳɹ�");
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
			//pAdapter->Description�а���"PCI"Ϊ����������//pAdapter->Type��71Ϊ����������
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

//��ȡ��������غ��� 
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

//���������Ϣ
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

//��½��Ϣ����
bool CMyClientDlg::OnLogonMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bAssistantID == ASS_GP_LOGON_SUCCESS)		//��½�ɹ�
	{
		//������Ϣ
		m_PlaceUserInfo = *((MSG_GP_R_LogonResult *)pNetData);
		lstrcpy(m_PlaceUserInfo.szMD5Pass, PWD);

		//��ȡ��Ϸ�б�
		m_pTCPSocket->SendData(MDM_GP_LIST, ASS_GP_LIST_KIND, 0);
		return true;
	}
	else if (pNetHead->bAssistantID == ASS_GP_LOGON_ERROR)	//��½ʧ��
	{
		static map<int, string> mapErrMsg = {
			{ERR_GP_USER_NO_FIND, "�����ʺţ�"},
			{ERR_GP_USER_PASS_ERROR , "�������"},
			{ERR_GP_USER_VALIDATA , "���ʺ��ѱ���ֹ��¼��"},
			{ERR_GP_USER_IP_LIMITED, "�����ڵ� IP ��ַ����ֹ��½��"},
			{ERR_GP_USER_EXIST, "�ܱ�Ǹ������û����Ѿ�����ע���ˣ�"},
			{ERR_GP_PASS_LIMITED, "���볤�ȴ���"},
			{ERR_GP_IP_NO_ORDER, "����Ҫ��ָ���� IP ��ַ��ϵͳ��½��"},
			{ERR_GP_USER_PASS_ERROR , "�Ѿ���¼"},
			{ERR_GP_USERNICK_EXIST, "�ǳ��Ѵ���"},
		};

		auto itorErr = mapErrMsg.find(pNetHead->bHandleCode);
		if (itorErr != mapErrMsg.end()) {
			ADD_LOG("��¼ʧ�ܣ�%s", itorErr->second);
		}
		else {
			ADD_LOG("��¼ʧ�ܣ�δ֪���� %d", pNetHead->bHandleCode);
		}

		//��������
		pClientSocket->CloseSocket(true);
		return true;
	}

	return true;
}

//�б���Ϣ
bool CMyClientDlg::OnListMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_GP_LIST_KIND:		//��ȡ������Ϸ����
	{
		m_GameList.AddGameKind((ComKindInfo *)pNetData, uDataSize / sizeof(ComKindInfo));
		return true;
	}
	case ASS_GP_LIST_NAME:		//��ȡ����Ϸ����
	{
		//��������
		m_GameList.AddGameName((ComNameInfo *)pNetData, uDataSize / sizeof(ComNameInfo));

		//������ʾ
		if (pNetHead->bHandleCode == ERR_GP_LIST_FINISH) //������Ϸ�����б������־
		{
			MSG_GP_SR_GetRoomStruct GetList;
			GetList.uKindID = m_iEnterKindID;
			GetList.uNameID = m_iEnterNameID;
			m_pTCPSocket->SendData(&GetList, sizeof(GetList), MDM_GP_LIST, ASS_GP_LIST_ROOM, 0);
		}
		return true;
	}
	case ASS_GP_LIST_ROOM:		//��ȡ����Ϸ����
	{
		//Ч������ 
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
	case ASS_GP_LIST_COUNT:		//��������
	{
		return true;
	}
	}
	return TRUE;
}

//���뷿��
bool CMyClientDlg::EnterRoom(int roomid)
{
	if (m_mapRoomItem.find(roomid) != m_mapRoomItem.end())
	{
		return false;
	}

	RoomInfo *pRoomInfo = m_GameList.FindRoomInfo(roomid);
	if (!pRoomInfo)
	{
		ADD_LOG("�Ҳ������� %d", roomid);
		return false;
	}

	return CreateGameRoom(pRoomInfo);
}

//������Ϸ����
bool CMyClientDlg::CreateGameRoom(RoomInfo* pRoomInfo)
{
	//�ж��Ƿ����ⷿ��
	if (pRoomInfo->uComType != TY_NORMAL_GAME && 
		pRoomInfo->uComType != TY_MATCH_GAME && 
		pRoomInfo->uComType != TY_MONEY_GAME)
	{
		ADD_LOG("��Ϸ���ʹ���comType = %d", pRoomInfo->uComType);
		return false;
	}
	//�ж���Ϸ�Ƿ�װ��Ч��汾
	TCHAR szProessName[31], szGameName[61];
	m_GameList.GetGameName(pRoomInfo->uRoomID, szGameName, sizeof(szGameName));
	m_GameList.GetProcessName(pRoomInfo->uRoomID, szProessName, sizeof(szProessName));

	try {

		CGameRoomEx* pRoomInfoItem = new CGameRoomEx(szGameName, pRoomInfo, &m_PlaceUserInfo, m_bAutoDesk, m_bRandomSitDesk);
		if (!pRoomInfoItem->Create(IDD_GAME_ROOM, this))
		{
			delete pRoomInfoItem;
			throw TEXT("CGameRoomEx create ʧ��");
		}

		m_mapRoomItem.insert(make_pair(pRoomInfo->uRoomID, pRoomInfoItem));
	}
	catch (...)
	{
		return false;
	}

	ADD_LOG("��Ϸ���䴴���ɹ� %d", pRoomInfo->uRoomID);
	ADD_LOG("����socket�Ͽ�");
	m_pTCPSocket->CloseSocket();
	KillTimer(TIME_CHECK_SOCKET_ERROR);

	return true;
}

//��½����ɹ�
LRESULT CMyClientDlg::OnLogonSucess(WPARAM wparam, LPARAM lparam)
{
	int iUserID = (int)lparam;
	char szLog[512] = { 0 };

	return 0;
}

//�رշ�������
LRESULT CMyClientDlg::OnCloseGameRoom(WPARAM wparam, LPARAM lparam)
{
	return 0;
}

// ��¼����ʧ��
LRESULT CMyClientDlg::OnLogonFail(WPARAM wparam, LPARAM lparam)
{
	int iUserID = (int)lparam;
	RoomInfo* pRoomInfo = (RoomInfo*)wparam;
	if (pRoomInfo) {
		ADD_LOG("%d ��½���� [%d] ʧ��", iUserID, pRoomInfo->uRoomID);
	}
	else {
		ADD_LOG("%d ��½���� [] ʧ��", iUserID);
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
