#include "StdAfx.h"
#include "GameRoomEx.h"
#include "gameroommessage.h"
#include "UpgradeMessage.h"
#include "MyClientDlg.h"


#define GS_WAIT_SETGAME					0				//�ȴ���������״̬
#define GS_WAIT_ARGEE					1				//�ȴ�ͬ������
#define GS_PLAY_GAME					20				//����״̬
#define GS_WAIT_BACK					21				//�ȴ���ѹ����
#define GS_WAIT_NEXT					23				//�ȴ���һ�̿�ʼ 

HINSTANCE CGameRoomEx::hGameDllInstance = NULL;

//DLL �ӿں�������
typedef IFrameInterface* (CreateAIGamesInterface)();
typedef void (FreeAIGamesInterface)(IFrameInterface* & pGameFrame);

static CreateAIGamesInterface* g_pCreateAIGamesInterface = NULL;
static FreeAIGamesInterface*   g_pFreeAIGamesInterface = NULL;

BEGIN_MESSAGE_MAP(CGameRoomEx, CDialog)
	ON_WM_COPYDATA()
	ON_WM_TIMER()
	ON_MESSAGE(IDM_SET_GAME_TIME, OnSetGameTime)
	ON_MESSAGE(IDM_KILL_GAME_TIME, OnKillGameTime)
	ON_MESSAGE(IDM_CLOSE, OnCloseGame)
	ON_MESSAGE(IDM_STAND_UP, OnStandingDesk)
	ON_MESSAGE(IDM_GET_SCORE, OnGetScore)
	ON_MESSAGE(IDM_START_ROLL, OnStartRoll)
	ON_MESSAGE(IDM_SEND_CTRL, OnSendCtrl)
END_MESSAGE_MAP()


//���캯��
CGameRoomEx::CGameRoomEx(TCHAR* szGameName, RoomInfo* pRoomInfo, MSG_GP_R_LogonResult* pLogonInfo, bool bAutoDesk, bool bRandomSitDesk)
	: CDialog(IDD_GAME_ROOM)
{
	m_IGameFrame = NULL;
	m_IsCloseSocket = false;
	m_bAutoDesk = bAutoDesk;
	m_bRandomSitDesk = bRandomSitDesk;
	m_bNowDesk = -1;
	memset(&m_LogonInfo, 0, sizeof(m_LogonInfo));

	m_pRoomInfo = pRoomInfo;
	memset(&m_GameInfo, 0, sizeof(m_GameInfo));
	lstrcpy(m_GameInfo.szGameName, szGameName);
	m_GameInfo.pIFindUser = (IFindUserInterface*)(&m_UserManage);
	m_GameInfo.bShowUserInfo = true;
	m_GameInfo.bEnableSound = true;
	m_GameInfo.bEnableWatch = false;
	m_GameInfo.uNameID = pRoomInfo->uNameID;
	m_GameInfo.uRoomID = pRoomInfo->uRoomID;
	m_GameInfo.uComType = pRoomInfo->uComType;
	m_GameInfo.uDeskPeople = pRoomInfo->uDeskPeople;
	m_GameInfo.dwRoomRule = pRoomInfo->dwRoomRule;
	m_IsUseBank = FALSE;
	m_bWillCloseRoom = false;
	m_nRunCnt = 0;

	m_TCPSocket = new CTCPClientSocket(this);
	CopyMemory(&m_LogonInfo, (MSG_GP_R_LogonResult*)pLogonInfo, sizeof(m_LogonInfo));

	return;
}

//��������
CGameRoomEx::~CGameRoomEx()
{
	if (m_IGameFrame != NULL)
	{
		delete m_IGameFrame;
		m_IGameFrame = NULL;
	}

	if (m_TCPSocket != NULL)
	{
		m_IsCloseSocket = true;
		delete m_TCPSocket;
		m_TCPSocket = NULL;
	}

	m_UserManage.CleanUserInfo();
}

//��ʼ������
BOOL CGameRoomEx::OnInitDialog()
{
	CDialog::OnInitDialog();
	ConnectToGameServer();
	return TRUE;
}

//��½��Ϸ����
void  CGameRoomEx::ConnectToGameServer()
{
	if (NULL != m_TCPSocket)
	{
		if (!m_TCPSocket->Connect(GATE_IP, GATE_PORT2))
		{
			ADD_LOG("��Ϸ���䣺Gate����ʧ��");
		}
	}

	return;
}

//����������Ϣ
bool CGameRoomEx::OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket)
{
	if (uErrorCode != 0)	//���ӳ��ִ���
	{
		ADD_LOG("���ӳ�����");
		SetTimer(TIMER_CHECK_RECONNET, TIMEOUT_CHECK_RECONNECT, NULL);
	}
	else {
		int i = ROOM_ID;
		m_TCPSocket->SendData(&i, sizeof(int), MDM_CONNECT, ASS_NET_GET, NOENCRYPTION);
	}
	return true;
}

