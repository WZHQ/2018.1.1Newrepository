#include<windows.h>
#include<stdio.h>
#include"��غ���.h"
//���⣺����˹����

// һ�����ƽ���������߳�
DWORD _stdcall Proc(LPVOID lParam)
{
	WaitForSingleObject(lParam, INFINITE);
	SendMessage(hWnd, WM_DESTROY, 0, 0);
	return 0;
}
// һ��������Ϸ�Ѷȵ��߳�
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


//�������ڴ�������
LRESULT CALLBACK WindowsProc(
	HWND hWnd,//���ھ��
	UINT uMsg,//��Ϣ���
	WPARAM wParam,//������Ϣ
	LPARAM lParam//������Ϣ
	);


//������
int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPWSTR lpCmdLine,int nCmdShow
	)
{   


	//������������
	TCHAR szWndclassName[] = L"����˹����";
	//���������ࣺ
	WNDCLASS wndclass;
	wndclass.cbClsExtra = 0;                       //�����������չ�ڴ�ռ��С
	wndclass.cbWndExtra = 0;                       //�����������չ�ռ��С
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);//�����ɫ�ı���
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);//��������ڴ����е���ʽ
	wndclass.hIcon = NULL;                         //���崰��ͼ��
	wndclass.hInstance = hInstance;                //���崰��ʵ�����
	wndclass.lpfnWndProc = WindowsProc;            //ָ�����ڹ��̺���
	wndclass.lpszClassName = szWndclassName;       //ָ������������
	wndclass.lpszMenuName = NULL;                  //�˵�����Ŀǰû�в˵���
	wndclass.style = CS_HREDRAW | CS_VREDRAW;      //���ڷ��
	/**************��������*******************/

	//��ϵͳע��ô����ࣺ
	RegisterClass(&wndclass);

	//���������ע��Ĵ���
	hWnd = CreateWindow(
		szWndclassName,           //��Ҫ�������ڵ�����
		lpCmdLine,            //���ڱ���
		 WS_SYSMENU|WS_MINIMIZEBOX,
		200, 100, 397, 520,
		NULL,
		NULL,
		hInstance,
		NULL
		);


		//��ʾ����
		ShowWindow(hWnd,              //Ҫ��ʾ�Ĵ��ڵľ��
			       nCmdShow);         //��ʾ�ķ�ʽ
		UpdateWindow(hWnd);           //���´���

									  //����һ����Ϣ����
		MSG msg;



		
		// ---------------------------��ʼ����Ϸ���ݼ������Map
		initmapAndDate();
		initFSfangkuai();
		//--------------------------- ��һ�θ��´���
		GameWillBegin(mdc);
		//=----------�����رճ�����̺߳��¼�
		HANDLE hEvent;
		hEvent = OpenEvent(EVENT_ALL_ACCESS, false, lpCmdLine);
		if (hEvent)
		{
			HANDLE hThread;
			DWORD ThreadID;
			hThread = CreateThread(NULL, NULL, Proc, hEvent, 0, &ThreadID);
		}
		
		
		
		//��Ϣѭ��
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
		}
		return msg.wParam;
}
// ���ڹ���
LRESULT CALLBACK WindowsProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (uMsg)
	{
	case WM_TIMER:
		//�жϻ��ܷ��½��������½��˾�ִ�к���Ĳ���
		if (isAbleDown())
		{
			g_Y++;

		}
		else
			if (Gameover())
			{
				
				KillTimer(hWnd, 1);
				Gamestate = 0;
				// ��¼��߷����ļ�zuigaofen��
				WirteHighScore();
				
			}
			else
		{//����Map
			UpdateMap();
			Deletehang();
			//�����������һ������
			g_whatX = g_whatX_Yuce;
			g_whatZ = g_whatZ_Yuce;
			g_whatX_Yuce = rand() % 7;
			g_whatZ_Yuce = rand() % 4;
			g_Y = -3;
			g_X = 7;
			
		}
		//˫������Ʊ߿�ͷ������
		if(Gameover())
			GameFinish(mdc);
		else
		DrawGame(mdc);
		break;

	case WM_KEYDOWN:
		if (Gamestate == 1)
		{
			T_old = GetTickCount();
			if (T_old - T_now >= 50)// ���ư�����ʹ��Ƶ��
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

	case WM_PAINT://��ͼ��Ϣ
	{
		//��ʼ��ͼ
		hdc=BeginPaint(hWnd,&ps);
		
		switch (Gamestate)
		{
		case 0: GameWillBegin(hdc); break;
		case 1:	DrawGame(hdc); break;
		case 2: GamePause(hdc); break;
		}

		//������ͼ
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
			
				if (y > 310 && y < 350)// ��ʼ
				{
					if (isbegin == FALSE)
					{
						// -----------���������Ѷȵ��߳�

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
							// �����߷�
							ReadHighScore();
						}

						// ��ʼ����Ϸ�������Map
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
				if (y > 370 && y < 410)// ��ͣ
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

				if (y > 430 && y < 470)// �˳�
				{
					PostQuitMessage(0);
					clearDate();
				}

			}
		break;
	case WM_DESTROY://����������Ϣ
	{
		PostQuitMessage(0);
		clearDate();
		break; 
	}
	default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	

}

	
