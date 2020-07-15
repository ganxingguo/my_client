#pragma once

#include "AICommonDef.h"

#define IDD_GAME_FRAME                  4001
#define IDM_SET_GAME_TIME				WM_USER+100
#define IDM_KILL_GAME_TIME				WM_USER+101

//��Ϸ������� 
class AFX_EXT_CLASS CLoveSendClass: public CDialog, public IFrameInterface
{
	//��������
protected:
	CWnd								* m_pGameRoom;					//���䴰��

	GameInfoStruct						* m_pGameInfo;					//��Ϸ��Ϣָ��
	UserItemStruct						* m_pUserInfo[MAX_PEOPLE];		//��Ϸ�����Ϣ
	UINT								m_uTimeID;						//��ʱ�� ID
	UINT								m_uTimeCount;					//��ʱ������
	BYTE								m_bTimeStation;					//������λ��
	TCHAR								m_ErrorMsg[MAX_PATH];			//������־
	int                                 m_nPowerOfGold;                ///��ұ���
	//��������

public:
	//���캯��
	CLoveSendClass(UINT uTemplate);
	//��������
	virtual ~CLoveSendClass();

	enum { IDD = IDD_GAME_FRAME };		     	//ID ����
	//���غ���
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//�ż��󶨺���
	virtual void DoDataExchange(CDataExchange * pDX);

protected:
	//��Ϸ��Ϣ������
	virtual bool HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//������Ϸ״̬
	virtual bool SetGameStation(void * pStationData, UINT uDataSize) = 0;
	//�������ý���
	virtual void ResetGameFrame();
	//����������Ϸ����
	virtual void ResetGameStation(int iGameStation = 1){};

	//��Ϸ���ƺ���
protected:
	//ͬ�⿪ʼ��Ϸ 
	virtual bool OnControlHitBegin() = 0;

	//���ܺ���
public:
	void SetInifile(LPCTSTR path);
	//��ȡ�Լ���Ϣ
	UserInfoStruct * GetMeUserInfo() { return &m_pGameInfo->pMeUserInfo->GameUserInfo; };
	//����״̬����
	void SetStationParameter(BYTE bGameStation);
	//��ȡ״̬����
	virtual BYTE GetStationParameter();
	//����������Ϣ����
	int SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
	//����������Ϣ����
	int SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);

	//��ܺ���
	virtual bool OnFrameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);

	//�ӿں���������Ҫ���أ�
public:
	//��ʼ��
	virtual bool InitFrame(GameInfoStruct * pGameInfo, CWnd * pGameRoom);
	//��������
	virtual bool AFCCreateFrame();

public:
	//�رմ���
	virtual bool AFCCloseFrame();
private:
	//������Ϣ 
	virtual UINT OnControlMessage(UINT uControlCode, void * pControlData, UINT uDataSize);
	//�����ȡ��Ϣ
	virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
public:
	//��ʱ��
	virtual bool SetGameTimer(BYTE bDeskStation,UINT uTimeCount,UINT uTimeID);
	//ɱ��ʱ��
	virtual bool KillGameTimer(UINT uTimeID);
	//���Ż��㵽��ͼλ��
	virtual BYTE ViewStation(BYTE bDeskStation);

	virtual void KillAllTimer() {};

public:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

