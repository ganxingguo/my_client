#include "StdAfx.h"
#include "UpGradeLogic.h"
#include "list"
using namespace std;

#include "UpgradeMessage.h"

//���캯��
CUpGradeGameLogic::CUpGradeGameLogic(void)
{
	//m_iNTNum=0;
	//m_iNTHuaKind=UG_ERROR_HUA;
	m_iStation[4]=500;

	for(int i=0; i < 4; i++)
	{
		m_iStation[i]=100*i;
	}

	//�����ƶ���
	::memset(m_iReplaceCardArray,0,sizeof(m_iReplaceCardArray));

	m_iReplaceCardArray[0] = 0x4E;	//С��
	m_iReplaceCardArray[1] = 0x4F;	//���

	m_iReplaceCardCount = 2;		//�����ƶ�������
}

///���ô�����ƶ���
///@param BYTE iCardList[] [in] �ƶ���9
///@param int iCardCount [in]   �Ƶ�����
///@return ��������ֵΪTRUE
BOOL CUpGradeGameLogic::SetReplaceCard(BYTE iCardList[],int iCardCount)
{
	ZeroMemory(m_iReplaceCardArray, sizeof(m_iReplaceCardArray));
	::CopyMemory(m_iReplaceCardArray,iCardList,iCardCount);
	m_iReplaceCardCount = iCardCount;
	return TRUE;
}

///��ȡ�����пɴ���������
///@param BYTE iCardList[] [in]�˿˶���
///@param int iCardCount   [in] �˿˵�����
///@return ����������ȡ���˿�����
int CUpGradeGameLogic::GetReplaceCardCount(BYTE iCardList[],int iCardCount)
{
	int iResultCount = 0;

	for(int i = 0;i < m_iReplaceCardCount;i++)
	{	
		iResultCount += GetCountBySpecifyCard(iCardList,iCardCount,m_iReplaceCardArray[i]);
	}

	return iResultCount;
}

///��ȡָ���Ƹ���
///@param  BYTE iCardList[] [in] Ҫ���������
///@param  int iCardCount	[in] Ҫ������Ƶ�����
///@param  BYTE bCard   [in] ָ������ֵ
///@return ���ظ���ֵ������
BYTE CUpGradeGameLogic::GetCountBySpecifyCard(BYTE iCardList[],int iCardCount,BYTE bCard)
{
	int count = 0;

	for(int i = 0; i < iCardCount; ++i)
	{
		if(iCardList[i] == bCard) 
		{
			count++;
		}
	}

	return count;
}


//��ȡ�˿˻�ɫ
BYTE CUpGradeGameLogic::GetCardHuaKind(BYTE iCard)
{ 
	int iHuaKind=(iCard&UG_HUA_MASK);
	
	return iHuaKind; 
}

//��ȡ�˿˴�С ��2 - 18 �� 15 ���������� �� 2 - 21 �� 15 ����������
int CUpGradeGameLogic::GetCardBulk(BYTE iCard, BOOL bExtVol)
{
	if ((iCard==0x4E)||(iCard==0x4F))
	{
		return bExtVol?(iCard-14):(iCard-62);	//��С��64+14-62=16			ֻ���ش�Сè��ֵ
	}

	int iCardNum=GetCardNum(iCard);
	int iHuaKind=GetCardHuaKind(iCard);

	if (iCardNum==14)	// AΪ1��,ţţ������	
	{
		if(bExtVol)		
			return ((iHuaKind>>4)+(1*4));
		else
			return 1;
	}
	return ((bExtVol)?((iHuaKind>>4)+(iCardNum*4)):(iCardNum));
}

//�����˿�
BOOL CUpGradeGameLogic::SortCard(BYTE iCardList[], BYTE bUp[], int iCardCount)
{
	BOOL bSorted=TRUE,bTempUp;
	int iTemp,iLast=iCardCount-1,iStationVol[45];

	//��ȡλ����ֵ
	for (int i=0;i<iCardCount;i++)
	{
		iStationVol[i]=GetCardBulk(iCardList[i],TRUE);
		///if (iStationVol[i]>=15) iStationVol[i]+=m_iStation[4];
		///else iStationVol[i]+=m_iStation[GetCardHuaKind(iCardList[i],FALSE)>>4];
	}

	//�������
	do
	{
		bSorted=TRUE;
		for (int i=0;i<iLast;i++)
		{
			if (iStationVol[i]<iStationVol[i+1])
			{	
				//����λ��
				iTemp=iCardList[i];
				iCardList[i]=iCardList[i+1];
				iCardList[i+1]=iTemp;
				iTemp=iStationVol[i];
				iStationVol[i]=iStationVol[i+1];
				iStationVol[i+1]=iTemp;
				if (bUp!=NULL)
				{
					bTempUp=bUp[i];
					bUp[i]=bUp[i+1];
					bUp[i+1]=bTempUp;
				}
				bSorted=FALSE;
			}	
		}
		iLast--;
	} while(!bSorted);
		
	return TRUE;
}



//��������
BOOL CUpGradeGameLogic::ReSortCard(BYTE iCardList[], int iCardCount)
{
	SortCard(iCardList,NULL,iCardCount);
	//====�������Ŵ�С
	int iStationVol[45];
	for (int i=0;i<iCardCount;i++)
	{
		iStationVol[i]=GetCardBulk(iCardList[i],false);
	}

	int Start=0;
	int j,step;
	BYTE CardTemp[8];					//��������Ҫ��λ������
	int CardTempVal[8];					//����������λ������ֵ
	for(int i=8;i>1;i--)				//����������һ������i����ͬ��ֵ
	{
		for(j=Start;j<iCardCount;j++)
		{
			CardTemp[0]=iCardList[j];			//���浱ǰi��������ͬ��ֵ
			CardTempVal[0]=iStationVol[j];
				for(step=1;step<i&&j+step<iCardCount;)			//��һ������i��ֵ��ȵ�����(����������ʱ������)
				{
					if(iStationVol[j]==iStationVol[j+step])
					{
						CardTemp[step]=iCardList[j+step];			//������������
						CardTempVal[step]=iStationVol[j+step];		//��ֵ
						step++;
					}
					else
						break;
				}

			if(step>=i)			//�ҵ�һ������i����ȵ����鴮��ʼλ��Ϊj,����λ��Ϊj+setp-1
			{					//����Start��ʼ��j���������setp��
				if(j!=Start)				//�ų���ʼ��������
				{
					for(;j>=Start;j--)					//��Start����j�ź��ƶ�i��
						{
							iCardList[j+i-1]=iCardList[j-1];
							iStationVol[j+i-1]=iStationVol[j-1];
						}
					for(int k=0;k<i;k++)				
					{
						iCardList[Start+k]=CardTemp[k];	//��Start��ʼ���ó�CardSave
						iStationVol[Start+k]=CardTempVal[k];
					}
				}
				Start=Start+i;
			}
			j=j+step-1;
		}
	}
	return true;
}


