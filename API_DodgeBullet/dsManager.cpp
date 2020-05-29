#include "dsManager.h"


dsManager::dsManager()
{
}


dsManager::~dsManager()
{
	std::map<std::string, dsResource *>::iterator  it;
	for (it = m_mapResource.begin(); it != m_mapResource.end(); it++)
	{
		dsResource *pRes = it->second;
		delete pRes;
	}
	m_mapResource.clear();
}

void dsManager::Destroy()
{
	MAP_RESOURCE::iterator it, it_end = m_mapResource.end();
	for (it = m_mapResource.begin(); it != it_end; ++it)
	{
		delete it->second;
	}
	m_mapResource.clear();
}


void dsManager::Add(std::string name, dsResource *pRes)
{
	m_mapResource[name] = pRes;
}


dsResource *dsManager::Find(std::string name)
{
	MAP_RESOURCE::iterator it = m_mapResource.find(name);
	if (it != m_mapResource.end())
	{
		return it->second;
	}

	return NULL;
}

void dsManager::Release(dsResource *pRes)
{
	MAP_RESOURCE::iterator it = m_mapResource.find(pRes->m_name);
	if (it != m_mapResource.end())
	{
		if (pRes->ReleaseRefCount() <= 0)
		{
			delete it->second;
			m_mapResource.erase(it);
		}
	}
}

