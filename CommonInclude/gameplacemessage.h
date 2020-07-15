#pragma once

#ifndef AFC_GAME_PLACE_MESSAGE_HEAD_FILE
#define AFC_GAME_PLACE_MESSAGE_HEAD_FILE
/********************************************************************************************/
/// 网络通信数据包定义
/********************************************************************************************/
///版本定义
#define GAME_PLACE_MAX_VER              4                       ///游戏大厅最新版本
#define GAME_PLACE_LESS_VER             4                       ///游戏大厅最低版本

#define MAX_GAME_ROOM		             150                       ///游戏最大房间

#pragma pack(1)
//用户注册
struct MSG_GP_S_Register
{
    BYTE                                byFromPhone;            //0-从PC注册，1-从移动端注册
    BYTE                                byFastRegister;         //0-快速注册，1-普通注册
    char                                szHardID[64];           //硬件ID
    char                                szName[64];             //用户名
    char                                szPswd[64];             //密码
    UINT                                LogonTimes;             //登陆次数
	int									dwUserID;
	char                                UserCheckPhone[16];             //手机验证码
	char                                PhoneNum[16];             //手机号
	int									AgencyID;
	char								Answer[100];
	int									AgentID;
	char								szIP[20];							//ip
	char								qudao[50];							//渠道号
	char								check[800];						//校验码
	char								Wechat[100];
	int									registerid;						//
	MSG_GP_S_Register()
    {
        memset(this, 0, sizeof(MSG_GP_S_Register));
    }
};
////获取配置
struct MSG_GP_S_GET_CONFIG
{
	int       id;
	char      val[150];
	int		  AgentID;
};
struct MSG_GP_S_GET_CONFIGTOCLINET
{
	int       id;
	char      val[150];
};

////获取配置
struct MSG_GP_S_GET_RAT
{
	INT										AgentID;
	INT										NameID;
	INT										Rat;
};


struct MSG_GP_S_GET_AGENTWECHAT
{
	char       WeChat[50];
};
struct MSG_GP_S_GET_CASH_LOG
{
	int								UserID;
	int								bType;
	char							OrderNum[50];
	char							BankNo[50];
	LONGLONG						lScore;
	int								Operate;
	int								CollectTime;
};
struct MSG_GP_S_GET_CASH_LOG_ETBET
{
	int								UserID;
	int								bType;
	char							MeEBAT[100];
	char							OtherEBAT[100];
	LONGLONG						lScore;
	LONGLONG						Ebat;
	int								CollectTime;
};

struct MSG_GP_S_ChPassword
{
	int								UserID;
	char							szHardID[24];
	char							UserCheckPhone[16];
	char							szMD5NewPass[80];
	int								PassType;
	char							PhoneNum[64];
	int								AgentID;

};

//////////////////////////////////////////////////////////////////

struct MSG_GP_WEB_ADD_SCORE
{
	int								UserID;
	int								ret;
};
struct MSG_GP_WEB_MAIL
{
	int								UserID;
};
struct MSG_GP_WEB_BROAD
{
	int								ID;
};
struct MSG_GP_WEB_INFO
{
	int								UserID;
	char                            info[1024];
};
struct MSG_GP_WEB_ALL_USER_INFO
{
	INT								bType;
	char                            info[1024];
};
struct MSG_GP_WEB_NEEDLEVEL
{
	int								PassTime;
};

///用户登陆（帐号）结构
struct MSG_GP_S_LogonByNameStruct
{
    UINT                                uRoomVer;                           ///大厅版本
    char                                szName[64];                         ///登陆名字
    char                                TML_SN[128];
    char                                szMD5Pass[52];                      ///登陆密码
    char                                szMathineCode[64];                  ///本机机器码 锁定机器
    char                                szCPUID[24];                        //CPU的ID
    char                                szHardID[24];                       //硬盘的ID
    char                                szIDcardNo[64];                     //证件号
    char                                szMobileVCode[8];                   //手机验证码
    int                                 gsqPs;
    int                                 iUserID;                            //用户ID登录，如果ID>0用ID登录
	char                                szdevice_info[50];                     //机器唯一标志吗
	int									szdevice_type;						//平台类型
	int									AgentID;
	char								szIP[20];							//ip
};
struct LogonInterval
{
	CString cKeySocketUserName;
	LONGLONG nTimeLogonReq;
	LONGLONG nTimeLogonAck;
	LONGLONG nTimeLogonSqlReq;
	LONGLONG nTimeLogonSqlAck;
};
struct LogonConnection
{
	CString cName;
	UINT nIndex;
	UINT nHandleCode;
};
struct LogonStatistics
{
	int nDay1;
	int nHour1;
	int nMinute10;
	int nLogonDay1;
	int nLogonHour1;
	int nLogonMinute10;
	LogonStatistics()
	{
		::memset(this, 0, sizeof(LogonStatistics));
		nDay1 = -1;
	}
};


