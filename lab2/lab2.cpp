#ifndef UNICODE
#define UNICODE
#endif 

#define id_button_add 100
#define id_button_delete 101
#define id_button_toRight 102
#define id_button_clear 103
#define id_listbox_left 104
#define id_listbox_right 105
#define id_edit 106

#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND editWnd, addBtnWnd, clearBtnWnd, toRightBtnWnd, deleteBtnWnd, listLeftWnd, listRightWnd;

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

    HWND hwnd = CreateWindow(CLASS_NAME, L"Lab2", WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 700, 350,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    //UpdateWindow(hwnd);

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

    EndPaint(hwnd, &ps);
}

BOOL onCreate(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    editWnd = CreateWindow(L"EDIT", LPCWSTR(NULL) , WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT,
        10, 10, 210, 20, hwnd, HMENU(id_edit), (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
    );
    addBtnWnd = CreateWindow(L"BUTTON", L"Add", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 35, 105, 40, hwnd, HMENU(id_button_add), (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
    );
    clearBtnWnd = CreateWindow(L"BUTTON", L"Clear", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        115, 35, 105, 40, hwnd, HMENU(id_button_clear), (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
    );
    toRightBtnWnd = CreateWindow(L"BUTTON", L"To right", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 80, 105, 40, hwnd, HMENU(id_button_toRight), (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
    );
    deleteBtnWnd = CreateWindow(L"BUTTON", L"Delete", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        115, 80, 105, 40, hwnd, HMENU(id_button_delete), (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
    );
    listLeftWnd = CreateWindow(L"LISTBOX", LPCWSTR(NULL), WS_BORDER | WS_VISIBLE | WS_CHILD,
        240, 10, 210, 300, hwnd, HMENU(id_listbox_left), (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
    );
    listRightWnd = CreateWindow(L"LISTBOX", LPCWSTR(NULL), WS_BORDER | WS_VISIBLE | WS_CHILD,
        460, 10, 210, 300, hwnd, HMENU(id_listbox_right), (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
    );

    if (editWnd == NULL || addBtnWnd == NULL || clearBtnWnd == NULL || toRightBtnWnd == NULL ||
        deleteBtnWnd == NULL || listLeftWnd == NULL || listRightWnd == NULL)
    {
        return FALSE;
    }
    return TRUE;
}

void onAdd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WCHAR text[1024];
    GetWindowText(editWnd, text, 1024);
    if (wcslen(text) > 0)
    {
        if (SendMessage(listLeftWnd, LB_FINDSTRINGEXACT, -1, (LPARAM)text) == LB_ERR)
        {
            SendMessage(listLeftWnd, LB_ADDSTRING, 0, (LPARAM)text);
        }
        
    }
}

void onDelete(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int pos = SendMessage(listLeftWnd, LB_GETCURSEL, 0, 0);
    if (pos != LB_ERR)
    {
        WCHAR buffer[256];
        SendMessage(listLeftWnd, LB_GETTEXT, (WPARAM)pos, (LPARAM)buffer);
        SendMessage(listLeftWnd, LB_DELETESTRING, (WPARAM)pos, 0);
    }
    pos = SendMessage(listRightWnd, LB_GETCURSEL, 0, 0);
    if (pos != LB_ERR)
    {
        WCHAR buffer[256];
        SendMessage(listRightWnd, LB_GETTEXT, (WPARAM)pos, (LPARAM)buffer);
        SendMessage(listRightWnd, LB_DELETESTRING, (WPARAM)pos, 0);
    }
}

void onToRight(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int pos = SendMessage(listLeftWnd, LB_GETCURSEL, 0, 0);
    if (pos == LB_ERR)
    {
        return;
    }
    WCHAR buffer[256];
    SendMessage(listLeftWnd, LB_GETTEXT, (WPARAM)pos, (LPARAM)buffer);
    if (SendMessage(listRightWnd, LB_FINDSTRINGEXACT, -1, (LPARAM)buffer) == LB_ERR)
    {
        SendMessage(listRightWnd, LB_ADDSTRING, 0, (LPARAM)buffer);
    }
}

void onClear(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    SendMessage(listLeftWnd, LB_RESETCONTENT, 0, 0);
    SendMessage(listRightWnd, LB_RESETCONTENT, 0, 0);
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

    case WM_CREATE:
        if (onCreate(hwnd, uMsg, wParam, lParam) == FALSE)
        {
            PostQuitMessage(0);
        }
        return 0;

    case WM_COMMAND:
    {
        int wmid = LOWORD(wParam);
        switch (wmid)
        {
        case id_button_add:
            onAdd(hwnd, uMsg, wParam, lParam);
            break;
        case id_button_clear:
            onClear(hwnd, uMsg, wParam, lParam);
            break;
        case id_button_delete:
            onDelete(hwnd, uMsg, wParam, lParam);
            break;
        case id_button_toRight:
            onToRight(hwnd, uMsg, wParam, lParam);
            break;        
        }
    }
    return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}