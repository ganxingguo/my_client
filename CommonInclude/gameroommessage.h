#pragma once

#ifndef AFC_GAME_ROOM_MESSAGE_HEAD_FILE
#define AFC_GAME_ROOM_MESSAGE_HEAD_FILE

//�ȼý���
#define JIUJI_RESLUT					0
//ȡ�ֽ��
#define GETSCORE_RESULT					1
//��ֽ��
#define STORESCORE_RESULT				2
//��ȡ������
#define TASK_RESULT						3
//������
#define BUY_RESULT						4
//������̨���
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




//������������ݸ���
struct MSG_GR_GRM_UpData
{
	//����
	bool        bAIWinAndLostAutoCtrl;  //�����˿�����Ӯ����
	__int64     iAIWantWinMoney[3];     /**<��������Ӯ���ƣ�������ӮǮ����1,2,3 4���öϵ� */
	int         iAIWinLuckyAt[4];           /**<��������Ӯ���ƣ�������������1,2,3,4ӮǮ�ĸ��� */
	__int64     iReSetAIHaveWinMoney;       //��������Ӯ���ƣ����û������Ѿ�ӮǮ����Ŀ

	__int64     iAIHaveWinMoney;        //������Ӯ��Ǯ
	//��Ӯ����
	bool        bWinProbCtrl;           //����
	int         dwUserID_win[20];       //�ɶ�ȡ20�飬����Ӱ������,���ID
	int         iProb_win[20];          //��Ӯ����[0,100]
	int         dwUserID_los[20];       //�ɶ�ȡ20�飬����Ӱ������,���ID
	int         iProb_los[20];          //��Ӯ����[0,100]

	MSG_GR_GRM_UpData()
	{
		memset(this, 0, sizeof(MSG_GR_GRM_UpData));
	}
};

///��Ϸ�����½
struct MSG_GR_S_RoomLogon
{
	UINT                                uNameID;                            ///���� ID
	int                                 dwUserID;                           ///�û� ID
	UINT                                uRoomVer;                           ///�����汾
	UINT                                uGameVer;                           ///��Ϸ�汾
	char                                szMD5Pass[50];                      ///��������
	bool								IsReturn;
	int									iGuildId;							///����id
	char								szIP[20];							//ip
	int									iMatchId;							///����id
	char								szCityInfo[50];
};

struct MSG_GR_S_RoomMatch 
{
	int									dwUserID;
	int									iMatchType;							///����ƥ������ 0 �����ׯ 1 ϵͳ��ׯ
};
struct MSG_GR_S_RoomLogonByUserName
{
	UINT                                uNameID;                            ///���� ID
	int                                 dwUserID;                           ///�û� ID
	char                                szName[61];                         ///��½����
	UINT                                uRoomVer;                           ///�����汾
	UINT                                uGameVer;                           ///��Ϸ�汾
	char                                szMD5Pass[50];                      ///��������
	bool								IsReturn;
	int									iGuildId;							///����id
	LONGLONG							MinScore;							//��ͷ���
	LONGLONG							MaxScore;							//��߷���
};

///���᷿��ƥ��
struct MSG_GR_S_RoomMatchGuild
{
	int									iUserId;		///���id
	int									iGuildId;		///����id
};

///��ȡ�û���չ��Ϣ
struct MSG_GR_S_GetUserExpand
{
	int                                 dwUserID;                           ///�û� ID
};

///��ȡ�û���չ��Ϣ
struct MSG_GR_S_UpdateRobotScore
{
	int                                 dwUserID;                           ///�û� ID
	LONGLONG							MinScore;							//��ͷ���
	LONGLONG							MaxScore;							//��߷���
};
///��Ϸ�����½
struct MSG_GR_R_LogonResult
{
	int                                 dwGamePower;                        ///�û�Ȩ��
	int                                 dwMasterPower;                      ///����Ȩ��
	int                                 dwRoomRule;                         ///���ù���
	UINT                                uLessPoint;                         ///���پ���ֵ
	UINT                                uMaxPoint;                          ///��ྭ��ֵ
	UserInfoStruct                      pUserInfoStruct;                        ///�û���Ϣ
	RECEIVEMONEY                        strRecMoney;                        //�Ǳ�������ҽ�Ҳ����Զ�����

	///wushuqun 2009.6.6
	///��¼����ʱ��ʱ��ȡ�����������
	int                                 nVirtualUser;
	int                                 nPresentCoinNum;  ///< ���ͽ������



	//����ר��
    int                                 iMatchID;
    int                                 iMatchNeedCnt;
    __int64                             iSignupFee;
	__int64                             i64TimeStart;
	__int64                             i64TimeEnd;
	__int64                             i64LowChip;
    int                                 iMatchRule;
    int                                 iSignupCnt;

