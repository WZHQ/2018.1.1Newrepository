#pragma once
#include<windows.h>
#include<stdio.h>
#include<time.h>
#include<math.h>


//----------定义全局变量----------//

// 整个游戏的全局设备环境上下文
HWND hWnd;
HDC g_hdc,mdc;
HBITMAP hBitmap;
HBRUSH brush44, brush_Blue, brush_Rad, brush_Gray, brush_Gray2, brush_Black, brush_QBlue;
HPEN pen,pen_Yellow,pen_Black,pen_Gray;
HFONT font_2, font_1;// 后面的数值象征着字的大小
DWORD T_old=0, T_now=0;

// "游戏区"的方块堆数组，有方块值为1，空白区域为0；（包含边框，边框位置=1）
int Map[27][15] = {0};
// 定义一个随时间改变的BOOL量
BOOL bChange = FALSE;
// 定义方块的形状结构体
struct g_fangkuai {
	int Rect[4][4];
};
// 陈列所有的方块
g_fangkuai allfangkuai[7][4] = {
	// 一字型
	0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,
	0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,
	0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,
	0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,

	// 田字型
	1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
	1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
	1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
	1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,

	// 甲字型
	0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,
	0,1,0,0,0,1,1,0,0,1,0,0,0,0,0,0,
	0,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0,
	0,1,0,0,1,1,0,0,0,1,0,0,0,0,0,0,

	// 左Z字型
	0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,
	0,0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,
	0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,
	0,0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,

	// 右Z字型
	0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,
	1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,
	0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,
	1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,

	// 左L字型
	1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,
	1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,
	1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,
	0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0,

	// 右L字型
	0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,
	1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,
	1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,
	1,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0
};
// 控制方块的当前形状种类
int g_whatX=2;
// 控制方块的当前方向
int g_whatZ=2;
//显示预测窗口方块
int g_whatX_Yuce, g_whatZ_Yuce;
// 指定方块矩阵的初始化位置,我决定取1代表20个像素，所以后面用它决定初始值时注意乘不乘20
// 又游戏区大小可知，设置初始值时，g_x在区间[1,13],g_y=0
int g_X=7, g_Y=-3;
// 定义得分
int Score=0;
// 定义最高分
int HighScore=0;
// 一个表明当前游戏状态的量0：未开始  1：正在玩   2：暂停
int Gamestate;
// 设置游戏难度// 是否重置难度
int Diffluence = 5; int reDiffluence = 1;
// 控制由此难度的线程
HANDLE hThread2;


//----------定义方法----------//

// 存入最高分
void WirteHighScore() {
	if (Score > HighScore)
	{
		FILE *fp;
		fopen_s(&fp, "./zuigaofen.dat", "w");
		fprintf_s(fp, "%d", Score);
		fclose(fp);
	}
}
// 写入最高分
void ReadHighScore() {
	FILE *fp;
	fopen_s(&fp, "./zuigaofen.dat", "r");
	if (fp != NULL)
	{
		fscanf_s(fp, "%d", &HighScore);
		fclose(fp);
	}
}
// 初始化游戏数据及Map数组
void initmapAndDate() {
	// ----------------------------------------- 初始化Map数组
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
	font_1 = CreateFont(-15, -7, 0, 0, 400, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, L"宋体");
	font_2 = CreateFont(-20, -10, 0, 0, 400, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, L"宋体");
	//------------------------------------------ 设置游戏的初始状态标志
	Gamestate = 0;
	//--------------------------------------------设置一个随机种子
	srand((unsigned)time(0));
	//----------------------------------------- 把窗口句柄给全局设备上下文g_hdc
	g_hdc = GetDC(hWnd);
	mdc = CreateCompatibleDC(g_hdc);
	hBitmap = CreateCompatibleBitmap(g_hdc, 397, 520);
	SelectObject(mdc, hBitmap);
	// 将最高分输出到屏幕
	ReadHighScore();
	
}

// 初始化最开始的两个方块
void initFSfangkuai() {
	srand((unsigned)time(0));
	g_whatX = rand() % 7;
	g_whatZ = rand() % 4;
	g_whatX_Yuce = (rand()+rand()) % 7;
	g_whatZ_Yuce = (rand()+rand()) % 4;
}

// 游戏结束   清除这些数据
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

// 重玩游戏初始化Map,Score
void initMapAndScore() {
	// ----------------------------------------- 初始化Map数组
	for (int i = 0; i < 27; ++i)
		Map[i][0] = Map[i][14] = 1;
	for (int j = 0; j < 15; ++j)
		Map[26][j] = 1;
	for (int i = 0; i < 26; ++i)
		for (int j = 1; j < 14; ++j)
			Map[i][j] = 0;
	//------------------------------------------ 初始化Score
	Score = 0;
}





