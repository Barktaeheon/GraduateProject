﻿// CodeDungeon.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "framework.h"
#include "CodeDungeon.h"
#include "Game.h"

unique_ptr<Game> g_Game = make_unique<Game>();
#define MAX_LOADSTRING 100
HWND hMainWnd;
HINSTANCE hInst;                                // 현재 인스턴스입니다. 인스턴스 = 프로그램의 정보
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, //응용 프로그램 시작 진입젖ㅁ
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CODEDUNGEON, szWindowClass, MAX_LOADSTRING);

    //윈도우 클래스를 등록함
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다: (주 윈도우 생성)
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }
    g_Game->Init(hInstance, hMainWnd);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CODEDUNGEON));

    MSG msg;



    while (1)
    {
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;
            if (!::TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
        }
        else
        {
            g_Game->Update();

        }
    }
    g_Game->Destroy();
   // _CrtDumpMemoryLeaks();
    return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;  //윈도우 프로시져 설정
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CODEDUNGEON));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    //wcex.IpszMenuName = NULL; //메뉴 없애려면
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    //윈도우 클래스를 등록하기 위한 윈도우 API 함수
    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
//주 윈도우를 생성하고 화면에 보이도록 하는 함수
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;
    RECT rc = { 0, 0, Factor::FRAME_BUFFER_WIDTH, Factor::FRAME_BUFFER_HEIGHT };
    DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_BORDER | WS_SYSMENU;
    AdjustWindowRect(&rc, dwStyle, FALSE);
    hMainWnd = CreateWindow(szWindowClass, szTitle, dwStyle, CW_USEDEFAULT,
        CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
    if (!hMainWnd) return(FALSE);


    ::ShowWindow(hMainWnd, nCmdShow);
    ::UpdateWindow(hMainWnd);

#ifdef _WITH_SWAPCHAIN_FULLSCREEN_STATE
    gGameFramework.ChangeSwapChainState();
#endif
    return(TRUE);
}

//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //long 타입
//윈도우 프로시저가 메세지 처리를 끝냈다고 운영체제에 알려주는 역할
//LRESULT = long
//콜백함수란? 특정 이벤트가 일어나면 자동으로 실행하는 함수

    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_SIZE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MOUSEMOVE:
    case WM_KEYDOWN:
    case WM_KEYUP:
        g_Game->OnProcessingWindowMessage(hWnd, message, wParam, lParam);
        break;
    case WM_COMMAND:
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        // 메뉴의 선택 영역을 구문 분석합니다.
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 그리기 코드를 추가합니다.
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;//운영체제가 이 메세지에 관여하지 않고 프로그래머가 관여한다.
    //-1을 반환하면 운영체제가 진행하는 작업을 취소한다.
}


// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{//wParam 키보드에 대한 값들 
    //https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes


//lParam 키가 눌렸을때 상태 나타내는
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
