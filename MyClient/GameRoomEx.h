#pragma once
#ifndef AFC_GAMEROOMEX_HEAD_FILE
#define AFC_GAMEROOMEX_HEAD_FILE
#include "resource.h"
#include "Stdafx.h"

#include "clientcomstruct.h"
#include "gameusermanage.h"
#include "gameroommessage.h"
#include "gameframedlg.h"
#include "AICommonDef.h"
#include "GameListCtrl.h"

#include <map>
#include <string.h>

#define IDM_SET_GAME_TIME				WM_USER+100
#define IDM_KILL_GAME_TIME				WM_USER+101
#define IDM_CLOSE                       WM_USER+102
#define IDM_STAND_UP					WM_USER+103  
#define IDM_GET_SCORE					WM_USER+104  
#define IDM_START_ROLL					WM_USER+110	
#define IDM_SEND_CTRL					WM_USER+111

#define TIMER_CHECK_STATE				50				//状态计时器
#define TIMEOUT_CHECK_STATE				10 * 1000			
#define TIMER_CHECK_CONNECT				51				//心跳检测
#define TIMEOUT_CHECK_CONNECT			3 * 1000
#define TIMER_CHECK_RECONNET			52				//重连
#define TIMEOUT_CHECK_RECONNECT			5 * 1000
#define TIMER_OPT						53
#define TIMEOUT_DO_OPT					500//1 * 1000

//游戏房间类
class CGameRoomEx : public CDialog, public IClientSocketService
{
public:
	static HINSTANCE				hGameDllInstance;				    //游戏逻辑组件句柄

public:
	CTCPClientSocket*               m_TCPSocket;
	CGameUserManage                 m_UserManage;

	GameInfoStruct					m_GameInfo;							//游戏信息
	IFrameInterface*			    m_IGameFrame;						//游戏框架
	RoomInfo*						m_pRoomInfo;						//房间信息
	bool                            m_IsCloseSocket;                    //socket关闭标示

	bool                            m_bAutoDesk;						//是否自动换桌
	bool                            m_bRandomSitDesk;					//是否随机坐桌
	INT								m_bNowDesk;                         //当前要坐的桌号

	bool							m_IsUseBank;
	bool							m_bWillCloseRoom;					//房间是否即将关闭
	int								m_nConnTimeOutCnt;					//心跳超时次数

	MSG_GP_R_LogonResult			m_LogonInfo;						//登陆信息
	int								m_nRunCnt;							//游戏计数

public:
	//构造函数
	CGameRoomEx(TCHAR* szGameName, RoomInfo* pRoomInfo, MSG_GP_R_LogonResult* pLogonInfo, bool m_bAutoDesk, bool bRandomSitDesk);
	virtual ~CGameRoomEx();

	DECLARE_MESSAGE_MAP()

	//重载函数
public:
	//初始化函数
	virtual BOOL OnInitDialog();

	//网络事件
public:
	//网络连接消息
	virtual bool OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket);
	//网络读取消息
	virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//网络关闭消息
	virtual bool OnSocketCloseEvent();

	//网络消息处理
protected:
	//连接消息处理
	bool OnConnectMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//房间登陆处理
	bool OnLogonMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//用户列表处理
	bool OnUserListMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//用户动作处理
	bool OnUserActionMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//房间信息处理
	bool OnRoomMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//框架消息
	bool OnGameFrameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//游戏消息
	bool OnGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//公告消息
	bool OnSystemMsgMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);

protected:
	// 登陆游戏服
	bool LogonGameServerByName();
	bool LogonGameServerByID();

	//连接游戏房间
	void ConnectToGameServer();
	//关闭房间
	void CloseGameRoom();
	//坐下
	bool SitToDesk(bool bSitNow = true);
	//匹配桌子
	bool MatchToDesk();

	//随机找座位
	bool FindRandDesk(int &bDeskIndex, BYTE &bDeskStation);
	//查找坐号
	bool FindNullDesk(int &bDeskIndex, BYTE &bDeskStation);
	//查找真人最多空桌子
	bool FindTureDesk(int &bDeskIndex, BYTE &bDeskStation);
	//查找坐号
	BYTE GetNullStation(int bDeskIndex);
	//启动游戏逻辑
	bool StartGameClient();
	//发送控制消息 
	inline UINT ControlGameClient(UINT uControlCode, void * pControlData, UINT uDataSize);

	//计时器
	void OnTimer(UINT_PTR uIDEvent);
	//需要换桌子
	bool NeedChange(BYTE bDeskIndex);
	//换桌子
	bool FindOtherDesk();
	//起立
	bool StandingDesk();
	//设置状态检测计时器
	bool StartCheckStationTime();

	//获取邀请对象
	LONG GetInvertID();

	// commond 消息
protected:
	LRESULT OnSetGameTime(WPARAM wparam, LPARAM lparam);
	LRESULT OnKillGameTime(WPARAM wparam, LPARAM lparam);
	LRESULT OnCloseGame(WPARAM wparam, LPARAM lparam);
	LRESULT OnStandingDesk(WPARAM wparam, LPARAM lparam);
	LRESULT OnGetScore(WPARAM wparam, LPARAM lparam);

	LRESULT OnStartRoll(WPARAM wparam, LPARAM lparam);
	LRESULT OnSendCtrl(WPARAM wparam, LPARAM lparam);

public:
	void FreeIGameFrame(IFrameInterface* & pGameFrame);

protected:
	int SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
	int SendGameData(void* pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
};
#endif