//��ȡ�˿�
BYTE CUpGradeGameLogic::GetCardFromHua(int iHuaKind, int iNum)
{
	if (iHuaKind!=UG_ERROR_HUA) return (iHuaKind+iNum-1);
	return 0;
}

//�Ƿ����
BOOL CUpGradeGameLogic::IsDouble(BYTE iCardList[], int iCardCount)
{
	if (iCardCount!=3)
		return FALSE;
	int temp[17]={0},itwo=0;
	for(int i=0;i<iCardCount;i++)
		temp[GetCardNum(iCardList[i])]++;

	for(int i=0;i<17;i++)
	{
		if(temp[i]==2)
			itwo++;
	}
	return (itwo==1);
}

//�Ƿ�����
BOOL CUpGradeGameLogic::IsThree(BYTE iCardList[], int iCardCount)
{
	if (iCardCount!=3)
		return FALSE;
	int temp[17]={0},ithree=0;;
	for(int i=0;i<iCardCount;i++)
		temp[GetCardNum(iCardList[i])]++;

	for(int i=0;i<17;i++)
	{
		if(temp[i]==3)
			ithree++;
	}
	return (ithree==1);
}
/*
//�Ƿ����ԣ��������ӣ�
BOOL CUpGradeGameLogic::IsCompleDouble(BYTE iCardList[], int iCardCount)
{
	if (iCardCount <4)
		return FALSE;

	int temp[17]={0},itwo=0;;
	for(int i=0;i<iCardCount;i++)
		temp[GetCardNum(iCardList[i])]++;

	for(int i=0;i<17;i++)
	{
		if(temp[i]==2)
			itwo++;
	}
	return (itwo==2);
}
*/
//�Ƿ�ͬ��(ͬ��Ϊ������Ϊһ�ֻ�ʽ)
BOOL CUpGradeGameLogic::IsSameHua(BYTE iCardList[], int iCardCount)
{
	if (iCardCount != 3) return FALSE;
	int hs = -1;
	SortCard(iCardList,NULL,iCardCount);
	for (int i = 0;i < iCardCount;i++)
	{
		int hua = GetCardHuaKind(iCardList[i]);
		if (hs < 0)
		{
			hs = hua;
			continue;
		}
		if (hs != hua) return FALSE;
	}
	return TRUE;
}
BOOL CUpGradeGameLogic::IsSpecial(BYTE iCardList[],int iCardCount)
{
	if(iCardCount!=3)
		return false;
	SortCard(iCardList,NULL,iCardCount);
	if(GetCardNum(iCardList[0])==5
		&&GetCardNum(iCardList[1])==3
		&&GetCardNum(iCardList[2])==2)
		return true;
	return false;
}
/*
//�Ƿ�Ϊ��«������һ�����ƴ�2��һ�����ƣ�
BOOL CUpGradeGameLogic::IsHuLu(BYTE iCardList[], int iCardCount)
{
	if (iCardCount != 5) return FALSE;
	
	ReSortCard(iCardList,iCardCount);

	int pd0 = GetCardNum(iCardList[0]),
		pd1 = GetCardNum(iCardList[1]),
		pd2 = GetCardNum(iCardList[2]),
		pd3 = GetCardNum(iCardList[3]),
		pd4 = GetCardNum(iCardList[4]);

	return ((pd0 == pd1) && (pd1 == pd2) && (pd3 == pd4));
}

//�Ƿ�Ϊ��֧��4��ͬ����С���ƣ�
BOOL CUpGradeGameLogic::IsTieZhi(BYTE iCardList[], int iCardCount)
{
	if (iCardCount < 4) 
		return FALSE;
	int pd = -1;
	for (int i = 0;i < iCardCount;i++)
	{
		int dian = GetCardNum(iCardList[i]);
		if (pd < 0)
		{
			pd =dian;
			continue;
		}
		if (pd != dian) 
			return FALSE;
	}
	return TRUE;
}
*/
//�Ƿ�ͬ��˳��5�ţ�
BOOL CUpGradeGameLogic::IsSameHuaContinue(BYTE iCardList[], int iCardCount)
{
	if (iCardCount != 3) 
		return FALSE;

	if (!IsSameHua(iCardList,iCardCount)) 
		return FALSE;
	if (!IsSingleContinue(iCardList,iCardCount)) return FALSE;

	return TRUE;
}
////�Ƿ�ڽܿ�
//BOOL CUpGradeGameLogic::IsBlackJack(BYTE iCardList[], int iCardCount,bool Split)
//{
//	if (Split) //�������û�кڽܿ�,
//		return false;
//	if (iCardCount != 2)//�ڽܿ�ֻ������
//		return false;
//	if (GetCardPoint(iCardList, iCardCount, Split) != 22)//�ڽܿ˵ĵ���Ϊ21��
//		return false;
//	return true;
//}

