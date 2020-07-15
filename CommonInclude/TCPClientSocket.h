#pragma once

#ifndef CTCPCLIENTSOCKET_HEAD_FILE
#define CTCPCLIENTSOCKET_HEAD_FILE

#include <Windows.h>
#include <WinSock2.h>

#include "basemessage.h"
#include "RC4Engine.h"

//���涨��
#define SED_SIZE                60000                                   //��������С
#define RCV_SIZE                30000                                   //��������С

//״̬����
#define NO_CONNECT              0                               //û������
#define CONNECTING              1                               //��������
#define CONNECTED               2                               //�ɹ�����

//��˵��
class CTCPClientSocket;

/*******************************************************************************************************/

//�ͻ����������ӿ�
interface  IClientSocketService
{
	//�ӿں���
public:
	//�����ȡ��Ϣ
	virtual bool OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket) = 0;
	//����������Ϣ
	virtual bool OnSocketConnectEvent(UINT uErrorCode, CTCPClientSocket * pClientSocket) = 0;
	//����ر���Ϣ
	virtual bool OnSocketCloseEvent() = 0;
};

/*******************************************************************************************************/

//�ͻ��� SOKET
class AFX_EXT_CLASS CTCPClientSocket : public CWnd
{
	DECLARE_MESSAGE_MAP()

	//��������
private:
	int                                 m_iReadBufLen;                  //����������
	BYTE                                m_bConnectState;                //����״̬
	SOCKET                              m_hSocket;                      //SOCKET ���
	IClientSocketService                * m_pIService;                  //����ӿ�
	BYTE                                m_szBuffer[RCV_SIZE];           //���ݻ�����

	int                                 m_iCheckCode;
	bool								m_bSetCheck;
	RC4Engine							iRcEngine;						//���ݼ��ܶ���
	RC4Engine							iRcEngineR;						//���ݼ��ܶ���
	//��������
public:
	//���캯��
	CTCPClientSocket(IClientSocketService * pIService);
	//��������
	virtual ~CTCPClientSocket();
	//��ʼ�� SOCKET ����
	bool InitSocketWnd();
	//�Ƿ��Ѿ�����
	BYTE GetSocketState()
	{
		return m_bConnectState;
	}
	//�ر� SOCKET
	bool CloseSocket(bool bNotify = true);
	//���ӷ�����
	bool Connect(const TCHAR * szServerIP, UINT uPort);
	//���ӷ�����
	bool Connect(long int dwServerIP, UINT uPort);
	//���ͺ���
	int SendData(void * pData, UINT uSize, BYTE bMainID, BYTE bAssistantID, UINT bHandleCode);
	//������ͺ���
	int SendData(BYTE bMainID, BYTE bAssistantID, UINT bHandleCode);
	//�������� û������ͷ
	int SendDataNonHead(void* pData, UINT uSize, BYTE bMainID, BYTE bAssistantID, UINT bHandleCode);
	//���ʹ���
	CString TranslateError(UINT uErrorCode);
	//����У���룬��������Կ
	void SetCheckCode(__int64 iCheckCode, int isecretkey);

	//�ڲ�����
private:
	//�ر� SOCKET ����
	bool CloseSocketWnd();
	//SOCKET ��Ϣ�������
	LRESULT OnSocketNotifyMesage(WPARAM wParam, LPARAM lParam);
};

/*******************************************************************************************************/

#endif