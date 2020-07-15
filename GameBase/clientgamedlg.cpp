#include "StdAfx.h"
#include "clientgamedlg.h"
#include <math.h>

#ifdef  SUPER_VERSION
//#include "..\..\�ͻ������\�����.2003\resource.h"
#include "platform/CardSet.h"
#endif


BEGIN_MESSAGE_MAP(CClientGameDlg, CLoveSendClass)
	ON_WM_TIMER()
END_MESSAGE_MAP()    


int CClientGameDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return __super::OnCreate(lpCreateStruct);
}

//���캯��
CClientGameDlg::CClientGameDlg() : CLoveSendClass(IDD_GAME_FRAME)
{
	m_bGameStation = GS_WAIT_SETGAME;           ///��Ϸ״̬

	//20130703 ljg ��255 �޸�Ϊ0 ��ֹ���������ر�
	m_byLgStation = 0;						///�Լ����߼�λ��

	m_bWatchMode = false;						///�Ƿ�Ϊ�Թ���

	InitGameData();

	//20140417 ljg ʱ�䲻�ܳ�ʼΪ0����Ȼ�����������쳣�����統������
	m_sIData.byBTime = 1;
	m_sIData.bySCTime = 1;
	m_sIData.byTTime = 1;
	m_sIData.byRTime = 1;

	CINIFile iniFile(CINIFile::GetAppPath()+"10900600_r.ini");

	m_RobotMode = iniFile.GetKeyVal("RobotSet", "RobotMode",0);	

	m_iWriteLog = iniFile.GetKeyVal("RobotSet","WriteLog",0);
}

//��������
CClientGameDlg::~CClientGameDlg()
{

}

/**
*��ʼ����Ϸ����
*
*return bool ��ʼ����Ϸ�����Ƿ�ɹ�
*/
bool CClientGameDlg::InitGameData()
{
	m_iSCCount = 0;										///��������

	::memset(m_iUCCount,0,sizeof(m_iUCCount));			///�����������

	::memset(m_byUCard,0,sizeof(m_byUCard));			///����˿�

	::memset(m_byDownBCard,0,sizeof(m_byDownBCard));		///�����ţ��

	::memset(m_sBData.bIsInGame,false,sizeof(m_sBData.bIsInGame));	///����Ƿ�����Ϸ��

	return true;
}

/**
* ����������Ϸ�пͻ��˵�����
*
*return bool true:�������óɹ���false:��������ʧ��
*/
bool CClientGameDlg::ResetGameData()
{
	InitGameData();		//��ʼ����Ϸ����
	return true ; 
}
void CClientGameDlg::OnTimer(UINT nIDEvent)
{

}

