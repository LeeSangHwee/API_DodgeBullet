#pragma once

#include "dsObject.h"
#include <string>



class dsResource : public dsObject
{
public:
	dsResource(std::string name);
	virtual ~dsResource();

	virtual void Load(std::string file);
	virtual void Release();

	virtual void Draw() {}
};

#define SAFE_DELETE(p)  { if(p) delete (p); (p) = NULL; }
