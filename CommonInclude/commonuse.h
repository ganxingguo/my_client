#pragma once
#define MAX_STRING_SIZE 128
#include <map>

#include "Encrypt.h"
#include "BZZlib.h"
#include <algorithm>
#include <string>
#include <direct.h>
#include <Windows.h>

using namespace std;
//#include "DecryptFile.h"

///定义输出
#ifndef EXT_CLASS
#	ifdef  AFC_FACE_DLL				/// 客户端平台BZW.dll类库
#		define EXT_CLASS _declspec(dllexport)
#	elif defined(_AFC_COM_DLL_EX)	/// 服务器平台类库AfcComDll.dll
#		define EXT_CLASS _declspec(dllexport)
#	elif defined(THIS_IS_DLL)		/// 游戏框架DllForGame.dll
#		define EXT_CLASS _declspec(dllexport)
#	else
#		define EXT_CLASS _declspec(dllimport)
#	endif
#endif


enum
{
	UNICODE_CALC_SIZE = 1,
	UNICODE_GET_BYTES = 2
};

///
/// 节点基类
class CBcfListItem
{
public:
	std::map<CString, CBcfListItem*> m_ChildMap;
	CBcfListItem()
	{
		m_ChildMap.clear();
	}
	virtual ~CBcfListItem()
	{
		std::map<CString, CBcfListItem*>::iterator iter = m_ChildMap.begin();
		while (iter != m_ChildMap.end())
		{
			CBcfListItem* pListItem = iter->second;
			if (pListItem != NULL)
			{
				delete pListItem;
			}
			++iter;
		}
		m_ChildMap.clear();
	}
	virtual void SetData(void* pData,UINT uSize)=0;
};
/// 键值map
class CBcfKeyItem : public CBcfListItem
{
	CString m_strKey;
	CString m_strValue;
	CString m_strMemo;
	CBcfKeyItem();///默认构造函数是不允许执行的
public:
	CBcfKeyItem(CString szKey, CString szValue, CString szMemo) : m_strKey(szKey), m_strValue(szValue), m_strMemo(szMemo){}
	virtual void SetData(void* pData,UINT uSize){};
	CString GetValue(){return m_strValue;}
	CString GetMemo(){return m_strMemo;}
};
/// 段map
class CBcfSectionItem : public CBcfListItem
{
	CString m_strSection;	/// 用来保存这个段的名字
public:
	CBcfSectionItem(CString szSection) : m_strSection(szSection){}
	virtual void SetData(void* pData,UINT uSize){};
};
/// 文件map
class CBcfFileNameItem : public CBcfListItem
{
	CString m_strFileName;	/// 用来保存这个文件名字
public:
	CBcfFileNameItem(CString szFileName) : m_strFileName(szFileName){}
	virtual void SetData(void* pData,UINT uSize){};
};
/// 供使用者调用的类
class EXT_CLASS CBcfFile
{
	static std::map<CString, CBcfFileNameItem*> mapFile;
	CBcfFileNameItem *m_pFile;
	bool m_bIsFileExist;
public:

	static __int64 my_atoi(const char *str)
	{
		__int64 result = 0;
		int signal = 1;
		/* 默认为正数 */
		if((*str>='0'&&*str<='9')||*str=='-'||*str=='+')
		{
			if(*str=='-'||*str=='+')
		 {
			 if(*str=='-')
				 signal = -1; /* 输入负数 */
			 str++;
		 }
		}
		else return 0;/* 开始转换 */
		while(*str>='0'&&*str<='9')
			result = result*10+(*str++ -'0');
		return signal*result;
	}
	static void ClearMap()
	{
		std::map<CString, CBcfFileNameItem*>::iterator iter = mapFile.begin();
		while (iter != mapFile.end())
		{
			CBcfFileNameItem* pListItem = iter->second;
			if (pListItem != NULL)
			{
				delete pListItem;
			}
			++iter;
		}
		mapFile.clear();
	}
	static CString GetWinSysPath()
	{
		char lpBuffer[MAX_STRING_SIZE];
		GetSystemDirectory( lpBuffer, MAX_STRING_SIZE);
		CString ss=lpBuffer;
		ss += "\\";
		return ss;

	}
	static bool IsFileExist(CString fileName)
	{
		WIN32_FIND_DATA  d;
		HANDLE  hd= FindFirstFile(fileName,&d);
		bool bRet = (hd != INVALID_HANDLE_VALUE);
		FindClose(hd);
		return bRet;
	}
	static CString GetAppPath(bool bFource=true)
	{
		bFource = true;
		if(!bFource)
		{
			char lpBuffer[MAX_STRING_SIZE];
			ZeroMemory(lpBuffer,sizeof(lpBuffer));
			GetCurrentDirectory(MAX_STRING_SIZE,lpBuffer);
			CString ss=lpBuffer;
			ss += "\\";
			return ss;
		}
		else
		{
			TCHAR szModuleName[MAX_PATH];
			ZeroMemory(szModuleName,sizeof(szModuleName));
			DWORD dwLength=GetModuleFileName(GetModuleHandle(0) /*AfxGetInstanceHandle()*/,szModuleName,sizeof(szModuleName));
			CString filepath=szModuleName;
			CString path;
			int nSlash = filepath.ReverseFind(_T('/'));
			if (nSlash == -1)
				nSlash = filepath.ReverseFind(_T('\\'));
			if (nSlash != -1 && filepath.GetLength() > 1)
			{
				path = filepath.Left(nSlash+1);
				SetCurrentDirectory(path);
				return path;
			}
			else
			{
				char lpBuffer[MAX_STRING_SIZE];
				ZeroMemory(lpBuffer,sizeof(lpBuffer));
				GetCurrentDirectory(MAX_STRING_SIZE,lpBuffer);
				CString ss=lpBuffer;
				ss += "\\";
				return ss;
			}

		}
	}

	///// 构造函数，创建或者给成员赋值
	//CBcfFile(CString szFileName)
	//{
	//	if (!IsFileExist(szFileName))
	//	{
	//		m_bIsFileExist = false;
	//		return;
	//	}
	//	m_bIsFileExist = false;
	//	/// 如果不存在，就创建之
	//	m_pFile = mapFile[szFileName];
	//	if (NULL == m_pFile)
	//	{
	//		bool bRet = ReadBcfFile(szFileName);
	//		if (bRet)	/// 如果文件存在，则赋值
	//		{
	//			m_bIsFileExist = true;
	//			mapFile[szFileName] = m_pFile;
	//		}
	//	}
	//	else
	//	{
	//		m_bIsFileExist = true;
	//	}
	//}
	///// 析构函数，其中不能释放指针，否则会出现野指针
	//~CBcfFile()
	//{
	//	m_pFile = NULL;
	//}
	/// 设置键值，内存读取时无效
	void SetKeyValString(CString secName,CString keyName,CString Val){}
	/// 关闭文件，不需要关闭
	void CloseFile(){}
	/// 取整数值
	int GetKeyVal(CString secName,CString keyName,int lpDefault)
	{
		if (!m_bIsFileExist)
		{
			return lpDefault;
		}
		TCHAR szSec[MAX_PATH];
		TCHAR szKey[MAX_PATH];
		UpperString(szSec, secName.GetBuffer());
		UpperString(szKey, keyName.GetBuffer());
		CBcfSectionItem *pSectionItem = (CBcfSectionItem *)m_pFile->m_ChildMap[szSec];
		if (NULL != pSectionItem)
		{
			CBcfKeyItem *pKeyItem = (CBcfKeyItem *)pSectionItem->m_ChildMap[szKey];
			if (NULL != pKeyItem)
			{
				return atoi(pKeyItem->GetValue());
			}
		}
		return lpDefault;
	}