//�����ȡ��Ϣ
bool CGameRoomEx::OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{

	switch (pNetHead->bMainID)
	{
	case MDM_CONNECT:			//������Ϣ,׼�����뷿�� 
	case MDM_GP_CONNECT:
	{
		return OnConnectMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	case MDM_GR_LOGON:			//�����½
	{
		return OnLogonMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	case MDM_GR_USER_LIST:		//�û��б�
	{
		return OnUserListMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	case MDM_GR_USER_ACTION:	//�û�����
	{
		return OnUserActionMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	case MDM_GR_ROOM:			//������Ϣ
	{
		return OnRoomMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	case MDM_GR_MESSAGE:		//ϵͳ��Ϣ
	{
		return OnSystemMsgMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	case MDM_GM_GAME_FRAME:		//��Ϸ���
	{
		return OnGameFrameMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	case MDM_GM_GAME_NOTIFY:	//��Ϸ��Ϣ
	{
		return OnGameMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	}
	//��Ϸ��Ϣ
	//if (m_IGameFrame != NULL)
	//{
	//	return m_IGameFrame->OnSocketReadEvent(pNetHead, pNetData, uDataSize, pClientSocket);
	//}


	return true;
}

//����ر���Ϣ
bool CGameRoomEx::OnSocketCloseEvent()
{
	try
	{
		if (m_IGameFrame)
		{
			m_IGameFrame->AFCCloseFrame();
			FreeIGameFrame(m_IGameFrame);
		}
	}
	catch (...)
	{
		if (m_IGameFrame != NULL)
		{
			FreeIGameFrame(m_IGameFrame);
		}
	}

	ADD_LOG("socket close : ");
	m_UserManage.DeleteGameUser(m_LogonInfo.dwUserID);
	::PostMessage(GetParent()->m_hWnd, IDM_CLOSE_GAME_ROOM, (WPARAM)m_pRoomInfo, m_LogonInfo.dwUserID);

	return true;
}

//�رշ���
void CGameRoomEx::CloseGameRoom()
{
	if (m_IGameFrame != NULL)
	{
		m_IGameFrame->KillAllTimer();
	}
	m_TCPSocket->CloseSocket(false);
	try
	{
		if (m_IGameFrame)
		{
			m_IGameFrame->AFCCloseFrame();
			FreeIGameFrame(m_IGameFrame);
		}
	}
	catch (...)
	{
		if (m_IGameFrame != NULL)
		{
			FreeIGameFrame(m_IGameFrame);
		}
	}	

	ADD_LOG("closeGameRoom : ")

	m_UserManage.DeleteGameUser(m_LogonInfo.dwUserID);
	::PostMessage(GetParent()->m_hWnd, IDM_CLOSE_GAME_ROOM, (WPARAM)m_pRoomInfo, m_LogonInfo.dwUserID);

	return;
}

//������Ϣ����
bool CGameRoomEx::OnConnectMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bAssistantID == ASS_NET_TEST)	//���������Ϣ
	{
		m_nConnTimeOutCnt = 0;
		return true;
	}
	else if ((pNetHead->bAssistantID == ASS_CONNECT_SUCCESS))	//���ӳɹ�
	{
		MSG_S_ConnectSuccess * pSuccess = (MSG_S_ConnectSuccess *)pNetData;
		if (pSuccess != NULL)
		{
			pClientSocket->SetCheckCode(pSuccess->i64CheckCode, SECRET_KEY);
		}

		ADD_LOG("��ϷRoom�����ӳɹ�");
		LogonGameServerByID();
		return true;
	}
	return true;
}

//001,�����½����,һ���뷿��  
bool CGameRoomEx::OnLogonMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_LOGON_SUCCESS:		//��½�ɹ�
	{
		//Ч������
		if (uDataSize != sizeof(MSG_GR_R_LogonResult)) {
			return false;
		}

		ADD_LOG("��½����ɹ�");

		MSG_GR_R_LogonResult * pLogonInfo = (MSG_GR_R_LogonResult *)pNetData;
		m_GameInfo.uLessPoint = pLogonInfo->uLessPoint;
		m_GameInfo.pMeUserInfo = m_UserManage.AddGameUser(&pLogonInfo->pUserInfoStruct, 1, 0);

		if (m_GameInfo.pMeUserInfo == NULL)
		{
			StandingDesk();
			return false;
		}

		::PostMessage(GetParent()->m_hWnd, IDM_LOGON_SUCESS, 0, m_LogonInfo.dwUserID);
		return true;
	}
	case ASS_GR_LOGON_ERROR:		//��½ʧ��
	{
		ADD_LOG("��½ʧ�� : %d", pNetHead->bHandleCode);

		::PostMessage(GetParent()->m_hWnd, IDM_LOGON_FAIL, 0, m_LogonInfo.dwUserID);
		return false;
	}
	case ASS_GR_SEND_FINISH:		//��½���
	{
		//������״̬����
		StartCheckStationTime();
		ADD_LOG("��½���");

		MatchToDesk();
		m_nRunCnt = 0;
		//SetTimer(TIMER_OPT, TIMEOUT_DO_OPT, NULL);
		return true;
	}
	case ASS_BEGIN_MATCH_ROOM:		// ƥ��ɹ�
	{
		//ADD_LOG("ƥ��ɹ�");
		return true;
	}
	case ERR_GR_NO_ENOUGH_ROOM_KEY:
	{
		StandingDesk();
	}

	}
	return true;
}


//����״̬����ʱ��
bool CGameRoomEx::StartCheckStationTime()
{
	SetTimer(TIMER_CHECK_CONNECT, TIMEOUT_CHECK_CONNECT, NULL);
	SetTimer(TIMER_CHECK_STATE, TIMEOUT_CHECK_STATE, NULL);
	return true;
}

//����
bool CGameRoomEx::StandingDesk()
{
	if (NULL != m_TCPSocket)
	{
		m_TCPSocket->SendData(MDM_GR_USER_ACTION, ASS_GR_USER_UP, 0);
	}
	return true;
}

//����
bool CGameRoomEx::SitToDesk(bool bSitNow)
{
	if (!m_TCPSocket) {
		return false;
	}

	if (m_GameInfo.dwRoomRule & GRR_QUEUE_GAME)
	{
		m_TCPSocket->SendData(MDM_GR_USER_ACTION, ASS_GR_JOIN_QUEUE, 0);
		return  true;
	}

	int bDeskIndex = -1;
	BYTE cbDeskStation = 255;
	if (m_bRandomSitDesk)
	{
		FindRandDesk(bDeskIndex, cbDeskStation);
	}
	else
	{
		FindTureDesk(bDeskIndex, cbDeskStation);
	}

	if (bDeskIndex == -1 || cbDeskStation == 255)
	{
		return true;
	}
	MSG_GR_S_UserSit UserSit;
	UserSit.bDeskIndex = bDeskIndex;
	UserSit.bDeskStation = cbDeskStation;
	m_bNowDesk = 0;
	m_TCPSocket->SendData(&UserSit, sizeof(UserSit), MDM_GR_USER_ACTION, ASS_GR_USER_SIT, 0);
	return true;
}

bool CGameRoomEx::MatchToDesk()
{
	MSG_GR_S_RoomMatch sendMsg;
	sendMsg.dwUserID = m_LogonInfo.dwUserID;
	sendMsg.iMatchType = 0;
	m_TCPSocket->SendData(&sendMsg, sizeof(sendMsg), MDM_GR_LOGON, ASS_BEGIN_MATCH_ROOM, 0);
	return true;
}

//�����һ��������
bool CGameRoomEx::FindRandDesk(int &bDeskIndex, BYTE &bDeskStation)
{
	srand(GetTickCount());
	int time = 0;
	UINT bDesk = rand() % m_pRoomInfo->uDeskCount;

	while (time++ < 5)
	{
		//��λ
		for (UINT station = 0; station < m_GameInfo.uDeskPeople; station++)
		{
			bool havepeople = false;
			for (int i = 0; i < m_UserManage.GetOnLineUserCount(); i++)
			{
				UserItemStruct * pFindUserItem = m_UserManage.FindOnLineUserByIndex(i);
				if (pFindUserItem == NULL)
				{
					continue;
				}

				if (pFindUserItem->GameUserInfo.bDeskNO == bDesk &&pFindUserItem->GameUserInfo.bDeskStation == station)//����λ�Ѿ�����
				{
					havepeople = true;
					break;
				}
			}
			if (havepeople != true)
			{
				bDeskIndex = bDesk;
				bDeskStation = station;
				return true;
			}
		}
	}
	return true;
}

//���ҿ�����
bool CGameRoomEx::FindNullDesk(int &bDeskIndex, BYTE &bDeskStation)
{
	//��ÿһ�����ӿ�ʼ����
	for (UINT desk = 0; desk < m_pRoomInfo->uDeskCount; desk++)
	{
		//��λ
		for (UINT station = 0; station < m_GameInfo.uDeskPeople; station++)
		{
			bool havepeople = false;
			for (int i = 0; i < m_UserManage.GetOnLineUserCount(); i++)
			{
				UserItemStruct * pFindUserItem = m_UserManage.FindOnLineUserByIndex(i);
				if (pFindUserItem == NULL)
					continue;
				if (pFindUserItem->GameUserInfo.bDeskNO == desk&&pFindUserItem->GameUserInfo.bUserState == USER_PLAY_GAME)//������Ϸ�Ѿ���ʼ
				{
					havepeople = true;
					break;
				}
				if (pFindUserItem->GameUserInfo.bDeskNO == desk &&pFindUserItem->GameUserInfo.bDeskStation == station)//��������
				{
					havepeople = true;
					break;
				}
			}
			if (havepeople != true)
			{
				bDeskIndex = desk;
				bDeskStation = station;
				return true;
			}
		}
	}
	return true;
}

//����ָ�����ӿ�λ��(���������ʱ����)
BYTE CGameRoomEx::GetNullStation(int bDeskIndex)
{
	BYTE bDeskStation = 255;
	//��λ
	for (UINT station = 0; station < m_GameInfo.uDeskPeople; station++)
	{
		bool havepeople = false;
		for (int i = 0; i < m_UserManage.GetOnLineUserCount(); i++)
		{
			UserItemStruct * pFindUserItem = m_UserManage.FindOnLineUserByIndex(i);
			if (pFindUserItem == NULL)
				continue;
			if (pFindUserItem->GameUserInfo.bDeskNO == bDeskIndex && pFindUserItem->GameUserInfo.bUserState == USER_PLAY_GAME)//������Ϸ�Ѿ���ʼ
			{
				havepeople = true;
				break;
			}
			if (pFindUserItem->GameUserInfo.bDeskNO == bDeskIndex &&pFindUserItem->GameUserInfo.bDeskStation == station)//��������
			{
				havepeople = true;
				break;
			}
		}
		if (havepeople != true)
		{
			bDeskStation = station;
			break;;
		}
	}
	return bDeskStation;
}
//����������������
bool CGameRoomEx::FindTureDesk(int &bDeskIndex, BYTE &bDeskStation)
{
	bDeskIndex = -1;
	bDeskStation = 255;
	//��ÿһ�����ӿ�ʼ����
	for (UINT desk = 0; desk < m_pRoomInfo->uDeskCount; desk++)
	{
		//��¼��Щλ����û��
		BYTE iTemp[180] = { 0 };

		//��λ
		for (UINT station = 0; station < m_GameInfo.uDeskPeople; station++)
		{
			bool havepeople = false;
			bool bVirtual = false;
			for (int i = 0; i < m_UserManage.GetOnLineUserCount(); i++)
			{
				UserItemStruct * pFindUserItem = m_UserManage.FindOnLineUserByIndex(i);
				if (pFindUserItem == NULL)
				{
					continue;
				}

				if (pFindUserItem->GameUserInfo.dwUserID < 0 && pFindUserItem->GameUserInfo.bDeskNO == desk)
				{
					bVirtual = true;
					break;
				}

				if (pFindUserItem->GameUserInfo.bDeskNO == desk && pFindUserItem->GameUserInfo.bDeskStation == station)//��������
				{
					havepeople = true;
					break;
				}
			}

			if (bVirtual)  //�������λ��������һ��
			{
				continue;
			}

			if (havepeople)
			{
				continue;
			}
			else
			{
				iTemp[station]++;
				if (bDeskIndex == -1)
				{
					bDeskIndex = desk;
					bDeskStation = station;
				}
			}
		}

		int iEtpStation[180] = { 0 };
		int iCount = 0;
		for (int i = 0; i < 180; i++)
		{
			if (iTemp[i] > 0)
			{
				iEtpStation[iCount] = i;
				iCount++;
			}
		}

		if (iCount == 0)
		{
			continue;
		}

		srand(GetTickCount());
		int iIndex = rand() % iCount;

		bDeskIndex = desk;
		bDeskStation = iEtpStation[iIndex];

		return true;
	}

	return true;
}

//�Ƿ���Ҫ������
bool CGameRoomEx::NeedChange(BYTE bDeskIndex)
{
	int count = 0;
	for (int i = 0; i < m_UserManage.GetOnLineUserCount(); i++)
	{
		UserItemStruct * pFindUserItem = m_UserManage.FindOnLineUserByIndex(i);
		if (pFindUserItem == NULL)
			continue;
		if (pFindUserItem->GameUserInfo.bDeskNO == bDeskIndex)//������Ϸ�Ѿ���ʼ
		{
			count++;
		}
	}
	if (count >= 2 || m_GameInfo.pMeUserInfo->GameUserInfo.bUserState == USER_PLAY_GAME)
		return false;

	return true;
}

//��������һ������
bool CGameRoomEx::FindOtherDesk()
{
	bool bFlag = true;
	SitToDesk(bFlag);
	return true;
}

bool CGameRoomEx::LogonGameServerByName()
{
	MSG_GR_S_RoomLogonByUserName RoomLogon;
	ZeroMemory(&RoomLogon, sizeof(RoomLogon));
	RoomLogon.uGameVer = 0;
	RoomLogon.uRoomVer = GAME_PLACE_MAX_VER;
	RoomLogon.uNameID = m_pRoomInfo->uNameID;
	RoomLogon.dwUserID = m_LogonInfo.dwUserID;
	RoomLogon.MaxScore = 0;
	RoomLogon.MinScore = 0;
	strcpy_s(RoomLogon.szName, ACCOUNT);
	strcpy_s(RoomLogon.szMD5Pass, PWD);
	m_TCPSocket->SendData(&RoomLogon, sizeof(RoomLogon), MDM_GR_LOGON, ASS_GR_LOGON_BY_USERNAME, 0);
	return true;
}

bool CGameRoomEx::LogonGameServerByID()
{
	ADD_LOG("���͵�½����");
	MSG_GR_S_RoomLogon logonMsg = { 0 };
	logonMsg.uNameID = m_pRoomInfo->uNameID;
	logonMsg.dwUserID = m_LogonInfo.dwUserID;
	logonMsg.uRoomVer = GAME_PLACE_MAX_VER;
	logonMsg.uGameVer = 1;
	logonMsg.IsReturn = 0;
	logonMsg.iGuildId = 0;
	logonMsg.iMatchId = 0;
	strcpy_s(logonMsg.szIP, "127.0.0.1");
	strcpy_s(logonMsg.szMD5Pass, PWD);
	m_TCPSocket->SendData(&logonMsg, sizeof(logonMsg), MDM_GR_LOGON, ASS_GR_LOGON_BY_ID, 0);
	return true;
}

//�û��б���
bool CGameRoomEx::OnUserListMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_ONLINE_USER:
	case ASS_GR_NETCUT_USER:	//�û��б�
	{
		UINT uUserCount = uDataSize / sizeof(UserInfoStruct);

		if (uUserCount > 0L)
		{
			//�������
			UserItemStruct * pUserItem = NULL;
			UserInfoStruct * pUserInfo = (UserInfoStruct *)pNetData;

			for (UINT i = 0; i < uUserCount; i++)
			{
				pUserItem = m_UserManage.AddGameUser(pUserInfo + i, 1, 0);
			}
		}
		return true;
	}
	case ASS_GR_DESK_STATION:	//����״̬ 
	{
		if (uDataSize != sizeof(MSG_GR_DeskStation)) return false;
		MSG_GR_DeskStation * pDeskStation = (MSG_GR_DeskStation *)pNetData;
		return true;
	}
	}
	return true;
}


//������Ϣ����
bool CGameRoomEx::OnRoomMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	ASSERT(pNetHead->bMainID == MDM_GR_ROOM);

	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_NORMAL_TALK:	//��ͨ����
	{
		return true;
	}
	case ASS_GR_SHORT_MSG:		//����Ϣ����
	{
		return true;
	}
	case ASS_GR_USER_AGREE:		//�û�ͬ��
	{
		return true;
	}
	case ASS_GR_GAME_BEGIN:		//��Ϸ��ʼ
	{
		//Ч������
		if (uDataSize != 0)
		{
			return false;
		}

		int bDeskIndex = pNetHead->bHandleCode;

		try
		{
			for (UINT i = 0; i < m_UserManage.GetOnLineUserCount(); i++)
			{
				UserItemStruct * pFindUserItem = m_UserManage.FindOnLineUserByIndex(i);
				if (pFindUserItem == NULL)
				{

					continue;
				}

				if (pFindUserItem != NULL)
				{
					if (pFindUserItem->GameUserInfo.bDeskNO == bDeskIndex)
					{
						pFindUserItem->GameUserInfo.bUserState = USER_PLAY_GAME;

					}
					else
					{

					}
				}
			}
		}
		catch (...)
		{

		}

		return true;
	}

	case ASS_GR_GAME_END:		//��Ϸ����
	{

		//Ч������
		if (uDataSize != 0)
		{
			return false;
		}

		bool bHaveplayer = false; //�Ƿ�����ʵ���
		//��������
		int bDeskIndex = pNetHead->bHandleCode;

		for (int i = 0; i < m_UserManage.GetOnLineUserCount(); i++)
		{
			UserItemStruct * pFindUserItem = m_UserManage.FindOnLineUserByIndex(i);
			if (pFindUserItem == NULL)
			{
				continue;
			}
			if (pFindUserItem->GameUserInfo.bDeskNO == bDeskIndex)
			{
				pFindUserItem->GameUserInfo.bUserState = USER_DESK_AGREE;

				if (pFindUserItem->GameUserInfo.isVirtual == false)
				{
					bHaveplayer = true;
				}
			}
		}

		StandingDesk();

		if (m_GameInfo.pMeUserInfo != NULL&&bDeskIndex == m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO)
		{
			if (rand() % 30 == 1)
			{
				if (m_bAutoDesk) {
					StandingDesk();
				}
			}
		}

		return true;
	}

	case ASS_GR_INSTANT_UPDATE:		//��ʱ���·������
	{
		return TRUE;
	}
	case ASS_GR_USER_POINT:		//�û���һ���
	{
		if (pNetHead->bHandleCode == 10)//ͬ�����
		{
			return true;
		}
		else if (pNetHead->bHandleCode == 11)//ͬ������ֵ
		{
			return true;
		}

		//Ч������
		if (uDataSize != sizeof(MSG_GR_R_UserPoint)) return false;
		MSG_GR_R_UserPoint * pUserPoint = (MSG_GR_R_UserPoint *)pNetData;

		//��������
		UserItemStruct * pUserItem = m_UserManage.FindOnLineUser(pUserPoint->dwUserID);
		if (pUserItem != NULL)
		{
			//��������

			pUserItem->GameUserInfo.dwPoint += pUserPoint->dwPoint;

			if (pNetHead->bHandleCode == 100)
				pUserItem->GameUserInfo.i64Money = pUserPoint->dwMoney;
			else
				pUserItem->GameUserInfo.i64Money += pUserPoint->dwMoney;

			pUserItem->GameUserInfo.uWinCount += pUserPoint->bWinCount;
			pUserItem->GameUserInfo.uLostCount += pUserPoint->bLostCount;
			pUserItem->GameUserInfo.uMidCount += pUserPoint->bMidCount;
			pUserItem->GameUserInfo.uCutCount += pUserPoint->bCutCount;
		}
		return true;
	}
	case ASS_GR_CONTEST_AWARD:
	{
		MSG_GR_S_ContestAward *pAward = (MSG_GR_S_ContestAward*)pNetData;
		if (NULL == pAward) return false;

		return true;
	}
	default:
		return true;
	}
	return true;
}

