#include"../DirectX/Window.h"
#include"../DirectX/Engine.h"
#include"../DirectX/Device.h"
#include"../DirectX/DrawTexture.h"
#include"../DirectX/Render.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmpLine, INT nCmdShow)
{

	HWND* hWnd;
	Window window;
	DirectX directX;
	Device device;
	Texture texture;
	Engine engine;

	Render render;
	Thing thing[4];

	if ((hWnd = engine.InitEngine(1000, 800, hInst, &directX, window, device,render,thing)) == NULL)
	{
		return 0;
	}

	render.RoadMesh(&thing[0], "Chips.x", &D3DXVECTOR3(0, 1, 20), &directX);
	render.RoadMesh(&thing[1], "Can.x", &D3DXVECTOR3(0, 1, 0), &directX);
	render.RoadMesh(&thing[2], "Ground.x", &D3DXVECTOR3(1, 0, 1), &directX);


	timeBeginPeriod(1);//今の時間をtimeに保存。
	DWORD time = timeGetTime();
	DWORD prevtime = 0;
	MSG msg;

	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message != WM_QUIT)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				break;
			}
		}
		else
		{
			if (time - prevtime < 1000 / 60) {

				device.UpdateKeyStatus(&directX);

				render.RenderThing(thing, &directX);
				
				// 左(物体の回転方向(Y軸))
				if (device.GetKeyStatus(DIK_A))
				{
					thing[1].heading += 0.1f;
				}

				// 下(物体の回転方向(X軸))
				if (device.GetKeyStatus(DIK_S))
				{
					thing[1].pitch += 0.1f;
				}

				// 右(物体の回転方向(Y軸))
				if (device.GetKeyStatus(DIK_D))
				{
					thing[1].heading -= 0.1f;
				}

				// 上(物体の移動方向(X軸))
				if (device.GetKeyStatus(DIK_W))
				{
					thing[1].pitch -= 0.1f;
				}

				// 右(物体の回転方向(Z軸))
				if (device.GetKeyStatus(DIK_Q))
				{
					thing[1].bank += 0.1;
				}

				// 左(物体の回転方向(Z軸))
				if (device.GetKeyStatus(DIK_E))
				{
					thing[1].bank -= 0.1;
				}
				
				if (device.GetKeyStatus(DIK_SPACE))
				{
					// 上(カメラの移動方向)
					if (device.GetKeyStatus(DIK_UP))
					{
						render.camera_pitch -= 0.1f;
					}

					// 下(カメラの移動方向)
					if (device.GetKeyStatus(DIK_DOWN))
					{
						render.camera_pitch += 0.1f;
					}

					// 右(カメラの移動方向)
					if (device.GetKeyStatus(DIK_RIGHT))
					{
						render.camera_heading += 0.1f;
					}

					// 左(カメラの移動方向)
					if (device.GetKeyStatus(DIK_LEFT))
					{
						render.camera_heading -= 0.1f;
					}

				}
				else if (device.GetKeyStatus(DIK_LSHIFT))
				{
					// 前(カメラの移動方向)
					if (device.GetKeyStatus(DIK_UP))
					{
						render.camera_z += 0.1f;
						render.camera_z_another += 0.1f;
					}

					// 後ろ(カメラの移動方向)
					if (device.GetKeyStatus(DIK_DOWN))
					{
						render.camera_z -= 0.1f;
						render.camera_z_another -= 0.1f;
					}
				}
				else if (device.GetKeyStatus(DIK_TAB)) // 注点回転
				{
					// 上(カメラの移動方向)
					if (device.GetKeyStatus(DIK_UP))
					{
						render.camera_y_another -= 0.1f;
					}

					// 下(カメラの移動方向)
					if (device.GetKeyStatus(DIK_DOWN))
					{
						render.camera_y_another += 0.1f;
					}

					// 右(カメラの移動方向)
					if (device.GetKeyStatus(DIK_RIGHT))
					{
						render.camera_x_another += 0.1f;
					}

					// 左(カメラの移動方向)
					if (device.GetKeyStatus(DIK_LEFT))
					{
						render.camera_x_another -= 0.1f;
					}
				}
				else // ノーマル回転
				{
					// 上(カメラの移動方向)
					if (device.GetKeyStatus(DIK_UP))
					{
						render.camera_y -= 0.1f;
						render.camera_y_another -= 0.1f;
					}

					// 下(カメラの移動方向)
					if (device.GetKeyStatus(DIK_DOWN))
					{
						render.camera_y += 0.1f;
						render.camera_y_another += 0.1f;
					}

					// 右(カメラの移動方向)
					if (device.GetKeyStatus(DIK_RIGHT))
					{
						render.camera_x += 0.1f;
						render.camera_x_another += 0.1f;
					}

					// 左(カメラの移動方向)
					if (device.GetKeyStatus(DIK_LEFT))
					{
						render.camera_x -= 0.1f;
						render.camera_x_another -= 0.1f;
					}

				}

				// ------------------
				
				// 右(物体の移動方向)
				if (device.GetKeyStatus(DIK_L))
				{
					thing[1].Position.x += 0.1f;
				}
				// 前(物体の移動方向)
				if (device.GetKeyStatus(DIK_I))
				{
					thing[1].Position.z += 0.1f;
				}
				// 左(物体の移動方向)
				if (device.GetKeyStatus(DIK_J))
				{
					thing[1].Position.x -= 0.1f;
				}
				// 後(物体の移動方向)
				if (device.GetKeyStatus(DIK_K))
				{
					thing[1].Position.z -= 0.1f;
				}

				// -------------------------

				if (device.GetKeyStatus(DIK_SPACE))
				{
					// 拡大(物体の拡大)
					if (device.GetKeyStatus(DIK_Z))
					{
						render.Perspective += 0.1;
					}

					// 縮小(物体の縮小)
					if (device.GetKeyStatus(DIK_C))
					{
						render.Perspective -= 0.1f;
					}
				}
				else
				{
					// 拡大(物体の拡大)
					if (device.GetKeyStatus(DIK_Z))
					{
						thing[1].scale += 0.1;
					}

					// 縮小(物体の縮小)
					if (device.GetKeyStatus(DIK_C))
					{
						thing[1].scale -= 0.1f;
					}
				}


				//-----------------------

				// リセット
				if (device.GetKeyStatus(DIK_R))
				{
					thing[1].bank = 0;
					thing[1].heading = 0;
					thing[1].pitch = 0;
					thing[1].Position.x = 0;
					thing[1].Position.y = 1;
					thing[1].Position.z = 0;
					thing[1].scale = 1.0f;
				}

				time = timeGetTime();
			}

			prevtime = time;
		}

	}

	timeEndPeriod(1);

	// エンジン終了
	engine.EndEngine(directX);

	return 0;
}
