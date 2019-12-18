#include "Main.h"

#include "Collision.h"
#include "Render.h"
#include "Move.h"


bool FPS = false;

Directx dx;

Camera camera;



void LookatCharacter();




INT WINAPI WinMain(_In_ HINSTANCE hInst,_In_opt_ HINSTANCE hPrevInst,_In_ LPSTR szStr,_In_ INT iCmdShow)
{
	HWND hWnd=NULL;
	MSG msg;
	static char szAppName[] = "Chapter11-2" ;     
     WNDCLASSEX  wndclass ;

     wndclass.cbSize           = sizeof (wndclass) ;
     wndclass.style              = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc    = WndProc ;
     wndclass.cbClsExtra      = 0 ;
     wndclass.cbWndExtra     = 0 ;
     wndclass.hInstance        = hInst ;
     wndclass.hIcon             = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor          = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH) ;
     wndclass.lpszMenuName = NULL ;
     wndclass.lpszClassName = szAppName ;
     wndclass.hIconSm         = LoadIcon (NULL, IDI_APPLICATION) ;

     RegisterClassEx (&wndclass) ;

     hWnd = CreateWindow (szAppName,szAppName,WS_OVERLAPPEDWINDOW,   
                    0,0,800,600,NULL,NULL,hInst,NULL) ;	

    ShowWindow (hWnd,SW_SHOW) ;
    UpdateWindow (hWnd) ;
	SetWindowText(hWnd,"");

	
	if(FAILED(dx.InitD3d(hWnd)))
	{
		return 0;
	}

	dx.InitThing(&Thing[0], _T("Sky.x"), &D3DXVECTOR3(0, 0, 0), 0);
	dx.InitThing(&Thing[1], _T("Ground.x"), &D3DXVECTOR3(0, 0, 0), 0);
	dx.InitThing(&Thing[2], _T("Building.x"), &D3DXVECTOR3(0, 0, 15), 10);
	dx.InitThing(&Thing[3], _T("Human.x"), &D3DXVECTOR3(0, 1.2, 0), 1.2);

	dx.InitSphere(&Thing[2]);
	dx.InitSphere(&Thing[3]);

    ZeroMemory( &msg, sizeof(msg) );
    while( msg.message!=WM_QUIT )
     {
		 if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
         {
			 TranslateMessage( &msg );
             DispatchMessage( &msg );
         }
         else
		 {		
			 dx.UpdateKeyState();
			 dx.DrawStart();
		
			 Move();
			 MoveCamera();

			 for (DWORD i = 0; i < THING_AMOUNT; i++)
			 {
				 StepMove(&Thing[i]);

				 RenderThing(&Thing[i]);
			 }
			 if (isCollision(Thing[2], Thing[3]))
			 {
				 dx.PrintFont(100, 100, "当たってる");
			 }
			 else
			 {
				 dx.PrintFont(100, 100, "当たってない");
			 }

			 dx.DrawEnd();
		 }                    
	 }
	 dx.FreeDx();
     return (INT)msg.wParam ;
}