string GetImageName(EmImageType emImage)
{
	static std::map<EmImageType, std::string> MapImageName = {
		{ ImageType_CaiShen, "����" },
		{ ImageType_MiLe,"����" },
		{ ImageType_LaoZhe, "����" },
		{ ImageType_JinYu, "����" },
		{ ImageType_FuDai, "����" },
		{ ImageType_RuYi, "����" },
		{ ImageType_XianTao, "����" },
		{ ImageType_BaoZi, "����" },
		{ ImageType_TangHuLu, "�Ǻ�«" },
		{ ImageType_JingLuo, "����" },
		{ ImageType_CaiShenDao, "����" }
	};

	auto itorMap = MapImageName.find(emImage);
	if (itorMap != MapImageName.end())
	{
		return itorMap->second;
	}

	char szTmp[16];
	sprintf_s(szTmp, "%d", emImage);

	return szTmp;
}

bool CGameRoomEx::OnGameFrameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	ASSERT(pNetHead->bMainID == MDM_GM_GAME_FRAME);

	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_GAME_INFO:			//��Ϸ��Ϣ
	{
		UINT uDstSize = sizeof(MSG_GM_S_GameInfo) - sizeof(MSG_GM_S_GameInfo::szMessage);
		if (uDstSize != uDataSize) {
			return false;
		}

		MSG_GM_S_GameInfo* pGameInfo = (MSG_GM_S_GameInfo*)pNetData;

		char szLog[512];
		string strLog;
		//sprintf_s(szLog, "��Ϸ״̬��%d", pGameInfo->bGameStation);
		strLog += szLog;
		//ADD_LOG(szLog);

		break;
	}
	case ASS_GM_GAME_STATION:		//������Ϣ
	{
		if (uDataSize != sizeof(CMD_S_GameStation)) {
			ADD_LOG("1089 ���ݴ�С�쳣");
			return false;
		}

		return true;
		CMD_S_GameStation* pResult = (CMD_S_GameStation*)pNetData;
		if (pResult->byGameStation == GS_WAIT_ARGEE)
		{
			char szLog[512];
			sprintf_s(szLog, "ӮǮ��%I64d, �����Ϸ�ۼ�ӮǮ��%I64d", pResult->iWinMoney, pResult->iFreeWinMoney);
			ADD_LOG("���������Ϸ��");
			ADD_LOG(szLog);

			sprintf_s(szLog, "ʣ����Ѵ�����%d, iBetMoney=%d", pResult->iLeftFreeGameCnt, pResult->iBetMoney);
			ADD_LOG(szLog);

			ADD_LOG("ͼ��");
			for (int i = 0; i < 3; ++i)
			{
				string strImage;
				for (int j = 0; j < 5; ++j)
				{
					strImage += GetImageName((EmImageType)pResult->iImageResult[i][j]);
					if (j != 4) {
						strImage += "-";
					}
				}
				ADD_LOG(strImage.c_str());
			}
		}
		return true;
	}
	}
	return false;
}


