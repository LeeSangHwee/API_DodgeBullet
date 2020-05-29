#include "MyImage.h"

CMyImage::CMyImage(void)
{
	m_pImage = NULL;
}

CMyImage::~CMyImage(void)
{
	if(m_pImage) 
		delete m_pImage; 
	
	m_pImage = NULL;
}

void CMyImage::Load(char *lpstrFile)
{
	if(m_pImage)
		delete m_pImage;

	WCHAR file[MAX_PATH];
	MultiByteToWideChar(CP_ACP, NULL, lpstrFile,  -1, file, MAX_PATH);



	m_pImage = Image::FromFile(file);
}

void CMyImage::Draw(Graphics *g, int x, int y)
{
	g->DrawImage(m_pImage, x, y);
}

void CMyImage::DrawCenter(Graphics *g, int x, int y, int xCenter, int yCenter)
{
	x = x - ( xCenter/72.0f * 96.0f);	
	y = y - ( yCenter/72.0f * 96.0f);	
	g->DrawImage(m_pImage, x, y, 0, 0, m_pImage->GetWidth(), m_pImage->GetHeight(), Gdiplus::UnitPixel);
}

void CMyImage::Draw(Graphics *g, int x, int y, int width, int height)
{
	g->DrawImage(m_pImage, x, y, width, height);
}