/**
* ��Ϣ������ GameMessage.h   
*
* @author duanxiaohui
* @version 1.0
* @date 2009.12.15
*/

#pragma once

#ifndef EZP_GAMEMESSAGE_H
#define EZP_GAMEMESSAGE_H

/** ������� */
#define  EZP_PLAY_COUNT              180

/** ��Ϣ���� �������ͻ�����Ϣ 160 ~ 200 */
/** ���������ׯ */
#define  EZP_MSG_APPLYZHUANG         160

/** �����ע */
#define  EZP_MSG_XIAZHU              161

/** ��Ϸ��ʼ */
#define  EZP_MSG_PLAY_GAME           162

/** ���� */
#define  EZP_MSG_SENDCARD            163

/** ������ׯʧ�� */
#define  EZP_MSG_APPLYZHUANG_ERR     164

/** ��ע��ʾ */
#define  EZP_MSG_XIAZHU_HINT         165

/** ���ӵ�е����� */
#define  GAMER_CARD  2

/** �������� */
#define  PAI_COUNT   32

/** һ��ϴ�ƴ��� */
#define  GAME_XIPAI_COUNT  72

/** ��ֵ��Сֵ */
#define  PAI_VALUE_MIN      1

/** ��ֵ���ֵ */
#define  PAI_VALUE_MAX      55

/** ��ׯ��Ҳ������� */
#define  LESS_APPLYZHUANG_MONEY_ERR  0x01

/** �ظ�������ׯ���� */
#define  REPEAT_APPLAYZHUANG_ERR     0x02

/** ������ׯ��Ϊ��ǰׯ�� */
#define  CUR_ZHUANG_ERR              0x03

/** �������� */
#define CHOUMA_KIND_COUNT   6

/** ������ֵ */
//static const int g_ChoumaValue[CHOUMA_KIND_COUNT] = {100, 1000, 10000, 100000, 1000000, 10000000};

/** ��ֵ�Ƿ���ȷ */
#define  PAI_IS_VALUE(paiKey)  ((paiKey >= PAI_VALUE_MIN && paiKey <= PAI_VALUE_MAX) ? TRUE : FALSE)

/// ������е������ݽṹ
struct PaiData
{
	BYTE   paiAry[GAMER_CARD];  /**< ������ */
	PaiData()
	{
		memset(paiAry,0,sizeof(paiAry));
	}
	PaiData(PaiData & paiData)
	{
		memcpy(paiAry,paiData.paiAry,sizeof(paiData.paiAry));
	}
	PaiData(BYTE first,BYTE second)
	{
		if (PAI_IS_VALUE(first) && PAI_IS_VALUE(second))
		{
			paiAry[0] = first;
			paiAry[1] = second;
			return;
		}
		else
		{
			memset(paiAry,0,sizeof(paiAry));
			return;
		}
	}
	void operator=(PaiData & paiData)
	{
		paiAry[0] = paiData.paiAry[0];
		paiAry[1] = paiData.paiAry[1];
	}
	BOOL operator==(PaiData paiData)
	{
		if (paiAry[0] == paiData.paiAry[0] && paiAry[1] == paiData.paiAry[1])
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
};

/// ��ע������Ϣ�ṹ
struct EZP_Xiazhu_Data
{
	__int64 iXiaZhu[EZP_PLAY_COUNT][6];    /**< 0˳�ţ�1˳��-�ǣ�2���ţ�3�ţ�4���ţ�5����-�� */
	__int64 iDeskInfo[6];                  /**< �������������������Ϣ */
	__int64 iRobotZhu[6];                  /**< ������ÿ�������µ�ע�� */
	__int64 iAllMoney[EZP_PLAY_COUNT];     /**< ����ܵ���ע�� */
	BOOL    bChipFlag[EZP_PLAY_COUNT];     /**< �����ע��־ TRUE: ��ʾ���������ע FALSE: �����û����ע */