bool CGameRoomEx::OnGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	ASSERT(pNetHead->bMainID == MDM_GM_GAME_NOTIFY);

	switch (pNetHead->bAssistantID)
	{
	case 60:		// �������
	{
		if (uDataSize != sizeof(CMD_S_TypeScroll)) {
			ADD_LOG("���ݳ�������");
			return false;;
		}
		CMD_S_TypeScroll* pResult = (CMD_S_TypeScroll*)pNetData;
		m_nRunCnt++;

		char szLog[512];
		sprintf_s(szLog, "ӮǮ��%I64d, �����Ϸ�ۼ�ӮǮ��%I64d ������%d", 
			pResult->iWinMoney, pResult->iFreeWinMoney, m_nRunCnt);
		//ADD_LOG("���������");
		ADD_LOG(szLog);

		sprintf_s(szLog, "ʣ�������Ϸ������%d, ��ǰ���Ƿ���ѣ�%d", pResult->iLeftFreeGameCnt, pResult->bFreeGame);
		ADD_LOG(szLog);

		//ADD_LOG("ͼ��");
		//for (int i = 0; i < 3; ++i)
		//{
		//	string strImage = "";
		//	for (int j = 0; j < 5; ++j)
		//	{
		//		string tmpImage = GetImageName((EmImageType)pResult->iImageResult[i][j]);
		//		strImage += tmpImage;
		//		if (tmpImage.length() == 4) {
		//			strImage += "--";
		//		}
		//		else if (tmpImage.length() == 6) {
		//			strImage += "-";
		//		}
		//	}

		//	ADD_LOG(strImage.c_str());
		//}
		//ADD_LOG("BOOL");
		//for (int i = 0; i < 3; ++i)
		//{
		//	string strBool = "";
		//	for (int j = 0; j < 5; ++j)
		//	{
		//		if (pResult->bRewardImage[i][j]) {
		//			strBool += "1";
		//		}
		//		else {
		//			strBool += "0";
		//		}
		//		if (j != 4) {
		//			strBool += "-";
		//		}
		//	}
		//	ADD_LOG(strBool.c_str());
		//}

		//ADD_LOG("�����������꣺");
		//for (int i = 0; i < pResult->lineTotalCnt; ++i)
		//{
		//	string strLine;
		//	char szTmp[128];
		//	for (int j = 0; j < 5; ++j)
		//	{
		//		if (pResult->allLines[i].points[j] >= 0 && 
		//			pResult->allLines[i].points[j] <= 3)
		//		{
		//			sprintf_s(szTmp, "(%d, %d) ", j, int(pResult->allLines[i].points[j]));
		//			strLine += szTmp;
		//		}
		//	}
		//	ADD_LOG(strLine.c_str());
		//	ADD_LOG("ӮǮ��%d", pResult->allLines[i].iWinMoney);
		//}

		//ADD_LOG("�ƾ�ID��%s", pResult->szGameID);
		ADD_LOG("  ");
		return true;
	}
	}
	return false;
}

