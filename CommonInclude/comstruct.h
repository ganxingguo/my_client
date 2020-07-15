#pragma once

#ifndef AFC_COM_STRUCT_HEAD_FILE
#define AFC_COM_STRUCT_HEAD_FILE

/********************************************************************************************/
#include "PropMessage.h"

///房间规则设置
#define GRR_MEMBER_ROOM         0x00000001L                         ///会员房间
#define GRR_IP_LIMITED          0x00000002L                         ///地址限制
#define GRR_ENABLE_WATCH        0x00000004L                         ///允许旁观
#define GRR_UNENABLE_WATCH      0x00000008L                         ///不许旁观
#define GRR_AUTO_SIT_DESK       0x00000010L                         ///自动坐下，现在用于防作弊场使用
#define GRR_LIMIT_DESK          0x00000020L                         ///限制座位
#define GRR_LIMIT_SAME_IP       0x00000040L                         ///限制同IP
#define GRR_RECORD_GAME         0x00000080L                         ///记录游戏
#define GRR_STOP_TIME_CONTROL   0x00000100L                         ///停止时间控制
#define GRR_ALL_ON_DESK         0x00000200L                         ///是否所有人坐下才开始启动
#define GRR_FORBID_ROOM_TALK    0x00000400L                         ///禁止房间聊天
#define GRR_FORBID_GAME_TALK    0x00000800L                         ///禁止游戏聊天
#define GRR_MATCH_REG           0x00001000L                         ///比赛报名
#define GRR_EXPERCISE_ROOM      0x00002000L                         ///训练场
#define GRR_VIDEO_ROOM          0x00004000L                         ///视频房间
///
///修改防作弊场可看见其他玩家姓名和头像问题！
#define GRR_NOTFUFENG           0x00008000L     ///不允许负积分
#define GRR_NOTCHEAT            0x00010000L     ///防作弊
///
///台费场
#define GRR_ALL_NEED_TAX        0x00020000L     ///收台费场,所有人都需缴纳一定数额台费
#define GRR_QUEUE_GAME          0x00040000L     /// 排队机

#define GRR_NOT_COST_POINT      0x00080000L     ///金币场不扣积分

#define GRR_CONTEST             0x00100000L     ///定时淘汰比赛场
#define GRR_TIMINGCONTEST       0x00800000L     ///定时赛        --RoomRule == 8388608


///百家乐
#define DESK_TYPE_NORMAL        0       ///
#define DESK_TYPE_BJL           1       ///



//=======================================分布式时，Z与B的通讯协议 rongqf20120425
#define MDM_ZD_PACKAGE                  123
#define ASS_ZD_CONNECT                  1
#define ASS_ZD_LOGON                    2

#define HDC_ZD_CONNECT                  1
#define HDC_ZD_DISCONNECT               2
#define HDC_ZD_KICK                     3
#define ASS_ZD_BATCH                    3
#define ASS_ZD_SINGLE                   4
#define HDC_ZD_FAIL                     2
#define ASS_ZD_BATCH_ToW                5               //转发给所有的W服务器
#define ASS_ZD_BATCH_ToGame             6               //转发给所有的W服务器
#define ASS_ZD_SINGLE_ToGame            7
#define ASS_ZD_POST_WEB					8				//投递网页

//喇叭消息
#define MZTW_WORLD_HORN							2//世界喇叭主消息id
#define ASS_ZTW_SEND							1//发送世界喇叭辅助消息id


//玩家信息
#define MZTW_USERINFO							4//通知玩家信息id
#define ASS_ZTW_SCORE							1//通知玩家信息id
#define ASS_ZTW_DJ_ENDGAME						2//通知电竞结束消息
//房卡信息
#define MZTW_ROOM_KEY					3//房卡消息id