	void Init()
	{
		memset(&iXiaZhu, 0, sizeof(iXiaZhu));
		memset(&iDeskInfo, 0, sizeof(iDeskInfo));
		memset(&iRobotZhu, 0, sizeof(iRobotZhu));
		memset(&iAllMoney, 0, sizeof(iAllMoney));
		memset(&bChipFlag, 0, sizeof(bChipFlag));
	}
};

/// ������ׯ��Ϣ�ṹ
struct EZP_APPLY_ZHUANG
{
	BYTE bDeskStation;  /**< ������ׯ�� */
	BOOL bFirst;        /**< �Ƿ��һ��������ׯ�� */
	BOOL bFlag;         /**< TRUE: ��ׯ FALSH: ��ׯ */
	BOOL bGoto;         /**< ֱ����ׯ������� */
	BOOL bIsRobot;      /**< �Ƿ�Ϊ�����˽�ׯ */
};

/// ������ׯ������Ϣ�ṹ
struct EZP_APPLY_ZHUANG_ERR
{
	BYTE bErrorCode;   /**< ������ */
};

/// ��ע��Ϣ��Ϣ�ṹ
struct EZP_XIAZHU_INFO
{
	BYTE bDeskStation; /**< ��ע�� */
	BYTE bType;        /**< ��ע���� */
	BYTE bMoneyType;   /**< ������� */
	__int64  iMoney;   /**< ��ע��� */
	BOOL bIsRobot;     /**< ��־��ע����Ƿ�Ϊ������ */		
};

/// ��ע��Ϣ��ʾ
struct EZP_XIAZHU_HINT
{
	BYTE bState;       /**< ��ע��ʾ��־ */	
};

/// ��Ϸ��ʽ��ʼ��Ϣ�ṹ
struct EZP_PLAY_GAME
{
	BYTE bCard[1];     /**< ������ */
};

/// ����һ���ƣ����ַ���λ����Ϣ�ṹ
struct EZP_SEND_CARD
{
	BYTE      bCard;		 /**< ���ַ��ƿ�ʼλ�õ��� */
	PaiData   userCard[4];   /**< ���������ĵ��� */
};

/// ��ʼ������Ϣ�ṹ
struct GameFaPaiData
{
	BOOL       bUseCard;       /**< TRUE: ��Ϸ��ʹ�ú�A  FALSE: ��Ϸ��ʹ�ô��� */
	BYTE       bRandCard[1];   /**< ���һ���ƣ�ȷ�Ϸ���λ�� */
	BYTE       bStartSendPos;  /**< ��ʼ����λ�� */
	PaiData    userCard[4];    /**< ÿ����ҵ���,ÿ��Ԫ�ض�Ӧ station �е�λ��*/
	BYTE       byDiceNum[2] ;  ///ɫ�ӵ���
};

/// �������ƽṹ��
typedef struct PaiValue
{
	char iValue[10];   /**< ��ֵ */
	void SetValue(char * str)
	{
		strcpy(iValue,str);
	}
	char * GetValue()
	{
		return iValue;
	}
	PaiValue()
	{
		memset(iValue,0,sizeof(iValue));
	}
};

/// ��Ϸ����������Ϣ�ṹ
struct GameResultData
{
	int        userPaiDian[4];							/**< �Ƶ��� */
	PaiValue   paiValue[4];									/**< �������� */
	PaiData    userData[4];                 /**< ÿ����ҵ��� */

	__int64    iSelfMoney;                  /**< ������Ӯ��� */
	__int64    iZhuangMoney;                /**< ����ׯ����Ӯ��� */
	__int64    iReturnMoney;								/**< ���ط� */
	__int64    dwMoney;                     /**< ���˽��, �ͻ��˲��ܼ�ʱˢ�½�ң����Դӷ������Ϸ���ȥ */
	__int64    dwZhuangMoney;               /**< ׯ�ҽ��, �ͻ��˲��ܼ�ʱˢ�½�ң����Դӷ������Ϸ���ȥ */
	BYTE       bKaiPai;                     /**< ˳���ԡ����ſ��� */

	GameResultData()
	{
		bKaiPai = 0;
		memset(paiValue, 0, sizeof(paiValue));
		memset(userPaiDian, 0, sizeof(userPaiDian));
	}
};

/// ��Ϸ������ʤ��������Ϣ�ṹ
struct GameEndData
{
	BOOL       bAutoZhuangXZ;               /**< ׯ�ҽ�Ҳ�����ׯ��������ׯ���Զ���ׯ */
	BOOL       bZhuangXZ;                   /**< ׯ���Լ�������ׯ�˻���ׯ������ */
	BOOL       bRobortCanApply;							/**< �������Ƿ����������ׯ */

	GameEndData()
	{
		::memset(this,NULL,sizeof(GameEndData));
	}
};

/// ÿ����Ϸ���ơ���ע��Ϣ��Ϣ�ṹ
struct GameInfoList
{
	BYTE  bKaiPai;        /**< �����Ƿ�(0x01:˳�š�0x02:���š�0x04:����) */
	BYTE  bIsXiaZhu;      /**< �����Ƿ���ע(0x01:˳�š�0x02:���š�0x04:����) */

	GameInfoList()
	{
		bKaiPai = 0;
		bIsXiaZhu = 0;
	}
};

/// ·����Ϣ
struct GameLuziInfo
{
	BYTE bKaiPai;         /**< ������Ϣ */
	char chGameBeen;      /**< ��ǰ�ڼ��� */
	char chGameCount;     /**< ��ǰ�ڼ��� */

	GameLuziInfo()
	{
		bKaiPai = 0;
		chGameBeen = 0;
		chGameCount = 0;
	}
};

#endif