///大厅登陆返回数据包
struct MSG_GP_R_LogonResult
{
    int                                 dwUserID;                           ///用户 ID
    int                                 dwGamePower;                        ///游戏权限
    int                                 dwMasterPower;                      ///管理权限
    int                                 dwMobile;                           ///手机号码
    int                                 dwAccID;                            ///Acc 号码
    ULONG                               dwLastLogonIP;                      ///上次登陆 IP
    ULONG                               dwNowLogonIP;                       ///现在登陆 IP
    UINT                                bLogoID;                            ///用户头像
    bool                                bBoy;                               ///性别
    char                                szName[61];                         ///用户登录名
    char                                Wechat[128];                        ///数字签名
    char                                szMD5Pass[50];                      ///用户密码
    //char                              nickName[32];                       ///用户昵称
    char                                nickName[100];                      ///用户昵称
    __int64                             i64Money;                           ///用户金币
    __int64                             i64Bank;                            ///用户财富
    DOUBLE                               iLotteries;                         ///奖券
    int                                 dwFascination;                      ///魅力
    int                                 dwDiamond;                          ///钻石
    int                                 iRoomKey;                           ///房卡数量

    char                                szRealName[50];                     ///真实姓名
    char                                szIDCardNo[36];                     ///证件号
    char                                szMobileNo[50];                     ///移动电话
    char                                szQQNum[20];                        ///QQ号码
    char                                szAdrNation[50];                    ///玩家的国藉
    char                                szAdrProvince[50];                  ///玩家所在的省份
    char                                szAdrCity[50];                      ///玩家所在的城市
    char                                szZipCode[10];                      ///邮政编码

    //end JianGK 20111107
    int                                 dwTimeIsMoney;                      ///上次登陆时长所换取的金币
    int                                 iVipTime;                           ///
    int                                 iDoublePointTime;                   ///双倍积分时间
    int                                 iProtectTime;                       ///护身符时间，保留
    bool                                bLoginBulletin;                     ///是否有登录公告，Fred Huang,2008-05-20
    int                                 iLockMathine;                       ///当前帐号是否锁定了某台机器，1为锁定，0为未锁定 zxj 2009-11-13
    int                                 iBindMobile;                        ///当前帐号是否绑定手机号码，1为绑定，0为未绑定 jianguankun 2012.10.10

    int                                 iAddFriendType;                     ///是否允许任何人加为好友

    char                                szAgentCode[20];                     ///代理验证码

    int                                 nNameID;                            /// nameid
    int                                 nRoomID;                            /// roomid
    int                                 nDeskIndex;                         /// deskindex
    char                                szIP[50];                           /// ip
    int                                 nPort;                              /// port
    char                                szPwd[60];                          /// 密码
	int									VIPLevel;
	int									IsYueKa;
	int									YueKatime;
	int									IsGetYkScore;
	int									IsHaveBankPass;
	int									AgencyLevel;
	char                                BankNo[20];                     ///代理验证码
	char                                Alipay[50];                     ///代理验证码
	int									Exper;
	char								MachineCode[64];
	char								EBAT[100];
	INT									AgentID;
	INT									IsCommonUser;
	INT									UserChouShui;
};

//Added by JianGuankun 20111108,用户信息结构及修改信息的数据包定义
typedef MSG_GP_R_LogonResult MSG_GP_UserInfo;

struct MSG_GP_S_UserInfo
    : public MSG_GP_UserInfo
{
    MSG_GP_S_UserInfo()
    {
        ::memset(this, 0, sizeof(MSG_GP_S_UserInfo));
    }
};




///获取代理游戏列表
struct MSG_GP_SR_GetAgentGameList
{
	int                                AgentID;                            ///类型 ID
};
///获取游戏房间数据包
struct MSG_GP_SR_GetRoomStruct
{
    UINT                                uKindID;                            ///类型 ID
    UINT                                uNameID;                            ///名字 ID
};


///获取游戏房间数据包
struct MSG_GP_SR_OnLineStruct
{
    ///UINT                             uType;                          ///类型 ID
    ///UINT                             uID;                            ///名字 ID
    UINT                                uKindID;                            ///类型 ID
    UINT                                uNameID;                            ///名字 ID
    UINT                                uOnLineCount;                       ///在线人数
};




///////////////////代理系统
//获取代理总数
struct MSG_GP_GET_TOTAL
{
	int         UserID;				//用户ID
};
struct MSG_GP_GET_DETAIL
{
	int         UserID;				//用户ID
};
struct MSG_GP_GET_ACHIEVE
{
	int         UserID;				//用户ID
};
struct MSG_GP_GET_REWARD
{
	int         UserID;				//用户ID
};
struct MSG_GP_GET_PROVISIONS
{
	int         UserID;				//用户ID
	LONGLONG    lScore;
};
struct MSG_GP_GET_PARTNERINFO
{
	int         UserID;				//用户ID
};
struct MSG_GP_GET_PARTNERSCORE
{
	int         UserID;				//用户ID
	LONGLONG    lScore;
};
struct MSG_GP_STORE_PARTNERSCORE
{
	int         UserID;				//用户ID
	LONGLONG    lScore;
};
struct MSG_GP_GET_PARTNERUSERINFO
{
	int         UserID;				//用户ID.
	int			Day;
};
struct MSG_GP_GET_PARTNERUSERINFODETAIL
{
	int         UserID;				//用户ID
	int         DownUserID;				//用户ID
	int			Day;
};
struct MSG_GP_OPEN_PARTNER
{
	int         UserID;				//用户ID
	int         Rat;				
	int			bType;
	LONGLONG	PartnerMaxWin;
};
struct MSG_GP_UPDATE_PARTNERTIME
{
	int         UserID;				//用户ID
	int         StartTime1;				
	int			EndTime1;
	int         StartTime2;				
	int			EndTime2;
	int         StartTime3;				
	int			EndTime3;
};
//读配置
struct MSG_GP_GET_CONFIG
{
	int         ID;				//用户ID
	int         AgentID;
};
struct MSG_GP_GET_RAT
{
	int         NameID;				//用户ID
	int         AgentID;
};
struct MSG_GP_GET_CHARGE_URL
{
	int         UserID;				//用户ID
};
struct MSG_GP_GET_TOTALFORB
{
	int         UserID;				//用户ID
};
struct MSG_GP_GET_DETAILFORB
{
	int         UserID;				//用户ID
	int         Day;				//用户ID
};
struct MSG_GP_GET_AGENCYREWARDFORB
{
	int         UserID;				//用户ID
};
struct MSG_GP_GET_AGENCYREWARDLOGFORB
{
	int         UserID;				//用户ID
};
////////////////////////////////////////////////////////////////
 //绑定支付宝，银行卡