bool CGameRoomEx::OnSystemMsgMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_SYSTEM_MESSAGE_EX:		// ϵͳ����
	{
		if (sizeof(MSG_GA_S_Message) != uDataSize) {
			ADD_LOG("���ݴ�С�쳣��1205");
			return false;
		}
		MSG_GA_S_Message* pMsg = (MSG_GA_S_Message*)pNetData;
		ADD_LOG("�յ�֪ͨ�� %s", pMsg->szMessage);
		break;
	}
	}
	return false;
}

//�û���������
bool CGameRoomEx::OnUserActionMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	switch (pNetHead->bAssistantID)
	{
		//��ʱ����
	case ASS_GR_EXPERCISE_OUTTIME:
	{
		/*StandingDesk()*/;
		ADD_LOG("��ʱ ���");
		return true;
	}
	case ASS_GR_USER_HIT_BEGIN:
	{
		//Ч������
		if (uDataSize != sizeof(MSG_GM_WatchSet))
			return false;
		MSG_GM_WatchSet* pUserID = (MSG_GM_WatchSet*)pNetData;

		ADD_LOG("�û�ͬ�⿪ʼ");
		//��������
		UserItemStruct * pUserItem = m_UserManage.FindOnLineUser(pUserID->dwUserID);
		pUserItem->GameUserInfo.bUserState = USER_DESK_AGREE;

		return true;
	}
	case ASS_GR_USER_UP:		//�û�����
	{
		//Ч������
		if (uDataSize != sizeof(MSG_GR_R_UserUp)) return false;
		MSG_GR_R_UserUp * pUserUp = (MSG_GR_R_UserUp *)pNetData;

		//ADD_LOG("�û�����");

		//��������
		UserItemStruct * pUserItem = m_UserManage.FindOnLineUser(pUserUp->dwUserID);
		if (pUserItem == NULL)
		{
			return false;
		}

		if (pUserItem->GameUserInfo.dwUserID < 0)
		{
			return false;
		}

		if (pUserItem != NULL)
		{
			//����
			INT bTemp = pUserItem->GameUserInfo.bDeskNO;
			//��������
			pUserItem->GameUserInfo.bDeskNO = -1;
			pUserItem->GameUserInfo.bDeskStation = 255;
			pUserItem->GameUserInfo.bUserState = pUserUp->bUserState;

			if (pUserUp->dwUserID == m_LogonInfo.dwUserID)//�Լ�����
			{
				if (m_IGameFrame != NULL)
				{
					m_IGameFrame->AFCCloseFrame();

					FreeIGameFrame(m_IGameFrame);
				}
				//�����ɹ����뿪
				//CloseGameRoom();
				return true;
			}

			//����
			if (bTemp == m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO)
			{
			}

		}

		return true;
	}
	case ASS_GR_USER_SIT:		//�û�����
	case ASS_GR_WATCH_SIT:		//�Թ�����
	{
		if (uDataSize != sizeof(MSG_GR_R_UserSit))
		{
			return false;
		}
		MSG_GR_R_UserSit * pUserSit = (MSG_GR_R_UserSit *)pNetData;

		//ADD_LOG("�û�����");

		//��������
		UserItemStruct * pUserItem = m_UserManage.FindOnLineUser(pUserSit->dwUserID);
		if (pUserItem != NULL)
		{
			/////////////////////////////////////////
			//��������
			pUserItem->GameUserInfo.bDeskNO = pUserSit->bDeskIndex;
			pUserItem->GameUserInfo.bDeskStation = pUserSit->bDeskStation;

			pUserItem->GameUserInfo.bUserState = pUserSit->bUserState;

			if (pUserItem->GameUserInfo.dwUserID == m_LogonInfo.dwUserID)//��������
			{
				// �Լ�����֮�󣬻�ȡ��Ϸ����
				//ADD_LOG("��ȡ��Ϸ��Ϣ");
				MSG_GM_S_ClientInfo tmpMsg;
				tmpMsg.bEnableWatch = false;
				m_TCPSocket->SendData(&tmpMsg, sizeof(tmpMsg), MDM_GM_GAME_FRAME, ASS_GM_GAME_INFO, 0);

				//3���¼��𲻴�������
				//StartGameClient();
				//���ƿͻ���
				//CM_UserState_Change ControlMessage;
				//ControlMessage.bActionCode = ACT_USER_SIT;
				//ControlMessage.bDeskStation = m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation;
				//ControlMessage.pUserItem = m_GameInfo.pMeUserInfo;
				//ControlGameClient(CM_USER_STATE, &ControlMessage, sizeof(ControlMessage));
			}
		}
		return true;
	}
	case ASS_GR_QUEUE_USER_SIT:
	{
		int* pData = (int*)pNetData;
		if (pNetHead->uMessageSize - sizeof(NetMessageHead) != sizeof(int) * (m_GameInfo.uDeskPeople + 1)) return false;
		int _DeskIndex = *pData;
		pData++;

		bool bIsSelf = false;
		for (int i = 1; i < m_GameInfo.uDeskPeople + 1; i++)
		{
			UserItemStruct * pUserItem = m_UserManage.FindOnLineUser((long)*pData);
			if (pUserItem != NULL)
			{
				// PengJiLin, 2010-5-19���Ŷӻ�������������ʾ�����Ϣ
				if (pUserItem->GameUserInfo.dwUserID != m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID && m_GameInfo.dwRoomRule & GRR_NOTCHEAT)
					strcpy(pUserItem->GameUserInfo.nickName, "���");

				pUserItem->GameUserInfo.bDeskNO = _DeskIndex;
				pUserItem->GameUserInfo.bDeskStation = i - 1;
				pUserItem->GameUserInfo.bUserState = USER_SITTING;

				if ((pUserItem->GameUserInfo.dwUserID == m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID))
				{
					bIsSelf = true;
					m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO = _DeskIndex;
					m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation = i - 1;
				}
			}
			pData++;
		}

		if (bIsSelf)
		{
			// �Լ�����֮�󣬻�ȡ��Ϸ����
			MSG_GM_S_ClientInfo tmpMsg;
			tmpMsg.bEnableWatch = false;
			m_TCPSocket->SendData(&tmpMsg, sizeof(tmpMsg), MDM_GM_GAME_FRAME, ASS_GM_GAME_INFO, 0);

			return true;
			pData = (int*)pNetData;
			pData++;

			//�ر���Ϸ��
			if (m_IGameFrame != NULL)
			{
				m_IGameFrame->AFCCloseFrame();
				FreeIGameFrame(m_IGameFrame);
			}

			StartGameClient();

			CM_UserState_Change ControlMessage;
			ControlMessage.bActionCode = ACT_USER_SIT;//USER_ARGEE;
			ControlMessage.bDeskStation = m_GameInfo.pMeUserInfo->GameUserInfo.bDeskStation;
			ControlMessage.pUserItem = m_GameInfo.pMeUserInfo;
			ControlGameClient(CM_USER_STATE, &ControlMessage, sizeof(ControlMessage));
		}

		return true;
	}
	case ASS_GR_USER_COME:		//�û�����
	{
		//Ч������
		if (uDataSize != sizeof(MSG_GR_R_UserCome)) return false;
		MSG_GR_R_UserCome * pUserCome = (MSG_GR_R_UserCome *)pNetData;

		UserItemStruct * pUserItem = m_UserManage.AddGameUser(&pUserCome->pUserInfoStruct, 1, 0);
		return true;
	}
	case ASS_GR_USER_LEFT:		//�û��뿪
	{
		//Ч������
		if (uDataSize != sizeof(MSG_GR_R_UserLeft)) return false;
		MSG_GR_R_UserLeft * pUserLeft = (MSG_GR_R_UserLeft *)pNetData;
		UserItemStruct * pUserItem = m_UserManage.FindOnLineUser(pUserLeft->dwUserID);
		if (pUserItem != NULL)
		{
			m_UserManage.DeleteGameUser(pUserItem);
		}

		return true;
	}
	case ASS_GR_USER_CUT:		//�û�����
	{
		//Ч������
		if (uDataSize != sizeof(MSG_GR_R_UserCut)) return false;
		MSG_GR_R_UserCut * pUserCut = (MSG_GR_R_UserCut *)pNetData;
		//��������
		UserItemStruct * pUserItem = m_UserManage.FindOnLineUser(pUserCut->dwUserID);
		if (pUserItem != NULL)
		{
			//��������
			pUserItem->GameUserInfo.bUserState = USER_CUT_GAME;
		}

		return true;
	}
	case ASS_GR_SET_TABLE_BASEPOINT:	//�����û����ӱ���
	{
		return true;
	}
	case ASS_GR_SIT_ERROR:		//���´���
	{
		ADD_LOG("���´��� %d", pNetHead->bHandleCode);
		if (m_GameInfo.pMeUserInfo->GameUserInfo.bUserState != USER_PLAY_GAME)
			CloseGameRoom();

		return true;
	}
	}
	return true;
}

