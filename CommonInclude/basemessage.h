#pragma once

#ifndef AFC_MESSAGE_HEAD_FILE
#define AFC_MESSAGE_HEAD_FILE

/********************************************************************************************/
//�����õĶ˿ں�
//�˿ڶ���
#define FLASH_SERVER_PORT       843                             ///ҳ����֤�������˿�//���ʱ�׼�˿ں�

#define GAME_LOGON_PORT         3015                            ///��Ϸ��½�˿�

#define CENTER_SERVER_PORT      13025                       ///���ķ���˿�
#define CENTER_DISTRI_PORT      13026                          ///���ķ�����ת�˿�

#define Z_W_CONNECT_PORT        13027                          ///������������Ϸ���������Ӷ˿�

/********************************************************************************************/

#define NUM_DESK_LOCK_PASS              1						//��������λ��

///������汾
#define DEV_LIB_VER             1                                   ///������汾
///ȫ�ְ�Ȩ���̶��ڳ�����޷��޸�
#define TMLcopyright            TEXT("����:")             ///��Ȩ

///�������ݰ��ṹͷ
struct NetMessageHead
{
	UINT                        uMessageSize;                       ///���ݰ���С
	UINT                        bMainID;                            ///����������
	UINT                        bAssistantID;                       ///������������ ID
	UINT                        bHandleCode;                        ///���ݰ��������
	UINT                        bReserve;                           ///�����ֶ�
};

///�������ݰ��ṹͷ
struct NetGateHead
{
public:
	int              nSocketIndex;
	int              nRoomId;
	int              nDeskId;
	int				 uBetchId;
	UINT            uDataSize;
	NetGateHead(UINT uSize = 0) {
		uDataSize = uSize;
		nSocketIndex = -1;
		nRoomId = -1;
		nDeskId = -1;
		uBetchId = -1;
	}
};
static const char*  CompressHead = "LZOCompress";
///��������
#define MAX_PEOPLE              180                                 ///�����Ϸ���� �ټ��� modify 8 -> 180 by wlr 20090714
#define INVALID_CHAIR			255									///��Ч��λ
#define MAX_SEND_SIZE           4000                                ///�����Ϣ��
#define MAX_SEND_SIZE_NEW           4080                                ///�����Ϣ��
#define MAX_TALK_LEN            500                                 ///����������ݳ���
#define NORMAL_TALK_LEN         200                                 ///��ͨ�������ݳ���
#define US_PLAY_GAME            20                                  ///�û���Ϸ״̬
#define NET_HEAD_SIZE           sizeof(NetMessageHead)              ///���ݰ�ͷ
#define NET_GATEHEAD_SIZE       sizeof(NetGateHead)              //���ܰ�ͷ
#define SOCKET_BUFFER           MAX_SEND_SIZE + NET_HEAD_SIZE + NET_GATEHEAD_SIZE      ///�������ݰ�
#define SOCKET_Compress_Size    strlen(CompressHead)
///��Ϸ���Ͷ���
#define TY_NORMAL_GAME          1                                   ///��ͨ��Ϸ
#define TY_MATCH_GAME           2                                   ///������Ϸ
#define TY_MONEY_GAME           3                                   ///�����Ϸ
#define TY_DRILLING_GAME        4                                   ///��������Ϸ(������)
#define TY_FASCINATION_GAME     5                                   ///��������Ϸ(������)
///
///Kylin 20090105 ������ҳ�
#define TY_SHABI_GAME       6                                   ///���ҳ���Ϸ
///
///֧�����Ͷ���
#define SUP_NORMAL_GAME         0x01                                ///��ͨ��Ϸ
#define SUP_MATCH_GAME          0x02                                ///������Ϸ
#define SUP_MONEY_GAME          0x04                                ///�����Ϸ

