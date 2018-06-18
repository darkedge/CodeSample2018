#include "stdafx.h"
#include "Win32Common.h"

// Global Variables:
HINSTANCE hInst;                                // current instance

static const wchar_t* CLASS_NAME = L"CodeSample2018Class";
static const wchar_t* WINDOW_NAME = L"CodeSample2018";

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    WindowsConsole console;

    WIN32_FAIL_IF_ZERO(MyRegisterClass(hInstance));

    HWND hWnd = CreateWindowW(CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    WIN32_FAIL_IF_ZERO(hWnd);

    ShowWindow(hWnd, nCmdShow);
    WIN32_FAIL_IF_ZERO(UpdateWindow(hWnd));

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_CLASSDC;
    wcex.lpfnWndProc    = WndProc;
    wcex.hInstance      = hInstance;
    wcex.hCursor        = LoadCursorW(nullptr, IDC_ARROW);
    wcex.lpszClassName  = CLASS_NAME;

    return RegisterClassExW(&wcex);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hWnd, message, wParam, lParam);
    }
    return 0;
}