//������Ϸ�ͻ���
bool CGameRoomEx::StartGameClient()
{
	try
	{
		//�������ļ��ж�ȡҪװ�ص�dll�ļ���
		if (hGameDllInstance == NULL)
		{
			CString strDllName;
			strDllName.Format("%d_AI.dll", m_pRoomInfo->uNameID);
			strDllName += "_AI.dll";
			CString strDllPath = CINIFile::GetAppPath() + strDllName;

			hGameDllInstance = AfxLoadLibrary(strDllPath.GetBuffer(0));
			if (hGameDllInstance != NULL)
			{
				g_pCreateAIGamesInterface = (CreateAIGamesInterface*)GetProcAddress(hGameDllInstance, TEXT("CreateAIGamesInterface"));
				g_pFreeAIGamesInterface = (FreeAIGamesInterface*)GetProcAddress(hGameDllInstance, TEXT("FreeAIGamesInterface"));
			}
		}
		if (m_IGameFrame != NULL)
		{
			FreeIGameFrame(m_IGameFrame);
			m_IGameFrame = NULL;
		}

		//��ȡ����ӿ�
		if (g_pCreateAIGamesInterface != NULL)
		{
			m_IGameFrame = g_pCreateAIGamesInterface();
		}

		m_IGameFrame->InitFrame(&m_GameInfo, this);

		return true;
	}
	catch (...)
	{

	}

	m_IGameFrame->AFCCreateFrame();

	return false;
}