///��ʾλ��
#define SHS_TALK_MESSAGE        0x01                                ///������Ϣ
#define SHS_MESSAGE_BOX         0x02                                ///������Ϣ
#define SHS_SYSTEM_SHOW         0x04                                ///ϵͳ��Ϣ
#define CodeSocketConnetID(nSocketId,nConnetId)(UINT64(nSocketId)*10000+(nConnetId)) //����GateScoketID
//����
#define DeCodeSocketID(nCode)((nCode)/10000)
#define DeCodeConnetID(nCode) (nCode%10000)
#pragma pack(1)
///�������ݰ��ṹͷ

class NetGateMessage
{
public:
	NetGateMessage(const NetGateHead *head = nullptr, const void* pData = nullptr,const UINT uSize = 0) {
		ZeroMemory(m_data, sizeof(m_data));
		m_uSize = 0;
		MakeData(head, pData,uSize);
	}
	~NetGateMessage() {
	}
	inline void MakeData(const NetGateHead *head, const void* pData,const UINT nBuffSize) {	
		if (head != nullptr)
		{
			memcpy(m_data, head, NET_GATEHEAD_SIZE);
			m_uSize += NET_GATEHEAD_SIZE;
		}
		
		if (pData != nullptr)
		{
			memcpy(m_data + m_uSize, pData,nBuffSize);
			m_uSize += nBuffSize;
		}
		
	}
	inline void* GetMessage() const {
		
		return m_uSize == 0 ? nullptr:(void *)m_data;
	}
	inline UINT GetSize() const { return m_uSize; }
private:
	BYTE m_data[MAX_SEND_SIZE_NEW];
	UINT m_uSize;
};

//�˴�Ϊ��Կ�ɿͻ��Լ�����������������Χ
#define SECRET_KEY  20160407
//�������ж�
#define NOENCRYPTION  20190722
///���ӳɹ���Ϣ
struct MSG_S_ConnectSuccess
{
    BYTE                        bMaxVer;                            ///���°汾����
    BYTE                        bLessVer;                           ///��Ͱ汾����
    BYTE                        bReserve[2];                        ///�����ֶ�
    UINT                        i64CheckCode;                       ///���ܺ��У���룬�ɿͻ��˽����ڰ�ͷ�з���
};
#pragma pack()

/********************************************************************************************/
/// ϵͳ�ں�ʹ�� ��MAINID ʹ�� 0 - 49 ��
/// ϵͳ����ͨ�� ��MAINID ʹ�� 50 - 99 ��
/// ��Ϸ�������� ��MAINID ʹ�� 100 - 149 ��
/// ������Ϸͨ�� ��MAINID ʹ�� 151 - 199 ��
/// ����ͨ�ű�־ ��MAINID ʹ�� 200 - 255 ��

/********************************************************************************************/

///������Ϣ
#define MDM_NO_FAILE                    0           ///��ʧ����Ϣ

///������Ϣ
#define MDM_CONNECT                     1           ///������Ϣ����

#define MDM_GP_CONNECT                  88           ///������Ϣ����
#define MDM_USER_OFFLLINE               89 //��ҵ���
#define MDM_USER_SUB_SIT                90 //�������
#define MDM_USER_SUB_UP                 91//����뿪
#define MDM_USER_KICKOUT                92//����
#define MDM_GATECONNECT                 98           ///������Ϣ����
#define MDM_G_SQLMESSAGE                      97
#define MDM_SQLMESSAGE                      99
///����������Ϣ��־
#define ASS_NET_TEST                    1           ///�������
#define ASS_CONNECT_SUCCESS             3           ///���ӳɹ�
#define ASS_NET_END                     4           ///�������ӽ�����Ϣ
#define ASS_NET_GET                     5           ///����������Կ
#define ASS_NET_CLOSE_SOCKET			6			//�������ͻ��˶�������ر���Ϣ
#define ASS_NET_CLOSE_SOCKETEX			7			//�������ͻ��˶�������ر���Ϣ
//#define ASS_CONNECT_SUCCESSSERVER       6           ///���ӳɹ�
/********************************************************************************************/

#endif