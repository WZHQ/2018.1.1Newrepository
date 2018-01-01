#include<windows.h>
#include<stdio.h>
#include"相关函数.h"
//主题：俄罗斯方块

// 一个控制结束程序的线程
DWORD _stdcall Proc(LPVOID lParam)
{
	WaitForSingleObject(lParam, INFINITE);
	SendMessage(hWnd, WM_DESTROY, 0, 0);
	return 0;
}
// 一个控制游戏难度的线程
DWORD WINAPI ContrlDiff(LPVOID)
{
	BOOL dengji[5] = { TRUE };
	
	while (1)
	{
		if (!reDiffluence)
		{
			dengji[0] = TRUE;
			if (Score >= 400 && dengji[1])
			{
				KillTimer(hWnd, 1);
				Diffluence = 4;
				SetTimer(hWnd, 1, 100 * Diffluence, NULL);
				dengji[1] = FALSE;
			}
			if (Score >= 1000 && dengji[2])
			{
				KillTimer(hWnd, 1);
				Diffluence = 3;
				SetTimer(hWnd, 1, 100 * Diffluence, NULL);
				dengji[2] = FALSE;
			}
			if (Score >= 2000 && dengji[2])
			{
				KillTimer(hWnd, 1);
				Diffluence = 2;
				SetTimer(hWnd, 1, 100 * Diffluence, NULL);
				dengji[2] = FALSE;
			}			
			if (Score >= 4000 && dengji[2])
			{
				KillTimer(hWnd, 1);
				Diffluence = 1;
				SetTimer(hWnd, 1, 100 * Diffluence, NULL);
				dengji[2] = FALSE;
			}
		}
		if (reDiffluence)
		{
			if (dengji[0])
			{
				for (int i = 1; i < 5; ++i)
					dengji[i] = TRUE;
				KillTimer(hWnd, 1);
				Diffluence = 5;
				SetTimer(hWnd, 1, 100 * Diffluence, NULL);
			}
			dengji[0] = FALSE;
			reDiffluence = 0;
		}
	}
	return 0;

}


//声明窗口处理函数：
LRESULT CALLBACK WindowsProc(
	HWND hWnd,//窗口句柄
	UINT uMsg,//消息编号
	WPARAM wParam,//附加消息
	LPARAM lParam//附加消息
	);


