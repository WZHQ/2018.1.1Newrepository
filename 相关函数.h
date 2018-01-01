#pragma once
#include<windows.h>
#include<stdio.h>
#include<time.h>
#include<math.h>


//----------����ȫ�ֱ���----------//

// ������Ϸ��ȫ���豸����������
HWND hWnd;
HDC g_hdc,mdc;
HBITMAP hBitmap;
HBRUSH brush44, brush_Blue, brush_Rad, brush_Gray, brush_Gray2, brush_Black, brush_QBlue;
HPEN pen,pen_Yellow,pen_Black,pen_Gray;
HFONT font_2, font_1;// �������ֵ�������ֵĴ�С
DWORD T_old=0, T_now=0;

// "��Ϸ��"�ķ�������飬�з���ֵΪ1���հ�����Ϊ0���������߿򣬱߿�λ��=1��
int Map[27][15] = {0};
// ����һ����ʱ��ı��BOOL��
BOOL bChange = FALSE;
// ���巽�����״�ṹ��
struct g_fangkuai {
	int Rect[4][4];
};
// �������еķ���
g_fangkuai allfangkuai[7][4] = {
	// һ����
	0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,
	0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,
	0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,
	0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,

	// ������
	1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
	1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
	1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
	1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,

	// ������
	0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,
	0,1,0,0,0,1,1,0,0,1,0,0,0,0,0,0,
	0,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0,
	0,1,0,0,1,1,0,0,0,1,0,0,0,0,0,0,

	// ��Z����
	0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,
	0,0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,
	0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,
	0,0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,

	// ��Z����
	0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,
	1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,
	0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,
	1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,

	// ��L����
	1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,
	1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,
	1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,
	0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0,

	// ��L����
	0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,
	1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,
	1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,
	1,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0
};
// ���Ʒ���ĵ�ǰ��״����
int g_whatX=2;
// ���Ʒ���ĵ�ǰ����
int g_whatZ=2;
//��ʾԤ�ⴰ�ڷ���
int g_whatX_Yuce, g_whatZ_Yuce;
// ָ���������ĳ�ʼ��λ��,�Ҿ���ȡ1����20�����أ����Ժ�������������ʼֵʱע��˲���20
// ����Ϸ����С��֪�����ó�ʼֵʱ��g_x������[1,13],g_y=0
int g_X=7, g_Y=-3;
// ����÷�
int Score=0;
// ������߷�
int HighScore=0;
// һ��������ǰ��Ϸ״̬����0��δ��ʼ  1��������   2����ͣ
int Gamestate;
// ������Ϸ�Ѷ�// �Ƿ������Ѷ�
int Diffluence = 5; int reDiffluence = 1;
// �����ɴ��Ѷȵ��߳�
HANDLE hThread2;


//----------���巽��----------//

