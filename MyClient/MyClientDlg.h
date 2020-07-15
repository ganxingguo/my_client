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

#define	TIME_CHECK_SOCKET_ERROR			1			// ������
#define TIMEROUT_CHECK_SOCKET_ERROR		5 * 1000
#define TIME_DELAY_LOGON				2			// ��ʱ��½
#define TIMEROUT_DELAY_LOGON			1 * 1000	

#define	TIMER_CHECK_GATECONNECT			7			// Gate���������ӳ�ʱ���
#define TIMEROUT_CHECK_GATECONNECT		5 * 1000	

// commond ��Ϣ
#define	IDM_CLOSE_GAME_ROOM			WM_USER+200		//�رշ���
#define	IDM_LOGON_FAIL				WM_USER+201		//��½ʧ��
#define	IDM_LOGON_SUCESS			WM_USER+203     //�ɹ���½����


class CMyClientDlg : public CDialog, public IClientSocketService
{
// ��̬
public:
	static CMyClientDlg* CreateInstance(CWnd* pParent = NULL);
	static void AddLog(const char* szLog, ...);

	static CMyClientDlg* s_pMainDlg;
	static CListBox*	s_pListBoxLog;

protected:
	CMyClientDlg(CWnd * pParent = NULL);
	~CMyClientDlg();

	enum { IDD = IDD_DIALOG };

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	// ���ɵ���Ϣӳ�亯��
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonClearLog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnStartRoll();
	afx_msg void OnBnClickedBtnSendCtrl();

	//����ӿ��¼�
public:
	//�����ȡ��Ϣ
	virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//����������Ϣ
	virtual bool OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket);
	//����ر���Ϣ
	virtual bool OnSocketCloseEvent();

	//������Ϣ����
	bool OnConnectMessage(NetMessageHead* pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//��½ ��Ϣ����
	bool OnLogonMessage(NetMessageHead* pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//�б���Ϣ
	bool OnListMessage(NetMessageHead* pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);

protected:
	//����������Ϣ����
	int SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
	//����������Ϣ����
	int SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);

protected:
	//����Gate������
	void ConnectToGateServer();
	//��ʼ��¼����
	void StartLogonHall();
	//������Ϸ����
	bool CreateGameRoom(RoomInfo* pGameRoomItem);
	//���뷿��
	bool EnterRoom(int roomid);

	//�رշ�������
	LRESULT OnCloseGameRoom(WPARAM wparam, LPARAM lparam);
	//��½����ɹ���Ϣ
	LRESULT OnLogonSucess(WPARAM wparam, LPARAM lparam);
	//��½����
	BOOL UserLogonHall();
	//��½ʧ��
	LRESULT OnLogonFail(WPARAM wparam, LPARAM lparam);

	CString GetMachineCode();
	CString GetMacAddress();

protected:
	CListBox								m_ListLog;		
	HICON									m_hIcon;
	int										m_iEnterKindID;								//��Ϸ����ID							
	int										m_iEnterNameID;								//��Ϸ����ID
	int										m_iEnterRoomID;								//��½����ID
	CGameListCtrl							m_GameList;									//��Ϸ�б�
	MSG_GP_R_LogonResult					m_PlaceUserInfo;							//�û�����
	map<UINT, CGameRoomEx*>					m_mapRoomItem;

	int										m_iAccounts;								//��ǰ�ʺ�
	bool                                    m_bAutoDesk;								//�Զ�����
	bool                                    m_bRandomSitDesk;							//�������
	int                                     m_iMachineCheckTime;						//�����

public:
	//ˢ�µ�½��������
	CTCPClientSocket*			m_pTCPSocket;		// �������Ӷ���
	bool m_bLogoning;			// �Ƿ��Ѿ���¼
	int m_CtrlGoalMoney;
	int m_CtrlLevel;
};



