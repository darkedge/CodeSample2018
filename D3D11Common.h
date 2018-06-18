#pragma once

#ifdef _DEBUG
#define D3D_TRY(_expr) \
do { \
    HRESULT	_hr = _expr; \
    if (FAILED( _hr )) { \
        PrintHResult(__FILENAME__, __LINE__, #_expr, _hr); \
        __debugbreak(); \
    } \
} while (0)
#else
#define D3D_TRY(expr) expr
#endif

void PrintHResult(const char* file, int line, const char* expr, HRESULT hr);
const char* FormatDXGIMessage(HRESULT hr);