	__int64 GetKeyVal(CString secName,CString keyName,__int64 lpDefault)
	{

		char str[255];
		sprintf_s(str, "%I64d", lpDefault); 

		CString __int64Str=GetKeyVal(secName,keyName,str);
		return my_atoi(__int64Str);
		
	}
	/// 取字符串
	CString GetKeyVal(CString secName,CString keyName,LPCTSTR lpDefault)
	{
		if (!m_bIsFileExist)
		{
			return lpDefault;
		}
		TCHAR szSec[MAX_PATH];
		TCHAR szKey[MAX_PATH];
		UpperString(szSec, secName.GetBuffer());
		UpperString(szKey, keyName.GetBuffer());
		CBcfSectionItem *pSectionItem = (CBcfSectionItem *)m_pFile->m_ChildMap[szSec];
		if (NULL != pSectionItem)
		{
			CBcfKeyItem *pKeyItem = (CBcfKeyItem *)pSectionItem->m_ChildMap[szKey];
			if (NULL != pKeyItem)
			{
				return pKeyItem->GetValue();
			}
		}
		return lpDefault;
	}
	/// 取注释
	CString GetKeyMemo(CString secName,CString keyName,LPCTSTR lpDefault)
	{
		if (!m_bIsFileExist)
		{
			return lpDefault;
		}
		TCHAR szSec[MAX_PATH];
		TCHAR szKey[MAX_PATH];
		UpperString(szSec, secName.GetBuffer());
		UpperString(szKey, keyName.GetBuffer());
		CBcfSectionItem *pSectionItem = (CBcfSectionItem *)m_pFile->m_ChildMap[szSec];
		if (NULL != pSectionItem)
		{
			CBcfKeyItem *pKeyItem = (CBcfKeyItem *)pSectionItem->m_ChildMap[szKey];
			if (NULL != pKeyItem)
			{
				return pKeyItem->GetMemo();
			}
		}
		return lpDefault;
	}
private:
	/// 转换成大写字符
	TCHAR *UpperString(TCHAR *szDes, LPCTSTR szSrc)
	{
		if ((szDes == NULL) || (NULL == szSrc))
		{
			return NULL;
		}
		int nLen = _tcslen(szSrc);
		//_tcscpy_s(szDes, nLen+1, szSrc);
		_tcscpy_s(szDes, nLen*sizeof(TCHAR), szSrc);
		for (int i=0; i<nLen+1; ++i)
		{
			if (szDes[i]>='a' && szDes[i]<='z')
			{
				szDes[i] = szDes[i]-0x20;
			}
		}
		return szDes;
	}
	///// 只能在构造函数中当文件还没有读取时调用
	//bool ReadBcfFile(CString szFileName)
	//{
	//	DWORD hFileHandle = cfgOpenFile(szFileName);
	//	if(hFileHandle<0x10)
	//		return false;
	//	/// 打开文件成功，进行读取过程
	//	POSITION posSection = cfgFindFirstSectionPosition(hFileHandle);
	//	if (NULL==posSection)
	//	{
	//		return false;
	//	}

	//	m_pFile = new CBcfFileNameItem(szFileName);
	//	/// 循环外面声明循环里用到的变量
	//	LPCTSTR szAnyCaseSection,szAnyCaseKeyName,szKeyValue,szKeyMemo;
	//	TCHAR szSection[MAX_PATH], szKeyName[MAX_PATH];
	//	int		nKeyAttrib;
	//	while (NULL != posSection)
	//	{
	//		szAnyCaseSection = cfgFindNextSection(hFileHandle, posSection);	/// 读一次就会改变一次posSection，直到尾部时，变成NULL
	//		/// 把字符串转换成大写
	//		UpperString(szSection, szAnyCaseSection);
	//		/// 创建一个段
	//		CBcfSectionItem *pSectionItem = new CBcfSectionItem(szSection);
	//		/// 放到文件map中
	//		m_pFile->m_ChildMap[szSection] = pSectionItem;
	//		POSITION posKey = cfgFindFistKeyPosition(hFileHandle, szSection);
	//		while (NULL != posKey)
	//		{
	//			cfgFindNextKey(hFileHandle, szSection, posKey, szAnyCaseKeyName, szKeyValue, nKeyAttrib);
	//			UpperString(szKeyName, szAnyCaseKeyName);
	//			szKeyMemo = cfgGetKeyMemo(hFileHandle, szSection, szKeyName);
	//			CBcfKeyItem *pKeyItem = new CBcfKeyItem(szKeyName, szKeyValue, szKeyMemo);
	//			pSectionItem->m_ChildMap[szKeyName] = pKeyItem;
	//		}
	//	}
	//	cfgClose(hFileHandle);
	//	return true;
	//}
};

/// Modified by zxd 20090810 }}
///


class CINIFile  
{
  CString lpFileName ;
  DWORD mMaxSize ;
public:

	static __int64 my_atoi(const char *str)
	{
		__int64 result = 0;
		int signal = 1;
		/* 默认为正数 */
		if((*str>='0'&&*str<='9')||*str=='-'||*str=='+')
		{
			if(*str=='-'||*str=='+')
		 {
			 if(*str=='-')
				 signal = -1; /* 输入负数 */
			 str++;
		 }
		}
		else return 0;/* 开始转换 */
		while(*str>='0'&&*str<='9')
			result = result*10+(*str++ -'0');
		return signal*result;
	}
	static bool IsFileExist(CString fileName)
	{
		WIN32_FIND_DATA  d;
		HANDLE  hd= FindFirstFile(fileName,&d);
		bool bRet = (hd != INVALID_HANDLE_VALUE);
		FindClose(hd);
		return bRet;
	}
	static void CopyFileTo(CString destFileName,CString srcFileName)
	{
		bool bb=IsFileExist(srcFileName);
		if(!bb)return;
		CopyFile( srcFileName, destFileName, FALSE);

	}
		
	static CString GetWinSysPath()
	{
		char lpBuffer[MAX_STRING_SIZE];
		GetSystemDirectory( lpBuffer, MAX_STRING_SIZE);
		CString ss=lpBuffer;
		ss += "\\";
		return ss;

	}
		
