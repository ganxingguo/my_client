#pragma once

#ifndef AFC_GAME_ROOM_MESSAGE_HEAD_FILE
#define AFC_GAME_ROOM_MESSAGE_HEAD_FILE

//救济金结果
#define JIUJI_RESLUT					0
//取分结果
#define GETSCORE_RESULT					1
//存分结果
#define STORESCORE_RESULT				2
//领取任务结果
#define TASK_RESULT						3
//购买结果
#define BUY_RESULT						4
//购买炮台结果
#define BUY_PAOTAI_RESULT				5
#define MAX_PROPCOUNT					20
#define MAX_EC_BYTE						3800

#define MATCH_NORMOL									1
#define MATCH_MAJONG									2
#define MATCH_FISH										3
#define MATCH_BAIREN									4
#define MATCH_DZ										5
#define MATCH_ERNN										6
#define MATCH_SDB										7
#define MATCH_REALPEOPLE								8
#define MATCH_HONGBAO									9



#define MAX_EXPERICE_TIME								10
/********************************************************************************************/

#include "BaseMessage.h"
#include "ComStruct.h"
#include <vector>
#include <oledb.h>
using namespace std;

#pragma pack(1)




//房间管理窗口数据更新
struct MSG_GR_GRM_UpData
{
	//奖池
	bool        bAIWinAndLostAutoCtrl;  //机器人控制输赢开关
	__int64     iAIWantWinMoney[3];     /**<机器人输赢控制：机器人赢钱区域1,2,3 4所用断点 */
	int         iAIWinLuckyAt[4];           /**<机器人输赢控制：机器人在区域1,2,3,4赢钱的概率 */
	__int64     iReSetAIHaveWinMoney;       //机器人输赢控制：重置机器人已经赢钱的数目

	__int64     iAIHaveWinMoney;        //机器人赢的钱
	//输赢控制
	bool        bWinProbCtrl;           //开关
	int         dwUserID_win[20];       //可读取20组，多了影响性能,玩家ID
	int         iProb_win[20];          //输赢概率[0,100]
	int         dwUserID_los[20];       //可读取20组，多了影响性能,玩家ID
	int         iProb_los[20];          //输赢概率[0,100]

	MSG_GR_GRM_UpData()
	{
		memset(this, 0, sizeof(MSG_GR_GRM_UpData));
	}
};

///游戏房间登陆
struct MSG_GR_S_RoomLogon
{
	UINT                                uNameID;                            ///名字 ID
	int                                 dwUserID;                           ///用户 ID
	UINT                                uRoomVer;                           ///大厅版本
	UINT                                uGameVer;                           ///游戏版本
	char                                szMD5Pass[50];                      ///加密密码
	bool								IsReturn;
	int									iGuildId;							///公会id
	char								szIP[20];							//ip
	int									iMatchId;							///比赛id
	char								szCityInfo[50];
};

struct MSG_GR_S_RoomMatch 
{
	int									dwUserID;
	int									iMatchType;							///百人匹配类型 0 玩家坐庄 1 系统坐庄
};
struct MSG_GR_S_RoomLogonByUserName
{
	UINT                                uNameID;                            ///名字 ID
	int                                 dwUserID;                           ///用户 ID
	char                                szName[61];                         ///登陆名字
	UINT                                uRoomVer;                           ///大厅版本
	UINT                                uGameVer;                           ///游戏版本
	char                                szMD5Pass[50];                      ///加密密码
	bool								IsReturn;
	int									iGuildId;							///公会id
	LONGLONG							MinScore;							//最低分数
	LONGLONG							MaxScore;							//最高分数
};

///公会房间匹配
struct MSG_GR_S_RoomMatchGuild
{
	int									iUserId;		///玩家id
	int									iGuildId;		///公会id
};

///获取用户扩展信息
struct MSG_GR_S_GetUserExpand
{
	int                                 dwUserID;                           ///用户 ID
};

///获取用户扩展信息
struct MSG_GR_S_UpdateRobotScore
{
	int                                 dwUserID;                           ///用户 ID
	LONGLONG							MinScore;							//最低分数
	LONGLONG							MaxScore;							//最高分数
};
///游戏房间登陆
struct MSG_GR_R_LogonResult
{
	int                                 dwGamePower;                        ///用户权限
	int                                 dwMasterPower;                      ///管理权限
	int                                 dwRoomRule;                         ///设置规则
	UINT                                uLessPoint;                         ///最少经验值
	UINT                                uMaxPoint;                          ///最多经验值
	UserInfoStruct                      pUserInfoStruct;                        ///用户信息
	RECEIVEMONEY                        strRecMoney;                        //非比赛场玩家金币不足自动赠送

	///wushuqun 2009.6.6
	///登录房间时即时获取虚拟玩家人数
	int                                 nVirtualUser;
	int                                 nPresentCoinNum;  ///< 赠送金币数量



	//比赛专用
    int                                 iMatchID;
    int                                 iMatchNeedCnt;
    __int64                             iSignupFee;
	__int64                             i64TimeStart;
	__int64                             i64TimeEnd;
	__int64                             i64LowChip;
    int                                 iMatchRule;
    int                                 iSignupCnt;

