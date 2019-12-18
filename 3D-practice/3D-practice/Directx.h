#ifndef DIRECTX_H_
#define DIRECTX_H_

#include <windows.h>
#include <d3dx9.h>
#include <dinput.h>
#include <tchar.h>

#define THING_AMOUNT 4


/**
 * @enum KEY_STATE　keyの状態
 */
enum KEY_STATE
{
	//! 押したとき
	PUSH,
	//! 離したとき
	RELEASE,
	//! 押している状態
	ON,
	//! 離している状態
	OFF
};

/**
 * @enum DIRECTION 移動の方向
 */
enum DIRECTION
{
	//! 止まっている
	STOP,
	//! 前
	FORWARD,
	//! 後ろ
	BACKWARD,
	//! 左
	LEFT,
	//! 右
	RIGHT,
	//! 上
	UP,
	//! 下
	DOWN
};

/**
 * @brief 球の情報を持った構造体 
 */
struct SPHERE
{
	//! 中心の座標
	D3DXVECTOR3 vecCenter;
	//! 半径
	FLOAT fRadius;
};

/**
 * @brief XFileの情報を持った構造体
 */
struct THING
{
	//! メッシュ
	LPD3DXMESH pMesh;
	//! メッシュマテリアル
	D3DMATERIAL9* pMeshMaterials;
	//! メッシュテクスチャ
	LPDIRECT3DTEXTURE9* pMeshTextures;
	//! マテリアル数
	DWORD dwNumMaterials;
	//! 位置
	D3DXVECTOR3 vecPosition;
	//! 移動行列
	D3DXMATRIXA16 matPosition;
	//! Yの回転行列
	D3DXMATRIX matRotation;
	//! WorldTransform
	D3DXMATRIX matWorld;
	//! Yの角度
	FLOAT fHeading = 0.0f;
	//! 移動方向
	DIRECTION Dir;
	//! 当たり判定で使う球の半径
	FLOAT collision_radius;
	
	THING()
	{
		ZeroMemory(this, sizeof(THING));
	}

	//! Sphereのメッシュ
	LPD3DXMESH pSphereMesh;
	//! Sphereのメッシュマテリアル
	D3DMATERIAL9* pSphereMeshMaterials;
	//! Shereの情報
	SPHERE Sphere;
	//! Sphereをrederingするか
	BOOL RenderSphere = false;
};


/**
 * @brief cameraの情報を持った構造体
 */
struct Camera
{
	//カメラ位置
	D3DXVECTOR3 vecEyePt = { 0.0f, 5.0f, -5.0f };
	//! 注視点
	D3DXVECTOR3 vecLookatPt = { 0.0f,0.0f,1.2f };
	//! 上方ベクトル
	D3DXVECTOR3 vecUpVec = { 0.0f, 1.0f, 0.0f };
	//! Xの回転行列
	D3DXMATRIX matRotationX;
	//! Yの回転行列
	D3DXMATRIX matRotationY;
	//! Zの回転行列
	D3DXMATRIX matRotationZ;
	//! ViewTranceform
	D3DXMATRIXA16 matView;
	//! 角度
	D3DXVECTOR3 degree;
	//! 前のフレームの角度
	D3DXVECTOR3 save_degree;
	//! 移動の方向
	DIRECTION Dir;
	//! 回転したい位置からの距離 == 半径
	D3DXVECTOR3 radius;
	//! ProjectionTransform
	D3DXMATRIXA16 matProj;
	//! Yの視野
	FLOAT FOV_Y = D3DX_PI / 4;
	//! xとyのaspect比..幅/高さ
	FLOAT aspect = 1.0f;
	//! 見る範囲のzの最小値
	FLOAT zn = 1.0f;
	//! 見る範囲のzの最大値
	FLOAT zf = 100.0f;
};






/**
 * @brief Directx 
 */
class Directx 
{

public:

	//! DirectObject
	LPDIRECT3D9 pD3d;
	//! DirextDevice
	LPDIRECT3DDEVICE9 pDevice;
	//! DirectInput
	LPDIRECTINPUT8 pDinput;
	//! DirectKeyBoardDevice
	LPDIRECTINPUTDEVICE8 pIDxKeyDevice;
	//! FontObject
	LPD3DXFONT pFont;
	//! Keyの状態
	KEY_STATE KeyState[256];
	//! Keyの状態を取得する
	inline KEY_STATE GetKey(int diks)
	{
		return KeyState[diks];
	}

	//! 3d
	HRESULT InitD3d(HWND);
	//! DirectInputの生成
	HRESULT InitDirectInput(HWND hWnd);
	//! Fontの生成
	HRESULT InitFont();
	//! 3dモデルの生成
	HRESULT InitThing(THING*, LPCSTR, D3DXVECTOR3*, FLOAT);
	//! 球の生成
	HRESULT InitSphere(THING* pThing);

	/**
	 * @brief  Keyの状態を更新する
	 */
	void UpdateKeyState();
	/**
	 * @brief  描画のスタート
	 */
	void DrawStart();
	/**
	 * @brief  描画のエンド
	 */
	void DrawEnd();
	/**
	 * @brief  ワールドトランスフォーム
	 */
	void WorldTransform(THING* pThing);
	/**
	 * @brief  ビュートランスフォーム
	 */
	void ViewTransform(Camera* camera);
	/**
	 * @brief プロジェクショントランスフォーム
	 */
	void ProjectionTransform(Camera * camera);
	/**
	 * @brief  lightを設定する
	 */
	void SetLight();
	/**
	 * @brief レンダリング
	 */
	void Rendering(THING* pThing);
	/**
	 * @brief  フォントの描画
	 */
	void PrintFont(LONG x, LONG y, const TCHAR* pString);
	/**
	 * @brief  Objectの解放
	 */
	VOID FreeDx();

};

//インスタンス
extern THING Thing[THING_AMOUNT];

#endif // !DIRECTX_H_