	static CString GetAppPath(bool bFource=true)
	 {
         // PengJiLin, 2010-6-7, 固定使用第二种方式获取程序路径，
         // 第一种方式在某些情况下会出现问题。
         bFource = true;

		if(!bFource)
		{
			char lpBuffer[MAX_STRING_SIZE];
			ZeroMemory(lpBuffer,sizeof(lpBuffer));
			GetCurrentDirectory(MAX_STRING_SIZE,lpBuffer);
			CString ss=lpBuffer;
			ss += "\\";
			return ss;
		}
		else
		{
			TCHAR szModuleName[MAX_PATH];
			ZeroMemory(szModuleName,sizeof(szModuleName));
			DWORD dwLength=GetModuleFileName(GetModuleHandle(0) /*AfxGetInstanceHandle()*/,szModuleName,sizeof(szModuleName));
			CString filepath=szModuleName;
			CString path;
			int nSlash = filepath.ReverseFind(_T('/'));
			if (nSlash == -1)
				nSlash = filepath.ReverseFind(_T('\\'));
			if (nSlash != -1 && filepath.GetLength() > 1)
			{
				path = filepath.Left(nSlash+1);
				SetCurrentDirectory(path);
				return path;
			}
			else
			{
				char lpBuffer[MAX_STRING_SIZE];
				ZeroMemory(lpBuffer,sizeof(lpBuffer));
				GetCurrentDirectory(MAX_STRING_SIZE,lpBuffer);
				CString ss=lpBuffer;
				ss += "\\";
				return ss;
			}

		}
	}

	int GetKeyVal(CString secName,CString keyName,int lpDefault)
	{
		return (int) GetPrivateProfileInt(
					secName,/// points to section name
					keyName,/// points to key name
					lpDefault,       /// return value if key name not found
					lpFileName///LPCTSTR lpFileName  /// initialization file name
					);

	}

	UINT GetKeyVal(CString secName,CString keyName,UINT lpDefault)
	{
		return (UINT) GetPrivateProfileInt(
					secName,/// points to section name
					keyName,/// points to key name
					lpDefault,       /// return value if key name not found
					lpFileName///LPCTSTR lpFileName  /// initialization file name
					);

	}

	__int64 GetKeyVal(CString secName,CString keyName,__int64 lpDefault)
	{
		
		char str[255];
		sprintf_s(str, 255, "%I64d", lpDefault); 
	    CString __int64Str=GetKeyVal(secName,keyName,str);
		return my_atoi(__int64Str);
	}
	

	CString GetKeyVal(CString secName,CString keyName,LPCTSTR lpDefault)
	{ 
		char* re = new char[mMaxSize + 1];
		ZeroMemory(re,mMaxSize);
		GetPrivateProfileString(
								secName,/// points to section name
								keyName,/// points to key name
								lpDefault,/// points to default string
								re,/// points to destination buffer
								mMaxSize,/// size of destination buffer
								lpFileName /// points to initialization filename
							);
		CString ss=re;
		delete []re;
		return ss;
	}
		
	void SetKeyValString(CString secName,CString keyName,CString Val)
	{ 

		WritePrivateProfileString(
								secName,/// pointer to section name
								keyName,/// pointer to key name
								Val,/// pointer to string to add
								lpFileName/// pointer to initialization filename
								);

	}

		
	CINIFile(CString FileName,int maxsize=MAX_STRING_SIZE)
	{
		lpFileName=FileName;
		mMaxSize = maxsize;

	}

	~CINIFile()	{}
	void SetINIFileName(CString fileName){lpFileName=fileName;}
	
};

/*
template<class T>
static void CopyPtrList(CPtrList &dest,CPtrList &src)
{
   for(POSITION pos=src.GetHeadPosition ();pos !=NULL;)
	{
		T *pi=(T *)src.GetNext (pos);
		if(pi)
		{
			T *pii=new T(); *pii=*pi;
			dest.AddTail (pii);
		}
	}
}

template<class T> 
static void EmptyPtrList(CPtrList &src)
{
	 while(!src.IsEmpty ())
	{
		T *pi=(T *)src.RemoveHead  ();
		delete pi;
	}
	src.RemoveAll ();
}


template<class T>
static void AddToPtrListToTail(CPtrList &dest,T &add)
{
  T *pii=new T(); *pii=add;
  dest.AddTail (pii);
}

template<class T>
static void AddToPtrListToHead(CPtrList &dest,T &add)
{
  T *pii=new T();*pii=add;
  dest.AddHead (pii);
}
*/
#define MAX_PASSWORD_LENGTH    255
struct EXT_CLASS FileItem
{
	void * _pBufFile;
	ULONG  _nFileLen;
	unsigned char _digest[MAX_PASSWORD_LENGTH];
	FileItem()
	{
		_pBufFile=NULL;
		_nFileLen=0;
		ZeroMemory(_digest,MAX_PASSWORD_LENGTH);
	}
	~FileItem()
	{
		if(_pBufFile!=NULL)
		{
			delete []_pBufFile;
			_pBufFile=NULL;
		}
	}
};


class CMemIO
{
public:
	CMemIO(unsigned int ulSize)
	{
		m_pBuffer = new char[ulSize];
		m_ulSize = ulSize;
		m_ulSeek = NULL;
		m_bNeedDel = true;
	}
	CMemIO(const char *pBuffer, unsigned int ulSize)
	{
		m_pBuffer = (char*)pBuffer;
		m_ulSize = ulSize;
		m_ulSeek = NULL;
		m_bNeedDel = false;
	}
	~CMemIO()
	{
		if (m_bNeedDel)
			delete m_pBuffer;
	}
private:
	CMemIO(const CMemIO &other);
	CMemIO &operator = (const CMemIO &other);

public:
	int Write(char *pbuf, unsigned int ulSize)
	{
		if ((m_ulSize-m_ulSeek)<ulSize)
		{
			return 0;
		}
		memcpy(m_pBuffer+m_ulSeek, pbuf, ulSize);
		m_ulSeek += ulSize;
		return ulSize;
	}
	const char *GetMemBlock(unsigned int ulSize, unsigned int *ulSizeGetted)
	{
		if ((m_ulSize-m_ulSeek)<ulSize)
		{
			return NULL;
		}
		*ulSizeGetted = ulSize;
		char *pRet = m_pBuffer+m_ulSeek;
		m_ulSeek += ulSize;
		return pRet;
	}
	unsigned int Read(char *pDst, unsigned int ulSize)
	{
		if ((m_ulSize-m_ulSeek)<ulSize)
		{
			return 0;
		}        
		memcpy(pDst, m_pBuffer+m_ulSeek, ulSize);
		//pDst = m_pBuffer+m_ulSeek;
		m_ulSeek += ulSize;
		return ulSize;
	}
	unsigned int Read(char **pDst, unsigned int ulSize)
	{
		if ((m_ulSize-m_ulSeek)<ulSize)
		{
			return 0;
		}                
		*pDst = m_pBuffer+m_ulSeek;
		m_ulSeek += ulSize;
		return ulSize;
	}
	void SeekToBegin()
	{
		m_ulSeek = 0;
	}

private:
	char            *m_pBuffer;
	unsigned int   m_ulSize;
	unsigned int   m_ulSeek;
	bool            m_bNeedDel;

};
/**	@brief 该类用于将一个文件夹的内容写成一个文件，并带还原功能
*/
struct FileInfo
{
	char          szFileName[MAX_PATH];
	unsigned int ulStartPos;
	unsigned int ulLen;       
};