	bool                                bGRMUser;//该玩家可以打开房间管理窗口
	bool                                bGRMRoom;//该房间可以打开房间管理窗口

	MSG_GR_GRM_UpData                   GRM_Updata;//管理窗口更新数据

	//房间桌子加密有效条件规则
	UINT                                itime;//有效时间长度
	UINT                                inumber;//最大局数
	int                                 iSignupCntReal;

	MSG_GR_R_LogonResult()
	{
		memset(this, 0, sizeof(MSG_GR_R_LogonResult));
	}


};

///游戏房间登陆
struct MSG_GR_R_OtherRoom
{
	int                                 iRoomID;
	CHAR                                szGameRoomName[61];                 ///房间名字
};

///游戏桌子状态
struct MSG_GR_DeskStation
{
	BYTE                                bDeskStation[400];                  ///桌子状态
};

///////////////////////////////////////////////
///公会桌子更新类型
enum eUpdateType
{
	UpdateType_null = 0, ///空
	UpdateType_sit = 1, ///玩家坐下
	UpdateType_left = 2, ///玩家离开
	UpdateType_begin = 3, ///游戏开始
	UpdateType_end = 4, ///游戏结束
	UpdateType_close = 5, ///房间关闭
};

///桌子状态
enum eDeskState
{
	State_null = 0,
	State_playing = 1,
	State_full = 2,
};

///公会桌子玩家信息
struct GuildDeskUser
{
	int iUserId;
	int iLogoId;
	char nickName[100];
};

///公会桌子
struct GuildDesk
{
	int iDeskNo; ///桌子下标

	int	iDeskIndex; ///实际桌子索引

	GuildDeskUser iDeskUser[8];

	eDeskState	eState;

	GuildDesk()
	{
		iDeskNo = 0;
		iDeskIndex = -1;
		ZeroMemory(iDeskUser, sizeof(iDeskUser));
		eState = State_null;
	}

	void clearAll()
	{
		iDeskNo = 0;
		iDeskIndex = -1;
		ZeroMemory(iDeskUser, sizeof(iDeskUser));
		eState = State_null;
	}

	void clearUser(BYTE bStation)
	{
		ZeroMemory(&iDeskUser[bStation], sizeof(GuildDeskUser));
	}
};

///请求桌子信息
struct MSG_GR_C_GuildDeskReq
{
	int		iGuildId;
};


///请求桌子信息
struct MSG_GR_C_USERSCORELOG
{
	int		UserID;
};

///桌子信息
struct MSG_GR_GuildDesk
{
	GuildDesk		data[MAX_SEND_SIZE / sizeof(GuildDesk)];
	int				dataCount;

	void clear()
	{
		for (size_t i = 0; i < MAX_SEND_SIZE / sizeof(GuildDesk); i++) {
			data[i].clearAll();
		}
		dataCount = 0;
	}
};

///更新桌子信息
struct MSG_GR_GuildDeskUpdate
{
	int		iDeskIndex;
	eUpdateType updateType;
	BYTE	bDeskStation;
	GuildDeskUser deskUser;
	eDeskState eState;

	MSG_GR_GuildDeskUpdate()
	{
		iDeskIndex = -1;
		updateType = UpdateType_null;
		bDeskStation = 0xFF;
		ZeroMemory(&deskUser, sizeof(deskUser));
		eState = State_null;
	}
};

////////////////////////////////////////////////////////////////////////



///用户离开房间
struct MSG_GR_R_UserLeft
{
	int                         dwUserID;                           ///用户 ID
};

///用户坐下
struct MSG_GR_S_UserSit
{
	UINT                                bDeskIndex;                         ///桌子索引
	BYTE                                bDeskStation;                       ///桌子位置
	TCHAR                               szPassword[61];                     ///桌子密码
};




///用户断线
struct MSG_GR_R_UserCut
{
	int                                 dwUserID;                           ///用户 ID
	UINT                                bDeskNO;                            ///游戏桌号
	BYTE                                bDeskStation;                       ///位置号码
};

///用户坐下或者起来
struct MSG_GR_R_UserSit
{
	int                                 dwUserID;                           ///用户 ID
	BYTE                                bLock;                              ///是否密码
	UINT                                bDeskIndex;                         ///桌子索引
	BYTE                                bDeskStation;                       ///桌子位置
	BYTE                                bUserState;                         ///用户状态
	int                                 bIsDeskOwner;                       ///台主

	UserInfoStruct                      pUserInfoStruct;                        ///用户信息
};

struct MSG_GR_R_UserUp
{
	INT                                 dwUserID;                           //用户 ID
	BYTE                                bLock;                              //是否密码
	UINT                                bDeskIndex;                         //桌子索引
	BYTE                                bDeskStation;                       //桌子位置
	BYTE                                bUserState;                         //用户状态
	bool                                bIsDeskOwner;                       //台主离开
};


///用户进入房间
struct MSG_GR_R_UserCome
{
	UserInfoStruct                      pUserInfoStruct;                        ///用户信息
	//MSG_GR_R_UserSit                    sitInfo;                                ///坐下消息

};