/**
* ���շ���˷��͹�������Ϣ����������Ϸ�ĵ���״̬��
* ����������ʱ����룬��������Ϸ�ĵ�ǰ״̬
*
* @param [in] void * pBuffer ���ݰ�
*
* @param [in] int nLen ���ݰ��ĳ���
*
* @return void 
*/
bool CClientGameDlg::SetGameStation(void * pStationData, UINT uDataSize)
{

	m_byLgStation = GetMeUserInfo()->bDeskStation;


	//������Ϸ״̬
	GameStationBase *pGameStationBase = (GameStationBase *)pStationData;

	m_sBData = pGameStationBase->sBData;

	SetStationParameter(m_sBData.bGameStation);

	//DebugPrintfN("SetGameStation ���ID %d ���λ�� %d ��Ϸ״̬ %d",GetMeUserInfo()->dwUserID,m_byLgStation,GetStationParameter());

	DebugPrintf("���յ��������� ����=��%d��  ״̬=��%d��",GetMeUserInfo()->dwUserID,GetStationParameter());
	///����״̬��������
	switch(GetStationParameter())
	{
	case GS_WAIT_SETGAME:
	case GS_WAIT_ARGEE:
		{
			GameStation_2 *pGameStation = (GameStation_2 *)pStationData;

			m_sIData = pGameStation->s_Data;

			m_Logic.SetCardShape(m_sIData.iCardShape);

			srand(GetTickCount()+ m_byLgStation);
			int iTemp = rand() % m_sIData.byBTime;
			iTemp = iTemp > 7?7:iTemp;	
			iTemp = iTemp < 4?4:iTemp;
			SetGameTimer(m_byLgStation,iTemp,ID_BEGIN_TIME);	//��ʼ��ʱ��

			break;
		}
	case GS_SEND_CARD:
		{
			GameStation_3 *pGameStation = (GameStation_3 *)pStationData;

			m_sIData = pGameStation->s_Data;

			::CopyMemory(m_iUCCount,pGameStation->iUCCount,sizeof(pGameStation->iUCCount));

			::CopyMemory(m_byUCard[m_byLgStation],pGameStation->byUCard,sizeof(pGameStation->byUCard));

			m_Logic.SetCardShape(m_sIData.iCardShape);

			break;
		}
	case GS_PLAY_GAME:
		{
			GameStation_4 *pGameStation = (GameStation_4 *)pStationData;
			m_sIData = pGameStation->s_Data;

			::CopyMemory(m_iUCCount,pGameStation->iUCCount,sizeof(pGameStation->iUCCount));

			::CopyMemory(m_byUCard[m_byLgStation],pGameStation->byUCard,sizeof(pGameStation->byUCard));

			::CopyMemory(m_byDownBCard[m_byLgStation],pGameStation->byDownCard,sizeof(pGameStation->byDownCard));

			m_Logic.SetCardShape(m_sIData.iCardShape);

			break ; 
		}
	case GS_WAIT_NEXT:
		{
			GameStation_5 *pGameStation = (GameStation_5 *)pStationData;

			m_sIData = pGameStation->s_Data;

			m_Logic.SetCardShape(m_sIData.iCardShape);

			srand(GetTickCount()+ m_byLgStation);
			int iTemp = rand() % m_sIData.byBTime;
			iTemp = iTemp > 7?7:iTemp;	
			iTemp = iTemp < 4?4:iTemp;
			SetGameTimer(m_byLgStation,iTemp,ID_BEGIN_TIME);	//��ʼ��ʱ��
			break;
		}
	default:
		{
			break;
		}
	}

	//DebugPrintfN("SetGameStation end");

	return true;
}


//ͬ�⿪ʼ��Ϸ 
bool CClientGameDlg::OnControlHitBegin()
{
	if((GetStationParameter()==GS_WAIT_SETGAME||GetStationParameter()==GS_WAIT_NEXT||GetStationParameter()==GS_WAIT_ARGEE))
	{
		SendGameData(MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);		//���ؿ�ʼ��Ϣ�������
	}
	return true;
}
//��Ϸ��Ϣ������
bool CClientGameDlg::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	//DebugPrintfN("HandleGameMessage ���ID %d ���λ�� %d ��Ϸ״̬ %d ��ϢID %d",GetMeUserInfo()->dwUserID,m_byLgStation,GetStationParameter(),pNetHead->bAssistantID);
	m_byLgStation = GetMeUserInfo()->bDeskStation;

	DebugPrintf("���յ���������Ϣ ����=��%d��  ��ϢID=��%d��",GetMeUserInfo()->dwUserID,pNetHead->bAssistantID);

	switch(pNetHead->bAssistantID)
	{
	case ASS_GM_AGREE_GAME:
		{

			if(uDataSize!=sizeof(MSG_GR_R_UserAgree))
			{
				return 0;
			}
			MSG_GR_R_UserAgree * pUser=(MSG_GR_R_UserAgree *)pNetData;

			SetStationParameter(GS_WAIT_ARGEE);		//����Ϊ�ȴ�״̬
			OnControlHitBegin();
			////���Թ��ߵ��˽�����ϵ� ������ť
			//if(m_byLgStation == pUser->bDeskStation)
			//{
			//	ResetGameData();	
			//}

			break;
		}
	case ASS_BEGIN_UPGRADE:
		{	
			if(uDataSize!=sizeof(BeginUpgradeStruct))
			{
				return 0;
			}

			ResetGameData();

			BeginUpgradeStruct *pBeginUpgrade = (BeginUpgradeStruct *)pNetData;

			m_iGamePlayNum = pBeginUpgrade->iGamePlayNum;
			m_iRealPlayNum = pBeginUpgrade->iRealPlayNum;

			m_byLgStation =  GetMeUserInfo()->bDeskStation;

			::CopyMemory(m_sBData.bIsInGame,pBeginUpgrade->bIsInGame,sizeof(m_sBData.bIsInGame));

			break;
		}
	case ASS_SEND_CARD:
		{
			if(uDataSize!=sizeof(SendCardStruct))
			{
				return 0;
			}
			SetStationParameter(GS_SEND_CARD);	//����״̬

			SendCardStruct *pSendCard = (SendCardStruct *)pNetData;

			::CopyMemory(m_iUCCount,pSendCard->iUCCount,sizeof(pSendCard->iUCCount));

			::CopyMemory(m_byUCard,pSendCard->byUCard,sizeof(pSendCard->byUCard));

			SetGameTimer(m_byLgStation,1,ID_SCARD_TIME);	//���÷��ƶ�ʱ��

			break;
		}
	case ASS_TAN_PAI:
		{
			if (m_bIsNote)
			{
				if(uDataSize!=sizeof(TanPaiStruct))
				{
					return 0;
				}

				TanPaiStruct *pTanPai = (TanPaiStruct *)pNetData;

				if (pTanPai->bIsRealPlayer)
				{
					SetGameTimer(GetMeUserInfo()->bDeskStation, 1, ID_THINK_TIME);
					m_bIsNote = false;
				}
			}

			break;
		}
	case ASS_CONTINUE_END:
		{
			if(uDataSize!=sizeof(GameEndStruct))
			{
				return 0;
			}

			KillAllTimer();

			GameEndStruct *pGameEnd = (GameEndStruct *)pNetData;

			srand(GetTickCount()+ m_byLgStation);
			int iTemp = rand() % m_sIData.byRTime;
			iTemp = iTemp > 5?5:iTemp;	
			iTemp = iTemp < 2?2:iTemp;
			SetGameTimer(m_byLgStation,iTemp,ID_RESULT_TIME);

			SetStationParameter(GS_WAIT_ARGEE);	//������ϷΪ�ȴ�״̬
			break;
		}
	default:
		{
			break;
		}
	}

	//DebugPrintfN("HandleGameMessage end");

	return __super::HandleGameMessage(pNetHead,pNetData,uDataSize,pClientSocket);
}

