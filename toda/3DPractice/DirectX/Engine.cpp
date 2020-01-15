#include "Engine.h"

HWND* Engine::InitEngine(int width, int height, HINSTANCE hInst, DirectX* directX, Window window, Device device,Render render, Thing* thing)
{
	HWND hWnd = NULL;
	static CHAR szAppName[] = "";
	FLOAT wnd_width = width;
	FLOAT wnd_height = height;

	window.MakeWindow(&hWnd, hInst, width, height);

	if (FAILED(device.BuildDxDevice(&hWnd, directX,render,thing)))
	{

		return 0;

	}
	return &hWnd;
}

void Engine::EndEngine(DirectX directX)
{

	if (directX.pDinput != nullptr)
	{
		directX.pDinput->Release();
		directX.pDinput = nullptr;
	}
	if (directX.pkey != nullptr)
	{
		directX.pkey->Release();
		directX.pkey = nullptr;
	}
	if (directX.pDevice != nullptr)
	{
		directX.pDevice->Release();
		directX.pDevice = nullptr;
	}
	if (directX.pDirect3d != nullptr)
	{
		directX.pDirect3d->Release();
		directX.pDirect3d = nullptr;
	}

}

bool Engine::DrawStart(DirectX* directX)
{
	directX->pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0f, 0);

	directX->pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	directX->pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	directX->pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (D3D_OK == directX->pDevice->BeginScene())
	{
		return true;
	}

	return false;
}

void Engine::DrawEnd(DirectX directX)
{
	directX.pDevice->EndScene();
	directX.pDevice->Present(NULL, NULL, NULL, NULL);
}

void Engine::UpdateInput(DirectX* directX, Device device)
{
	device.UpdateKeyStatus(directX);
}
