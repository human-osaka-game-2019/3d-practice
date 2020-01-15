#include <windows.h>
#include <d3dx9.h>
#include <dinput.h>

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define THING_AMOUNT 4+1

enum DIRECTION
{
	STOP,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

struct THING
{
	LPD3DXMESH pMesh;
	D3DMATERIAL9* pMeshMaterials;
	LPDIRECT3DTEXTURE9* pMeshTextures;
	DWORD dwNumMaterials;
	D3DXVECTOR3 vecPosition;
	D3DXMATRIX matRotation;
	D3DXMATRIX matWorld;
	FLOAT fHeading;
	DIRECTION Dir;
	THING()
	{
		ZeroMemory(this, sizeof(THING));
	}
};

LPDIRECT3D9 pD3d;
LPDIRECT3DDEVICE9 pDevice;

LPDIRECTINPUT8 pDinput;
LPDIRECTINPUTDEVICE8 pKeyDevice;

BYTE Key[256];
HRESULT InitDinput(HWND hWnd);
void UpdateKeyState();
bool GetKey(BYTE);

THING Thing[THING_AMOUNT];
FLOAT fCameraX = 0, fCameraY = 1.0f, fCameraZ = -3.0f, fCameraHeading = 0, fCameraPitch = 0;
FLOAT fPerspective = 4.0f;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT InitD3d(HWND);
HRESULT InitThing(THING*, CONST TCHAR*, D3DXVECTOR3*);
VOID Render();
VOID RenderThing(THING*);
VOID StepMove(THING*);
VOID FreeDx();

//
//INT WINAPI WinMain( HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR szStr,INT iCmdShow)
//アプリケーションのエントリー関数
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	HWND hWnd = NULL;
	MSG msg;
	// ウィンドウの初期化
	static char szAppName[] = "DirectX提出課題";
	WNDCLASSEX  wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);

	hWnd = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW,
		0, 0, 800, 600, NULL, NULL, hInst, NULL);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	// ダイレクト３Dの初期化関数を呼ぶ
	if (FAILED(InitD3d(hWnd)))
	{
		return 0;
	}
	if (FAILED(InitDinput(hWnd)))
	{
		return 0;
	}
	// メッセージループ
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Render();
			UpdateKeyState();
			if (GetKeyState(DIK_RETURN)) 
			{
				break;
			}
		}
	}
	// メッセージループから抜けたらオブジェクトを全て開放する
	FreeDx();
	// OSに戻る（アプリケーションを終了する）
	return (INT)msg.wParam;
}

