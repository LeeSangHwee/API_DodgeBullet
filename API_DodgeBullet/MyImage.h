#pragma once

#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;

class CMyImage
{
protected:
	Image *m_pImage;

public:
	CMyImage(void);
	~CMyImage(void);


	void Load(char *lpstrFile);
	void Draw(Graphics *g, int x, int y);
	void DrawCenter(Graphics *g, int x, int y, int xCenter, int yCenter);
	void Draw(Graphics *g, int x, int y, int width, int height);
	void Draw();
};
