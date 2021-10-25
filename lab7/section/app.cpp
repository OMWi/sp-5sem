#ifndef UNICODE
#define UNICODE
#endif 

#define ID_LISTBOX 100

#include <Windows.h>

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

    HWND hwnd = CreateWindow(CLASS_NAME, L"Title", WS_CAPTION | WS_SYSMENU,
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



HWND listbox;

DWORD WINAPI ThreadFunc(LPVOID text)
{
    CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);
    while (true)
    {
        Sleep(250);
        EnterCriticalSection(&cs);
        SendMessage(listbox, LB_ADDSTRING, 0, (LPARAM)text);
        LeaveCriticalSection(&cs);
        
    }
}

HANDLE thread1, thread2;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CREATE:
        RECT rect;
        GetClientRect(hwnd, &rect);
        listbox = CreateWindow(L"LISTBOX", LPCWSTR(NULL), WS_BORDER | WS_VISIBLE | WS_CHILD | WS_VSCROLL,
            5, 5, rect.right - 10, rect.bottom - 10, hwnd, HMENU(ID_LISTBOX), (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
        );
        thread1 = CreateThread(NULL, 0, ThreadFunc, (LPVOID)L"first", 0, NULL);
        thread2 = CreateThread(NULL, 0, ThreadFunc, (LPVOID)L"second", 0, NULL);

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
        }
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}