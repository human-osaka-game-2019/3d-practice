#ifndef ENGINE_H
#define ENGINE_H


#include"Window.h"
#include"Device.h"

class Engine
{
public:

	HWND* InitEngine(int width, int height, HINSTANCE hInst, DirectX* directX, Window window, Device device);

	void EndEngine(DirectX directX);

	bool DrawStart(DirectX* directX);

	void DrawEnd(DirectX directX);

	void UpdateInput(DirectX* directX, Device device);
};


#endif // !1