//�Ƿ�˳(����3��)
BOOL CUpGradeGameLogic::IsSingleContinue(BYTE iCardList[], int iCardCount)
{
	if (iCardCount != 3) return FALSE;
	
	//if (IsSameHua(iCardList,iCardCount)) return FALSE;

	SortCard(iCardList,NULL,iCardCount);
	
	int hs = -1;
	int temp1,temp2,temp3;
	temp1=GetCardNum(iCardList[0]);
	temp2=GetCardNum(iCardList[1]);
	temp3=GetCardNum(iCardList[2]);
	for (int i = 0;i < iCardCount - 1;i++)
	{
		//if (GetCardNum(iCardList[i]) == 2 || iCardList[i] == 0x4E || iCardList[i] == 0x4F)//���ܺ�2�ʹ�С��
		//	return FALSE;
		
		if (GetCardNum(iCardList[i]) % 15 != (GetCardNum(iCardList[i+1]) + 1) % 15)
		{
		if(temp1==14 && temp2==3 &&temp3==2)//1,2,3��˳��
			return true;
		else 
			return FALSE;
				
		}
	}
	return TRUE;
}

//�Ƿ�˫˳(��������3��)
/*BOOL CUpGradeGameLogic::IsDoubleContinue(BYTE iCardList[], int iCardCount)
{
	if (iCardCount < 6 || iCardCount % 2 != 0) return FALSE;

	SortCard(iCardList,NULL,iCardCount);

	for (int i = 0;i < iCardCount - 2;i++)
	{
		if (GetCardNum(iCardList[i]) == 2 || iCardList[i] == 0x4E || iCardList[i] == 0x4F)
			return FALSE;
		if ((GetCardNum(iCardList[i]) % 15) != ((GetCardNum(iCardList[i+2]) + 1) % 15))
			return FALSE;
	}
	return TRUE;
}

//�Ƿ���˳������ͬ������3����
BOOL CUpGradeGameLogic::IsThreeContinue(BYTE iCardList[], int iCardCount)
{
	if (iCardCount < 9 || iCardCount % 3 != 0) return FALSE;

	SortCard(iCardList,NULL,iCardCount);

	for (int i = 0;i < iCardCount - 3;i++)
	{
		if (GetCardNum(iCardList[i]) == 2 || iCardList[i] == 0x4E || iCardList[0] == 0x4F)
			return FALSE;
		if ((GetCardNum(iCardList[i]) % 15) != ((GetCardNum(iCardList[i+3]) + 1) % 15))
			return FALSE;
	}
	return TRUE;
}

//�Ƿ�ɻ������(��˳+ͬ�����ĵ��Ż����)
BOOL CUpGradeGameLogic::IsPlaneAndWing(BYTE iCardList[], int iCardCount)
{
	if (iCardCount < 8) return FALSE;
	
	SetCard(iCardList,iCardCount);
	
	bool IsDaiOne = true;
	int shunCount = 0,daiCount = 0;
	PSTypeNode *first = NULL;
	
	int iTempNum[5];
	int iCnt = 0;
	for (POSITION pos = m_ptrPSList.GetHeadPosition();pos != NULL;)
	{
		PSTypeNode *pn = (PSTypeNode *) m_ptrPSList.GetNext(pos);
		if (!pn) continue;
		if (!first)
		{
			first = pn;
			shunCount++;
			iTempNum[iCnt++] = pn->mps;
			continue;
		}
		if (first->mcount != 3) return FALSE;
		if (pn->mcount == 3)
		{
			if ((pn->mps + 1) % 15 == first->mps % 15)
			{
				first = pn;
				shunCount++;
				iTempNum[iCnt++] = pn->mps;
				continue;
			}
			else return FALSE;
		}

		if (pn->mcount == 2) 
		{
			daiCount++;
			IsDaiOne = false;
			continue;
		}
		else if (pn->mcount == 1)
		{
			if (!IsDaiOne) return FALSE;

			daiCount++;
			continue;
		}
		
	}

	if (shunCount != daiCount) return FALSE;

	BYTE iBackCard[20];
	iCnt = 0;
	int j = 0, p = 0;
	for (int i = 0;i < iCardCount;i++)
	{
		if (j == 3)
		{
			p++;
			j = 0;
		}
		if (GetCardNum(iCardList[i]) == iTempNum[p])
		{
			iBackCard[iCnt++] = iCardList[i];
			iCardList[i] = 0;
			j++;

		}
	}
	
	for (int i = 0;i < iCardCount;i++)
	{
		if (iCardList[i] != 0) iBackCard[iCnt++] = iCardList[i];
	}
	
	::CopyMemory(iCardList,iBackCard,sizeof(BYTE)*iCnt);

	return TRUE;
}

//�Ƿ��Ĵ�������ͬ��+2���Ż�2���ƣ�
BOOL CUpGradeGameLogic::IsFourAndTwo(BYTE iCardList[], int iCardCount)
{
	if (iCardCount < 6) return FALSE;
	
	SortCard(iCardList,NULL,iCardCount);

	int iTempNum = -1;
	for (int i = 0;i < iCardCount - 3;i++)
	{
		if (GetCardNum(iCardList[i]) == GetCardNum(iCardList[i+3])) 
		{
			iTempNum = GetCardNum(iCardList[i]);
			break;
		}
	}

	if (iTempNum < 0) return FALSE;
	
	BYTE iTempCard[4];
	int iCnt = 0;

	for (int i = 0;i < iCardCount;i++)
	{
		if (iTempNum != GetCardNum(iCardList[i]))
		{
			iTempCard[iCnt++] = iCardList[i];
			continue;
		}
	}
	
	
	if (iCnt == 2) //�Ĵ�2����
	{
		if (GetCardNum(iTempCard[0]) == GetCardNum(iTempCard[1])) return FALSE;

		BYTE iBackCard[6];
		int j = 0;
		for (int i = 0;i < iCardCount;i++)
		{
			if (GetCardNum(iCardList[i]) == iTempNum) iBackCard[j++] = iCardList[i];
		}
		iBackCard[4] = (GetCardNum(iTempCard[0]) > GetCardNum(iTempCard[1])) ? iTempCard[0] : iTempCard[1];
		::CopyMemory(iCardList,iBackCard,sizeof(iBackCard));
	}
	else if (iCnt == 4) //�Ĵ�2����
	{
		if (!((IsDouble(iTempCard,2) && IsDouble(&iTempCard[2],2)) && (GetCardNum(iTempCard[0]) != GetCardNum(iTempCard[2])))) return FALSE;
		
		BYTE iBackCard[8];
		int j = 0;
		for (int i = 0;i < iCardCount;i++)
		{
			if (GetCardNum(iCardList[i]) == iTempNum) iBackCard[j++] = iCardList[i];
		}
		for (int i = 0;i < 4;i++)
		{
			iBackCard[j+i] = iTempCard[i];
		}
		::CopyMemory(iCardList,iBackCard,sizeof(iBackCard));
	}
	
	
	return TRUE;
}

//�Ƿ�510K
BOOL CUpGradeGameLogic::Is510KBomb(BYTE iCardList[], int iCardCount)
{
	if (iCardCount != 3) return FALSE;

	bool h5 = false,
		 h10 = false,
		 hk = false;

	SortCard(iCardList,NULL,iCardCount);
	if (GetCardNum(iCardList[0]) == 13) hk = true;//13 Is K
	if (GetCardNum(iCardList[1]) == 10) h10 = true;
	if (GetCardNum(iCardList[2]) == 5) h5 =true;
	if (hk && h10 && h5) return TRUE;
	return FALSE;
}


//�Ƿ�ը��
BOOL CUpGradeGameLogic::IsBomb(BYTE iCardList[], int iCardCount)
{
	if (iCardCount < 4) return FALSE;

	int pd1 = GetCardNum(iCardList[0]);
	
	for (int i = 0;i < iCardCount;i++)
	{
		if (GetCardNum(iCardList[i]) != pd1) return FALSE;
		else continue;
	}

	return TRUE;
}

//�Ƿ���
BOOL CUpGradeGameLogic::IsRocket(BYTE iCardList[], int iCardCount)
{
	if (iCardCount != 4) return FALSE;
	SortCard(iCardList,NULL,iCardCount);
	return ((iCardList[0] == 0x4F)&&(iCardList[1] == 0x4F)&&(iCardList[2] == 0x4E)&&(iCardList[3] == 0x4E));
}

//�Ƿ�ͬ��
BOOL CUpGradeGameLogic::IsSameHua(BYTE iCardList[], int iCardCount)
{
	if (iCardCount <= 0) return FALSE;
	
	int iFirstHua = GetCardHuaKind(iCardList[0],FALSE);
	
	for (int i = 0;i < iCardCount;i++)
	{
		if (GetCardHuaKind(iCardList[i],FALSE) != iFirstHua) return FALSE;
		else continue;
	}

	return TRUE;
}
*/