	bool                                bGRMUser;//����ҿ��Դ򿪷��������
	bool                                bGRMRoom;//�÷�����Դ򿪷��������

	MSG_GR_GRM_UpData                   GRM_Updata;//�����ڸ�������

	//�������Ӽ�����Ч��������
	UINT                                itime;//��Чʱ�䳤��
	UINT                                inumber;//������
	int                                 iSignupCntReal;

	MSG_GR_R_LogonResult()
	{
		memset(this, 0, sizeof(MSG_GR_R_LogonResult));
	}


};

///��Ϸ�����½
struct MSG_GR_R_OtherRoom
{
	int                                 iRoomID;
	CHAR                                szGameRoomName[61];                 ///��������
};

///��Ϸ����״̬
struct MSG_GR_DeskStation
{
	BYTE                                bDeskStation[400];                  ///����״̬
};

///////////////////////////////////////////////
///�������Ӹ�������
enum eUpdateType
{
	UpdateType_null = 0, ///��
	UpdateType_sit = 1, ///�������
	UpdateType_left = 2, ///����뿪
	UpdateType_begin = 3, ///��Ϸ��ʼ
	UpdateType_end = 4, ///��Ϸ����
	UpdateType_close = 5, ///����ر�
};

///����״̬
enum eDeskState
{
	State_null = 0,
	State_playing = 1,
	State_full = 2,
};

///�������������Ϣ
struct GuildDeskUser
{
	int iUserId;
	int iLogoId;
	char nickName[100];
};

///��������
struct GuildDesk
{
	int iDeskNo; ///�����±�

	int	iDeskIndex; ///ʵ����������

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

///����������Ϣ
struct MSG_GR_C_GuildDeskReq
{
	int		iGuildId;
};


///����������Ϣ
struct MSG_GR_C_USERSCORELOG
{
	int		UserID;
};

///������Ϣ
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

///����������Ϣ
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



///�û��뿪����
struct MSG_GR_R_UserLeft
{
	int                         dwUserID;                           ///�û� ID
};

///�û�����
struct MSG_GR_S_UserSit
{
	UINT                                bDeskIndex;                         ///��������
	BYTE                                bDeskStation;                       ///����λ��
	TCHAR                               szPassword[61];                     ///��������
};




///�û�����
struct MSG_GR_R_UserCut
{
	int                                 dwUserID;                           ///�û� ID
	UINT                                bDeskNO;                            ///��Ϸ����
	BYTE                                bDeskStation;                       ///λ�ú���
};

///�û����»�������
struct MSG_GR_R_UserSit
{
	int                                 dwUserID;                           ///�û� ID
	BYTE                                bLock;                              ///�Ƿ�����
	UINT                                bDeskIndex;                         ///��������
	BYTE                                bDeskStation;                       ///����λ��
	BYTE                                bUserState;                         ///�û�״̬
	int                                 bIsDeskOwner;                       ///̨��

	UserInfoStruct                      pUserInfoStruct;                        ///�û���Ϣ
};

struct MSG_GR_R_UserUp
{
	INT                                 dwUserID;                           //�û� ID
	BYTE                                bLock;                              //�Ƿ�����
	UINT                                bDeskIndex;                         //��������
	BYTE                                bDeskStation;                       //����λ��
	BYTE                                bUserState;                         //�û�״̬
	bool                                bIsDeskOwner;                       //̨���뿪
};


///�û����뷿��
struct MSG_GR_R_UserCome
{
	UserInfoStruct                      pUserInfoStruct;                        ///�û���Ϣ
	//MSG_GR_R_UserSit                    sitInfo;                                ///������Ϣ

};



///����ṹ
struct MSG_GR_HASH
{
	CHAR                                szMessage[65];        ///��������
};

///����ṹ
struct MSG_GR_RS_NormalTalk
{
	COLORREF                            crColor;                            ///������ɫ
	SHORT                               iLength;                            ///��Ϣ����
	int                                 dwSendID;                           ///�û� ID
	int                                 dwTargetID;                         ///Ŀ�� ID
	int                                 nDefaultIndex;                      ///=0����������ݣ�>0��ѡ�������
	CHAR                                szMessage[MAX_TALK_LEN + 1];        ///��������
};



///�û�ͬ��ṹ
struct MSG_GR_R_UserAgree
{
	UINT                                bDeskNO;                            ///��Ϸ����
	BYTE                                bDeskStation;                       ///λ�ú���
	BYTE                                bAgreeGame;                         ///ͬ���־
};

///�û�����ֵ
struct MSG_GR_R_UserPoint
{
	int                                 dwUserID;                           ///�û� ID
	__int64                             dwPoint;                            ///�û�����ֵ
	__int64                             dwMoney;                            ///�û����
	__int64                             dwSend;                             ///����
	BYTE                                bWinCount;                          ///ʤ��
	BYTE                                bLostCount;                         ///���
	BYTE                                bMidCount;                          ///ƽ��
	BYTE                                bCutCount;                          ///�Ӿ�
	RECEIVEMONEY                        strAutoSendMoney;                   //����Զ����� 2013-01-29 wangzhitu
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///����������

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

///��̨ ����/�رճ���
struct MSG_GR_ContestWebOp
{
	int					iOpType; ///2 ���/���� 3 ɾ��/�ر�
	int					iMatchId;
};

///��̨�޸� ֪ͨ���
struct MSG_GR_S_ContestWebOp
{
	int					iOpType; ///3 ɾ��/�ر� 4 �޸ı�������
	int					iMatchId;
	__int64				iSignupFee;
};
///�㲥����
struct MSG_GR_S_ContestBroadSignup
{
	int					iSignupCnt;
	int					iSignupCntReal;
};

///�ȴ�����
struct MSG_GR_S_ContestWaitOver
{
	int					iMatchUserCount; ///��ǰʣ���������
    int                 iRankNum;	///Ŀǰ����
    int                 iLeftDesk;  ///��ʣ����
};

///��Ϸ�и�������
struct MSG_GR_S_ContestUpdateRank
{
	int                 iRankNum;	///Ŀǰ����
};

///֪ͨ��һ��
struct MSG_GR_S_ContestNextRound
{
	int					iMatchUserCount; ///��ǰʣ���������
	int					iRankNum; ///Ŀǰ����
};

///��̭/����
struct MSG_GR_S_ContestAward
{
	int					iRankNum;
	LONGLONG			iRankAward;
};

///��������Ϣ
struct MSG_GR_ContestAwardReq
{
	int					iUserID;
};

///������Ϣ
struct MSG_GR_S_ContestAwardReq
{
	LONGLONG			iRankAward[20];
};

///����
struct MSG_GR_ContestApply
{
    int					iUserID;  
    int					iTypeID;   //�������ͣ�0-������1-����
};

struct MSG_GR_S_ContestApply
{
	int					iTypeID;	//0 ���� 1 ����
};

///���������Ϣ
struct MSG_GR_UserContest
{
	int					iUserID;
};

///��ұ�����Ϣ
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

///�޸��û����ӱ����趨
struct MSG_GR_S_SetDeskBasePoint
{
	int                                 dwUserID;                           ///�û�ID
	int                                 iDeskBasePoint;                     ///�û��趨�����ӱ���
	BYTE                                bDeskOwner;                         ///�Ƿ�̨��
};



///������������ݰ�
struct MSG_GR_S_CutUser
{
	int                         dwUserID;                           ///����Ա�û� ID
	int                         dwTargetID;                         ///Ŀ���û� ID
};


/********************************************************************************************/
//���
struct CMD_C_STORE_SCORE
{
	LONGLONG Score;
};

//ȡ��
struct CMD_C_GET_SCORE
{
	LONGLONG							Score;
	char								MD5_BANKPASS[54];
};

//��������������
struct CMD_C_UPDATE_PROPCARD
{
	int                                 UserID;                             ///�û�ID
	int									PropCardID;
	int									PropCardCount;
};
//�����������������
struct CMD_S_UPDATE_PROPCARD
{
	int									ret;
	int									IsADD;
	int                                 UserID;                             ///�û�ID
	int									PropCardID;
	int									PropCardCount;
};

//���ͬ��������
struct CMD_S_SYNCHRO_PROPCARD
{
	int									IsADD;
	int                                 UserID;                             ///�û�ID
	int									PropCardID;
	int									PropCardCount;
};
//��ȡ�������б�
struct CMD_C_GET_PROPCARD
{
	int                                 UserID;                             ///�û�ID
};

///��Ϸ��Ϣ
struct MSG_GM_S_ClientInfo
{
	BYTE                                bEnableWatch;                       ///�����Թ�
};

struct MSG_GR_USERINFO
{
	INT										UserID;								//�û�id
	INT64									RoomKey;							//����
	INT64									WalletMoney;						//���
};

struct MSG_GR_C_GET_USERINFO
{
	int    dwUserID;           //�û�ID
};


//���ͬ��������
struct CMD_S_HASH
{
	char        HashResult[64];
	BYTE        Reslut[1000];
};
///��Ϸ��Ϣ
struct MSG_GM_S_GameInfo
{
	BYTE                                bGameStation;                       ///��Ϸ״̬
	BYTE                                bWatchOther;                        ///�����Թ�
	BYTE                                bWaitTime;                          ///�ȴ�ʱ��
	BYTE                                bReserve;                           ///�����ֶ�
	UINT                                iGameRoundCount;                    //��Ϸ����(�����Ӽ��ܰ󶨵�)
	UINT                                iGameRoundMax;                      //��Ϸ������
	/*******************MSG_GAME_RULE**************************************************************/
	int									nCostID;							//��������ID
	__int64                             iGameRule;                          //gamerule
	bool								bAAFanKa;							//�Ƿ�AA�Ʒ���
	int									iCost;								//���ķ���
	/*******************MSG_GAME_RULE**************************************************************/
	__int64                             i64UseDeskLockPassTime;             //���ӵ�ʹ��ʣ��ʱ��
	int                                 iGameStatisticsPoint[MAX_PEOPLE];//ÿ���˿�ʼ���ܷ���
	char                                szPwd[61];                          //����������

	int									iGuildId;							///����Id
	//�����д������ѧ����������³�Ա������һ�ɷ���szMessage����ǰ��
	char                                szMessage[1000];                    ///ϵͳ��Ϣ

	MSG_GM_S_GameInfo()
	{
		ZeroMemory(this, sizeof(MSG_GM_S_GameInfo));
	}
};

///�Թ�����
struct MSG_GM_WatchSet
{
	int                         dwUserID;                           ///���ö���
};



/********************************************************************************************/

///��Ϣ���ݰ�
struct MSG_GA_S_Message
{
	BYTE                                bFontSize;                          ///�����С
	BYTE                                bCloseFace;                         ///�رս���
	BYTE                                bShowStation;                       ///��ʾλ��
	CHAR                                szMessage[1000];                    ///��Ϣ����
};





//��Ϸ������ϢATT
struct GameNoticeMessage
{
	UINT            bDeskIndex;         //���Ӻ�
	BYTE            bAwardCardShape;    //���ͽ���
	TCHAR           szMessage[255];     //δ������Ϸ �ڷ��� �ұ߿���ʾ��������
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
	LONGLONG                            Score;                            //�û�����
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
//�û�������Ϣ��չ
struct UserInfoExpand
{
	int				UserID;
	DOUBLE			iLotteries;       //��ȯ
	int				ExpPlus;
	int				IonPlus;
	int				FallCardPlus;
	int				HongBaoCount;
	int				BasicBonus;
};
//�ܾ����ݽڵ�����ӵı������ݲ���(һ����ҵ�����)
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
	//�ܾ����ݽڵ�(һ����ҵ�����)
	int                                 dwUserID;                           ///�û� ID
	int                                 taiZhuID;                            ///̨��ID
	__int64                             dwPoint;                            ///�û�����ֵ
	__int64                             dwMoney;                            ///�û����
	__int64                             dwSend;                             ///����
	BYTE                                bWinCount;                          ///ʤ��
	BYTE                                bLostCount;                         ///���
	BYTE                                bMidCount;                          ///ƽ��
	BYTE                                bCutCount;                          ///�Ӿ�
	BYTE                                bMaxLzCount;                          ///�����ׯ����

	//�����ֶΣ���������Ϸ����ʹ��
	Game_StatisticsOther                data;