struct FilePos
{
	unsigned int ulStartPos;
	unsigned int ulLen;       
};

typedef std::map<std::string, FilePos> PackPosInfo;

struct Zipmen
{
	void *pBuffer;
	unsigned int uLen;
	PackPosInfo posInfo;
	Zipmen()
	{
		pBuffer = NULL;
		uLen = 0;
	}
};

/**
* @brief 得到打内存的PosInfo
*/
static void GetPosInfo(const char *pPacked, const unsigned int ulMemLen, PackPosInfo &posInfo)
{
	CMemIO mem(pPacked, ulMemLen);    
	mem.SeekToBegin();

	int nSize = 0;
	mem.Read((char*)&nSize, sizeof(nSize));

	FileInfo *pPos = new FileInfo[nSize];
	if (mem.Read((char**)&pPos, sizeof(FileInfo)*nSize)<sizeof(FileInfo)*nSize)
	{
		return ;
	}

	for (int i=0; i<nSize; ++i)
	{
		FilePos pos;
		pos.ulLen = pPos[i].ulLen;
		pos.ulStartPos = pPos[i].ulStartPos;
		std::string strFileName = pPos[i].szFileName;      
		transform(strFileName.begin(), strFileName.end(), strFileName.begin(), tolower);
		posInfo[strFileName] = pos;
	}
};

// static HRESULT ToIStream(void *pBuf,ULONG cb,IStream * & pStream)
//{
//	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb);
//
//	if(hGlobal == NULL)
//	{
//		return(FALSE);
//	}
//
//	void* pData = GlobalLock(hGlobal);
//	memcpy(pData, pBuf, cb);
//	GlobalUnlock(hGlobal);
//
//	return CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
//}
//static ULONG LoadFileFromZip(TCHAR *fileName,TCHAR *ZipFile,char **pBuf,char*password=NULL)
//{
//	return 0;
//}

//static ULONG LoadFileFromZip(TCHAR *fileName,void *BufZip,unsigned int lenBufZip,char **pBuf,char*password=NULL)
//{
//	memcpy(*pBuf, BufZip, lenBufZip);
//	return lenBufZip;
//}

///资源文件解密类,主要是读取图片文件。
/// 供使用者调用的类
class EXT_CLASS  CUnZipRes
{
	static std::map<CString,FileItem *> mapFile;
	bool m_bIsFileExist;
	static unsigned char m_bPassWord[MAX_PASSWORD_LENGTH];
public:
	CUnZipRes(){}
	~CUnZipRes(){}
	static void ClearMap()
	{
		std::map<CString,FileItem *>::iterator iter=mapFile.begin();
		while (iter != mapFile.end())
		{
			FileItem * pFileIterm = iter->second;
			if (pFileIterm != NULL)
			{
				delete pFileIterm;
			}
			++iter;
		}
		mapFile.clear();
	}
	
	static void SetPassWord(char * password)
	{
		memset(m_bPassWord,0,MAX_PASSWORD_LENGTH);
		if(password!=NULL)
		{
			memcpy(m_bPassWord,password,min(strlen(password) ,MAX_PASSWORD_LENGTH));
		}
	}

	static bool ReadFile(TCHAR * pPath, unsigned char* password)
	{
		CString strPath(pPath);
		strPath.Replace("/","\\");
		strPath.Replace("//","\\");
		strPath.Replace("\\\\","\\");
		strPath.Replace("Image","image");
		//DebugPrintf("传入文件名%s",pPath);

		CString strPathTmp=GetAppPath(false);
		strPathTmp=strPathTmp.Left(strPathTmp.GetLength()-1);  ///<取得当前进程路径
		int pos=strPath.Find(strPathTmp);
		CString strMap;
		char * pchar=NULL;
		CString strP;
		if(pos==-1)
		{
			///判断是上级目录的文件还是当前目录的文件
			strMap=strPathTmp;
			int pos=strPath.Find("..\\");
			CString strFileName=strPath;
			if(pos!=-1)  ///<如果是上级目录的文件，则在当前进程目录的路径下除去最后面一个文件夹
			{
				int  posMap=strPathTmp.ReverseFind('\\');
				strMap=strPathTmp.Left(posMap);
				strFileName=strPath.Right(strPath.GetLength()-3);
			}


			strP=strMap+"\\"+strFileName;     ///<将处理的过的路径再加上文件名为真正文件的全路径
			pchar=strP.GetBuffer(strP.GetLength() + 1);
		}else
		{
			pchar=strPath.GetBuffer(strPath.GetLength()+1);     ///<如果是传的全路径，则直接处理
			int  posMap=strPath.ReverseFind('\\');
			strMap=strPath.Left(posMap);
		}	
		
		///<如果是已经读取过的文件，则不再读取了
		if(mapFile.find(strMap)!=mapFile.end() )
		{
			return true;
		}

		unsigned char digest[MAX_PASSWORD_LENGTH];
		ZeroMemory(digest,MAX_PASSWORD_LENGTH);
		//encryptPW(password,strlen((char *)password),digest);
		FileItem * pFileItem = new FileItem();
		unsigned long len;
		pFileItem->_pBufFile = DecryptZipFile(pchar,(char*)password,&len);
		pFileItem->_nFileLen=len;


		Zipmen m_FileMem;
		GetPosInfo((char*)pFileItem->_pBufFile, len, m_FileMem.posInfo);
		std::map<std::string, FilePos>::iterator it = m_FileMem.posInfo.begin();
		for (;it != m_FileMem.posInfo.end();it++)
		{
			//char sz[MAX_PATH];
			//_stprintf(sz,"Package:%s",const_cast<char *>(it->first.c_str()));
			//OutputDebugString(sz);
			

			//char *pBuf = new char[it->second.ulLen]; 
			//memcpy(pBuf, ((BYTE*)pFileItem->_pBufFile + it->second.ulStartPos), it->second.ulLen);
		//m_FileMem.pBuffer = DecryptZipFile(pchar, (char*)password, &m_FileMem.uLen);
		//GetPosInfo((char*)m_FileMem.pBuffer, m_FileMem.uLen, m_FileMem.posInfo);
			FILE *s = NULL;
			string sf = "d:\\test\\" + it->first;

			for (int i = 0; i < (int)sf.size(); i++)
			{
				if (sf[i] == '\\')
				{
					{
						//ouf<<"创建目录："<<tmppath<<endl;
						_mkdir(sf.substr(0, i).c_str());
					}
				}
			}

			if( !(fopen_s(&s,sf.c_str(), "wb"))) 
			{
				//ouf<<"写文件："<<soufilename<<"失败"<<endl;
				////AfxMessageBox(sf.c_str());
				continue;
			}

			fwrite(((BYTE*)pFileItem->_pBufFile + it->second.ulStartPos), it->second.ulLen, 1, s);
			fclose(s);
		}

		if(len==0||pFileItem->_pBufFile==NULL)
		{
			return false;
		}
		memcpy(pFileItem->_digest,digest,MAX_PASSWORD_LENGTH);
		mapFile[strMap]=pFileItem;

		return true;
	}

