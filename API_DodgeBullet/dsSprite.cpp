#include "dsSprite.h"


#define IMAGE_PATH  ".\\Data\\Image\\"

dsSprite::dsSprite(std::string name) : dsResource(name)
{
	m_pImage = NULL;
}


dsSprite::~dsSprite()
{
	Release();
}


void dsSprite::Load(std::string file)
{
	if (m_pImage)
		delete m_pImage;

	std::string path = IMAGE_PATH;
	path = path + file;

	WCHAR pathfile[MAX_PATH];
	MultiByteToWideChar(CP_ACP, NULL, path.data(), -1, pathfile, MAX_PATH);


	m_pImage = Image::FromFile(pathfile);
}

void dsSprite::Release()
{
	if (m_pImage)
		delete m_pImage;

	m_pImage = NULL;
}

void dsSprite::Draw(Graphics *g)
{
//	if (m_pImage->GetType() != ImageType::ImageTypeUnknown)
//	  MessageBox(NULL, "Sprite Draw", "Draw", MB_OK);

	g->DrawImage(m_pImage, 200, 200);
}


void dsSprite::Draw(Graphics *g, int x, int y)
{
	g->DrawImage(m_pImage, x, y);
}

void dsSprite::Draw(Graphics *g, int x, int y, int width, int height)
{
	g->DrawImage(m_pImage, x, y, width, height);
}