//主函数
int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPWSTR lpCmdLine,int nCmdShow
	)
{   


	//窗口类型名：
	TCHAR szWndclassName[] = L"俄罗斯方块";
	//声明窗口类：
	WNDCLASS wndclass;
	wndclass.cbClsExtra = 0;                       //窗口类额外扩展内存空间大小
	wndclass.cbWndExtra = 0;                       //窗口类额外扩展空间大小
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);//定义白色的背景
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);//定义鼠标在窗口中的样式
	wndclass.hIcon = NULL;                         //定义窗口图标
	wndclass.hInstance = hInstance;                //定义窗口实力句柄
	wndclass.lpfnWndProc = WindowsProc;            //指定窗口过程函数
	wndclass.lpszClassName = szWndclassName;       //指定窗口类型名
	wndclass.lpszMenuName = NULL;                  //菜单名（目前没有菜单）
	wndclass.style = CS_HREDRAW | CS_VREDRAW;      //窗口风格
	/**************声明结束*******************/

	//向系统注册该窗口类：
	RegisterClass(&wndclass);

	//创建这个刚注册的窗口
	hWnd = CreateWindow(
		szWndclassName,           //将要创建窗口的类名
		lpCmdLine,            //窗口标题
		 WS_SYSMENU|WS_MINIMIZEBOX,
		200, 100, 397, 520,
		NULL,
		NULL,
		hInstance,
		NULL
		);


		//显示窗口
		ShowWindow(hWnd,              //要显示的窗口的句柄
			       nCmdShow);         //显示的方式
		UpdateWindow(hWnd);           //更新窗口

									  //定义一个消息类型
		MSG msg;



		
		// ---------------------------初始化游戏数据及方块堆Map
		initmapAndDate();
		initFSfangkuai();
		//--------------------------- 第一次更新窗口
		GameWillBegin(mdc);
		//=----------创建关闭程序的线程和事件
		HANDLE hEvent;
		hEvent = OpenEvent(EVENT_ALL_ACCESS, false, lpCmdLine);
		if (hEvent)
		{
			HANDLE hThread;
			DWORD ThreadID;
			hThread = CreateThread(NULL, NULL, Proc, hEvent, 0, &ThreadID);
		}
		
		
		
		//消息循环
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
		}
		return msg.wParam;
}
// 窗口过程
LRESULT CALLBACK WindowsProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (uMsg)
	{
	case WM_TIMER:
		//判断还能否下降，不能下降了就执行后面的操作
		if (isAbleDown())
		{
			g_Y++;

		}
		else
			if (Gameover())
			{
				
				KillTimer(hWnd, 1);
				Gamestate = 0;
				// 记录最高分在文件zuigaofen里
				WirteHighScore();
				
			}
			else
		{//更新Map
			UpdateMap();
			Deletehang();
			//重新随机生成一个方块
			g_whatX = g_whatX_Yuce;
			g_whatZ = g_whatZ_Yuce;
			g_whatX_Yuce = rand() % 7;
			g_whatZ_Yuce = rand() % 4;
			g_Y = -3;
			g_X = 7;
			
		}
		//双缓冲绘制边框和方块矩阵
		if(Gameover())
			GameFinish(mdc);
		else
		DrawGame(mdc);
		break;

	case WM_KEYDOWN:
		if (Gamestate == 1)
		{
			T_old = GetTickCount();
			if (T_old - T_now >= 50)// 控制按键的使用频率
			{
				switch (wParam)
				{

				case VK_LEFT:
					if (isAbleLeft())
						g_X--;
					DrawGame(mdc);
					break;
				case VK_RIGHT:
					if (isAbleRight())
						g_X++;
					DrawGame(mdc);
					break;
				case VK_DOWN:
					if (isAbleDown())
						g_Y++;
					DrawGame(mdc);
					break;
				case VK_UP:
					if (isAbleRotate())
						g_whatZ = (g_whatZ + 1) % 4;
					DrawGame(mdc);
					break;

				}
				T_now = T_old;
			}
		}
		break;

	case WM_PAINT://绘图消息
	{
		//开始绘图
		hdc=BeginPaint(hWnd,&ps);
		
		switch (Gamestate)
		{
		case 0: GameWillBegin(hdc); break;
		case 1:	DrawGame(hdc); break;
		case 2: GamePause(hdc); break;
		}

		//结束绘图
		EndPaint(hWnd, &ps);
	
	}
		break;
	case WM_LBUTTONDOWN:
		int x, y;
		x = LOWORD(lParam);
		y = HIWORD(lParam);
			if (x > 304 && x < 380)
			{
				static BOOL ispause = FALSE;
				static BOOL isbegin = FALSE;
			
				if (y > 310 && y < 350)// 开始
				{
					if (isbegin == FALSE)
					{
						// -----------创建控制难度的线程

						DWORD ThreadID2;
						hThread2 = CreateThread(NULL, NULL, ContrlDiff, 0, 0, &ThreadID2);
						Gamestate = 1;
						//SetTimer(hWnd, 1, 100*Diffluence, NULL);
						DrawGame(mdc);
						isbegin = TRUE;
					}
					else
					{
						if (Gameover())
						{
							reDiffluence = 1;
							//SetTimer(hWnd, 1, 100 * Diffluence, NULL);
							Gamestate = 1;
							// 输出最高分
							ReadHighScore();
						}

						// 初始化游戏区方块堆Map
						reDiffluence = 1;
						initMapAndScore();
						initFSfangkuai();
						g_Y = -3;
						g_X = 7;
						DrawGame(mdc);
						if (ispause == TRUE)
						{
							SetTimer(hWnd, 1, 100*Diffluence, NULL);
							ispause = FALSE;
							Gamestate = 1;
						}
					
					}

				}
				if (y > 370 && y < 410)// 暂停
				{

					if (!(Gamestate == 0))
					{
						if (ispause == TRUE)
						{
							Gamestate = 1;
							SetTimer(hWnd, 1, 100*Diffluence, NULL);
							DrawGame(mdc);
							ispause = FALSE;
						}
						else
						{
							Gamestate = 2;
							KillTimer(hWnd, 1);
							GamePause(mdc);
							ispause = TRUE;
						}

					}
				}

				if (y > 430 && y < 470)// 退出
				{
					PostQuitMessage(0);
					clearDate();
				}

			}
		break;
	case WM_DESTROY://窗口销毁消息
	{
		PostQuitMessage(0);
		clearDate();
		break; 
	}
	default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	

}

	