///聊天结构
struct MSG_GR_HASH
{
	CHAR                                szMessage[65];        ///聊天内容
};

///聊天结构
struct MSG_GR_RS_NormalTalk
{
	COLORREF                            crColor;                            ///字体颜色
	SHORT                               iLength;                            ///信息长度
	int                                 dwSendID;                           ///用户 ID
	int                                 dwTargetID;                         ///目标 ID
	int                                 nDefaultIndex;                      ///=0，输入的内容，>0，选择的内容
	CHAR                                szMessage[MAX_TALK_LEN + 1];        ///聊天内容
};



///用户同意结构
struct MSG_GR_R_UserAgree
{
	UINT                                bDeskNO;                            ///游戏桌号
	BYTE                                bDeskStation;                       ///位置号码
	BYTE                                bAgreeGame;                         ///同意标志
};

///用户经验值
struct MSG_GR_R_UserPoint
{
	int                                 dwUserID;                           ///用户 ID
	__int64                             dwPoint;                            ///用户经验值
	__int64                             dwMoney;                            ///用户金币
	__int64                             dwSend;                             ///赠送
	BYTE                                bWinCount;                          ///胜局
	BYTE                                bLostCount;                         ///输局
	BYTE                                bMidCount;                          ///平局
	BYTE                                bCutCount;                          ///逃局
	RECEIVEMONEY                        strAutoSendMoney;                   //添加自动赠送 2013-01-29 wangzhitu
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///斗地主比赛

struct MSG_GR_S_BotMatchInfo
{
	MSG_GR_S_BotMatchInfo() 
	{
		ZeroMemory(this, sizeof(this));
	}

	int MatchID;
	int MatchRule;
	int MatchNeedCnt;
	__int64	iSignupFee;
	int SignupCnt;
	int SignupCntReal;
};

///后台 开启/关闭场次
struct MSG_GR_ContestWebOp
{
	int					iOpType; ///2 添加/开启 3 删除/关闭
	int					iMatchId;
};

///后台修改 通知玩家
struct MSG_GR_S_ContestWebOp
{
	int					iOpType; ///3 删除/关闭 4 修改报名费用
	int					iMatchId;
	__int64				iSignupFee;
};
///广播报名
struct MSG_GR_S_ContestBroadSignup
{
	int					iSignupCnt;
	int					iSignupCntReal;
};

///等待结算
struct MSG_GR_S_ContestWaitOver
{
	int					iMatchUserCount; ///当前剩余参赛人数
    int                 iRankNum;	///目前排名
    int                 iLeftDesk;  ///还剩几桌
};

///游戏中更新排名
struct MSG_GR_S_ContestUpdateRank
{
	int                 iRankNum;	///目前排名
};

///通知下一轮
struct MSG_GR_S_ContestNextRound
{
	int					iMatchUserCount; ///当前剩余参赛人数
	int					iRankNum; ///目前排名
};

///淘汰/结算
struct MSG_GR_S_ContestAward
{
	int					iRankNum;
	LONGLONG			iRankAward;
};

///请求奖励信息
struct MSG_GR_ContestAwardReq
{
	int					iUserID;
};

///奖励信息
struct MSG_GR_S_ContestAwardReq
{
	LONGLONG			iRankAward[20];
};

///报名
struct MSG_GR_ContestApply
{
    int					iUserID;  
    int					iTypeID;   //操作类型：0-报名，1-退赛
};

struct MSG_GR_S_ContestApply
{
	int					iTypeID;	//0 报名 1 退赛
};

///请求比赛信息
struct MSG_GR_UserContest
{
	int					iUserID;
};

///玩家比赛信息
struct MSG_GR_S_UserContest
{
	MSG_GR_S_UserContest()
	{
		ZeroMemory(this, sizeof(this));
	}

	int					iMatchState;
	int					iMatchID;
	int					iMatchRule;
	int					iMatchNeedCnt;
	int					iSignupCnt;
	bool				bSignup;

