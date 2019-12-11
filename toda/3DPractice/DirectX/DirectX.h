#ifndef DIRECTX_H
#define DIRECTX_H

#include<windows.h>
#include<d3dx9.h>
#include<tchar.h>
#include<dinput.h>

class DirectX
{
public:
	DirectX();
	~DirectX();

	LPDIRECT3D9 pDirect3d;
	LPDIRECT3DDEVICE9 pDevice;
	LPDIRECTINPUT8 pDinput;
	LPDIRECTINPUTDEVICE8 pkey;
	LPD3DXMESH pMesh;
	D3DMATERIAL9* pMeshMaterials;
	LPDIRECT3DTEXTURE9* pMeshTextures;

	DWORD dwNumMaterials = 0;
};

#endif// !1
