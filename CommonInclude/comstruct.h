#pragma once

#ifndef AFC_COM_STRUCT_HEAD_FILE
#define AFC_COM_STRUCT_HEAD_FILE

/********************************************************************************************/
#include "PropMessage.h"

///�����������
#define GRR_MEMBER_ROOM         0x00000001L                         ///��Ա����
#define GRR_IP_LIMITED          0x00000002L                         ///��ַ����
#define GRR_ENABLE_WATCH        0x00000004L                         ///�����Թ�
#define GRR_UNENABLE_WATCH      0x00000008L                         ///�����Թ�
#define GRR_AUTO_SIT_DESK       0x00000010L                         ///�Զ����£��������ڷ����׳�ʹ��
#define GRR_LIMIT_DESK          0x00000020L                         ///������λ
#define GRR_LIMIT_SAME_IP       0x00000040L                         ///����ͬIP
#define GRR_RECORD_GAME         0x00000080L                         ///��¼��Ϸ
#define GRR_STOP_TIME_CONTROL   0x00000100L                         ///ֹͣʱ�����
#define GRR_ALL_ON_DESK         0x00000200L                         ///�Ƿ����������²ſ�ʼ����
#define GRR_FORBID_ROOM_TALK    0x00000400L                         ///��ֹ��������
#define GRR_FORBID_GAME_TALK    0x00000800L                         ///��ֹ��Ϸ����
#define GRR_MATCH_REG           0x00001000L                         ///��������
#define GRR_EXPERCISE_ROOM      0x00002000L                         ///ѵ����
#define GRR_VIDEO_ROOM          0x00004000L                         ///��Ƶ����
///
///�޸ķ����׳��ɿ����������������ͷ�����⣡
#define GRR_NOTFUFENG           0x00008000L     ///����������
#define GRR_NOTCHEAT            0x00010000L     ///������
///
///̨�ѳ�
#define GRR_ALL_NEED_TAX        0x00020000L     ///��̨�ѳ�,�����˶������һ������̨��
#define GRR_QUEUE_GAME          0x00040000L     /// �Ŷӻ�

#define GRR_NOT_COST_POINT      0x00080000L     ///��ҳ����ۻ���

#define GRR_CONTEST             0x00100000L     ///��ʱ��̭������
#define GRR_TIMINGCONTEST       0x00800000L     ///��ʱ��        --RoomRule == 8388608


///�ټ���
#define DESK_TYPE_NORMAL        0       ///
#define DESK_TYPE_BJL           1       ///



//=======================================�ֲ�ʽʱ��Z��B��ͨѶЭ�� rongqf20120425
#define MDM_ZD_PACKAGE                  123
#define ASS_ZD_CONNECT                  1
#define ASS_ZD_LOGON                    2

#define HDC_ZD_CONNECT                  1
#define HDC_ZD_DISCONNECT               2
#define HDC_ZD_KICK                     3
#define ASS_ZD_BATCH                    3
#define ASS_ZD_SINGLE                   4
#define HDC_ZD_FAIL                     2
#define ASS_ZD_BATCH_ToW                5               //ת�������е�W������
#define ASS_ZD_BATCH_ToGame             6               //ת�������е�W������
#define ASS_ZD_SINGLE_ToGame            7
#define ASS_ZD_POST_WEB					8				//Ͷ����ҳ

//������Ϣ
#define MZTW_WORLD_HORN							2//������������Ϣid
#define ASS_ZTW_SEND							1//�����������ȸ�����Ϣid


//�����Ϣ
#define MZTW_USERINFO							4//֪ͨ�����Ϣid
#define ASS_ZTW_SCORE							1//֪ͨ�����Ϣid
#define ASS_ZTW_DJ_ENDGAME						2//֪ͨ�羺������Ϣ
//������Ϣ
#define MZTW_ROOM_KEY					3//������Ϣid

