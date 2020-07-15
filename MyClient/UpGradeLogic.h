#pragma once

//#include "StdAfx.h"

//��������
#define	UG_HUA_MASK					0xF0			//1111 0000
#define	UG_VALUE_MASK				0x0F			//0000 1111

//�˿˻�ɫ
#define UG_FANG_KUAI				0x00			//����	0000 0000
#define UG_MEI_HUA					0x10			//÷��	0001 0000
#define UG_HONG_TAO					0x20			//����	0010 0000
#define UG_HEI_TAO					0x30			//����	0011 0000
#define UG_NT_CARD					0x40			//����	0100 0000
#define UG_ERROR_HUA				0xF0			//����  1111 0000

//�˿˳�������
//#define SH_BLACK_JACK               10              //�ڽܿ�
//#define SH_THREE					7				//����
//#define SH_SAME_HUA_CONTINUE		6				//ͬ��˳
//#define SH_SAME_HUA					5				//ͬ��
//#define SH_CONTINUE					4				//˳��
//#define SH_DOUBLE					3				//����
//#define SH_SPECIAL					1				//����235
//#define SH_OTHER					2				//����
//#define UG_ERROR_KIND				0				//����
//ţţ����
#define UG_NO_POINT					0x00			//�޵���
#define UG_BULL_ONE                 0x01            //ţһ
#define UG_BULL_TWO                 0x02            //ţ��
#define UG_BULL_THREE               0x03            //ţ��
#define UG_BULL_FOUR                0x04            //ţ��
#define UG_BULL_FIVE                0x05            //ţ��
#define UG_BULL_SIX                 0x06            //ţ��
#define UG_BULL_SEVEN               0x07            //ţ��
#define UG_BULL_EIGHT               0x08            //ţ��
#define UG_BULL_NINE                0x09            //ţ��
#define UG_BULL_BULL				0x0A			//ţţ
#define UG_BULL_SILVER              0X0B            //��ţ
#define UG_BULL_GOLD                0X0C            //��ţ
#define UG_FIVE_SMALL               0X0D            //��С
#define UG_BULL_BOMB                0X0E            //ը��
/*#define SH_OTHER					1				//ɢ��
#define SH_DOUBLE					2				//����
#define SH_TWO_DOUBLE				3				//����
#define SH_THREE					4				//����
#define SH_CONTINUE					5				//˳��
#define SH_SAME_HUA					6				//ͬ��
#define SH_HU_LU					7				//��«
#define SH_TIE_ZHI					8				//��֧
#define SH_SAME_HUA_CONTINUE		9				//ͬ��˳
*/
//�����߼��� 
class CUpGradeGameLogic
{
	//��������
private:
	//BYTE					m_iNTNum;							//��������
	//BYTE					m_iNTHuaKind;						//���ƻ�ɫ
	int						m_iStation[5];						//���λ�ã����飬÷�������ң����ң����ƣ�
	
	//CPtrList				m_ptrPSList;//���б�

	DWORD					m_iCardShape;	//֧������

	BYTE					m_iReplaceCardArray[54];		  /**<���������*/

	BYTE					m_iReplaceCardCount;		      /**<����������L��*/

	//struct PSTypeNode
	//{
	//	int mps,mcount;

	//	PSTypeNode()
	//	{
	//		mps = -1;
	//		mcount = 0;
	//	}
	//};
	//��������
public:
	//���캯��
	CUpGradeGameLogic(void);

	//���ܺ���������������
public:
	///�O�ô��������
	BOOL SetReplaceCard(BYTE iCardList[],int iCardCount);

	///��ȡ�����пɴ���������
	int GetReplaceCardCount(BYTE iCardList[],int iCardCount);

	///��ȡָ���Ƹ���
	BYTE GetCountBySpecifyCard(BYTE iCardList[],int iCardCount,BYTE bCard);
	
	//��ȡ�˿�����
	inline int GetCardNum(BYTE iCard) { return (iCard&UG_VALUE_MASK)+1; }
	//��ȡ�˿˻�ɫ
	BYTE GetCardHuaKind(BYTE iCard);
	//�����˿�
	BOOL SortCard(BYTE iCardList[], BYTE bUp[], int iCardCount);
	//ɾ���˿�
	int RemoveCard(BYTE iRemoveCard[], int iRemoveCount, BYTE iCardList[], int iCardCount);
		//���Ƶ�
	//int GetCardPoint(BYTE Card[], int CardCount, bool split);
	//��
	int GetPoint(int Card);

	//��������
	BOOL ReSortCard(BYTE iCardList[], int iCardCount);
	//����==iCard�ĵ�������iCardList�е����(��ʼλ��,���K�cλ��)
	int GetSerialBySpecifyCard(BYTE iCardList[],int iStart,int iCardCount,BYTE iCard);

	//��������
public:
	//�Ƿ���
	inline BOOL IsOnlyOne(BYTE iCardList[], int iCardCount) { return iCardCount==1; };
	//�Ƿ����
	BOOL IsDouble(BYTE iCardList[], int iCardCount);
	//�Ƿ�����
	BOOL IsThree(BYTE iCardList[], int iCardCount);
	
	//�õ����л�ɫ
	BYTE GetCardListHua(BYTE iCardList[], int iCardCount);
    //��ȡ����
	BYTE GetCardShape(BYTE iCardList[], int iCardCount);
	//��ȡ�˿���Դ�С
	int GetCardBulk(BYTE iCard, BOOL bExtVol = false);
	//�Աȵ���
	BOOL CompareOnlyOne(BYTE iFirstCard, BYTE iNextCard);