/*void CUpGradeGameLogic::SetCard(BYTE iCardList[], int iCardCount)
{
	if (iCardCount <= 0) return;
	ClearPSInfo();
	
	//add node
	for (int i = 0;i < iCardCount;i++)
	{
		int pd = GetCardNum(iCardList[i]);
		bool flag = false;

		for (POSITION pos = m_ptrPSList.GetHeadPosition();pos != NULL;)
		{
			PSTypeNode * ps = (PSTypeNode *) m_ptrPSList.GetNext(pos);
			if (!ps) continue;
			if (pd == ps->mps)
			{
				ps->mcount++;
				flag = true;
				break;
			}
		}

		if (!flag)
		{
			PSTypeNode * pn = new PSTypeNode();
			pn->mps = pd;
			pn->mcount = 1;
			m_ptrPSList.AddTail(pn);
		}
	}

	CPtrList temp;
	bool Is2In = false;

	for (POSITION pos = m_ptrPSList.GetHeadPosition();pos != NULL;)
	{
		PSTypeNode * pi = (PSTypeNode *) m_ptrPSList.GetNext(pos);
		if (!pi) continue;
		PSTypeNode *pn = new PSTypeNode();
		pn->mps = pi->mps;
		pn->mcount = pi->mcount;
		temp.AddTail(pn);
		if (pn->mps == 2) Is2In = true;
	}

	ClearPSInfo();
	//sort
	for (POSITION pos = temp.GetHeadPosition();pos != NULL;)
	{
		PSTypeNode *pi = (PSTypeNode *) temp.GetNext(pos);
		if (!pi) continue;
		PSTypeNode *pn = new PSTypeNode();
		pn->mps = pi->mps;
		pn->mcount = pi->mcount;

		if (m_ptrPSList.IsEmpty())
		{
			m_ptrPSList.AddTail(pn);
			continue;
		}

		bool bAdd = false;
		for (POSITION pos1 = m_ptrPSList.GetHeadPosition();pos1 != NULL;)
		{	
			POSITION tp = pos1;
			PSTypeNode *pii = (PSTypeNode *) m_ptrPSList.GetNext(pos1);
			if (!pii) continue;

			if ((pn->mcount > pii->mcount) || ((pn->mcount == pii->mcount) && (pn->mps > pii->mps))) 
			{
				m_ptrPSList.InsertBefore(tp,pn);
				bAdd = true;
				continue;
			}

		}

		if (!bAdd)
		{
			m_ptrPSList.AddTail(pn);
		}
	}

	while (!temp.IsEmpty())
	{
		PSTypeNode *ps = (PSTypeNode *) temp.RemoveHead();
		//delete ps;
	}
	temp.RemoveAll();

	if (Is2In)
	{
		PSTypeNode *pii = (PSTypeNode *) m_ptrPSList.RemoveHead();
		if (!pii) return;
		if (pii->mps == 14)//14ΪA
		{
			m_ptrPSList.AddTail(pii);
		}
		else
		{
			m_ptrPSList.AddHead(pii);
		}
	}

	return;
}



void CUpGradeGameLogic::ClearPSInfo()
{
	while (!m_ptrPSList.IsEmpty())
	{
		PSTypeNode *ps = (PSTypeNode *) m_ptrPSList.RemoveHead();
		//delete ps;
	}
	m_ptrPSList.RemoveAll();

	return;
}

*/
//�õ����л�ɫ
BYTE CUpGradeGameLogic::GetCardListHua(BYTE iCardList[], int iCardCount)
{
	int iHuaKind=GetCardHuaKind(iCardList[0]);
	if (GetCardHuaKind(iCardList[iCardCount-1])!=iHuaKind) return UG_ERROR_HUA;
	return iHuaKind;
}

