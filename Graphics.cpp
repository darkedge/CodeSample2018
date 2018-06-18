#include "stdafx.h"
#include "Win32Common.h"
#include "Graphics.h"

Graphics::Graphics(HWND hWnd)
{
    D3D_FEATURE_LEVEL featureLevel;
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    DXGI_SWAP_CHAIN_DESC sd = {};
    {
        sd.BufferCount = 2;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hWnd;
        sd.SampleDesc.Count = 4;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    }

    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        createDeviceFlags,
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        &sd,
        this->swapChain.GetAddressOf(),
        this->device.GetAddressOf(),
        &featureLevel,
        this->deviceContext.GetAddressOf()
    );

    this->CreateDepthStencilState();

    RECT clientRect = {};
    WIN32_FAIL_IF_ZERO(GetClientRect(hWnd, &clientRect));

    this->viewport.TopLeftX = 0.0f;
    this->viewport.TopLeftY = 0.0f;
    this->viewport.MinDepth = 0.0f;
    this->viewport.MaxDepth = 1.0f;
    this->viewport.Width = (float)(clientRect.right - clientRect.left);
    this->viewport.Height = (float)(clientRect.bottom - clientRect.top);

    this->CreateDepthTexture(clientRect);
    this->CreateRenderTargetView();
    this->CreateRasterizerState();
}

Graphics::~Graphics()
{
}

HRESULT Graphics::CreateDepthStencilState()
{
    D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc = {};
    depthStencilStateDesc.DepthEnable = TRUE;
    depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
    return this->device->CreateDepthStencilState(&depthStencilStateDesc, this->depthStencilState.ReleaseAndGetAddressOf());
}

HRESULT Graphics::CreateDepthTexture(const RECT& rect)
{
    HRESULT hr;

    D3D11_TEXTURE2D_DESC depthStencilBufferDesc = {};
    depthStencilBufferDesc.ArraySize = 1;
    depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilBufferDesc.Width = rect.right - rect.left;
    depthStencilBufferDesc.Height = rect.bottom - rect.top;
    depthStencilBufferDesc.MipLevels = 1;
    depthStencilBufferDesc.SampleDesc.Count = 4; // 4x multi-sample anti-aliasing

    hr = this->device->CreateTexture2D(&depthStencilBufferDesc, nullptr, this->depthTexture.ReleaseAndGetAddressOf());
    if (FAILED(hr))
    {
        return hr;
    }

    hr = this->device->CreateDepthStencilView(this->depthTexture.Get(), nullptr, this->depthStencilView.ReleaseAndGetAddressOf());
    if (FAILED(hr))
    {
        return hr;
    }

    return S_OK;
}

HRESULT Graphics::CreateRenderTargetView()
{
    HRESULT hr;

    if (!this->swapChain)
    {
        return E_FAIL;
    }
    else
    {
        DXGI_SWAP_CHAIN_DESC sd = {};
        hr = this->swapChain->GetDesc(&sd);
        if (FAILED(hr))
        {
            return hr;
        }

        Microsoft::WRL::ComPtr<ID3D11Texture2D> surface;
        D3D11_RENDER_TARGET_VIEW_DESC rd = {};
        rd.Format = sd.BufferDesc.Format;
        rd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

        hr = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)surface.GetAddressOf());
        if (FAILED(hr))
        {
            return hr;
        }

        hr = this->device->CreateRenderTargetView(surface.Get(), &rd, this->renderTargetView.ReleaseAndGetAddressOf());
        if (FAILED(hr))
        {
            return hr;
        }

        return S_OK;
    }
}

HRESULT Graphics::CreateRasterizerState()
{
    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.MultisampleEnable = TRUE;

    return this->device->CreateRasterizerState(&rasterizerDesc, this->rasterizerState.ReleaseAndGetAddressOf());
}

void Graphics::Update()
{
    float color[] = { 114 / 255.0f, 144 / 255.0f, 154 / 255.0f, 1.0f };

    this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), color);
    this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    this->deviceContext->RSSetState(this->rasterizerState.Get());
    this->deviceContext->RSSetViewports(1, &this->viewport);

    this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());
    this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 1);

    this->swapChain->Present(0, 0);
}