// ������߷�
void WirteHighScore() {
	if (Score > HighScore)
	{
		FILE *fp;
		fopen_s(&fp, "./zuigaofen.dat", "w");
		fprintf_s(fp, "%d", Score);
		fclose(fp);
	}
}
// д����߷�
void ReadHighScore() {
	FILE *fp;
	fopen_s(&fp, "./zuigaofen.dat", "r");
	if (fp != NULL)
	{
		fscanf_s(fp, "%d", &HighScore);
		fclose(fp);
	}
}
// ��ʼ����Ϸ���ݼ�Map����
void initmapAndDate() {
	// ----------------------------------------- ��ʼ��Map����
	for (int i = 0; i < 27; ++i)
		Map[i][0] = Map[i][14] = 1;
	for (int j = 0; j < 15; ++j)
		Map[26][j] = 1;
	for (int i = 0; i < 26; ++i)
		for (int j = 1; j < 14; ++j)
			Map[i][j] = 0;
	//-------------------------------- -brush_Blue, brush_Rad, brush_Gray, brush_Gray2, brush_Black, brush_QBlue
	brush_Blue = CreateSolidBrush(RGB(0, 0, 255));
	brush_Rad = CreateSolidBrush(RGB(255, 0, 0));
	brush_Gray = CreateSolidBrush(RGB(20, 20, 20));
	brush_Gray2 = CreateSolidBrush(RGB(100, 100, 100));
	brush_Black = CreateSolidBrush(RGB(0, 0, 0));
	brush_QBlue = CreateSolidBrush(RGB(0, 255, 255));
	//-------------------------------------------pen_Yellow,pen_Black,pen_Gray;
	pen_Yellow = CreatePen(0, 2, RGB(255, 255, 0));
	pen_Black = (HPEN)GetStockObject(BLACK_PEN);
	pen_Gray = CreatePen(0, 1, RGB(50, 50, 50));
	//--------------------------------------------- font_2, font_1;
	font_1 = CreateFont(-15, -7, 0, 0, 400, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, L"����");
	font_2 = CreateFont(-20, -10, 0, 0, 400, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, L"����");
	//------------------------------------------ ������Ϸ�ĳ�ʼ״̬��־
	Gamestate = 0;
	//--------------------------------------------����һ���������
	srand((unsigned)time(0));
	//----------------------------------------- �Ѵ��ھ����ȫ���豸������g_hdc
	g_hdc = GetDC(hWnd);
	mdc = CreateCompatibleDC(g_hdc);
	hBitmap = CreateCompatibleBitmap(g_hdc, 397, 520);
	SelectObject(mdc, hBitmap);
	// ����߷��������Ļ
	ReadHighScore();
	
}

// ��ʼ���ʼ����������
void initFSfangkuai() {
	srand((unsigned)time(0));
	g_whatX = rand() % 7;
	g_whatZ = rand() % 4;
	g_whatX_Yuce = (rand()+rand()) % 7;
	g_whatZ_Yuce = (rand()+rand()) % 4;
}

// ��Ϸ����   �����Щ����
void clearDate() {
	DeleteObject(brush_Black);
	DeleteObject(brush_Rad);
	DeleteObject(brush_Gray);
	DeleteObject(brush_Gray2);
	DeleteObject(brush_Blue);
	DeleteObject(brush_QBlue);
	DeleteObject(pen_Black);
	DeleteObject(pen_Yellow);
	DeleteObject(pen_Gray);
	KillTimer(hWnd, 1);
	DeleteDC(mdc);
	ReleaseDC(hWnd, g_hdc);
	CloseHandle(hThread2);
}

// ������Ϸ��ʼ��Map,Score
void initMapAndScore() {
	// ----------------------------------------- ��ʼ��Map����
	for (int i = 0; i < 27; ++i)
		Map[i][0] = Map[i][14] = 1;
	for (int j = 0; j < 15; ++j)
		Map[26][j] = 1;
	for (int i = 0; i < 26; ++i)
		for (int j = 1; j < 14; ++j)
			Map[i][j] = 0;
	//------------------------------------------ ��ʼ��Score
	Score = 0;
}