//��ȡ����
BYTE CUpGradeGameLogic::GetCardShape(BYTE iCardList[], int iCardCount)
{
	/***************************************************
	ͬ��˳>��֧>��«>ͬ��>˳��>����>����>����>ɢ��
	***************************************************/

	//if (iCardCount <= 0) 	return UG_ERROR_KIND;//�Ƿ���
	//if (IsBlackJack(iCardList,iCardCount,false)) return SH_BLACK_JACK;//�ڽܿ�

	//return SH_OTHER;//ɢ��
	return 0;
}

bool CUpGradeGameLogic::IsVariationShape(BYTE  iCardList[],int iCardCount,BYTE iUpCard[],int iUpCount)
{
	//������ж๦���� һ���ܴճ�����
	if(!IsBull(iUpCard,3))
	{
		return false;
	}

	int total = CountPoint(iCardList,iCardCount);

	if(total%10 == 0)
	{
		return false;
	}

	if(GetReplaceCardCount(iUpCard,iUpCount) > 0 && CountPoint(iUpCard,iUpCount)%10 != 0)
	{
		return true;
	}

	BYTE byCard[USER_HAND_CARD] = {0};
	int iCount = GetRemainCard(iCardList,iCardCount,iUpCard,iUpCount,byCard);

	if(GetReplaceCardCount(byCard,iCount) > 0)
	{
		if(CountPoint(byCard,iCount)%10!=0)	//û����Ϊ�����
		{
			return true;
		}
	}

	return false;
}

//�Ƚ��������ƵĴ�С
int CUpGradeGameLogic::CompareCard(BYTE iFirstCard[], int iFirstCount,BYTE iFirstUpCard[], 
								   BYTE iSecondCard[], int iSecondCount,BYTE iSecondUpCard[])
{
	//***************************************************/
	BYTE iFirCard[5],iSecCard[5];

	::CopyMemory(iFirCard,iFirstCard,sizeof(BYTE)*iFirstCount);
	::CopyMemory(iSecCard,iSecondCard,sizeof(BYTE)*iSecondCount);

	SortCard(iFirCard,NULL,iFirstCount);
	SortCard(iSecCard,NULL,iSecondCount);

	BYTE iFirstCardShape = GetShape(iFirCard,iFirstCount,iFirstUpCard);
	BYTE iSecondCardShape = GetShape(iSecCard,iSecondCount,iSecondUpCard);

	if (iFirstCardShape != iSecondCardShape) 
	{
		return (iFirstCardShape - iSecondCardShape > 0) ? 1 : -1;
	}
	else
	{

		int bFirstShape = IsVariationShape(iFirstCard,iFirstCount,iFirstUpCard,3)?0:1;
		int bSecondShape = IsVariationShape(iSecondCard,iSecondCount,iSecondUpCard,3)?0:1;

		if(bFirstShape != bSecondShape)	//����������ͬ��������С����
		{
			return bFirstShape > bSecondShape?1:-1;
		}

		BYTE MaxFir = GetMaxCard(iFirCard, iFirstCount);

		BYTE MaxSec = GetMaxCard(iSecCard, iSecondCount);

		if (GetCardBulk(MaxFir) != GetCardBulk(MaxSec))		//����ţţ�ͱȽ������
		{
			return (GetCardBulk(MaxFir) - GetCardBulk(MaxSec) > 0) ? 1 : -1;
		}
		else //�����Ҳһ���ͱȽ�����ƻ�
		{
			return (GetCardHuaKind(MaxFir) - GetCardHuaKind(MaxSec) > 0) ? 1 : -1;
		}
	}
	
	return 0;
}
//�Ƚ��������ƵĴ�С
//int CUpGradeGameLogic::CompareCard(BYTE iFirstCard[], int iFirstCount, BYTE iSecondCard[], int iSecondCount)
//{
//	//***************************************************/
//	BYTE iFirCard[5],iSecCard[5];
//
//	::CopyMemory(iFirCard,iFirstCard,sizeof(BYTE)*iFirstCount);
//	::CopyMemory(iSecCard,iSecondCard,sizeof(BYTE)*iSecondCount);
//
//
//	SortCard(iFirCard,NULL,iFirstCount);
//	SortCard(iSecCard,NULL,iSecondCount);
//
//	BYTE iFirstCardShape = GetShape(iFirCard,iFirstCount),
//		iSecondCardShape = GetShape(iSecCard,iSecondCount);
//	//switch(iFirstCardShape)
//	//{
//	/*case UG_BULL_BULL:
//	case UG_BULL_ONE:
//	case UG_BULL_TWO:
//	case UG_BULL_THREE:
//	case UG_BULL_FOUR:
//	case UG_BULL_FIVE:
//	case UG_BULL_SIX:
//	case UG_BULL_SEVEN:
//	case UG_BULL_EIGHT:
//	case UG_BULL_NINE:
//	case UG_NO_POINT:*/
//		if (iFirstCardShape != iSecondCardShape) 
//		{
//			return (iFirstCardShape - iSecondCardShape > 0) ? 1 : -1;
//		}
//		else
//		{
//			BYTE MaxFir = GetMaxCard(iFirCard, iFirstCount),
//				MaxSec = GetMaxCard(iSecCard, iSecondCount);
//			if (GetCardBulk(MaxFir) != GetCardBulk(MaxSec)) //����ţţ�ͱȽ������
//				return (GetCardBulk(MaxFir) - GetCardBulk(MaxSec) > 0) ? 1 : -1;
//			else //�����Ҳһ���ͱȽ�����ƻ�
//			{
//				return (GetCardHuaKind(MaxFir) - GetCardHuaKind(MaxSec) > 0) ? 1 : -1;
//			}
//		}
////		break;
//	//}
//	return 0;
//}

