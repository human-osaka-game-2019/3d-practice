#include<d3dx9.h>
#include<tchar.h>
#include<dinput.h>

#include"window.h"
#include"Engine.h"


DWORD WINAPI Thread(LPVOID* data)
{
	//1000ミリ秒（1秒）おきにループ
	Sleep(1000);
	//MessageBox(0, "Sleep処理が終了しました", NULL, MB_OK);

	ExitThread(0);
}

////// ウィンドウプロシージャー //////

LRESULT CALLBACK WindowProcedure(HWND hInst, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HANDLE th;
	DWORD result;

	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CREATE:
		//スレッドを作成
		th = CreateThread(
			0,                                // 0 でデフォルトの値 
			0,                                // 0 でデフォルトの値
			(LPTHREAD_START_ROUTINE)Thread,   // スレッド実行時の関数 
			(LPVOID)"カウント数表示：",       // スレッド実行時の関数の引数
			0,                                // 実行タイミング      m,
			NULL                              // スレッド識別子を保存するためのDWORD型のポインタの指定
		);

		return 0;

	case WM_CLOSE:

		// スレッドが終わったかチェックする関数
		// 終了していたら終了コードが第二引数に入る
		GetExitCodeThread(
			th,       // スレッドハンドル
			&result   // 終了ステータス(終了すれば STILL_ACTIVE 以外の値が入る)
		);

		//終わったらハンドルを閉じる。
		if (STILL_ACTIVE != result) {
			//closehandleで閉じる。
			CloseHandle(th);
			//ループを抜ける。
			break;
		}
		return 0;
	}

	return DefWindowProc(hInst, iMsg, wParam, lParam);
}

/**

*@fn HWND Make_Wnd(HINSTANCE hInst, int width, int height)
* ウィンドウの初期化、生成関数
* @param hInst  インスタントハンドル
* @param width  ウィンドウの幅の値
* @param height ウィンドウの高さの値
* @return デバイス作成用のインスタントハンドルを返す

*/

//  window用
HWND Window::MakeWindow(HWND* WindowHandle, HINSTANCE hInst, int WindowWidth, int WindowHeight)
{
	////////////////////////// ウィンドウ初期化 //////////////////////////////

	static char szAppName[] = "TEST";
	WNDCLASSEX wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WindowProcedure;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//////////////////////////// ウィンドウ生成 //////////////////////////

	RegisterClassEx(&wndclass);
	*WindowHandle = CreateWindow(szAppName, szAppName, (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) | WS_VISIBLE, 0, 0, WindowWidth, WindowHeight, NULL, NULL, hInst, NULL);

	RECT rx; //ウィンドウ領域
	RECT cx; //クライアント領域
	GetWindowRect(*WindowHandle, &rx);
	GetClientRect(*WindowHandle, &cx);
	const int new_width = WindowWidth + (rx.right - rx.left) - (cx.right - cx.left);
	const int new_height = WindowHeight + (rx.bottom - rx.top) - (cx.bottom - cx.top);
	const int disp_width = GetSystemMetrics(SM_CXSCREEN);
	const int disp_height = GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(*WindowHandle, NULL, (disp_width - new_width) / 2,
		(disp_height - new_height) / 2,
		new_width, new_height, SWP_SHOWWINDOW);

	ShowWindow(*WindowHandle, SW_SHOW);
	UpdateWindow(*WindowHandle);

	return *WindowHandle;
}

// フルスクリーンモード用
HWND Window::MakeFullWindow(HINSTANCE hInst, int WindowWidth, int WindowHeight)
{
	////////////////////////// ウィンドウ初期化 //////////////////////////////

	static char szAppName[] = "TEST";
	WNDCLASSEX wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WindowProcedure;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//////////////////////////// ウィンドウ生成 //////////////////////////

	RegisterClassEx(&wndclass);
	HWND WindowHandle = CreateWindow(szAppName, szAppName, WS_VISIBLE | WS_POPUP, 0, 0, WindowWidth, WindowHeight, NULL, NULL, hInst, NULL);

	//ShowWindow(hInsT, SW_SHOW);
	//UpdateWindow(hInsT);

	return WindowHandle;
}
