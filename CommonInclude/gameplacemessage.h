#pragma once

#ifndef AFC_GAME_PLACE_MESSAGE_HEAD_FILE
#define AFC_GAME_PLACE_MESSAGE_HEAD_FILE
/********************************************************************************************/
/// ����ͨ�����ݰ�����
/********************************************************************************************/
///�汾����
#define GAME_PLACE_MAX_VER              4                       ///��Ϸ�������°汾
#define GAME_PLACE_LESS_VER             4                       ///��Ϸ������Ͱ汾

#define MAX_GAME_ROOM		             150                       ///��Ϸ��󷿼�

#pragma pack(1)
//�û�ע��
struct MSG_GP_S_Register
{
    BYTE                                byFromPhone;            //0-��PCע�ᣬ1-���ƶ���ע��
    BYTE                                byFastRegister;         //0-����ע�ᣬ1-��ͨע��
    char                                szHardID[64];           //Ӳ��ID
    char                                szName[64];             //�û���
    char                                szPswd[64];             //����
    UINT                                LogonTimes;             //��½����
	int									dwUserID;
	char                                UserCheckPhone[16];             //�ֻ���֤��
	char                                PhoneNum[16];             //�ֻ���
	int									AgencyID;
	char								Answer[100];
	int									AgentID;
	char								szIP[20];							//ip
	char								qudao[50];							//������
	char								check[800];						//У����
	char								Wechat[100];
	int									registerid;						//
	MSG_GP_S_Register()
    {
        memset(this, 0, sizeof(MSG_GP_S_Register));
    }
};
////��ȡ����
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

////��ȡ����
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

///�û���½���ʺţ��ṹ
struct MSG_GP_S_LogonByNameStruct
{
    UINT                                uRoomVer;                           ///�����汾
    char                                szName[64];                         ///��½����
    char                                TML_SN[128];
    char                                szMD5Pass[52];                      ///��½����
    char                                szMathineCode[64];                  ///���������� ��������
    char                                szCPUID[24];                        //CPU��ID
    char                                szHardID[24];                       //Ӳ�̵�ID
    char                                szIDcardNo[64];                     //֤����
    char                                szMobileVCode[8];                   //�ֻ���֤��
    int                                 gsqPs;
    int                                 iUserID;                            //�û�ID��¼�����ID>0��ID��¼
	char                                szdevice_info[50];                     //����Ψһ��־��
	int									szdevice_type;						//ƽ̨����
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


///������½�������ݰ�
struct MSG_GP_R_LogonResult
{
    int                                 dwUserID;                           ///�û� ID
    int                                 dwGamePower;                        ///��ϷȨ��
    int                                 dwMasterPower;                      ///����Ȩ��
    int                                 dwMobile;                           ///�ֻ�����
    int                                 dwAccID;                            ///Acc ����
    ULONG                               dwLastLogonIP;                      ///�ϴε�½ IP
    ULONG                               dwNowLogonIP;                       ///���ڵ�½ IP
    UINT                                bLogoID;                            ///�û�ͷ��
    bool                                bBoy;                               ///�Ա�
    char                                szName[61];                         ///�û���¼��
    char                                Wechat[128];                        ///����ǩ��
    char                                szMD5Pass[50];                      ///�û�����
    //char                              nickName[32];                       ///�û��ǳ�
    char                                nickName[100];                      ///�û��ǳ�
    __int64                             i64Money;                           ///�û����
    __int64                             i64Bank;                            ///�û��Ƹ�
    DOUBLE                               iLotteries;                         ///��ȯ
    int                                 dwFascination;                      ///����
    int                                 dwDiamond;                          ///��ʯ
    int                                 iRoomKey;                           ///��������

    char                                szRealName[50];                     ///��ʵ����
    char                                szIDCardNo[36];                     ///֤����
    char                                szMobileNo[50];                     ///�ƶ��绰
    char                                szQQNum[20];                        ///QQ����
    char                                szAdrNation[50];                    ///��ҵĹ���
    char                                szAdrProvince[50];                  ///������ڵ�ʡ��
    char                                szAdrCity[50];                      ///������ڵĳ���
    char                                szZipCode[10];                      ///��������

    //end JianGK 20111107
    int                                 dwTimeIsMoney;                      ///�ϴε�½ʱ������ȡ�Ľ��
    int                                 iVipTime;                           ///
    int                                 iDoublePointTime;                   ///˫������ʱ��
    int                                 iProtectTime;                       ///�����ʱ�䣬����
    bool                                bLoginBulletin;                     ///�Ƿ��е�¼���棬Fred Huang,2008-05-20
    int                                 iLockMathine;                       ///��ǰ�ʺ��Ƿ�������ĳ̨������1Ϊ������0Ϊδ���� zxj 2009-11-13
    int                                 iBindMobile;                        ///��ǰ�ʺ��Ƿ���ֻ����룬1Ϊ�󶨣�0Ϊδ�� jianguankun 2012.10.10

    int                                 iAddFriendType;                     ///�Ƿ������κ��˼�Ϊ����

    char                                szAgentCode[20];                     ///������֤��

    int                                 nNameID;                            /// nameid
    int                                 nRoomID;                            /// roomid
    int                                 nDeskIndex;                         /// deskindex
    char                                szIP[50];                           /// ip
    int                                 nPort;                              /// port
    char                                szPwd[60];                          /// ����
	int									VIPLevel;
	int									IsYueKa;
	int									YueKatime;
	int									IsGetYkScore;
	int									IsHaveBankPass;
	int									AgencyLevel;
	char                                BankNo[20];                     ///������֤��
	char                                Alipay[50];                     ///������֤��
	int									Exper;
	char								MachineCode[64];
	char								EBAT[100];
	INT									AgentID;
	INT									IsCommonUser;
	INT									UserChouShui;
};

//Added by JianGuankun 20111108,�û���Ϣ�ṹ���޸���Ϣ�����ݰ�����
typedef MSG_GP_R_LogonResult MSG_GP_UserInfo;

struct MSG_GP_S_UserInfo
    : public MSG_GP_UserInfo
{
    MSG_GP_S_UserInfo()
    {
        ::memset(this, 0, sizeof(MSG_GP_S_UserInfo));
    }
};




///��ȡ������Ϸ�б�
struct MSG_GP_SR_GetAgentGameList
{
	int                                AgentID;                            ///���� ID
};
///��ȡ��Ϸ�������ݰ�
struct MSG_GP_SR_GetRoomStruct
{
    UINT                                uKindID;                            ///���� ID
    UINT                                uNameID;                            ///���� ID
};


///��ȡ��Ϸ�������ݰ�
struct MSG_GP_SR_OnLineStruct
{
    ///UINT                             uType;                          ///���� ID
    ///UINT                             uID;                            ///���� ID
    UINT                                uKindID;                            ///���� ID
    UINT                                uNameID;                            ///���� ID
    UINT                                uOnLineCount;                       ///��������
};




///////////////////����ϵͳ
//��ȡ��������
struct MSG_GP_GET_TOTAL
{
	int         UserID;				//�û�ID
};
struct MSG_GP_GET_DETAIL
{
	int         UserID;				//�û�ID
};
struct MSG_GP_GET_ACHIEVE
{
	int         UserID;				//�û�ID
};
struct MSG_GP_GET_REWARD
{
	int         UserID;				//�û�ID
};
struct MSG_GP_GET_PROVISIONS
{
	int         UserID;				//�û�ID
	LONGLONG    lScore;
};
struct MSG_GP_GET_PARTNERINFO
{
	int         UserID;				//�û�ID
};
struct MSG_GP_GET_PARTNERSCORE
{
	int         UserID;				//�û�ID
	LONGLONG    lScore;
};
struct MSG_GP_STORE_PARTNERSCORE
{
	int         UserID;				//�û�ID
	LONGLONG    lScore;
};
struct MSG_GP_GET_PARTNERUSERINFO
{
	int         UserID;				//�û�ID.
	int			Day;
};
struct MSG_GP_GET_PARTNERUSERINFODETAIL
{
	int         UserID;				//�û�ID
	int         DownUserID;				//�û�ID
	int			Day;
};
struct MSG_GP_OPEN_PARTNER
{
	int         UserID;				//�û�ID
	int         Rat;				
	int			bType;
	LONGLONG	PartnerMaxWin;
};
struct MSG_GP_UPDATE_PARTNERTIME
{
	int         UserID;				//�û�ID
	int         StartTime1;				
	int			EndTime1;
	int         StartTime2;				
	int			EndTime2;
	int         StartTime3;				
	int			EndTime3;
};
//������
struct MSG_GP_GET_CONFIG
{
	int         ID;				//�û�ID
	int         AgentID;
};
struct MSG_GP_GET_RAT
{
	int         NameID;				//�û�ID
	int         AgentID;
};
struct MSG_GP_GET_CHARGE_URL
{
	int         UserID;				//�û�ID
};
struct MSG_GP_GET_TOTALFORB
{
	int         UserID;				//�û�ID
};
struct MSG_GP_GET_DETAILFORB
{
	int         UserID;				//�û�ID
	int         Day;				//�û�ID
};
struct MSG_GP_GET_AGENCYREWARDFORB
{
	int         UserID;				//�û�ID
};
struct MSG_GP_GET_AGENCYREWARDLOGFORB
{
	int         UserID;				//�û�ID
};
////////////////////////////////////////////////////////////////
 //��֧���������п�
struct MSG_GP_UPDATE_ALIPAY
{
	int						UserID;
	char					RealName[50];  //��ʵ����
	char					Alipay[50];  //��ʵ����
	int						bType;         //����
	int						BankType;
};
struct MSG_GP_GET_CASH_ACQUIRE
{
	int						UserID;
};
struct MSG_GP_GET_CASH
{
	int						UserID;
	int						bType;         //����
	LONGLONG				lScore;		   //���
	char					address[100];	//��ַ
};
struct MSG_GP_GET_CASH_LOG
{
	int						UserID;
	int						bType;         //����
};

struct MSG_GP_GET_RECHARGE_LOG
{
	int						nUserID;
};


///��½��������½��Ϣ
struct DL_GP_RoomListPeoCountStruct
{
    UINT                uID;                            ///�û� ID
    UINT                uOnLineCount;                   ///��������
    int                 uVirtualUser;                   ///��չ����������///20081211 , Fred Huang
};




struct MSG_GP_C_GET_USERINFO
{
    unsigned int    dwUserID;           //�û�ID
};

struct MSG_GP_S_GET_USERINFO
{
    unsigned int    dwUserID;           //�û�ID
    __int64         i64WalletMoney;     //���ϵ�Ǯ
    __int64         i64BankMoney;       //���е�Ǯ
    int             dwFascination;      //����
    int             dwDiamond;          //��ʯ
    char            szNickName[61];     //�ǳ�
    int             dwLogoID;           //ͷ��ID
    bool            bBoy;               //�Ա�
    char            szSignDescr[128];   //����ǩ��
    int             iRoomKey;           //��������
	DOUBLE			iLotteries;       //��ȯ
};

struct MSG_GP_C_FIND_PWD
{
    char        szName[64];             //�û���
    char        szMobileNo[50];         //�ֻ�����
};

struct MSG_GP_S_FIND_PWD
{
    int         iError;                 //0�ɹ�,1�˺Ų�����,2û�а��ֻ���,3 4�ֻ����벻��
};



//��ʼ��ѯ
struct  MSG_GP_C_USE_ROOM_KEYINFO
{
    int iUserID;                                //���id
};


struct MSG_GP_C_DESK_LOCK_PASS			//�����û�ID�ţ���AA�Ʒ����ķ������Ƿ�ӵ���㹻�ķ�������
{
	int				nUserID;
	char            szLockPass[61];     ///< ��������
	MSG_GP_C_DESK_LOCK_PASS()
	{
		nUserID = 0;
		memset(szLockPass, 0, sizeof(szLockPass));
	}
};

struct MSG_GP_S_DESK_LOCK_PASS
{
    int             iNameID;            //��ϷID
    int             iDeskID;            //����ID
    int             uServicePort;       //��������˿�
    char            szServiceIP[25];    //������ IP ��ַ
	bool			bAAFanKa;			//�Ƿ�AA�Ʒ���
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
	int			CreateDate;				//����ʱ��
	int			OperateDate;				//����ʱ��
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
	char            NickName[50];     //����
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
	LONGLONG                            Score;                            //�û�����
	INT									ret;
};


//ƥ�䷿��,������Ҫ������
struct MSG_GP_MATCH_ROOM_MIN_ROOM_KEY
{
	int	iNameID;			//��ϷID
	int RoomID;
	int nMinRoomKey;		//������Ҫ������
	MSG_GP_MATCH_ROOM_MIN_ROOM_KEY()
	{
		iNameID = 0;
		nMinRoomKey = 0;
	}
};




//��һ����Ϸ
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
	int                                 UserID;                             ///�û�ID
};
struct MSG_P_READ_MAIL
{
	int                                 UserID;                             ///�û�ID
	int                                 MailID;                             ///�û�ID
	int									bType;
};