//�Աȵ���
BOOL CUpGradeGameLogic::CompareOnlyOne(BYTE iFirstCard, BYTE iNextCard)
{
	int iFirstNum = GetCardNum(iFirstCard);//������
	int iNextNum  = GetCardNum(iNextCard);//������

	if (iFirstCard == 0x4F) return FALSE;//����
	if (iNextCard == 0x4F) return TRUE;

	if (iFirstCard == 0x4E) return FALSE;//С��
	if (iNextCard == 0x4E) return TRUE;

	if (iFirstNum == 2) return FALSE;//2
	if (iNextNum == 2) return TRUE;

	return ((iNextNum - iFirstNum) > 0 ? TRUE : FALSE);//����

	/*int iFristHua=GetCardHuaKind(iFirstCard,FALSE);
	int iNextHua=GetCardHuaKind(iNextCard,FALSE);
	if (iFristHua!=iNextHua)
	{
		//��ͬ��ɫ�Ա�
		if (iFristHua==UG_NT_CARD) return TRUE;
		return (iNextHua!=UG_NT_CARD);
	}
	
	//ͬ��ɫ�Ա�
	return GetCardBulk(iFirstCard,FALSE)>=GetCardBulk(iNextCard,FALSE);*/
}

//�Զ����ƺ���
BOOL CUpGradeGameLogic::AutoOutCard(BYTE iHandCard[], int iHandCardCount, BYTE iBaseCard[], int iBaseCardCount,
									BYTE iResultCard[], int & iResultCardCount, BOOL bFirstOut)
{
	if (bFirstOut == TRUE)
	{
		iResultCard[0] = iHandCard[iHandCardCount-1];
		iResultCardCount = 1;
	}
	return TRUE;
}


//��� 0 λ�˿�
int CUpGradeGameLogic::RemoveNummCard(BYTE iCardList[], int iCardCount)
{
	int iRemoveCount=0;
	for (int i=0; i<iCardCount; i++)
	{
		if (iCardList[i]!=0) iCardList[i-iRemoveCount]=iCardList[i];
		else iRemoveCount++;
	}
	return iRemoveCount;
}

//�����˿�
BYTE CUpGradeGameLogic::RandCard(BYTE iCard[], int iCardCount,bool bhaveKing)
{
	static const BYTE m_CardArray[54]={
			0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,		//���� 2 - A
			0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,		//÷�� 2 - A
			0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,		//���� 2 - A
			0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,		//���� 2 - A
			0x4E, 0x4F};
			//С�����
 

	BYTE iSend=0,iStation=0,iCardList[162];
	static int temp = 0;
	if (temp > 9999999999)
		temp = 0;
	srand((unsigned)GetCurrentTime()+(temp++));
	
	
	//int j = 0,n = 0;
	//if(iCardCount!=28)
	//  for (int i = 0;i < iCardCount;i += 13)
	//    {
	//	  j = n * 13;
	//	  ::CopyMemory(&iCardList[i],&m_CardArray[j],sizeof(BYTE)*13);
	//	   n++;
	//    }
	//else //���Ż�ɫֻȡ8-A
	//{
	//	for (int i = 0;i < iCardCount;i += 7)
	//    {
	//	  j = n * 13 + 6;
	//	  ::CopyMemory(&iCardList[i],&m_CardArray[j],sizeof(BYTE)*7);
	//	   n++;
	//    }
	//}
	if ( bhaveKing)
	{
		for (int i=0;i<iCardCount;i+=54)
			::CopyMemory(&iCardList[i],m_CardArray,sizeof(m_CardArray));
	}
	else
	{
		for (int i=0;i<iCardCount;i+=52)
			::CopyMemory(&iCardList[i],m_CardArray,sizeof(m_CardArray));
	}


	do
	{
		iStation=rand()%(iCardCount-iSend);
		iCard[iSend]=iCardList[iStation];
		iSend++;
		iCardList[iStation]=iCardList[iCardCount-iSend];
	} while (iSend<iCardCount);

	//for (int i=0;i<108;i++)
	//	::CopyMemory(&iCard[i],&m_CardArray[53],1);

	return iCardCount;
}

//ɾ���˿�
int CUpGradeGameLogic::RemoveCard(BYTE iRemoveCard[], int iRemoveCount, BYTE iCardList[], int iCardCount)
{
	//��������
	if ((iRemoveCount>iCardCount)) return 0;

	//��Ҫɾ����������
	int iDeleteCount=0;
	for (int i=0; i<iRemoveCount; i++)
	{
		for (int j=0; j<iCardCount; j++)
		{
			if (iRemoveCard[i]==iCardList[j])
			{
				iDeleteCount++;
				iCardList[j] = 0;
				break;
			}
		}
	}
	RemoveNummCard(iCardList,iCardCount);
	if (iDeleteCount!=iRemoveCount) return 0;

	return iDeleteCount;
}
int CUpGradeGameLogic::GetPoint(int Card)
{
	if (Card == 0x00)
		return 0;
	switch (GetCardNum(Card))
	{
	case 10:
		return 10;
	case 11:
		return 10;
	case 12:
		return 10;
	case 13:
		return 10;
	case 14:
		return 1;
	case 15:
		return 10;
	case 16:
		return 10;
	default:
		return GetCardNum(Card);
	}
}

//int CUpGradeGameLogic::GetCardPoint(BYTE Card[], int CardCount, bool split)
//{
//	int CardPoint = 0;
//	for (int i = 0; i < CardCount; i++)
//	  CardPoint += GetPoint(Card[i]);
//	if (CardPoint > 21)
//	{
//		for (int i = 0; i < CardCount; i++)
//		{
//			//�����A,���Ƶ���������21,A���Զ���Ϊ1�㼴��ȥ10��
//			if (GetCardNum(Card[i]) == 14 && CardPoint > 21)
//			      CardPoint -= 10;
//		}
//	}
//	if (CardPoint >21)
//		CardPoint = -1;//����21��ͱ���
//	if (CardCount == 2 && !split && CardPoint == 21)
//		return 22;//�ڽܿ�����Ϊ22�������
//
//	 return CardPoint;

