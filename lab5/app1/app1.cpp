#ifndef UNICODE
#define UNICODE
#endif 

#define ID_RADIORED 100
#define ID_RADIOBLUE 101
#define ID_RADIOGREEN 102
#define ID_CHECKBOXDRAW 103
#define ID_TIMERUPD 104
#define ID_RHOMBUSBTN 120
#define ID_SQUAREBTN 121
#define ID_CIRCLEBTN 122
#define ID_STARBTN 123

#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND hwnd2;
HWND radioBtnRed, radioBtnBlue, radioBtnGreen;
HWND rhombusBtn, squareBtn, circleBtn, starBtn;
HWND checkboxDraw;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(CLASS_NAME, L"app1", WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
        NULL, NULL, hInstance, NULL
    );
    if (hwnd == NULL)
    {
        return 0;
    }
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}




enum Shapes
{
    RHOMBUS, SQUARE, CIRCLE, STAR
};

typedef struct drawParams
{
    COLORREF color;
    BOOL draw;
    Shapes shape;
}DrawParams;

DrawParams params;

void sSendMessage(DrawParams& params, COPYDATASTRUCT dataStruct, HWND hwnd)
{
    dataStruct.cbData = sizeof(params);
    dataStruct.dwData = 1;
    dataStruct.lpData = &params;
    SendMessage(hwnd2, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)(LPVOID)&dataStruct);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    BOOL prevDraw;
    static COPYDATASTRUCT messageStruct;

    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CREATE:
        radioBtnRed = CreateWindow(L"button", L"Red", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP | WS_TABSTOP,
            0, 0, 100, 50, hwnd, (HMENU)ID_RADIORED, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        radioBtnBlue = CreateWindow(L"button", L"Blue", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            0, 50, 100, 50, hwnd, (HMENU)ID_RADIOBLUE, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        radioBtnGreen = CreateWindow(L"button", L"Green", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            0, 100, 100, 50, hwnd, (HMENU)ID_RADIOGREEN, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        checkboxDraw = CreateWindow(L"button", L"Draw", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_GROUP | WS_TABSTOP,
            0, 150, 100, 50, hwnd, (HMENU)ID_CHECKBOXDRAW, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        rhombusBtn = CreateWindow(L"button", L"Rhombus", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP | WS_TABSTOP,
            105, 0, 100, 50, hwnd, (HMENU)ID_RHOMBUSBTN, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        squareBtn = CreateWindow(L"button", L"Square", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            105, 50, 100, 50, hwnd, (HMENU)ID_SQUAREBTN, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        circleBtn = CreateWindow(L"button", L"Circle", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            105, 100, 100, 50, hwnd, (HMENU)ID_CIRCLEBTN, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        starBtn = CreateWindow(L"button", L"Star", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            105, 150, 100, 50, hwnd, (HMENU)ID_STARBTN, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        SendMessage(radioBtnRed, BM_SETCHECK, BST_CHECKED, NULL);
        params.color = 0x000000FF;
        SendMessage(rhombusBtn, BM_SETCHECK, BST_CHECKED, NULL);
        params.shape = RHOMBUS;
        params.draw = FALSE;
        SetTimer(hwnd, ID_TIMERUPD, 1000, TIMERPROC(NULL));
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
    }
    return 0;

    case WM_COMMAND:
    {
        int wmid = LOWORD(wParam);
        switch (wmid)
        {
        case ID_RADIORED:
            params.color = 0x000000FF;
            sSendMessage(params, messageStruct, hwnd);
            break;
        case ID_RADIOGREEN:
            params.color = 0x0000FF00;
            sSendMessage(params, messageStruct, hwnd);
            break;
        case ID_RADIOBLUE:
            params.color = 0x00FF0000;
            sSendMessage(params, messageStruct, hwnd);
            break;
        case ID_RHOMBUSBTN:
            params.shape = RHOMBUS;
            sSendMessage(params, messageStruct, hwnd);
            break;
        case ID_SQUAREBTN:
            params.shape = SQUARE;
            sSendMessage(params, messageStruct, hwnd);
            break;
        case ID_CIRCLEBTN:
            params.shape = CIRCLE;
            sSendMessage(params, messageStruct, hwnd);
            break;
        case ID_STARBTN:
            params.shape = STAR;
            sSendMessage(params, messageStruct, hwnd);
            break;
        case ID_CHECKBOXDRAW:
            if (SendMessage(checkboxDraw, BM_GETCHECK, 0, 0) == BST_CHECKED) {
                prevDraw = true;
            }
            else {
                prevDraw = false;
            }
            params.draw = prevDraw;
            sSendMessage(params, messageStruct, hwnd);
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    return 0;

    case WM_TIMER:
        if (!hwnd2)
        {
            OutputDebugString(L"second window not found\n");
            hwnd2 = FindWindowW(NULL, L"app2");
            return 0;            
        }
        KillTimer(hwnd, ID_TIMERUPD);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}