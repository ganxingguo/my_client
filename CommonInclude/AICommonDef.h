#pragma once

#include "basemessage.h"
#include "TCPClientSocket.h"
/*******************************************************************************************************/

//#define  GS_SEND_CARD 20
//#define  GS_WAIT_NEXT 23


//控制消息定义
#define CM_USER_STATE					1						//状态改变
#define IDM_GETPOINT						WM_USER + 430				//积分
#define IDM_GET_ROOMNAME					WM_USER+435					//得到房间名称

//动作代码
#define ACT_USER_UP						1						//用户起来
#define ACT_WATCH_UP					2						//旁观起来
#define ACT_USER_SIT					3						//用户坐下
#define ACT_WATCH_SIT					4						//旁观坐下
#define ACT_USER_CUT					5						//用户断线
#define ACT_USER_AGREE					6						//用户同意
#define ACT_GAME_BEGIN					7						//游戏开始
#define ACT_GAME_END					8						//游戏结束
#define ACT_USER_POINT					9						//用户经验值

//用户状态改变
struct CM_UserState_Change
{
	BYTE								bActionCode;			//动作代码
	BYTE								bDeskStation;			//桌子位置
	UserItemStruct						* pUserItem;			//用户指针
};

/*******************************************************************************************************/

//游戏框架接口
interface IFrameInterface
{
	//函数定义
public:
	//初始化（首次生成使用）
	virtual bool InitFrame(GameInfoStruct * pGameInfo, CWnd * pGameRoom)=0;
	virtual ~IFrameInterface() {}
	//建立窗口
	virtual bool AFCCreateFrame()=0;
	//关闭窗口
	virtual bool AFCCloseFrame()=0;
	//控制消息 
	virtual UINT OnControlMessage(UINT uControlCode, void * pControlData, UINT uDataSize)=0;
	//网络读取消息
	virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)=0;
	//游戏计时器
	virtual bool OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount) = 0;

	virtual void KillAllTimer() = 0;

	virtual BYTE GetStationParameter() = 0;
};

/*******************************************************************************************************/


/*******************************************************************************************************/

//查找用户接口
interface IFindUserInterface
{
	//函数定义
public:
	//查找用户
	virtual UserItemStruct * FindOnLineUser(long int dwUserID)=0;
	//查找用户
	//virtual UserItemStruct * FindOnLineUser(int dwAccID)=0;
	//查找用户
	virtual INT_PTR FindOnLineUser(const TCHAR * szUserName, CPtrArray & ResultPtrArray)=0;
	//查找用户
	virtual INT_PTR FindOnLineUser(BYTE bDeskIndex, CPtrArray & PlayPtrArray, CPtrArray & WatchPtrArray)=0;

	virtual INT_PTR GetOnLineUserCount() = 0;

	virtual UserItemStruct * FindOnLineUserByIndex(int index) = 0;

	//增加用户
	virtual UserItemStruct * AddGameUser(UserInfoStruct * pUserInfo, UINT uSortIndex, UINT uConnection) = 0;
	//删除用户
	virtual bool DeleteGameUser(long int dwUserID) = 0;
	//删除用户
	virtual bool DeleteGameUser(UserItemStruct * pUserInfo) = 0;
};


/*******************************************************************************************************/
//网络消息发送接口
interface ISocketSendInterface
{
	//函数定义
public:
	//发送网络消息函数
	virtual int SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)=0;
	//发送网络消息函数
	virtual int SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)=0;

	//	virtual CTCPClientSocket* GetNewSocketPtr() = 0;
	//	virtual void ReleaseSocket() = 0;
};
/*******************************************************************************************************/