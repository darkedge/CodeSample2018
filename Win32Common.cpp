#include "stdafx.h"
#include "Win32Common.h"

static void Win32PrintError(const char* file, int line, const char* expr, DWORD err);

// Retrieve the system error message for the last-error code
void Win32PrintLastError(const char* file, int line, const char* expr)
{
    DWORD err = GetLastError();
    Win32PrintError(file, line, expr, err);
}

static void Win32PrintError(const char* file, int line, const char* expr, DWORD err)
{
    wchar_t* lpMsgBuf;

    // Potential recursion issue if FormatMessageW keeps failing
    WIN32_FAIL_IF_ZERO(FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        err,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPWSTR>(&lpMsgBuf),
        0,
        nullptr
    ));

    // Display the error message
    std::wcout << file << L":" << line << L" - " << expr << L" failed with error ";
    std::wcout << L"0x" << std::hex << std::setfill(L'0') << std::setw(8) << std::setprecision(8) << err;
    std::wcout << L": " << lpMsgBuf << std::endl;

    LocalFree(reinterpret_cast<HLOCAL>(lpMsgBuf));
}

RECT CenteredWindowRectangle(HWND hwnd, LONG width, LONG height, bool frame)
{
    RECT desktopRect = {};
    if (hwnd)
    {
        HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONULL);
        MONITORINFO info = {};
        info.cbSize = sizeof(MONITORINFO);
        WIN32_FAIL_IF_ZERO(GetMonitorInfoW(monitor, &info));
        // rcWork takes task bar into account
        desktopRect = info.rcMonitor;
    }
    else
    {
        WIN32_FAIL_IF_ZERO(GetClientRect(GetDesktopWindow(), &desktopRect));
    }

    LONG x = (desktopRect.left + desktopRect.right) / 2 - width / 2;
    LONG y = (desktopRect.bottom + desktopRect.top) / 2 - height / 2;

    RECT windowRect = {};
    windowRect.left = x;
    windowRect.top = y;
    windowRect.right = x + width;
    windowRect.bottom = y + height;

    if (frame)
    {
        WIN32_FAIL_IF_ZERO(AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE));
    }

    return windowRect;
}

WindowsConsole::WindowsConsole()
{
    WIN32_FAIL_IF_ZERO(AllocConsole());
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
}

WindowsConsole::~WindowsConsole()
{
    WIN32_FAIL_IF_ZERO(FreeConsole());
}