	int					iLeftDesk;
	int					iRankNum;
	int					iUserCount;
};

///修改用户桌子倍数设定
struct MSG_GR_S_SetDeskBasePoint
{
	int                                 dwUserID;                           ///用户ID
	int                                 iDeskBasePoint;                     ///用户设定的桌子倍数
	BYTE                                bDeskOwner;                         ///是否台主
};



///踢玩家下线数据包
struct MSG_GR_S_CutUser
{
	int                         dwUserID;                           ///管理员用户 ID
	int                         dwTargetID;                         ///目标用户 ID
};


/********************************************************************************************/
//存分
struct CMD_C_STORE_SCORE
{
	LONGLONG Score;
};

//取分
struct CMD_C_GET_SCORE
{
	LONGLONG							Score;
	char								MD5_BANKPASS[54];
};

//更新能量卡数量
struct CMD_C_UPDATE_PROPCARD
{
	int                                 UserID;                             ///用户ID
	int									PropCardID;
	int									PropCardCount;
};
//更新能量卡数量结果
struct CMD_S_UPDATE_PROPCARD
{
	int									ret;
	int									IsADD;
	int                                 UserID;                             ///用户ID
	int									PropCardID;
	int									PropCardCount;
};

//玩家同步能量卡
struct CMD_S_SYNCHRO_PROPCARD
{
	int									IsADD;
	int                                 UserID;                             ///用户ID
	int									PropCardID;
	int									PropCardCount;
};
//获取能量卡列表
struct CMD_C_GET_PROPCARD
{
	int                                 UserID;                             ///用户ID
};

///游戏信息
struct MSG_GM_S_ClientInfo
{
	BYTE                                bEnableWatch;                       ///允许旁观
};

struct MSG_GR_USERINFO
{
	INT										UserID;								//用户id
	INT64									RoomKey;							//房卡
	INT64									WalletMoney;						//金币
};

struct MSG_GR_C_GET_USERINFO
{
	int    dwUserID;           //用户ID
};


//玩家同步能量卡
struct CMD_S_HASH
{
	char        HashResult[64];
	BYTE        Reslut[1000];
};
///游戏信息
struct MSG_GM_S_GameInfo
{
	BYTE                                bGameStation;                       ///游戏状态
	BYTE                                bWatchOther;                        ///允许旁观
	BYTE                                bWaitTime;                          ///等待时间
	BYTE                                bReserve;                           ///保留字段
	UINT                                iGameRoundCount;                    //游戏局数(跟桌子加密绑定的)
	UINT                                iGameRoundMax;                      //游戏最大局数
	/*******************MSG_GAME_RULE**************************************************************/
	int									nCostID;							//房卡消耗ID
	__int64                             iGameRule;                          //gamerule
	bool								bAAFanKa;							//是否AA制房卡
	int									iCost;								//消耗房卡
	/*******************MSG_GAME_RULE**************************************************************/
	__int64                             i64UseDeskLockPassTime;             //桌子的使用剩余时间
	int                                 iGameStatisticsPoint[MAX_PEOPLE];//每个人开始的总分数
	char                                szPwd[61];                          //服务器密码

	int									iGuildId;							///公会Id
	//服务端写包不科学，但凡添加新成员变量，一律放在szMessage数组前面
	char                                szMessage[1000];                    ///系统消息

	MSG_GM_S_GameInfo()
	{
		ZeroMemory(this, sizeof(MSG_GM_S_GameInfo));
	}
};

///旁观设置
struct MSG_GM_WatchSet
{
	int                         dwUserID;                           ///设置对象
};



/********************************************************************************************/

///消息数据包
struct MSG_GA_S_Message
{
	BYTE                                bFontSize;                          ///字体大小
	BYTE                                bCloseFace;                         ///关闭界面
	BYTE                                bShowStation;                       ///显示位置
	CHAR                                szMessage[1000];                    ///消息内容
};





//游戏公告消息ATT
struct GameNoticeMessage
{
	UINT            bDeskIndex;         //桌子号
	BYTE            bAwardCardShape;    //牌型奖励
	TCHAR           szMessage[255];     //未进入游戏 在房间 右边框显示公告内容
	GameNoticeMessage()
	{
		::memset(this, 0, sizeof(GameNoticeMessage));
	}
};




struct MSG_GP_EC_LIST
{
	char                            info[MAX_EC_BYTE];
	int								EventID;
	int								GameID;
	int								uSize;
	INT							IsFirst;
};

struct MSG_GP_GET_SPECIAL_EC_LIST
{
	int			EventID;
};
struct MSG_GP_GET_GAMETYPE_EC_LIST
{
	int			EventID;
};
struct MSG_GP_GET_EVENT_EC_LIST
{
	int			EventID;
};
struct MSG_GP_GET_REALTIME_EC_LIST
{
	int			EventID;
	int			GameID;
};
struct MSG_GP_EC_END_GAME
{
	INT			EventID;
	INT			GameID;
	INT			BetID;
	INT         Bet_Num;
};

struct MSG_GP_EC_END_GAME_LIST
{
	MSG_GP_EC_END_GAME EndGame[200];
	int			       GameCount;
	INT							IsFirst;
};

struct MSG_GP_EC_BET
{
	int			EventID;
	INT			MatchID;
	int         BetID;
	int			BetNumID;
	INT			UserID;
	LONGLONG	lScore;
};

struct MSG_GP_EC_BET_RESULT
{
	int			lRet;
	int			EventID;
	INT			MatchID;
	int         BetID;
	INT			UserID;
	LONGLONG	lScore;
	INT			Rat;
};


struct MSG_GP_EC_GET_RESULT
{
	INT			UserID;
};


struct MSG_GP_EC_RESULT
{
	int			lEventID;
	INT			lMatchID;
	int         lBetID;
	int         lBetNumID;
	INT			lUserID;
	LONGLONG	lBetScore;
	INT			lBetRat;
	INT			lMatchState;
	LONGLONG	lWinScore;
	int			lCollecttime;
	int			lEndtime;
};

struct MSG_GR_WEB_BUY_MONEY
{
	INT       UserID;
};

struct MSG_GR_KICK_USEROUT
{
	INT       UserID;
};

struct MSG_GR_WEB_ADD_STOCK
{
	LONGLONG	lScore;
	bool        IsStart;
};

struct StoreResultStruct
{
	int									UserID;
	LONGLONG                            Score;                            //用户分数
	LONGLONG							OperatScore;
	INT									ret;
};


struct PropCardList
{
	int				UserID;
	int				PropCardID;
	int				PropCardCount;
};
struct BuyMoney
{
	int									  UserID;
	__int64								score;
	__int64								OperatScore;
};
//用户基本信息扩展
struct UserInfoExpand
{
	int				UserID;
	DOUBLE			iLotteries;       //礼券
	int				ExpPlus;
	int				IonPlus;
	int				FallCardPlus;
	int				HongBaoCount;
	int				BasicBonus;
};
//总局数据节点中外加的备用数据部分(一个玩家的数据)
struct Game_StatisticsOther
{
	int iReserveData[20];