//}
int CUpGradeGameLogic::GetShape(BYTE iCardList[],int iCardCount,BYTE iUpCard[])
{
	
	if (IsBombBull(iCardList,iCardCount)&&(m_iCardShape&(0x01<<2))) 
	{
		return UG_BULL_BOMB;
	}

	if (IsFiveSmall(iCardList,iCardCount)&&(m_iCardShape&(0x01<<3))) 
	{
		return UG_FIVE_SMALL;
	}

	if (IsGoldBull(iCardList,iCardCount)&&(m_iCardShape&(0x01<<1))) 
	{
		return UG_BULL_GOLD;
	}

	if (IsSilverBull(iCardList,iCardCount)&&(m_iCardShape&(0x01))) 
	{
		return UG_BULL_SILVER;
	}

	if(!IsBull(iUpCard,3))
	{
		return UG_NO_POINT;
	}


	if(IsBullBull(iCardList,iCardCount,iUpCard))
	{
		return UG_BULL_BULL;
	}

	switch(IsHaveNote(iCardList,iCardCount,iUpCard,3))
	{
	case 1:
		return UG_BULL_ONE;
	case 2:
		return UG_BULL_TWO;
	case 3:
		return UG_BULL_THREE;
	case 4:
		return UG_BULL_FOUR;
	case 5:
		return UG_BULL_FIVE;
	case 6:
		return UG_BULL_SIX;
	case 7:
		return UG_BULL_SEVEN;
	case 8:
		return UG_BULL_EIGHT;
	case 9:
		return UG_BULL_NINE;
	default:break;

	}

	return UG_NO_POINT;

}
//int CUpGradeGameLogic::GetShape(BYTE iCardList[],int iCardCount)
//{
//	if (IsBombBull(iCardList,iCardCount)&&(m_iCardShape&(0x01<<2))) 
//	{
//		return UG_BULL_BOMB;
//	}
//
//	if (IsFiveSmall(iCardList,iCardCount)&&(m_iCardShape&(0x01<<3))) 
//	{
//		return UG_FIVE_SMALL;
//	}
//
//	if (IsGoldBull(iCardList,iCardCount)&&(m_iCardShape&(0x01<<1))) 
//	{
//		return UG_BULL_GOLD;
//	}
//
//	if (IsSilverBull(iCardList,iCardCount)&&(m_iCardShape&(0x01))) 
//	{
//		return UG_BULL_SILVER;
//	}
//	
//	if(IsBullBull(iCardList,iCardCount))
//	{
//		return UG_BULL_BULL;
//	}
//
//	switch(IsHaveNote(iCardList,iCardCount))
//	{
//	case 1:
//		return UG_BULL_ONE;
//	case 2:
//		return UG_BULL_TWO;
//	case 3:
//		return UG_BULL_THREE;
//	case 4:
//		return UG_BULL_FOUR;
//	case 5:
//		return UG_BULL_FIVE;
//	case 6:
//		return UG_BULL_SIX;
//	case 7:
//		return UG_BULL_SEVEN;
//	case 8:
//		return UG_BULL_EIGHT;
//	case 9:
//		return UG_BULL_NINE;
//
//	}
//	return UG_NO_POINT;
//
//}
//�������е���
int CUpGradeGameLogic::CountPoint(BYTE  iCardList[],int iCardCount)
{
	int point = 0;
	for (int i=0;i<iCardCount;i++)
	{
		int temp = GetPoint(iCardList[i]);
		if (temp == 14)
			temp = 1;
		point += temp;
	}
	return point;
}

//��ȡ������3���Ƶ����������� ����������
int CUpGradeGameLogic::GetRemainCard(BYTE iCardList[],int iCardCount,BYTE iUpCard[],int iUpCount,BYTE byOtherCard[])
{
	int iCount = 0;

	if(CountPoint(iUpCard,iUpCount) <= 0)
	{
		return iCount;
	}
	
	for(int i = 0; i < iCardCount; ++i)
	{
		bool bEqual = false;
		for(int b = 0; b < iUpCount; ++b)
		{
			if(iCardList[i] == iUpCard[b])
			{
				bEqual = true;
				break;
			}
		}

		if(bEqual)
		{
			continue;
		}
		
		byOtherCard[iCount++] = iCardList[i];
	}

	return iCount;
}


//ͳ��ѡ��ָ���������Ƿ�������20,10,0�������Ϊ��0ֵ,��ʾ���µ�����,����0��ʾ������
bool CUpGradeGameLogic::CanSumIn(BYTE iUpCard[],int iUpCount)
{
	//int total = CountPoint(iCardList,iCardCount);
	//for (int i=0; i<3; i++)
	//{
	//	for (int j=i+1; j<4; j++)
	//		for (int k = j+1; k<iCardCount; k++)
	//		{
	//			int temp = GetPoint(iCardList[i])+GetPoint(iCardList[j])+GetPoint(iCardList[k]);
	//			if (temp==30||temp==10||temp==20)
	//			{
	//				return total-temp;
	//			}

	//		}
	//}

	int total = CountPoint(iUpCard,iUpCount);

	if(total%10 == 0)
	{
		return true;
	}

	if(GetReplaceCardCount(iUpCard,iUpCount) > 0)
	{
		return true;
	}

	return false;
}
/// �ж��Ƿ�ţţ����
/// @param BYTE  iCardList[] ���б�
/// @param int iCardCoun ������
bool CUpGradeGameLogic::IsBullBull(BYTE  iCardList[],int iCardCount,BYTE iUpCard[])
{
	if(!IsBull(iUpCard,3))		//û�е���
	{
		return false;
	}

	BYTE byCard[USER_HAND_CARD] = {0};
	int iCount = GetRemainCard(iCardList,iCardCount,iUpCard,3,byCard);

	int total = CountPoint(byCard,iCount);

	if((total > 0) && (total % 10 == 0))
	{
		return true;
	}

	if(GetReplaceCardCount(byCard,iCount) > 0)
	{
		return true;
	}

	return false;
}

