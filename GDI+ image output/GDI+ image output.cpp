// GDI+ image output.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "GDI+ image output.h"

#include <objidl.h>
#include <gdiplus.h>
#include <windowsx.h>
#include <vector>

#pragma comment (lib, "Gdiplus.lib")

#define MAX_LOADSTRING 100

using namespace std;

ULONG_PTR gdiplusToken;
HWND hWnd;


bool drawRectangle = false;
bool SelectUnit = false;
bool TrackMove = false;
bool ControlUnit = false;

//window size
int X = GetSystemMetrics(SM_CXSCREEN);
int Y = GetSystemMetrics(SM_CYSCREEN);

//struct unit {
//    int PosUnit;
//    int HitboxUnit;
//    int NumberUnit;
//};
// 
//unit pose 0
int PosUnit[] = { 100, 100 };
int HitboxUnit[] = { 100 + 50, 100 + 50 };
int NumberUnit;

int newWidth = 50;
int newHeight = 50;

//Rect select units coordinates
int RectSXStart = 0;
int RectSYStart = 0;
int RectSXEnd = 0;
int RectSYEnd = 0;

POINT pt;

int OPU[] = { PosUnit[0]+ pt.x ,PosUnit[1]+ pt.y };

//char control move
void CharMove() {
    if (GetAsyncKeyState('D')) {
        PosUnit[0] += 25;
    }
    if (GetAsyncKeyState('A')) {
        PosUnit[0] -= 25;
    }
    if (GetAsyncKeyState('S')) {
        PosUnit[1] += 25;
    }
    if (GetAsyncKeyState('W')) {
        PosUnit[1] -= 25;
    }
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) //get returns a 16-bit number that determines whether the key is pressed, but to read it, you need a high-bit mask of 0x8000
    {
        drawRectangle = true;
        SelectUnit = false;
    }

    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) //unselect unit on ESC
    {
        SelectUnit = false;
        //drawRectangle = false;
        RectSXStart = 0;
        RectSYStart = 0;
        RectSXEnd = 0;
        RectSYEnd = 0;
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
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
            {
                running = false;
            }
        }
        else
        {
            CharMove();
            InvalidateRect(hWnd, nullptr, FALSE);
           // Sleep(10);
        }
    }
}

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

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
    case WM_LBUTTONDOWN:
        drawRectangle = true;
        RectSXStart = GET_X_LPARAM(lParam);
        RectSYStart = GET_Y_LPARAM(lParam);
        RectSXEnd = RectSXStart;
        RectSYEnd = RectSYStart;
        SetCapture(hWnd);
        break;

    case WM_MOUSEMOVE:
        if (drawRectangle)
        {
            RectSXEnd = GET_X_LPARAM(lParam);
            RectSYEnd = GET_Y_LPARAM(lParam);
            InvalidateRect(hWnd, nullptr, FALSE);
        }
        break;

    case WM_LBUTTONUP:
        drawRectangle = false;
        ReleaseCapture();
        InvalidateRect(hWnd, nullptr, FALSE);
        break;

    case WM_RBUTTONDOWN:
        if(SelectUnit)
        {
            GetCursorPos(&pt);
            ScreenToClient(hWnd, &pt);

            PosUnit[0] = pt.x - (newWidth / 2);
            PosUnit[1] = pt.y - (newHeight / 2);
        }
        

    case WM_RBUTTONUP:
        TrackMove = true;
           

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        Gdiplus::Bitmap backBuffer(X, Y);
        Gdiplus::Graphics* backGraphics = Gdiplus::Graphics::FromImage(&backBuffer);
        

        Gdiplus::Image* BG = Gdiplus::Image::FromFile(L"Flame.bmp");
        Gdiplus::Image* unit = Gdiplus::Image::FromFile(L"stickman.bmp");
                
        //draw BG
        if (BG && BG->GetLastStatus() == Gdiplus::Status::Ok)
        {
            for (int i = 0; i < X; i += 512)
            {
                for (int O = 0; O < Y; O += 512)
                {
                    backGraphics->DrawImage(BG, i, O);
                }
            }
        }
        delete BG;

        if (TrackMove)
        {
            /*vector<int, int>*Pos = new vector<int, int>();
            Pos[0].push_back(PosUnit[0] + (newWidth / 2));
            Pos[1].push_back(PosUnit[1] + (newHeight / 2));
            Gdiplus::Graphics LineMove(hdc);
            Gdiplus::Pen LineCol(Gdiplus::Color(255, 100, 255, 0), 2);
            backGraphics->DrawLine(&LineCol, 1, 1, pt.x, pt.y);
            delete Pos;*/

            vector<int>pos;
            pos.push_back(PosUnit[0] + (newWidth / 2));
            pos.push_back(PosUnit[1] + (newHeight / 2));
            Gdiplus::Graphics LineMove(hdc);
            Gdiplus::Pen LineCol(Gdiplus::Color(255, 100, 255, 0), 2);
            backGraphics->DrawLine(&LineCol, pos[0], pos[1], pt.x, pt.y);
        }

        //draw unit
        if (unit && unit->GetLastStatus() == Gdiplus::Status::Ok)
        {
            int originalWidth = unit->GetWidth();
            int originalHeight = unit->GetHeight();            

            backGraphics->DrawImage(unit, PosUnit[0], PosUnit[1], newWidth, newHeight);

            /*if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) && SelectUnit)
            {
                GetCursorPos(&pt);
                ScreenToClient(hWnd, &pt);
                
                PosUnit[0] = pt.x - (newWidth / 2);
                PosUnit[1] = pt.y - (newHeight / 2);
                
            }*/
            
        }
        delete unit;

        //draw rectangle selecting units
        if (drawRectangle)
        {
            Gdiplus::Pen myRedPen(Gdiplus::Color(255, 100, 255, 0), 2);

            int RectSX = min(RectSXStart, RectSXEnd);
            int RectSY = min(RectSYStart, RectSYEnd);
            int RectSHeigh = abs(RectSXEnd-RectSXStart);
            int RectSWigth = abs(RectSYEnd-RectSYStart);

            backGraphics->DrawRectangle(&myRedPen, RectSX, RectSY, RectSHeigh, RectSWigth);
        }

        //selection rect
        if (min(RectSXStart,RectSXEnd) < PosUnit[0] & max(RectSXStart, RectSXEnd) > PosUnit[0]+newWidth)
        {
            SelectUnit = true;
        }

        

        if (SelectUnit)
        {
            
            Gdiplus::Graphics graph(hdc);
            Gdiplus::Pen RectCol(Gdiplus::Color(255, 100, 255, 0), 2);
            Gdiplus::Rect RectPos(PosUnit[0], PosUnit[1], newWidth, newHeight);
            backGraphics->DrawRectangle(&RectCol, RectPos);
        }

        //second buffer
        Gdiplus::Graphics screenGraphics(hdc);
        screenGraphics.DrawImage(&backBuffer, 0, 0, 0, 0, X, Y, Gdiplus::UnitPixel);
        delete backGraphics;

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