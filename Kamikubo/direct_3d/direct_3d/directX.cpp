#include"directX.h"

namespace
{
	const INT KEY_MAX = 256;
}

DirectX* DirectX::directx = NULL;

DirectX::DirectX()
{
}

HRESULT DirectX::BuildDxDevice(HWND hWnd, CONST TCHAR* filepath)
{
	if (FAILED(InitD3Device(hWnd, filepath)))
	{
		return E_FAIL;
	}

	if (FAILED(InitDinput(hWnd)))
	{
		return E_FAIL;
	}

	pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (pDirect3D == NULL)
	{
		MessageBox(0, _T("Direct3Dの作成に失敗しました"), NULL, MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT DirectX::InitDinput(HWND hWnd)
{
	HRESULT hr;

	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDinput, NULL)))
	{
		MessageBox(0, _T("DirectInputの作成に失敗しました"), NULL, MB_OK);
		return hr;
	}

	if (FAILED(hr = pDinput->CreateDevice(GUID_SysKeyboard, &pDxIKeyDevice, NULL)))
	{
		return hr;
	}

	if (FAILED(hr = pDxIKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}

	if (FAILED(hr = pDxIKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}

	pDxIKeyDevice->Acquire();
	return S_OK;
}

KEY_STATE DirectX::GetInputState(BYTE curr_diks, KEY_STATE prev_diks)
{
	if (IsKeyPressed(curr_diks))
	{
		return (prev_diks == KEY_STATE::OFF) ? KEY_STATE::PRESS : KEY_STATE::ON;
	}

	return (prev_diks == KEY_STATE::ON) ? KEY_STATE::RELEASE : KEY_STATE::OFF;
}

BOOL DirectX::IsKeyPressed(BYTE curr_diks)
{
	// キーが押されているときはcurr_diksの最上位ビットが立っている
	return (curr_diks & mask.NUM) ? TRUE : FALSE;
}

VOID DirectX::CheckInputStateDetails()
{
	BYTE curr_diks[KEY_MAX];

	static KEY_STATE prev_diks[KEY_MAX] = {};

	pDxIKeyDevice->GetDeviceState(sizeof(curr_diks), &curr_diks);

	for (int i = 0; i < KEY_MAX; i++)
	{
		key.m_state[i] = GetInputState(curr_diks[i], prev_diks[i]);
		prev_diks[i] = (IsKeyPressed(curr_diks[i])) ? KEY_STATE::ON : KEY_STATE::OFF;
	}
}

VOID DirectX::UpdateKeyState()
{
	HRESULT hr = pDxIKeyDevice->Acquire();

	if ((hr == DI_OK) || (hr == S_FALSE))
	{
		CheckInputStateDetails();
	}
}

VOID DirectX::ModeChange()
{
	m_crrent_mode = Mode::No;

	if (GetKeyState(DIK_1) == KEY_STATE::ON)
		m_crrent_mode = Mode::G_Translation;

	if (GetKeyState(DIK_2) == KEY_STATE::ON)
		m_crrent_mode = Mode::G_Rotation;

	if (GetKeyState(DIK_3) == KEY_STATE::ON)
		m_crrent_mode = Mode::C_Move;

	if (GetKeyState(DIK_4) == KEY_STATE::ON)
		m_crrent_mode = Mode::C_Zoom;

	if (GetKeyState(DIK_5) == KEY_STATE::PRESS)
		switch (is_sphere_render)
		{
		case TRUE:
			is_sphere_render = FALSE;
			break;
		case FALSE:
			is_sphere_render = TRUE;
			break;
		default:
			break;
		}

}

VOID DirectX::Move()
{
	ModeChange();

	switch (m_crrent_mode)
	{
	case Mode::G_Translation:
		pFont->DrawTextA(NULL, "ジオメトリの移動", -1, &rect[0], DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
		Translation();
		break;
	case Mode::G_Rotation:
		pFont->DrawTextA(NULL, "ジオメトリの回転", -1, &rect[0], DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
		Rotation();
		break;
	case Mode::C_Move:
		pFont->DrawTextA(NULL, "カメラの移動、回転", -1, &rect[0], DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
		CameraMove();
		break;
	case Mode::C_Zoom:
		pFont->DrawTextA(NULL, "カメラのズーム", -1, &rect[0], DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
		CameraZoomChange();
		break;
	default:
		break;
	}
}

VOID DirectX::Translation()
{
	if (GetKeyState(DIK_UP) == KEY_STATE::ON)
		thing[1].vecPosition.y += 0.05;

	if (GetKeyState(DIK_LEFT) == KEY_STATE::ON)
		thing[1].vecPosition.x -= 0.05;

	if (GetKeyState(DIK_DOWN) == KEY_STATE::ON)
		thing[1].vecPosition.y -= 0.05;

	if (GetKeyState(DIK_RIGHT) == KEY_STATE::ON)
		thing[1].vecPosition.x += 0.05;

	if (GetKeyState(DIK_RSHIFT) == KEY_STATE::ON)
		thing[1].vecPosition.z += 0.05;

	if (GetKeyState(DIK_RCONTROL) == KEY_STATE::ON)
		thing[1].vecPosition.z -= 0.05;
}

VOID DirectX::Rotation()
{

	if (GetKeyState(DIK_UP) == KEY_STATE::ON)
		thing[1].rotation_state.x += 0.05;

	if (GetKeyState(DIK_LEFT) == KEY_STATE::ON)
		thing[1].rotation_state.y += 0.05;

	if (GetKeyState(DIK_DOWN) == KEY_STATE::ON)
		thing[1].rotation_state.x -= 0.05;

	if (GetKeyState(DIK_RIGHT) == KEY_STATE::ON)
		thing[1].rotation_state.y -= 0.05;

	if (GetKeyState(DIK_RSHIFT) == KEY_STATE::ON)
		thing[1].rotation_state.z += 0.05;

	if (GetKeyState(DIK_RCONTROL) == KEY_STATE::ON)
		thing[1].rotation_state.z -= 0.05;
}

VOID DirectX::CameraMove()
{
	if (GetKeyState(DIK_UP) == KEY_STATE::ON)
		camera.y -= 0.1f;

	if (GetKeyState(DIK_RIGHT) == KEY_STATE::ON)
		camera.x -= 0.1f;

	if (GetKeyState(DIK_LEFT) == KEY_STATE::ON)
		camera.x += 0.1f;

	if (GetKeyState(DIK_DOWN) == KEY_STATE::ON)
		camera.y += 0.1f;

	if (GetKeyState(DIK_W) == KEY_STATE::ON)
		camera.pitch -= 0.1f;

	if (GetKeyState(DIK_A) == KEY_STATE::ON)
		camera.heading -= 0.1f;

	if (GetKeyState(DIK_S) == KEY_STATE::ON)
		camera.pitch += 0.1f;

	if (GetKeyState(DIK_D) == KEY_STATE::ON)
		camera.heading += 0.1f;

	if (GetKeyState(DIK_SPACE) == KEY_STATE::PRESS)
	{
		switch (type)
		{
		case CameraMove::R1:
			type = CameraMove::R2;
			break;
		case CameraMove::R2:
			type = CameraMove::R1;
			break;
		default:
			break;
		}
	}

}

VOID DirectX::CameraZoomChange()
{
	if (GetKeyState(DIK_UP) == KEY_STATE::ON)
		camera.perspective -= 0.1f;

	if (GetKeyState(DIK_DOWN) == KEY_STATE::ON)
		camera.perspective += 0.1f;
}


HRESULT DirectX::InitD3Device(HWND hWnd, CONST TCHAR* FilePath)
{
	if (NULL == (pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(0, _T("Direct3Dの作成に失敗しました"), _T(""), MB_OK);
		return E_FAIL;
	}

	InitPresentParameters(hWnd);

	if (FAILED(pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_MIXED_VERTEXPROCESSING,
		&D3dPresentParameters, &pDevice)))
	{

		MessageBox(0, _T("HALモードでDIRECT3Dデバイスの作成に失敗しました\nREFモードで再試行します"), NULL, MB_OK);
		if (FAILED(pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
			D3DCREATE_MIXED_VERTEXPROCESSING,
			&D3dPresentParameters, &pDevice)))
		{
			MessageBox(0, _T("DIRECT3Dデバイスの作成に失敗しました"), NULL, MB_OK);
			return E_FAIL;
		}
	}

	// Xファイル毎にメッシュを作成する
	InitThing(&thing[0], "OneMeshLauncher.x", &D3DXVECTOR3(-3, 0, -3));
	InitThing(&thing[1], "OneMeshTank.x", &D3DXVECTOR3(6, 0, 6));

	InitSphere(pDevice, &thing[0]);
	InitSphere(pDevice, &thing[1]);
	// Zバッファー処理を有効にする
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// カリングはしない
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// アンビエントライト（環境光）を設定する
	pDevice->SetRenderState(D3DRS_AMBIENT, 0x00555555);
	// スペキュラ（鏡面反射）を有効にする
	pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	//スフィアを透明にレンダリングしたいのでアルファブレンディングを設定する
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	if (FAILED(D3DXCreateFont(
		pDevice,                 /* デバイス */
		60,                      /* 文字の高さ */
		0,                       /* 文字幅 */
		FW_NORMAL,               /* フォントの太さ */
		1,                       /* MIPMAPのレベル */
		FALSE,                   /* イタリックか？ */
		DEFAULT_CHARSET,         /* 文字セット */
		OUT_DEFAULT_PRECIS,      /* 出力精度 */
		DEFAULT_QUALITY,         /* 出力品質 */
		DEFAULT_PITCH | FF_SWISS,/* フォントピッチとファミリ */
		_T("ＭＳ Ｐゴシック"),       /* フォント名 */
		&pFont)))
	{
		MessageBox(0, _T("フォントの作成に失敗しました"), "", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

//完全コピー
HRESULT DirectX::InitSphere(LPDIRECT3DDEVICE9 pDevice, Thing* pThing)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	VOID* pVerties = NULL;
	D3DXVECTOR3 vecCenter;
	FLOAT fRadius;

	if (FAILED(pThing->pMesh->GetVertexBuffer(&pVB)))
	{
		return E_FAIL;
	}

	if (FAILED(pVB->Lock(0, 0, &pVerties, 0)))
	{
		pVB->Release();
		return E_FAIL;
	}

	HRESULT hr = D3DXComputeBoundingSphere((D3DXVECTOR3*)pVerties, pThing->pMesh->GetNumVertices(),
		D3DXGetFVFVertexSize(pThing->pMesh->GetFVF()), &vecCenter,
		&fRadius);

	pVB->Unlock();
	pVB->Release();

	if (FAILED(hr))
	{
		return hr;
	}

	pThing->sphere.vecCenter = vecCenter;
	pThing->sphere.fRadius = fRadius;

	hr = D3DXCreateSphere(pDevice, fRadius, 24, 24, &pThing->pSphereMesh, NULL);

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
	pThing->pSphereMeshMaterials->Emissive.r = 0.1f;
	pThing->pSphereMeshMaterials->Emissive.g = 0.1f;
	pThing->pSphereMeshMaterials->Emissive.b = 0.1f;
	pThing->pSphereMeshMaterials->Power = 120.0f;

	return S_OK;
}

//完全コピー
HRESULT DirectX::InitThing(Thing* pThing, LPCSTR szXFileName, D3DXVECTOR3* pvecPosition)
{
	// メッシュの初期位置
	memcpy(&pThing->vecPosition, pvecPosition, sizeof(D3DXVECTOR3));
	// Xファイルからメッシュをロードする	
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromX(szXFileName, D3DXMESH_SYSTEMMEM,
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

VOID DirectX::InitPresentParameters(HWND hWnd)
{
	ZeroMemory(&D3dPresentParameters, sizeof(D3dPresentParameters));

	D3dPresentParameters.BackBufferWidth = window_size.WIDTH;
	D3dPresentParameters.BackBufferHeight = window_size.HEIGHT;
	D3dPresentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	D3dPresentParameters.BackBufferCount = 1;
	D3dPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	D3dPresentParameters.MultiSampleQuality = 0;
	D3dPresentParameters.hDeviceWindow = hWnd;
	D3dPresentParameters.EnableAutoDepthStencil = TRUE;
	D3dPresentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	D3dPresentParameters.Flags = 0;
	D3dPresentParameters.FullScreen_RefreshRateInHz = 0;
	D3dPresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	D3dPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3dPresentParameters.Windowed = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

VOID DirectX::RefreshMatrices(Thing* pThing)
{
	//ワールドトランスフォーム（絶対座標変換）
	D3DXMATRIXA16 matWorld, matPosition, matRotation;
	D3DXMatrixIdentity(&matWorld);

	//ジオメトリ回転
	D3DXMatrixRotationYawPitchRoll(&matRotation, pThing->rotation_state.y, pThing->rotation_state.x, pThing->rotation_state.z);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matRotation);

	D3DXMatrixTranslation(&matPosition, pThing->vecPosition.x, pThing->vecPosition.y,
		pThing->vecPosition.z);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matPosition);

	pDevice->SetTransform(D3DTS_WORLD, &matWorld);

	//ビュートランスフォーム（視点座標変換）
	D3DXVECTOR3 vecEyePt(camera.x, camera.y, camera.z);//カメラ位置
	D3DXVECTOR3 vecLookatPt(camera.x - 22.0f, camera.y - 12.0f, camera.z + 18.0f);//注視位置
	D3DXVECTOR3 vecUpVec(0.0f, 1.0f, 0.0f);//上方位置
	D3DXMATRIXA16 matView, matCameraPos, matHeading, matPitch;

	D3DXMatrixIdentity(&matView);

	//カメラ回転
	D3DXMatrixRotationX(&matPitch, camera.pitch);
	D3DXMatrixRotationY(&matHeading, camera.heading);

	D3DXMatrixLookAtLH(&matCameraPos, &vecEyePt, &vecLookatPt, &vecUpVec);

	switch (type)
	{
	case CameraMove::R1:
		D3DXMatrixMultiply(&matView, &matView, &matHeading);
		D3DXMatrixMultiply(&matView, &matView, &matPitch);
		D3DXMatrixMultiply(&matView, &matView, &matCameraPos);
		break;
	case CameraMove::R2:
		D3DXMatrixMultiply(&matView, &matView, &matCameraPos);
		D3DXMatrixMultiply(&matView, &matView, &matHeading);
		D3DXMatrixMultiply(&matView, &matView, &matPitch);
		break;
	default:
		break;
	}

	pDevice->SetTransform(D3DTS_VIEW, &matView);

	// プロジェクショントランスフォーム（射影変換）
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / camera.perspective, 1.0f, 1.0f, 100.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

VOID DirectX::SetLight()
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

	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDirection);
	light.Range = 200.0f;
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);
}

VOID DirectX::RenderThing(Thing* pThing)
{
	RefreshMatrices(pThing);

	//InitRender();

	// ライトをあてる 白色で鏡面反射ありに設定
	SetLight();

	// レンダリング	 
	for (DWORD i = 0; i < pThing->dwNumMaterials; i++)
	{
		pDevice->SetMaterial(&pThing->pMeshMaterials[i]);
		pDevice->SetTexture(0, pThing->pMeshTextures[i]);
		pThing->pMesh->DrawSubset(i);
	}

	if (is_sphere_render)
	{
		pDevice->SetMaterial(pThing->pSphereMeshMaterials);
		pThing->pSphereMesh->DrawSubset(0);
	}

}

BOOL DirectX::Impact(Thing* pThingA, Thing* pThingB)
{
	D3DXVECTOR3 vecLenght = pThingB->vecPosition - pThingA->vecPosition;

	FLOAT fLenght = D3DXVec3Length(&vecLenght);

	if (fLenght < pThingA->sphere.fRadius + pThingB->sphere.fRadius)
	{
		return TRUE;
	}

	return FALSE;
}

VOID DirectX::Render()
{

	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_XRGB(100, 100, 100), 1.0f, 0);

	SetRect(&rect[0], 50, 50, 60, 100);
	SetRect(&rect[1], 100, 300, 500, 500);

	if (SUCCEEDED(pDevice->BeginScene()))
	{
		Move();

		for (DWORD i = 0; i < 2; i++)
		{
			RenderThing(&thing[i]);
		}

		if (Impact(&thing[0], &thing[1]))
		{
			pFont->DrawTextA(NULL, "衝突", -1, &rect[1], DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
		}

		pDevice->EndScene();
	}

	pDevice->Present(NULL, NULL, NULL, NULL);
}

/////////////////////////////////////////////////////////////////////////////////////////

DirectX::~DirectX()
{

	for (DWORD i = 0; i < 2; i++)
	{
		thing[i].pMesh->Release();
	}
	pDevice->Release();
	pDinput->Release();
	pDxIKeyDevice->Release();
	pFont->Release();
}