	//�Ƿ�����һ
	//BOOL IsThreeAndOne(BYTE iCardList[], int iCardCount);
	//�Ƿ�˳��
	BOOL IsSingleContinue(BYTE iCardList[], int iCardCount);
//	//�Ƿ�����
//	BOOL IsCompleDouble(BYTE iCardList[], int iCardCount);
	//�Ƿ�ͬ��
	BOOL IsSameHua(BYTE iCardList[], int iCardCount);
//	//�Ƿ��«
//	BOOL IsHuLu(BYTE iCardList[], int iCardCount);
//	//�Ƿ���֧
//	BOOL IsTieZhi(BYTE iCardList[], int iCardCount);
	//�Ƿ�ͬ��˳
	BOOL IsSameHuaContinue(BYTE iCardList[], int iCardCount);
	//��������
	BOOL IsSpecial(BYTE iCardList[],int iCardCount);
	//�Ƿ�ڽܿ�
	BOOL IsBlackJack(BYTE iCardList[], int iCardCount,bool Split);
	//�Ƿ�˫˳
	//BOOL IsDoubleContinue(BYTE iCardList[], int iCardCount);
	//�Ƿ���˳
	//BOOL IsThreeContinue(BYTE iCardList[], int iCardCount);
	//�Ƿ�ɻ������
	//BOOL IsPlaneAndWing(BYTE iCardList[], int iCardCount);
	//�Ƿ��Ĵ���
	//BOOL IsFourAndTwo(BYTE iCardList[], int iCardCount);
	//�Ƿ�ը��
	//BOOL IsBomb(BYTE iCardList[], int iCardCount);
	//�Ƿ���
	//BOOL IsRocket(BYTE iCardList[], int iCardCount);
	//�Ƿ�510K
	//BOOL Is510KBomb(BYTE iCardList[], int iCardCount);
	//�Ƿ�ͬ��
	//BOOL IsSameHua(BYTE iCardList[], int iCardCount);

	//��������
public:
	//��ȡ�˿�
	BYTE GetCardFromHua(int iHuaKind, int iNum);
	//���ҷ���
	//int FindPoint(BYTE iCardList[], int iCardCount);

	//������ר�ú���
public:
	//�����˿�
	BYTE RandCard(BYTE iCard[], int iCardCount,bool bhaveKing=true);

	//�ͻ��˺���
public:
	//�Զ����ƺ���
	BOOL AutoOutCard(BYTE iHandCard[], int iHandCardCount, BYTE iBaseCard[], int iBaseCardCount, BYTE iResultCard[], int & iResultCardCount, BOOL bFirstOut);
	//�Ƿ���Գ���
	BOOL CanOutCard(BYTE iOutCard[], int iOutCount, BYTE iBaseCard[], int iBaseCount, BYTE iHandCard[], int iHandCount);

	bool IsVariationShape(BYTE  iCardList[],int iCardCount,BYTE iUpCard[],int iUpCount);

	//�Ƚ������ƴ�С
	int CompareCard(BYTE iFirstCardList[], int iFirstCardCount,BYTE iFirstUpCard[], BYTE iSecondCardList[], int iSecondCardCount,BYTE iSecondUpCard[]);
		//�Ƚ������ƴ�С
	
	//int CompareCard(BYTE iFirstCardList[], int iFirstCardCount, BYTE iSecondCardList[], int iSecondCardCount);
	
	//��ȡ������3���Ƶ����������� ����������
	int GetRemainCard(BYTE iCardList[],int iCardCount,BYTE iUpCard[],int iUpCount,BYTE byOtherCard[]);

	//�Ƿ������ȡ��������20,10,0��
	bool CanSumIn(BYTE iUpCard[],int iUpCount=3);

	//�õ���ǰ����
	int GetShape(BYTE iCardList[],int iCardCount, BYTE iUpCard[]);

	////�õ���ǰ����
	//int GetShape(BYTE iCardList[],int iCardCount);

	//��ţ
	bool IsBull(BYTE iCardList[],int iCardCount);

	//�������еĵ���
	int CountPoint(BYTE  iCardList[],int iCardCount);

	// ���ţ������
	BOOL GetBull(BYTE iCardList[], int iCardCount, BYTE iCardResult[]);

	///������Ϸ����
	void SetCardShape(DWORD iCardShape){m_iCardShape=iCardShape;}

	//�ڲ�����
private:
	///��� 0 λ�˿�
	int RemoveNummCard(BYTE iCardList[], int iCardCount);

	///ţţ
	bool IsBullBull(BYTE  iCardList[],int iCardCount,BYTE iUpCard[]);

	/// ��ţ
	bool IsGoldBull(BYTE  iCardList[],int iCardCount=5);

	/// ��ţ
	bool IsSilverBull(BYTE  iCardList[],int iCardCount=5);

	/// ը��ţ
	bool IsBombBull(BYTE  iCardList[],int iCardCount=5);

	/// ��С
	bool IsFiveSmall(BYTE  iCardList[],int iCardCount=5);

	int IsHaveNote(BYTE  iCardList[],int iCardCount,BYTE iUpCard[],int iUpCount);

	/// �õ�������������
	int GetMaxCard(BYTE  iCardList[],int iCardCount=5);
	
};