#define ASS_ZTW_ROOM_KEY_USE				1//发送辅助消息id（主服务器通知游戏服务器某房间的某桌子已经可以使用了）
#define ASS_ZTW_ROOM_CUTNET_USER				2//发送辅助消息id（询问该玩家是否在某房间内处于断线状态）
#define RES_TELL_CLIENT						0	//通知客户端
#define RES_UN_TELL_CLIENT					1//不用通知客户端


//房间信息
#define MZTW_ROOMINFO						5//通知房间信息id
#define ASS_ZTW_GAMERECORED					1//通知游戏记录
#define ASS_ZTW_NEWGAMERECORED				2//通知新游戏记录
#define ASS_ZTW_CHANGEGAMESTATION			3//修改游戏状态
#define ASS_ZTW_WEB_POST					4   //网页投递

///公会消息
#define MZTW_GUILD							6

#define ASS_ZTW_GUILD_DELETE				1
#define ASS_ZTW_GUILD_CHANGERULE			2
#define ASS_ZTW_GUILD_KICKUSER				3



#pragma pack(1)



struct MZTW_Mess_Room_Key_Use_struct
{
	int				iUserID;			//用户id
	int				iNameID;			//游戏ID
	int				iRoomID;			//房间ID
	int				iDeskID;			//桌子ID
	char			szLockPass[61];		//上锁密码
	COleDateTime                        tTime;                              //使用记录时间
	int       id;             //id
	int       iGameRound;     //局数
	__int64   iGameRule;      //flag
	int		  iCost;		  //消耗房卡
	bool	  bAAFanKa;		  //是否AA制房卡
	MZTW_Mess_Room_Key_Use_struct()
	{
		ZeroMemory(this, sizeof(MZTW_Mess_Room_Key_Use_struct));
	}
};



struct MZTW_Guild_Delete 
{
	int		iGuildId;	///公会id
};
struct MZTW_Guild_ChangeRule 
{
	int		iGuildId;	///公会id
	BYTE	bOpType;	///1 修改税率 2 移除房间
	int		iTaxRate;
};
struct MZTW_Guild_KickUser
{
	int		iGuildId;	///公会id
	int		iUserId;
};


struct MSG_ZDistriInfo
{
	int     dwUserID;
	int     nZID;
	int     uSocketIndex;
	char    MachineCode[64];
};
struct MSG_KickUser
{
	int     uSocketIndex;
	int		nConnectID;
};
struct MSG_GP_S_BASIC_ROOMINFO
{
    MSG_GP_S_BASIC_ROOMINFO()
    {
        memset(this, 0, sizeof(MSG_GP_S_BASIC_ROOMINFO));
    }
	INT				NameID;
	int				RoomID;
	INT				OnlineUser;
	LONGLONG		RoomMinScore;
	INT				GameStation;
	int				LeftTime;
	int				ChangeTime;
	int				NowTime;
	int				DeskID;
};
struct MSG_GP_S_GET_ROOMINFO
{
	MSG_GP_S_BASIC_ROOMINFO		BasicRoomInfo;
    int                         nXiTongZuoZhuang;       //0：系统坐庄， 1：玩家坐庄
	int							RecordCount;
	BYTE						pData[1000];
  
};

struct MZTW_Mess_UserInfo
{
	int			lUserID;
	LONGLONG    lScore;
};
struct MZTW_Mess_World_Horn_struct
{
	char UserName[50];
	char data[200];
	int	 bType;
	int  VipLevel;
	int  IsYueKa;
	LONGLONG lScore;
	char enddata[50];
	INT	  NameID;
	MZTW_Mess_World_Horn_struct()
	{
		ZeroMemory(this, sizeof(MZTW_Mess_World_Horn_struct));
	}
};
///end of 百家乐
///
///
///游戏组件信息结构 （服务器 DLL 组件获取）
struct ServerDllInfoStruct
{
    ///游戏组件信息
    UINT                        uServiceVer;                        ///服务版本
    UINT                        uNameID;                            ///游戏名字 ID
    UINT                        uSupportType;                       ///支持类型
    UINT                        uDeskPeople;                        ///游戏人数
    char                        szGameName[61];                     ///游戏名字
    char                        szGameTable[61];                    ///数据库表名字
    char                        szDLLFileName[61];                  ///文件名字

