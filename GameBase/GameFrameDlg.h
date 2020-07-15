#pragma once

#include "AICommonDef.h"

#define IDD_GAME_FRAME                  4001
#define IDM_SET_GAME_TIME				WM_USER+100
#define IDM_KILL_GAME_TIME				WM_USER+101

//游戏基础框架 
class AFX_EXT_CLASS CLoveSendClass: public CDialog, public IFrameInterface
{
	//变量定义
protected:
	CWnd								* m_pGameRoom;					//房间窗口

	GameInfoStruct						* m_pGameInfo;					//游戏信息指针
	UserItemStruct						* m_pUserInfo[MAX_PEOPLE];		//游戏玩家信息
	UINT								m_uTimeID;						//定时器 ID
	UINT								m_uTimeCount;					//定时器计数
	BYTE								m_bTimeStation;					//定期器位置
	TCHAR								m_ErrorMsg[MAX_PATH];			//错误日志
	int                                 m_nPowerOfGold;                ///金币倍率
	//函数定义

public:
	//构造函数
	CLoveSendClass(UINT uTemplate);
	//析构函数
	virtual ~CLoveSendClass();

	enum { IDD = IDD_GAME_FRAME };		     	//ID 号码
	//重载函数
protected:
	//初始化函数
	virtual BOOL OnInitDialog();
	//倥件绑定函数
	virtual void DoDataExchange(CDataExchange * pDX);

protected:
	//游戏消息处理函数
	virtual bool HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
	//设置游戏状态
	virtual bool SetGameStation(void * pStationData, UINT uDataSize) = 0;
	//重新设置界面
	virtual void ResetGameFrame();
	//重新设置游戏数据
	virtual void ResetGameStation(int iGameStation = 1){};

	//游戏控制函数
protected:
	//同意开始游戏 
	virtual bool OnControlHitBegin() = 0;

	//功能函数
public:
	void SetInifile(LPCTSTR path);
	//获取自己信息
	UserInfoStruct * GetMeUserInfo() { return &m_pGameInfo->pMeUserInfo->GameUserInfo; };
	//设置状态参数
	void SetStationParameter(BYTE bGameStation);
	//获取状态参数
	virtual BYTE GetStationParameter();
	//发送网络消息函数
	int SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
	//发送网络消息函数
	int SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);

	//框架函数
	virtual bool OnFrameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);

	//接口函数（不需要重载）
public:
	//初始化
	virtual bool InitFrame(GameInfoStruct * pGameInfo, CWnd * pGameRoom);
	//建立窗口
	virtual bool AFCCreateFrame();

public:
	//关闭窗口
	virtual bool AFCCloseFrame();
private:
	//控制消息 
	virtual UINT OnControlMessage(UINT uControlCode, void * pControlData, UINT uDataSize);
	//网络读取消息
	virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket);
public:
	//计时器
	virtual bool SetGameTimer(BYTE bDeskStation,UINT uTimeCount,UINT uTimeID);
	//杀计时器
	virtual bool KillGameTimer(UINT uTimeID);
	//桌号换算到视图位置
	virtual BYTE ViewStation(BYTE bDeskStation);

	virtual void KillAllTimer() {};

public:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

