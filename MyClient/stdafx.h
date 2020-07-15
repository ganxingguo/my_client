// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 头中排除极少使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows XP 或更高版本的功能。
#define WINVER 0x0501		// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0410 // 将它更改为适合 Windows Me 或更高版本的相应值。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 6.0 或更高版本的功能。
#define _WIN32_IE 0x0600	// 将此值更改为相应的值，以适用于 IE 的其他版本。值。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT


#include <afxsock.h>		// MFC 套接字扩展

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

// 添加日志宏
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