    ///辅助信息
    char                        szWriter[61];                       ///软件开发者
    char                        szDllNote[51];                      ///备注信息
};

///公共组件加载信息结构   （服务器设置数据）
struct ManageInfoStruct
{
    int                         iRoomFlag[2];                       // 不使用的字段，占位用

    ///游戏信息
    UINT                        uNameID;                            ///游戏名字 ID
    char                        szGameName[61];                     ///游戏名字
    UINT                        dwRoomRule;                         ///游戏设置规则
    UINT                        dwUserPower;                        ///游戏附加权限
    char                        szLockTable[31];                    ///锁定表名字
    char                        szIPRuleTable[31];                  ///IP 限制表名字
    char                        szNameRuleTable[31];                ///UserName限制表名字

    int                         iRoomInfoFlag[2];                   // 不使用的字段，占位用

    ///设置信息
    UINT                        uComType;                           ///游戏类型
    UINT                        uDeskType;                          ///游戏桌类型，主要为添加百家乐桌
    UINT                        uBasePoint;                         ///游戏基础分
    UINT                        uLessPoint;                         ///游戏的最少分数
    UINT                        uMaxPoint;                          ///游戏的最大分数
    UINT                        uMaxLimite;                         ///最大封顶值
    UINT                        uRunPublish;                        ///逃跑扣分
    UINT                        uTax;                               ///税收比例
    UINT                        uListenPort;                        ///监听端口
    UINT                        uMaxPeople;                         ///最大连接数目
    int                         iSocketSecretKey;                   ///socket加密的密钥

    //比赛专用
    int                                 iGameID;
    int                                 iMatchID;
    int                                 iContestID;
    int                                 iRankNum;//排名数量
    int                                 *pAwards;//比赛奖励
    int                                 *pAwardTypes;//比赛奖励类型
    int                                 iLowCount;
    __int64                             i64Chip;
    __int64                             i64LowChip;
    __int64                             i64TimeStart;
    __int64                             i64TimeEnd;
    int                                 iTimeout;
    int                                 iLeasePeople;                   ///比赛人数少于此值，代表结束比赛
    int                                 iUpPeople;                      ///比赛人数达到此值，才开始分配桌子，并设置房间状态为开始游戏
    int                                 iRoomState;

    //租赁房间ID与时间
    int                                 iLeaseID;
    __int64                             i64LeaseTimeStart;
    __int64                             i64LeaseTimeEnd;


    //每局超过时间是否赠送金币
    int                                 iTimeLength;
    __int64                             iSendCount;

    ///
    ///Vip登陆功能
    UINT uVipReseverCount;///Vip保留登陆数目
    ///

    bool                        bVIPRoom;                            ///< 金葫芦二代，是否VIP房间（VIP房间设置密码）

    UINT                        uDeskCount;                         ///游戏桌数目
    char                        szGameTable[31];                    ///信息表名字
    UINT                        uStopLogon;                         ///停止登陆房间

    int                     uVirtualUser;
    int                     uVirtualGameTime;


    ///混战场房间ID       --begin
    UINT                        uBattleRoomID;
    ///混战场房间信息表
    char                        szBattleGameTable[31];

    ///-------------------  end

    char            szRoomPassWord[61];                     //房间密码  房间加密功能
    ///服务器信息
    UINT                        uRoomID;                            ///房间ID
    char                        szGameRoomName[61];                 ///服务器名字
	wchar_t                     GameName[61];                 ///服务器名字
    ///中心数据库信息
    char                        szSQLIP[16];                        ///中心数据库地址
    char                        szSQLName[61];                      ///中心数据库用户名字
    char                        szSQLPass[61];                      ///中心数据库密码
    char                        szSQLDataName[61];                  ///中心数据库名字
    SHORT                       nPort;