	Game_StatisticsOther()
	{
		ZeroMemory(this, sizeof(Game_StatisticsOther));
	}
};

struct Game_StatisticsMessage
{
	//总局数据节点(一个玩家的数据)
	int                                 dwUserID;                           ///用户 ID
	int                                 taiZhuID;                            ///台主ID
	__int64                             dwPoint;                            ///用户经验值
	__int64                             dwMoney;                            ///用户金币
	__int64                             dwSend;                             ///赠送
	BYTE                                bWinCount;                          ///胜局
	BYTE                                bLostCount;                         ///输局
	BYTE                                bMidCount;                          ///平局
	BYTE                                bCutCount;                          ///逃局
	BYTE                                bMaxLzCount;                          ///最高连庄次数

	//保留字段，供具体游戏具体使用
	Game_StatisticsOther                data;

	Game_StatisticsMessage()
	{
		ZeroMemory(this, sizeof(Game_StatisticsMessage));
	}
};
#pragma pack()
//=====================================================================================
#define MDM_GR_WEB_INFO							160							//网页消息
#define ASS_GP_SEND_EC_LIST						5   ///所有电竞列表
#define ASS_GP_SEND_CLASS_EC_LIST						6	   ///赛事分类
#define ASS_GP_SEND_SPECIAL_EC_LIST						7	   ///4.1 返回某项目未开始和进行中的赛事专题
#define ASS_GP_SEND_GAMETYPE_EC_LIST						8	   ///赛事专题下的比赛列表
#define ASS_GP_SEND_EVENT_EC_LIST						9	   /// 获取某项目下比赛列表
#define ASS_GP_SEND_REALTIME_INFO								10	   ///比赛动态信息
#define ASS_GP_SEND_END_GAME_LIST								11	   ///比赛结束消息 

#define ASS_GP_SEND_BUY_MONEY								12	   ///网页通知充值成功 
#define ASS_GP_LOAD_STOCK								13	   ///加载库存 
#define ASS_GP_CLOSE_ROOM								14	   ///关闭房间 
#define ASS_GP_WEB_MATCH								15		///开启/关闭比赛场次
#define ASS_GP_GET_REAL_STOCK								16		///获取实时库存数据
#define ASS_GP_GET_ROOMMINSCORE								17		///重新加载房间最小金币
#define ASS_GP_KICK_USEROUT								18		///踢出房间
#define ASS_GP_WEB_SET_AGENTID							19		///设置桌子代理Id
#define ASS_GP_WEB_SET_CONTROL							20		//设置控制等级
#define ASS_GP_WEB_LOAD_ROOM_STOCK_INFO					21		//重新加载房间控制信息
#define ASS_GP_WEB_GAME_INFO							22		//web获取场境信息
#define ASS_GM_WEB_GAME_STATION							23		//游戏状态
#define ASS_WEB_LEFT_DESK								24		//web离开桌子

#define ASS_WEB_USER_SIT								25		//通知web玩家坐下
#define	ASS_WEB_USER_UP									26

#define ASS_WEB_SET_SHI_SHI_CONTROL_INFO				27		//后台设置时时控制信息

//=====================================================================================
#define MDM_GP_EC_INFO							161							//客户端电竞消息
#define ASS_GP_EC_LIST							1							//获取所有电竞列表
#define ASS_GP_EC_BET							2							//下注
#define ASS_GP_EC_RESULT						3							//获取战绩
#define ASS_GP_SPECIAL_EC_LIST							4							//获取所有电竞列表
#define ASS_GP_CLASS_EC_LIST							5							//获取赛事分类
#define ASS_GP_GAMETYPE_EC_LIST							6							//赛事专题下的比赛列表
#define ASS_GP_EVENT_EC_LIST						7					/// 获取某项目下比赛列表
#define ASS_GP_REALTIME_INFO							8				///比赛动态信息
#define ASS_GP_ENDGAME							9				///开奖结果
/********************************************************************************************/

///游戏主标识



#define ASS_GM_MESSAGE_EX               180                                 ///游戏消息特殊游戏ATT
/********************************************************************************************/

///大厅登陆
#define MDM_GR_LOGON                    100                                 ///大厅登陆
#define ASS_GR_LOGON_BY_ID              5                                   ///通过用户 ID 登陆
#define ASS_GR_LOGON_SUCCESS            2                                   ///登陆成功
#define ASS_GR_LOGON_ERROR              3                                   ///登陆失败
#define ASS_GR_SEND_FINISH              4                                   ///登陆完成
#define ASS_GR_GET_USEREXPAND           6                                   ///获取用户扩展信息
#define ASS_GR_UPDATE_ROBOT_SCORE       7                                   ///机器人获取金币
#define ASS_GR_LOGON_BY_USERNAME		8                                   ///机器人登录
#define ASS_GR_IS_VIPROOM               16                                   ///<是否VIP房间
#define ASS_GR_VIP_PW                   17                                   ///<VIP房间需要密码
#define ASS_GR_VIP_NO_PW                18                                   ///< VIP房间不需要密码(第一个进入不需要密码)
#define ASS_GR_NO_VIP                   19                                   ///< 不是VIP房间
#define ASS_GR_VIPROOM_PW               20                                   ///< VIP房间密码
#define ASS_GR_VIPROOM_PW_RIGHT         21                                   ///< VIP房间密码正确
#define ASS_BEGIN_MATCH_ROOM			22									//匹配房间
#define ASS_END_MATCH_ROOM				23									//取消匹配房间
#define ASS_GP_GETROOMINFO				24

#define ERR_GR_FULL_DESK					100000			///桌子已经满了
#define ERR_GR_NO_ENOUGH_ROOM_KEY			255				///玩家房卡不足
#define ERR_GR_IN_ROOM						254				///玩家正在游戏中
#define ERR_GR_GUILD_MATCHERR				253				///公会房间匹配错误

////////////////////////////////////////////////////////////////////
///用户列表
#define MDM_GR_USER_LIST                101                                 ///用户列表
#define ASS_GR_ONLINE_USER              1                                   ///在线用户
#define ASS_GR_NETCUT_USER              2                                   ///断线用户
#define ASS_GR_DESK_STATION             3                                   ///桌子状态
////////////////////////////////////////////////////////////////////
///用户动作
#define MDM_GR_USER_ACTION              102                                 ///用户动作
#define ASS_GR_USER_UP                  1                                   ///用户起来
#define ASS_GR_USER_SIT                 2                                   ///用户坐下
#define ASS_GR_WATCH_UP                 3                                   ///旁观起来
#define ASS_GR_WATCH_SIT                4                                   ///旁观坐下
#define ASS_GR_USER_COME                5                                   ///用户进入
#define ASS_GR_USER_LEFT                6                                   ///用户离开
#define ASS_GR_USER_CUT                 7                                   ///用户断线
#define ASS_GR_SIT_ERROR                8                                   ///坐下错误
#define ASS_GR_SET_TABLE_BASEPOINT      9                                   ///改变桌子倍数
#define ASS_GR_USER_HIT_BEGIN           10                                  ///用户同意开始
#define ASS_GR_JOIN_QUEUE               11                                  ///<加入排队机
#define ASS_GR_QUIT_QUEUE               12                                  ///<退出排队机
#define ASS_GR_QUEUE_USER_SIT           13                                  ///<排队用户坐下
#define ASS_GR_LEASE_TIMEOVER           14                                  ///<排队用户坐下
#define ASS_GR_SIT_CUTRESIT             16                                  //断线重连,重新坐下
#define ASS_GR_CONTEST_APPLY            24                                  //比赛场报名
	#define HC_GR_CONTEST_APPLY_ALREADY			1	///已报名
	#define HC_GR_CONTEST_APPLY_FULL			2	///人已满
	#define HC_GR_CONTEST_APPLY_MONEY			3	///金币不足
	#define HC_GR_CONTEST_APPLY_SIGNUPCNT		4	///报名次数已满
	#define HC_GR_CONTEST_APPLY_QUITERR			5	///退赛失败
	#define HC_GR_CONTEST_APPLY_ENABLE			6	///场次关闭
#define ASS_GR_EXPERCISE_OUTTIME		27                                  //练习场超时
////////////////////////////////////////////////////////////////////
///房间信息
#define MDM_GR_ROOM                     103                                 ///房间信息
#define ASS_GR_NORMAL_TALK              1                                   ///普通聊天
#define ASS_GR_HIGH_TALK                2                                   ///高级聊天
#define ASS_GR_USER_AGREE               3                                   ///用户同意
#define ASS_GR_GAME_BEGIN               4                                   ///游戏开始
#define ASS_GR_GAME_END                 5                                   ///游戏结束
#define ASS_GR_USER_POINT               6                                   ///用户经验值
#define ASS_GR_SHORT_MSG                7                                   ///聊短信息
#define ASS_GR_ROOM_SET                 8                                   ///房间设置
#define ASS_GR_INVITEUSER               9                                   ///邀请用户
#define ASS_GR_INSTANT_UPDATE           10                                  ///即时更新分数金币
#define ASS_GR_UPDATE_CHARM             11                                  ///即时更新魅力
#define ASS_GR_ROOM_PASSWORD_SET        12                                  ///房间设置
#define ASS_GR_ROOM_QUEUE_READY         13                                  ///排队机准备
#define ASS_GR_GET_NICKNAME_ONID        14          // 根据ID获取昵称，PengJiLin, 2010-8-23
#define ASS_GR_OWNER_T_ONE_LEFT_ROOM    15          // 房主踢玩家离开房间, PengJiLin, 2010-8-26
#define ASS_GR_GET_NICKNAME_ONID_INGAME 16          // 根据ID获取昵称 游戏中 lxl 2010-11-17
#define ASS_GR_USER_CONTEST             17                                  ///用户比赛信息
#define ASS_GP_CONTEST_WEB				18									///比赛房间消息通知
#define ASS_GR_CONTEST_ROUNDOVER        19                                  ///比赛一轮结束
#define ASS_GR_CONTEST_END				20                                  ///比赛结束
#define ASS_GR_CONTEST_AWARD            21                                  ///结算/淘汰
#define ASS_GR_CONTEST_WAIT_GAMEOVER    22                                  ///比赛结束，但是有用户还在打最后一局，通知已打完的用户等待排名
#define ASS_GR_CONTEST_UPDATE_RANK      23                                  ///更新排名
#define ASS_GR_CONTEST_AWARD_REQ        24                                  ///获取比赛奖励详情
#define ASS_GR_CONTEST_SIGNUP_BROAD     25                                  ///广播报名
#define ASS_GR_CONTEST_NEXT_ROUND       26                                  ///通知下一轮
#define ASS_GR_HASH				        28                                  ///hash值对应的结果
#define ASS_GR_GET_HASH					29                                  ///获取hash地址
#define ASS_GR_SEND_HASH				30                                  ///hash值
#define ASS_GR_GUILD_DESKINFO			31									///公会对战桌子信息
#define ASS_GR_GUILD_DESKINFO_UPDATE	32									///公会对战桌子更新
#define ASS_GR_USERSCORELOG				33									///获取用户日志
#define ASS_GR_NEWUSERSCORELOG				34									///新的用户日志




/********************************************************************************************/
#define MDM_GR_MESSAGE                  106                                 ///信息消息
///信息消息
#define ASS_GR_SYSTEM_MESSAGE           1                                   ///系统消息




///

/********************************************************************************************/

///框架消息
#define MDM_GM_GAME_FRAME               150                                 ///框架消息
#define ASS_GM_GAME_INFO				1                                   ///游戏信息
#define ASS_GM_GAME_STATION             2                                   ///游戏状态
#define ASS_GM_FORCE_QUIT               3                                   ///强行退出
#define ASS_GM_NORMAL_TALK              4                                   ///普通聊天
#define ASS_GM_HIGH_TALK                5                                   ///高级聊天
#define ASS_GM_WATCH_SET                6                                   ///旁观设置 
#define ASS_GM_CLEAN_USER               9                                   //比赛场清理用户信息
#define ASS_GM_USE_KICK_PROP            7                                   // 使用踢人卡
#define ASS_GM_USE_ANTI_KICK_PROP       8                                   // 使用防踢卡
#define ASS_GM_SET_VIDEOADDR            10                                  //设置视频服务器地址
#define ASS_GM_STATISTICS               11                                  //总局通知
#define CODE_GM_STATISTICS_PART         1//一部分数据
#define CODE_GM_STATISTICS_FINISH       2//最后的数据
#define ASS_GM_DESKLOCKPASS_UNUSE       12                                  ///桌子密码已经超时，请桌子上所有的玩家离开此桌子
#define ASS_GM_DESKLOCKPASS_TIMEOUT     13                                  ///桌子已经超过五分钟未开始了，清场重置
#define ASS_GM_DESK_SETRULE             14                                  ///设置规则

#define ASS_CONTINUE_NEXT_ROUND			17						//继续下一轮消息

#define ASS_STORE_SCORE					18						//游戏存分
#define ASS_GET_SCORE					19						//游戏取分
#define	ASS_JIUJI_RESULT				20						//发送救济金结果
#define	ASS_JIUJI_TIPS					21						//发送救济金提示
#define	ASS_LEVEL_UP					22						//发送升级提示
#define	ASS_GET_TASKLIST				23						//获取任务列表
#define	ASS_UPDATE_TASK					24						//获取任务奖励
#define	ASS_GET_TASKNAME				25						//获取任务名字
#define	ASS_SEND_COMPLETETASK			26						//获取完成任务数量
#define	ASS_SEND_BUYMONEY				27						//获取购买成功
#define	ASS_GET_PROP_CARD				28						//获取能量卡
#define	ASS_UPDATE_PROP_CARD			29						//更新能量卡
#define	ASS_SYNCHRO_PROP_CARD			30						//玩家同步能量卡
#define	ASS_RETRY_MATCH					31						//玩家重新报名
#define ASS_BUY_PaoTai					32						//玩家购买炮台

#define ASS_ROOM_GUILD_CHANGE			33						//公会修改
#define HC_ROOM_GUILD_DELETE				1	///解散
#define HC_ROOM_GUILD_TAXRATE				2	///税率
#define HC_ROOM_GUILD_CLOSEROOM				3	///关闭房间
#define HC_ROOM_GUILD_KICK					4	///被踢出厅



/********************************************************************************************/
#define MDM_GM_MESSAGE                  151                                 ///信息消息
///信息消息
#define ASS_GM_SYSTEM_MESSAGE           1                                   ///系统消息

#define ASS_GM_SYSTEM_MESSAGE_EX        2                                   ///系统消息特殊游戏专用（ATT）

/********************************************************************************************/
///游戏消息

#define MDM_GM_GAME_NOTIFY              180                                 ///游戏消息
#define ASS_GM_AGREE_GAME               1                                   ///同意游戏

#define ASS_GR_CHANGE_OWNER			    3                                   //切换房主
#define ASS_GR_USERINFO_GET			    4                                   //获取用户信息

/********************************************************************************************/
#define MDM_GM_GAME_ADMIN				181                                 ///游戏消息
#define ASS_ADMIN_USER_LIST_REQ			1									//
#define ASS_ADMIN_USER_LIST_ACK			2									//
#define ASS_ADMIN_GAME_SCENE			3									//
#define ASS_ADMIN_GAME_RECORD			4									//

/********************************************************************************************/

///错误代码
#define ERR_GR_ERROR_UNKNOW             0                                   ///未知错误
#define ERR_GR_LOGON_SUCCESS            1                                   ///登陆成功
#define ERR_GR_USER_NO_FIND             2                                   ///用户不存在
#define ERR_GR_USER_PASS_ERROR          3                                   ///用户密码错误
#define ERR_GR_USER_VALIDATA            4                                   ///用户帐号禁用
#define ERR_GR_USER_IP_LIMITED          5                                   ///登陆 IP 禁止
#define ERR_GR_IP_NO_ORDER              6                                   ///不是指定地址
#define ERR_GR_ONLY_MEMBER              7                                   ///会员游戏房间
#define ERR_GR_IN_OTHER_ROOM            8                                   ///正在其他房间
#define ERR_GR_ACCOUNTS_IN_USE          9                                   ///帐号正在使用
#define ERR_GR_PEOPLE_FULL              10                                  ///人数已经满
#define ERR_GR_LIST_PART                11                                  ///部分用户列表
#define ERR_GR_LIST_FINISH              12                                  ///全部用户列表
#define ERR_GR_STOP_LOGON               13                                  ///暂停登陆服务

#define ERR_GR_DESK_NULL	            20
#define ERR_GR_GUILDERR					21									///公会错误
#define ERR_GR_GUILD_ROOMERR			22									///公会房间错误
#define ERR_GR_CONTESTERR				23									///比赛信息错误



#define ERR_GR_MATCH_LOGON              160                                 ///游戏房间
#define ERR_GR_TIME_OVER                161                                 ///时间到期

///wushuqun 2009.6.5
///不在混战场活动时间内
#define ERR_GR_BATTLEROOM_OUTTIME       162

///用户坐下错误码
#define ERR_GR_SIT_SUCCESS              50                                  ///成功坐下
#define ERR_GR_BEGIN_GAME               51                                  ///游戏已经开始
#define ERR_GR_ALREAD_USER              52                                  ///已经有人存在
#define ERR_GR_PASS_ERROR               53                                  ///密码错误
#define ERR_GR_IP_SAME                  54                                  ///IP 相同
#define ERR_GR_CUT_HIGH                 55                                  ///断线率太高
#define ERR_GR_POINT_LOW                56                                  ///经验值太低
#define ERR_GR_POINT_HIGH               57                                  ///经验值太高
#define ERR_GR_NO_FRIEND                58                                  ///不受欢迎
#define ERR_GR_POINT_LIMIT              59                                  ///经验值不够
#define ERR_GR_CAN_NOT_LEFT             60                                  ///不能离开
#define ERR_GR_NOT_FIX_STATION          61                                  ///不是这位置
#define ERR_GR_MATCH_FINISH             62                                  ///比赛结束
#define ERR_GR_MONEY_LIMIT              63                                  ///金币太低
#define ERR_GR_MATCH_WAIT               64                                  ///比赛场排队提示
#define ERR_GR_IP_SAME_3                65                                  ///IP前3 相同
#define ERR_GR_IP_SAME_4                66                                  ///IP前4 相同
#define ERR_GR_UNENABLE_WATCH           67                                  ///不允许旁观
#define ERR_GR_DESK_FULL                68                                  /// 百家乐 桌子座位满了，无法分配座位给玩家 add by wlr 09.07.14
#define ERR_GR_WAIT_DESK_RECYLE         69                                  ///桌子还没有回收，暂时不能用

///玩家离开标识
#define ERR_GR_USER_UP_NOMAL			0									///正常离开
#define ERR_GR_USER_UP_MONEY			1									///金币不足
#define ERR_GR_USER_UP_GUILD_KICK		2									///VIP厅踢人
#define ERR_GR_USER_UP_NOACTION			3									///长时间未操作
#define ERR_GR_USER_UP_REMATCH			4									///重新匹配
#define ERR_GR_USER_UP_ROOMCLOSE		5									///VIP厅房间关闭
#define ERR_GR_USER_UP_RECORD_INFO    6                  //玩家保存进度离开




/********************************************************************************************/

#endif