//���Ϳ�����Ϣ 
UINT CGameRoomEx::ControlGameClient(UINT uControlCode, void * pControlData, UINT uDataSize)
{
	try
	{
		if (m_IGameFrame != NULL)
		{
			m_IGameFrame->OnControlMessage(uControlCode, pControlData, uDataSize);
		}
	}
	catch (...)
	{
	}

	return 0;
}

//��ʱ����Ϣ
LRESULT CGameRoomEx::OnSetGameTime(WPARAM wparam, LPARAM lparam)
{
	SetTimer(wparam, lparam, NULL);
	return TRUE;
}

//�ر���Ϸ,����Ϸ�еĻ����˷�ǿ����Ϣ��
LRESULT CGameRoomEx::OnCloseGame(WPARAM wparam, LPARAM lparam)
{
	ADD_LOG("onclosegame: ");

	m_bWillCloseRoom = true;
	if (NULL != m_IGameFrame)
	{
		if (m_IGameFrame->GetStationParameter() >= 20 && m_IGameFrame->GetStationParameter() <= 23)
		{
			SendGameData(MDM_GM_GAME_FRAME, ASS_GM_FORCE_QUIT, 0);
		}

		m_IGameFrame->AFCCloseFrame();
		if (m_IGameFrame != NULL)
		{
			FreeIGameFrame(m_IGameFrame);
		}
	}

	return TRUE;
}

//��ʱ����Ϣ
LRESULT CGameRoomEx::OnKillGameTime(WPARAM wparam, LPARAM lparam)
{
	if ((UINT)wparam == 0)
	{
		for (int i = 100; i < 110; i++)
		{
			KillTimer(i);
		}
		return 0;
	}

	KillTimer((UINT)wparam);
	return TRUE;
}

//ȡǮ
LRESULT CGameRoomEx::OnGetScore(WPARAM wparam, LPARAM lparam)
{
	if (m_GameInfo.pMeUserInfo->GameUserInfo.i64Money < wparam && rand() % 10>8)
	{
		MSG_GR_S_UpdateRobotScore pUpdateRobotScore;
		ZeroMemory(&pUpdateRobotScore, sizeof(pUpdateRobotScore));
		pUpdateRobotScore.dwUserID = m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID;
		pUpdateRobotScore.MinScore = wparam;
		pUpdateRobotScore.MaxScore = wparam + 2000000;
		SendGameData(&pUpdateRobotScore, sizeof(pUpdateRobotScore), MDM_GR_LOGON, ASS_GR_UPDATE_ROBOT_SCORE, 0);
	}
	return TRUE;
}

