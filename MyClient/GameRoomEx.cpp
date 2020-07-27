#include "StdAfx.h"
#include "GameRoomEx.h"
#include "gameroommessage.h"
#include "UpgradeMessage.h"
#include "MyClientDlg.h"


#define GS_WAIT_SETGAME					0				//等待东家设置状态
#define GS_WAIT_ARGEE					1				//等待同意设置
#define GS_PLAY_GAME					20				//发牌状态
#define GS_WAIT_BACK					21				//等待扣压底牌
#define GS_WAIT_NEXT					23				//等待下一盘开始 

HINSTANCE CGameRoomEx::hGameDllInstance = NULL;

//DLL 接口函数定义
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


//构造函数
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

//析构函数
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

//初始化函数
BOOL CGameRoomEx::OnInitDialog()
{
	CDialog::OnInitDialog();
	ConnectToGameServer();
	return TRUE;
}

//登陆游戏房间
void  CGameRoomEx::ConnectToGameServer()
{
	if (NULL != m_TCPSocket)
	{
		if (!m_TCPSocket->Connect(GATE_IP, GATE_PORT2))
		{
			ADD_LOG("游戏房间：Gate连接失败");
		}
	}

	return;
}

//网络连接消息
bool CGameRoomEx::OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket)
{
	if (uErrorCode != 0)	//连接出现错误
	{
		ADD_LOG("连接出错了");
		SetTimer(TIMER_CHECK_RECONNET, TIMEOUT_CHECK_RECONNECT, NULL);
	}
	else {
		int i = ROOM_ID;
		m_TCPSocket->SendData(&i, sizeof(int), MDM_CONNECT, ASS_NET_GET, NOENCRYPTION);
	}
	return true;
}

