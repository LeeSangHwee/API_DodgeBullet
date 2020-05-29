#include <time.h>
#include <math.h>

#include "MyImage.h"
#include "dsSpriteManager.h"
#include "dsOpenALSoundManager.h"

Graphics *g_BackBuffer;
Graphics *g_MainBuffer;
Bitmap   *g_Bitmap;

dsSprite *g_pUser;
dsSprite *g_pBullet;
dsSprite *g_pNumScore[10];
dsSprite *g_pNumTime[10];
dsSprite *g_pCharScore;
dsSprite *g_pCharTime;
dsSprite *g_pCharGameOver;
dsSprite *g_pBubble_explo[10];

dsSound *pBoomSound;

bool	  KeyBuffer[256];
int       g_score = 0;
int       g_time = 60;
float	  RandPos_fX;
float	  RandPos_fY;
int		  ExitGame = 0;
int		  Frame = 0;

struct USER
{
	double DiePos_fX, DiePos_fY;
	double Pos_fX, Pos_fY;
	bool bState;

	DWORD ani_time;
	DWORD interval;
	Rect  colRect;
};
USER g_User;

struct BULLET
{
	int iState;

	double Pos_fX, Pos_fY;
	double StartPos_fX, StartPos_fY;
	double MovePos_fX, MovePos_fY;

	Rect  colRect;
};
BULLET g_Bullet[4][25];

void OnUpdate(HWND hWnd, DWORD tick);
void CreateBuffer(HWND hWnd, HDC hDC);
void ReleaseBuffer(HWND hWnd, HDC hDC);

void Initialize();
void OnDrawScore();
void OnUpdateTime(DWORD tick);
void BulletPos();
void BulletMovePos();
void BulletMove();
void Collision();

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS   wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = NULL;
	wndclass.hCursor = NULL;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = "HelloWin";

	if (RegisterClass(&wndclass) == 0)
	{
		return 0;
	}

	HWND hwnd = CreateWindow("HelloWin", "20172448 LeeSangHwee",
		WS_OVERLAPPEDWINDOW,
		0, 0, 1000, 1000,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		return 0;
	}

	HDC hDC = GetDC(hwnd);
	CreateBuffer(hwnd, hDC);

	// �̹���
	dsSpriteManager *pSpriteManager = GetSpriteManager();
	
	g_pUser = pSpriteManager->Create("User", "User.png");
	g_pBullet = pSpriteManager->Create("Bullet", "Bullet.png");
	g_pCharScore = pSpriteManager->Create("Score", "Char_Score.png");
	g_pCharTime = pSpriteManager->Create("Time", "Char_Time.png");
	g_pCharGameOver = pSpriteManager->Create("GameOver", "Char_GameOver.png");

	char cExplo[11];
	for (int i = 0; i <= 11; i++)
	{
		wsprintf(cExplo, "bubble_explo%02d.png", i);
		g_pBubble_explo[i] = pSpriteManager->Create(cExplo, cExplo);
	}

	char cNum[512];
	for (int i = 0; i < 10; i++)
	{
		wsprintf(cNum, "n_%02d.png", i);
		g_pNumScore[i] = pSpriteManager->Create(cNum, cNum);
		wsprintf(cNum, "n_%02d.png", i);
		g_pNumTime[i] = pSpriteManager->Create(cNum, cNum);
	}
	
	// ����
	dsOpenALSoundManager *pSoundManger = GetOpenALSoundManager();
	pBoomSound = pSoundManger->LoadSound("boom.wav", true);
	
	// �� �ʱ�ȭ
	Initialize();

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	DWORD tick = GetTickCount();
	while (1)
	{
		//������ �޼����� ������� �޼����� ó���Ѵ�.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //�޼����� ���� ��� ���� ������ �����Ѵ�.
		{
			DWORD curTick = GetTickCount();

			switch (ExitGame)
			{
			case 0:
			case 1:
				OnUpdate(hwnd, curTick - tick);
				tick = curTick;
				g_MainBuffer->DrawImage(g_Bitmap, 0, 0);
				break;
			case 2:
				g_pCharGameOver->Draw(g_BackBuffer, 400, 450, 200, 40);
				g_MainBuffer->DrawImage(g_Bitmap, 0, 0);

				Sleep(3000);
				exit(1);
			}			
		}
	}
	ReleaseBuffer(hwnd, hDC);
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			g_User.Pos_fX -= 10.0f;
			g_User.colRect.X = g_User.Pos_fX;
		}			
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			g_User.Pos_fX += 10.0f;
			g_User.colRect.X = g_User.Pos_fX;
		}			
		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			g_User.Pos_fY -= 10.0f;
			g_User.colRect.Y = g_User.Pos_fY;
		}			
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			g_User.Pos_fY += 10.0f;
			g_User.colRect.Y = g_User.Pos_fY;
		}			
		break;
	case WM_KEYUP:
		break;
	case WM_CREATE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void CreateBuffer(HWND hWnd, HDC hDC)
{
	GdiplusStartupInput			gdiplusStartupInput;
	ULONG_PTR					gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	RECT rc;
	GetClientRect(hWnd, &rc);

	g_Bitmap = new Bitmap(rc.right - rc.left, rc.bottom - rc.top);
	g_BackBuffer = new Graphics(g_Bitmap);
	g_BackBuffer->SetPageUnit(Gdiplus::Unit::UnitPixel);

	g_MainBuffer = new Graphics(hDC);
	g_MainBuffer->SetPageUnit(Gdiplus::Unit::UnitPixel);
}
void ReleaseBuffer(HWND hWnd, HDC hDC)
{
	ReleaseDC(hWnd, hDC);

	delete g_Bitmap;
	delete g_BackBuffer;
	delete g_MainBuffer;
}
void OnUpdate(HWND hWnd, DWORD tick)
{
	srand((unsigned)time(NULL));

	Color color(0, 0, 0);
	g_BackBuffer->Clear(color);

	if (hWnd == NULL)
		return;

	Color penColor(127, 255, 0);
	Pen pen(penColor);

	// ����⸦ â ũ�� ������ �̵�
	if (0.0f > g_User.Pos_fX)
		g_User.Pos_fX = 0.0f;
	else if (g_User.Pos_fX > 950.0f)
		g_User.Pos_fX = 950.0f; 

	if (0.0f > g_User.Pos_fY)
		g_User.Pos_fY = 0.0f;
	else if (g_User.Pos_fY > 930.0f)
		g_User.Pos_fY = 930.0f;

	if (ExitGame == 0)
	{
		g_pUser->Draw(g_BackBuffer, g_User.Pos_fX, g_User.Pos_fY, 30, 30);
		g_BackBuffer->DrawRectangle(&pen, g_User.colRect);

		BulletMove();
		Collision();
	}

	if (ExitGame == 1)
	{
		// ����� ���� �̹���
		g_pBubble_explo[Frame]->Draw(g_BackBuffer, g_User.DiePos_fX, g_User.DiePos_fY, 30, 40);
		Frame++, Sleep(50);

		if (Frame == 11)
			ExitGame = 2;
	}	

	OnDrawScore();
	OnUpdateTime(tick);
}