/**
* ��ʾ
*/
void CClientGameDlg::OnHitCue()
{
	//DebugPrintfN("OnHitCue ���ID %d ���λ�� %d ��Ϸ״̬ %d",GetMeUserInfo()->dwUserID,m_byLgStation,GetStationParameter());

	m_Logic.GetBull(m_byUCard[GetMeUserInfo()->bDeskStation],m_iUCCount[GetMeUserInfo()->bDeskStation], m_byDownBCard[GetMeUserInfo()->bDeskStation]);

	//DebugPrintfN("OnHitCue end");
}

/**
* ̯��
*/
void CClientGameDlg::OnHitTanPai()
{
	OnHitCue();

	//DebugPrintfN("OnHitTanPai ���ID %d ���λ�� %d ��Ϸ״̬ %d",GetMeUserInfo()->dwUserID,m_byLgStation,GetStationParameter());

	TanPaiStruct tanpai;

	tanpai.byStaiton = m_byLgStation;
	::CopyMemory(tanpai.byDownCard,m_byDownBCard[m_byLgStation],sizeof(m_byDownBCard[m_byLgStation]));

	SendGameData(&tanpai,sizeof(TanPaiStruct),MDM_GM_GAME_NOTIFY,ASS_TAN_PAI,0);

	//DebugPrintfN("OnHitTanPa end");
}


/*
* ��ӡ��־�ļ�
*/
void CClientGameDlg::DebugPrintf(const char *p, ...)
{
	if (0 == m_iWriteLog)
	{
		return ;
	}

	char szFilename[256];
	CString strPath = CINIFile::GetAppPath() +"\\GameLog\\";

	SHCreateDirectoryEx(NULL, strPath, NULL);

	CTime time = CTime::GetCurrentTime();

	sprintf(szFilename, "%s%d_%d_%d_%d%d%d_log.txt",strPath ,NAME_ID,GetMeUserInfo()->bDeskNO,GetMeUserInfo()->bDeskStation,time.GetYear(), time.GetMonth(), time.GetDay());
	FILE *fp = fopen( szFilename, "a" );
	if (NULL == fp)
	{
		return;
	}

	//����ʱ��
	char cTime[30];
	::memset(cTime,0,sizeof(cTime));
	sprintf(cTime,"[%d:%d:%d] ",time.GetHour(),time.GetMinute(),time.GetSecond());
	fprintf(fp,cTime);

	va_list arg;
	va_start( arg, p );
	vfprintf(fp, p, arg );
	fprintf(fp,"\n");
	va_end( arg, p );

	fclose(fp);
}