struct MSG_GP_UPDATE_ALIPAY
{
	int						UserID;
	char					RealName[50];  //真实姓名
	char					Alipay[50];  //真实姓名
	int						bType;         //类型
	int						BankType;
};
struct MSG_GP_GET_CASH_ACQUIRE
{
	int						UserID;
};
struct MSG_GP_GET_CASH
{
	int						UserID;
	int						bType;         //类型
	LONGLONG				lScore;		   //金额
	char					address[100];	//地址
};
struct MSG_GP_GET_CASH_LOG
{
	int						UserID;
	int						bType;         //类型
};

struct MSG_GP_GET_RECHARGE_LOG
{
	int						nUserID;
};


///登陆服务器登陆信息
struct DL_GP_RoomListPeoCountStruct
{
    UINT                uID;                            ///用户 ID
    UINT                uOnLineCount;                   ///在线人数
    int                 uVirtualUser;                   ///扩展机器人人数///20081211 , Fred Huang
};




struct MSG_GP_C_GET_USERINFO
{
    unsigned int    dwUserID;           //用户ID
};

struct MSG_GP_S_GET_USERINFO
{
    unsigned int    dwUserID;           //用户ID
    __int64         i64WalletMoney;     //身上的钱
    __int64         i64BankMoney;       //银行的钱
    int             dwFascination;      //魅力
    int             dwDiamond;          //钻石
    char            szNickName[61];     //昵称
    int             dwLogoID;           //头像ID
    bool            bBoy;               //性别
    char            szSignDescr[128];   //个性签名
    int             iRoomKey;           //房卡数量
	DOUBLE			iLotteries;       //礼券
};

struct MSG_GP_C_FIND_PWD
{
    char        szName[64];             //用户名
    char        szMobileNo[50];         //手机号码
};

struct MSG_GP_S_FIND_PWD
{
    int         iError;                 //0成功,1账号不存在,2没有绑定手机号,3 4手机号码不对
};



//开始查询
struct  MSG_GP_C_USE_ROOM_KEYINFO
{
    int iUserID;                                //玩家id
};


struct MSG_GP_C_DESK_LOCK_PASS			//增加用户ID号，在AA制房卡的房间检测是否拥有足够的房卡进入
{
	int				nUserID;
	char            szLockPass[61];     ///< 上锁密码
	MSG_GP_C_DESK_LOCK_PASS()
	{
		nUserID = 0;
		memset(szLockPass, 0, sizeof(szLockPass));
	}
};

struct MSG_GP_S_DESK_LOCK_PASS
{
    int             iNameID;            //游戏ID
    int             iDeskID;            //桌子ID
    int             uServicePort;       //大厅服务端口
    char            szServiceIP[25];    //服务器 IP 地址
	bool			bAAFanKa;			//是否AA制房卡
};





struct MSG_GP_S_ADDQUESTION
{
	int		ret;
};
struct MSG_GP_S_QUESTIONLIST
{
	int				UserID;
	int				QuestionID;
	char			Question[250];
	char			Answer[250];
	int			IsUse;
	int			CreateDate;				//创建时间
	int			OperateDate;				//操作时间
};



struct MSG_GP_S_PLAYERSCORE
{
	INT				UserID;
	LONGLONG		Score;
	INT				ret;
};
struct MSG_GP_S_TRANSFERINFO
{
	INT				UserID;
	char            NickName[50];     //类型
	int             LogoID;
	INT				ret;
};
struct MSG_GP_S_BANKREALTIME
{
	INT				UserID;
	LONGLONG		LastGain;
	LONGLONG		TotalGain;
	LONGLONG		Rate;
	LONGLONG		PerGain;
	LONGLONG		UnSecondReach;
};
struct MSG_GP_S_JIUJI
{
	int									UserID;
	int									JiuJiTimes;
	LONGLONG                            Score;                            //用户分数
	INT									ret;
};