//
//LRESULT CALLBACK WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
// ウィンドウプロシージャ関数
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch ((CHAR)wParam)
		{
		//キャラの移動はDInputを使用していない
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case VK_LEFT:
			Thing[3].Dir = LEFT;
			break;
		case VK_RIGHT:
			Thing[3].Dir = RIGHT;
			break;
		case VK_UP:
			Thing[3].Dir = FORWARD;
			break;
		case VK_DOWN:
			Thing[3].Dir = BACKWARD;
			break;
		}
		break;
	}

	//キャラの方向転換
	if (Key[DIK_A] & 0x80)
	{
		Thing[3].fHeading -= 0.1f;
	}
	if (Key[DIK_D] & 0x80)
	{
		Thing[3].fHeading += 0.1f;
	}

	//ズームイン、ズームアウト
	if (Key[DIK_I] & 0x80)
	{
		fPerspective += 0.1;
	}
	if (Key[DIK_O] & 0x80)
	{
		fPerspective -= 0.1;
	}

	//カメラの左右移動
	if (Key[DIK_F] & 0x80)
	{
		fCameraX -= 0.1f;
	}
	if (Key[DIK_H] & 0x80)
	{
		fCameraX += 0.1f;
	}

	if (Key[DIK_R] & 0x80)
	{
		fCameraY -= 0.1f;
	}
	if (Key[DIK_Y] & 0x80)
	{
		fCameraY += 0.1f;
	}

	if (Key[DIK_T] & 0x80)
	{
		fCameraZ -= 0.1f;
	}
	if (Key[DIK_B] & 0x80)
	{
		fCameraZ += 0.1f;
	}

	//カメラ回転
	if (Key[DIK_J] & 0x80)
	{
		fCameraHeading -= 0.1f;
	}
	if (Key[DIK_L] & 0x80)
	{
		fCameraHeading += 0.1f;
	}
	if (Key[DIK_K] & 0x80)
	{
		fCameraPitch -= 0.1f;
	}
	if (Key[DIK_M] & 0x80)
	{
		fCameraPitch += 0.1f;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

//
//HRESULT InitD3d(HWND hWnd)
//ダイレクト3Dの初期化関数
HRESULT InitD3d(HWND hWnd)
{
	// 「Direct3D」オブジェクトの作成
	if (NULL == (pD3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(0, "Direct3Dの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}
	// 「DIRECT3Dデバイス」オブジェクトの作成
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

	// Xファイル毎にメッシュを作成する
	InitThing(&Thing[0], "Sky.x", &D3DXVECTOR3(0, 0, 0));
	InitThing(&Thing[1], "Ground.x", &D3DXVECTOR3(0, 0, 0));
	InitThing(&Thing[2], "Building.x", &D3DXVECTOR3(0, 0, 15));
	InitThing(&Thing[3], "Human.x", &D3DXVECTOR3(0, 1.2, 0));
	// Zバッファー処理を有効にする
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// アンビエントライト（環境光）を設定する
	pDevice->SetRenderState(D3DRS_AMBIENT, 0x00111111);
	// スペキュラ（鏡面反射）を有効にする
	pDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	return S_OK;
}

HRESULT InitDinput(HWND hWnd)
{
	HRESULT hr;
	//DirectInputオブジェクトの作成
	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDinput, NULL)))
	{
		return hr;
	}
	//Directデバイスオブジェクトの作成
	if (FAILED(hr = pDinput->CreateDevice(GUID_SysKeyboard,
		&pKeyDevice, NULL)))
	{
		return hr;
	}
	//デバイスをキーボードに設定
	if (FAILED(hr = pKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}
	//協調レベルの設定
	if (FAILED(hr = pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
		//デバイスを取得する
		pKeyDevice->Acquire();
	return S_OK;
}

//
//HRESULT InitThing(THING *pThing,LPSTR szXFileName,D3DXVECTOR3* pvecPosition)
//
HRESULT InitThing(THING* pThing, CONST TCHAR* szXFileName, D3DXVECTOR3* pvecPosition)
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

//
//VOID Render()
//Xファイルから読み込んだメッシュをレンダリングする関数
VOID Render()
{
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(100, 100, 100), 1.0f, 0);

	if (SUCCEEDED(pDevice->BeginScene()))
	{
		for (DWORD i = 0; i < THING_AMOUNT; i++)
		{
			StepMove(&Thing[i]);
			RenderThing(&Thing[i]);
		}
		pDevice->EndScene();
	}
	pDevice->Present(NULL, NULL, NULL, NULL);
}
//
//VOID RenderThing(THING* pThing)
//
VOID RenderThing(THING* pThing)
{
	//ワールドトランスフォーム（絶対座標変換）
	D3DXMATRIXA16 matWorld, matPosition;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation(&matPosition, pThing->vecPosition.x, pThing->vecPosition.y,
		pThing->vecPosition.z);
	D3DXMatrixMultiply(&matWorld, &matWorld, &matPosition);
	pDevice->SetTransform(D3DTS_WORLD, &pThing->matWorld);
	// ビュートランスフォーム（視点座標変換）
	D3DXMATRIXA16 matView,matCameraPosition, matHeading, matPitch;
	D3DXVECTOR3 vecEyePt(fCameraX+5, fCameraY+5, fCameraZ-5); //カメラ（視点）位置
	D3DXVECTOR3 vecLookatPt(fCameraX,fCameraY-1.0f,fCameraZ+3.0f);//注視位置
	D3DXVECTOR3 vecUpVec(0.0f, 1.0f, 0.0f);//上方位置

	//カメラを回転させる際に使用する
	D3DXMatrixIdentity(&matView);
	D3DXMatrixRotationY(&matHeading, fCameraHeading);
	D3DXMatrixRotationX(&matPitch, fCameraPitch);

	D3DXMatrixLookAtLH(&matCameraPosition, &vecEyePt, &vecLookatPt, &vecUpVec);

	//カメラを回転させる際に使用する
	D3DXMatrixMultiply(&matView, &matView, &matHeading);
	D3DXMatrixMultiply(&matView, &matView, &matPitch);
	D3DXMatrixMultiply(&matView, &matView, &matCameraPosition);

	pDevice->SetTransform(D3DTS_VIEW, &matView);
	// プロジェクショントランスフォーム（射影変換）
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / fPerspective, 1.0f, 1.0f, 100.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	// ライトをあてる 白色で鏡面反射ありに設定
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
	// レンダリング	 
	for (DWORD i = 0; i < pThing->dwNumMaterials; i++)
	{
		pDevice->SetMaterial(&pThing->pMeshMaterials[i]);
		pDevice->SetTexture(0, pThing->pMeshTextures[i]);
		pThing->pMesh->DrawSubset(i);
	}
}
//
//VOID StepMove(THING* pThing)
//回転、ステップ移動を含むワールドトランスフォーム行列を計算
VOID StepMove(THING* pThing)
{
	D3DXMATRIXA16 matPosition;
	D3DXMatrixIdentity(&pThing->matWorld);

	//回転行列を計算
	D3DXMatrixRotationY(&pThing->matRotation, pThing->fHeading);
	//ステップ移動量を計算 

	// まずは軸ベクトルを用意する 今回は2軸のみ（Y軸は考慮しない）
	D3DXVECTOR3 vecAxisX(1.0f, 0.0f, 0.0f);//変換前のX軸ベクトル
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);//変換前Z軸ベクトル
	//X、Z軸ベクトルそのものをを現在の回転状態により変換する
	D3DXVec3TransformCoord(&vecAxisX, &vecAxisX, &pThing->matRotation);
	D3DXVec3TransformCoord(&vecAxisZ, &vecAxisZ, &pThing->matRotation);

	switch (pThing->Dir)
	{
	case LEFT:
		pThing->vecPosition -= vecAxisX * 0.1f;
		break;
	case RIGHT:
		pThing->vecPosition += vecAxisX * 0.1f;;
		break;
	case FORWARD:
		pThing->vecPosition += vecAxisZ * 0.1f;
		break;
	case BACKWARD:
		pThing->vecPosition -= vecAxisZ * 0.1f;
		break;
	}
	pThing->Dir = STOP;
	D3DXMatrixTranslation(&matPosition, pThing->vecPosition.x, pThing->vecPosition.y, pThing->vecPosition.z);
	D3DXMatrixMultiply(&pThing->matWorld, &pThing->matRotation, &matPosition);
}

void UpdateKeyState() 
{
	HRESULT hr = pKeyDevice->Acquire();
	if ((hr == DI_OK) || (hr == S_FALSE)) {
		pKeyDevice->GetDeviceState(sizeof(Key), &Key);
	}
}

//
//VOID FreeDx()
// 作成したDirectXオブジェクトの開放
VOID FreeDx()
{
	for (DWORD i = 0; i < THING_AMOUNT; i++)
	{
		SAFE_RELEASE(Thing[i].pMesh);
	}
	SAFE_RELEASE(pDevice);
	SAFE_RELEASE(pD3d);
	SAFE_RELEASE(pDinput);
	SAFE_RELEASE(pKeyDevice);
}