    ///登陆数据库信息
    char                        szLogonSQLIP[16];                   ///登陆数据库地址
    char                        szLogonSQLName[61];                 ///登陆数据库用户名字
    char                        szLogonSQLPass[61];                 ///登陆数据库密码
    char                        szLogonSQLDataName[61];             ///登陆数据库名字
    SHORT                       logonPort;

    ///本地数据库信息
    char                        szNativeSQLIP[61];                  ///本地数据库地址
    char                        szNativeSQLName[61];                ///本地数据库用户名字
    char                        szNativeSQLPass[61];                ///本地数据库密码
    char                        szNativeSQLDataName[61];            ///本地数据库名字
    SHORT                       NativePort;

    ManageInfoStruct()
    {
        //iSocketSecretKey = SECRET_KEY;
    }
};

///内核数据 （服务器 DLL 组件启动设置）
struct KernelInfoStruct
{
    ///游戏数据
    UINT                        uNameID;                            ///游戏名字 ID 号码
    UINT                        uDeskPeople;                        ///游戏人数

    UINT                        uMinDeskPeople;                     ///部分游戏支持M-N个人游戏

    ///内核数据
    BYTE                        bMaxVer;                            ///软件最高版本
    BYTE                        bLessVer;                           ///软件最低版本
    BYTE                        bStartSQLDataBase;                  ///使用数据库连接
    BYTE                        bNativeDataBase;                    ///使用本地数据库
    BYTE                        bLogonDataBase;                     ///使用登陆数据库
    BYTE                        bStartTCPSocket;                    ///是否启动网络
    UINT                        uAcceptThreadCount;                 ///应答线程数目
    UINT                        uSocketThreadCount;                 ///SOCKET 线程数目
	BYTE                        bStartHttpDataBase;                  ///使用网页请求连接
};

/********************************************************************************************/

///列表项挂接数据类型
#define GLK_NO_DATA             0                                   ///没有数据
#define GLK_GAME_KIND           1                                   ///游戏类型
#define GLK_GAME_NAME           2                                   ///游戏名字
#define GLK_GAME_ROOM           3                                   ///游戏房间
#define GLK_GAME_USE            4                                   ///游戏使用
#define GLK_GAME_WEB            5                                   ///从INI读的WEB
#define GLK_GAME_ADD            12                                  ///联系我们

//{add by rongqiufen 2010.7.2
#define GLK_GAME_KING_WEBGAME   6                                   ///从bcf读的WEB
#define GLK_GAME_WEBGAME        7                                   ///从bcf读的WEB
#define GLK_GAME_KING_EXEGAME   8                                   ///从bcf读的WEB
#define GLK_GAME_EXEGAME        9                                   ///从bcf读的WEB
#define GLK_GAME_SELLLIST       10                                  ///销售列表
#define GLK_GAME_OPENINFO       11                                  ///开放说明
//add by rongqiufen 2010.7.2}

///游戏列表辅助结构
struct AssistantHead
{
    UINT                        uSize;                              ///数据大小
    UINT                        bDataType;                          ///类型标识
};


///游戏类型de结构
struct ComKindInfo///加入游戏类型AddTreeData
{
    AssistantHead               Head;
    UINT                        uKindID;                            ///游戏类型 ID 号码
    char                        szKindName[61];                     ///游戏类型名字

    // duanxiaohui 20111107 游戏大厅二级目录功能
    UINT                        uParentKindID;                      ///父游戏类型ID号码
    // end duanxiaohui
};

///游戏名称结构
struct ComNameInfo
{
    AssistantHead               Head;
    UINT                        uKindID;                            ///游戏类型 ID 号码
    UINT                        uNameID;                            ///游戏名称 ID 号码
    char                        szGameName[61];                     ///游戏名称
    char                        szGameProcess[61];                  ///游戏进程名
    UINT                        m_uOnLineCount;                     ///在线人数
    UINT                        uVer;                               ///版本
    ///
    ///Kylin 20090202 游戏人数初始化问题
    ComNameInfo()
    {
        m_uOnLineCount = 0;
    }
    ///

};

