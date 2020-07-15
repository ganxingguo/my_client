#pragma once

#ifndef BZGAME_PROP_MESSAGE_HEAD_FILE
#define BZGAME_PROP_MESSAGE_HEAD_FILE


/*
�ļ�˵�������ļ���������IM������ص���Ϣ�ͽṹ��
����ƽ̨ʱ����Ҫ�ڸ���Ŀstdafx.h�а������ļ�
�ļ�������Fred Huang 2008-08-08
��Ϣ��ʽ��MDM_		��ʾ����Ϣ
	ASS_PROP	��ʾ��IM�������Ϣ
	ASS_PROP	��ʾ�ɿͻ�����������˷��͵���Ϣ
	ASS_PROP	��ʾ�ɷ���������ͻ��˷��͵���Ϣ
�ṹ��ʽ��MSG_		��ʾ����Ϣ�ṹ��
	MSG_PROP	��ͷ����ص���Ϣ�ṹ
	MSG_PROP_C_ �ɿͻ��˷����������˵���Ϣ�ṹ��
	MSG_PROP_S_ �ɷ������˷����ͻ��˵���Ϣ�ṹ��
*/

//����Ϣ
#define MDM_GP_PROP						140					//�����е��������Ϣ
#define MDM_GR_PROP						165					//�������Ϸ�е�����ص���Ϣ

#define BRD_MSG_BIG						0x1					//������
#define BRD_MSG_SMALL					0x2					//С����


//��������Ϣ
#define ASS_PROP_GETUSERPROP			0x01				//ȡ�÷������ϴ洢�ĸ��˵���
#define ASS_PROP_GETPROPINFO			0x02				//ȡ��ĳ�����ߵ���Ϣ
#define ASS_PROP_BUY					0x03				//�������
#define ASS_PROP_GIVE					0x04				//���͵���
#define ASS_PROP_VIP_CHECKTIME			0x05				//���VIPʱ��
#define ASS_PROP_KICKUSER				0x06				//����
#define ASS_PROP_BIG_BOARDCASE			0x07				//��������Ϣ
#define ASS_PROP_SMALL_BOARDCASE		0x08				//С������Ϣ

// PengJiLin, 2010-9-13, ʹ�����˿�����
#define ASS_PROP_NEW_KICKUSER           0x09

// PengJiLin, 2010-9-14, ʹ�����˿�������Ϣ���
#define ASS_PROP_NEW_KICKISER_INFO          0x0a

// PengJiLin, 2010-10-13, �̵���ߵļ�ʱ����
#define ASS_PROP_BUY_NEW				0x0b

//��Ϸ����Ϣ
#define ASS_PROP_USEPROP				0x10				//ʹ�õ���

//���߲��������Ϣ                                          //add wyl 
#define ASS_PROP_RESULT					0x11				//���߹�����

#define ASS_PROP_BUY_VIP				0x12				//����VIP

#define ASS_PROP_GIVE_VIP				0x13				//����VIP


#define ASS_PROP_BIG_BOARDCASE_BUYANDUSE	0x14			//��ʱ��������Ⱥ�ʹ��


#define DTR_GR_PROP_USE_ERR_PARAM_INVALID	10
#define DTR_GR_PROP_USE_ERR_UNKNOW			20
#define DTR_GR_PROP_USE_ERR_NOPROP			30
#define DTK_GR_PROP_USE_SUCCEED				80


#define DTK_GP_PROP_BUY_ERROR				10
#define DTK_GP_PROP_BUY_NOMONEY				20
#define DTK_GP_PROP_BUY_SUCCEED				80
#define DTK_GP_PROP_BUYANDUSE_SUCCEED		81

#define DTK_GP_PROP_GIVE_ERROR				10
#define DTK_GP_PROP_GIVE_NOENOUGH			20
#define DTK_GP_PROP_GIVE_FAILEDSEND			50
#define DTK_GP_PROP_GIVE_NOTARGETUSER		60
#define DTK_GP_PROP_GIVE_SUCCEED_BEGIVER	70
#define DTK_GP_PROP_GIVE_SUCCEED_GIVER		80

#define DTK_GP_VIP_BUY_ERROR				10
#define DTK_GP_VIP_BUY_NOMONEY				20
#define DTK_GR_VIP_BUY_SUCCEED				30
#define DTK_GR_VIP_BUY_NOUSER				40
#define DTK_GR_VIP_GIVE_SUCCESS				50

#define MAX_BLOCK_PROP_SIZE				512					//ÿ����Ϣ���������

#define BROADCAST_MESSAGE_MAX			255					//�㲥��Ϣ����


#pragma pack(1)
//=======================���ݽṹ����=====================
/*
�ṹ��ȡ�÷������ϴ洢�ĸ��˵���
���ݣ��û���ID����ֹ���󣬿��Բ�Ҫ�ģ������غ�����

*/

typedef struct  
{
	long		dwUserID;						//�û�ID

}MSG_PROP_C_GETSAVED;

