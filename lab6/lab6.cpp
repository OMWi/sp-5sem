#ifndef UNICODE
#define UNICODE
#endif 
#define _USE_MATH_DEFINES

#define ID_STARTBTN 100
#define ID_STOPBTN 101

#include <Windows.h>
#include <math.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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

    HWND hwnd = CreateWindow(CLASS_NAME, L"Lab 6", WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
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


HWND startBtn, stopBtn;

struct Params
{
    POINT center;
    HWND hwnd;
    INT radius;
    INT circleRadius;
};

DWORD WINAPI ThreadFunc(LPVOID pParam)
{
    Params* params = (Params*)pParam;
    POINT center = params->center;
    INT radius = params->radius;
    INT circleRadius = params->circleRadius;

    POINT circleCenter;
    HWND hwnd = params->hwnd;
    RECT rect;
    rect.bottom = center.y + radius + circleRadius;
    rect.top = center.y - radius - circleRadius;
    rect.left = center.x - radius - circleRadius;
    rect.right = center.x + radius + circleRadius;

    DOUBLE angle = M_PI / 4; DOUBLE dAngle = M_PI / 90;
    circleCenter.x = center.x + radius * cos(angle);
    circleCenter.y = center.y - radius * sin(angle);

    while (true)
    {
        HDC hdc = GetDC(hwnd);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
        SetDCBrushColor(hdc, 0x0000FF00);
        Ellipse(hdc, circleCenter.x - circleRadius, circleCenter.y - circleRadius, circleCenter.x + circleRadius, circleCenter.y + circleRadius);
        ReleaseDC(hwnd, hdc);
        // change coords
        angle -= dAngle;
        circleCenter.x = center.x + radius * cos(angle);
        circleCenter.y = center.y - radius * sin(angle);
        Sleep(10);
    }
}

static HANDLE hThreads[6];
static INT threadNum = 6;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CREATE:
    {
        startBtn = CreateWindow(L"button", L"Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            5, 5, 100, 40, hwnd, (HMENU)ID_STARTBTN, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        stopBtn = CreateWindow(L"button", L"Stop", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            110, 5, 100, 40, hwnd, (HMENU)ID_STOPBTN, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

        RECT rect;
        GetClientRect(hwnd, &rect);
        rect.top = 45;
        int xSectionSize = (rect.right - rect.left) / threadNum;
        int ySectionSize = rect.bottom - rect.top;

        for (int i = 0; i < threadNum; i++)
        {
            Params* params = new Params;
            params->center.x = xSectionSize * i + xSectionSize / 2;
            params->center.y = rect.top + ySectionSize / 2;
            params->hwnd = hwnd;
            params->circleRadius = 10;
            int radius = xSectionSize < ySectionSize ? xSectionSize / 2 : ySectionSize / 2;
            radius -= params->circleRadius;
            radius -= 3;
            params->radius = radius;
            hThreads[i] = CreateThread(NULL, 0, ThreadFunc, (LPVOID*)params, CREATE_SUSPENDED, NULL);
        }
    }
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
        case ID_STARTBTN:
            for (int i = 0; i < threadNum; i++)
            {
                ResumeThread(hThreads[i]);
                Sleep(300);
            }
            break;

        case ID_STOPBTN:
            for (int i = 0; i < threadNum; i++)
            {
                SuspendThread(hThreads[i]);
            }
            break;
        }
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}