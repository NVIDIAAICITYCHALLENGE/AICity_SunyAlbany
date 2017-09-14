#include "defs.h"
#include <config/param.h>

//*****************************************************************************************//


//-----------------------------------------------------------------------------------------------------------------//
//*************************************************** Allot ID ***************************************************//
//----------------------------------------------------------------------------------------------------------------//
CAllotID::CAllotID()
{
	//------------------------------- ID Variables -----------------------------------//
	m_nIDCount = 0;
	m_pOccupyID = new IDSet;
	m_pOccupyID->clear();
	m_pFreeID = new IDSet;
	m_pFreeID->clear();
}

CAllotID::~CAllotID()
{
	m_nIDCount = 0;
	if (m_pOccupyID) {
		RELEASE(m_pOccupyID);
		m_pOccupyID = 0;
	}
	if (m_pFreeID) {
		RELEASE(m_pFreeID);
		m_pFreeID = 0;
	}
}

//--------------------------------------- ID Dispatcher part -------------------------------------------------//
bool CAllotID::AppendIDs()
{
	int nFree = (int)(m_pFreeID->size());
	if (nFree < G_AppNumber) {
		for (int i = 0; i < G_AppNumber; i++) {
			m_pFreeID->push_back(m_nIDCount + i);
		}
		m_nIDCount += G_AppNumber;
	}
	return true;
}

bool CAllotID::ReclaimIDs(int id)
{
	bool fMark = false;
	IDSet::iterator id_iter;
	for (id_iter = m_pOccupyID->begin(); id_iter != m_pOccupyID->end(); id_iter++) {
		if (*id_iter == id) {
			id_iter = m_pOccupyID->erase(id_iter);
			fMark = true;
			break;
		}
	}
	if (!fMark) {
		fprintf(stderr, "Errors in ID Assignment!\n");
		return false;
	}
	else {
		m_pFreeID->push_back(id);
		return true;
	}
}

int CAllotID::DispatchID()
{
	//check whether the free ID is enough or not.
	IDSet::iterator id_iter;
	int nFree = (int)(m_pFreeID->size());
	int aID = -1;
	if (nFree > 0) {
		id_iter = m_pFreeID->begin();
		aID = *id_iter;
		id_iter = m_pFreeID->erase(id_iter);	
	}
	else {
		AppendIDs();
		id_iter = m_pFreeID->begin();
		aID = *id_iter;
		id_iter = m_pFreeID->erase(id_iter);
	}
	//add it to the occupied ID.
	m_pOccupyID->push_back(aID);
	return aID;
}

//Reset the ID allocator.
bool CAllotID::Reset()
{
	m_nIDCount = 0;
	//Release the free ID.
	m_pFreeID->clear();
	//Release the occupy ID.
	m_pOccupyID->clear();
	return true;
}