// head.h : ���ķ���������֮�����Ŀͻ��˹�������

#pragma once

#define  CenterType_ClientSend  0xCE9DE9FEC885411d
#define CenterType_ClientEnter 0x831E2B518659DECF

class CCenterHead
{
    public:
        enum enumCenterType
        {
            CenterServerPort = 65320 //6688
        };
        class Message
        {
            public:
                int m_is_haveZhuanZhang;
                char m_strGameSerialNO[20];////�ͻ��˵�ǰ�汾ϵ�к�
                char m_strMainserverIPAddr[40];         ////��������IP��ַ
                UINT m_iMainserverPort;                 ////���������˿ں�
                char m_strWebRootADDR[128],             ////��վ��·�����ڳ������漰���ļ���Ŀ¼���������ַ����չ
                     m_strHomeADDR[128],                    ////������¼���һ����ʾ��ҳ��
                     m_strHelpADDR[128],                    ////����ҳWEB��ַ
                     m_strDownLoadSetupADDR[128],       ////�ͻ��˰�װ��������ҳWEB��ַ
                     m_strDownLoadUpdatepADDR[128],     ////�ͻ��˸��³�������ҳWEB��ַ
                     m_strRallAddvtisFlashADDR[128],        //�ͻ��˴���FLASH�������ҳWEB��ַ
                     m_strRoomRollADDR[200];            //�ͻ��˴����������

                int  m_nHallInfoShowClass;
                int  m_nEncryptType;                    //ƽ̨�����õļ��ܷ�ʽ��1λMD5��2λSHA1��Ĭ��Ϊ2; 2009-5-30 zxj
                UINT m_nFunction;               ///< ���ܿ��� Y.C.YAN
                long m_lNomalIDFrom;                    ///< ���«��ɢ��ID��ʼ
                long m_lNomalIDEnd;                     ///< ���«��ɢ��ID����
                int  m_nIsUsingIMList;                  ///< �Ƿ�ʹ�ú����б�

				UINT m_iGameserverPort;                 ////������������˿ں�
                Message()
                {
                    m_iMainserverPort    = 0;
                    m_nHallInfoShowClass = 0;
                    m_nEncryptType       = 1;                   //ƽ̨�����õļ��ܷ�ʽ��1λMD5��2λSHA1��Ĭ��Ϊ2; 2009-5-30 zxj
                    m_nFunction          = 0;
                    m_nIsUsingIMList     = 1;                   //Ĭ��Ϊ��ʾ�����б�
                    memset(m_strGameSerialNO, 0, sizeof(m_strGameSerialNO));
                    memset(m_strMainserverIPAddr, 0, sizeof(m_strMainserverIPAddr));
                    memset(m_strHomeADDR, 0, sizeof(m_strHomeADDR));
                    memset(m_strWebRootADDR, 0, sizeof(m_strWebRootADDR));
                    memset(m_strHelpADDR, 0, sizeof(m_strHelpADDR));
                    memset(m_strDownLoadSetupADDR, 0, sizeof(m_strDownLoadSetupADDR));
                    memset(m_strDownLoadUpdatepADDR, 0, sizeof(m_strDownLoadUpdatepADDR));
                    memset(m_strRallAddvtisFlashADDR, 0, sizeof(m_strRallAddvtisFlashADDR));
                    memset(m_strRoomRollADDR, 0, sizeof(m_strRoomRollADDR));

                }
                ~Message() {}
        };
};

typedef   CCenterHead::Message     CenterServerMsg;