	static bool IsFileExist(CString fileName)
	{
		//CString str=GetAppPath(false);
		//CString  strName=str+fileName;
		WIN32_FIND_DATA  d;
		HANDLE  hd= FindFirstFile(fileName,&d);
		bool bRet = (hd != INVALID_HANDLE_VALUE);
		FindClose(hd);
		return bRet;
	}

	static IStream * LoadFileResFromZip(TCHAR *fileName)
	{
		if(fileName==NULL)
		{
			return NULL;
		}
		CString strFile(fileName);
		//DebugPrintf("传入全路径%s",fileName);
		strFile.Replace("/","\\");  ///<将斜干统一成"\"
		strFile.Replace("//","\\");
		strFile.Replace("\\\\","\\");
		strFile.Replace("Image","image");
		//DebugPrintf("图片名称%s",fileName);

		std::map<CString,FileItem *>::iterator iter;
		///先判断是否是全路径，如果是，则直接取路径看在MAP中有没，如果有，则直接读该文件，如果没有，则进行其它判断
		int posAll=strFile.Find("image");
		if(posAll==-1)
		{
			return NULL;
		}
		CString strMapAll=strFile.Left(posAll-1);
		//DebugPrintf("传入中的Image左边部分%s",strMapAll);
		iter=mapFile.find(strMapAll);
		char * pchar=NULL;
		CString strP;
		if(iter==mapFile.end())///<如果本来就有该路径，说明传的就的全路径
		{

			CString strPathTmp=GetAppPath(false);   ///<获取当前进程路径
			int posfile=strFile.Find(strPathTmp);   
			if(posfile!=-1)///<判断传入路径包含磁盘路径没，若有，则减去
			{
				strFile=strFile.Right(strFile.GetLength()-strPathTmp.GetLength());
			}
			strPathTmp=strPathTmp.Left(strPathTmp.GetLength()-1);   ///<这是因为取得的当前进程路径最后会有一个“\”，所以长度减一
			
			

			///<下面是找出图片所在文件的内存文件名
			int pos1=strFile.Find("image");
			strP=strFile.Right(strFile.GetLength()-pos1);
			pchar=strP.GetBuffer(strP.GetLength() + 1);   ///<先读出文件名，保存

			int pos=strFile.Find("..\\"); 
			CString strMap;
			if(pos!=-1)  ///<如果是上级目录，则将最后面的一个文件夹除去
			{
				int  posMap=strPathTmp.ReverseFind('\\');
				strMap=strPathTmp.Left(posMap);
				iter=mapFile.find(strMap);

			}else
			{
				CString strAll=GetAllPath(strFile); ///<获取全路径
				int posTmp=strAll.Find("image");
				strMap=strAll.Left(posTmp-1);
				iter=mapFile.find(strMap);
				if(iter==mapFile.end())
				{
					CString strMapAll=strMap+"\\"+"image.r";
					TCHAR * pTmp=strMapAll.GetBuffer(strMapAll.GetLength()+1);
					ReadFile(pTmp,m_bPassWord);
					iter=mapFile.find(strMap);
				}
				//iter=mapFile.find(strPathTmp);
			} 

			
		}else
		{
			int pos1=strFile.Find("image");
			strP=strFile.Right(strFile.GetLength()-pos1);
			pchar=strP.GetBuffer(strP.GetLength() + 1);
		}


		if(iter==mapFile.end())
		{
			return NULL;
		}

		///<如果找到了，则将图片相对路径找出来
		//int pos1=strFile.Find("image");
		//CString strP=strFile.Right(strFile.GetLength()-pos1);
		//TCHAR * pchar=strP.GetBuffer(strP.GetLength() + 1);
		//DebugPrintf("图片相对路径%s",pchar);
		//CString strPath(pchar);



		void * pBufFile=iter->second->_pBufFile;
		unsigned int len=iter->second->_nFileLen;
		ULONG size=0;
		unsigned char digest[MAX_PASSWORD_LENGTH]={0};
		memcpy(digest,iter->second->_digest,MAX_PASSWORD_LENGTH);
		char *pBuf = NULL;
		/*int pos=strFile.Find("image");*/
		//strFile=strFile.Right(strFile.GetLength()-pos);
		//TCHAR * pcharPath=strFile.GetBuffer(strPath.GetLength() + 1);  ///<剥去当前EXE的路径，取出相对路径来找图片内存
		if(len==0||pBufFile==NULL)
		{
			return NULL;
		}
		//size = LoadFileFromZip(pchar,pBufFile,len,&pBuf,(char*)digest);
		Zipmen m_FileMem;
		GetPosInfo((char*)pBufFile, len, m_FileMem.posInfo);

		string sTolower = pchar;
        transform(sTolower.begin(), sTolower.end(), sTolower.begin(), tolower); 
		std::map<std::string, FilePos>::iterator it = m_FileMem.posInfo.find(sTolower);

		if (it != m_FileMem.posInfo.end())
		{
			char *pBuf = new char[it->second.ulLen]; 
			memcpy(pBuf, ((BYTE*)pBufFile + it->second.ulStartPos), it->second.ulLen);
			IStream *pStream = NULL;
			ToIStream(pBuf,it->second.ulLen,pStream);
			if(pBuf!=NULL)
			{
				delete []pBuf;
				pBuf=NULL;
			}
			return pStream;
		}
		return NULL;

		//DebugPrintf("读取图片大小%d",size);
		if(size==0 || pBuf==NULL)
		{
			return NULL;
		}
		IStream *pStream = NULL;
		ToIStream(pBuf,size,pStream);
		if(pBuf!=NULL)
		{
			delete []pBuf;
			pBuf=NULL;
		}
		return pStream;
	}
	