LRESULT CGameRoomEx::OnStartRoll(WPARAM wparam, LPARAM lparam)
{
	if (m_nRunCnt >= RUN_CNT)
	{
		KillTimer(TIMER_OPT);
		return 0;
	}

	if (m_GameInfo.pMeUserInfo && 
		m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO == -1)
	{
		MatchToDesk();
	}
	else if (m_TCPSocket) {
		ADD_LOG("��ʼ������������");
		if (ROOM_ID == 188 || ROOM_ID == 189 || ROOM_ID == 190)
		{
			CMD_C_StartRoll tmpMsg;
			tmpMsg.iBetMoney = 100;
			m_TCPSocket->SendData(&tmpMsg, sizeof(tmpMsg), MDM_GM_GAME_NOTIFY, ASS_C_START_ROLL, 0);
			if (m_nRunCnt >= RUN_CNT)
			{
				KillTimer(TIMER_OPT);
			}
		}
		else if (ROOM_ID == 159){
			struct stStartRoll	
			{
				BYTE		byDeskStation;
				INT         byDeposit;
				INT			byMublite;
				stStartRoll()
				{
					memset(this, -1, sizeof(CMD_C_StartRoll));
				}
			};

			stStartRoll startMsg;
			startMsg.byDeskStation = 0;
			startMsg.byDeposit = 1;
			startMsg.byMublite = 1;
			//m_TCPSocket->SendData(&startMsg, sizeof(startMsg), MDM_GM_GAME_NOTIFY, 82, 0);
		}
	}

	return 0;
}


LRESULT CGameRoomEx::OnSendCtrl(WPARAM wparam, LPARAM lparam)
{
	m_TCPSocket->SendData(MDM_GR_WEB_INFO, ASS_GP_GET_ROOMMINSCORE, 0);
	Sleep(1000);

	Control666Info ctrlInfo;
	ctrlInfo.nUserid = m_LogonInfo.dwUserID;
	ctrlInfo.CtlCurPoint = 0;					//��ǰ����
	ctrlInfo.CtlGoalPoint = (__int64)wparam;	//Ŀ�����
	ctrlInfo.CtlLevel = (__int64)lparam;		//���Ƶȼ�
	m_TCPSocket->SendData(&ctrlInfo, sizeof(ctrlInfo), MDM_GR_WEB_INFO, ASS_GP_WEB_SET_CONTROL, 0);
	//m_TCPSocket->SendDataNonHead(&ctrlInfo, sizeof(ctrlInfo), MDM_GR_WEB_INFO, ASS_GP_WEB_SET_CONTROL, 0);
	return 0;
}

//��������վ����
LRESULT CGameRoomEx::OnStandingDesk(WPARAM wparam, LPARAM lparam)
{
	StandingDesk();
	return TRUE;
}

//��ʱ��
void CGameRoomEx::OnTimer(UINT_PTR uIDEvent)
{
	if (m_bWillCloseRoom) {
		return;
	}

	//100-110��ʱ��Ϊ��Ϸ�߼�ʹ��
	if (uIDEvent >= 100 && uIDEvent <= 200)
	{
		KillTimer(uIDEvent);
		if (m_IGameFrame != NULL)
		{
			m_IGameFrame->OnGameTimer(0, uIDEvent, 0);
		}
		return;
	}

	//1-100Ϊ����ʹ��
	switch (uIDEvent)
	{
	case TIMER_CHECK_RECONNET:
	{
		KillTimer(TIMER_CHECK_RECONNET);
		if (NULL != m_TCPSocket)
		{
			m_TCPSocket->Connect(m_pRoomInfo->szServiceIP, m_pRoomInfo->uServicePort);
		}
		break;
	}
	case TIMER_CHECK_CONNECT:
	{
		m_nConnTimeOutCnt++;
		if (NULL != m_TCPSocket)
			m_TCPSocket->SendData(MDM_CONNECT, ASS_NET_TEST, 0);
		if (m_nConnTimeOutCnt > 3) {
			//������ʱ
			ADD_LOG("������ʱ");
		}
	}
	break;
	case TIMER_CHECK_STATE://��ǰ״̬����
	{
		SYSTEMTIME CurTime;
		GetLocalTime(&CurTime);
		if (m_GameInfo.pMeUserInfo == NULL) {
			break;
		}

		if (m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO == -1)//δ�����������¼�ʱ��
		{
			//OnSitDesk(true);
			//MatchToDesk();
		}
		else
		{
			//ADD_LOG("��ʼ������......");
			//CMD_C_StartRoll tmpMsg;
			//tmpMsg.iBetMoney = 1000;
			//m_TCPSocket->SendData(&tmpMsg, sizeof(tmpMsg), 180, 80, 0);
		}
		break;
	}
	case TIMER_OPT:
	{
		OnStartRoll(0, 0);
		break;
	}
	}
	return __super::OnTimer(uIDEvent);
}


//��ȡ�������
LONG CGameRoomEx::GetInvertID()
{
	//�����������û��в���
	for (int i = 0; i < m_UserManage.GetOnLineUserCount(); i++)
	{
		UserItemStruct * pFindUserItem = m_UserManage.FindOnLineUserByIndex(i);
		if (pFindUserItem == NULL)
		{
			continue;
		}
		if (pFindUserItem->GameUserInfo.bDeskNO == m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO //ͬ��
			|| pFindUserItem->GameUserInfo.bUserState == USER_PLAY_GAME)					   //������Ϸ�Ѿ���ʼ
		{
			continue;
		}
		//����Ƿ����㹻
		if (pFindUserItem->GameUserInfo.i64Money < m_GameInfo.uLessPoint)
		{
			continue;
		}
		//�����Ƿ����������
		if (pFindUserItem->GameUserInfo.isVirtual/*IsMachineID(pFindUserItem->GameUserInfo.dwUserID)*/)
		{
			continue;
		}
		return pFindUserItem->GameUserInfo.dwUserID;
	}
	return 0;
}

//����������Ϣ����
int CGameRoomEx::SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	if ((NULL != m_TCPSocket) && (!m_IsCloseSocket))
	{
		return m_TCPSocket->SendData(bMainID, bAssistantID, bHandleCode);//����������
	}
	return 0;
}

//����������Ϣ����
int CGameRoomEx::SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	if ((NULL != m_TCPSocket) && (!m_IsCloseSocket))
	{
		return m_TCPSocket->SendData(pData, uBufLen, bMainID, bAssistantID, bHandleCode);
	}
	return 0;
}

void CGameRoomEx::FreeIGameFrame(IFrameInterface* & pGameFrame)
{
	if (g_pFreeAIGamesInterface != NULL)
	{
		g_pFreeAIGamesInterface(pGameFrame);
		pGameFrame = NULL;
	}
	else
	{
		try
		{
			if (pGameFrame != NULL)
			{
				delete pGameFrame;
				pGameFrame = NULL;
			}
		}
		catch (...)
		{

		}
	}
}