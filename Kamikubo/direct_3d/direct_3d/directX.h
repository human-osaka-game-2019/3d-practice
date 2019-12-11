/**
* @file DirectX.h
*/

#ifndef DirectX_H_
#define DirectX_H_

#include <windows.h>
#include <d3dx9.h>
#include <dinput.h>
#include <tchar.h>

/**
 * @enum KEY_STATE
 * DirectInputで使用するキーの状態
 */
enum class KEY_STATE
{
	//! 押していない状態
	OFF,
	//! 押した瞬間
	PRESS,
	//! 押している状態
	ON,
	//! 放した瞬間
	RELEASE,

};

/**
* @enum Mode
* 現在どの動作かの判断用
*/
enum class Mode
{
	//! 何もしない
	No,
	//! ジオメトリ移動
	G_Translation,
	//! ジオメトリ回転
	G_Rotation,
	//! カメラの移動、回転
	C_Move,
	//! カメラのズーム操作
	C_Zoom,
};

/**
* @enum CameraMove
* カメラの回転方法
*/
enum class CameraMove
{
	//! ジオメトリを中心に回転
	R1,
	//! 自分の位置を固定して回転
	R2,
};

/**
* @brief ジオメトリ回転用ステータス
*/
struct RotationState
{
	FLOAT x = 0.0f;
	FLOAT y = 0.0f;
	FLOAT z = 0.0f;

	RotationState(FLOAT set_x = 0.0f, FLOAT set_y = 0.0f, FLOAT set_z = 0.0f) :x(set_x), y(set_y), z(set_z) {}
};

/**
* @brief スフィア情報
*/
struct Sphere
{
	D3DXVECTOR3 vecCenter;
	FLOAT fRadius;
};

/**
* @brief ジオメトリ情報
*/
struct Thing
{
	LPD3DXMESH pMesh;
	LPD3DXMESH pSphereMesh;
	D3DMATERIAL9* pMeshMaterials;
	D3DMATERIAL9* pSphereMeshMaterials;
	LPDIRECT3DTEXTURE9* pMeshTextures;
	DWORD dwNumMaterials;
	D3DXVECTOR3 vecPosition;

	RotationState rotation_state;
	Sphere sphere;

	Thing()
	{
		ZeroMemory(this, sizeof(Thing));
	}
};

/**
* @brief カスタムバーテックス
*/
struct CUSTOMVERTEX
{
	FLOAT x, y, z;
	DWORD color;
};

/**
* @brief カメラの情報
*/
struct CameraState
{
	FLOAT x = 22.0f;
	FLOAT y = 12.0f;
	FLOAT z = -18.0f;

	FLOAT heading = 0.0f;
	FLOAT pitch = 0.0f;

	FLOAT perspective = 4.0f;
};

/**
* @brief Windowのサイズ
*/
class WindowSize
{
private:

	WindowSize(const WindowSize&);
	WindowSize& operator =(const WindowSize&);
public:

	WindowSize() {}
	const INT WIDTH = 1920;
	const INT HEIGHT = 1080;
};

/**
* @brief DirectInputで使用するKeyState
*/
class Key
{
private:

	Key(const Key&);
	Key& operator =(const Key&);
public:

	Key() {}
	KEY_STATE m_state[256];
};

/**
* @brief マスク値
*/
class Mask
{
private:

	Mask(const Mask&);
	Mask& operator =(const Mask&);
public:

	Mask() {}
	const INT NUM = 0x80;
};

/**
* @brief 3D用
* @note シングル
*/
class DirectX
{
private:

	WindowSize window_size;

	Key key;

	Mask mask;

	Thing thing[2];

	CameraState camera;

	RECT rect[2];

	Mode m_crrent_mode = Mode::No;

	static DirectX* directx;

	DirectX();

	~DirectX();

	DirectX(const DirectX&);

	DirectX& operator = (const DirectX&);

	static DirectX* instance;

	BOOL is_sphere_render = FALSE;

	CameraMove type = CameraMove::R1;

public:

	// デバイス
	LPDIRECT3DDEVICE9 pDevice;

	//バッファ
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;

	//　Direct3Dのインターフェイス
	LPDIRECT3D9 pDirect3D;

	//　ウィンドウの設定
	D3DPRESENT_PARAMETERS D3dPresentParameters;

