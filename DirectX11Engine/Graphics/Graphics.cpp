#include "Graphics.h"

bool Graphics::Initialize(HWND hWnd, int width, int height)
{
	if (!InitializeDirectX(hWnd, width, height))
	{
		return false;
	}

	if (!InitializeShaders())
	{
		return false;
	}

	return true;
}

void Graphics::RenderFrame()
{
	float bgcolor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);
	this->swapChain->Present(1, NULL);

}

bool Graphics::InitializeDirectX(HWND hWnd, int width, int height)
{
	vector<AdapterData> adapters = AdapterReader::GetAdapters();

	if (adapters.size() < 1)
	{
		ErrorLogger::Log("No IDXGI Adapters found.");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hWnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	
	HRESULT hr; 

	hr = D3D11CreateDeviceAndSwapChain(	adapters[0].pAdapter, //IDXGI Adapter
										D3D_DRIVER_TYPE_UNKNOWN, //Graphic Driver Type
										NULL, //Software Driver
										NULL, //Flags for runtime layer.
										NULL, //Feature level array
										0, //# of elements in the feature level array
										D3D11_SDK_VERSION, //SDK version
										&scd, //Swap Chain Description
										this->swapChain.GetAddressOf(), //Swap Chain Address
										this->device.GetAddressOf(), //Device Address
										NULL, //Supported feature level
										this->deviceContext.GetAddressOf() //Device Context Address
									
		);

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create device and swapchain");
		return false;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "GetBuffer Failed");
		return false;
	}

	hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create render target view.");
		return false;
	}

	this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), NULL);

	//Create the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;

	//Set the viewport
	this->deviceContext->RSSetViewports(1, &viewport);

	return true;
}

bool Graphics::InitializeShaders()
{
	wstring shaderfolder = L"";

#pragma region DetermineShaderPath
	if (IsDebuggerPresent() == TRUE)
	{
#ifdef _DEBUG //Debug Mode
	#ifdef _WIN64 //x64
		shaderfolder = L"..\\x64\\Debug\\";
	#else  //x86 (Win32)
		shaderfolder = L"..\\Debug\\";
	#endif
#else //Release Mode
	#ifdef _WIN64 //x64
		shaderfolder = L"..\\x64\\Release\\";
	#else  //x86 (Win32)
		shaderfolder = L"..\\Release\\";
	#endif
#endif
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT numElements = ARRAYSIZE(layout);

	if (!vertexshader.Initialize(this->device, L"..\\x64\\Debug\\vertexshader.cso", layout, numElements))
	{
		return false;
	}

	if (!pixelshader.Initialize(this->device, L"..\\x64\\Debug\\pixelshader.cso"))
	{
		return false;
	}

	return true;
}