void LookatCharacter()
{
	camera.vecLookatPt = Thing[3].vecPosition;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{	
	switch(iMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		case WM_KEYDOWN:
		switch((CHAR)wParam)
		{
			case VK_ESCAPE:
				PostQuitMessage(0);
			break;
		}
		break;
	}
	return DefWindowProc (hWnd, iMsg, wParam, lParam) ;	
}



VOID MoveCamera()
{

	D3DXMatrixRotationX(&camera.matRotationX, camera.degree.x);
	D3DXMatrixRotationY(&camera.matRotationY, camera.degree.y);
	D3DXMatrixRotationZ(&camera.matRotationZ, camera.degree.z);

	D3DXVECTOR3 vecAxisX(1.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vecAxisY(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);

	D3DXVec3TransformCoord(&vecAxisX, &vecAxisX, &camera.matRotationY);
	D3DXVec3TransformCoord(&vecAxisY, &vecAxisY, &camera.matRotationY);
	D3DXVec3TransformCoord(&vecAxisZ, &vecAxisZ, &camera.matRotationY);

	switch (camera.Dir)
	{
	case LEFT:
		camera.vecEyePt -= vecAxisX * 0.1f;
		camera.vecLookatPt -= vecAxisX * 0.1f;
		break;
	case RIGHT:
		camera.vecEyePt += vecAxisX * 0.1f;
		camera.vecLookatPt += vecAxisX * 0.1f;
		break;
	case UP:
		camera.vecEyePt += vecAxisY * 0.1f;
		camera.vecLookatPt += vecAxisY * 0.1f;
		break;
	case DOWN:
		camera.vecEyePt -= vecAxisY * 0.1f;
		camera.vecLookatPt -= vecAxisY * 0.1f;
		break;
	case FORWARD:
		camera.vecEyePt += vecAxisZ * 0.1f;
		camera.vecLookatPt += vecAxisZ * 0.1f;
		break;
	case BACKWARD:
		camera.vecEyePt -= vecAxisZ * 0.1f;
		camera.vecLookatPt -= vecAxisZ * 0.1f;
		break;
	case STOP:

		break;
	default:

		break;
	}
	camera.Dir = STOP;

	if (camera.degree == camera.save_degree)
	{
		D3DXMatrixRotationX(&camera.matRotationX,0.0f);
		D3DXMatrixRotationY(&camera.matRotationY,0.0f);
		D3DXMatrixRotationZ(&camera.matRotationZ,0.0f);
	}
	else
	{
		D3DXVECTOR3 camera_move_pos = camera.save_degree - camera.degree;

		D3DXMatrixRotationX(&camera.matRotationX, camera_move_pos.x);
		D3DXMatrixRotationY(&camera.matRotationY, camera_move_pos.y);
		D3DXMatrixRotationZ(&camera.matRotationZ, camera_move_pos.z);
	}
	
	camera.radius = camera.vecEyePt - camera.vecLookatPt;

	D3DXVECTOR3 cameraRotationPos = {camera.radius};

	D3DXVec3TransformCoord(&cameraRotationPos, &cameraRotationPos, &camera.matRotationX);
	D3DXVec3TransformCoord(&cameraRotationPos, &cameraRotationPos, &camera.matRotationY);
	D3DXVec3TransformCoord(&cameraRotationPos, &cameraRotationPos, &camera.matRotationZ);

	camera.vecEyePt = cameraRotationPos + camera.vecLookatPt;
}

VOID Move()
{

	camera.save_degree = camera.degree;
	Thing[3].Sphere.vecCenter = Thing[3].vecPosition;

	//キャラクターの回転
	if (dx.GetKey(DIK_Z) == ON)
	{
		Thing[3].fHeading -= 0.1;
	}
	if (dx.GetKey(DIK_C) == ON)
	{
		Thing[3].fHeading += 0.1;
	}
	//キャラクターの移動
	if (dx.GetKey(DIK_A) == ON)
	{
		Thing[3].Dir = LEFT;
		LookatCharacter();
	}
	if (dx.GetKey(DIK_D) == ON)
	{
		Thing[3].Dir = RIGHT;
		LookatCharacter();
	}
	if (dx.GetKey(DIK_W) == ON)
	{
		Thing[3].Dir = FORWARD;
		LookatCharacter();
	}
	if (dx.GetKey(DIK_S) == ON)
	{
		Thing[3].Dir = BACKWARD;
		LookatCharacter();
	}


	//カメラの移動
	if (dx.GetKey(DIK_RIGHT) == ON)
	{
		camera.Dir = RIGHT;
	}
	if (dx.GetKey(DIK_LEFT) == ON)
	{
		camera.Dir = LEFT;
	}
	if (dx.GetKey(DIK_RSHIFT) == ON)
	{
		camera.Dir = UP;
	}
	if (dx.GetKey(DIK_END) == ON)
	{
		camera.Dir = DOWN;
	}
	if (dx.GetKey(DIK_UP) == ON)
	{
		camera.Dir = FORWARD;
	}
	if (dx.GetKey(DIK_DOWN) == ON)
	{
		camera.Dir = BACKWARD;
	}

	//カメラの回転
	if (dx.GetKey(DIK_J) == ON)
	{
		camera.degree.y -= 0.01;
	}
	if (dx.GetKey(DIK_L) == ON)
	{
		camera.degree.y += 0.01;
	}
	if (dx.GetKey(DIK_U) == ON)
	{
		camera.degree.x += 0.01;
	}
	if (dx.GetKey(DIK_O) == ON)
	{
		camera.degree.x -= 0.01;
	}
	if (dx.GetKey(DIK_I) == ON)
	{
		camera.degree.z += 0.01;
	}
	if (dx.GetKey(DIK_K) == ON)
	{
		camera.degree.z -= 0.01;
	}



	//ズームイン・ズームアウト
	if (dx.GetKey(DIK_Q) == ON)
	{
		camera.FOV_Y -= 0.01f;
	}
	if (dx.GetKey(DIK_E) == ON)
	{
		camera.FOV_Y += 0.01f;
	}

	//モード切替
	if (dx.GetKey(DIK_SPACE) == PUSH)
	{
		if (Thing[2].RenderSphere == true)
		{
			Thing[2].RenderSphere = false;
			Thing[3].RenderSphere = false;
		}
		else if (Thing[2].RenderSphere == false)
		{
			Thing[2].RenderSphere = true;
			Thing[3].RenderSphere = true;
		}
		
	}

}




HRESULT Directx::InitDirectInput(HWND hWnd)
{
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID * *)& pDinput, NULL)))
	{
		MessageBox(0, ("DirectInputの作成に失敗しました"), NULL, MB_OK);
		return E_FAIL;
	}
	if (FAILED(pDinput->CreateDevice(GUID_SysKeyboard, &pIDxKeyDevice, NULL)))
	{
		return E_FAIL;
	}

	if (FAILED(pIDxKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	if (FAILED(pIDxKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return E_FAIL;
	}

	pIDxKeyDevice->Acquire();
	return S_OK;
}

HRESULT Directx::InitFont()
{
	if (FAILED(D3DXCreateFont(
		pDevice,                
		16,                     
		0,                      
		FW_NORMAL,              
		1,                      
		FALSE,                  
		DEFAULT_CHARSET,        
		OUT_DEFAULT_PRECIS,     
		DEFAULT_QUALITY,        
		DEFAULT_PITCH | FF_SWISS,
		"ＭＳ Ｐゴシック",      
		&pFont)))
	{
		MessageBox(0, "フォントの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}
}