//---------------------------- 绘图-------------------------
// 绘制当前方块的4*4的方格
void Drawfangkuai(HDC hdc) {



	//最初给它一个随机方块(但只执行一次)
	static bool DoOnce = TRUE;
	if (DoOnce)
	{
		g_whatX = rand() % 7;
		g_whatZ = rand() % 4;
		DoOnce = FALSE;
	}
	// 绘制方块矩阵
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
// 绘制预测方块的4*4的方格
void Drawfangkuai_yuce(HDC hdc)
{
	
	// 绘制方块矩阵
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
// 画整体界面边框// 和游戏区为1的方格
void PaintKuang(HDC hdc)
{
	SelectObject(hdc, brush_Rad);
	SelectObject(mdc, pen_Black);
	//左边一列：
	for (int i = 1; 20 * i <= 480; ++i)
	{
		Rectangle(hdc, 0, 20 * (i - 1), 20, 20 * i);
	}
	//底边一行：
	for (int i = 1; 20 * i <= 260; ++i)
	{
		Rectangle(hdc, 20 * i, 460, 20 * i + 20, 480);
	}
	//右边一列：
	for (int i = 1; 20 * i <= 480; ++i)
	{
		Rectangle(hdc, 280, 20 * (i - 1), 300, 20 * i);
	}

	// 绘制右边积分版&菜单板
	SelectObject(hdc, brush_Gray2);
	Rectangle(hdc, 300, 100, 400, 500);
	
	// 绘制“游戏区”的网格
	SelectObject(hdc, brush_Black);
	SelectObject(hdc, pen_Gray);
	for (int i = 1; 20 * i <= 460; ++i)
	{
		for (int j = 1; 20 * j <= 260; ++j)
			Rectangle(hdc, 20 * j, 20 * (i - 1), 20 * j + 20, 20 * i);
	}
	// 绘制“预告区”的网格
	for (int i = 1; i <= 5; ++i)
	{
		for (int j = 1; j <= 4;++j)
			Rectangle(hdc, 20 * j+280, 20 * (i - 1), 20 * j + 300, 20 * i);
	}
	// 绘制“游戏区”的Map
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
// 绘制得分框
void Getfenshu(HDC hdc)
{
	SelectObject(hdc, pen_Yellow);
	SelectObject(hdc, brush_Gray);
	SelectObject(hdc, font_2);
	//---- 绘制得分
	Rectangle(hdc, 304, 200, 380, 290);
	SetBkColor(hdc, RGB(20, 20, 20));
	SetTextColor(hdc, RGB(255, 0, 255));
	TextOut(hdc, 320, 220, L"得分", 4);
	 static wchar_t str[20];
	 swprintf_s(str, TEXT("  %d"), Score);
	 TextOut(hdc, 315, 250, str, wcslen(str));//315,250

	 //------------------ 绘制最高分
	 Rectangle(hdc, 304, 110, 380, 190);
	 TextOut(hdc, 312, 128, TEXT("最高分"), 3);
	 static wchar_t str2[20];
	 swprintf_s(str2, L"  %d", HighScore);
	 TextOut(hdc, 312, 153, str2, wcslen(str2));
}
// 绘制菜单框
void MenuKuang(HDC hdc) {
	SelectObject(hdc, pen_Yellow);
	SelectObject(hdc, brush_Gray);
	SelectObject(hdc, font_2);
	Rectangle(hdc, 304, 310, 380, 350);
	Rectangle(hdc, 304, 370, 380, 410);
	Rectangle(hdc, 304, 430, 380, 470);
	SetBkColor(hdc, RGB(20, 20, 20));
	SetTextColor(hdc, RGB(255, 0, 255));
	TextOut(hdc, 320, 320, TEXT("重玩"),4);
	TextOut(hdc, 320, 380, L"暂停", 4);
	TextOut(hdc, 320, 440, L"退出", 4);

}
// 游戏开始时总的绘图函数
void DrawGame(HDC mdc) {
	//双缓冲
	// 绘制边框
	PaintKuang(mdc);
	// 绘制预测区方块
	Drawfangkuai_yuce(mdc);
	// 绘制游戏区
	Drawfangkuai(mdc);
	// 绘制得分框
	Getfenshu(mdc);
	// 绘制菜单框
	MenuKuang(mdc);

	BitBlt(g_hdc, 0, 0, 398, 520, mdc, 0, 0, SRCCOPY);

}
// 游戏暂停时总的绘图函数
void GamePause(HDC mdc) {
	// 绘制边框
	PaintKuang(mdc);
	// 绘制预测区方块
	Drawfangkuai_yuce(mdc);
	// 绘制游戏区
	Drawfangkuai(mdc);
	// 绘制得分框
	Getfenshu(mdc);
	// 绘制菜单框
	MenuKuang(mdc);
	TextOut(mdc, 320, 380, L"继续", 4);
	Rectangle(mdc, 60, 160, 240, 220);
	wchar_t str[] = L"还要继续么，菜鸟";
	TextOut(mdc, 70, 180, str, wcslen(str));
	BitBlt(g_hdc, 0, 0, 398, 520, mdc, 0, 0, SRCCOPY);
}
// 游戏未开始时总的绘图函数
void GameWillBegin(HDC mdc) {
	// 绘制边框
	PaintKuang(mdc);
	// 绘制得分框
	Getfenshu(mdc);
	// 绘制菜单框
	MenuKuang(mdc);
	TextOut(mdc, 320, 320, L"开始", 4);
	Rectangle(mdc, 60, 160, 240, 220);
	wchar_t str[] = L"你说开始那就开始";
	TextOut(mdc, 70, 180, str, wcslen(str));
	BitBlt(g_hdc, 0, 0, 398, 520, mdc, 0, 0, SRCCOPY);
}
// 游戏失败时的总的窗口绘图函数
void GameFinish(HDC mdc) {
	// 绘制边框
	PaintKuang(mdc);
	// 绘制得分框
	Getfenshu(mdc);
	// 绘制菜单框
	MenuKuang(mdc);
	TextOut(mdc, 320, 320, L"重玩", 4);
	Rectangle(mdc, 60, 160, 240, 220);
	wchar_t str[] = L"不服气？重新再玩";
	TextOut(mdc, 70, 180, str, wcslen(str));
	BitBlt(g_hdc, 0, 0, 398, 520, mdc, 0, 0, SRCCOPY);
}



//------------------------- 判定消去，游戏失败-------------------
// 当方块不能下降后，将方块位置的Map值给1，（更新Map）
void UpdateMap()
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			if (allfangkuai[g_whatX][g_whatZ].Rect[i][j])
			{
				// 下面是将方格的位置转化为在Map上的位置
				int tempA = g_Y + i;
				int tempB = g_X + j;
				// 判断正下方元素是否为1，为1则不能下降
				Map[tempA+3][tempB] = 1;
			}
		}
}
// 当有一行满的时候消去一行
int Deletehang() {
	
	int hezhi=0;// 检查每一行Map元素的和值，全唯一则和值为13，全为0 则说明检测到了由下到上的没有方块的第一行，则之后的都不用检查了
	int beilu=0;// 查看一次性消去了几行
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
		}	// 上面代码找到了要消去的行号i
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
	// 用倍率beilu计算得分
	Score = 20 * beilu*beilu + Score;
	return 1;
}
// 判断游戏结束
bool Gameover() {

	for (int j = 1; j < 13; ++j)
		if (Map[3][j])
			return TRUE;
	return FALSE;
}