//������ж�ʱ��
void CClientGameDlg::KillAllTimer()
{
	KillTimer(ID_BEGIN_TIME);		//��ʼ��ʱ��
	KillTimer(ID_THINK_TIME);		//˼����ʱ��
	KillTimer(ID_SCARD_TIME);		//���ƶ�ʱ��
	KillTimer(ID_RESULT_TIME);		//������ʱ��
	return;
}

//��ʱ����Ϣ
bool CClientGameDlg::OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)
{
	if(uTimeCount <= 0)
	{
		//DebugPrintfN("OnGameTimer ���ID %d ���λ�� %d ��Ϸ״̬ %d ��ʱ��ID %d",GetMeUserInfo()->dwUserID,m_byLgStation,GetStationParameter(),uTimeID);
	}

	DebugPrintf("��ʱ�� ����=��%d��  ʱ��ID=��%d��",GetMeUserInfo()->dwUserID,uTimeID);

	switch(uTimeID)
	{
	case ID_BEGIN_TIME:		//��Ϸ��ʼ��ʱ��
		{
			if(uTimeCount <= 0)
			{
				KillTimer(ID_BEGIN_TIME);

				OnControlHitBegin();
			}
			
			break;
		}
	case ID_SCARD_TIME:			//���ƶ�ʱ��
		{
			if(uTimeCount <= 0)
			{
				KillTimer(ID_SCARD_TIME);


				m_bIsNote = false;

				//�������    
				if ((m_iGamePlayNum == 2) || m_iRealPlayNum == 0 || m_RobotMode)
				{
					/// changed by zdk at 2014-1-23 ������̯��ʱ���޸�Ϊ1~2��
					srand(GetTickCount()+GetMeUserInfo()->bDeskStation);
					int  iRandNo = rand() % 3;
					switch(iRandNo)
					{
					case 0:
						SetGameTimer(GetMeUserInfo()->bDeskStation, (rand() % 2)+1, ID_THINK_TIME);
						break;
					case 1:
						SetGameTimer(GetMeUserInfo()->bDeskStation, (rand() % 2)+1, ID_THINK_TIME);
						break;
					case 2:
						SetGameTimer(GetMeUserInfo()->bDeskStation, (rand() % 2)+2, ID_THINK_TIME);
						break;
					default:
						SetGameTimer(GetMeUserInfo()->bDeskStation, (rand() % 2)+2, ID_THINK_TIME);
						break;
					}	
				}
				else if((m_iGamePlayNum >= 3) && (m_iRealPlayNum >=1))
				{
					srand(GetTickCount()+GetMeUserInfo()->bDeskStation);

					int  iRandNo = rand() % 2;

					switch(iRandNo)
					{
					case 0:
						SetGameTimer(GetMeUserInfo()->bDeskStation, (rand() % 2)+1, ID_THINK_TIME);
						break;
					case 1:
						m_bIsNote = true;
						break;		//������50%����̯�ƣ���̯�ƵĻ���������̯�ƾ�����̯��
					default:
						SetGameTimer(GetMeUserInfo()->bDeskStation, (rand() % 2)+1, ID_THINK_TIME);
					}
				}
			}
		
			break;
		}
	case ID_THINK_TIME:			//��Ϸ����ʱ���ؽ����ʱ��
		{
			if(uTimeCount <= 0)
			{
				KillTimer(ID_THINK_TIME);

				OnHitTanPai();
			}
		
			break;
		}
	case ID_RESULT_TIME:		//������ʱ��
		{
			if(uTimeCount <= 0)
			{
				KillTimer(ID_RESULT_TIME);

				OnControlHitBegin();
			}

			break;
		}
	default:
		{
			break;
		}
	}

	if(uTimeCount <= 0)
	{
		//DebugPrintfN("OnGameTimer end");
	}

	return true;
}