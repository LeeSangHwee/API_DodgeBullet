#include "dsSpriteManager.h"

static dsSpriteManager g_dsSpriteManager;

dsSpriteManager *GetSpriteManager()
{
	return &g_dsSpriteManager;
}


dsSpriteManager::dsSpriteManager()
{
}


dsSpriteManager::~dsSpriteManager()
{
}


dsSprite *dsSpriteManager::Create(std::string name, std::string file)
{
	dsSprite *pRes = (dsSprite *)Find(name);
	if (pRes != NULL)
		return pRes;

	pRes = new dsSprite(name);
	Add(name, pRes);

	pRes->Load(file);

	return pRes;
}