struct MSG_P_DELETE_MAIL
{
	int                                 UserID;                             ///�û�ID
	int                                 MailID;                             ///�û�ID
};

struct MSG_P_GET_QUESTUONLIST
{
	int                                 UserID;                             ///�û�ID
};

struct MSG_P_ADD_QUESTUON
{
	int                                 UserID;                             ///�û�ID
	char								Question[250];						//����
};

struct MSG_P_STORE_SCORE
{
	int                                 UserID;                             ///�û�ID
	int									PlayerID;						//��ɫ��
	LONGLONG							Score;							//����
};
struct MSG_P_GET_JIUJI
{
	int                                 UserID;                             ///�û�ID
};

struct MSG_P_SET_BANKPASS
{
	int                                 UserID;                             ///�û�ID
	char								MD5_BANKPASS[54];					
	int									bType;
};

struct MSG_P_GET_BANKLOG
{
	int                                 UserID;                             ///�û�
};
struct MSG_P_GET_BANKREALTIME
{
	int                                 UserID;                             ///�û�ID
};
struct MSG_P_GET_SCORE
{
	int                                 UserID;                             ///�û�ID
	int									PlayerID;						//��ɫ��
	LONGLONG							Score;							//����
	char								MD5_BANKPASS[54];

};
struct MSG_P_TRANSFER_INFO
{
	char								Phone[16];					//�Է�id
	int                                 UserID;                             ///�û�ID
};
struct MSG_P_TRANSFER_SCORE
{
	int                                 UserID;                             ///�û�ID
	char								Phone[16];					//�Է�id
	LONGLONG							Score;							//����
	char								MD5_BANKPASS[54];
	int                                 TargetUserID;                             ///�û�ID
};