#define ASS_ZTW_ROOM_KEY_USE				1//���͸�����Ϣid����������֪ͨ��Ϸ������ĳ�����ĳ�����Ѿ�����ʹ���ˣ�
#define ASS_ZTW_ROOM_CUTNET_USER				2//���͸�����Ϣid��ѯ�ʸ�����Ƿ���ĳ�����ڴ��ڶ���״̬��
#define RES_TELL_CLIENT						0	//֪ͨ�ͻ���
#define RES_UN_TELL_CLIENT					1//����֪ͨ�ͻ���


//������Ϣ
#define MZTW_ROOMINFO						5//֪ͨ������Ϣid
#define ASS_ZTW_GAMERECORED					1//֪ͨ��Ϸ��¼
#define ASS_ZTW_NEWGAMERECORED				2//֪ͨ����Ϸ��¼
#define ASS_ZTW_CHANGEGAMESTATION			3//�޸���Ϸ״̬
#define ASS_ZTW_WEB_POST					4   //��ҳͶ��

///������Ϣ
#define MZTW_GUILD							6

#define ASS_ZTW_GUILD_DELETE				1
#define ASS_ZTW_GUILD_CHANGERULE			2
#define ASS_ZTW_GUILD_KICKUSER				3



#pragma pack(1)



struct MZTW_Mess_Room_Key_Use_struct
{
	int				iUserID;			//�û�id
	int				iNameID;			//��ϷID
	int				iRoomID;			//����ID
	int				iDeskID;			//����ID
	char			szLockPass[61];		//��������
	COleDateTime                        tTime;                              //ʹ�ü�¼ʱ��
	int       id;             //id
	int       iGameRound;     //����
	__int64   iGameRule;      //flag
	int		  iCost;		  //���ķ���
	bool	  bAAFanKa;		  //�Ƿ�AA�Ʒ���
	MZTW_Mess_Room_Key_Use_struct()
	{
		ZeroMemory(this, sizeof(MZTW_Mess_Room_Key_Use_struct));
	}
};



struct MZTW_Guild_Delete 
{
	int		iGuildId;	///����id
};
struct MZTW_Guild_ChangeRule 
{
	int		iGuildId;	///����id
	BYTE	bOpType;	///1 �޸�˰�� 2 �Ƴ�����
	int		iTaxRate;
};
struct MZTW_Guild_KickUser
{
	int		iGuildId;	///����id
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
    int                         nXiTongZuoZhuang;       //0��ϵͳ��ׯ�� 1�������ׯ
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
///end of �ټ���
///
///
///��Ϸ�����Ϣ�ṹ �������� DLL �����ȡ��
struct ServerDllInfoStruct
{
    ///��Ϸ�����Ϣ
    UINT                        uServiceVer;                        ///����汾
    UINT                        uNameID;                            ///��Ϸ���� ID
    UINT                        uSupportType;                       ///֧������
    UINT                        uDeskPeople;                        ///��Ϸ����
    char                        szGameName[61];                     ///��Ϸ����
    char                        szGameTable[61];                    ///���ݿ������
    char                        szDLLFileName[61];                  ///�ļ�����

    ///������Ϣ
    char                        szWriter[61];                       ///���������
    char                        szDllNote[51];                      ///��ע��Ϣ
};

///�������������Ϣ�ṹ   ���������������ݣ�
struct ManageInfoStruct
{
    int                         iRoomFlag[2];                       // ��ʹ�õ��ֶΣ�ռλ��

    ///��Ϸ��Ϣ
    UINT                        uNameID;                            ///��Ϸ���� ID
    char                        szGameName[61];                     ///��Ϸ����
    UINT                        dwRoomRule;                         ///��Ϸ���ù���
    UINT                        dwUserPower;                        ///��Ϸ����Ȩ��
    char                        szLockTable[31];                    ///����������
    char                        szIPRuleTable[31];                  ///IP ���Ʊ�����
    char                        szNameRuleTable[31];                ///UserName���Ʊ�����

    int                         iRoomInfoFlag[2];                   // ��ʹ�õ��ֶΣ�ռλ��

