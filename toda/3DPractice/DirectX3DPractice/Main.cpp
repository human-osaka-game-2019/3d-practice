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
	render.RoadMesh(&thing[1], "Can.x", &D3DXVECTOR3(0, 0, 0), &directX);
	render.RoadMesh(&thing[2], "Ground.x", &D3DXVECTOR3(1, 0, 1), &directX);

	render.InitSphere(directX.pDevice, &thing[0]);
	render.InitSphere(directX.pDevice, &thing[1]);


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
				
				// 左回転
				if (device.GetKeyStatus(DIK_A))
				{
					thing[1].heading += 0.1f;
				}

				// 下回転
				if (device.GetKeyStatus(DIK_S))
				{
					thing[1].pitch += 0.1f;
				}

				// 右回転
				if (device.GetKeyStatus(DIK_D))
				{
					thing[1].heading -= 0.1f;
				}

				// 上移動
				if (device.GetKeyStatus(DIK_W))
				{
					thing[1].pitch -= 0.1f;
				}

				// 右回転
				if (device.GetKeyStatus(DIK_Q))
				{
					thing[1].bank += 0.1;
				}

				// 左回転
				if (device.GetKeyStatus(DIK_E))
				{
					thing[1].bank -= 0.1;
				}
				
				if (device.GetKeyStatus(DIK_SPACE)) // 注視点移動
				{
					// 上
					if (device.GetKeyStatus(DIK_UP))
					{
						render.camera.pitch -= 0.1f;
					}

					// 下
					if (device.GetKeyStatus(DIK_DOWN))
					{
						render.camera.pitch += 0.1f;
					}

					// 右
					if (device.GetKeyStatus(DIK_RIGHT))
					{
						render.camera.yaw += 0.1f;
					}

					// 左
					if (device.GetKeyStatus(DIK_LEFT))
					{
						render.camera.yaw -= 0.1f;
					}

				}
				else if (device.GetKeyStatus(DIK_LSHIFT)) // カメラ
				{
					// 前移動
					if (device.GetKeyStatus(DIK_UP))
					{
						render.camera.z += 0.1f;
						render.camera.z_another += 0.1f;
					}

					// 後移動
					if (device.GetKeyStatus(DIK_DOWN))
					{
						render.camera.z -= 0.1f;
						render.camera.z_another -= 0.1f;
					}
				}
				else if (device.GetKeyStatus(DIK_TAB)) // 一人称
				{
					// 上移動
					if (device.GetKeyStatus(DIK_UP))
					{
						render.camera.y_another -= 0.1f;
					}

					// 下移動
					if (device.GetKeyStatus(DIK_DOWN))
					{
						render.camera.y_another += 0.1f;
					}

					// 右移動
					if (device.GetKeyStatus(DIK_RIGHT))
					{
						render.camera.x_another += 0.1f;
					}

					// 左移動
					if (device.GetKeyStatus(DIK_LEFT))
					{
						render.camera.x_another -= 0.1f;
					}
				}
				else // 注視点
				{
					// 上移動
					if (device.GetKeyStatus(DIK_UP))
					{
						render.camera.y -= 0.1f;
						render.camera.y_another -= 0.1f;
					}

					// 下移動
					if (device.GetKeyStatus(DIK_DOWN))
					{
						render.camera.y += 0.1f;
						render.camera.y_another += 0.1f;
					}

					// 右移動
					if (device.GetKeyStatus(DIK_RIGHT))
					{
						render.camera.x += 0.1f;
						render.camera.x_another += 0.1f;
					}

					// 左移動
					if (device.GetKeyStatus(DIK_LEFT))
					{
						render.camera.x -= 0.1f;
						render.camera.x_another -= 0.1f;
					}

				}

				// 物体
				
				// 右移動
				if (device.GetKeyStatus(DIK_L))
				{
					thing[1].Position.x += 0.1f;
					thing[1].sphere.Center.x += 0.1f;
				}
				// 前物体
				if (device.GetKeyStatus(DIK_I))
				{
					thing[1].Position.z += 0.1f;
				}
				// 左物体
				if (device.GetKeyStatus(DIK_J))
				{
					thing[1].Position.x -= 0.1f;
				}
				// 後物体
				if (device.GetKeyStatus(DIK_K))
				{
					thing[1].Position.z -= 0.1f;
				}

				// -------------------------

				if (device.GetKeyStatus(DIK_SPACE))
				{
					// 拡大(カメラ)
					if (device.GetKeyStatus(DIK_Z))
					{
						render.camera.Perspective += 0.1;
					}

					// 縮小(カメラ)
					if (device.GetKeyStatus(DIK_C))
					{
						render.camera.Perspective -= 0.1f;
					}
				}
				else
				{
					// 拡大(物体)
					if (device.GetKeyStatus(DIK_Z))
					{
						thing[1].scale += 0.1;
					}

					// 縮小(物体)
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
					render.camera.x = 0;
					render.camera.y = 2;
					render.camera.z = -10;
					render.camera.x_another = 0;
					render.camera.y_another = 0;
					render.camera.z_another = 0;
					render.camera.yaw = 0;
					render.camera.pitch = 0;
					render.camera.roll = 0;
					render.camera.Perspective = 4;
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
