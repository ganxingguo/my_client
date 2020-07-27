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

#define TIMER_CHECK_STATE				50				//״̬��ʱ��
#define TIMEOUT_CHECK_STATE				10 * 1000			
#define TIMER_CHECK_CONNECT				51				//�������
#define TIMEOUT_CHECK_CONNECT			3 * 1000
#define TIMER_CHECK_RECONNET			52				//����
#define TIMEOUT_CHECK_RECONNECT			5 * 1000
#define TIMER_OPT						53
#define TIMEOUT_DO_OPT					500//1 * 1000

//��Ϸ������
class CGameRoomEx : public CDialog, public IClientSocketService
{
public:
	static HINSTANCE				hGameDllInstance;				    //��Ϸ�߼�������

public:
	CTCPClientSocket*               m_TCPSocket;
	CGameUserManage                 m_UserManage;

	GameInfoStruct					m_GameInfo;							//��Ϸ��Ϣ
	IFrameInterface*			    m_IGameFrame;						//��Ϸ���
	RoomInfo*						m_pRoomInfo;						//������Ϣ
	bool                            m_IsCloseSocket;                    //socket�رձ�ʾ

	bool                            m_bAutoDesk;						//�Ƿ��Զ�����
	bool                            m_bRandomSitDesk;					//�Ƿ��������
	INT								m_bNowDesk;                         //��ǰҪ��������

	bool							m_IsUseBank;
	bool							m_bWillCloseRoom;					//�����Ƿ񼴽��ر�
	int								m_nConnTimeOutCnt;					//������ʱ����

	MSG_GP_R_LogonResult			m_LogonInfo;						//��½��Ϣ
	int								m_nRunCnt;							//��Ϸ����

public:
	//���캯��
	CGameRoomEx(TCHAR* szGameName, RoomInfo* pRoomInfo, MSG_GP_R_LogonResult* pLogonInfo, bool m_bAutoDesk, bool bRandomSitDesk);
	virtual ~CGameRoomEx();

	DECLARE_MESSAGE_MAP()

	//���غ���
public:
	//��ʼ������
	virtual BOOL OnInitDialog();

	//�����¼�
public:
	//����������Ϣ
	virtual bool OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket);
	//�����ȡ��Ϣ
	virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//����ر���Ϣ
	virtual bool OnSocketCloseEvent();

	//������Ϣ����
protected:
	//������Ϣ����
	bool OnConnectMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�����½����
	bool OnLogonMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�û��б���
	bool OnUserListMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�û���������
	bool OnUserActionMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//������Ϣ����
	bool OnRoomMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�����Ϣ
	bool OnGameFrameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//��Ϸ��Ϣ
	bool OnGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//������Ϣ
	bool OnSystemMsgMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);

protected:
	// ��½��Ϸ��
	bool LogonGameServerByName();
	bool LogonGameServerByID();

	//������Ϸ����
	void ConnectToGameServer();
	//�رշ���
	void CloseGameRoom();
	//����
	bool SitToDesk(bool bSitNow = true);
	//ƥ������
	bool MatchToDesk();

	//�������λ
	bool FindRandDesk(int &bDeskIndex, BYTE &bDeskStation);
	//��������
	bool FindNullDesk(int &bDeskIndex, BYTE &bDeskStation);
	//����������������
	bool FindTureDesk(int &bDeskIndex, BYTE &bDeskStation);
	//��������
	BYTE GetNullStation(int bDeskIndex);
	//������Ϸ�߼�
	bool StartGameClient();
	//���Ϳ�����Ϣ 
	inline UINT ControlGameClient(UINT uControlCode, void * pControlData, UINT uDataSize);

	//��ʱ��
	void OnTimer(UINT_PTR uIDEvent);
	//��Ҫ������
	bool NeedChange(BYTE bDeskIndex);
	//������
	bool FindOtherDesk();
	//����
	bool StandingDesk();
	//����״̬����ʱ��
	bool StartCheckStationTime();

	//��ȡ�������
	LONG GetInvertID();

	// commond ��Ϣ
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