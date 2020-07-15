#pragma once

#ifndef CLIENTCOMSTRUCT_HEAD_FILE
#define CLIENTCOMSTRUCT_HEAD_FILE

//#include "PublicDefine.h"

/*******************************************************************************************************/

//������Ϸ�ͻ���//GetFrameInterface
#define GETFRAMEINTERFACE(GAME_DLG_CLASS) \
	extern "C" __declspec(dllexport) IFrameInterface * BeginInitGoGame() \
{ \
	return new GAME_DLG_CLASS();\
}

/*******************************************************************************************************/

//��ȡͼ�꺯��//GetGameIcon
#define GETGAMEICON(NAME_ID) extern "C" __declspec(dllexport) HICON BeginInitGameIcon(UINT uNameID, bool bGameList) \
{ \
	CString msg;\
	msg.Format("%d.ico",NAME_ID);\
	if (bGameList) return LoadIcon(GetModuleHandle(msg),MAKEINTRESOURCE(IDI_CLIENT_LIST)); \
	return LoadIcon(GetModuleHandle(msg),MAKEINTRESOURCE(IDI_CLIENT_ROOM)); \
}

/*******************************************************************************************************/

//��ȡ�Ǽǵȼ�����//GetOrderFunction
#define GETORDERFUNCTION(ORDER_FUNCTION) extern "C" __declspec(dllexport) GetOrderName * SetGameInitClass(UINT uNameID) \
{ \
	return GetGameOrderName; \
}

/*******************************************************************************************************/

//��ȡ�ͻ�����Ϣ//GetGameClientInfo
#define GETCLIENTINFO extern "C" __declspec(dllexport) bool InitShareMemory(ClientDLLInfoStruct * pClientInfo, UINT uVer) \
{ \
	if (uVer==DEV_LIB_VER) \
	{ \
		pClientInfo->uNameID=NAME_ID; \
		pClientInfo->uPlayCount=PLAY_COUNT; \
		pClientInfo->uProgramVer=GAME_MAX_VER; \
		pClientInfo->uChangeVer=GAME_CHANGE_VER; \
		pClientInfo->uSupperType=SUPPER_TYPE; \
		lstrcpy(pClientInfo->szGameName,GAMENAME); \
		CString msg;\
		msg.Format("%d.ico",NAME_ID);\
		lstrcpy(pClientInfo->szClientDLLName,msg); \
		return true; \
	} \
	return false; \
}

/*******************************************************************************************************/

//��ȡ�ȼ����ֺ�������
typedef TCHAR * (GetOrderName)(__int64 dwPoint);

//�ӿ�����
interface IFindUserInterface;
interface ISocketSendInterface;

/*******************************************************************************************************/

//�û���Ϣ�ṹ
struct UserItemStruct
{
	bool							bMatchUser;							//�Ƿ����
	UINT							uSortIndex;							//��������
	UINT							uConnection;						//��ҹ�ϵ
	int						        dwLogonTime;						//��½ʱ��
	UserInfoStruct					GameUserInfo;						//�û���Ϣ
};

//��Ϸ������Ϣ�ṹ
struct GameInfoStruct
{
	IFindUserInterface				* pIFindUser;						//���ҽӿ�
	//������Ϣ
	bool							bDeskOnwer;							//�Ƿ�̨��
	BYTE							bGameStation;						//��Ϸ״̬
	UserItemStruct					* pMeUserInfo;						//�����Ϣ
	long int						dwGamePower;						//�û�Ȩ��
	long int						dwMasterPower;						//����Ȩ��
	long int						dwRoomRule;							//���ù���
	GetOrderName					* pOrderName;						//�ȼ�����

	//���ò���
	bool							bEnableSound;						//��������
	bool							bEnableWatch;						//�����Թ�
	bool							bShowUserInfo;						//��ʾ����
	bool							bEnableBackSound;					//��������
	//�ӿڱ���
	ISocketSendInterface			* pISocketSend;						//���ͽӿ�

	//��������
	TCHAR							szProcessName[31];					//��������
	UINT							uDeskPeople;						//��Ϸ����
	UINT							uRoomID;							//�����ʾ
	DWORD							dwGameMSVer;						//���汾��
	DWORD							dwGameLSVer;						//���汾��
	UINT							uComType;							//��Ϸ����
	UINT							uNameID;							//���ֱ�ʾ
	TCHAR							szGameName[61];						//��Ϸ����
	UINT							uLessPoint;							//���پ���ֵ

	int							uVirtualUser;						//20081211 , Fred Huang
	int							uVirtualGameTime;					//20081211 , Fred Huang
	bool                            bIsInRecord;                        //�Ƿ���¼���� yjj  090223  
};

//��ȡ��Ϸ����ṹ
struct ClientDLLInfoStruct
{
	UINT							uNameID;							//���� ID
	UINT							uChangeVer;							//�޸İ汾
	UINT							uProgramVer;						//����汾
	UINT							uPlayCount;							//��Ϸ����
	UINT							uSupperType;						//֧������
	TCHAR							szGameName[61];						//��Ϸ����
	TCHAR							szClientDLLName[61];				//�ļ�����
};

/*******************************************************************************************************/

#endif