struct MSG_GP_GET_DEVICE
{
	int									UserID;
	char                                szdevice_info[50];                     //����Ψһ��־��
	int									szdevice_type;						//ƽ̨����
};

struct MSG_GP_WEB_BUY_MONEY
{
	INT       UserID;
};
//���ߵ����ϣ��֪���Լ���ǰ�ķ�������
struct MSG_GP_S_Room_CutNet_User_struct
{
	int iuserid;//�û�id
	int inameid;//��Ϸid
	int iroomid;//����id
	int ideskid;//����id
	char            szLockPass[61];     ///< ��������(ԭʼ��λ����)
	MSG_GP_S_Room_CutNet_User_struct()
	{
		iuserid = 0; //�û�id
		inameid = 0; //��Ϸid
		iroomid = 0; //����id
		ideskid = 0; //����id
	}
};
#pragma pack()
/********************************************************************************************/
/// ͨ�ű�ʶ����
/********************************************************************************************/

//================================================================================================
#define MDM_GP_REGISTER                 99              //���ע��
#define ASS_GP_REGISTER                 1
#define ERR_REGISTER_ERROR              0
#define ERR_REGISTER_SUCCESS            1
#define ERR_REGISTER_NAME_EXIST 2
//================================================================================================
///ͨ������ʶ
#define MDM_GP_LOGON                    100                             ///������½
///ͨ�Ÿ�����ʶ
#define ASS_GP_LOGON_BY_NAME            1                                   ///ͨ���û����ֵ�½
#define ASS_GP_LOGON_BY_ACC             2                                   ///ͨ���û�ACC ��½
#define ASS_GP_LOGON_BY_MOBILE          3                                   ///ͨ���û��ֻ���½
#define ASS_GP_LOGON_REG                4                                   ///�û�ע��
#define ASS_GP_LOGON_SUCCESS            5                                   ///��½�ɹ�
#define ASS_GP_LOGON_ERROR              6                                   ///��½ʧ��
#define ASS_GP_LOGON_ALLO_PART          7                                   ///��ص�½
#define ASS_GP_LOGON_LOCK_VALID         8                                   ///������֤

