//
//  dsObject.mm
//  spEngine
//
//  Created by Ju Woosuk on 11. 2. 11..
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "dsObject.h"

dsObject::dsObject()
{
	m_refCount = 0;
	
	AddRefCount();
}

dsObject::dsObject(std::string name) : m_name(name)
{
	m_refCount = 0;
	
	AddRefCount();
}

dsObject::~dsObject()
{
}

short dsObject::AddRefCount()
{
	m_refCount++;
	
	return m_refCount;
}

short dsObject::ReleaseRefCount()
{
	m_refCount--;
	
	return m_refCount;
}