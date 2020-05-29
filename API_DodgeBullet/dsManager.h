#pragma once

#include "dsResource.h"
#include <map>

class dsManager
{
protected:
	typedef std::map<std::string, dsResource *> MAP_RESOURCE;
	MAP_RESOURCE		m_mapResource;


protected:
	void Destroy();

	void Add(std::string name, dsResource *pRes);

public:
	dsManager();
	virtual ~dsManager();

	virtual dsResource *Create(std::string resname) { return NULL; }
	virtual dsResource *Create(std::string resname, std::string filename) { return NULL; }
	dsResource *Find(std::string name);
	void Release(dsResource *pRes);
};