// PengJiLin, 2010-6-2, ����ǵ�����������������Ϣ֪ͨ����ˡ�(Ÿ��)
#define ASS_GP_LOGON_BY_SOFTWARE        10
#define ASS_GP_LOGON_MOBILE_VALID       11                                  ///�ֻ���֤
#define ASS_GP_LOGIN_SWITCH             12                                  //�����ܿ���
#define ASS_GP_CHECK_PHONE              13                                  //�ֻ��󶨶�����֤
#define ASS_GP_GET_DEVICE	            14                                  //�����ȡ����id
//================================================================================================
//��ɫϵͳ
#define MDM_GP_PLAYER                   150									///�û���ɫ
#define ASS_GP_ADD_PLAYER				14                                  //�û���ɫ����
#define ASS_GP_SWITCH_PLAYER			15                                  //�û���ɫ�л�
#define ASS_GP_TRANS_PLAYER				16                                  //�û���ɫת��
#define ASS_GP_CHANGE_PNAME				17                                  //�û���ɫ����
#define ASS_GP_STORE_SCORE				18                                  //�û���ɫ���
#define ASS_GP_GET_SCORE				19                                  //�û���ɫȡ��
#define ASS_GP_LOGON_PLAYER_LIST        20                                  ///�û���ɫ
#define ASS_GP_GET_JIUJI		        21                                  ///�û��ȼý�
#define ASS_GP_JIUJI_TIP				22                                  ///�û��ȼý���ʾ
#define ASS_GP_SET_BANKPASS				23									//������������
#define ASS_GP_GET_BANKLOG				24									//��ȡ��¼
#define ASS_GP_GET_BANKREALTIME			25									//��ȡʵʱ����
#define ASS_GP_TRANSFER_SCORE			26									//���չ�ת��
#define ASS_GP_TRANSFER_INFO			27									//���չ�ת�˻�ȡ��Ϣ
//================================================================================================
//�ʼ�ϵͳ
#define MDM_GP_MAIL						151									///�ʼ�ϵͳ
#define ASS_GP_GET_MAILLIST				10                                  //��ȡ�ʼ��б�
#define ASS_GP_GET_GOODS			    11                                  ///��ȡ��Ʒ
#define ASS_GP_READ_MAIL			    12                                  ///���ʼ�
#define ASS_GP_DELETE_MAIL			    13                                  ///ɾ���ʼ�
#define ASS_GP_GET_BROAD_MAIL			14                                  ///�㲥�ʼ�

#define ASS_GP_GET_QUESTION				15									//��ȡ�����б�
#define ASS_GP_ADD_QUESTION				16									//���������б�
//================================================================================================
#define MDM_RANK						152									//����
#define ASS_GP_GET_RANKLIST				1									//��ȡ����
#define ASS_GP_GET_BOSSRANKLIST				2									//��ȡboss����
//����
///////////////////////////////////////////////////////////////////////////////
//����
#define MDM_TASK						153									//����
#define ASS_GP_GET_TASKLIST				1									//��ȡ�����б�
#define ASS_GP_UPDATE_TASK				2									//��ȡ������
#define ASS_GP_GET_TASKNAME				3									//��ȡ������
///////////////////////////////////////////////////////////////////////////////
//vip�¿�
#define MDM_VIP							154									//VIP

#define ASS_GP_GET_VIP					1									//��ȡ�����б�
#define ASS_GP_GET_YkScore				2									//��ȡ�¿�����
#define ASS_GP_GET_FavCompain			3									//��ȡ�ػݻ
#define ASS_GP_GET_NewUserRewardStateList	4									//��ȡ���ֽ����б�
#define ASS_GP_GET_NewUserReward		5									//��ȡ���ֽ���
#define ASS_GP_GET_NewUserRewardList	6									//��ȡ���ֽ����б�
#define ASS_GP_GET_VipList				7									//��ȡvip�����б�
#define ASS_GP_GET_UserVipReward		8									//��ȡvip����
#define ASS_GP_GET_PaoTaiList			9									//��ȡ��̨�б�
#define ASS_GP_Get_UserPaoTai			10									//��ȡ�û���̨
#define ASS_GP_BUY_PaoTai				11									//������̨
///////////////////////////////////////////////////////////////////////////////
#define MDM_CONFIG							155									//CONFIG
#define ASS_GP_GET_CONFIG					1									//��ȡ����
#define ASS_GP_GET_GAMERECORED				2									//��ȡ��Ϸ��¼
#define ASS_GP_GET_RAT						3									//��ȡ��Ϸ˰�ձ���
#define ASS_GP_GET_CHARGE_URL				4									//��ȡ��ֵurl
#define ASS_GP_GET_CHARGE_URLNEW				5									//��ȡ��ֵurl
///////////////////////////////////////////////////////////////////////////////
#define MDM_USERSCORE						156									//�û���ֵ���
#define ASS_GP_GET_SHOUCHONG					1									//��ȡ�׳�
///////////////////////////////////////////////////////////////////////////////
#define MDM_FISH_MATCH						157									//�������
#define ASS_GP_GET_FISH_MATCH_INFO					1									//��ȡ�����б�
#define ASS_GP_ENROLL_FISH_MATCH					2									//��������
#define ASS_GP_GET_USER_MATCH_INFO					3									//��ȡ�û�������Ϣ
#define ASS_GP_GET_FISH_MATCH_RANK					4									//��ȡ��������
#define ASS_GP_GET_FISH_MATCH_REWARD				5									//��ȡ���������б�

///////////////////////////////////////////////////////////////////////////////
#define MDM_AGENCY							158							//����ϵͳ
#define ASS_GP_GET_TOTAL					1							//��ȡ������
#define ASS_GP_GET_DETAIL					2							//��ȡ��ϸ
#define ASS_GP_GET_NEEDLEVEL					3							//��ȡ����
#define ASS_GP_GET_ACHIEVE					4							//��ȡҵ��
#define ASS_GP_GET_REWARD					5							//��ȡ����
#define ASS_GP_GET_PARTNERINFO					6							//��ȡ�ϻ�����Ϣ
#define ASS_GP_GET_PARTNERSCORE					7							//�ʽ��ת����
#define ASS_GP_STORE_PARTNERSCORE					8							//��ת���ʽ��
#define ASS_GP_GET_PARTNERUSERINFO					9							//��ȡ�ϻ��������Ϣ
#define ASS_GP_GET_PARTNERUSERINFODETAIL					10							//��ȡ�ϻ��������ϸ��Ϣ
#define ASS_GP_OPEN_PARTNER					11							//�����ϻ��˹���
#define ASS_GP_UPDATE_PARTNERTIME					12							//�����ϻ���ʱ��
#define ASS_GP_GET_PROVISIONS					13							//Ԥ��
#define ASS_GP_UPDATE_PARTNERTIME_TEMP					14							//����ʱ��
#define ASS_GP_GET_TOTAL_FORMODEB					15							//��ȡ������
#define ASS_GP_GET_DETAIL_FORMODEB					16							//��ȡ��ϸ
#define ASS_GP_GET_AGENCYREWARD_FORMODEB					17							//��ȡ����
#define ASS_GP_GET_AGENCYREWARDLOG_FORMODEB					18							//��ȡ������¼
#define ASS_GP_GET_AGENCYREWARDRANK_FORMODEB				19							//����������

#define ASS_GP_GET_AGENCYREWARD_LEVEL						20					//��ȡ��ǰ�ƹ�ȼ�
#define ASS_GP_GET_AGENCY_SUBUSER							21					//��ѯ�¼��û�
///////////////////////////////////////////////////////////////////////////////
#define MDM_CASH							159							//����
#define ASS_GP_UPDATE_ALIPAY				1							//��֧����
#define ASS_GP_GET_AGENTWECHAT				2							//��ȡ����΢��
#define ASS_GP_GET_CASH						3							//����
#define ASS_GP_GET_CASH_LOG					4							//���ּ�¼
#define ASS_GP_GET_CASH_ACQUIRE					5							//��ȡ������Ҫ����
#define ASS_GP_GET_CASH_LOG_ETBET					6							//et���ּ�¼

#define ASS_GP_RECHARGE_LOG					7							//



//=====================================================================================
#define MDM_GP_WEB_INFO							160							//��ҳ��Ϣ
#define ASS_GP_WEB_ADD_SCORE						1   ///��ֵ
#define ASS_GP_WEB_MAIL						2   ///�ʼ�
#define ASS_GP_SYSTEM_BROAD						3   ///���ع���
#define ASS_GP_SENDTO_ALL_USER						4   ///����ȫ����Ϣ
#define ASS_GP_LOAD_AGENT_GAMELIST						5   ///���¼��ش����б�
#define ASS_GP_LOAD_SYSTEM_LIST						6   ///���¼��������б�
#define ASS_GP_LOAD_LUCKAWARD						7   ///���¼�����������
#define ASS_GP_LOAD_NEEDLEVEL						8   ///���¼����ƹ�ȼ�
#define ASS_GP_LOAD_BROAD						9   ///��������
#define ASS_GP_SEND_BUY_MONEY								12	   ///��ҳ֪ͨ��ֵ�ɹ� 
#define ASS_GP_RELOAD_LIST								13	   ///֪ͨ���¼�����Ϸ�б� 
#define ASS_GP_RELOAD_URL								14	   ///֪ͨ���¼���URL�б� 
#define ASS_GP_RELOAD_RAT								15	   ///֪ͨ���¼���˰��
//================================================================================================