//匹配房间,最少需要房卡数
struct MSG_GP_MATCH_ROOM_MIN_ROOM_KEY
{
	int	iNameID;			//游戏ID
	int RoomID;
	int nMinRoomKey;		//最少需要房卡数
	MSG_GP_MATCH_ROOM_MIN_ROOM_KEY()
	{
		iNameID = 0;
		nMinRoomKey = 0;
	}
};




//上一个游戏
struct MSG_LAST_GAME_INFO
{
	int iUserID;
    int iNameID;
    int iRoomID;
    int iDeskID;
    char szPwd[61];
};




struct MSG_GET_NEED_LEVEL
{
	int				levelid;
	LONGLONG		needexper;
	int				Rat;
	char			LevelName[50];
};

struct MSG_GET_ACHIEVE
{
	LONGLONG    DirectAchieve;
	LONGLONG    TeamAchieve;
	int			ID;
};
struct MSG_GET_REWARD
{
	LONGLONG    WeekDirectAchieve;
	LONGLONG    WeekTeamAchieve;
	LONGLONG    WeekRewrd;
	LONGLONG	PayTotal;
	LONGLONG	DepositTotal;
	INT			Flag;
	CHAR		RewardDate[50];
};

struct MSG_GET_AGENCY_LEVEL
{
	MSG_GET_AGENCY_LEVEL()
	{
		nUserID = 0;
		nLevel = 0;
	}
	int		nUserID;
	int		nLevel;
};

struct MSG_GET_RET_SUBUSER
{
	MSG_GET_RET_SUBUSER()
	{
		ZeroMemory(this, sizeof(MSG_GET_RET_SUBUSER));
	}
	int			nUserID;
	LONGLONG	PayTotal;
	LONGLONG	DepositTotal;
	LONGLONG	nTotalReward;
	CHAR		SearchDate[50];
};

struct MSG_GP_S_GET_PROVISIONS
{
	int    ret;
	LONGLONG Score;
};
struct MSG_GP_S_GET_PARTNERINFO
{
	LONGLONG        PartnerScore;
	LONGLONG        PartnerIncome;
	INT				PartnerRat;
	INT				IsPartner;
	INT				UserCount;
	INT				StopTime;
	INT				StartTime1;
	INT				EndTime1;
	INT				StartTime2;
	INT				EndTime2;
	INT				StartTime3;
	INT				EndTime3;
	INT				TempStartTime1;
	INT				TempEndTime1;
	INT				TempStartTime2;
	INT				TempEndTime2;
	INT				TempStartTime3;
	INT				TempEndTime3;
	LONGLONG        PartnerMaxWin;
};
struct MSG_GP_S_GET_PARTNERSCORE
{
	LONGLONG        PartnerScore;
	LONGLONG        BankScore;
	int				ret;
};
struct MSG_GP_S_STORE_PARTNERSCORE
{

	LONGLONG        PartnerScore;
	LONGLONG        BankScore;
	int				ret;
};
struct MSG_GP_S_OPEN_PARTNER
{
	int				ret;
	int             UserID;
};
struct MSG_GP_S_UPDATE_PARTNERTIME
{
	int				ret;
	int             UserID;
};
struct MSG_GP_S_GET_PARTNERUSERINFO
{
	int				UserID;
	char			NickName[50];
	LONGLONG		SumScore;
};
struct MSG_GP_S_GET_PARTNERUSERINFODETAIL
{
	int				RoomID;
	INT				CreateDate;
	LONGLONG		Score;
	INT				PartnerRat;
};


struct MSG_GP_S_GET_TOTALFORB
{
	LONGLONG   LastReward;
	LONGLONG   DirectRevenue;
	LONGLONG   TeamRevenue;
	LONGLONG   TotalReward;
	LONGLONG   AgencyReward;
	INT   DirectCount;
	INT   TeamCount;
};
struct MSG_GP_S_GET_DETAILFORB
{
	CHAR      NickName[50];
	LONGLONG   DirectRevenue;
	LONGLONG   TeamRevenue;
};
struct MSG_GP_S_GET_AGENCYREWARDFORB
{
	INT ret;
};
struct MSG_GP_S_GET_AGENCYREWARDLOGFORB
{
	LONGLONG   TradeScore;
	INT   CreateDate;
};
struct MSG_GP_S_GET_AGENCYREWARDRANKFORB
{
	int				UserID;
	char			NickName[50];
	LONGLONG		Revenue;
};
struct MSG_GP_S_GET_SAGENCYREWARDRANKFORB
{
	int             AgentID;
	int				UserID;
	char			NickName[50];
	LONGLONG		Revenue;
};
struct MSG_GP_S_GET_TOTAL
{
	int				UserID;
	int				UpAgencyID;
	int				TeamCount;
	int				DirectCount;
	int				NewAdd;
	int				TodayActive;
	LONGLONG		WeekDirectAchieve;
	LONGLONG		WeekTeamAchieve;
	LONGLONG        PredictReward;
	LONGLONG        TotalReward;
	LONGLONG        AgencyReward;
	LONGLONG        UsedReward;
	LONGLONG        ProvisionsScore;
	LONGLONG        LeftProvisionsReward;
	LONGLONG        TodayProvisionsScore;
};
struct MSG_GP_S_GET_DETAIL
{
	int				UserID;
	char			NickName[50];
	int				TeamCount;
	int				DirectCount;
	LONGLONG		TeamRevenue;
	LONGLONG		DirectRevenue;
};
struct MSG_GP_S_GET_REVENUELEVEL
{
	INT				LevelID;
	int				B;
	INT				C;
	INT				D;
};