/*
�ṹ���ӷ�������ȡ�õĸ��˵��ߣ�ÿһ������һ����Ϣ��
      ���ش�ŵĸ��˵�������
*/
typedef struct  
{
	long		dwUserID;						//�û�ID��
	int			nPropID;						//����ID��
	int			nHoldCount;						//ӵ�е��ߵ�����
	TCHAR		szPropName[50];					//��������
	int			attribAction;					//��������
	int			attribValue;					//ȡֵ����
	int			dwCost;							//��ǰ�ñʽ��׻��˶���Ǯ����ZXD��ӣ����������ṩ������
	int			iPrice;							//���߼۸�			
	int			iVipPrice;						//VIP���߼۸�
}MSG_PROP_S_GETPROP,_TAG_USERPROP;

struct  UseBroadResult
{
	int		UserID;
	int     CostScore;
	int		ret;
	CHAR								szMessage[BROADCAST_MESSAGE_MAX+1];//��������
};
/*
�ṹ���û�ʹ�õ��ߵ���Ϣ�ṹ�壬˳����һЩ�������ص�����
*/
typedef struct  
{
	long		dwUserID;						//ʹ�õ��ߵ��û�ID
	long		dwTargetUserID;					//ʹ�õ���ʱ�Ķ����û�ID���������Լ�
	int			nPropID;						//����ID
	int			nPropActionAttrib;				//���߲�������
	int			nPropValueAttrib;				//����ȡֵ����
	int			iDoubleTime;					//˫������ʱ��
	int			iProtectTime;					//�����ʱ��
	TCHAR		szPropName[50];					//��������
}_TAG_USINGPROP;

//�û�����VIP���ص��������     add by wyl   11-5-21
typedef struct  
{
	TCHAR		szTargetUser[32];				//���Ͷ�����ǳ�
	long		dwUserID;						//�û�ID��
	int			nPropID;						//����ID��
	int			nHoldCount;						//ӵ�е��ߵ�����
	TCHAR		szPropName[50];					//��������
	int			attribAction;					//��������
	int			attribValue;					//ȡֵ����
	int			dwCost;							//��ǰ�ñʽ��׻��˶���Ǯ����ZXD��ӣ����������ṩ������
}MSG_PROP_S_BUYVIP,_TAG_VIPBUG;

/*
�ṹ�����ж���������£���ͬ���û����͵���ʹ����Ϣ
*/
typedef struct  
{
	long	dwUserID;
	long	dwTargetUserID;
	int		nUserStation;
	int		nTargetUserStation;
	int		nPropID;
	int		nPropActionAttrib;
	int		nPropValueAttrib;
}_TAG_PROP_MOIVE_USE;

struct _TAG_PROP_BUY
{
	long	dwUserID;					//������ID
	int		nPropID;					//����ID
	long	iPropPayMoney;				//�ܹ��Ľ��
	int		nPropBuyCount;				//��������
	bool	bUse;

	_TAG_PROP_BUY()
	{
		ZeroMemory(this, sizeof(_TAG_PROP_BUY));
		bUse = false;
	}
};


typedef struct    //add by wyl  for VIP 
{
	long	dwUserID;					//������ID
	int		nPropType;					//��������
	TCHAR	szTargetUser[32];			//���Ͷ�����ǳ�
}_TAG_PROP_BUY_VIP;


typedef struct  
{
	long	dwUserID;					//���͵��û�ID
	TCHAR	szUser[32];					//�����ߵ��ǳ�
	TCHAR	szTargetUser[32];			//���Ͷ�����ǳ�
	int		nPropID;					//���͵ĵ���ID
	int		nGiveCount;					//���͵ĵ�������
	int		iPropValue;					//���߼�ֵ
}_TAG_PROP_GIVE;

typedef struct  
{	
	_TAG_PROP_GIVE	propGiveInfo;		//����������Ϣ
	_TAG_USERPROP propInfo;				//���߾�����Ϣ
}_TAG_PROP_GIVE_FOR;

typedef struct  
{
	int									nPropID;							//����ID��
	int									dwUserID;							//�û�ID
	COLORREF							crColor;							//������ɫ
	SHORT								iLength;							//��Ϣ����
	CHAR								szUserName[32];						//�û�����;		
	CHAR								szMessage[BROADCAST_MESSAGE_MAX+1];//��������
} _TAG_BOARDCAST;

struct BOARDCAST  
{
	int									dwUserID;							//�û�ID	
	CHAR								szMessage[BROADCAST_MESSAGE_MAX+1];//��������
} ;
//vip ʱ����
typedef struct
{
	long int						dwUserID;								//���ID
	int								iVipTime;								//vipʱ��
	int								iDoublePointTime;					//˫������ʱ��
	int								iProtectTime;						//�����ʱ�䣬����
}_TAG_PROP_TIME;

typedef struct
{
	long int						dwUserID;								//���ID
	long int						dwDestID;								//Ŀ������
}_TAG_KICKUSER;

// PengJiLin, 2010-9-13, ʹ�����˿�����
typedef struct
{
    long int  dwUserID;
    long int  dwDestID;
}_TAG_NEW_KICKUSER;

typedef struct 
{
	long		dwUserID;						//ID
	char		sNickName[61];					//�ǳ�
}MSG_IM_C_GETFRIENDLIST,MSG_IM_S_GETFRIENDLIST;


struct TPropChange
{
	int	iUserId;
	int iPropId;
	int iPropCount;
	int iOpType;		//1��ʾ��ȡ,2��ʾ����(��ʾ)
};
#pragma pack()
#endif