///ͨ������ʶ
#define MDM_GP_LIST                     101                             ///��Ϸ�б�
///ͨ�Ÿ�����ʶ
#define ASS_GP_LIST_KIND                1                                   ///��ȡ��Ϸ�����б�
#define ASS_GP_LIST_NAME                2                                   ///��ȡ��Ϸ�����б�
#define ASS_GP_LIST_ROOM                3                                   ///��ȡ��Ϸ�����б�
#define ASS_GP_LIST_COUNT               4                                   ///��ȡ��Ϸ�����б�
#define ASS_GP_ROOM_LIST_PEOCOUNT       5                                   ///��ȡ��Ϸ�����б�
#define ASS_GP_ROOM_PASSWORD            6                                   //���ͷ������� Add by lxl 2010-10-08����ͼ�������뷿��ʱ���ʹ���Ϣ
#define ASS_GP_GET_SELLGAMELIST         7                                   //��ȡ��Ϸ�����б�

#define ASS_GP_CONTINUE_NEXT_ROUND		8									//������һ����Ϸ

//================================================================================================
///ͨ������ʶ
#define MDM_GP_MESSAGE                  103                             ///ϵͳ��Ϣ
///ͨ�Ÿ�����ʶ
#define ASS_GP_NEWS_SYSMSG              1                               ///���ź�ϵͳ��Ϣ
//#define ASS_GP_DUDU                       2                               ///С����
#define ASS_GP_TALK_MSG                 3                               //������Ϣ
#define ASS_GP_WORLD_HORN               4                               //����������Ϣ

//================================================================================================
///��������������ID zxj 2009-11-12
#define MDM_GP_LOCK_ACCOUNT             106
#define ASS_LOCK_ACCOUNT                1

//================================================================================================
///�����һ�
#define MDM_GP_CHARMEXCHANGE            109
#define ASS_GETLIST                     1
#define ASS_EXCHANGE                    2
#define ASS_GETHUAFEILIST               3
#define ASS_GETGOODLIST					4
#define ASS_GETGOODLOG					5

//================================================================================================
//{{ Added by JianGuankun 20111108 ����û����Ϲ���ͨѶЭ��
#define MDM_GP_USERINFO                 115     //�û�����
#define ASS_GP_USERINFO_UPDATE_BASE     1       //�û����»�����Ϣ
#define ASS_GP_USERINFO_UPDATE_DETAIL   2       //�û�������ϸ��Ϣ
#define ASS_GP_USERINFO_UPDATE_PWD      3       //�û��޸�����
#define ASS_GP_USERINFO_ACCEPT          5       //������ѽ���
#define ASS_GP_USERINFO_NOTACCEPT       6       //�����δ�ܽ���
#define ASS_GP_USERINFO_NICKNAMEID      10      //��������ǳ���ID��ID���ǳ�
#define ASS_GP_USERINFO_GET             11      //��ȡָ���û�ID�ĸ�������
#define ASS_GP_FIND_PWD                 12      //�һ�����
#define ASS_GP_SETINVITECODE            13      //����������
#define ASS_GP_SETUPDATELOGO            14      //����logo
#define ASS_GP_SHARE_WX_REWARD          15      //΢�ŷ�����



//================================================================================================
#define MDM_GP_SET_LOGO                     113
#define ASS_GP_SET_LOGO                     1

