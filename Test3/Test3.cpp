// Test3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// http://www.directxtutorial.com/Lesson.aspx?lessonid=11-4-5

#ifndef UNICODE
#define UNICODE
#endif 

#include <Windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

IDXGISwapChain* swapchain;
ID3D11Device* dev;
ID3D11DeviceContext* devcon;
ID3D11RenderTargetView* backbuffer;
ID3D11Buffer* pVBuffer;
ID3D11InputLayout* pLayout;

ID3D11VertexShader* pVS;
ID3D11PixelShader* pPS;

void InitD3D(HWND hWnd);
void CleanD3D(void);
void RenderFrame(void);
void InitPipeline(void);
void InitGraphics(void);

struct VERTEX {
	FLOAT X, Y, Z;
	D3DXCOLOR Color;
};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	HWND hwnd;

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= WindowProc;
	wc.hInstance		= hInstance;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	//wc.hbrBackground	= (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName	= TEXT("WindowClass1");

	RegisterClassEx(&wc);

	RECT wr = { 0,0, SCREEN_WIDTH, SCREEN_HEIGHT };

	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hwnd = CreateWindowEx(NULL,
						  TEXT("WindowClass1"),
						  TEXT("Our First Windowed Program"),
						  WS_OVERLAPPEDWINDOW,
						  CW_USEDEFAULT, CW_USEDEFAULT,
						  SCREEN_WIDTH, SCREEN_HEIGHT,
						  NULL,
						  NULL,
						  hInstance,
						  NULL);

	ShowWindow(hwnd, nCmdShow);

	InitD3D(hwnd);

	MSG msg = {0};

	while (TRUE) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			RenderFrame();
		}
	}

	CleanD3D();
	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		} break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void InitD3D(HWND hWnd)
{
	// DIRECT3D Initialization
	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount			= 1;
	scd.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width	= SCREEN_WIDTH;
	scd.BufferDesc.Height	= SCREEN_HEIGHT;
	scd.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow		= hWnd;
	scd.SampleDesc.Count	= 4;
	scd.Windowed			= TRUE;
	scd.Flags				= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D11CreateDeviceAndSwapChain(NULL,
								  D3D_DRIVER_TYPE_HARDWARE,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  D3D11_SDK_VERSION,
								  &scd,
								  &swapchain,
								  &dev,
								  NULL,
								  &devcon);

	// SET RENDER TARGETS
	ID3D11Texture2D* pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	devcon->OMSetRenderTargets(1, &backbuffer, NULL);

	// SET VIEWPORT NORMALIZE
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX	= 0;
	viewport.TopLeftY	= 0;
	viewport.Width		= SCREEN_WIDTH;
	viewport.Height		= SCREEN_HEIGHT;

	devcon->RSSetViewports(1, &viewport);

	InitPipeline();
	InitGraphics();
}

void CleanD3D() {
	swapchain->SetFullscreenState(FALSE, NULL);

	pLayout->Release();
	pVS->Release();
	pPS->Release();
	pVBuffer->Release();
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();
}

void RenderFrame(void) 
{
	devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

	// Select which vertex buffer to display
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

	// Select which primitive typewe are using
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Draw the vertex buffer to the back buffer;
	devcon->Draw(3, 0);

	swapchain->Present(0, 0);
}

void InitPipeline(void)
{
	ID3D10Blob* VS, * PS;
	D3DX11CompileFromFile(TEXT("shaders.shader"), 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
	D3DX11CompileFromFile(TEXT("shaders.shader"), 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS, 0, 0);

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	devcon->IASetInputLayout(pLayout);
}

void InitGraphics(void) 
{
	// Create a triangle using VERTEX struct
	VERTEX OurVertices[] = { 
		{  0.0f	,  0.5f, 0.0f, D3DXCOLOR(1.0f,0.0f,0.0f,1.0f) },
		{  0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f,1.0f,0.0f,1.0f) },
		{ -0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f,0.0f,1.0f,1.0f) } 
	};

	// Create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

	bd.Usage			= D3D11_USAGE_DYNAMIC;
	bd.ByteWidth		= sizeof(VERTEX) * 3;
	bd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;

	dev->CreateBuffer(&bd, NULL, &pVBuffer);

	// Copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));
	devcon->Unmap(pVBuffer, NULL);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
