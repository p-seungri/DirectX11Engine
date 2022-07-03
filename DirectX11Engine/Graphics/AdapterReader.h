#pragma once
#include "..\\ErrorLogger.h"
#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib,"DXGI.lib")
#include <wrl\client.h>
#include <vector> 
using namespace std;

class AdapterData
{
public:
	AdapterData(IDXGIAdapter* pAdapter);
	IDXGIAdapter* pAdapter = nullptr;
	DXGI_ADAPTER_DESC description;
};

class AdapterReader
{
public:
	static vector<AdapterData> GetAdapters();
private:
	static vector<AdapterData> adapters;
};