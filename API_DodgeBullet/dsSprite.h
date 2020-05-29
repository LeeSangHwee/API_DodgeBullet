#pragma once

#include "dsResource.h"

#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;


class dsSprite : public dsResource
{
protected:
	Image *m_pImage;

public:
	dsSprite(std::string name);
	virtual ~dsSprite();

	void Load(std::string file);
	void Release();

	void Draw(Graphics *g);
	void Draw(Graphics *g, int x, int y);
	void Draw(Graphics *g, int x, int y, int width, int height);
};