struct MSG_GP_S_GET_SUBUSER
{
	int				UserID;
	int				SearchDate;
};

struct MSG_GET_AGENTGAMELIST
{
	int				AgentID;
	char			GameList[100];
};


struct MSG_P_GET_MAILLIST
{
	int                                 UserID;                             ///用户ID
};
struct MSG_P_READ_MAIL
{
	int                                 UserID;                             ///用户ID
	int                                 MailID;                             ///用户ID
	int									bType;
};

struct MSG_P_DELETE_MAIL
{
	int                                 UserID;                             ///用户ID
	int                                 MailID;                             ///用户ID
};

struct MSG_P_GET_QUESTUONLIST
{
	int                                 UserID;                             ///用户ID
};

struct MSG_P_ADD_QUESTUON
{
	int                                 UserID;                             ///用户ID
	char								Question[250];						//问题
};

struct MSG_P_STORE_SCORE
{
	int                                 UserID;                             ///用户ID
	int									PlayerID;						//角色名
	LONGLONG							Score;							//分数
};
struct MSG_P_GET_JIUJI
{
	int                                 UserID;                             ///用户ID
};

struct MSG_P_SET_BANKPASS
{
	int                                 UserID;                             ///用户ID
	char								MD5_BANKPASS[54];					
	int									bType;
};

struct MSG_P_GET_BANKLOG
{
	int                                 UserID;                             ///用户
};
struct MSG_P_GET_BANKREALTIME
{
	int                                 UserID;                             ///用户ID
};
struct MSG_P_GET_SCORE
{
	int                                 UserID;                             ///用户ID
	int									PlayerID;						//角色名
	LONGLONG							Score;							//分数
	char								MD5_BANKPASS[54];

};
struct MSG_P_TRANSFER_INFO
{
	char								Phone[16];					//对方id
	int                                 UserID;                             ///用户ID
};
struct MSG_P_TRANSFER_SCORE
{
	int                                 UserID;                             ///用户ID
	char								Phone[16];					//对方id
	LONGLONG							Score;							//分数
	char								MD5_BANKPASS[54];
	int                                 TargetUserID;                             ///用户ID
};

struct MSG_GP_GET_DEVICE
{
	int									UserID;
	char                                szdevice_info[50];                     //机器唯一标志吗
	int									szdevice_type;						//平台类型
};

struct MSG_GP_WEB_BUY_MONEY
{
	INT       UserID;
};
//断线的玩家希望知道自己先前的房间桌子
struct MSG_GP_S_Room_CutNet_User_struct
{
	int iuserid;//用户id
	int inameid;//游戏id
	int iroomid;//房间id
	int ideskid;//桌子id
	char            szLockPass[61];     ///< 上锁密码(原始两位密码)
	MSG_GP_S_Room_CutNet_User_struct()
	{
		iuserid = 0; //用户id
		inameid = 0; //游戏id
		iroomid = 0; //房间id
		ideskid = 0; //桌子id
	}
};
#pragma pack()
/********************************************************************************************/
/// 通信标识定义
/********************************************************************************************/

//================================================================================================
#define MDM_GP_REGISTER                 99              //玩家注册
#define ASS_GP_REGISTER                 1
#define ERR_REGISTER_ERROR              0
#define ERR_REGISTER_SUCCESS            1
#define ERR_REGISTER_NAME_EXIST 2
//================================================================================================
///通信主标识
#define MDM_GP_LOGON                    100                             ///大厅登陆
///通信辅助标识
#define ASS_GP_LOGON_BY_NAME            1                                   ///通过用户名字登陆
#define ASS_GP_LOGON_BY_ACC             2                                   ///通过用户ACC 登陆
#define ASS_GP_LOGON_BY_MOBILE          3                                   ///通过用户手机登陆
#define ASS_GP_LOGON_REG                4                                   ///用户注册
#define ASS_GP_LOGON_SUCCESS            5                                   ///登陆成功
#define ASS_GP_LOGON_ERROR              6                                   ///登陆失败
#define ASS_GP_LOGON_ALLO_PART          7                                   ///异地登陆
#define ASS_GP_LOGON_LOCK_VALID         8                                   ///锁机验证

