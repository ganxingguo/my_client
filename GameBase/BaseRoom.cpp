///��������
#include "StdAfx.h"
#include "BaseRoom.h"
#include <afxsock.h>

BEGIN_MESSAGE_MAP(CBaseRoom, CDialog)
//	ON_WM_SIZE()
END_MESSAGE_MAP()


//��������ʱ�� ID
#define ID_PRO_TIME					100			
/*******************************************************************************************************/


/*******************************************************************************************************/
//���캯��
CBaseRoom::CBaseRoom(UINT uTemplate) :CDialog(uTemplate)
{
	OutputDebugString("@@@ CBaseRoom() ------- !");

	//��ʼ�� SOCKET
	//if (!AfxSocketInit())
	//{
	//}

	m_TCPSocket = new CTCPClientSocket(this);

	//m_TCPSocket = new CTCPClientSocket(this);
	//OutputDebugString("@@@ new CTCPClientSocket(this) ------- !");

	//m_TCPSocket = new CTCPClientSocket(this);
}

//��������
CBaseRoom::~CBaseRoom()
{
	try
	{
		//if (m_TCPSocket != NULL)
		//	delete m_TCPSocket;

		ReleaseSocket();
	}
	catch (...)
	{
		m_TCPSocket = NULL;
	}
}

//��ʼ������
BOOL CBaseRoom::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetClassLong(m_hWnd, GCL_HBRBACKGROUND, NULL);

	return TRUE;
}

CTCPClientSocket* CBaseRoom::GetNewSocketPtr()
{
	if (m_TCPSocket == NULL)
	{
		m_TCPSocket = new CTCPClientSocket(this);
	}

	return m_TCPSocket;
}

void CBaseRoom::ReleaseSocket()
{
	if (m_TCPSocket != NULL)
	{
		delete m_TCPSocket;
		m_TCPSocket = NULL;
	}
}

//����������Ϣ����
int CBaseRoom::SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	return m_TCPSocket->SendData(bMainID,bAssistantID,bHandleCode);//����������
}

//����������Ϣ����
int CBaseRoom::SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	return m_TCPSocket->SendData(pData,uBufLen,bMainID,bAssistantID,bHandleCode);
}

/*******************************************************************************************************/