	Game_StatisticsMessage()
	{
		ZeroMemory(this, sizeof(Game_StatisticsMessage));
	}
};
#pragma pack()
//=====================================================================================
#define MDM_GR_WEB_INFO							160							//��ҳ��Ϣ
#define ASS_GP_SEND_EC_LIST						5   ///���е羺�б�
#define ASS_GP_SEND_CLASS_EC_LIST						6	   ///���·���
#define ASS_GP_SEND_SPECIAL_EC_LIST						7	   ///4.1 ����ĳ��Ŀδ��ʼ�ͽ����е�����ר��
#define ASS_GP_SEND_GAMETYPE_EC_LIST						8	   ///����ר���µı����б�
#define ASS_GP_SEND_EVENT_EC_LIST						9	   /// ��ȡĳ��Ŀ�±����б�
#define ASS_GP_SEND_REALTIME_INFO								10	   ///������̬��Ϣ
#define ASS_GP_SEND_END_GAME_LIST								11	   ///����������Ϣ 

#define ASS_GP_SEND_BUY_MONEY								12	   ///��ҳ֪ͨ��ֵ�ɹ� 
#define ASS_GP_LOAD_STOCK								13	   ///���ؿ�� 
#define ASS_GP_CLOSE_ROOM								14	   ///�رշ��� 
#define ASS_GP_WEB_MATCH								15		///����/�رձ�������
#define ASS_GP_GET_REAL_STOCK								16		///��ȡʵʱ�������
#define ASS_GP_GET_ROOMMINSCORE								17		///���¼��ط�����С���
#define ASS_GP_KICK_USEROUT								18		///�߳�����
#define ASS_GP_WEB_SET_AGENTID							19		///�������Ӵ���Id
#define ASS_GP_WEB_SET_CONTROL							20		//���ÿ��Ƶȼ�
#define ASS_GP_WEB_LOAD_ROOM_STOCK_INFO					21		//���¼��ط��������Ϣ
#define ASS_GP_WEB_GAME_INFO							22		//web��ȡ������Ϣ
#define ASS_GM_WEB_GAME_STATION							23		//��Ϸ״̬
#define ASS_WEB_LEFT_DESK								24		//web�뿪����

#define ASS_WEB_USER_SIT								25		//֪ͨweb�������
#define	ASS_WEB_USER_UP									26

#define ASS_WEB_SET_SHI_SHI_CONTROL_INFO				27		//��̨����ʱʱ������Ϣ

//=====================================================================================
#define MDM_GP_EC_INFO							161							//�ͻ��˵羺��Ϣ
#define ASS_GP_EC_LIST							1							//��ȡ���е羺�б�
#define ASS_GP_EC_BET							2							//��ע
#define ASS_GP_EC_RESULT						3							//��ȡս��
#define ASS_GP_SPECIAL_EC_LIST							4							//��ȡ���е羺�б�
#define ASS_GP_CLASS_EC_LIST							5							//��ȡ���·���
#define ASS_GP_GAMETYPE_EC_LIST							6							//����ר���µı����б�
#define ASS_GP_EVENT_EC_LIST						7					/// ��ȡĳ��Ŀ�±����б�
#define ASS_GP_REALTIME_INFO							8				///������̬��Ϣ
#define ASS_GP_ENDGAME							9				///�������
/********************************************************************************************/

///��Ϸ����ʶ



#define ASS_GM_MESSAGE_EX               180                                 ///��Ϸ��Ϣ������ϷATT
/********************************************************************************************/

///������½
#define MDM_GR_LOGON                    100                                 ///������½
#define ASS_GR_LOGON_BY_ID              5                                   ///ͨ���û� ID ��½
#define ASS_GR_LOGON_SUCCESS            2                                   ///��½�ɹ�
#define ASS_GR_LOGON_ERROR              3                                   ///��½ʧ��
#define ASS_GR_SEND_FINISH              4                                   ///��½���
#define ASS_GR_GET_USEREXPAND           6                                   ///��ȡ�û���չ��Ϣ
#define ASS_GR_UPDATE_ROBOT_SCORE       7                                   ///�����˻�ȡ���
#define ASS_GR_LOGON_BY_USERNAME		8                                   ///�����˵�¼
#define ASS_GR_IS_VIPROOM               16                                   ///<�Ƿ�VIP����
#define ASS_GR_VIP_PW                   17                                   ///<VIP������Ҫ����
#define ASS_GR_VIP_NO_PW                18                                   ///< VIP���䲻��Ҫ����(��һ�����벻��Ҫ����)
#define ASS_GR_NO_VIP                   19                                   ///< ����VIP����
#define ASS_GR_VIPROOM_PW               20                                   ///< VIP��������
#define ASS_GR_VIPROOM_PW_RIGHT         21                                   ///< VIP����������ȷ
#define ASS_BEGIN_MATCH_ROOM			22									//ƥ�䷿��
#define ASS_END_MATCH_ROOM				23									//ȡ��ƥ�䷿��
#define ASS_GP_GETROOMINFO				24

#define ERR_GR_FULL_DESK					100000			///�����Ѿ�����
#define ERR_GR_NO_ENOUGH_ROOM_KEY			255				///��ҷ�������
#define ERR_GR_IN_ROOM						254				///���������Ϸ��
#define ERR_GR_GUILD_MATCHERR				253				///���᷿��ƥ�����

////////////////////////////////////////////////////////////////////
///�û��б�
#define MDM_GR_USER_LIST                101                                 ///�û��б�
#define ASS_GR_ONLINE_USER              1                                   ///�����û�
#define ASS_GR_NETCUT_USER              2                                   ///�����û�
#define ASS_GR_DESK_STATION             3                                   ///����״̬
////////////////////////////////////////////////////////////////////
///�û�����
#define MDM_GR_USER_ACTION              102                                 ///�û�����
#define ASS_GR_USER_UP                  1                                   ///�û�����
#define ASS_GR_USER_SIT                 2                                   ///�û�����
#define ASS_GR_WATCH_UP                 3                                   ///�Թ�����
#define ASS_GR_WATCH_SIT                4                                   ///�Թ�����
#define ASS_GR_USER_COME                5                                   ///�û�����
#define ASS_GR_USER_LEFT                6                                   ///�û��뿪
#define ASS_GR_USER_CUT                 7                                   ///�û�����
#define ASS_GR_SIT_ERROR                8                                   ///���´���
#define ASS_GR_SET_TABLE_BASEPOINT      9                                   ///�ı����ӱ���
#define ASS_GR_USER_HIT_BEGIN           10                                  ///�û�ͬ�⿪ʼ
#define ASS_GR_JOIN_QUEUE               11                                  ///<�����Ŷӻ�
#define ASS_GR_QUIT_QUEUE               12                                  ///<�˳��Ŷӻ�
#define ASS_GR_QUEUE_USER_SIT           13                                  ///<�Ŷ��û�����
#define ASS_GR_LEASE_TIMEOVER           14                                  ///<�Ŷ��û�����
#define ASS_GR_SIT_CUTRESIT             16                                  //��������,��������
#define ASS_GR_CONTEST_APPLY            24                                  //����������
	#define HC_GR_CONTEST_APPLY_ALREADY			1	///�ѱ���
	#define HC_GR_CONTEST_APPLY_FULL			2	///������
	#define HC_GR_CONTEST_APPLY_MONEY			3	///��Ҳ���
	#define HC_GR_CONTEST_APPLY_SIGNUPCNT		4	///������������
	#define HC_GR_CONTEST_APPLY_QUITERR			5	///����ʧ��
	#define HC_GR_CONTEST_APPLY_ENABLE			6	///���ιر�
#define ASS_GR_EXPERCISE_OUTTIME		27                                  //��ϰ����ʱ
////////////////////////////////////////////////////////////////////
///������Ϣ
#define MDM_GR_ROOM                     103                                 ///������Ϣ
#define ASS_GR_NORMAL_TALK              1                                   ///��ͨ����
#define ASS_GR_HIGH_TALK                2                                   ///�߼�����
#define ASS_GR_USER_AGREE               3                                   ///�û�ͬ��
#define ASS_GR_GAME_BEGIN               4                                   ///��Ϸ��ʼ
#define ASS_GR_GAME_END                 5                                   ///��Ϸ����
#define ASS_GR_USER_POINT               6                                   ///�û�����ֵ
#define ASS_GR_SHORT_MSG                7                                   ///�Ķ���Ϣ
#define ASS_GR_ROOM_SET                 8                                   ///��������
#define ASS_GR_INVITEUSER               9                                   ///�����û�
#define ASS_GR_INSTANT_UPDATE           10                                  ///��ʱ���·������
#define ASS_GR_UPDATE_CHARM             11                                  ///��ʱ��������
#define ASS_GR_ROOM_PASSWORD_SET        12                                  ///��������
#define ASS_GR_ROOM_QUEUE_READY         13                                  ///�Ŷӻ�׼��
#define ASS_GR_GET_NICKNAME_ONID        14          // ����ID��ȡ�ǳƣ�PengJiLin, 2010-8-23
#define ASS_GR_OWNER_T_ONE_LEFT_ROOM    15          // ����������뿪����, PengJiLin, 2010-8-26
#define ASS_GR_GET_NICKNAME_ONID_INGAME 16          // ����ID��ȡ�ǳ� ��Ϸ�� lxl 2010-11-17
#define ASS_GR_USER_CONTEST             17                                  ///�û�������Ϣ
#define ASS_GP_CONTEST_WEB				18									///����������Ϣ֪ͨ
#define ASS_GR_CONTEST_ROUNDOVER        19                                  ///����һ�ֽ���
#define ASS_GR_CONTEST_END				20                                  ///��������
#define ASS_GR_CONTEST_AWARD            21                                  ///����/��̭
#define ASS_GR_CONTEST_WAIT_GAMEOVER    22                                  ///�����������������û����ڴ����һ�֣�֪ͨ�Ѵ�����û��ȴ�����
#define ASS_GR_CONTEST_UPDATE_RANK      23                                  ///��������
#define ASS_GR_CONTEST_AWARD_REQ        24                                  ///��ȡ������������
#define ASS_GR_CONTEST_SIGNUP_BROAD     25                                  ///�㲥����
#define ASS_GR_CONTEST_NEXT_ROUND       26                                  ///֪ͨ��һ��
#define ASS_GR_HASH				        28                                  ///hashֵ��Ӧ�Ľ��
#define ASS_GR_GET_HASH					29                                  ///��ȡhash��ַ
#define ASS_GR_SEND_HASH				30                                  ///hashֵ
#define ASS_GR_GUILD_DESKINFO			31									///�����ս������Ϣ
#define ASS_GR_GUILD_DESKINFO_UPDATE	32									///�����ս���Ӹ���
#define ASS_GR_USERSCORELOG				33									///��ȡ�û���־
#define ASS_GR_NEWUSERSCORELOG				34									///�µ��û���־




/********************************************************************************************/
#define MDM_GR_MESSAGE                  106                                 ///��Ϣ��Ϣ
///��Ϣ��Ϣ
#define ASS_GR_SYSTEM_MESSAGE           1                                   ///ϵͳ��Ϣ




///

/********************************************************************************************/

///�����Ϣ
#define MDM_GM_GAME_FRAME               150                                 ///�����Ϣ
#define ASS_GM_GAME_INFO				1                                   ///��Ϸ��Ϣ
#define ASS_GM_GAME_STATION             2                                   ///��Ϸ״̬
#define ASS_GM_FORCE_QUIT               3                                   ///ǿ���˳�
#define ASS_GM_NORMAL_TALK              4                                   ///��ͨ����
#define ASS_GM_HIGH_TALK                5                                   ///�߼�����
#define ASS_GM_WATCH_SET                6                                   ///�Թ����� 
#define ASS_GM_CLEAN_USER               9                                   //�����������û���Ϣ
#define ASS_GM_USE_KICK_PROP            7                                   // ʹ�����˿�
#define ASS_GM_USE_ANTI_KICK_PROP       8                                   // ʹ�÷��߿�
#define ASS_GM_SET_VIDEOADDR            10                                  //������Ƶ��������ַ
#define ASS_GM_STATISTICS               11                                  //�ܾ�֪ͨ
#define CODE_GM_STATISTICS_PART         1//һ��������
#define CODE_GM_STATISTICS_FINISH       2//��������
#define ASS_GM_DESKLOCKPASS_UNUSE       12                                  ///���������Ѿ���ʱ�������������е�����뿪������
#define ASS_GM_DESKLOCKPASS_TIMEOUT     13                                  ///�����Ѿ����������δ��ʼ�ˣ��峡����
#define ASS_GM_DESK_SETRULE             14                                  ///���ù���

#define ASS_CONTINUE_NEXT_ROUND			17						//������һ����Ϣ

#define ASS_STORE_SCORE					18						//��Ϸ���
#define ASS_GET_SCORE					19						//��Ϸȡ��
#define	ASS_JIUJI_RESULT				20						//���;ȼý���
#define	ASS_JIUJI_TIPS					21						//���;ȼý���ʾ
#define	ASS_LEVEL_UP					22						//����������ʾ
#define	ASS_GET_TASKLIST				23						//��ȡ�����б�
#define	ASS_UPDATE_TASK					24						//��ȡ������
#define	ASS_GET_TASKNAME				25						//��ȡ��������
#define	ASS_SEND_COMPLETETASK			26						//��ȡ�����������
#define	ASS_SEND_BUYMONEY				27						//��ȡ����ɹ�
#define	ASS_GET_PROP_CARD				28						//��ȡ������
#define	ASS_UPDATE_PROP_CARD			29						//����������
#define	ASS_SYNCHRO_PROP_CARD			30						//���ͬ��������
#define	ASS_RETRY_MATCH					31						//������±���
#define ASS_BUY_PaoTai					32						//��ҹ�����̨

#define ASS_ROOM_GUILD_CHANGE			33						//�����޸�
#define HC_ROOM_GUILD_DELETE				1	///��ɢ
#define HC_ROOM_GUILD_TAXRATE				2	///˰��
#define HC_ROOM_GUILD_CLOSEROOM				3	///�رշ���
#define HC_ROOM_GUILD_KICK					4	///���߳���



/********************************************************************************************/
#define MDM_GM_MESSAGE                  151                                 ///��Ϣ��Ϣ
///��Ϣ��Ϣ
#define ASS_GM_SYSTEM_MESSAGE           1                                   ///ϵͳ��Ϣ

#define ASS_GM_SYSTEM_MESSAGE_EX        2                                   ///ϵͳ��Ϣ������Ϸר�ã�ATT��

/********************************************************************************************/
///��Ϸ��Ϣ

#define MDM_GM_GAME_NOTIFY              180                                 ///��Ϸ��Ϣ
#define ASS_GM_AGREE_GAME               1                                   ///ͬ����Ϸ

#define ASS_GR_CHANGE_OWNER			    3                                   //�л�����
#define ASS_GR_USERINFO_GET			    4                                   //��ȡ�û���Ϣ

/********************************************************************************************/
#define MDM_GM_GAME_ADMIN				181                                 ///��Ϸ��Ϣ
#define ASS_ADMIN_USER_LIST_REQ			1									//
#define ASS_ADMIN_USER_LIST_ACK			2									//
#define ASS_ADMIN_GAME_SCENE			3									//
#define ASS_ADMIN_GAME_RECORD			4									//

/********************************************************************************************/

///�������
#define ERR_GR_ERROR_UNKNOW             0                                   ///δ֪����
#define ERR_GR_LOGON_SUCCESS            1                                   ///��½�ɹ�
#define ERR_GR_USER_NO_FIND             2                                   ///�û�������
#define ERR_GR_USER_PASS_ERROR          3                                   ///�û��������
#define ERR_GR_USER_VALIDATA            4                                   ///�û��ʺŽ���
#define ERR_GR_USER_IP_LIMITED          5                                   ///��½ IP ��ֹ
#define ERR_GR_IP_NO_ORDER              6                                   ///����ָ����ַ
#define ERR_GR_ONLY_MEMBER              7                                   ///��Ա��Ϸ����
#define ERR_GR_IN_OTHER_ROOM            8                                   ///������������
#define ERR_GR_ACCOUNTS_IN_USE          9                                   ///�ʺ�����ʹ��
#define ERR_GR_PEOPLE_FULL              10                                  ///�����Ѿ���
#define ERR_GR_LIST_PART                11                                  ///�����û��б�
#define ERR_GR_LIST_FINISH              12                                  ///ȫ���û��б�
#define ERR_GR_STOP_LOGON               13                                  ///��ͣ��½����

#define ERR_GR_DESK_NULL	            20
#define ERR_GR_GUILDERR					21									///�������
#define ERR_GR_GUILD_ROOMERR			22									///���᷿�����
#define ERR_GR_CONTESTERR				23									///������Ϣ����



#define ERR_GR_MATCH_LOGON              160                                 ///��Ϸ����
#define ERR_GR_TIME_OVER                161                                 ///ʱ�䵽��

///wushuqun 2009.6.5
///���ڻ�ս���ʱ����
#define ERR_GR_BATTLEROOM_OUTTIME       162

///�û����´�����
#define ERR_GR_SIT_SUCCESS              50                                  ///�ɹ�����
#define ERR_GR_BEGIN_GAME               51                                  ///��Ϸ�Ѿ���ʼ
#define ERR_GR_ALREAD_USER              52                                  ///�Ѿ����˴���
#define ERR_GR_PASS_ERROR               53                                  ///�������
#define ERR_GR_IP_SAME                  54                                  ///IP ��ͬ
#define ERR_GR_CUT_HIGH                 55                                  ///������̫��
#define ERR_GR_POINT_LOW                56                                  ///����ֵ̫��
#define ERR_GR_POINT_HIGH               57                                  ///����ֵ̫��
#define ERR_GR_NO_FRIEND                58                                  ///���ܻ�ӭ
#define ERR_GR_POINT_LIMIT              59                                  ///����ֵ����
#define ERR_GR_CAN_NOT_LEFT             60                                  ///�����뿪
#define ERR_GR_NOT_FIX_STATION          61                                  ///������λ��
#define ERR_GR_MATCH_FINISH             62                                  ///��������
#define ERR_GR_MONEY_LIMIT              63                                  ///���̫��
#define ERR_GR_MATCH_WAIT               64                                  ///�������Ŷ���ʾ
#define ERR_GR_IP_SAME_3                65                                  ///IPǰ3 ��ͬ
#define ERR_GR_IP_SAME_4                66                                  ///IPǰ4 ��ͬ
#define ERR_GR_UNENABLE_WATCH           67                                  ///�������Թ�
#define ERR_GR_DESK_FULL                68                                  /// �ټ��� ������λ���ˣ��޷�������λ����� add by wlr 09.07.14
#define ERR_GR_WAIT_DESK_RECYLE         69                                  ///���ӻ�û�л��գ���ʱ������

///����뿪��ʶ
#define ERR_GR_USER_UP_NOMAL			0									///�����뿪
#define ERR_GR_USER_UP_MONEY			1									///��Ҳ���
#define ERR_GR_USER_UP_GUILD_KICK		2									///VIP������
#define ERR_GR_USER_UP_NOACTION			3									///��ʱ��δ����
#define ERR_GR_USER_UP_REMATCH			4									///����ƥ��
#define ERR_GR_USER_UP_ROOMCLOSE		5									///VIP������ر�
#define ERR_GR_USER_UP_RECORD_INFO    6                  //��ұ�������뿪




/********************************************************************************************/

#endif