	//　DirectInputのキーボードデバイス
	LPDIRECTINPUTDEVICE8 pDxIKeyDevice;

	//　DirectInputのインターフェイス
	LPDIRECTINPUT8 pDinput;

	//フォントオブジェクト
	LPD3DXFONT pFont;

	/**
	* @brief インスタンス取得関数
	* @note インスタンスがNULLの場合はnewしてから返す
	*/
	static DirectX* GetInstance()
	{
		if (directx == NULL)
			directx = new DirectX;
		return directx;
	}

	/**
	* @brief インスタンス破棄用関数
	*/
	static VOID Destroy()
	{
		delete directx;
		directx = NULL;
	}

	/**
	* @brief デバイスの初期化
	* @param hWnd ウィンドウハンドル
	*/
	HRESULT InitDinput(HWND hWnd);

	/**
	* @brief デバイスの作成
	* @param hWnd ウィンドウハンドル
	* @param FilePath ファイルパス
	*/
	HRESULT BuildDxDevice(HWND hWnd, const TCHAR* FilePath);

	/**
	* @brief デバイスの初期化
	* @param hWnd ウィンドウハンドル
	* @param FilePath ファイルパス
	*/
	HRESULT InitD3Device(HWND hWnd, const TCHAR* FilePath);

	/**
	* @brief ウィンドウの初期設定
	* @param hWnd ウィンドウハンドル
	*/
	VOID InitPresentParameters(HWND hWnd);


	HRESULT InitSphere(LPDIRECT3DDEVICE9 pDevice, Thing* pThing);

	/**
	* @brief ライトの設定用関数
	*/
	VOID SetLight();

	/**
	* @brief キーボードの状態を更新
	*/
	VOID UpdateKeyState();

	/**
	* @brief キーの状態を取得
	* @param diks 入手したいキーのマクロ
	* @return キーの状態を返す
	*/
	inline KEY_STATE GetKeyState(INT diks)const
	{
		return key.m_state[diks];
	}

	/**
	* @brief 現在のキー入力状態を取得する
	* @param curr_diks 最上位ビットチェック用配列
	* @param prev_diks 前のキー状態記憶用配列
	* @return 現在のキー入力状態
	*/
	KEY_STATE GetInputState(BYTE curr_diks, KEY_STATE prev_diks);

	/**
	* @brief キーが押されたかを判断する
	* @param curr_diks 最上位ビットチェック用配列
	* @return 押されていたならtrueを返す
	*/
	BOOL IsKeyPressed(BYTE curr_diks);

	/**
	* @brief Xファイル読み込み関数
	* @param pThing ジオメトリ情報
	* @param szXFileName Xファイルの名前
	* @param pvecPosition レンダリングする座標
	*/
	HRESULT InitThing(Thing* pThing, LPCSTR szXFileName, D3DXVECTOR3* pvecPosition);

	/**
	* @brief レンダリングされるものの情報を設定する関数
	* @param pThing ジオメトリ情報
	*/
	VOID RenderThing(Thing* pThing);

	/**
	* @brief スフィアの当たり判定関数
	* @param pThingA ジオメトリ情報
	* @param pThingB ジオメトリ情報
	*/
	BOOL Impact(Thing* pThingA, Thing* pThingB);

	/**
	* @brief 最上位ビットが立っているかを確認し、キーの状態を確認する
	* @sa KEY_STATE GetInputState(BYTE curr_diks, BYTE prev_diks)
	* @sa KEY_STATE GetPrevDik(BYTE curr_diks);
	*/
	VOID CheckInputStateDetails();

	/**
	* @brief レンダリング関数
	*/
	VOID Render();

	/**
	* @brief ワールド、ビュー、射影変換関数
	* @param pThing ジオメトリ情報
	*/
	VOID RefreshMatrices(Thing* pThing);

	/**
	* @brief 動作を切り替える関数
	*/
	VOID ModeChange();

	/**
	* @brief 動作
	*/
	VOID Move();

	/**
	* @brief ジオメトリ回転用関数
	*/
	VOID Rotation();

	/**
	* @brief ジオメトリ移動用関数
	*/
	VOID Translation();

	/**
	* @brief カメラ挙動用関数
	*/
	VOID CameraMove();

	/**
	* @brief カメラのズームイン、ズームアウト用関数
	*/
	VOID CameraZoomChange();
};

#endif