	static char * LoadFileBufferFromZip(TCHAR *fileName,int & BuffLen)
	{
		if(fileName==NULL)
		{
			return NULL;
		}
		CString strFile(fileName);
		//DebugPrintf("传入全路径%s",fileName);
		strFile.Replace("/","\\");  ///<将斜干统一成"\"
		strFile.Replace("//","\\");
		strFile.Replace("\\\\","\\");
		strFile.Replace("Image","image");
		//DebugPrintf("图片名称%s",fileName);

		std::map<CString,FileItem *>::iterator iter;
		///先判断是否是全路径，如果是，则直接取路径看在MAP中有没，如果有，则直接读该文件，如果没有，则进行其它判断
		int posAll=strFile.Find("image");
		if(posAll==-1)
		{
			return NULL;
		}
		CString strMapAll=strFile.Left(posAll-1);
		//DebugPrintf("传入中的Image左边部分%s",strMapAll);
		iter=mapFile.find(strMapAll);
		char  * pchar=NULL;
		CString strP;
		if(iter==mapFile.end())///<如果本来就有该路径，说明传的就的全路径
		{

			CString strPathTmp=GetAppPath(false);   ///<获取当前进程路径
			int posfile=strFile.Find(strPathTmp);   
			if(posfile!=-1)///<判断传入路径包含磁盘路径没，若有，则减去
			{
				strFile=strFile.Right(strFile.GetLength()-strPathTmp.GetLength());
			}
			strPathTmp=strPathTmp.Left(strPathTmp.GetLength()-1);   ///<这是因为取得的当前进程路径最后会有一个“\”，所以长度减一



			///<下面是找出图片所在文件的内存文件名
			int pos1=strFile.Find("image");
			strP=strFile.Right(strFile.GetLength()-pos1);
			pchar=strP.GetBuffer(strP.GetLength() + 1);   ///<先读出文件名，保存

			int pos=strFile.Find("..\\"); 
			CString strMap;
			if(pos!=-1)  ///<如果是上级目录，则将最后面的一个文件夹除去
			{
				int  posMap=strPathTmp.ReverseFind('\\');
				strMap=strPathTmp.Left(posMap);
				iter=mapFile.find(strMap);

			}else
			{
				CString strAll=GetAllPath(strFile); ///<获取全路径
				int posTmp=strAll.Find("image");
				strMap=strAll.Left(posTmp-1);
				iter=mapFile.find(strMap);
				if(iter==mapFile.end())
				{
					CString strMapAll=strMap+"\\"+"image.r";
					TCHAR * pTmp=strMapAll.GetBuffer(strMapAll.GetLength()+1);
					ReadFile(pTmp,m_bPassWord);
					iter=mapFile.find(strMap);
				}
				//iter=mapFile.find(strPathTmp);
			} 


		}else
		{
			int pos1=strFile.Find("image");
			strP=strFile.Right(strFile.GetLength()-pos1);
			pchar=strP.GetBuffer(strP.GetLength() + 1);
		}


		if(iter==mapFile.end())
		{
			return NULL;
		}

		///<如果找到了，则将图片相对路径找出来
		//int pos1=strFile.Find("image");
		//CString strP=strFile.Right(strFile.GetLength()-pos1);
		//TCHAR * pchar=strP.GetBuffer(strP.GetLength() + 1);
		//DebugPrintf("图片相对路径%s",pchar);
		//CString strPath(pchar);



		void * pBufFile=iter->second->_pBufFile;
		UINT len=iter->second->_nFileLen;
		ULONG size=0;
		unsigned char digest[MAX_PASSWORD_LENGTH]={0};
		memcpy(digest,iter->second->_digest,MAX_PASSWORD_LENGTH);
		char *pBuf = NULL;
		/*int pos=strFile.Find("image");*/
		//strFile=strFile.Right(strFile.GetLength()-pos);
		//TCHAR * pcharPath=strFile.GetBuffer(strPath.GetLength() + 1);  ///<剥去当前EXE的路径，取出相对路径来找图片内存
		if(pBufFile==NULL || len==0)
		{
			return NULL;
		}
		//size = LoadFileFromZip(pchar,pBufFile,len,&pBuf,(char*)digest);

		Zipmen m_FileMem;
		GetPosInfo((char*)pBufFile, len, m_FileMem.posInfo);
		std::map<std::string, FilePos>::iterator it = m_FileMem.posInfo.find(pchar);
		if (it != m_FileMem.posInfo.end())
		{
			char *pBuf = new char[it->second.ulLen]; 
			memcpy(pBuf, ((BYTE*)pBufFile + it->second.ulStartPos), it->second.ulLen);
			return pBuf;
		}
		return NULL;
		
		//DebugPrintf("读取图片大小%d",size);
		if(size==0 || pBuf==NULL)
		{
			return NULL;
		}
		BuffLen=size;
		return pBuf;
	}



	static CString GetAppPath(bool bFource=true)
	{
		bFource = true;
		if(!bFource)
		{
			TCHAR lpBuffer[1024];
			ZeroMemory(lpBuffer,sizeof(lpBuffer));
			GetCurrentDirectory(1024,lpBuffer);
			CString ss=lpBuffer;
			ss += "\\";
			return ss;
		}
		else
		{
			TCHAR szModuleName[MAX_PATH];
			ZeroMemory(szModuleName,sizeof(szModuleName));
			DWORD dwLength=GetModuleFileName(GetModuleHandle(0) /*AfxGetInstanceHandle()*/,szModuleName,sizeof(szModuleName));
			CString filepath=szModuleName;
			CString path;
			int nSlash = filepath.ReverseFind(_T('/'));
			if (nSlash == -1)
				nSlash = filepath.ReverseFind(_T('\\'));
			if (nSlash != -1 && filepath.GetLength() > 1)
			{
				path = filepath.Left(nSlash+1);
				SetCurrentDirectory(path);
				return path;
			}
			else
			{
				TCHAR lpBuffer[1024];
				ZeroMemory(lpBuffer,sizeof(lpBuffer));
				GetCurrentDirectory(1024,lpBuffer);
				CString ss=lpBuffer;
				ss += "\\";
				return ss;
			}

		}
	}
	
	static CString GetAllPath(CString strCurPath)
	{
		CString strPathTmp=GetAppPath(false);   ///<获取当前进程路径
		strPathTmp=strPathTmp.Left(strPathTmp.GetLength()-1);   ///<这是因为取得的当前进程路径最后会有一个“\”，所以长度减一
		
		int posCur=strCurPath.Find(strPathTmp);
		
		if(posCur!=-1)
		{
			strCurPath.Replace("\\\\","\\");
			return strCurPath;
		}else
		{
			int pos=strCurPath.Find("..\\");  
			if(pos!=-1)  ///<如果是上级目录，则将最后面的一个文件夹除去
			{
				int  posMap=strPathTmp.ReverseFind('\\');
				strPathTmp=strPathTmp.Left(posMap);
			}
			int posfile=strCurPath.Find(".\\");
			if(posfile!=-1)
			{
				strCurPath=strCurPath.Right(strCurPath.GetLength()-posfile-1);
			}
			CString strAll=strPathTmp+"\\"+strCurPath;

			strAll.Replace("\\\\","\\");
			return strAll;
		}
	}
};
typedef union _uacode
{
	struct {
		BYTE LowByte;
		BYTE HighByte;
	}DUMMYSTRUCTNAME;
	struct {
		BYTE LowByte;
		BYTE HighByte;
	} u;
	wchar_t ch;
}UACODE;
//字符转码类
class CChineseCode
{

public:

	string Unicode2AnsiCode(wstring str)
	{
		const wchar_t *p = str.c_str();
		UACODE ua;
		string sResult(str.length()*6+1,'\0');
		char*  pa = &sResult[0];
		for(wstring::const_iterator it = str.begin();it!=str.end();it++)
		{
			ua.ch = *it;
			if(ua.HighByte)
			{
				sprintf_s(pa,7,"\\u%02x%02x",ua.HighByte,ua.LowByte);
				pa+=6;
			}else{
				*pa = ua.LowByte;
				pa++;
			}

		}
		return sResult.c_str();
	}


