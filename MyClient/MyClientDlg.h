#pragma once

#include "stdafx.h"
#include "resource.h"

#include "GameListCtrl.h"
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include "afxwin.h"
using namespace std;

#pragma once

#define	TIME_CHECK_SOCKET_ERROR			1			// 错误检查
#define TIMEROUT_CHECK_SOCKET_ERROR		5 * 1000
#define TIME_DELAY_LOGON				2			// 延时登陆
#define TIMEROUT_DELAY_LOGON			1 * 1000	

#define	TIMER_CHECK_GATECONNECT			7			// Gate服务器连接超时检测
#define TIMEROUT_CHECK_GATECONNECT		5 * 1000	

// commond 消息
#define	IDM_CLOSE_GAME_ROOM			WM_USER+200		//关闭房间
#define	IDM_LOGON_FAIL				WM_USER+201		//登陆失败
#define	IDM_LOGON_SUCESS			WM_USER+203     //成功登陆房间


class CMyClientDlg : public CDialog, public IClientSocketService
{
// 静态
public:
	static CMyClientDlg* CreateInstance(CWnd* pParent = NULL);
	static void AddLog(const char* szLog, ...);

	static CMyClientDlg* s_pMainDlg;
	static CListBox*	s_pListBoxLog;

protected:
	CMyClientDlg(CWnd * pParent = NULL);
	~CMyClientDlg();

	enum { IDD = IDD_DIALOG };

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL OnInitDialog();

	// 生成的消息映射函数
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonClearLog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnStartRoll();
	afx_msg void OnBnClickedBtnSendCtrl();

	//网络接口事件
public:
	//网络读取消息
	virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//网络连接消息
	virtual bool OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket);
	//网络关闭消息
	virtual bool OnSocketCloseEvent();

	//连接消息处理
	bool OnConnectMessage(NetMessageHead* pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//登陆 消息处理
	bool OnLogonMessage(NetMessageHead* pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//列表信息
	bool OnListMessage(NetMessageHead* pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);

protected:
	//发送网络消息函数
	int SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
	//发送网络消息函数
	int SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);

protected:
	//连接Gate服务器
	void ConnectToGateServer();
	//开始登录大厅
	void StartLogonHall();
	//创建游戏房间
	bool CreateGameRoom(RoomInfo* pGameRoomItem);
	//进入房间
	bool EnterRoom(int roomid);

	//关闭房间请求
	LRESULT OnCloseGameRoom(WPARAM wparam, LPARAM lparam);
	//登陆房间成功消息
	LRESULT OnLogonSucess(WPARAM wparam, LPARAM lparam);
	//登陆大厅
	BOOL UserLogonHall();
	//登陆失败
	LRESULT OnLogonFail(WPARAM wparam, LPARAM lparam);

	CString GetMachineCode();
	CString GetMacAddress();

protected:
	CListBox								m_ListLog;		
	HICON									m_hIcon;
	int										m_iEnterKindID;								//游戏类型ID							
	int										m_iEnterNameID;								//游戏名称ID
	int										m_iEnterRoomID;								//登陆房间ID
	CGameListCtrl							m_GameList;									//游戏列表
	MSG_GP_R_LogonResult					m_PlaceUserInfo;							//用户资料
	map<UINT, CGameRoomEx*>					m_mapRoomItem;

	int										m_iAccounts;								//当前帐号
	bool                                    m_bAutoDesk;								//自动换桌
	bool                                    m_bRandomSitDesk;							//随机坐桌
	int                                     m_iMachineCheckTime;						//检测间隔

public:
	//刷新登陆房间人数
	CTCPClientSocket*			m_pTCPSocket;		// 网络连接对象
	bool m_bLogoning;			// 是否已经登录
	int m_CtrlGoalMoney;
	int m_CtrlLevel;
};