// PengJiLin, 2010-6-2, 如果是第三方启动，发此消息通知服务端。(鸥朴)
#define ASS_GP_LOGON_BY_SOFTWARE        10
#define ASS_GP_LOGON_MOBILE_VALID       11                                  ///手机验证
#define ASS_GP_LOGIN_SWITCH             12                                  //请求功能开关
#define ASS_GP_CHECK_PHONE              13                                  //手机绑定短信验证
#define ASS_GP_GET_DEVICE	            14                                  //极光获取本机id
//================================================================================================
//角色系统
#define MDM_GP_PLAYER                   150									///用户角色
#define ASS_GP_ADD_PLAYER				14                                  //用户角色增加
#define ASS_GP_SWITCH_PLAYER			15                                  //用户角色切换
#define ASS_GP_TRANS_PLAYER				16                                  //用户角色转移
#define ASS_GP_CHANGE_PNAME				17                                  //用户角色改名
#define ASS_GP_STORE_SCORE				18                                  //用户角色存分
#define ASS_GP_GET_SCORE				19                                  //用户角色取分
#define ASS_GP_LOGON_PLAYER_LIST        20                                  ///用户角色
#define ASS_GP_GET_JIUJI		        21                                  ///用户救济金
#define ASS_GP_JIUJI_TIP				22                                  ///用户救济金提示
#define ASS_GP_SET_BANKPASS				23									//设置银行密码
#define ASS_GP_GET_BANKLOG				24									//获取记录
#define ASS_GP_GET_BANKREALTIME			25									//获取实时利率
#define ASS_GP_TRANSFER_SCORE			26									//保险柜转账
#define ASS_GP_TRANSFER_INFO			27									//保险柜转账获取信息
//================================================================================================
//邮件系统
#define MDM_GP_MAIL						151									///邮件系统
#define ASS_GP_GET_MAILLIST				10                                  //获取邮件列表
#define ASS_GP_GET_GOODS			    11                                  ///领取奖品
#define ASS_GP_READ_MAIL			    12                                  ///读邮件
#define ASS_GP_DELETE_MAIL			    13                                  ///删除邮件
#define ASS_GP_GET_BROAD_MAIL			14                                  ///广播邮件

#define ASS_GP_GET_QUESTION				15									//获取问题列表
#define ASS_GP_ADD_QUESTION				16									//增加问题列表
//================================================================================================
#define MDM_RANK						152									//排行
#define ASS_GP_GET_RANKLIST				1									//获取排行
#define ASS_GP_GET_BOSSRANKLIST				2									//获取boss排行
//排行
///////////////////////////////////////////////////////////////////////////////
//任务
#define MDM_TASK						153									//任务
#define ASS_GP_GET_TASKLIST				1									//获取任务列表
#define ASS_GP_UPDATE_TASK				2									//领取任务奖励
#define ASS_GP_GET_TASKNAME				3									//获取任务名
///////////////////////////////////////////////////////////////////////////////
//vip月卡
#define MDM_VIP							154									//VIP

#define ASS_GP_GET_VIP					1									//获取任务列表
#define ASS_GP_GET_YkScore				2									//领取月卡奖励
#define ASS_GP_GET_FavCompain			3									//获取特惠活动
#define ASS_GP_GET_NewUserRewardStateList	4									//获取新手奖励列表
#define ASS_GP_GET_NewUserReward		5									//获取新手奖励
#define ASS_GP_GET_NewUserRewardList	6									//获取新手奖励列表
#define ASS_GP_GET_VipList				7									//获取vip奖励列表
#define ASS_GP_GET_UserVipReward		8									//获取vip奖励
#define ASS_GP_GET_PaoTaiList			9									//获取炮台列表
#define ASS_GP_Get_UserPaoTai			10									//获取用户炮台
#define ASS_GP_BUY_PaoTai				11									//购买炮台
///////////////////////////////////////////////////////////////////////////////
#define MDM_CONFIG							155									//CONFIG
#define ASS_GP_GET_CONFIG					1									//获取配置
#define ASS_GP_GET_GAMERECORED				2									//获取游戏记录
#define ASS_GP_GET_RAT						3									//获取游戏税收比例
#define ASS_GP_GET_CHARGE_URL				4									//获取充值url
#define ASS_GP_GET_CHARGE_URLNEW				5									//获取充值url
///////////////////////////////////////////////////////////////////////////////
#define MDM_USERSCORE						156									//用户充值相关
#define ASS_GP_GET_SHOUCHONG					1									//获取首冲
///////////////////////////////////////////////////////////////////////////////
#define MDM_FISH_MATCH						157									//捕鱼比赛
#define ASS_GP_GET_FISH_MATCH_INFO					1									//获取比赛列表
#define ASS_GP_ENROLL_FISH_MATCH					2									//报名比赛
#define ASS_GP_GET_USER_MATCH_INFO					3									//获取用户比赛信息
#define ASS_GP_GET_FISH_MATCH_RANK					4									//获取比赛排名
#define ASS_GP_GET_FISH_MATCH_REWARD				5									//获取比赛奖励列表

///////////////////////////////////////////////////////////////////////////////
#define MDM_AGENCY							158							//代理系统
#define ASS_GP_GET_TOTAL					1							//获取总人数
#define ASS_GP_GET_DETAIL					2							//获取明细
#define ASS_GP_GET_NEEDLEVEL					3							//获取经验
#define ASS_GP_GET_ACHIEVE					4							//获取业绩
#define ASS_GP_GET_REWARD					5							//获取奖励
#define ASS_GP_GET_PARTNERINFO					6							//获取合伙人信息
#define ASS_GP_GET_PARTNERSCORE					7							//资金池转入余额宝
#define ASS_GP_STORE_PARTNERSCORE					8							//余额宝转入资金池
#define ASS_GP_GET_PARTNERUSERINFO					9							//获取合伙人玩家信息
#define ASS_GP_GET_PARTNERUSERINFODETAIL					10							//获取合伙人玩家详细信息
#define ASS_GP_OPEN_PARTNER					11							//开启合伙人功能
#define ASS_GP_UPDATE_PARTNERTIME					12							//开启合伙人时间
#define ASS_GP_GET_PROVISIONS					13							//预提
#define ASS_GP_UPDATE_PARTNERTIME_TEMP					14							//更新时间
#define ASS_GP_GET_TOTAL_FORMODEB					15							//获取总人数
#define ASS_GP_GET_DETAIL_FORMODEB					16							//获取明细
#define ASS_GP_GET_AGENCYREWARD_FORMODEB					17							//提取奖励
#define ASS_GP_GET_AGENCYREWARDLOG_FORMODEB					18							//提取奖励记录
#define ASS_GP_GET_AGENCYREWARDRANK_FORMODEB				19							//奖励周排行

