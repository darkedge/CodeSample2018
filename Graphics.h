#pragma once

class Graphics
{
public:
    Graphics(HWND hWnd);
    ~Graphics();

    void Update();

private:
    HRESULT CreateRasterizerState();
    HRESULT CreateDepthStencilState();
    HRESULT CreateRenderTargetView();
    HRESULT CreateDepthTexture(const RECT& rect);

    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthTexture;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

    D3D11_VIEWPORT viewport;
    D3D11_VIEWPORT projection;
};
