#pragma once

#include "basemessage.h"
#include "TCPClientSocket.h"
/*******************************************************************************************************/

//#define  GS_SEND_CARD 20
//#define  GS_WAIT_NEXT 23


//������Ϣ����
#define CM_USER_STATE					1						//״̬�ı�
#define IDM_GETPOINT						WM_USER + 430				//����
#define IDM_GET_ROOMNAME					WM_USER+435					//�õ���������

//��������
#define ACT_USER_UP						1						//�û�����
#define ACT_WATCH_UP					2						//�Թ�����
#define ACT_USER_SIT					3						//�û�����
#define ACT_WATCH_SIT					4						//�Թ�����
#define ACT_USER_CUT					5						//�û�����
#define ACT_USER_AGREE					6						//�û�ͬ��
#define ACT_GAME_BEGIN					7						//��Ϸ��ʼ
#define ACT_GAME_END					8						//��Ϸ����
#define ACT_USER_POINT					9						//�û�����ֵ

//�û�״̬�ı�
struct CM_UserState_Change
{
	BYTE								bActionCode;			//��������
	BYTE								bDeskStation;			//����λ��
	UserItemStruct						* pUserItem;			//�û�ָ��
};

/*******************************************************************************************************/

//��Ϸ��ܽӿ�
interface IFrameInterface
{
	//��������
public:
	//��ʼ�����״�����ʹ�ã�
	virtual bool InitFrame(GameInfoStruct * pGameInfo, CWnd * pGameRoom)=0;
	virtual ~IFrameInterface() {}
	//��������
	virtual bool AFCCreateFrame()=0;
	//�رմ���
	virtual bool AFCCloseFrame()=0;
	//������Ϣ 
	virtual UINT OnControlMessage(UINT uControlCode, void * pControlData, UINT uDataSize)=0;
	//�����ȡ��Ϣ
	virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)=0;
	//��Ϸ��ʱ��
	virtual bool OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount) = 0;

	virtual void KillAllTimer() = 0;

	virtual BYTE GetStationParameter() = 0;
};

/*******************************************************************************************************/


/*******************************************************************************************************/

//�����û��ӿ�
interface IFindUserInterface
{
	//��������
public:
	//�����û�
	virtual UserItemStruct * FindOnLineUser(long int dwUserID)=0;
	//�����û�
	//virtual UserItemStruct * FindOnLineUser(int dwAccID)=0;
	//�����û�
	virtual INT_PTR FindOnLineUser(const TCHAR * szUserName, CPtrArray & ResultPtrArray)=0;
	//�����û�
	virtual INT_PTR FindOnLineUser(BYTE bDeskIndex, CPtrArray & PlayPtrArray, CPtrArray & WatchPtrArray)=0;

	virtual INT_PTR GetOnLineUserCount() = 0;

	virtual UserItemStruct * FindOnLineUserByIndex(int index) = 0;

	//�����û�
	virtual UserItemStruct * AddGameUser(UserInfoStruct * pUserInfo, UINT uSortIndex, UINT uConnection) = 0;
	//ɾ���û�
	virtual bool DeleteGameUser(long int dwUserID) = 0;
	//ɾ���û�
	virtual bool DeleteGameUser(UserItemStruct * pUserInfo) = 0;
};


/*******************************************************************************************************/
//������Ϣ���ͽӿ�
interface ISocketSendInterface
{
	//��������
public:
	//����������Ϣ����
	virtual int SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)=0;
	//����������Ϣ����
	virtual int SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)=0;

	//	virtual CTCPClientSocket* GetNewSocketPtr() = 0;
	//	virtual void ReleaseSocket() = 0;
};
/*******************************************************************************************************/