#define ASS_GP_GET_AGENCYREWARD_LEVEL						20					//获取当前推广等级
#define ASS_GP_GET_AGENCY_SUBUSER							21					//查询下级用户
///////////////////////////////////////////////////////////////////////////////
#define MDM_CASH							159							//提现
#define ASS_GP_UPDATE_ALIPAY				1							//绑定支付宝
#define ASS_GP_GET_AGENTWECHAT				2							//获取代理微信
#define ASS_GP_GET_CASH						3							//提现
#define ASS_GP_GET_CASH_LOG					4							//提现记录
#define ASS_GP_GET_CASH_ACQUIRE					5							//获取提现需要条件
#define ASS_GP_GET_CASH_LOG_ETBET					6							//et提现记录

#define ASS_GP_RECHARGE_LOG					7							//



//=====================================================================================
#define MDM_GP_WEB_INFO							160							//网页消息
#define ASS_GP_WEB_ADD_SCORE						1   ///充值
#define ASS_GP_WEB_MAIL						2   ///邮件
#define ASS_GP_SYSTEM_BROAD						3   ///加载公告
#define ASS_GP_SENDTO_ALL_USER						4   ///发送全局信息
#define ASS_GP_LOAD_AGENT_GAMELIST						5   ///重新加载代理列表
#define ASS_GP_LOAD_SYSTEM_LIST						6   ///重新加载配置列表
#define ASS_GP_LOAD_LUCKAWARD						7   ///重新加载轮盘配置
#define ASS_GP_LOAD_NEEDLEVEL						8   ///重新加载推广等级
#define ASS_GP_LOAD_BROAD						9   ///加载喇叭
#define ASS_GP_SEND_BUY_MONEY								12	   ///网页通知充值成功 
#define ASS_GP_RELOAD_LIST								13	   ///通知重新加载游戏列表 
#define ASS_GP_RELOAD_URL								14	   ///通知重新加载URL列表 
#define ASS_GP_RELOAD_RAT								15	   ///通知重新加载税率
//================================================================================================


///通信主标识
#define MDM_GP_LIST                     101                             ///游戏列表
///通信辅助标识
#define ASS_GP_LIST_KIND                1                                   ///获取游戏类型列表
#define ASS_GP_LIST_NAME                2                                   ///获取游戏名字列表
#define ASS_GP_LIST_ROOM                3                                   ///获取游戏房间列表
#define ASS_GP_LIST_COUNT               4                                   ///获取游戏人数列表
#define ASS_GP_ROOM_LIST_PEOCOUNT       5                                   ///获取游戏人数列表
#define ASS_GP_ROOM_PASSWORD            6                                   //发送房间密码 Add by lxl 2010-10-08，试图进入密码房间时发送此消息
#define ASS_GP_GET_SELLGAMELIST         7                                   //获取游戏销售列表

#define ASS_GP_CONTINUE_NEXT_ROUND		8									//继续下一轮游戏

//================================================================================================
///通信主标识
#define MDM_GP_MESSAGE                  103                             ///系统消息
///通信辅助标识
#define ASS_GP_NEWS_SYSMSG              1                               ///新闻和系统消息
//#define ASS_GP_DUDU                       2                               ///小喇叭
#define ASS_GP_TALK_MSG                 3                               //聊天消息
#define ASS_GP_WORLD_HORN               4                               //世界喇叭消息

//================================================================================================
///锁定机器命令主ID zxj 2009-11-12
#define MDM_GP_LOCK_ACCOUNT             106
#define ASS_LOCK_ACCOUNT                1

//================================================================================================
///魅力兑换
#define MDM_GP_CHARMEXCHANGE            109
#define ASS_GETLIST                     1
#define ASS_EXCHANGE                    2
#define ASS_GETHUAFEILIST               3
#define ASS_GETGOODLIST					4
#define ASS_GETGOODLOG					5