///游戏房间列表结构
struct ComRoomInfo
{
    AssistantHead               Head;
    UINT                        uComType;                           ///游戏类型
    UINT                        uKindID;                            ///游戏类型 ID 号码
    UINT                        uNameID;                            ///游戏名称 ID 号码
    UINT                        uRoomID;                            ///游戏房间 ID 号码
    UINT                        uPeopleCount;                       ///游戏在线人数
    UINT                        iUpPeople;                          ///比赛房间用户达到值后才开赛
    UINT                        uDeskPeople;                        ///每桌游戏人数
    UINT                        uDeskCount;                         ///游戏大厅桌子数目
    UINT                        uServicePort;                       ///大厅服务端口
    char                        szServiceIP[25];                    ///服务器 IP 地址
    char                        szGameRoomName[61];                 ///游戏房间名称
    int                         uVirtualUser;                       ///20081211 , Fred Huang
    int                         uVirtualGameTime;                   ///20081211 , Fred Huang
    UINT                        uVer;                               ///版本
    UINT                        dwRoomRule;                         ///游戏房间规则
    bool                        bVIPRoom;                           ///< 金葫芦二代，是否VIP房间（VIP房间设置密码）
    int                         iBasePoint;                         //基础倍数
    UINT                        uLessPoint;                         //金币或积分下限
    UINT                        uMaxPoint;                          //金币或积分上限
    UINT                        uRoomTypeID;                        //房间一级分类（初级场0、高级场1、富豪场2）
    UINT                        uRoomTypeSecondID;                  //房间二级分类（主要是用来给一级分类的房间排序的，如初级场1号房0，初级场2号房1，3号房2等）

    int                                 iContestID;
    __int64                             i64TimeStart;
    __int64                             i64TimeEnd;

    //租赁房间ID与时间
    int                                 iLeaseID;
    __int64                             i64LeaseTimeStart;
    __int64                             i64LeaseTimeEnd;
    ///wushuqun 2009.6.5
    ///混战场房间ID       --begin
    UINT                        uBattleRoomID;
    ///混战场房间信息表
    char                        szBattleGameTable[31];
    bool            bHasPassword;           // 有无密码 Add by lxl 2010-10-08
    UINT            dwTax;                  // 房间房费 Add by wyl 11-5-16
    UINT                        uUserDeskCount;                     //使用的桌子号
    ///
    ///Kylin 20090202 游戏人数初始化问题
	bool operator <(const ComRoomInfo& info) const
	{
		return (this->uRoomID < info.uRoomID);
	}

    ComRoomInfo()
    {
        uUserDeskCount = 0;
        uPeopleCount = 0;
        bHasPassword = false;
    }
    ///
};

///游戏大厅辅助结构
struct GameInsideInfo
{
    AssistantHead               Head;                               ///列表头
    UINT                        uTrunID;                            ///转向 ID
    char                        szDisplayName[61];                  ///显示名字
};

/********************************************************************************************/

///用户状态定义
#define USER_NO_STATE           0                                   ///没有状态，不可以访问
#define USER_LOOK_STATE         1                                   ///进入了大厅没有坐下
#define USER_SITTING            2                                   ///坐在游戏桌
#define USER_ARGEE              3                                   ///同意状态
#define USER_WATCH_GAME         4                                   ///旁观游戏
#define USER_DESK_AGREE         5                                   ///大厅同意
#define USER_CUT_GAME           20                                  ///断线状态         （游戏中状态）
#define USER_PLAY_GAME          21                                  ///游戏进行中状态    （游戏中状态）




///用户信息结构
struct UserInfoStruct
{
    int                         dwUserID;                           ///ID 号码
    int                         dwExperience;                       ///经验值
    int                         dwAccID;                            ///ACC 号码
    int                         dwPoint;                            ///分数
    __int64                     i64Money;                           ///金币
    __int64                     i64Bank;                            ///银行
    UINT                        uWinCount;                          ///胜利数目
    UINT                        uLostCount;                         ///输数目
    UINT                        uCutCount;                          ///强退数目
    UINT                        uMidCount;                          ///和局数目
    char                        szName[61];                         ///登录名
  