//网络读取消息
bool CGameRoomEx::OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{

	switch (pNetHead->bMainID)
	{
	case MDM_CONNECT:			//连接消息,准备进入房间 
	case MDM_GP_CONNECT:
	{
		return OnConnectMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	case MDM_GR_LOGON:			//房间登陆
	{
		return OnLogonMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	case MDM_GR_USER_LIST:		//用户列表
	{
		return OnUserListMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	case MDM_GR_USER_ACTION:	//用户动作
	{
		return OnUserActionMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	case MDM_GR_ROOM:			//房间信息
	{
		return OnRoomMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	case MDM_GR_MESSAGE:		//系统消息
	{
		return OnSystemMsgMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	case MDM_GM_GAME_FRAME:		//游戏框架
	{
		return OnGameFrameMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	case MDM_GM_GAME_NOTIFY:	//游戏消息
	{
		return OnGameMessage(pNetHead, pNetData, uDataSize, pClientSocket);
	}
	}
	//游戏信息
	//if (m_IGameFrame != NULL)
	//{
	//	return m_IGameFrame->OnSocketReadEvent(pNetHead, pNetData, uDataSize, pClientSocket);
	//}


	return true;
}

//网络关闭消息
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

//关闭房间
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

//连接消息处理
bool CGameRoomEx::OnConnectMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bAssistantID == ASS_NET_TEST)	//网络测试信息
	{
		m_nConnTimeOutCnt = 0;
		return true;
	}
	else if ((pNetHead->bAssistantID == ASS_CONNECT_SUCCESS))	//连接成功
	{
		MSG_S_ConnectSuccess * pSuccess = (MSG_S_ConnectSuccess *)pNetData;
		if (pSuccess != NULL)
		{
			pClientSocket->SetCheckCode(pSuccess->i64CheckCode, SECRET_KEY);
		}

		ADD_LOG("游戏Room：连接成功");
		LogonGameServerByID();
		return true;
	}
	return true;
}

//001,房间登陆处理,一进入房间  
bool CGameRoomEx::OnLogonMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_LOGON_SUCCESS:		//登陆成功
	{
		//效验数据
		if (uDataSize != sizeof(MSG_GR_R_LogonResult)) {
			return false;
		}

		ADD_LOG("登陆房间成功");

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
	case ASS_GR_LOGON_ERROR:		//登陆失败
	{
		ADD_LOG("登陆失败 : %d", pNetHead->bHandleCode);

		::PostMessage(GetParent()->m_hWnd, IDM_LOGON_FAIL, 0, m_LogonInfo.dwUserID);
		return false;
	}
	case ASS_GR_SEND_FINISH:		//登陆完成
	{
		//设置总状态跟踪
		StartCheckStationTime();
		ADD_LOG("登陆完成");

		MatchToDesk();
		m_nRunCnt = 0;
		//SetTimer(TIMER_OPT, TIMEOUT_DO_OPT, NULL);
		return true;
	}
	case ASS_BEGIN_MATCH_ROOM:		// 匹配成功
	{
		//ADD_LOG("匹配成功");
		return true;
	}
	case ERR_GR_NO_ENOUGH_ROOM_KEY:
	{
		StandingDesk();
	}

	}
	return true;
}


//设置状态检测计时器
bool CGameRoomEx::StartCheckStationTime()
{
	SetTimer(TIMER_CHECK_CONNECT, TIMEOUT_CHECK_CONNECT, NULL);
	SetTimer(TIMER_CHECK_STATE, TIMEOUT_CHECK_STATE, NULL);
	return true;
}

//起立
bool CGameRoomEx::StandingDesk()
{
	if (NULL != m_TCPSocket)
	{
		m_TCPSocket->SendData(MDM_GR_USER_ACTION, ASS_GR_USER_UP, 0);
	}
	return true;
}

//坐下
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

//随机坐一个空桌子
bool CGameRoomEx::FindRandDesk(int &bDeskIndex, BYTE &bDeskStation)
{
	srand(GetTickCount());
	int time = 0;
	UINT bDesk = rand() % m_pRoomInfo->uDeskCount;

	while (time++ < 5)
	{
		//坐位
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

				if (pFindUserItem->GameUserInfo.bDeskNO == bDesk &&pFindUserItem->GameUserInfo.bDeskStation == station)//此坐位已经有人
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

//查找空桌子
bool CGameRoomEx::FindNullDesk(int &bDeskIndex, BYTE &bDeskStation)
{
	//从每一个桌子开始查找
	for (UINT desk = 0; desk < m_pRoomInfo->uDeskCount; desk++)
	{
		//坐位
		for (UINT station = 0; station < m_GameInfo.uDeskPeople; station++)
		{
			bool havepeople = false;
			for (int i = 0; i < m_UserManage.GetOnLineUserCount(); i++)
			{
				UserItemStruct * pFindUserItem = m_UserManage.FindOnLineUserByIndex(i);
				if (pFindUserItem == NULL)
					continue;
				if (pFindUserItem->GameUserInfo.bDeskNO == desk&&pFindUserItem->GameUserInfo.bUserState == USER_PLAY_GAME)//桌上游戏已经开始
				{
					havepeople = true;
					break;
				}
				if (pFindUserItem->GameUserInfo.bDeskNO == desk &&pFindUserItem->GameUserInfo.bDeskStation == station)//此桌有人
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

//查找指定桌子空位置(处理被邀请的时候处理)
BYTE CGameRoomEx::GetNullStation(int bDeskIndex)
{
	BYTE bDeskStation = 255;
	//坐位
	for (UINT station = 0; station < m_GameInfo.uDeskPeople; station++)
	{
		bool havepeople = false;
		for (int i = 0; i < m_UserManage.GetOnLineUserCount(); i++)
		{
			UserItemStruct * pFindUserItem = m_UserManage.FindOnLineUserByIndex(i);
			if (pFindUserItem == NULL)
				continue;
			if (pFindUserItem->GameUserInfo.bDeskNO == bDeskIndex && pFindUserItem->GameUserInfo.bUserState == USER_PLAY_GAME)//桌上游戏已经开始
			{
				havepeople = true;
				break;
			}
			if (pFindUserItem->GameUserInfo.bDeskNO == bDeskIndex &&pFindUserItem->GameUserInfo.bDeskStation == station)//此桌有人
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
//查找真人最多空桌子
bool CGameRoomEx::FindTureDesk(int &bDeskIndex, BYTE &bDeskStation)
{
	bDeskIndex = -1;
	bDeskStation = 255;
	//从每一个桌子开始查找
	for (UINT desk = 0; desk < m_pRoomInfo->uDeskCount; desk++)
	{
		//记录哪些位子上没人
		BYTE iTemp[180] = { 0 };

		//坐位
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

				if (pFindUserItem->GameUserInfo.bDeskNO == desk && pFindUserItem->GameUserInfo.bDeskStation == station)//此桌有人
				{
					havepeople = true;
					break;
				}
			}

			if (bVirtual)  //被封的座位，查找下一桌
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

//是否需要换桌子
bool CGameRoomEx::NeedChange(BYTE bDeskIndex)
{
	int count = 0;
	for (int i = 0; i < m_UserManage.GetOnLineUserCount(); i++)
	{
		UserItemStruct * pFindUserItem = m_UserManage.FindOnLineUserByIndex(i);
		if (pFindUserItem == NULL)
			continue;
		if (pFindUserItem->GameUserInfo.bDeskNO == bDeskIndex)//桌上游戏已经开始
		{
			count++;
		}
	}
	if (count >= 2 || m_GameInfo.pMeUserInfo->GameUserInfo.bUserState == USER_PLAY_GAME)
		return false;

	return true;
}

//查找另外一张桌子
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
	ADD_LOG("发送登陆请求");
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

//用户列表处理
bool CGameRoomEx::OnUserListMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_ONLINE_USER:
	case ASS_GR_NETCUT_USER:	//用户列表
	{
		UINT uUserCount = uDataSize / sizeof(UserInfoStruct);

		if (uUserCount > 0L)
		{
			//定义变量
			UserItemStruct * pUserItem = NULL;
			UserInfoStruct * pUserInfo = (UserInfoStruct *)pNetData;

			for (UINT i = 0; i < uUserCount; i++)
			{
				pUserItem = m_UserManage.AddGameUser(pUserInfo + i, 1, 0);
			}
		}
		return true;
	}
	case ASS_GR_DESK_STATION:	//桌子状态 
	{
		if (uDataSize != sizeof(MSG_GR_DeskStation)) return false;
		MSG_GR_DeskStation * pDeskStation = (MSG_GR_DeskStation *)pNetData;
		return true;
	}
	}
	return true;
}


//房间信息处理
bool CGameRoomEx::OnRoomMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	ASSERT(pNetHead->bMainID == MDM_GR_ROOM);

	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_NORMAL_TALK:	//普通聊天
	{
		return true;
	}
	case ASS_GR_SHORT_MSG:		//短信息聊天
	{
		return true;
	}
	case ASS_GR_USER_AGREE:		//用户同意
	{
		return true;
	}
	case ASS_GR_GAME_BEGIN:		//游戏开始
	{
		//效验数据
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

	case ASS_GR_GAME_END:		//游戏结束
	{

		//效验数据
		if (uDataSize != 0)
		{
			return false;
		}

		bool bHaveplayer = false; //是否有真实玩家
		//处理数据
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

	case ASS_GR_INSTANT_UPDATE:		//即时更新分数金币
	{
		return TRUE;
	}
	case ASS_GR_USER_POINT:		//用户金币积分
	{
		if (pNetHead->bHandleCode == 10)//同步金币
		{
			return true;
		}
		else if (pNetHead->bHandleCode == 11)//同步经验值
		{
			return true;
		}

		//效验数据
		if (uDataSize != sizeof(MSG_GR_R_UserPoint)) return false;
		MSG_GR_R_UserPoint * pUserPoint = (MSG_GR_R_UserPoint *)pNetData;

		//处理数据
		UserItemStruct * pUserItem = m_UserManage.FindOnLineUser(pUserPoint->dwUserID);
		if (pUserItem != NULL)
		{
			//设置数据

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
		{ ImageType_CaiShen, "财神" },
		{ ImageType_MiLe,"弥勒" },
		{ ImageType_LaoZhe, "老者" },
		{ ImageType_JinYu, "金鱼" },
		{ ImageType_FuDai, "福袋" },
		{ ImageType_RuYi, "如意" },
		{ ImageType_XianTao, "仙桃" },
		{ ImageType_BaoZi, "包子" },
		{ ImageType_TangHuLu, "糖葫芦" },
		{ ImageType_JingLuo, "金锣" },
		{ ImageType_CaiShenDao, "财神到" }
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
	case ASS_GM_GAME_INFO:			//游戏信息
	{
		UINT uDstSize = sizeof(MSG_GM_S_GameInfo) - sizeof(MSG_GM_S_GameInfo::szMessage);
		if (uDstSize != uDataSize) {
			return false;
		}

		MSG_GM_S_GameInfo* pGameInfo = (MSG_GM_S_GameInfo*)pNetData;

		char szLog[512];
		string strLog;
		//sprintf_s(szLog, "游戏状态：%d", pGameInfo->bGameStation);
		strLog += szLog;
		//ADD_LOG(szLog);

		break;
	}
	case ASS_GM_GAME_STATION:		//场景消息
	{
		if (uDataSize != sizeof(CMD_S_GameStation)) {
			ADD_LOG("1089 数据大小异常");
			return false;
		}

		return true;
		CMD_S_GameStation* pResult = (CMD_S_GameStation*)pNetData;
		if (pResult->byGameStation == GS_WAIT_ARGEE)
		{
			char szLog[512];
			sprintf_s(szLog, "赢钱：%I64d, 免费游戏累计赢钱：%I64d", pResult->iWinMoney, pResult->iFreeWinMoney);
			ADD_LOG("进入财神到游戏：");
			ADD_LOG(szLog);

			sprintf_s(szLog, "剩余免费次数：%d, iBetMoney=%d", pResult->iLeftFreeGameCnt, pResult->iBetMoney);
			ADD_LOG(szLog);

			ADD_LOG("图案");
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
	case 60:		// 滚动结果
	{
		if (uDataSize != sizeof(CMD_S_TypeScroll)) {
			ADD_LOG("数据长度有误");
			return false;;
		}
		CMD_S_TypeScroll* pResult = (CMD_S_TypeScroll*)pNetData;
		m_nRunCnt++;

		char szLog[512];
		sprintf_s(szLog, "赢钱：%I64d, 免费游戏累计赢钱：%I64d 次数：%d", 
			pResult->iWinMoney, pResult->iFreeWinMoney, m_nRunCnt);
		//ADD_LOG("滚动结果：");
		ADD_LOG(szLog);

		sprintf_s(szLog, "剩余免费游戏次数：%d, 当前局是否免费：%d", pResult->iLeftFreeGameCnt, pResult->bFreeGame);
		ADD_LOG(szLog);

		//ADD_LOG("图案");
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

		//ADD_LOG("所有线条坐标：");
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
		//	ADD_LOG("赢钱：%d", pResult->allLines[i].iWinMoney);
		//}

		//ADD_LOG("牌局ID：%s", pResult->szGameID);
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
	case ASS_GM_SYSTEM_MESSAGE_EX:		// 系统公告
	{
		if (sizeof(MSG_GA_S_Message) != uDataSize) {
			ADD_LOG("数据大小异常：1205");
			return false;
		}
		MSG_GA_S_Message* pMsg = (MSG_GA_S_Message*)pNetData;
		ADD_LOG("收到通知： %s", pMsg->szMessage);
		break;
	}
	}
	return false;
}

//用户动作处理
bool CGameRoomEx::OnUserActionMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	switch (pNetHead->bAssistantID)
	{
		//超时踢人
	case ASS_GR_EXPERCISE_OUTTIME:
	{
		/*StandingDesk()*/;
		ADD_LOG("超时 提出");
		return true;
	}
	case ASS_GR_USER_HIT_BEGIN:
	{
		//效验数据
		if (uDataSize != sizeof(MSG_GM_WatchSet))
			return false;
		MSG_GM_WatchSet* pUserID = (MSG_GM_WatchSet*)pNetData;

		ADD_LOG("用户同意开始");
		//处理数据
		UserItemStruct * pUserItem = m_UserManage.FindOnLineUser(pUserID->dwUserID);
		pUserItem->GameUserInfo.bUserState = USER_DESK_AGREE;

		return true;
	}
	case ASS_GR_USER_UP:		//用户起来
	{
		//效验数据
		if (uDataSize != sizeof(MSG_GR_R_UserUp)) return false;
		MSG_GR_R_UserUp * pUserUp = (MSG_GR_R_UserUp *)pNetData;

		//ADD_LOG("用户起立");

		//处理数据
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
			//桌号
			INT bTemp = pUserItem->GameUserInfo.bDeskNO;
			//设置数据
			pUserItem->GameUserInfo.bDeskNO = -1;
			pUserItem->GameUserInfo.bDeskStation = 255;
			pUserItem->GameUserInfo.bUserState = pUserUp->bUserState;

			if (pUserUp->dwUserID == m_LogonInfo.dwUserID)//自己起来
			{
				if (m_IGameFrame != NULL)
				{
					m_IGameFrame->AFCCloseFrame();

					FreeIGameFrame(m_IGameFrame);
				}
				//起立成功才离开
				//CloseGameRoom();
				return true;
			}

			//本桌
			if (bTemp == m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO)
			{
			}

		}

		return true;
	}
	case ASS_GR_USER_SIT:		//用户坐下
	case ASS_GR_WATCH_SIT:		//旁观坐下
	{
		if (uDataSize != sizeof(MSG_GR_R_UserSit))
		{
			return false;
		}
		MSG_GR_R_UserSit * pUserSit = (MSG_GR_R_UserSit *)pNetData;

		//ADD_LOG("用户坐下");

		//处理数据
		UserItemStruct * pUserItem = m_UserManage.FindOnLineUser(pUserSit->dwUserID);
		if (pUserItem != NULL)
		{
			/////////////////////////////////////////
			//设置数据
			pUserItem->GameUserInfo.bDeskNO = pUserSit->bDeskIndex;
			pUserItem->GameUserInfo.bDeskStation = pUserSit->bDeskStation;

			pUserItem->GameUserInfo.bUserState = pUserSit->bUserState;

			if (pUserItem->GameUserInfo.dwUserID == m_LogonInfo.dwUserID)//本人坐下
			{
				// 自己入座之后，获取游戏场景
				//ADD_LOG("获取游戏信息");
				MSG_GM_S_ClientInfo tmpMsg;
				tmpMsg.bEnableWatch = false;
				m_TCPSocket->SendData(&tmpMsg, sizeof(tmpMsg), MDM_GM_GAME_FRAME, ASS_GM_GAME_INFO, 0);

				//3以下级别不处理坐下
				//StartGameClient();
				//控制客户端
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
				// PengJiLin, 2010-5-19，排队机不在桌子上显示玩家信息
				if (pUserItem->GameUserInfo.dwUserID != m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID && m_GameInfo.dwRoomRule & GRR_NOTCHEAT)
					strcpy(pUserItem->GameUserInfo.nickName, "玩家");

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
			// 自己入座之后，获取游戏场景
			MSG_GM_S_ClientInfo tmpMsg;
			tmpMsg.bEnableWatch = false;
			m_TCPSocket->SendData(&tmpMsg, sizeof(tmpMsg), MDM_GM_GAME_FRAME, ASS_GM_GAME_INFO, 0);

			return true;
			pData = (int*)pNetData;
			pData++;

			//关闭游戏端
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
	case ASS_GR_USER_COME:		//用户进入
	{
		//效验数据
		if (uDataSize != sizeof(MSG_GR_R_UserCome)) return false;
		MSG_GR_R_UserCome * pUserCome = (MSG_GR_R_UserCome *)pNetData;

		UserItemStruct * pUserItem = m_UserManage.AddGameUser(&pUserCome->pUserInfoStruct, 1, 0);
		return true;
	}
	case ASS_GR_USER_LEFT:		//用户离开
	{
		//效验数据
		if (uDataSize != sizeof(MSG_GR_R_UserLeft)) return false;
		MSG_GR_R_UserLeft * pUserLeft = (MSG_GR_R_UserLeft *)pNetData;
		UserItemStruct * pUserItem = m_UserManage.FindOnLineUser(pUserLeft->dwUserID);
		if (pUserItem != NULL)
		{
			m_UserManage.DeleteGameUser(pUserItem);
		}

		return true;
	}
	case ASS_GR_USER_CUT:		//用户断线
	{
		//效验数据
		if (uDataSize != sizeof(MSG_GR_R_UserCut)) return false;
		MSG_GR_R_UserCut * pUserCut = (MSG_GR_R_UserCut *)pNetData;
		//处理数据
		UserItemStruct * pUserItem = m_UserManage.FindOnLineUser(pUserCut->dwUserID);
		if (pUserItem != NULL)
		{
			//设置数据
			pUserItem->GameUserInfo.bUserState = USER_CUT_GAME;
		}

		return true;
	}
	case ASS_GR_SET_TABLE_BASEPOINT:	//设置用户桌子倍数
	{
		return true;
	}
	case ASS_GR_SIT_ERROR:		//坐下错误
	{
		ADD_LOG("坐下错误 %d", pNetHead->bHandleCode);
		if (m_GameInfo.pMeUserInfo->GameUserInfo.bUserState != USER_PLAY_GAME)
			CloseGameRoom();

		return true;
	}
	}
	return true;
}

//启动游戏客户端
bool CGameRoomEx::StartGameClient()
{
	try
	{
		//从配置文件中读取要装载的dll文件名
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

		//获取管理接口
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

//发送控制消息 
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

//计时器消息
LRESULT CGameRoomEx::OnSetGameTime(WPARAM wparam, LPARAM lparam)
{
	SetTimer(wparam, lparam, NULL);
	return TRUE;
}

//关闭游戏,在游戏中的机器人发强退消息，
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

//计时器消息
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

//取钱
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
		ADD_LOG("开始滚动。。。。");
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
	ctrlInfo.CtlCurPoint = 0;					//当前分数
	ctrlInfo.CtlGoalPoint = (__int64)wparam;	//目标分数
	ctrlInfo.CtlLevel = (__int64)lparam;		//控制等级
	m_TCPSocket->SendData(&ctrlInfo, sizeof(ctrlInfo), MDM_GR_WEB_INFO, ASS_GP_WEB_SET_CONTROL, 0);
	//m_TCPSocket->SendDataNonHead(&ctrlInfo, sizeof(ctrlInfo), MDM_GR_WEB_INFO, ASS_GP_WEB_SET_CONTROL, 0);
	return 0;
}

//从桌子上站起来
LRESULT CGameRoomEx::OnStandingDesk(WPARAM wparam, LPARAM lparam)
{
	StandingDesk();
	return TRUE;
}

//计时器
void CGameRoomEx::OnTimer(UINT_PTR uIDEvent)
{
	if (m_bWillCloseRoom) {
		return;
	}

	//100-110计时器为游戏逻辑使用
	if (uIDEvent >= 100 && uIDEvent <= 200)
	{
		KillTimer(uIDEvent);
		if (m_IGameFrame != NULL)
		{
			m_IGameFrame->OnGameTimer(0, uIDEvent, 0);
		}
		return;
	}

	//1-100为房间使用
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
			//心跳超时
			ADD_LOG("心跳超时");
		}
	}
	break;
	case TIMER_CHECK_STATE://当前状态处理
	{
		SYSTEMTIME CurTime;
		GetLocalTime(&CurTime);
		if (m_GameInfo.pMeUserInfo == NULL) {
			break;
		}

		if (m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO == -1)//未坐下启动坐下计时器
		{
			//OnSitDesk(true);
			//MatchToDesk();
		}
		else
		{
			//ADD_LOG("开始滚动：......");
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


//获取邀请对象
LONG CGameRoomEx::GetInvertID()
{
	//在所有在线用户中查找
	for (int i = 0; i < m_UserManage.GetOnLineUserCount(); i++)
	{
		UserItemStruct * pFindUserItem = m_UserManage.FindOnLineUserByIndex(i);
		if (pFindUserItem == NULL)
		{
			continue;
		}
		if (pFindUserItem->GameUserInfo.bDeskNO == m_GameInfo.pMeUserInfo->GameUserInfo.bDeskNO //同桌
			|| pFindUserItem->GameUserInfo.bUserState == USER_PLAY_GAME)					   //桌上游戏已经开始
		{
			continue;
		}
		//玩家是否金币足够
		if (pFindUserItem->GameUserInfo.i64Money < m_GameInfo.uLessPoint)
		{
			continue;
		}
		//处理是否邀请机器人
		if (pFindUserItem->GameUserInfo.isVirtual/*IsMachineID(pFindUserItem->GameUserInfo.dwUserID)*/)
		{
			continue;
		}
		return pFindUserItem->GameUserInfo.dwUserID;
	}
	return 0;
}

//发送网络消息函数
int CGameRoomEx::SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	if ((NULL != m_TCPSocket) && (!m_IsCloseSocket))
	{
		return m_TCPSocket->SendData(bMainID, bAssistantID, bHandleCode);//处理主类型
	}
	return 0;
}

//发送网络消息函数
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