#ifndef MAIN_H_
#define MAIN_H_

#include "Directx.h"

//! directxのインスタンス
extern Directx dx;
//! cameraのインスタンス
extern Camera camera;
//! 
/**
 * @brief  ウィンドウプロシージャ
 */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);




#endif // !MAIN_H_
