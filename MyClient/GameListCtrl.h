#pragma once
#include "StdAfx.h"
#include "comstruct.h"

struct KindInfo
{
	UINT			uKindID;               //游戏类型 ID
	char			szKindName[61];        //游戏类型名字
};

struct NameInfo
{
	UINT			uKindID;				//游戏类型 ID 
	UINT			uNameID;				//游戏名称 ID 
	char			szGameName[61];			//游戏名称
	char			szGameProcess[61];		//游戏进程名
	UINT			m_uOnLineCount;			//在线人数
};

struct RoomInfo
{
    UINT			uComType;			//游戏类型
	UINT			uKindID;			//游戏类型 ID 号码
	UINT			uNameID;			//游戏名称 ID 号码
	UINT			uRoomID;			//游戏房间 ID 号码
	UINT			uPeopleCount;		//游戏在线人数
	UINT			iUpPeople;			//比赛房间用户达到值后才开赛
	UINT			uDeskPeople;		//每桌游戏人数
	UINT			uDeskCount;			//游戏大厅桌子数目
	UINT			uServicePort;		//大厅服务端口
	char			szServiceIP[25];	//服务器 IP 地址
	char			szGameRoomName[61];	//游戏房间名称
	int				uVirtualUser;		//20081211 , Fred Huang
	int				uVirtualGameTime;	//20081211 , Fred Huang
	UINT			uVer;				//版本
	UINT			dwRoomRule;			//游戏房间规则
	bool			bVIPRoom;			//< 金葫芦二代，是否VIP房间（VIP房间设置密码）
	int				iBasePoint;			//基础倍数
	UINT			uLessPoint;			//金币或积分下限
	UINT			uMaxPoint;			//金币或积分上限
	UINT			uRoomTypeID;		//房间一级分类（初级场0、高级场1、富豪场2）
	UINT			uRoomTypeSecondID;	//房间二级分类（主要是用来给一级分类的房间排序的，如初级场1号房0，初级场2号房1，3号房2等）

	int				iContestID;
	__int64			i64TimeStart;
	__int64			i64TimeEnd;

	//租赁房间ID与时间
	int				iLeaseID;
	__int64			i64LeaseTimeStart;
	__int64			i64LeaseTimeEnd;
	//混战场房间ID       --begin
	UINT			uBattleRoomID;
	//混战场房间信息表
	char			szBattleGameTable[31];
	bool			bHasPassword;			// 有无密码 Add by lxl 2010-10-08
	UINT			dwTax;					// 房间房费 Add by wyl 11-5-16
	UINT			uUserDeskCount;			//使用的桌子号
};


//游戏列表控制类
class CGameListCtrl
{
public:
	CGameListCtrl() {}
	virtual ~CGameListCtrl();

protected:
    bool UpdateNameOnLineData(DL_GP_RoomListPeoCountStruct* arrInfos, UINT nCnt);
	bool UpdateRoomOnLineData(DL_GP_RoomListPeoCountStruct* arrInfos, UINT nCnt);
	
//功能函数
public:
	// 添加
	bool AddGameKind(ComKindInfo* arrKinds, UINT nCnt);
	bool AddGameName(ComNameInfo* arrNames, UINT nCnt);
	bool AddGameRoom(ComRoomInfo* arrRooms, UINT nCnt, UINT uKindID, UINT uNameID);

	//获取
	KindInfo* FindKindInfo(UINT uKindID);
	NameInfo* FindNameInfo(UINT uNameID);
	RoomInfo* FindRoomInfo(UINT uRoomID);

	//更新总在线人数
	bool UpdateOnLineCount();
	//获取进程名字
	bool GetProcessName(UINT nRoomID, TCHAR* szProcessName, UINT uBufferSize);
	//获取游戏名字
	bool GetGameName(UINT nRoomID, TCHAR* szGameName, UINT uBufferSize);

private:
	map<UINT, KindInfo*>		m_mapGameKind;
	map<UINT, NameInfo*>		m_mapGameName;
	map<UINT, RoomInfo*>		m_mapGameRoom;
};

/*******************************************************************************************************/	
