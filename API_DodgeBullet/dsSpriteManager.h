#pragma once

#include "dsManager.h"
#include "dsSprite.h"

class dsSpriteManager : public dsManager
{
public:
	dsSpriteManager();
	virtual ~dsSpriteManager();

	dsSprite *Create(std::string name, std::string file);
};

dsSpriteManager *GetSpriteManager();