void Initialize() // �� �ʱ�ȭ
{
	// ĳ���� �ʱ�ȭ
	g_User.Pos_fX = 450.0f;
	g_User.Pos_fY = 450.0f;
	g_User.bState = 0;
	g_User.ani_time = 1000 + rand() % 600;
	g_User.interval = 0;
	g_User.colRect.X = g_User.Pos_fX;
	g_User.colRect.Y = g_User.Pos_fY;
	g_User.colRect.Width = 30;
	g_User.colRect.Height = 30;

	BulletPos();
	BulletMovePos();
}
void OnDrawScore()
{
	char imgIndex[4];
	int idx, value, score, x;

	g_pCharScore->Draw(g_BackBuffer, 10, 10, 100, 40);

	idx = 0, value = 1000, score = g_score;
	for (; value >= 10; value /= 10)
	{
		imgIndex[idx++] = score / value;
		score = score % value;
	}
	imgIndex[idx] = score;

	x = 130;
	for (int i = 0; i < 4; i++)
	{
		g_pNumScore[imgIndex[i]]->Draw(g_BackBuffer, x, 10, 30, 40);
		x += 30;
	}
}
void OnUpdateTime(DWORD tick)
{
	g_pCharTime->Draw(g_BackBuffer, 10, 50, 100, 40);

	if (g_time <= 0)
	{
		ExitGame = 1;
		return;
	}		

	static DWORD interval = 0;

	interval += tick;
	if (interval >= 1000)
	{
		g_time--;
		interval -= 1000;
	}

	char imgIndex[2];
	imgIndex[0] = g_time / 10;
	imgIndex[1] = g_time % 10;

	int x = 130;
	for (int i = 0; i < 2; i++)
	{
		g_pNumTime[imgIndex[i]]->Draw(g_BackBuffer, x, 50, 30, 40);
		x += 30;
	}
}
void BulletPos() // �Ѿ��� ����ؾ� �� ��ġ ���� �ʱ�ȭ
{
	int time_inum;

	for (int iLine = 0; iLine < 4; ++iLine)
	{
		for (int iBullet = 0; iBullet < 25; ++iBullet)
		{
			time_inum = (rand() % 10000);

			switch (iLine)
			{
			case 0: // ��
				g_Bullet[iLine][iBullet].Pos_fX = (-10.0f);
				g_Bullet[iLine][iBullet].Pos_fY = time_inum / 10.0;
				break;
			case 1: // ��
				g_Bullet[iLine][iBullet].Pos_fX = 1010.0f;
				g_Bullet[iLine][iBullet].Pos_fY = time_inum / 10.0;
				break;
			case 2: // ��
				g_Bullet[iLine][iBullet].Pos_fX = time_inum / 10.0;
				g_Bullet[iLine][iBullet].Pos_fY = (-10.0f);
				break;
			case 3: // ��
				g_Bullet[iLine][iBullet].Pos_fX = time_inum / 10.0;
				g_Bullet[iLine][iBullet].Pos_fY = 1010.0f;
				break;
			}
			g_Bullet[iLine][iBullet].StartPos_fX = g_Bullet[iLine][iBullet].Pos_fX;
			g_Bullet[iLine][iBullet].StartPos_fY = g_Bullet[iLine][iBullet].Pos_fY;
		}
	}
}
void BulletMovePos() // �Ѿ��� �����ؾ� �� ��ġ ���� �ʱ�ȭ
{
	int time_inum;

	for (int iLine = 0; iLine < 4; ++iLine)
	{
		for (int iBullet = 0; iBullet < 25; ++iBullet)
		{
			time_inum = time_inum = (rand() % 10000);

			switch (iLine)
			{
			case 0: // ��
				g_Bullet[iLine][iBullet].MovePos_fX = 1010.0f;
				g_Bullet[iLine][iBullet].MovePos_fY = time_inum / 10.0 + 50.0f;
				break;
			case 1: // ��
				g_Bullet[iLine][iBullet].MovePos_fX = (-10.0f);
				g_Bullet[iLine][iBullet].MovePos_fY = time_inum / 10.0;
				break;
			case 2: // ��
				g_Bullet[iLine][iBullet].MovePos_fX = time_inum / 10.0;
				g_Bullet[iLine][iBullet].MovePos_fY = 1010.0f;				
				break;
			case 3: // ��				
				g_Bullet[iLine][iBullet].MovePos_fX = time_inum / 10.0;
				g_Bullet[iLine][iBullet].MovePos_fY = (-10.0f);
				break;
			}
		}
	}
}
void BulletMove()
{
	Color penColor(255, 0, 0);
	Pen pen(penColor);

	for (int iLine = 0; iLine < 4; ++iLine)
	{
		for (int iBullet = 0; iBullet < 25; ++iBullet)
		{
			g_Bullet[iLine][iBullet].colRect.X = g_Bullet[iLine][iBullet].Pos_fX;
			g_Bullet[iLine][iBullet].colRect.Y = g_Bullet[iLine][iBullet].Pos_fY;
			g_Bullet[iLine][iBullet].colRect.Width = 10;
			g_Bullet[iLine][iBullet].colRect.Height = 10;

			// �Ѿ� �׸���
			g_pBullet->Draw(g_BackBuffer, g_Bullet[iLine][iBullet].Pos_fX, g_Bullet[iLine][iBullet].Pos_fY, 10, 10);
			g_BackBuffer->DrawRectangle(&pen, g_Bullet[iLine][iBullet].colRect);

			switch (iLine)
			{
			case 0: // �� -> ��
				if (g_Bullet[iLine][iBullet].Pos_fX < g_Bullet[iLine][iBullet].MovePos_fX)
				{
					if ((-10.0f) <= g_Bullet[iLine][iBullet].Pos_fY && g_Bullet[iLine][iBullet].Pos_fY <= 1010.f)
					{
						// �Ѿ� �̵� ���� ���6
						double Cal_fY = g_Bullet[iLine][iBullet].MovePos_fY - g_Bullet[iLine][iBullet].StartPos_fY;

						if (0.0f <= Cal_fY) // �Ѿ˹��� : ������ �Ʒ�
						{
							g_Bullet[iLine][iBullet].Pos_fX += 3.0f;
							g_Bullet[iLine][iBullet].Pos_fY += Cal_fY / 340.0f;
						}
						else if (0.0f > Cal_fY) // �Ѿ˹��� : ������ ��
						{
							g_Bullet[iLine][iBullet].Pos_fX += 3.0f;
							g_Bullet[iLine][iBullet].Pos_fY -= Cal_fY / 340.0f;
						}
					}
					else if ((-10.0f) > g_Bullet[iLine][iBullet].Pos_fY || g_Bullet[iLine][iBullet].Pos_fY > 1010.f)
					{
						int time_inum = time_inum = (rand() % 10000);

						g_Bullet[iLine][iBullet].Pos_fX = (-10.0f);
						g_Bullet[iLine][iBullet].Pos_fY = time_inum / 10.0;
						g_Bullet[iLine][iBullet].MovePos_fX = 1010.0f;
						g_Bullet[iLine][iBullet].MovePos_fY = time_inum / 10.0 + 50.0f;

						g_score += 10;
					}
				}
				else if(g_Bullet[iLine][iBullet].Pos_fX >= g_Bullet[iLine][iBullet].MovePos_fX)
				{
					int time_inum = time_inum = (rand() % 10000);

					g_Bullet[iLine][iBullet].Pos_fX = (-10.0f);
					g_Bullet[iLine][iBullet].Pos_fY = time_inum / 10.0;
					g_Bullet[iLine][iBullet].MovePos_fX = 1010.0f;
					g_Bullet[iLine][iBullet].MovePos_fY = time_inum / 10.0 + 50.0f;

					g_score += 10;
				}
				break;
			case 1: // �� -> ��
				if (g_Bullet[iLine][iBullet].Pos_fX > g_Bullet[iLine][iBullet].MovePos_fX)
				{
					if ((-10.0f) <= g_Bullet[iLine][iBullet].Pos_fY && g_Bullet[iLine][iBullet].Pos_fY <= 1010.f)
					{
						// �Ѿ� �̵� ���� ���
						float Cal_fY = g_Bullet[iLine][iBullet].MovePos_fY - g_Bullet[iLine][iBullet].StartPos_fY;
			
						if (0.0f <= Cal_fY) // �Ѿ˹��� : ���� �Ʒ�
						{
							g_Bullet[iLine][iBullet].Pos_fX -= 3.0f;
							g_Bullet[iLine][iBullet].Pos_fY += Cal_fY / 340.0f;
						}
						else if (0.0f > Cal_fY) // �Ѿ˹��� : ���� ��
						{
							g_Bullet[iLine][iBullet].Pos_fX -= 3.0f;
							g_Bullet[iLine][iBullet].Pos_fY += Cal_fY / 340.0f;
						}
					}
					else if ((-10.0f) > g_Bullet[iLine][iBullet].Pos_fY && g_Bullet[iLine][iBullet].Pos_fY > 1010.f)
					{
						int time_inum = time_inum = (rand() % 10000);

						g_Bullet[iLine][iBullet].Pos_fX = 1010.0f;
						g_Bullet[iLine][iBullet].Pos_fY = time_inum / 10.0;
						g_Bullet[iLine][iBullet].MovePos_fX = (-10.0f);
						g_Bullet[iLine][iBullet].MovePos_fY = time_inum / 10.0;

						g_score += 10;
					}
				}
				else if (g_Bullet[iLine][iBullet].Pos_fX >= g_Bullet[iLine][iBullet].MovePos_fX)
				{
					int time_inum = time_inum = (rand() % 10000);

					g_Bullet[iLine][iBullet].Pos_fX = 1010.0f;
					g_Bullet[iLine][iBullet].Pos_fY = time_inum / 10.0;
					g_Bullet[iLine][iBullet].MovePos_fX = (-10.0f);
					g_Bullet[iLine][iBullet].MovePos_fY = time_inum / 10.0;

					g_score += 10;
				}
				break;
			case 2: // �� -> ��
				if (g_Bullet[iLine][iBullet].Pos_fY < g_Bullet[iLine][iBullet].MovePos_fY)
				{
					if ((-10.0f) <= g_Bullet[iLine][iBullet].Pos_fX && g_Bullet[iLine][iBullet].Pos_fX <= 1010.f)
					{
						// �Ѿ� �̵� ���� ���
						float Cal_fX = g_Bullet[iLine][iBullet].MovePos_fX - g_Bullet[iLine][iBullet].StartPos_fX;
			
						if (0.0f <= Cal_fX) // �Ѿ˹��� : ������ �Ʒ�
						{
							g_Bullet[iLine][iBullet].Pos_fX += Cal_fX / 340.0f;
							g_Bullet[iLine][iBullet].Pos_fY += 3.0f;
						}
						else if (0.0f > Cal_fX) // �Ѿ˹��� : ���� �Ʒ�
						{
							g_Bullet[iLine][iBullet].Pos_fX -= Cal_fX / 340.0f;
							g_Bullet[iLine][iBullet].Pos_fY += 3.0f;
						}
					}
					else if ((-10.0f) > g_Bullet[iLine][iBullet].Pos_fX && g_Bullet[iLine][iBullet].Pos_fX > 1010.f)
					{
						int time_inum = time_inum = (rand() % 10000);

						g_Bullet[iLine][iBullet].Pos_fX = time_inum / 10.0;
						g_Bullet[iLine][iBullet].Pos_fY = (-10.0f);
						g_Bullet[iLine][iBullet].MovePos_fX = time_inum / 10.0;
						g_Bullet[iLine][iBullet].MovePos_fY = 1010.0f;

						g_score += 10;
					}
				}
				else if (g_Bullet[iLine][iBullet].Pos_fX >= g_Bullet[iLine][iBullet].MovePos_fX)
				{
					int time_inum = time_inum = (rand() % 10000);

					g_Bullet[iLine][iBullet].Pos_fX = time_inum / 10.0;
					g_Bullet[iLine][iBullet].Pos_fY = (-10.0f);
					g_Bullet[iLine][iBullet].MovePos_fX = time_inum / 10.0;
					g_Bullet[iLine][iBullet].MovePos_fY = 1010.0f;

					g_score += 10;
				}
				break;
			case 3: // �� -> ��
				if (g_Bullet[iLine][iBullet].Pos_fY > g_Bullet[iLine][iBullet].MovePos_fY)
				{
					if ((-10.0f) <= g_Bullet[iLine][iBullet].Pos_fX && g_Bullet[iLine][iBullet].Pos_fX <= 1010.f)
					{
						// �Ѿ� �̵� ���� ���
						float Cal_fX = g_Bullet[iLine][iBullet].MovePos_fX - g_Bullet[iLine][iBullet].StartPos_fX;
			
						if (0.0f <= Cal_fX) // �Ѿ˹��� : ������ ��
						{
							g_Bullet[iLine][iBullet].Pos_fX += Cal_fX / 340.0f;;
							g_Bullet[iLine][iBullet].Pos_fY -= 3.0f;
						}
						else if (0.0f > Cal_fX) // �Ѿ˹��� : ���� ��
						{
							g_Bullet[iLine][iBullet].Pos_fX -= Cal_fX / 340.0f;;
							g_Bullet[iLine][iBullet].Pos_fY -= 3.0f;
						}
					}
					else if ((-10.0f) > g_Bullet[iLine][iBullet].Pos_fX && g_Bullet[iLine][iBullet].Pos_fX > 1010.f)
					{
						int time_inum = time_inum = (rand() % 10000);

						g_Bullet[iLine][iBullet].Pos_fX = time_inum / 10.0;
						g_Bullet[iLine][iBullet].Pos_fY = 1010.0f;
						g_Bullet[iLine][iBullet].MovePos_fX = time_inum / 10.0;
						g_Bullet[iLine][iBullet].MovePos_fY = (-10.0f);

						g_score += 10;
					}
				}
				else if (g_Bullet[iLine][iBullet].Pos_fX >= g_Bullet[iLine][iBullet].MovePos_fX)
				{
					int time_inum = time_inum = (rand() % 10000);

					g_Bullet[iLine][iBullet].Pos_fX = time_inum / 10.0;
					g_Bullet[iLine][iBullet].Pos_fY = 1010.0f;
					g_Bullet[iLine][iBullet].MovePos_fX = time_inum / 10.0;
					g_Bullet[iLine][iBullet].MovePos_fY = (-10.0f);

					g_score += 10;
				}
				break;
			}
		}
	}
}
void Collision()
{
	float pow_fx, pow_fy;

	for (int iLine = 0; iLine < 4; ++iLine)
	{
		for (int iBullet = 0; iBullet < 25; ++iBullet)
		{
			// �Ѿ˰� �������� �浹 ��
			pow_fx = (g_Bullet[iLine][iBullet].Pos_fX + 5) - (g_User.Pos_fX + 15);
			pow_fy = (g_Bullet[iLine][iBullet].Pos_fY + 5) - (g_User.Pos_fY + 15);

			if (sqrt(pow(pow_fx, 2) + pow(pow_fy, 2)) <= 18) // �浹�� ����
			{				
				// ���ӿ��� ���� ���
				if (pBoomSound)
					pBoomSound->Play();

				//���� ���� ����
				g_User.DiePos_fX = g_User.Pos_fX;
				g_User.DiePos_fY = g_User.Pos_fY;
				ExitGame = 1;
			}
		}
	}
}