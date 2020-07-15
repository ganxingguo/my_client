#include "StdAfx.h"
#include "GameListCtrl.h"

//��������������
bool CGameListCtrl::UpdateOnLineCount()
{
	return true;
}

//��������
bool CGameListCtrl::UpdateNameOnLineData(DL_GP_RoomListPeoCountStruct* arrInfos, UINT nCnt)
{
	for (int i = 0; i < nCnt; ++i)
	{
		if (m_mapGameName.find(arrInfos[i].uID) != m_mapGameName.end())
		{
			m_mapGameName[arrInfos[i].uID]->m_uOnLineCount = arrInfos[i].uOnLineCount;
		}
	}

    UpdateOnLineCount();
	return true;
}

//��������
bool CGameListCtrl::UpdateRoomOnLineData(DL_GP_RoomListPeoCountStruct* arrInfos, UINT nCnt)
{
	for (int i = 0; i < nCnt; ++i)
	{
		if (m_mapGameRoom.find(arrInfos[i].uID) != m_mapGameRoom.end())
		{
			m_mapGameRoom[arrInfos[i].uID]->uPeopleCount = arrInfos[i].uOnLineCount;
		}
	}
	return true;
}

//��������
CGameListCtrl::~CGameListCtrl()
{
	auto itorKind = m_mapGameKind.begin();
	for (; itorKind != m_mapGameKind.end(); ++itorKind)
	{
		delete itorKind->second;
	}

	auto itorName = m_mapGameName.begin();
	for (; itorName	!= m_mapGameName.end(); ++itorName)
	{
		delete itorName->second;
	}

	auto itorRoom = m_mapGameRoom.begin();
	for (; itorRoom != m_mapGameRoom.end(); ++itorRoom)
	{
		delete itorRoom->second;
	}

	m_mapGameKind.clear();
	m_mapGameName.clear();
	m_mapGameRoom.clear();
	return;
}

// �������
bool CGameListCtrl::AddGameKind(ComKindInfo* arrKinds, UINT nCnt)
{
	for (int i = 0; i < nCnt; ++i) 
	{
		if (m_mapGameKind.find(arrKinds[i].uKindID) == m_mapGameKind.end())
		{
			KindInfo* pInfo = new KindInfo;
			memcpy(pInfo, (char *)&arrKinds[i] + sizeof(AssistantHead), sizeof(*pInfo));
			m_mapGameKind.insert(make_pair(arrKinds[i].uKindID, pInfo));
		}
	}

	return true;
}

// �������
bool CGameListCtrl::AddGameName(ComNameInfo* arrNames, UINT nCnt)
{
	for (int i = 0; i < nCnt; ++i)
	{
		if (m_mapGameName.find(arrNames[i].uNameID) == m_mapGameName.end())
		{
			NameInfo* pInfo = new NameInfo;
			memcpy(pInfo, (char *)&arrNames[i] + sizeof(AssistantHead), sizeof(*pInfo));
			m_mapGameName.insert(make_pair(arrNames[i].uNameID, pInfo));
		}
	}
	return true;
}

//��������
bool CGameListCtrl::AddGameRoom(ComRoomInfo* arrRooms, UINT nCnt, UINT nKindID, UINT nNameID)
{
	for (int i = 0; i < nCnt; ++i)
	{
		if (m_mapGameRoom.find(arrRooms[i].uRoomID) == m_mapGameRoom.end())
		{
			RoomInfo* pInfo = new RoomInfo;
			memcpy(pInfo, (char *)&arrRooms[i] + sizeof(AssistantHead), sizeof(*pInfo));
			m_mapGameRoom.insert(make_pair(arrRooms[i].uRoomID, pInfo));
		}
	}
	return true;
}

//������������
KindInfo* CGameListCtrl::FindKindInfo(UINT uKindID)
{
	if (m_mapGameKind.find(uKindID) != m_mapGameKind.end())
	{
		return m_mapGameKind[uKindID];
	}
	return NULL;
}

//������������
NameInfo* CGameListCtrl::FindNameInfo(UINT uNameID)
{
	if (m_mapGameName.find(uNameID) != m_mapGameName.end())
	{
		return m_mapGameName[uNameID];
	}
	return NULL;
}

//���ҷ�������
RoomInfo* CGameListCtrl::FindRoomInfo(UINT uRoomID)
{
	if (m_mapGameRoom.find(uRoomID) != m_mapGameRoom.end())
	{
		return m_mapGameRoom[uRoomID];
	}
	return NULL;
}

//��ȡ��������
bool CGameListCtrl::GetProcessName(UINT nRoomID, TCHAR* szProcessName, UINT uBufferSize)
{
	if (m_mapGameRoom.find(nRoomID) != m_mapGameRoom.end())
	{
		RoomInfo* pInfo = m_mapGameRoom[nRoomID];
		if (m_mapGameName.find(pInfo->uNameID) != m_mapGameName.end())
		{
			NameInfo* pNameInfo = m_mapGameName[pInfo->uNameID];
			memcpy(szProcessName, pNameInfo->szGameProcess, uBufferSize);
			return true;
		}
	}
	return false;
}

//��ȡ��Ϸ����
bool CGameListCtrl::GetGameName(UINT nRoomID, TCHAR* szGameName, UINT uBufferSize)
{
	if (m_mapGameRoom.find(nRoomID) != m_mapGameRoom.end())
	{
		RoomInfo* pInfo = m_mapGameRoom[nRoomID];
		if (m_mapGameName.find(pInfo->uNameID) != m_mapGameName.end())
		{
			NameInfo* pNameInfo = m_mapGameName[pInfo->uNameID];
			memcpy(szGameName, pNameInfo->szGameName, uBufferSize);
			return true;
		}
	}
	return false;
}
