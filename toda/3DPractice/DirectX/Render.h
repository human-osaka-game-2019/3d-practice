#ifndef RENDER_H
#define RENDER_H

#include<string>

#include "DirectX.h"

struct Thing
{
	LPD3DXMESH pMesh;
	D3DMATERIAL9* pMeshMaterials;
	LPDIRECT3DTEXTURE9* pMeshTextures;
	DWORD dwNumMaterials;

	//! 移動変数
	D3DXVECTOR3 Position;

	//! 回転変数
	float pitch = 0;
	float heading = 0;
	float bank = 0;

	//! 大きさの変数
	float scale = 1.0f;


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


	float x_another = 0;
	float y_another = 0;
	float z_another = 0;


	float heading = 0;
	float pitch = 0;


	float Perspective = 4;

	// 角度
	float angle_x;
	float angle_y;
	float angle_z;

};

class Render
{
public:

	bool RoadMesh(Thing* thing, LPCSTR file_name, D3DXVECTOR3* pvecPosition,DirectX* directX);

	void RenderThing(Thing* thing,DirectX* directX);

	Camera camera;

private:

	D3DXMATRIXA16 WorldMatrix;
	D3DXMATRIXA16 ViewMatrix;
	D3DXMATRIXA16 ProjectionMatrix;
	D3DXMATRIXA16 PositionMatrix;
	D3DXMATRIXA16 RotationMatrix, RotationMatrix_2;
	D3DXMATRIXA16 ScaleMatrix;
	D3DXMATRIXA16 CameraPositionMatrix;
	D3DXMATRIXA16 PitcMatrix;
	D3DXMATRIXA16 HeadingMatrix;
	D3DXMATRIXA16 PitchMatrix;

	D3DLIGHT9 light;
	
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;


	static const int THING_AMOUNT = 4;

	struct CUSTOMVERTEX
	{
		FLOAT x, y, z;
		DWORD color;
	};
	
	void WorldTransform(Thing* thing, DirectX* directX);
	
	void ViewTransform(Thing* thing, DirectX* directX);
	
	void ProjectionTransform(Thing* thing, DirectX* directX);
	
	void Rendering(Thing* thing, DirectX* directX);
	
	void SetLight(DirectX* directX);




};

#endif