    ///������Ϣ
    UINT                        uComType;                           ///��Ϸ����
    UINT                        uDeskType;                          ///��Ϸ�����ͣ���ҪΪ��Ӱټ�����
    UINT                        uBasePoint;                         ///��Ϸ������
    UINT                        uLessPoint;                         ///��Ϸ�����ٷ���
    UINT                        uMaxPoint;                          ///��Ϸ��������
    UINT                        uMaxLimite;                         ///���ⶥֵ
    UINT                        uRunPublish;                        ///���ܿ۷�
    UINT                        uTax;                               ///˰�ձ���
    UINT                        uListenPort;                        ///�����˿�
    UINT                        uMaxPeople;                         ///���������Ŀ
    int                         iSocketSecretKey;                   ///socket���ܵ���Կ

    //����ר��
    int                                 iGameID;
    int                                 iMatchID;
    int                                 iContestID;
    int                                 iRankNum;//��������
    int                                 *pAwards;//��������
    int                                 *pAwardTypes;//������������
    int                                 iLowCount;
    __int64                             i64Chip;
    __int64                             i64LowChip;
    __int64                             i64TimeStart;
    __int64                             i64TimeEnd;
    int                                 iTimeout;
    int                                 iLeasePeople;                   ///�����������ڴ�ֵ�������������
    int                                 iUpPeople;                      ///���������ﵽ��ֵ���ſ�ʼ�������ӣ������÷���״̬Ϊ��ʼ��Ϸ
    int                                 iRoomState;

    //���޷���ID��ʱ��
    int                                 iLeaseID;
    __int64                             i64LeaseTimeStart;
    __int64                             i64LeaseTimeEnd;


    //ÿ�ֳ���ʱ���Ƿ����ͽ��
    int                                 iTimeLength;
    __int64                             iSendCount;

    ///
    ///Vip��½����
    UINT uVipReseverCount;///Vip������½��Ŀ
    ///

    bool                        bVIPRoom;                            ///< ���«�������Ƿ�VIP���䣨VIP�����������룩

    UINT                        uDeskCount;                         ///��Ϸ����Ŀ
    char                        szGameTable[31];                    ///��Ϣ������
    UINT                        uStopLogon;                         ///ֹͣ��½����

    int                     uVirtualUser;
    int                     uVirtualGameTime;


    ///��ս������ID       --begin
    UINT                        uBattleRoomID;
    ///��ս��������Ϣ��
    char                        szBattleGameTable[31];

    ///-------------------  end

    char            szRoomPassWord[61];                     //��������  ������ܹ���
    ///��������Ϣ
    UINT                        uRoomID;                            ///����ID
    char                        szGameRoomName[61];                 ///����������
	wchar_t                     GameName[61];                 ///����������
    ///�������ݿ���Ϣ
    char                        szSQLIP[16];                        ///�������ݿ��ַ
    char                        szSQLName[61];                      ///�������ݿ��û�����
    char                        szSQLPass[61];                      ///�������ݿ�����
    char                        szSQLDataName[61];                  ///�������ݿ�����
    SHORT                       nPort;

    ///��½���ݿ���Ϣ
    char                        szLogonSQLIP[16];                   ///��½���ݿ��ַ
    char                        szLogonSQLName[61];                 ///��½���ݿ��û�����
    char                        szLogonSQLPass[61];                 ///��½���ݿ�����
    char                        szLogonSQLDataName[61];             ///��½���ݿ�����
    SHORT                       logonPort;

    ///�������ݿ���Ϣ
    char                        szNativeSQLIP[61];                  ///�������ݿ��ַ
    char                        szNativeSQLName[61];                ///�������ݿ��û�����
    char                        szNativeSQLPass[61];                ///�������ݿ�����
    char                        szNativeSQLDataName[61];            ///�������ݿ�����
    SHORT                       NativePort;