//---------------------------- ��ͼ-------------------------
// ���Ƶ�ǰ�����4*4�ķ���
void Drawfangkuai(HDC hdc) {



	//�������һ���������(��ִֻ��һ��)
	static bool DoOnce = TRUE;
	if (DoOnce)
	{
		g_whatX = rand() % 7;
		g_whatZ = rand() % 4;
		DoOnce = FALSE;
	}
	// ���Ʒ������
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (allfangkuai[g_whatX][g_whatZ].Rect[i][j] == 1)
			{
				SelectObject(hdc, brush_Blue);
				Rectangle(hdc, g_X * 20 + 20 * j, g_Y * 20 + 20 * i, g_X * 20 + 20 * j + 20, g_Y * 20 + 20 * i + 20);
			}
			/*else
			{
				brush44 = CreateSolidBrush(RGB(0, 50, 50));
				SelectObject(hdc, brush44);
				Rectangle(hdc, g_X*20 + 20 * j, g_Y*20 + 20 * i, g_X*20 + 20 * j + 20, g_Y*20 + 20 * i + 20);
			}*/
		}
	}

}
// ����Ԥ�ⷽ���4*4�ķ���
void Drawfangkuai_yuce(HDC hdc)
{
	
	// ���Ʒ������
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (allfangkuai[g_whatX_Yuce][g_whatZ_Yuce].Rect[i][j] == 1)
			{
				SelectObject(hdc, brush_Blue);
				Rectangle(hdc, 300 + 20 * j, 0 + 20 * i, 300 + 20 * j + 20, 0 + 20 * i + 20);
			}
		/*	else
			{
				brush44 = CreateSolidBrush(RGB(0, 50, 50));
				SelectObject(hdc, brush44);
				Rectangle(hdc, 300 + 20 * j, 0 + 20 * i, 300 + 20 * j + 20, 0 + 20 * i + 20);
			}*/
		}
	}


}
// ���������߿�// ����Ϸ��Ϊ1�ķ���
void PaintKuang(HDC hdc)
{
	SelectObject(hdc, brush_Rad);
	SelectObject(mdc, pen_Black);
	//���һ�У�
	for (int i = 1; 20 * i <= 480; ++i)
	{
		Rectangle(hdc, 0, 20 * (i - 1), 20, 20 * i);
	}
	//�ױ�һ�У�
	for (int i = 1; 20 * i <= 260; ++i)
	{
		Rectangle(hdc, 20 * i, 460, 20 * i + 20, 480);
	}
	//�ұ�һ�У�
	for (int i = 1; 20 * i <= 480; ++i)
	{
		Rectangle(hdc, 280, 20 * (i - 1), 300, 20 * i);
	}

	// �����ұ߻��ְ�&�˵���
	SelectObject(hdc, brush_Gray2);
	Rectangle(hdc, 300, 100, 400, 500);
	
	// ���ơ���Ϸ����������
	SelectObject(hdc, brush_Black);
	SelectObject(hdc, pen_Gray);
	for (int i = 1; 20 * i <= 460; ++i)
	{
		for (int j = 1; 20 * j <= 260; ++j)
			Rectangle(hdc, 20 * j, 20 * (i - 1), 20 * j + 20, 20 * i);
	}
	// ���ơ�Ԥ������������
	for (int i = 1; i <= 5; ++i)
	{
		for (int j = 1; j <= 4;++j)
			Rectangle(hdc, 20 * j+280, 20 * (i - 1), 20 * j + 300, 20 * i);
	}
	// ���ơ���Ϸ������Map
	SelectObject(hdc, brush_QBlue);
	for (int i = 0; i < 26; ++i)
	{
		for (int j = 1; j < 14; ++j)
		{
			if (Map[i][j] == 1)
				Rectangle(hdc, j * 20, (i-3) * 20, j * 20 + 20, (i-3) * 20 + 20);
		}
	}
}
// ���Ƶ÷ֿ�
void Getfenshu(HDC hdc)
{
	SelectObject(hdc, pen_Yellow);
	SelectObject(hdc, brush_Gray);
	SelectObject(hdc, font_2);
	//---- ���Ƶ÷�
	Rectangle(hdc, 304, 200, 380, 290);
	SetBkColor(hdc, RGB(20, 20, 20));
	SetTextColor(hdc, RGB(255, 0, 255));
	TextOut(hdc, 320, 220, L"�÷�", 4);
	 static wchar_t str[20];
	 swprintf_s(str, TEXT("  %d"), Score);
	 TextOut(hdc, 315, 250, str, wcslen(str));//315,250

	 //------------------ ������߷�
	 Rectangle(hdc, 304, 110, 380, 190);
	 TextOut(hdc, 312, 128, TEXT("��߷�"), 3);
	 static wchar_t str2[20];
	 swprintf_s(str2, L"  %d", HighScore);
	 TextOut(hdc, 312, 153, str2, wcslen(str2));
}
// ���Ʋ˵���
void MenuKuang(HDC hdc) {
	SelectObject(hdc, pen_Yellow);
	SelectObject(hdc, brush_Gray);
	SelectObject(hdc, font_2);
	Rectangle(hdc, 304, 310, 380, 350);
	Rectangle(hdc, 304, 370, 380, 410);
	Rectangle(hdc, 304, 430, 380, 470);
	SetBkColor(hdc, RGB(20, 20, 20));
	SetTextColor(hdc, RGB(255, 0, 255));
	TextOut(hdc, 320, 320, TEXT("����"),4);
	TextOut(hdc, 320, 380, L"��ͣ", 4);
	TextOut(hdc, 320, 440, L"�˳�", 4);

}
// ��Ϸ��ʼʱ�ܵĻ�ͼ����
void DrawGame(HDC mdc) {
	//˫����
	// ���Ʊ߿�
	PaintKuang(mdc);
	// ����Ԥ��������
	Drawfangkuai_yuce(mdc);
	// ������Ϸ��
	Drawfangkuai(mdc);
	// ���Ƶ÷ֿ�
	Getfenshu(mdc);
	// ���Ʋ˵���
	MenuKuang(mdc);

	BitBlt(g_hdc, 0, 0, 398, 520, mdc, 0, 0, SRCCOPY);

}
// ��Ϸ��ͣʱ�ܵĻ�ͼ����
void GamePause(HDC mdc) {
	// ���Ʊ߿�
	PaintKuang(mdc);
	// ����Ԥ��������
	Drawfangkuai_yuce(mdc);
	// ������Ϸ��
	Drawfangkuai(mdc);
	// ���Ƶ÷ֿ�
	Getfenshu(mdc);
	// ���Ʋ˵���
	MenuKuang(mdc);
	TextOut(mdc, 320, 380, L"����", 4);
	Rectangle(mdc, 60, 160, 240, 220);
	wchar_t str[] = L"��Ҫ����ô������";
	TextOut(mdc, 70, 180, str, wcslen(str));
	BitBlt(g_hdc, 0, 0, 398, 520, mdc, 0, 0, SRCCOPY);
}
// ��Ϸδ��ʼʱ�ܵĻ�ͼ����
void GameWillBegin(HDC mdc) {
	// ���Ʊ߿�
	PaintKuang(mdc);
	// ���Ƶ÷ֿ�
	Getfenshu(mdc);
	// ���Ʋ˵���
	MenuKuang(mdc);
	TextOut(mdc, 320, 320, L"��ʼ", 4);
	Rectangle(mdc, 60, 160, 240, 220);
	wchar_t str[] = L"��˵��ʼ�ǾͿ�ʼ";
	TextOut(mdc, 70, 180, str, wcslen(str));
	BitBlt(g_hdc, 0, 0, 398, 520, mdc, 0, 0, SRCCOPY);
}
// ��Ϸʧ��ʱ���ܵĴ��ڻ�ͼ����
void GameFinish(HDC mdc) {
	// ���Ʊ߿�
	PaintKuang(mdc);
	// ���Ƶ÷ֿ�
	Getfenshu(mdc);
	// ���Ʋ˵���
	MenuKuang(mdc);
	TextOut(mdc, 320, 320, L"����", 4);
	Rectangle(mdc, 60, 160, 240, 220);
	wchar_t str[] = L"����������������";
	TextOut(mdc, 70, 180, str, wcslen(str));
	BitBlt(g_hdc, 0, 0, 398, 520, mdc, 0, 0, SRCCOPY);
}



