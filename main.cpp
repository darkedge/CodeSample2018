#include "stdafx.h"
#include "Win32Common.h"
#include "Game.h"

static const wchar_t* CLASS_NAME = L"CodeSample2018Class";
static const wchar_t* WINDOW_NAME = L"CodeSample2018";

// Forward declarations
ATOM MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Entry point
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    WindowsConsole console;

    WIN32_FAIL_IF_ZERO(MyRegisterClass(hInstance));

    HWND hWnd = CreateWindowExW(
        0,
        CLASS_NAME,
        WINDOW_NAME,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        0,
        CW_USEDEFAULT,
        0,
        nullptr,
        nullptr,
        hInstance,
        nullptr);

    WIN32_FAIL_IF_ZERO(hWnd);

    ShowWindow(hWnd, nCmdShow);
    WIN32_FAIL_IF_ZERO(UpdateWindow(hWnd));

    auto game = std::make_unique<Game>(hWnd);

    MSG msg;
    bool running = true;
    while (true)
    {
        // Pump window messages through WndProc
        while (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                running = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        game->Update();

        if (!running) break;
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