    ManageInfoStruct()
    {
        //iSocketSecretKey = SECRET_KEY;
    }
};

///�ں����� �������� DLL ����������ã�
struct KernelInfoStruct
{
    ///��Ϸ����
    UINT                        uNameID;                            ///��Ϸ���� ID ����
    UINT                        uDeskPeople;                        ///��Ϸ����

    UINT                        uMinDeskPeople;                     ///������Ϸ֧��M-N������Ϸ

    ///�ں�����
    BYTE                        bMaxVer;                            ///�����߰汾
    BYTE                        bLessVer;                           ///�����Ͱ汾
    BYTE                        bStartSQLDataBase;                  ///ʹ�����ݿ�����
    BYTE                        bNativeDataBase;                    ///ʹ�ñ������ݿ�
    BYTE                        bLogonDataBase;                     ///ʹ�õ�½���ݿ�
    BYTE                        bStartTCPSocket;                    ///�Ƿ���������
    UINT                        uAcceptThreadCount;                 ///Ӧ���߳���Ŀ
    UINT                        uSocketThreadCount;                 ///SOCKET �߳���Ŀ
	BYTE                        bStartHttpDataBase;                  ///ʹ����ҳ��������
};

/********************************************************************************************/

///�б���ҽ���������
#define GLK_NO_DATA             0                                   ///û������
#define GLK_GAME_KIND           1                                   ///��Ϸ����
#define GLK_GAME_NAME           2                                   ///��Ϸ����
#define GLK_GAME_ROOM           3                                   ///��Ϸ����
#define GLK_GAME_USE            4                                   ///��Ϸʹ��
#define GLK_GAME_WEB            5                                   ///��INI����WEB
#define GLK_GAME_ADD            12                                  ///��ϵ����

//{add by rongqiufen 2010.7.2
#define GLK_GAME_KING_WEBGAME   6                                   ///��bcf����WEB
#define GLK_GAME_WEBGAME        7                                   ///��bcf����WEB
#define GLK_GAME_KING_EXEGAME   8                                   ///��bcf����WEB
#define GLK_GAME_EXEGAME        9                                   ///��bcf����WEB
#define GLK_GAME_SELLLIST       10                                  ///�����б�
#define GLK_GAME_OPENINFO       11                                  ///����˵��
//add by rongqiufen 2010.7.2}

///��Ϸ�б����ṹ
struct AssistantHead
{
    UINT                        uSize;                              ///���ݴ�С
    UINT                        bDataType;                          ///���ͱ�ʶ
};


///��Ϸ����de�ṹ
struct ComKindInfo///������Ϸ����AddTreeData
{
    AssistantHead               Head;
    UINT                        uKindID;                            ///��Ϸ���� ID ����
    char                        szKindName[61];                     ///��Ϸ��������

    // duanxiaohui 20111107 ��Ϸ��������Ŀ¼����
    UINT                        uParentKindID;                      ///����Ϸ����ID����
    // end duanxiaohui
};

///��Ϸ���ƽṹ
struct ComNameInfo
{
    AssistantHead               Head;
    UINT                        uKindID;                            ///��Ϸ���� ID ����
    UINT                        uNameID;                            ///��Ϸ���� ID ����
    char                        szGameName[61];                     ///��Ϸ����
    char                        szGameProcess[61];                  ///��Ϸ������
    UINT                        m_uOnLineCount;                     ///��������
    UINT                        uVer;                               ///�汾
    ///
    ///Kylin 20090202 ��Ϸ������ʼ������
    ComNameInfo()
    {
        m_uOnLineCount = 0;
    }
    ///

};

///��Ϸ�����б�ṹ
struct ComRoomInfo
{
    AssistantHead               Head;
    UINT                        uComType;                           ///��Ϸ����
    UINT                        uKindID;                            ///��Ϸ���� ID ����
    UINT                        uNameID;                            ///��Ϸ���� ID ����
    UINT                        uRoomID;                            ///��Ϸ���� ID ����
    UINT                        uPeopleCount;                       ///��Ϸ��������
    UINT                        iUpPeople;                          ///���������û��ﵽֵ��ſ���
    UINT                        uDeskPeople;                        ///ÿ����Ϸ����
    UINT                        uDeskCount;                         ///��Ϸ����������Ŀ
    UINT                        uServicePort;                       ///��������˿�
    char                        szServiceIP[25];                    ///������ IP ��ַ
    char                        szGameRoomName[61];                 ///��Ϸ��������
    int                         uVirtualUser;                       ///20081211 , Fred Huang
    int                         uVirtualGameTime;                   ///20081211 , Fred Huang
    UINT                        uVer;                               ///�汾
    UINT                        dwRoomRule;                         ///��Ϸ�������
    bool                        bVIPRoom;                           ///< ���«�������Ƿ�VIP���䣨VIP�����������룩
    int                         iBasePoint;                         //��������
    UINT                        uLessPoint;                         //��һ��������
    UINT                        uMaxPoint;                          //��һ��������
    UINT                        uRoomTypeID;                        //����һ�����ࣨ������0���߼���1��������2��
    UINT                        uRoomTypeSecondID;                  //����������ࣨ��Ҫ��������һ������ķ�������ģ��������1�ŷ�0��������2�ŷ�1��3�ŷ�2�ȣ�

    int                                 iContestID;
    __int64                             i64TimeStart;
    __int64                             i64TimeEnd;

    //���޷���ID��ʱ��
    int                                 iLeaseID;
    __int64                             i64LeaseTimeStart;
    __int64                             i64LeaseTimeEnd;
    ///wushuqun 2009.6.5
    ///��ս������ID       --begin
    UINT                        uBattleRoomID;
    ///��ս��������Ϣ��
    char                        szBattleGameTable[31];
    bool            bHasPassword;           // �������� Add by lxl 2010-10-08
    UINT            dwTax;                  // ���䷿�� Add by wyl 11-5-16
    UINT                        uUserDeskCount;                     //ʹ�õ����Ӻ�
    ///
    ///Kylin 20090202 ��Ϸ������ʼ������
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

///��Ϸ���������ṹ
struct GameInsideInfo
{
    AssistantHead               Head;                               ///�б�ͷ
    UINT                        uTrunID;                            ///ת�� ID
    char                        szDisplayName[61];                  ///��ʾ����
};

/********************************************************************************************/

///�û�״̬����
#define USER_NO_STATE           0                                   ///û��״̬�������Է���
#define USER_LOOK_STATE         1                                   ///�����˴���û������
#define USER_SITTING            2                                   ///������Ϸ��
#define USER_ARGEE              3                                   ///ͬ��״̬
#define USER_WATCH_GAME         4                                   ///�Թ���Ϸ
#define USER_DESK_AGREE         5                                   ///����ͬ��
#define USER_CUT_GAME           20                                  ///����״̬         ����Ϸ��״̬��
#define USER_PLAY_GAME          21                                  ///��Ϸ������״̬    ����Ϸ��״̬��




///�û���Ϣ�ṹ
struct UserInfoStruct
{
    int                         dwUserID;                           ///ID ����
    int                         dwExperience;                       ///����ֵ
    int                         dwAccID;                            ///ACC ����
    int                         dwPoint;                            ///����
    __int64                     i64Money;                           ///���
    __int64                     i64Bank;                            ///����
    UINT                        uWinCount;                          ///ʤ����Ŀ
    UINT                        uLostCount;                         ///����Ŀ
    UINT                        uCutCount;                          ///ǿ����Ŀ
    UINT                        uMidCount;                          ///�;���Ŀ
    char                        szName[61];                         ///��¼��
  