//------------------------- �ж���ȥ����Ϸʧ��-------------------
// �����鲻���½��󣬽�����λ�õ�Mapֵ��1��������Map��
void UpdateMap()
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			if (allfangkuai[g_whatX][g_whatZ].Rect[i][j])
			{
				// �����ǽ������λ��ת��Ϊ��Map�ϵ�λ��
				int tempA = g_Y + i;
				int tempB = g_X + j;
				// �ж����·�Ԫ���Ƿ�Ϊ1��Ϊ1�����½�
				Map[tempA+3][tempB] = 1;
			}
		}
}
// ����һ������ʱ����ȥһ��
int Deletehang() {
	
	int hezhi=0;// ���ÿһ��MapԪ�صĺ�ֵ��ȫΨһ���ֵΪ13��ȫΪ0 ��˵����⵽�����µ��ϵ�û�з���ĵ�һ�У���֮��Ķ����ü����
	int beilu=0;// �鿴һ������ȥ�˼���
	for (int i = 25; i >0; --i)
	{
		for (int j = 1; j < 14; ++j)
		{
			hezhi = hezhi + Map[i][j];
		}
		if (hezhi == 0)
		{
			if(beilu)
			Score = 10 * pow(2,beilu) + Score;
			return 1;
		}	// ��������ҵ���Ҫ��ȥ���к�i
		if (hezhi==13)
		{
			for (int k = i; k > 0; --k)
				for (int j = 1; j < 14; ++j)
					Map[k][j] = Map[k - 1][j];
			beilu++; 
			i++;
		}

		hezhi = 0;
	}
	// �ñ���beilu����÷�
	Score = 20 * beilu*beilu + Score;
	return 1;
}
// �ж���Ϸ����
bool Gameover() {

	for (int j = 1; j < 13; ++j)
		if (Map[3][j])
			return TRUE;
	return FALSE;
}