//================================================================================================
//{{ Added by JianGuankun 20111108 添加用户资料管理通讯协议
#define MDM_GP_USERINFO                 115     //用户资料
#define ASS_GP_USERINFO_UPDATE_BASE     1       //用户更新基本信息
#define ASS_GP_USERINFO_UPDATE_DETAIL   2       //用户更新详细信息
#define ASS_GP_USERINFO_UPDATE_PWD      3       //用户修改密码
#define ASS_GP_USERINFO_ACCEPT          5       //服务端已接受
#define ASS_GP_USERINFO_NOTACCEPT       6       //服务端未能接受
#define ASS_GP_USERINFO_NICKNAMEID      10      //根据玩家昵称找ID或ID找昵称
#define ASS_GP_USERINFO_GET             11      //获取指定用户ID的个人资料
#define ASS_GP_FIND_PWD                 12      //找回密码
#define ASS_GP_SETINVITECODE            13      //设置邀请码
#define ASS_GP_SETUPDATELOGO            14      //设置logo
#define ASS_GP_SHARE_WX_REWARD          15      //微信分享奖励



//================================================================================================
#define MDM_GP_SET_LOGO                     113
#define ASS_GP_SET_LOGO                     1

/********************************************************************************************/
///错误代码
#define ERR_GP_ERROR_UNKNOW             0                                   ///未知错误
#define ERR_GP_LOGON_SUCCESS            1                                   ///登陆成功
#define ERR_GP_USER_NO_FIND             2                                   ///登陆名字错误
#define ERR_GP_USER_PASS_ERROR          3                                   ///用户密码错误
#define ERR_GP_USER_VALIDATA            4                                   ///用户帐号禁用
#define ERR_GP_USER_IP_LIMITED          5                                   ///登陆 IP 禁止
#define ERR_GP_USER_EXIST               6                                   ///用户已经存在
#define ERR_GP_PASS_LIMITED             7                                   ///密码禁止效验
#define ERR_GP_IP_NO_ORDER              8                                   ///不是指定地址 
#define ERR_GP_LIST_PART                9                                   ///部分游戏列表
#define ERR_GP_LIST_FINISH              10                                  ///全部游戏列表
#define ERR_GP_USER_LOGON               11                                  ///此帐号已经登录
#define ERR_GP_USERNICK_EXIST           12                                  ///此昵称已经存在
#define ERR_GP_USER_BAD                 13                                  ///未法字符
#define ERR_GP_IP_FULL                  14                                  ///IP已满
#define ERR_GP_LOCK_SUCCESS             15                                  ///锁定机器成功   2009-11-12
#define ERR_GP_ACCOUNT_HAS_LOCK         16                                  ///机器已经处于锁定状态   2009-11-12
#define ERR_GP_UNLOCK_SUCCESS           17                                  ///解除锁定成功 2009-11-12
#define ERR_GP_NO_LOCK                  18                                  ///机器根本就没有锁定，所以解锁失败 2009-11-12
#define ERR_GP_CODE_DISMATCH            19                                  ///机器码不匹配，解锁失败。 2009-11-12
#define ERR_GP_ACCOUNT_LOCKED           20                                  ///本账号锁定了某台机器，登录失败 2009-11-12 zxj
#define ERR_GP_MATHINE_LOCKED           21
#define ERR_GP_VER_ERROR				22
#define ERR_GP_USER_PASS_ERROR2				23
#define ERR_GP_USER_PASS_ERROR3				24
#define ERR_GP_USER_PASS_ERROR4				25
// PengJiLin, 2010-6-7, 第三方认证返回的错误码。鸥朴
#define ERR_GP_USER_NOT_EXIST           30      // 用户不存在
#define ERR_GP_USER_OVERDATE            31      // 用户已过期
#define ERR_GP_DATABASE_FUNC_ERROR      32      // 数据库操作失败
#define ERR_GP_DATABASE_RETURN_ERROR    33      // 数据库返回执行错误结果
/********************************************************************************************/
///异常代码
//JianGuankun 20111110
#define EXP_GP_ALLO_PARTY               50                              ///本账号异地登陆


//================================================================================================
#define MDM_GP_DESK_LOCK_PASS           254                                 ////桌子密码消息
#define ASS_GP_DESK_LOCK_PASS               1                                   ///询问桌子密码是否正确(正常询问)

#define ASS_GP_GET_ROOM_KEY_USE_INFO        3                                   ///查询自己开启的桌子信息
#define ASS_GP_GET_ROOM                     4                                   ///查询自己先前所在的房间
#define ASS_GET_MIN_ROOM_KEY_NUM				8								//获取匹配房间需要最小房卡数



#define ERR_DESK_LOCK_PASS_ERROR                0//失败
#define ERR_DESK_LOCK_PASS_SUCCESS              1//成功
#define ERR_DESK_LOCK_NO_ENOUGH_ROOMKEY			2//没有足够的房卡加入(AA制房间)

#define ERR_DESK_LOCK_PASS_ERROR                0//失败
#define ERR_DESK_LOCK_PASS_SUCCESS              1//成功

#define ERR_USE_ROOM_KEY_ERROR                  0//失败
#define ERR_USE_ROOM_KEY_SUCCESS                1//成功
#define ERR_USE_ROOM_KEY_RECOME                 2//重新回到已经在玩的房间
#define ERR_USE_ROOM_KEY_EXISTS                 3 //已经登陆在数据库的其他的地方，无法创建房间

#define ERR_USE_ROOM_KEY_PART                   0//部分
#define ERR_USE_ROOM_KEY_FINISH                 1//完成

#define ERR_ROOM_CREATE_INFO_PART               0
#define ERR_ROOM_CREATE_INFO_FINISH             1





#endif
