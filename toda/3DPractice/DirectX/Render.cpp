#include "Render.h"

void Render::WorldTransform(Thing* thing, DirectX* directX)
{
	D3DXMatrixIdentity(&WorldMatrix);
	// 移動処理
	D3DXMatrixTranslation(&PositionMatrix, thing->Position.x, thing->Position.y,thing->Position.z);
	D3DXMatrixMultiply(&WorldMatrix, &WorldMatrix, &PositionMatrix);
	// 回転処理
	D3DXMatrixIdentity(&RotationMatrix);
	D3DXMatrixRotationX(&RotationMatrix_2, thing->pitch);
	D3DXMatrixMultiply(&RotationMatrix, &RotationMatrix, &RotationMatrix_2);
	D3DXMatrixRotationY(&RotationMatrix_2, thing->heading);
	D3DXMatrixMultiply(&RotationMatrix, &RotationMatrix, &RotationMatrix_2);
	D3DXMatrixRotationZ(&RotationMatrix_2, thing->bank);
	D3DXMatrixMultiply(&RotationMatrix, &RotationMatrix, &RotationMatrix_2);
	D3DXMatrixMultiply(&WorldMatrix, &WorldMatrix, &RotationMatrix);
	// 拡縮
	D3DXMatrixScaling(&ScaleMatrix, thing->scale, thing->scale,thing->scale);
	D3DXMatrixMultiply(&WorldMatrix, &WorldMatrix, &ScaleMatrix);

	directX->pDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);
}

void Render::ViewTransform(Thing* thing, DirectX* directX)
{
	D3DXVECTOR3 vecEyePt( //カメラ（視点）位置
		0.0f, // X座標
		1.0f, // Y座標
		-3.0f // Z座標
	);

	D3DXVECTOR3 vecLookatPt( //注視位置
		0.0f, // X座標
		0.0f, // Y座標
		0.0f // Z座標
	);

	D3DXVECTOR3 vecUpVec( //上方位置 (0.0f,1.0f,0.0f) が通常値
		0.0f, // X座標
		3.0f, // Y座標
		0.0f  // Z座標
	);

	/*
	D3DXVECTOR3 vecEyePt(thing->camera_x, thing->camera_y, thing->camera_z); //カメラ（視点）位置
	D3DXVECTOR3 vecLookatPt(0.0f, 1.0f, 3.0f);//注視位置
	D3DXVECTOR3 vecUpVec(0.0f, 1.0f, 0.0f);//上方位置
	D3DXMatrixIdentity(&ViewMatrix);
	D3DXMatrixRotationY(&matHeading, fCameraHeading);
	D3DXMatrixRotationX(&matPitch, fCameraPitch);
	D3DXMatrixLookAtLH(&matCameraPosition, &vecEyePt, &vecLookatPt, &vecUpVec);
	D3DXMatrixMultiply(&matView, &matView, &matHeading);
	D3DXMatrixMultiply(&matView, &matView, &matPitch);
	D3DXMatrixMultiply(&matView, &matView, &matCameraPosition);
	*/
	D3DXMatrixLookAtLH(&ViewMatrix, &vecEyePt, &vecLookatPt, &vecUpVec);
	directX->pDevice->SetTransform(D3DTS_VIEW, &ViewMatrix);
}

void Render::ProjectionTransform(Thing* thing,DirectX* directX)
{
	D3DXMatrixPerspectiveFovLH(&ProjectionMatrix, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	directX->pDevice->SetTransform(D3DTS_PROJECTION, &ProjectionMatrix);
}

void Render::Rendering(Thing* thing,DirectX* directX)
{
	for (DWORD i = 0; i < thing->dwNumMaterials; i++)
	{
		directX->pDevice->SetMaterial(&thing->pMeshMaterials[i]);
		directX->pDevice->SetTexture(0,thing->pMeshTextures[i]);
		thing->pMesh->DrawSubset(i);
	}
}

bool Render::RoadMesh(Thing* thing, LPCSTR file_name, D3DXVECTOR3* pvecPosition,DirectX* directX)
{
	// メッシュの初期位置
	memcpy(&thing->Position, pvecPosition, sizeof(D3DXVECTOR3));

	// Xファイルからメッシュをロードする
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromX(file_name, D3DXMESH_SYSTEMMEM,
		directX->pDevice, NULL, &pD3DXMtrlBuffer, NULL,
		&thing->dwNumMaterials,&thing->pMesh)))
	{
		MessageBox(NULL, "Xファイルの読み込みに失敗しました", NULL, MB_OK);
		return E_FAIL;
	}

	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	
	thing->pMeshMaterials = new D3DMATERIAL9[thing->dwNumMaterials];
	thing->pMeshTextures = new LPDIRECT3DTEXTURE9[thing->dwNumMaterials];

	for (DWORD i = 0; i < thing->dwNumMaterials; i++)
	{
		thing->pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
		thing->pMeshMaterials[i].Ambient = thing->pMeshMaterials[i].Diffuse;
		thing->pMeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(directX->pDevice,
				d3dxMaterials[i].pTextureFilename,&thing->pMeshTextures[i])))
			{
				MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
			}
		}
	}

	pD3DXMtrlBuffer->Release();
	
	return true;
}

void Render::RenderThing(Thing* thing,DirectX* directX)
{
	directX->pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(100, 100, 100), 1.0f, 0);

	if (SUCCEEDED(directX->pDevice->BeginScene()))
	{
		for (DWORD i = 0; i < THING_AMOUNT; i++)
		{
			WorldTransform(&thing[i], directX);
			ViewTransform(&thing[i], directX);
			ProjectionTransform(&thing[i], directX);
			SetLight(directX);
			Rendering(&thing[i], directX);
		}

		directX->pDevice->EndScene();
	}
	directX->pDevice->Present(NULL, NULL, NULL, NULL);
}

void Render::SetLight(DirectX* directX)
{
	D3DXVECTOR3 vecDirection(1, 1, 1);
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Specular.r = 1.0f;
	light.Specular.g = 1.0f;
	light.Specular.b = 1.0f;
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDirection);
	light.Range = 200.0f;
	directX->pDevice->SetLight(0, &light);
	directX->pDevice->LightEnable(0, TRUE);
}
