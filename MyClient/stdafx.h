// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define WINVER 0x0501		// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINDOWS 0x0410 // ��������Ϊ�ʺ� Windows Me ����߰汾����Ӧֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ���ض��� IE 6.0 ����߰汾�Ĺ��ܡ�
#define _WIN32_IE 0x0600	// ����ֵ����Ϊ��Ӧ��ֵ���������� IE �������汾��ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT


#include <afxsock.h>		// MFC �׽�����չ

#include "commonuse.h"
#include "centerserverhead.h"
#include "gameplacemessage.h"
#include "basemessage.h"
#include "comstruct.h"
#include "gameroomex.h"
#include "clientcomstruct.h"
#include "gameusermanage.h"
#include "gameroommessage.h"

#pragma comment(lib, "AFCSocket.lib")

// �����־��
#define ADD_LOG(szLog, ...)\
	CMyClientDlg::AddLog(szLog, __VA_ARGS__);

//#define GATE_IP			"127.0.0.1"
#define GATE_IP			"47.91.166.58"
#define GATE_PORT1		37115
#define GATE_PORT2		37116

#define RUN_CNT			10000

#define NAME_ID			40066000
#define ROOM_ID			188	
//#define NAME_ID			40060000
//#define ROOM_ID			159

//#define ACCOUNT			"BYPhone599990782"
//#define PWD				"103b70723e4dc645882f46323c2f6d44"

#define ACCOUNT			"BYPhone314401451"
#define PWD				"e10adc3949ba59abbe56e057f20f883e"

//#define ACCOUNT			"BYPhone603409715"
//#define PWD				"200820e3227815ed1756a6b531e7e0d2"

//#define ACCOUNT			"BYPhone419764998"

//#define ACCOUNT			"BYPhone625718351"
//#define ACCOUNT			"BYPhone703260004"
//#define PWD				"200820e3227815ed1756a6b531e7e0d2"