//------------------------------------ �ƶ��㷨---------------
// �жϷ����ܷ��½�
bool isAbleDown() {
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			if (allfangkuai[g_whatX][g_whatZ].Rect[i][j])
			{
				// �����ǽ������λ��ת��Ϊ��Map�ϵ�λ��
				int tempA = g_Y + i;
				int tempB = g_X + j;
				// �ж����·�Ԫ���Ƿ�Ϊ1��Ϊ1�����½�
				if (Map[tempA + 1+3][tempB])
					return FALSE;
			}
		}
// ���·�Ԫ��û��Ϊ1�����½�
	return TRUE;
}
// �жϷ����ܷ�����
bool isAbleLeft() {
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			if (allfangkuai[g_whatX][g_whatZ].Rect[i][j])
			{
				// �����ǽ������λ��ת��Ϊ��Map�ϵ�λ��
				int tempA = g_Y + i;
				int tempB = g_X + j;
				// �ж�����Ԫ���Ƿ�Ϊ1��Ϊ1��������
				if (Map[tempA+3][tempB - 1])
					return FALSE;
			}
		}
	// ����Ԫ��û��Ϊ1��������
	return TRUE;
}
// �жϷ����ܷ�����
bool isAbleRight() {
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			if (allfangkuai[g_whatX][g_whatZ].Rect[i][j])
			{
				// �����ǽ������λ��ת��Ϊ��Map�ϵ�λ��
				int tempA = g_Y + i;
				int tempB = g_X + j;
				// �ж����ҷ�Ԫ���Ƿ�Ϊ1��Ϊ1��������
				if (Map[tempA+3][tempB + 1])
					return FALSE;
			}
		}
	// ���ҷ�Ԫ��û��Ϊ1��������
	return TRUE;
}
// �жϷ����ܷ���ת�������������Ϸ�����ѵģ����д��Ż���
bool isAbleRotate() {
	//�������ж�Ϊ��תһ�κ�ķ���
	int tempwhatZ;
	tempwhatZ = (g_whatZ + 1) % 4;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			if (allfangkuai[g_whatX][tempwhatZ].Rect[i][j])
			{
				// �����ǽ������λ��ת��Ϊ��Map�ϵ�λ��
				int tempA = g_Y + i;
				int tempB = g_X + j;
				// �ж���ת��MapԪ���Ƿ�Ϊ1��Ϊ1������ת
				if (Map[tempA+3][tempB])
					return FALSE;
			}
		}
	// ��ת��Ԫ��û��Ϊ1������ת
	return TRUE;
}








/*

����WM_KEYDOWN��Ҫ����WM_CHAR�ģ�

WM_CHAR�����ǵ�WM_KEYDOWN��Ӧ���ˣ�����Ӧ����֪���ǲ��Ƕ��̣߳�������ֻҪ�м��̰��£������ӦWM_KEYDOWN��Ȼ��������ӦWM_CHAR




*/