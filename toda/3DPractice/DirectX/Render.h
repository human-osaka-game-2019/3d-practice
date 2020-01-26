#ifndef RENDER_H
#define RENDER_H

#include<string>

#include "DirectX.h"

struct Sphere
{
	D3DXVECTOR3 Center;
	float Radius;
};

/**
 メッシュのレンダリング用構造体
 */
struct Thing
{
	//! メッシュ
	LPD3DXMESH pMesh;
	//! メッシュマテリアル
	D3DMATERIAL9* pMeshMaterials;
	//! メッシュテクスチャ
	LPDIRECT3DTEXTURE9* pMeshTextures;
	//! マテリアル数
	DWORD dwNumMaterials;
	//! スフィアのメッシュマテリアル
	D3DMATERIAL9* pSphereMeshMaterials;
	//! スフィアのメッシュ
	LPD3DXMESH pSphereMesh = nullptr;
	//! スフィア情報
	Sphere sphere;

	//! 移動変数
	D3DXVECTOR3 Position;

	//! 回転変数
	float pitch = 0;
	float heading = 0;
	float bank = 0;

	//! 大きさの変数
	float scale = 1.0f;

	//! コンストラクタ
	Thing()
	{
		ZeroMemory(this, sizeof(Thing));
		scale = 1;
	}
};

class Camera
{
public:

	//!カメラ変数
	float x = 0;
	float y = 2;
	float z = -10;

	//! 一人称用変数
	float x_another = 0;
	float y_another = 0;
	float z_another = 0;

	//! カメラの回転用変数
	float yaw = 0;
	float pitch = 0;
	float roll = 0;

	//! 拡縮用変数
	float Perspective = 4;

	//! 角度
	float angle_x;
	float angle_y;
	float angle_z;

};

/**
 レンダリング用クラス
*/
class Render
{
public:
	/**
	 * メッシュをロードする関数
	 * @param Thing* thing メッシュのレンダリング構造体のポインター
	 * @param LPCSTR file_name メッシュの名前
	 * @param D3DXVECTOR3* pvecPositio メッシュの初期座標
	 * @param DirectX* directX DirectXClass のポインター
	 * @return 成功なら ture 失敗なら false を返す
	 */
	bool RoadMesh(Thing* thing, LPCSTR file_name, D3DXVECTOR3* pvecPosition,DirectX* directX);

	/**
	 * メッシュをレンダリングする関数
	 * @param Thing* thing メッシュのレンダリング構造体のポインター
	 * @param DirectX* directX DirectXClass のポインター
	 */
	void RenderThing(Thing* thing,DirectX* directX);

	/**
	 * スフィアをレンダリングする関数
	 * @param LPDIRECT3DDEVICE9 pDevice	 pDeviceのポインター
	 * @param Thing* thing メッシュのレンダリング構造体のポインター
	 * @return 成功なら S_OK 失敗なら E_FALE を返す
	 */
	HRESULT InitSphere(LPDIRECT3DDEVICE9 pDevice, Thing* thing);

	/**
	 * 球との当たり判定を取る関数
	 * @param Thing* thingA	判定を取るメッシュのレンダリング構造体のポインター
	 * @param Thing* thingB 判定を取るメッシュのレンダリング構造体のポインター
	 * @return 成功なら S_OK 失敗なら E_FALE を返す
	 */
	BOOL Impact(Thing* thingA, Thing* thingB);

	/**
	 * 文字を描画する関数
	 * @param LPD3DXFONT pFont LPD3DXFONT のポインター
	 * @param LPCSTR szStr 表示する文字列
	 * @param INT iX 表示するX座標
	 * @param INT iY 表示するY座標
	 * @param DirectX* directX DirectXClass のポインター
	 */
	void RenderString(LPD3DXFONT pFont, LPCSTR szStr, INT iX, INT iY,DirectX* directX);

	Camera camera;

private:

	//! ワールドトランスフォームの計算結果保存用行列
	D3DXMATRIXA16 WorldMatrix;
	//! ビュートランスフォームの計算結果保存用行列
	D3DXMATRIXA16 ViewMatrix;
	//! プロジェクショントランスフォームの計算結果保存用行列
	D3DXMATRIXA16 ProjectionMatrix;
	//! メッシュのポジションの計算結果保存用行列
	D3DXMATRIXA16 PositionMatrix;
	//! メッシュの回転の計算結果保存用行列
	D3DXMATRIXA16 RotationMatrix, RotationMatrix_2;
	//! メッシュの大きさの計算結果保存用行列
	D3DXMATRIXA16 ScaleMatrix;
	//! カメラのポジションの計算結果保存用行列
	D3DXMATRIXA16 CameraPositionMatrix;
	//! カメラのヨー方向の計算結果保存用行列
	D3DXMATRIXA16 YawMatrix;
	//! カメラのピッチ方向の計算結果保存用行列
	D3DXMATRIXA16 PitchMatrix;
	//! カメラのロール方向の計算結果保存用行列
	D3DXMATRIXA16 RollMatrix;

	D3DLIGHT9 light;
	
	

	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;

	//! メッシュの最大数
	static const int THING_AMOUNT = 4;

	//! スフィアを表示するかどうかのフラグ
	bool boRenderSphere = true;

	/**
	 * ワールドトランスフォーム関数
	 * @param Thing* thing メッシュのレンダリング構造体のポインター
	 * @param DirectX* directX DirectXClassのポインター
	 */
	void WorldTransform(Thing* thing, DirectX* directX);
	
	/**
	 * ビュートランスフォーム関数
	 * @param Thing* thing メッシュのレンダリング構造体のポインター
	 * @param DirectX* directX DirectXClassのポインター
	 */
	void ViewTransform(Thing* thing, DirectX* directX);
	
	/**
	 * プロジェクショントランスフォーム関数
	 * @param Thing* thing メッシュのレンダリング構造体のポインター
	 * @param DirectX* directX DirectXClassのポインター
	 */
	void ProjectionTransform(Thing* thing, DirectX* directX);
	
	/**
	 * メッシュのレンダリング関数
	 * @param Thing* thing メッシュのレンダリング構造体のポインター
	 * @param DirectX* directX DirectXClassのポインター
	 */
	void Rendering(Thing* thing, DirectX* directX);
	
	/**
	 * ライトの照射関数
	 * @param DirectX* directX DirectXClassのポインター
	 */
	void SetLight(DirectX* directX);

};

#endif
