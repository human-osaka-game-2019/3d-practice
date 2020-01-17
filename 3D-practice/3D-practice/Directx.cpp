

#include "Directx.h"


#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

THING Thing[THING_AMOUNT];


HRESULT Directx::InitD3d(HWND hWnd)
{
	if (NULL == (pD3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(0, "Direct3Dの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_MIXED_VERTEXPROCESSING,
		&d3dpp, &pDevice)))
	{
		MessageBox(0, "HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します", NULL, MB_OK);
		if (FAILED(pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
			D3DCREATE_MIXED_VERTEXPROCESSING,
			&d3dpp, &pDevice)))
		{
			MessageBox(0, "DIRECT3Dデバイスの作成に失敗しました", NULL, MB_OK);
			return E_FAIL;
		}
	}

	InitDirectInput(hWnd);
	InitFont();

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_AMBIENT, 0x00111111);
	pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	return S_OK;
}

HRESULT Directx::InitThing(THING* pThing, LPCSTR szXFileName, D3DXVECTOR3* pvecPosition, FLOAT collision_radius)
{
	pThing->collision_radius = collision_radius;

	memcpy(&pThing->vecPosition, pvecPosition, sizeof(D3DXVECTOR3));
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromX(_T(szXFileName), D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &pD3DXMtrlBuffer, NULL,
		&pThing->dwNumMaterials, &pThing->pMesh)))
	{
		MessageBox(NULL, "Xファイルの読み込みに失敗しました", szXFileName, MB_OK);
		return E_FAIL;
	}
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	pThing->pMeshMaterials = new D3DMATERIAL9[pThing->dwNumMaterials];
	pThing->pMeshTextures = new LPDIRECT3DTEXTURE9[pThing->dwNumMaterials];

	for (DWORD i = 0; i < pThing->dwNumMaterials; i++)
	{
		pThing->pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
		pThing->pMeshMaterials[i].Ambient = pThing->pMeshMaterials[i].Diffuse;
		pThing->pMeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(pDevice,
				d3dxMaterials[i].pTextureFilename,
				&pThing->pMeshTextures[i])))
			{
				MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
			}
		}
	}
	pD3DXMtrlBuffer->Release();

	return S_OK;
}


void Directx::UpdateKeyState()
{

	BYTE curr_diks[256];

	static KEY_STATE prev_diks[256] = { KEY_STATE::OFF };

	HRESULT hr = pIDxKeyDevice->Acquire();

	if ((hr == DI_OK) || (hr == S_FALSE))
	{
		pIDxKeyDevice->GetDeviceState(sizeof(curr_diks), &curr_diks);
		for (INT i = 0; i < 256; i++)
		{
			if (curr_diks[i] & 0x80)
			{
				if (prev_diks[i] == KEY_STATE::OFF)
				{
					KeyState[i] = KEY_STATE::PUSH;
				}
				else
				{
					KeyState[i] = KEY_STATE::ON;
				}

				prev_diks[i] = KEY_STATE::ON;
			}
			else {
				if (prev_diks[i] == KEY_STATE::ON)
				{
					KeyState[i] = KEY_STATE::RELEASE;
				}
				else {
					KeyState[i] = KEY_STATE::OFF;
				}
				prev_diks[i] = KEY_STATE::OFF;
			}

		}
	}
}

HRESULT Directx::InitSphere(THING* pThing)
{
	HRESULT hr = NULL;
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	VOID* pVertices = NULL;
	D3DXVECTOR3 vecCenter;
	FLOAT fRadius;

	pThing->RenderSphere = true;

	if (FAILED(hr))
	{
		return hr;
	}
	pThing->Sphere.vecCenter = pThing->vecPosition;
	pThing->Sphere.fRadius = pThing->collision_radius;
	hr = D3DXCreateSphere(pDevice, pThing->Sphere.fRadius, 24, 24, &pThing->pSphereMesh, NULL);
	if (FAILED(hr))
	{
		return hr;
	}

	pThing->pSphereMeshMaterials = new D3DMATERIAL9;
	pThing->pSphereMeshMaterials->Diffuse.r = 1.0f;
	pThing->pSphereMeshMaterials->Diffuse.g = 1.0f;
	pThing->pSphereMeshMaterials->Diffuse.b = 1.0f;
	pThing->pSphereMeshMaterials->Diffuse.a = 0.5f;
	pThing->pSphereMeshMaterials->Ambient = pThing->pSphereMeshMaterials->Diffuse;
	pThing->pSphereMeshMaterials->Specular.r = 1.0f;
	pThing->pSphereMeshMaterials->Specular.g = 1.0f;
	pThing->pSphereMeshMaterials->Specular.b = 1.0f;
	pThing->pSphereMeshMaterials->Specular.a = 0.5f;
	pThing->pSphereMeshMaterials->Emissive.r = 0.1f;
	pThing->pSphereMeshMaterials->Emissive.g = 0.1f;
	pThing->pSphereMeshMaterials->Emissive.b = 0.1f;
	pThing->pSphereMeshMaterials->Emissive.a = 0.5f;
	pThing->pSphereMeshMaterials->Power = 120.0f;

	return S_OK;
}


void Directx::DrawStart()
{
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(100, 100, 100), 1.0f, 0);
	pDevice->BeginScene();
}

void Directx::DrawEnd()
{
	pDevice->EndScene();
	pDevice->Present(NULL, NULL, NULL, NULL);
}


void Directx::WorldTransform(THING* pThing)
{
	pDevice->SetTransform(D3DTS_WORLD, &pThing->matWorld);
}

void Directx::ViewTransform(Camera*camera)
{
	pDevice->SetTransform(D3DTS_VIEW, &camera->matView);
}

void Directx::ProjectionTransform(Camera* camera)
{
	pDevice->SetTransform(D3DTS_PROJECTION, &camera->matProj);
}

void Directx::Rendering(THING* pThing)
{
	for (DWORD i = 0; i < pThing->dwNumMaterials; i++)
	{
		pDevice->SetMaterial(&pThing->pMeshMaterials[i]);
		pDevice->SetTexture(0, pThing->pMeshTextures[i]);
		pThing->pMesh->DrawSubset(i);
	}

	if (pThing->RenderSphere)
	{
		pDevice->SetMaterial(pThing->pSphereMeshMaterials);
		pThing->pSphereMesh->DrawSubset(0);
	}
}

void Directx::SetLight()
{
	D3DXVECTOR3 vecDirection(1, 1, 1);
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Specular.r = 1.0f;
	light.Specular.g = 1.0f;
	light.Specular.b = 1.0f;
	D3DXVec3Normalize((D3DXVECTOR3*)& light.Direction, &vecDirection);
	light.Range = 200.0f;
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);
}

void Directx::PrintFont(LONG x, LONG y, const TCHAR* pString)
{
	RECT rect;

	rect = { };

	SetRect(&rect, x, y, 0, 0);

	//表示
	pFont->DrawText(
		NULL,
		pString,
		-1,
		&rect,
		DT_LEFT | DT_NOCLIP,
		D3DCOLOR_ARGB(255, 0xff, 0xff, 0xff)
	);
}

VOID Directx::FreeDx()
{
	for (DWORD i = 0; i < 4; i++)
	{
		SAFE_RELEASE(Thing[i].pMesh);
	}
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pDevice);
	SAFE_RELEASE(pD3d);
}