	int  U8ToUnicodeChar(char* szU8,char *m_char,int pSize)
	{
		//UTF8 to Unicode
		//预转换，得到所需空间的大小
		int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, -1, NULL, 0);
		//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
		wchar_t* wszString = new wchar_t[wcsLen + 1];
		//转换
		::MultiByteToWideChar(CP_UTF8, NULL, szU8, -1, wszString, wcsLen);
		//最后加上'\0'
		wszString[wcsLen] = '\0';

		
		int len = WideCharToMultiByte(CP_ACP, 0, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
		if (pSize < len+1)
			return 0;
		WideCharToMultiByte(CP_ACP, 0, wszString, wcslen(wszString), m_char, len, NULL, NULL);
		delete[]wszString;// psText的清除
		m_char[len] = '\0';
		return len;
	}
	void Utf8toUnicode(char *utf, int &nSize,wchar_t pwText[],int pSize)
	{
		if(!utf || !strlen(utf))
		{
			nSize = 0;
			return ;
		}

		int dwUnicodeLen = MultiByteToWideChar(CP_UTF8, 0, utf, -1, NULL, 0);
		int num = (1+dwUnicodeLen) * sizeof(wchar_t);
		if (pSize<num)
			return ;
		memset(pwText, 0, num);
		MultiByteToWideChar(CP_UTF8, 0, utf, -1, pwText, dwUnicodeLen);
		pwText[ dwUnicodeLen] = '\0';
		nSize = dwUnicodeLen - 1;
	}