    UINT                        bLogoID;                            ///头像 ID 号码
    UINT                        bDeskNO;                            ///游戏桌号
    BYTE                        bDeskStation;                       ///桌子位置
    BYTE                        bUserState;                         ///用户状态
    BYTE                        bMember;                            ///会员等级
    BYTE                        bGameMaster;                        ///管理等级
    ULONG                       dwUserIP;                           ///登录IP地址
    bool                        bBoy;                               ///性别
    char                        nickName[100];                      ///用户昵称
    UINT                        uDeskBasePoint;                     ///设置的桌子倍数
    int                         dwFascination;                      ///魅力
    int                         iVipTime;                           ///会员时间
    int                         iDoublePointTime;                   ///双倍积分时间
    int                         iProtectTime;                       ///护身符时间，保留
    int                         isVirtual;                          ///是否是扩展机器人 ///20081211 , Fred Huang
    UINT                        dwTax;                               ///房费    add by wyl    11-5-16



   

    ///3 ,电视比赛VIP玩家
    int                         userType;


    UINT                         userInfoEx1;    ///扩展字段1，用于邮游钻石身份作用时间，由黄远松添加
    UINT                         userInfoEx2;    ///扩展字段2，用于GM处理之禁言时效，由zxd添加于20100805

    int                          bDisAble;        //用于判断是不是银商  add by lxl 2010-12-10,挪用为封号功能


    ///比赛专用
    int                         iContestID;   //比赛ID，唯一的标识一场比赛
    __int64                     i64ContestScore;
    int                         iContestCount;

    int                         iRankNum;       //排行名次
	UINT                        iEndTime;		//淘汰赛本局完成时间
    int                         iAward;         //比赛获取的奖励
    int                         iAwardType;     //比赛获取的奖励类型

	//添加个人房卡数量
	int							iRoomKeyNum;	//个人房卡
	int							PlayerID;
	int							JiuJiTimes;
	__int64						TotalScore;
	int							PlayerLevel;
	int							Exper;
	__int64						KuoZhan;
	int							VIPLevel;
	int							IsYueKa;
	int							YueKatime;
	int							UserChouShui;
	
    UserInfoStruct()
    {
        ZeroMemory(this, sizeof(UserInfoStruct));
    }
};



///用户规则结构
struct UserRuleStruct
{
    ///变量定义
    BYTE                        bSameIP;                            ///相同 IP
    BYTE                        bIPLimite;                          ///不于IP前几位的玩家游戏
    BYTE                        bPass;                              ///设置密码
    BYTE                        bLimitCut;                          ///限制断线
    BYTE                        bLimitPoint;                        ///限制分数
    BYTE                        bCutPercent;                        ///逃跑率
    char                        szPass[61];                         ///桌子密码
    int                         dwLowPoint;                         ///最低分数
    int                         dwHighPoint;                        ///最高分数
};


//自动赠送添加的结构体--2013-01-28 wangzhitu
typedef struct tag_RECEIVEMONEY
{
    bool      bISOpen;
    //bool      bISAuto;
    __int64   i64Money;
    __int64   i64MinMoney;
    int       iCount;
    int       iTotal;
    int       iTime;
    int       iResultCode;
    int       iLessPoint;
    struct tag_RECEIVEMONEY()
    {
        bISOpen = false;
        //bISAuto = false;
        i64Money = 0;
        i64MinMoney = 0;
        iCount = 0;
        iTotal = 0;
        iTime  = 0;
        iResultCode = 0;
        iLessPoint  = 0;
    }

} RECEIVEMONEY;




/********************************************************************************************/


struct TLoginGameInfo
{
    int                 iKindID;
    int                 iGameID;
    int                 iLoginCount;
    COleDateTime        oelLastTime;
};