/// �ж��Ƿ��ţ����
/// @param BYTE  iCardList[] ���б�
/// @param int iCardCoun ������
bool CUpGradeGameLogic::IsGoldBull(BYTE  iCardList[],int iCardCount)
{
	if (iCardCount != 5)
	{
		return false;
	}
	for (int i=0; i<iCardCount; i++)
	{
		//��ţ��JQK
		if (GetCardNum(iCardList[i]) != 11 && GetCardNum(iCardList[i]) != 12
			&& GetCardNum(iCardList[i]) != 13 && GetCardNum(iCardList[i]) != 15
			&& GetCardNum(iCardList[i]) != 16)
		{
			return false;
		}
	}
	return true;
}
/// �ж��Ƿ���ţ����
/// @param BYTE  iCardList[] ���б�
/// @param int iCardCoun ������
bool CUpGradeGameLogic::IsSilverBull(BYTE  iCardList[],int iCardCount)
{
	if (iCardCount != 5)
	{
		return false;
	}
	for (int i=0; i<iCardCount; i++)
	{
		//��ţ��10 J Q K ��С��
		if (GetCardNum(iCardList[i]) != 11 && GetCardNum(iCardList[i]) != 12
			&& GetCardNum(iCardList[i]) != 13 && GetCardNum(iCardList[i]) != 15
			&& GetCardNum(iCardList[i]) != 16 &&  GetCardNum(iCardList[i]) != 10)
		{
			return false;
		}
	}
	return true;
}
/// �ж��Ƿ�ը��ţ����
/// @param BYTE  iCardList[] ���б�
/// @param int iCardCoun ������
bool CUpGradeGameLogic::IsBombBull(BYTE  iCardList[],int iCardCount)
{
	if (iCardCount != 5)
	{
		return false;
	}

	int temp[17]={0};

	for (int i=0; i<iCardCount; i++)
	{
		temp[GetCardNum(iCardList[i])]++;
	}
	for (int i=0; i<17; i++)
	{
       if (temp[i] == 4)
	   {
		   return true;
	   }
	}
	return false;
}
/// �ж��Ƿ���Сţ����
/// @param BYTE  iCardList[] ���б�
/// @param int iCardCoun ������
bool CUpGradeGameLogic::IsFiveSmall(BYTE  iCardList[],int iCardCount)
{
	if (iCardCount != 5)
	{
		return false;
	}

	int iPoint = 0;
	for (int i=0; i<iCardCount; i++)
	{
		iPoint += GetPoint(iCardList[i]);
	}

	if (iPoint <= 10)
	{
		return true;
	}
	else
	{
		return false;
	}
}

///�Ƿ�Ϊ�е�
/// @param BYTE  iCardList[] ���б�
/// @param int iCardCoun ������
int CUpGradeGameLogic::IsHaveNote(BYTE iCardList[],int iCardCount,BYTE iUpCard[],int iUpCount)
{
	if(!IsBull(iUpCard,3))
	{
		return -1;
	}

	BYTE byCard[USER_HAND_CARD] = {0};
	int iCount = GetRemainCard(iCardList,iCardCount,iUpCard,iUpCount,byCard);

	int Note = CountPoint(byCard,iCount);

	return (Note % 10);
}
///�õ�������������(����ɫ)
/// @param BYTE  iCardList[] ���б�
/// @param int iCardCoun ������
int CUpGradeGameLogic::GetMaxCard(BYTE  iCardList[],int iCardCount)
{
	int temp = 0;
	BYTE card = 0;
	for (int i = 0; i < iCardCount; i++)
	{
		if (temp == 0)
		{
			temp = GetCardBulk(iCardList[i]);
			card = iCardList[i];
		}
		else
		{
			if (temp < GetCardBulk(iCardList[i]))
			{
				temp = GetCardBulk(iCardList[i]);
				card =iCardList[i];
			}
			else if (temp == GetCardBulk(iCardList[i]))
			{
				if(GetCardHuaKind(card) < GetCardHuaKind(iCardList[i]))
				{
					temp = GetCardBulk(iCardList[i]);
					card =iCardList[i];
				}
			}
		}

	}
	return card;
}

/// �ж��Ƿ���ţ
/// @param BYTE  iCardList[] ���б�
/// @param int iCardCoun ������
bool CUpGradeGameLogic::IsBull(BYTE  iCardList[],int iCardCount)
{
	return CanSumIn(iCardList,iCardCount);
}

//����==iCard�ĵ�������iCardList�е����
int  CUpGradeGameLogic::GetSerialBySpecifyCard(BYTE iCardList[],int iStart,int iCardCount,BYTE iCard)
{
	for(int i = iStart;i < iCardCount;i ++)
	{
		if(iCardList[i] == iCard)
			return i;
	}

	return -1;
}

BOOL CUpGradeGameLogic::GetBull(BYTE iCardList[],int iCardCount,BYTE iBullCard[])
{
	struct sCard
	{
		int iShape;		//����
		BYTE byVal[3];	//��ֵ

		sCard()
		{
			iShape = 0;
			::memset(byVal,0,sizeof(byVal));
		}

		sCard& operator =(sCard _TsCard)
		{
			iShape = _TsCard.iShape;
			for(int i = 0; i < 3; ++i)
			{
				byVal[i] = _TsCard.byVal[i];
			}
			
			return *this;
		}
	};

	list<sCard> byCList;

	for (int i = 0; i < 3; i++)
	{
		for (int j = i + 1; j < 4; j++)
		{
			for (int k = j + 1; k < iCardCount; k++)
			{
				sCard sTempCard;
				sTempCard.byVal[0]=iCardList[i];
				sTempCard.byVal[1]=iCardList[j];
				sTempCard.byVal[2]=iCardList[k];

				if (IsBull(sTempCard.byVal,3))
				{
					sTempCard.iShape = GetShape(iCardList,iCardCount,sTempCard.byVal);

					if(UG_BULL_BULL == sTempCard.iShape && GetReplaceCardCount(iCardList,iCardCount)<=0)
					{
						::CopyMemory(iBullCard,sTempCard.byVal,sizeof(sTempCard.byVal));
						return true;
					}

					byCList.push_back(sTempCard);
				}
			}
		}
	}

	//�б�Ϊ�� ˵��û�п���ϵ�����
	if(byCList.empty())
	{
		return false;
	}

	 
	//����������
	list<sCard>::iterator it = byCList.begin();

	sCard sTempCard;

	sTempCard = *it;

	for(; it != byCList.end(); ++it)
	{
		if(sTempCard.iShape < it->iShape)
		{
			sTempCard = *it;
		}
	}

	::CopyMemory(iBullCard,sTempCard.byVal,sizeof(sTempCard.byVal));

	return true;
}