    UINT                        bLogoID;                            ///ͷ�� ID ����
    UINT                        bDeskNO;                            ///��Ϸ����
    BYTE                        bDeskStation;                       ///����λ��
    BYTE                        bUserState;                         ///�û�״̬
    BYTE                        bMember;                            ///��Ա�ȼ�
    BYTE                        bGameMaster;                        ///����ȼ�
    ULONG                       dwUserIP;                           ///��¼IP��ַ
    bool                        bBoy;                               ///�Ա�
    char                        nickName[100];                      ///�û��ǳ�
    UINT                        uDeskBasePoint;                     ///���õ����ӱ���
    int                         dwFascination;                      ///����
    int                         iVipTime;                           ///��Աʱ��
    int                         iDoublePointTime;                   ///˫������ʱ��
    int                         iProtectTime;                       ///�����ʱ�䣬����
    int                         isVirtual;                          ///�Ƿ�����չ������ ///20081211 , Fred Huang
    UINT                        dwTax;                               ///����    add by wyl    11-5-16



   

    ///3 ,���ӱ���VIP���
    int                         userType;


    UINT                         userInfoEx1;    ///��չ�ֶ�1������������ʯ�������ʱ�䣬�ɻ�Զ�����
    UINT                         userInfoEx2;    ///��չ�ֶ�2������GM����֮����ʱЧ����zxd�����20100805

    int                          bDisAble;        //�����ж��ǲ�������  add by lxl 2010-12-10,Ų��Ϊ��Ź���


    ///����ר��
    int                         iContestID;   //����ID��Ψһ�ı�ʶһ������
    __int64                     i64ContestScore;
    int                         iContestCount;

    int                         iRankNum;       //��������
	UINT                        iEndTime;		//��̭���������ʱ��
    int                         iAward;         //������ȡ�Ľ���
    int                         iAwardType;     //������ȡ�Ľ�������

	//��Ӹ��˷�������
	int							iRoomKeyNum;	//���˷���
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



///�û�����ṹ
struct UserRuleStruct
{
    ///��������
    BYTE                        bSameIP;                            ///��ͬ IP
    BYTE                        bIPLimite;                          ///����IPǰ��λ�������Ϸ
    BYTE                        bPass;                              ///��������
    BYTE                        bLimitCut;                          ///���ƶ���
    BYTE                        bLimitPoint;                        ///���Ʒ���
    BYTE                        bCutPercent;                        ///������
    char                        szPass[61];                         ///��������
    int                         dwLowPoint;                         ///��ͷ���
    int                         dwHighPoint;                        ///��߷���
};


//�Զ�������ӵĽṹ��--2013-01-28 wangzhitu
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
    Control_None = 0,			//������
    Control_Auto = 1,			//ʵʱ�����Զ�
	Control_Manual = 2,			//ʵʱ�����ֶ�
	Control_Single666 = 3,		//����666//
	Control_Single101 = 4,		//����101//
	Control_Single = 5,			//����
	Control_Stock = 6,			//���������
	Control_StockPersonal = 7,	//���˿�����
};

//ʱʱ����
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
	/*------------------�����¿���---------------------*/
	int                 nType;                      //Contol_Type��0�����ƣ�1�Զ����ƣ�2�ֶ�����
	int                 nSystemWin;                 //0�������ƣ�1ׯ�䣬2ׯ���䣬3ϵͳ�䣬4ϵͳ����
	int                 nControlNum;                //���ƾ���,�ֶ�����������1��
	int                 nContrlDesk;                //�Զ�����ʱ�����Ƶ����Ӻ�
	int                 nControlArea;               //���ƿ�������,ÿ����Ϸ��һ����
                                                    //����ţţ��(��λ��0x01��0x02��0x04��0x08��Ӧ�죬�أ�������)
};


//ǿ��666��Ϣ
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
	__int64				CtlCurPoint;				//��ǰ����
	__int64				CtlGoalPoint;				//Ŀ�����
	int                 CtlLevel;					//���Ƶȼ�
};                                                       


//�°���Ʒ������
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

/*-------------��̨ר�ýṹ��-------------------------------------*/
#define  TEST_WEB_MSG			1
#define  MAX_PLAYER_180			180

//�������
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
	BYTE        nGameState;			//��Ϸ״̬
	BYTE        nTimeLeave;			//��״̬ʣ��ʱ��
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
