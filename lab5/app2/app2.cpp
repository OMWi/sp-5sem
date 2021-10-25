#ifndef UNICODE
#define UNICODE
#endif 

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

    HWND hwnd = CreateWindow(CLASS_NAME, L"app2", WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
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
BOOL btnClick = FALSE;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static PCOPYDATASTRUCT dataStruct;
    static POINT point;
    static RECT rect;

    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CREATE:        
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        GetClientRect(hwnd, &rect);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        if (params.draw && btnClick) {
            INT x = (int)point.x;
            INT y = (int)point.y;
            SelectObject(hdc, GetStockObject(DC_BRUSH));
            SetDCBrushColor(hdc, params.color);
            switch (params.shape)
            {
            case RHOMBUS:
            {
                POINT points[] = { {x - 40, y}, {x, y - 25}, {x + 40, y}, {x, y + 25} };
                Polygon(hdc, points, sizeof(points) / sizeof(points[0]));
                break;
            }

            case SQUARE:
                Rectangle(hdc, x - 40, y - 40, x + 40, y + 40);
                break;
            case CIRCLE:
                Ellipse(hdc, x - 40, y - 40, x + 40, y + 40);
                break;
            case STAR:
            {
                POINT points[] = { {x - 50, y - 10 }, {x - 15, y - 10 }, {x, y - 50 }, {x + 15, y - 10 }, {x + 50, y - 10 },
                    {x + 20, y + 10},{x + 35, y + 50 }, {x, y + 25 }, {x - 35, y + 50 }, {x - 20, y + 10 }, {x - 50, y - 10 } };
                Polygon(hdc, points, sizeof(points) / sizeof(points[0]));

                break;
            }
            default:
                break;
            }
        }
        EndPaint(hwnd, &ps);
    }
    return 0;

    case WM_COPYDATA:
    {
        dataStruct = (PCOPYDATASTRUCT)lParam;
        params = *(drawParams*)(dataStruct->lpData);
        if (params.draw)
            InvalidateRect(hwnd, &rect, TRUE);
    }
    return 0;

    case WM_LBUTTONDOWN:
        if (params.draw) {
            point.x = LOWORD(lParam);
            point.y = HIWORD(lParam);
            btnClick = TRUE;
            InvalidateRect(hwnd, &rect, TRUE);
        }
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}