//------------------------------------ 移动算法---------------
// 判断方块能否下降
bool isAbleDown() {
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			if (allfangkuai[g_whatX][g_whatZ].Rect[i][j])
			{
				// 下面是将方格的位置转化为在Map上的位置
				int tempA = g_Y + i;
				int tempB = g_X + j;
				// 判断正下方元素是否为1，为1则不能下降
				if (Map[tempA + 1+3][tempB])
					return FALSE;
			}
		}
// 正下方元素没有为1，能下降
	return TRUE;
}
// 判断方块能否左移
bool isAbleLeft() {
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			if (allfangkuai[g_whatX][g_whatZ].Rect[i][j])
			{
				// 下面是将方格的位置转化为在Map上的位置
				int tempA = g_Y + i;
				int tempB = g_X + j;
				// 判断正左方元素是否为1，为1则不能左移
				if (Map[tempA+3][tempB - 1])
					return FALSE;
			}
		}
	// 正左方元素没有为1，能左移
	return TRUE;
}
// 判断方块能否右移
bool isAbleRight() {
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			if (allfangkuai[g_whatX][g_whatZ].Rect[i][j])
			{
				// 下面是将方格的位置转化为在Map上的位置
				int tempA = g_Y + i;
				int tempB = g_X + j;
				// 判断正右方元素是否为1，为1则不能右移
				if (Map[tempA+3][tempB + 1])
					return FALSE;
			}
		}
	// 正右方元素没有为1，能右移
	return TRUE;
}
// 判断方块能否旋转（这个函数是游戏中最难的，还有待优化）
bool isAbleRotate() {
	//将方块判定为旋转一次后的方块
	int tempwhatZ;
	tempwhatZ = (g_whatZ + 1) % 4;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			if (allfangkuai[g_whatX][tempwhatZ].Rect[i][j])
			{
				// 下面是将方格的位置转化为在Map上的位置
				int tempA = g_Y + i;
				int tempB = g_X + j;
				// 判断旋转后Map元素是否为1，为1则不能旋转
				if (Map[tempA+3][tempB])
					return FALSE;
			}
		}
	// 旋转后元素没有为1，能旋转
	return TRUE;
}








/*

好像WM_KEYDOWN是要先于WM_CHAR的，

WM_CHAR好像是等WM_KEYDOWN响应完了，再响应，不知道是不是多线程，，，，只要有键盘按下，便会响应WM_KEYDOWN，然后马上响应WM_CHAR




*/