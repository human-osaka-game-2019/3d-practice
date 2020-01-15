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

	//!カメラ変数
	float camera_x = 0;
	float camera_y = 0;
	float camera_z = 0;

	Thing()
	{
		ZeroMemory(this, sizeof(Thing));
		scale = 1;
	}
};

class Render
{
public:

	bool RoadMesh(Thing* thing, LPCSTR file_name, D3DXVECTOR3* pvecPosition,DirectX* directX);

	void RenderThing(Thing* thing,DirectX* directX);



private:

	D3DXMATRIXA16 WorldMatrix;
	D3DXMATRIXA16 ViewMatrix;
	D3DXMATRIXA16 ProjectionMatrix;
	D3DXMATRIXA16 PositionMatrix;
	D3DXMATRIXA16 RotationMatrix, RotationMatrix_2;
	D3DXMATRIXA16 ScaleMatrix;

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
