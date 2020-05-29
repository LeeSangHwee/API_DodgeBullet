//
//  dsObject.h


#ifndef _DSOBJECT_H_
#define _DSOBJECT_H_

#include <string>

class dsObject
{
public:
	std::string m_name;
	
protected:
	short m_refCount;
	
public:
	dsObject();	
	dsObject(std::string name);
	virtual ~dsObject();
	
	short AddRefCount();
	short ReleaseRefCount();
};

#endif
