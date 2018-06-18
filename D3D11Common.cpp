#include "stdafx.h"
#include "Win32Common.h"
#include "D3D11Common.h"

void PrintHResult(const char* file, int line, const char* expr, HRESULT hr)
{
    const char* errStr = "%s:%d - %s failed with error 0x%08lx: %s\n";
    if (HIWORD(hr) == 0x8007)
    {
        // Standard WinAPI error
        LPVOID lpMsgBuf;

        DWORD numBytes = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            hr,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPWSTR)&lpMsgBuf,
            0,
            nullptr
        );

        WIN32_FAIL_IF_ZERO(numBytes);

        char buf[512] = {};
        snprintf(buf, 512, "%s failed with error 0x%08lx: %ws", expr, hr, (LPCWSTR)lpMsgBuf);
        printf("%s\n", buf);

        LocalFree(lpMsgBuf);
    }
    else
    {
        // DXGI error
        printf(errStr, file, line, expr, hr, FormatDXGIMessage(hr));
    }
}

const char* FormatDXGIMessage(HRESULT hr)
{
    switch (hr)
    {
    case DXGI_ERROR_DEVICE_HUNG:
        return "The application's device failed due to badly formed commands sent by the application. This is an design-time issue that should be investigated and fixed.";
    case DXGI_ERROR_DEVICE_REMOVED:
        return "The video card has been physically removed from the system, or a driver upgrade for the video card has occurred. The application should destroy and recreate the device. For help debugging the problem, call ID3D10Device::GetDeviceRemovedReason.";
    case DXGI_ERROR_DEVICE_RESET:
        return "The device failed due to a badly formed command. This is a run-time issue; The application should destroy and recreate the device.";
    case DXGI_ERROR_DRIVER_INTERNAL_ERROR:
        return "The driver encountered a problem and was put into the device removed state.";
    case DXGI_ERROR_FRAME_STATISTICS_DISJOINT:
        return "An event (for example, a power cycle) interrupted the gathering of presentation statistics.";
    case DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE:
        return "The application attempted to acquire exclusive ownership of an output, but failed because some other application (or device within the application) already acquired ownership.";
    case DXGI_ERROR_INVALID_CALL:
        return "The application provided invalid parameter data; this must be debugged and fixed before the application is released.";
    case DXGI_ERROR_MORE_DATA:
        return "The buffer supplied by the application is not big enough to hold the requested data.";
    case DXGI_ERROR_NONEXCLUSIVE:
        return "A global counter resource is in use, and the Direct3D device can't currently use the counter resource.";
    case DXGI_ERROR_NOT_CURRENTLY_AVAILABLE:
        return "The resource or request is not currently available, but it might become available later.";
    case DXGI_ERROR_NOT_FOUND:
        return "When calling IDXGIObject::GetPrivateData, the GUID passed in is not recognized as one previously passed to IDXGIObject::SetPrivateData or IDXGIObject::SetPrivateDataInterface. When calling IDXGIFactory::EnumAdapters or IDXGIAdapter::EnumOutputs, the enumerated ordinal is out of range.";
    case DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED:
        return "Reserved";
    case DXGI_ERROR_REMOTE_OUTOFMEMORY:
        return "Reserved";
    case DXGI_ERROR_WAS_STILL_DRAWING:
        return "The GPU was busy at the moment when a call was made to perform an operation, and did not execute or schedule the operation.";
    case DXGI_ERROR_UNSUPPORTED:
        return "The requested functionality is not supported by the device or the driver.";
    case DXGI_ERROR_ACCESS_LOST:
        return "The desktop duplication interface is invalid. The desktop duplication interface typically becomes invalid when a different type of image is displayed on the desktop.";
    case DXGI_ERROR_WAIT_TIMEOUT:
        return "The time-out interval elapsed before the next desktop frame was available.";
    case DXGI_ERROR_SESSION_DISCONNECTED:
        return "The Remote Desktop Services session is currently disconnected.";
    case DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE:
        return "The DXGI output (monitor) to which the swap chain content was restricted is now disconnected or changed.";
    case DXGI_ERROR_CANNOT_PROTECT_CONTENT:
        return "DXGI can't provide content protection on the swap chain. This error is typically caused by an older driver, or when you use a swap chain that is incompatible with content protection.";
    case DXGI_ERROR_ACCESS_DENIED:
        return "You tried to use a resource to which you did not have the required access privileges. This error is most typically caused when you write to a shared resource with read-only access.";
    case DXGI_ERROR_NAME_ALREADY_EXISTS:
        return "The supplied name of a resource in a call to IDXGIResource1::CreateSharedHandle is already associated with some other resource.";
    case DXGI_ERROR_SDK_COMPONENT_MISSING:
        return "The operation depends on an SDK component that is missing or mismatched.";
    case S_OK:
        return "The method succeeded without an error.";
    default:
        return "Unknown error message";
    }
}
