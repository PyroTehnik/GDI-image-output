// GDI+ image output.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "GDI+ image output.h"

#include <objidl.h>
#include <gdiplus.h>

#pragma comment (lib, "Gdiplus.lib")

#define MAX_LOADSTRING 100

ULONG_PTR gdiplusToken;
HWND hWnd;

int X = GetSystemMetrics(SM_CXSCREEN);
int Y = GetSystemMetrics(SM_CYSCREEN);

int x = 0;
int y = 0;

void input() {
    if (GetAsyncKeyState('D')) {
        x += 100;
    }
    if (GetAsyncKeyState('A')) {
        x -= 100;
    }
}



HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Gdiplus::GdiplusStartupInput gdiplusStartupInput; // GDI+ open
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GDIIMAGEOUTPUT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GDIIMAGEOUTPUT));


        MSG msg;
        bool running = true;
    
        while (running)
        {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    running = false;
                }
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
            {
                input();
                InvalidateRect(hWnd, nullptr, FALSE);
                Sleep(10);
            }
        }
}   


//    MSG msg;
//
//    while (GetMessage(&msg, nullptr, 0, 0))
//    {
//        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
//        {
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//        }
//        else
//        {
//            input();
//           // InvalidateRect(HWND, nullptr, FALSE);
//           // Sleep(10);
//        }
//        
//    }
//
//    return (int)msg.wParam;
//}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GDIIMAGEOUTPUT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GDIIMAGEOUTPUT);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
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
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        Gdiplus::Graphics graphics(hdc);
        Gdiplus::Image* BG = Gdiplus::Image::FromFile(L"Flame.bmp");
        Gdiplus::Image* Crter = Gdiplus::Image::FromFile(L"stickman.bmp");

        int WCrter = Crter->GetWidth();
        int HCrter = Crter->GetHeight();

        //DRAW BG
        if (BG->GetLastStatus() == Gdiplus::Status::Ok)
        {
            for (int i = 0; i < X; i += 512)
            {
                for (int O = 0; O < Y; O += 512)
                {
                    graphics.DrawImage(BG, i, O);


                }
            }

            delete BG;
        }

        if (Crter->GetLastStatus() == Gdiplus::Status::Ok)
        {
            graphics.DrawImage(Crter, x, y);
        }
        delete Crter;
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        Gdiplus::GdiplusShutdown(gdiplusToken); // GDI+ close
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

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