/********************************************************************************************/
///�������
#define ERR_GP_ERROR_UNKNOW             0                                   ///δ֪����
#define ERR_GP_LOGON_SUCCESS            1                                   ///��½�ɹ�
#define ERR_GP_USER_NO_FIND             2                                   ///��½���ִ���
#define ERR_GP_USER_PASS_ERROR          3                                   ///�û��������
#define ERR_GP_USER_VALIDATA            4                                   ///�û��ʺŽ���
#define ERR_GP_USER_IP_LIMITED          5                                   ///��½ IP ��ֹ
#define ERR_GP_USER_EXIST               6                                   ///�û��Ѿ�����
#define ERR_GP_PASS_LIMITED             7                                   ///�����ֹЧ��
#define ERR_GP_IP_NO_ORDER              8                                   ///����ָ����ַ 
#define ERR_GP_LIST_PART                9                                   ///������Ϸ�б�
#define ERR_GP_LIST_FINISH              10                                  ///ȫ����Ϸ�б�
#define ERR_GP_USER_LOGON               11                                  ///���ʺ��Ѿ���¼
#define ERR_GP_USERNICK_EXIST           12                                  ///���ǳ��Ѿ�����
#define ERR_GP_USER_BAD                 13                                  ///δ���ַ�
#define ERR_GP_IP_FULL                  14                                  ///IP����
#define ERR_GP_LOCK_SUCCESS             15                                  ///���������ɹ�   2009-11-12
#define ERR_GP_ACCOUNT_HAS_LOCK         16                                  ///�����Ѿ���������״̬   2009-11-12
#define ERR_GP_UNLOCK_SUCCESS           17                                  ///��������ɹ� 2009-11-12
#define ERR_GP_NO_LOCK                  18                                  ///����������û�����������Խ���ʧ�� 2009-11-12
#define ERR_GP_CODE_DISMATCH            19                                  ///�����벻ƥ�䣬����ʧ�ܡ� 2009-11-12
#define ERR_GP_ACCOUNT_LOCKED           20                                  ///���˺�������ĳ̨��������¼ʧ�� 2009-11-12 zxj
#define ERR_GP_MATHINE_LOCKED           21
#define ERR_GP_VER_ERROR				22
#define ERR_GP_USER_PASS_ERROR2				23
#define ERR_GP_USER_PASS_ERROR3				24
#define ERR_GP_USER_PASS_ERROR4				25
// PengJiLin, 2010-6-7, ��������֤���صĴ����롣Ÿ��
#define ERR_GP_USER_NOT_EXIST           30      // �û�������
#define ERR_GP_USER_OVERDATE            31      // �û��ѹ���
#define ERR_GP_DATABASE_FUNC_ERROR      32      // ���ݿ����ʧ��
#define ERR_GP_DATABASE_RETURN_ERROR    33      // ���ݿⷵ��ִ�д�����
/********************************************************************************************/
///�쳣����
//JianGuankun 20111110
#define EXP_GP_ALLO_PARTY               50                              ///���˺���ص�½


//================================================================================================
#define MDM_GP_DESK_LOCK_PASS           254                                 ////����������Ϣ
#define ASS_GP_DESK_LOCK_PASS               1                                   ///ѯ�����������Ƿ���ȷ(����ѯ��)

#define ASS_GP_GET_ROOM_KEY_USE_INFO        3                                   ///��ѯ�Լ�������������Ϣ
#define ASS_GP_GET_ROOM                     4                                   ///��ѯ�Լ���ǰ���ڵķ���
#define ASS_GET_MIN_ROOM_KEY_NUM				8								//��ȡƥ�䷿����Ҫ��С������



#define ERR_DESK_LOCK_PASS_ERROR                0//ʧ��
#define ERR_DESK_LOCK_PASS_SUCCESS              1//�ɹ�
#define ERR_DESK_LOCK_NO_ENOUGH_ROOMKEY			2//û���㹻�ķ�������(AA�Ʒ���)

#define ERR_DESK_LOCK_PASS_ERROR                0//ʧ��
#define ERR_DESK_LOCK_PASS_SUCCESS              1//�ɹ�

#define ERR_USE_ROOM_KEY_ERROR                  0//ʧ��
#define ERR_USE_ROOM_KEY_SUCCESS                1//�ɹ�
#define ERR_USE_ROOM_KEY_RECOME                 2//���»ص��Ѿ�����ķ���
#define ERR_USE_ROOM_KEY_EXISTS                 3 //�Ѿ���½�����ݿ�������ĵط����޷���������

#define ERR_USE_ROOM_KEY_PART                   0//����
#define ERR_USE_ROOM_KEY_FINISH                 1//���

#define ERR_ROOM_CREATE_INFO_PART               0
#define ERR_ROOM_CREATE_INFO_FINISH             1





#endif
