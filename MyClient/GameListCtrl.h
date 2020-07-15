#pragma once
#include "StdAfx.h"
#include "comstruct.h"

struct KindInfo
{
	UINT			uKindID;               //��Ϸ���� ID
	char			szKindName[61];        //��Ϸ��������
};

struct NameInfo
{
	UINT			uKindID;				//��Ϸ���� ID 
	UINT			uNameID;				//��Ϸ���� ID 
	char			szGameName[61];			//��Ϸ����
	char			szGameProcess[61];		//��Ϸ������
	UINT			m_uOnLineCount;			//��������
};

struct RoomInfo
{
    UINT			uComType;			//��Ϸ����
	UINT			uKindID;			//��Ϸ���� ID ����
	UINT			uNameID;			//��Ϸ���� ID ����
	UINT			uRoomID;			//��Ϸ���� ID ����
	UINT			uPeopleCount;		//��Ϸ��������
	UINT			iUpPeople;			//���������û��ﵽֵ��ſ���
	UINT			uDeskPeople;		//ÿ����Ϸ����
	UINT			uDeskCount;			//��Ϸ����������Ŀ
	UINT			uServicePort;		//��������˿�
	char			szServiceIP[25];	//������ IP ��ַ
	char			szGameRoomName[61];	//��Ϸ��������
	int				uVirtualUser;		//20081211 , Fred Huang
	int				uVirtualGameTime;	//20081211 , Fred Huang
	UINT			uVer;				//�汾
	UINT			dwRoomRule;			//��Ϸ�������
	bool			bVIPRoom;			//< ���«�������Ƿ�VIP���䣨VIP�����������룩
	int				iBasePoint;			//��������
	UINT			uLessPoint;			//��һ��������
	UINT			uMaxPoint;			//��һ��������
	UINT			uRoomTypeID;		//����һ�����ࣨ������0���߼���1��������2��
	UINT			uRoomTypeSecondID;	//����������ࣨ��Ҫ��������һ������ķ�������ģ��������1�ŷ�0��������2�ŷ�1��3�ŷ�2�ȣ�

	int				iContestID;
	__int64			i64TimeStart;
	__int64			i64TimeEnd;

	//���޷���ID��ʱ��
	int				iLeaseID;
	__int64			i64LeaseTimeStart;
	__int64			i64LeaseTimeEnd;
	//��ս������ID       --begin
	UINT			uBattleRoomID;
	//��ս��������Ϣ��
	char			szBattleGameTable[31];
	bool			bHasPassword;			// �������� Add by lxl 2010-10-08
	UINT			dwTax;					// ���䷿�� Add by wyl 11-5-16
	UINT			uUserDeskCount;			//ʹ�õ����Ӻ�
};


//��Ϸ�б������
class CGameListCtrl
{
public:
	CGameListCtrl() {}
	virtual ~CGameListCtrl();

protected:
    bool UpdateNameOnLineData(DL_GP_RoomListPeoCountStruct* arrInfos, UINT nCnt);
	bool UpdateRoomOnLineData(DL_GP_RoomListPeoCountStruct* arrInfos, UINT nCnt);
	
//���ܺ���
public:
	// ���
	bool AddGameKind(ComKindInfo* arrKinds, UINT nCnt);
	bool AddGameName(ComNameInfo* arrNames, UINT nCnt);
	bool AddGameRoom(ComRoomInfo* arrRooms, UINT nCnt, UINT uKindID, UINT uNameID);

	//��ȡ
	KindInfo* FindKindInfo(UINT uKindID);
	NameInfo* FindNameInfo(UINT uNameID);
	RoomInfo* FindRoomInfo(UINT uRoomID);

	//��������������
	bool UpdateOnLineCount();
	//��ȡ��������
	bool GetProcessName(UINT nRoomID, TCHAR* szProcessName, UINT uBufferSize);
	//��ȡ��Ϸ����
	bool GetGameName(UINT nRoomID, TCHAR* szGameName, UINT uBufferSize);

private:
	map<UINT, KindInfo*>		m_mapGameKind;
	map<UINT, NameInfo*>		m_mapGameName;
	map<UINT, RoomInfo*>		m_mapGameRoom;
};

/*******************************************************************************************************/	
