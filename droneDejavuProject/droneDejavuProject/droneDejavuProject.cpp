// droneDejavuProject.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "droneDejavuProject.h"
#include <minwinbase.h>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// 확장 비트맵 코드
HINSTANCE g_hInstance;
ULONG_PTR gdiplusToken;
GdiplusStartupInput gdiplusStartupInput;
DRONEPOS** g_dronePos;
Image* g_image;
HBITMAP g_droneBitmap;

// 확장 윈도우 컨트롤 코드
HWND hDroneList;
HWND hClusterList;

// 드론, 클러스터링 컨트롤
char droneCount[][15] = { "1", "2", "3", "4", "5" , "6", "7", "8"};
int droneCountSize = sizeof(droneCount) / sizeof(droneCount[0]);
char clusterCount[][15] = { "1", "2", "3", "4", "5", "6" };
int clusterCountSize = sizeof(clusterCount) / sizeof(clusterCount[0]);
TCHAR str[4];
int inputDroneCount = 3;
int inputClusterCount = 2;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

	// 무스비 코드
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	g_hInstance = hInstance;
	g_dronePos = InitializeDroneCount(inputDroneCount, inputClusterCount);

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DRONEDEJAVUPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRONEDEJAVUPROJECT));

    MSG msg;

    // 기본 메시지 루프입니다.
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRONEDEJAVUPROJECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DRONEDEJAVUPROJECT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
    switch (message)
    {
	case WM_CREATE:
		{
			InitControlBox(hWnd, &hDroneList, &hClusterList, g_hInstance, droneCount, droneCountSize, clusterCount, clusterCountSize);
			InitImage(g_hInstance, MAKEINTRESOURCE(IDB_DRONESMALL), &g_droneBitmap, g_dronePos, inputDroneCount, inputClusterCount);
			SetTimer(hWnd, 1, 500, NULL);
			//SetTimer(hWnd, 2, 50, NULL);
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case IDC_DRONE_CNT_COMBOBOX:
				{
					switch(HIWORD(wParam))
					{
						case CBN_SELCHANGE:
							int selectItem = SendMessage(hDroneList, CB_GETCURSEL, 0, 0);
							SendMessage(hDroneList, CB_GETLBTEXT, selectItem, (LPARAM)str);
							inputDroneCount = _wtoi(str);
							//MessageBox(hWnd, str, str, 0);
							ZeroMemory(str, sizeof(str));
							break;
					}

					KillTimer(hWnd, 1);
					delete(*g_dronePos);
					delete(g_dronePos);

					// 드론 재할당
					g_dronePos = InitializeDroneCount(inputDroneCount, inputClusterCount);
					// 드론 위치 초기화
					InitImage(g_hInstance, MAKEINTRESOURCE(IDB_DRONESMALL), &g_droneBitmap, g_dronePos, inputDroneCount, inputClusterCount);
					SetTimer(hWnd, 1, 500, NULL);

				}
				break;
			case IDC_CLUSTER_CNT_COMBOBOX:
				{
					switch(HIWORD(wParam))
					{
						case CBN_SELCHANGE:
							int selectItem = SendMessage(hClusterList, CB_GETCURSEL, 0, 0);
							SendMessage(hClusterList, CB_GETLBTEXT, selectItem, (LPARAM)str);
							inputClusterCount = _wtoi(str);
							//MessageBox(hWnd, str, str, 0);
							ZeroMemory(str, sizeof(str));
							break;
					}

					KillTimer(hWnd, 1);
					delete(*g_dronePos);
					delete(g_dronePos);

					// 드론 재할당
					g_dronePos = InitializeDroneCount(inputDroneCount, inputClusterCount);
					InitImage(g_hInstance, MAKEINTRESOURCE(IDB_DRONESMALL), &g_droneBitmap, g_dronePos, inputDroneCount, inputClusterCount);
					SetTimer(hWnd, 1, 500, NULL);
				}
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
			//ShowPngImage(g_image, g_hInstance, MAKEINTRESOURCE(IDB_PNG1), hdc);
			//ShowBmpImage(hdc, g_hInstance, MAKEINTRESOURCE(IDB_DRONE));

            EndPaint(hWnd, &ps);
        }
        break;
	case WM_TIMER:
		{
			RECT rcClient;
			HDC hdc = GetDC(hWnd);

			GetClientRect(hWnd, &rcClient);
			DronePosUpdate(&rcClient, g_dronePos, inputDroneCount, inputClusterCount);
			DrawDrone(hdc, &rcClient, g_dronePos, g_droneBitmap, g_hInstance, MAKEINTRESOURCE(IDB_DRONESMALL), 
				hWnd, inputDroneCount, inputClusterCount);

			ReleaseDC(hWnd, hdc);
		}
		break;
    case WM_DESTROY:
		KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