	int UnicodeToUtf8(wchar_t *unicode,char szUtf8[],int size)
	{
		int len;
		len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
		if (size<len + 1)
			return 0;
		memset(szUtf8, 0, len + 1);
		len=WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, len, NULL, NULL);
		return len;
	}

	void Wchar_tToString(std::string& szDst, wchar_t *wchar)
	{
		wchar_t * wText = wchar;
		DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,wText,-1,NULL,0,NULL,FALSE);// WideCharToMultiByte的运用
		char *psText; // psText为char*的临时数组，作为赋值给std::string的中间变量
		psText = new char[dwNum];
		WideCharToMultiByte (CP_OEMCP,NULL,wText,-1,psText,dwNum,NULL,FALSE);// WideCharToMultiByte的再次运用
		szDst = psText;// std::string赋值
		delete []psText;// psText的清除
	}

	void UTF_8ToUnicode(wchar_t* pOut, char *pText)

	{

		char* uchar = (char *)pOut;

		uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);

		uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);

		return;

	}

	void UnicodeToUTF_8(char* pOut, wchar_t* pText)
	{

		// 注意 WCHAR高低字的顺序,低字节在前，高字节在后

		char* pchar = (char *)pText;

		pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));

		pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);

		pOut[2] = (0x80 | (pchar[0] & 0x3F));

		return;

	}

	void UnicodeToGB2312(char* pOut, wchar_t uData[],int &size)
	{
		UINT nCodePage = 936; //GB2312
		//WideCharToMultiByte(CP_ACP, NULL, &uData, 1, pOut, sizeof(wchar_t), NULL, NULL);
		INT dwNum = WideCharToMultiByte(nCodePage,0,uData,-1,NULL,0,NULL,NULL);
		if ((INT)(size*sizeof(wchar_t))<dwNum + 1)
		{
			
			return ;
		}
		memset(pOut, 0, dwNum + 1);
		dwNum = WideCharToMultiByte(nCodePage,0,uData,-1,pOut,dwNum,NULL,NULL);
		size=dwNum;
		return;

	}

	void Gb2312ToUnicode(wchar_t* pOut, char *gbBuffer,int &size)
	{
		UINT nCodePage = 936; //GB2312
		//::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, gbBuffer, 2, pOut, 1);
		int dwNum = MultiByteToWideChar(nCodePage, 0, gbBuffer, -1, NULL, 0);
		if (size<(INT)(dwNum*sizeof(wchar_t)))
		{
			return ;
		}

		dwNum = MultiByteToWideChar(nCodePage ,0 ,gbBuffer ,-1 ,pOut ,dwNum );
		size=dwNum;
		return ;

	}

	int  Utf8ToGb2312(char *p,char psText[],int size)
	{
		int dwNum = MultiByteToWideChar(CP_UTF8, 0, p, -1, NULL, 0);

		wchar_t *pwText = (wchar_t*)malloc(dwNum * sizeof(wchar_t));
		dwNum = MultiByteToWideChar(CP_UTF8, 0, p, -1, pwText, dwNum);
		dwNum = WideCharToMultiByte(CP_ACP, 0, pwText, -1, NULL, 0, NULL, NULL);

		if (size<dwNum + 1)
		{
			free(pwText);
			pwText=NULL;
			return 0;
		}
		memset(psText, 0, dwNum + 1);
		WideCharToMultiByte(CP_ACP, 0, pwText, -1, psText, dwNum, NULL, NULL);
		free(pwText);
		pwText=NULL;
		return dwNum;
	}

	int  Gb2312ToUtf8(const char *p,char psText[],int size)
	{
		int dwNum = MultiByteToWideChar(CP_ACP, 0, p, -1, NULL, 0);
		wchar_t *pwText = (wchar_t*)malloc(dwNum*sizeof(wchar_t));
		dwNum = MultiByteToWideChar(CP_ACP ,0 ,p ,-1 ,pwText ,dwNum );
		dwNum = WideCharToMultiByte(CP_UTF8,0,pwText,-1,NULL,0,NULL,NULL);
		if (size<dwNum + 1)
		{
			free(pwText);
			pwText=NULL;
			return 0;
		}
		memset(psText, 0, dwNum + 1);
		dwNum = WideCharToMultiByte(CP_UTF8,0,pwText,-1,psText,dwNum,NULL,NULL);
		free(pwText);
		pwText=NULL;
		return dwNum;
	}


	BOOL IsTextUTF8(char* str,ULONGLONG length)
	{
		int i;
		DWORD nBytes=0;//UFT8可用1-6个字节编码,ASCII用一个字节
		UCHAR chr;
		BOOL bAllAscii=TRUE; //如果全部都是ASCII, 说明不是UTF-8
		for(i=0;i<length;i++)
		{
			chr= *(str+i);
			if( (chr&0x80) != 0 ) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
				bAllAscii= FALSE;
			if(nBytes==0) //如果不是ASCII码,应该是多字节符,计算字节数
			{
				if(chr>=0x80)
				{
					if(chr>=0xFC&&chr<=0xFD)
						nBytes=6;
					else if(chr>=0xF8)
						nBytes=5;
					else if(chr>=0xF0)
						nBytes=4;
					else if(chr>=0xE0)
						nBytes=3;
					else if(chr>=0xC0)
						nBytes=2;
					else
					{
						return FALSE;
					}
					nBytes--;
				}
			}
			else //多字节符的非首字节,应为 10xxxxxx
			{
				if( (chr&0xC0) != 0x80 )
				{
					return FALSE;
				}
				nBytes--;
			}
		}
		if( nBytes > 0 ) //违返规则
		{
			return FALSE;
		}
		if( bAllAscii ) //如果全部都是ASCII, 说明不是UTF-8
		{
			return FALSE;
		}
		return TRUE;
	}





	unsigned int utf8_decode( char *s, unsigned int *pi )
	{
		unsigned int c;
		int i = *pi;
		/* one digit utf-8 */
		if ((s[i] & 128)== 0 ) {
			c = (unsigned int) s[i];
			i += 1;
		} else if ((s[i] & 224)== 192 ) { /* 110xxxxx & 111xxxxx == 110xxxxx */
			c = (( (unsigned int) s[i] & 31 ) << 6) +
				( (unsigned int) s[i+1] & 63 );
			i += 2;
		} else if ((s[i] & 240)== 224 ) { /* 1110xxxx & 1111xxxx == 1110xxxx */
			c = ( ( (unsigned int) s[i] & 15 ) << 12 ) +
				( ( (unsigned int) s[i+1] & 63 ) << 6 ) +
				( (unsigned int) s[i+2] & 63 );
			i += 3;
		} else if ((s[i] & 248)== 240 ) { /* 11110xxx & 11111xxx == 11110xxx */
			c =  ( ( (unsigned int) s[i] & 7 ) << 18 ) +
				( ( (unsigned int) s[i+1] & 63 ) << 12 ) +
				( ( (unsigned int) s[i+2] & 63 ) << 6 ) +
				( (unsigned int) s[i+3] & 63 );
			i+= 4;
		} else if ((s[i] & 252)== 248 ) { /* 111110xx & 111111xx == 111110xx */
			c = ( ( (unsigned int) s[i] & 3 ) << 24 ) +
				( ( (unsigned int) s[i+1] & 63 ) << 18 ) +
				( ( (unsigned int) s[i+2] & 63 ) << 12 ) +
				( ( (unsigned int) s[i+3] & 63 ) << 6 ) +
				( (unsigned int) s[i+4] & 63 );
			i += 5;
		} else if ((s[i] & 254)== 252 ) { /* 1111110x & 1111111x == 1111110x */
			c = ( ( (unsigned int) s[i] & 1 ) << 30 ) +
				( ( (unsigned int) s[i+1] & 63 ) << 24 ) +
				( ( (unsigned int) s[i+2] & 63 ) << 18 ) +
				( ( (unsigned int) s[i+3] & 63 ) << 12 ) +
				( ( (unsigned int) s[i+4] & 63 ) << 6 ) +
				( (unsigned int) s[i+5] & 63 );
			i += 6;
		} else {
			c = '?';
			i++;
		}
		*pi = i;
		return c;
	}
	std::string UrlEncode(const std::string& src)
	{
		static    char hex[] = "0123456789ABCDEF";
		std::string dst;

		for (size_t i = 0; i < src.size(); i++)
		{
			unsigned char ch = src[i];
			if (isalnum(ch))
			{
				dst += ch;
			}
			else
				if (src[i] == ' ')
				{
					dst += '+';
				}
				else
				{
					unsigned char c = static_cast<unsigned char>(src[i]);
					dst += '%';
					dst += hex[c / 16];
					dst += hex[c % 16];
				}
		}
		return dst;
	}
	std::string UrlDecode(const std::string& src)
	{
		std::string dst, dsturl;
		size_t srclen = src.size();
		for (size_t i = 0; i < srclen; i++)
		{
			if (src[i] == '%')
			{
				if(isxdigit(src[i + 1]) && isxdigit(src[i + 2]))
				{
					char c1 = src[++i];
					char c2 = src[++i];
					c1 = c1 - 48 - ((c1 >= 'A') ? 7 : 0) - ((c1 >= 'a') ? 32 : 0);
					c2 = c2 - 48 - ((c2 >= 'A') ? 7 : 0) - ((c2 >= 'a') ? 32 : 0);
					dst += (unsigned char)(c1 * 16 + c2);
				}
			}
			else
				if (src[i] == '+')
				{
					dst += ' ';
				}
				else
				{
					dst += src[i];
				}
		}
		size_t len = dst.size();

		for(unsigned int pos = 0; pos < len;)
		{
			unsigned int nvalue = utf8_decode((char *)dst.c_str(), &pos);
			dsturl += (unsigned char)nvalue;
		}
		return dsturl;
	}


	//将unicode转义字符序列转换为内存中的unicode字符串
	int unicode_bytes(char* p_unicode_escape_chars,wchar_t *bytes,int flag)
	{


		int unicode_count=0;
		int length=strlen(p_unicode_escape_chars);
		for (int char_index=0;char_index<length;char_index++)
		{
			char unicode_hex[5];
			memset(unicode_hex,0,5);

			char ascii[2];
			memset(ascii,0,2);

			if (*(p_unicode_escape_chars+char_index)=='\\')
			{
				char_index++;
				if (char_index<length)
				{
					if (*(p_unicode_escape_chars+char_index)=='u')
					{
						if (flag==UNICODE_GET_BYTES)
						{
							memcpy(unicode_hex,p_unicode_escape_chars+char_index+1,4);

							//sscanf不可以使用unsigned short类型
							//否则：Run-Time Check Failure #2 - Stack around the variable 'a' was corrupted.
							char a=0;
							sscanf_s(unicode_hex,"%04x",&a);
							bytes[unicode_count++]=a;
						}
						else if(flag==UNICODE_CALC_SIZE)
						{
							unicode_count++;
						}
						char_index+=4;
					}
					//\"要保留
					else if (*(p_unicode_escape_chars+char_index)=='\"')
					{
						if (flag==UNICODE_GET_BYTES)
						{
							memcpy(ascii,p_unicode_escape_chars+char_index-1,1);
							char a=0;
							sscanf_s(ascii,"%c",&a, sizeof(a));
							bytes[unicode_count++]=a;

							memset(ascii,0,2);
							memcpy(ascii,p_unicode_escape_chars+char_index,1);
							sscanf_s(ascii,"%c",&a,sizeof(a));
							bytes[unicode_count++]=a;
						}
						else  if(flag==UNICODE_CALC_SIZE)
						{
							unicode_count++;
							unicode_count++;
						}

					}
				}
			}
			else
			{
				if (flag==UNICODE_GET_BYTES)
				{
					memcpy(ascii,p_unicode_escape_chars+char_index,1);
					char a=0;
					sscanf_s(ascii,"%c",&a,sizeof(a));
					bytes[unicode_count++]=a;
				}
				else if(flag==UNICODE_CALC_SIZE)
				{
					unicode_count++;
				}
			}
		}

		return unicode_count;
	}

	void UnEscape(char* p_unicode_escape_chars,WCHAR p_bytes[] )
	{
		int nBytes=unicode_bytes(p_unicode_escape_chars,NULL,UNICODE_CALC_SIZE);

		unicode_bytes(p_unicode_escape_chars,p_bytes,UNICODE_GET_BYTES);
		p_bytes[nBytes]=0;


	}

	void UnicodeZhuanUtf8(char szUtf8[],int &Index)
	{

		//临时保存下原数组的长度
		int TempIndex=Index;
		//wchar_t数组
		WCHAR		temp[100000];
		ZeroMemory(temp,sizeof(temp));

		UnEscape(szUtf8,temp);

		//memcpy((void*)temp,szUtf8,Index);
		//目标数组
		char         Info[100000]; 
		ZeroMemory(Info,sizeof(Info));
		Index = WideCharToMultiByte(CP_UTF8, 0, temp, -1, NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_UTF8, 0, temp, -1, Info, Index, NULL, NULL);
		ZeroMemory(szUtf8,TempIndex);
		//拷贝到输出数组
		memcpy(szUtf8,Info,Index);

	}

};