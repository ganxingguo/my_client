
#ifndef _BZZLIB_H
#define _BZZLIB_H

#include "Encrypt.h"

// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� BZZLIB_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// BZZLIB_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef BZZLIB_EXPORTS
#define BZZLIB_API __declspec(dllexport)
#else
#define BZZLIB_API __declspec(dllimport)
#endif

//BZZLIB_API ULONG LoadFileFromZip(TCHAR *fileName,TCHAR *ZipFile,char **pBuf,char*password=NULL);
//BZZLIB_API ULONG LoadFileFromZip(char *fileName,void *BufZip,unsigned int lenBufZip,char **pBuf,char*password=NULL);
BZZLIB_API void * DecryptZipFile(char *fileName,char*passWord,unsigned long *pLen);
BZZLIB_API HRESULT ToIStream(void *pBuf,ULONG cb,IStream * & pStream);
#endif