struct StcokOpertion
{
	__int64				MinStock;
	__int64				MaxStock;
	int					Rat;
};
struct DeskAgentID 
{
	int					iDeskId;
	int					iAgentId;
};



enum Contol_Type
{
    Control_None = 0,			//不控制
    Control_Auto = 1,			//实时控制自动
	Control_Manual = 2,			//实时控制手动
	Control_Single666 = 3,		//单控666//
	Control_Single101 = 4,		//单控101//
	Control_Single = 5,			//单控
	Control_Stock = 6,			//房间库存控制
	Control_StockPersonal = 7,	//个人库存控制
};

//时时控制
struct ShiShiControl
{
    ShiShiControl()
    {
        ClearData();
    }
    void ClearData()
    {
        memset(this, 0, sizeof(ShiShiControl));
    }
	/*------------------加上新控制---------------------*/
	int                 nType;                      //Contol_Type，0不控制，1自动控制，2手动控制
	int                 nSystemWin;                 //0不做控制，1庄输，2庄不输，3系统输，4系统不输
	int                 nControlNum;                //控制局数,手动控制这里填1局
	int                 nContrlDesk;                //自动控制时，控制的桌子号
	int                 nControlArea;               //控制开奖区域,每个游戏不一样，
                                                    //百人牛牛：(按位与0x01，0x02，0x04，0x08对应天，地，玄，黄)
};


//强制666信息
struct Control666Info
{
	Control666Info()
	{
		ClearData();
	}
	void ClearData()
	{
		nUserid = 0;
		CtlCurPoint = 0;
		CtlGoalPoint = 0;
		CtlLevel = 0;
	}
	int					nUserid;
	__int64				CtlCurPoint;				//当前分数
	__int64				CtlGoalPoint;				//目标分数
	int                 CtlLevel;					//控制等级
};                                                       


//新版控制房间控制
struct RoomStockControl
{
	RoomStockControl()
	{
		memset(this, 0, sizeof(RoomStockControl));
	}
	void SetMaxValue()
	{

	}

	int			nRoomID; 
	int			nEnable;
	int			nA_area;	//0-A	1
	int			nA_probability;
	int			nB_area;	//A-B	2
	int			nB_probability;
	int			nC_area;	//B-C	3
	int			nC_probability;
	int			nD_area;	//C-D	4
	int			nD_probability;
	int			nE_area;	//D-E	5
	int			nE_probability;
	int			nF_area;	//E-F	6
	int			nF_probability;
	int			nG_area;	//F-G	7
	int			nG_probability;
	int			nH_area;	//G-H	8
	int			nH_probability;
	int			nI_area;	//H-I	9
	int			nI_probability;
	int			nJ_area;	//I-J	10
	int			nJ_probability;

	int			nDark_Tax;


};

/*-------------后台专用结构体-------------------------------------*/
#define  TEST_WEB_MSG			1
#define  MAX_PLAYER_180			180

//玩家坐下
struct MSG_GR_WEB_UserSit//160-25
{
	MSG_GR_WEB_UserSit()
	{
		memset(this, 0, sizeof(MSG_GR_WEB_UserSit));
	}
	int			nDeskID;
	int			nUserID;
	bool		bVirtual;
	BYTE		nChiarID;
};
struct UserListReq//181-1
{
	int			nDeskID;
};
struct UserListAck//181-2
{
	int			nDeskID;
	int			nUserID[MAX_PLAYER_180];
	bool		bVirtual[MAX_PLAYER_180];
	int			nUserCount;
};
struct DeskGameScene//181-3
{
	int			nDeskID;
	BYTE        nGameState;			//游戏状态
	BYTE        nTimeLeave;			//该状态剩余时间
};
struct RecordReq//181-102
{
	int			nDeskID;
};
struct RecordAck//181-103
{
	int			nDeskID;
	int			nRecordCount;
};
#pragma pack()

#endif
