#pragma once

#define __FILENAME__ (strrchr("\\" __FILE__, '\\') + 1)

#ifdef _DEBUG
#define DEBUG_BREAK() DebugBreak()
#else
#define DEBUG_BREAK() exit(1)
#endif

#define WIN32_FAIL_IF(_expr, x) \
if ((_expr) == (x)) { \
    Win32PrintLastError(__FILENAME__, __LINE__, #_expr); \
    DEBUG_BREAK(); \
}

#define WIN32_FAIL_IF_NOT(_expr, x) \
if ((_expr) != (x)) { \
    Win32PrintLastError(__FILENAME__, __LINE__, #_expr); \
    DEBUG_BREAK(); \
}

#define WIN32_FAIL_IF_ZERO(_expr) WIN32_FAIL_IF(_expr, 0)

#define SAFE_RELEASE(_ptr) \
if ((_ptr) != nullptr) { \
    (_ptr)->Release(); \
    (_ptr) = nullptr; \
}

void Win32PrintLastError(const char* file, int line, const char* expr);
RECT CenteredWindowRectangle(HWND hwnd, LONG width, LONG height, bool frame);

// RAII class for a console window
class WindowsConsole
{
public:
    WindowsConsole();
    ~WindowsConsole();
};
