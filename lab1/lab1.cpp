#ifndef UNICODE
#define UNICODE
#endif 

#define IDM_START 100
#define IDM_STOP 101
#define IDT_TIMER 200

#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

struct Text {
    int x;
    int y;
    LPCWSTR str;
};

Text text;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    HCURSOR hCurs = LoadCursor(hInstance, IDC_WAIT);

    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_CROSS);
    wc.hIcon = LoadIcon(NULL, IDI_SHIELD);

    RegisterClass(&wc);

    HMENU hmenu = CreateMenu();
    AppendMenuW(hmenu, NULL, IDM_START, L"Start");
    AppendMenuW(hmenu, NULL, IDM_STOP, L"Stop");

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"Lab1", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, hmenu, hInstance, NULL
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void onDestroy(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PostQuitMessage(0);
}

void onPaint(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
    TextOut(hdc, text.x, text.y, text.str, wcslen(text.str));

    EndPaint(hwnd, &ps);
}

void onCreate(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    RECT rect;
    GetClientRect(hwnd, &rect);
    text.x = 0;
    text.y = (rect.bottom - rect.top) / 2;  
    text.str = L"Test string";
}

void onTimer(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    RECT rect;
    GetClientRect(hwnd, &rect);
    text.x += 1;
    if (text.x > rect.right)
    {
        text.x = 0;
    }
    text.y = (rect.bottom - rect.top) / 2;
    InvalidateRect(hwnd, NULL, TRUE);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        onDestroy(hwnd, uMsg, wParam, lParam);
        return 0;

    case WM_PAINT:
        onPaint(hwnd, uMsg, wParam, lParam);
        return 0;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_START:
            SetTimer(hwnd, IDT_TIMER, 1, TIMERPROC(NULL));
            break;

        case IDM_STOP:
            KillTimer(hwnd, IDT_TIMER);
            break;
        }        
    }
    return 0;
        

    case WM_CREATE:
        onCreate(hwnd, uMsg, wParam, lParam);
        return 0;

    case WM_TIMER:
        onTimer(hwnd, uMsg, wParam, lParam);
        return 0;
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}