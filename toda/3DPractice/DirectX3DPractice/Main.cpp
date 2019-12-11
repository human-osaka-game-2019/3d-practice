#include"../DirectX/Window.h"
#include"../DirectX/Engine.h"
#include"../DirectX/Device.h"
#include"../DirectX/DrawTexture.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmpLine, INT nCmdShow)
{

	HWND* hWnd;
	Window window;
	DirectX directX;
	Device device;
	Texture texture;
	Engine engine;

	if ((hWnd = engine.InitEngine(800, 600, hInst, &directX, window, device)) == NULL)
	{
		return 0;
	}

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

				engine.DrawStart(&directX);



				engine.DrawEnd(directX);

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
