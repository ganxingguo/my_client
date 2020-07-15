#pragma once
//#include "defines.h"
//#include "../../../../sdk/include/GameLanchSdk/LoveSendClassInExe.h"
#include "upgrademessage.h"
#include "./UpGradeLogic.h"
#include <list>

#define MAST_PEOPLE			-1							//�Ƿ�Ϊ����Ա

//��ʱ�� ID
#define ID_BEGIN_TIME				100					//��ʼ��ʱ��
#define ID_THINK_TIME				103					//˼����ʱ��
#define ID_SCARD_TIME				104					//���ƶ�ʱ��
#define ID_RESULT_TIME				105					//������ʱ��


//��Ϸ����� 
class CClientGameDlg : public CLoveSendClass
{
private:
	CUpGradeGameLogic		m_Logic;							///�߼�

	IniData					m_sIData;							///�����INI�ļ�����

	BData					m_sBData;							///��Ϸ��������

	BYTE					m_bGameStation;						///��Ϸ״̬

	BYTE					m_byLgStation;						///����߼�λ��

	bool					m_bWatchMode;						///�Ƿ�Ϊ�Թ���

	bool					m_bWatch;							///�Ƿ�����Թ�

	bool					m_bSound;							///�Ƿ񲥷���Ч

	int						m_iUCCount[PLAY_COUNT];				///�����������

	BYTE					m_byUCard[PLAY_COUNT][USER_HAND_CARD];	///����˿�
				
	BYTE                    m_byDownBCard[PLAY_COUNT][USER_HAND_CARD];			///�¶���

	int						m_iSCCount;							///��������

	int						m_iGamePlayNum;						//��Ϸ����

	int						m_iRealPlayNum;						//��ʵ�������

	bool					m_bZhuangjia;						//�Ƿ���ʵ�����ׯ

	bool					m_bIsNote;							//�������Ƿ�ȴ���ʵ�����ע�����ע

	bool					m_RobotMode;						//�����˲���ģʽ Ĭ��Ϊ0(0Ϊ���˰棬1Ϊ���ٰ�)
	
public:
	//���캯��
	CClientGameDlg();
	//��������
	virtual ~CClientGameDlg();


	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	LRESULT OnShowFinishDlg(WPARAM wParam, LPARAM lParam);
public:
	/**
	*��ʼ����Ϸ����
	*
	*return bool ��ʼ����Ϸ�����Ƿ�ɹ�
	*/
	bool InitGameData();

	/**
	* ����������Ϸ�пͻ��˵�����
	*
	*return bool true:�������óɹ���false:��������ʧ��
	*/
	bool  ResetGameData();

	/**
	* ��ʾ
	*/
	void OnHitCue(); 

	/**
	* ̯��
	*/
	void OnHitTanPai();

	/**
	* ��ӡ��־
	*
	*/
	void DebugPrintf(const char *p, ...);

public:

	//--------------------------------���غ���-----------------------------------------
	//������Ϸ״̬
	virtual bool SetGameStation(void * pStationData, UINT uDataSize);

	//������ж�ʱ��
	void KillAllTimer();
	
	//��ʱ����Ϣ
	virtual bool OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount);

	//ͬ�⿪ʼ��Ϸ 
	virtual bool OnControlHitBegin();

	//��Ϸ��Ϣ������
	virtual bool HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
};
