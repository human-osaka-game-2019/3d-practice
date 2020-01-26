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
	
	D3DXVECTOR3 vecEyePt(camera.x,camera.y, camera.z); //カメラ（視点）位置
	D3DXVECTOR3 vecLookatPt(camera.x_another, camera.y_another -1.0f,camera.z_another +3.0f);//注視位置
	D3DXVECTOR3 vecUpVec(0.0f, 1.0f, 0.0f);//上方位置

	// 注視点を原点に移動
	D3DXVECTOR3 vec3 = vecEyePt - vecLookatPt;

	D3DXMatrixRotationX(&PitchMatrix, camera.pitch);
	D3DXMatrixRotationY(&YawMatrix, camera.yaw);
	D3DXMatrixRotationZ(&RollMatrix, camera.roll);

	D3DXVec3TransformCoord(&vec3, &vec3, &PitchMatrix);
	D3DXVec3TransformCoord(&vec3, &vec3, &YawMatrix);
	D3DXVec3TransformCoord(&vec3, &vec3, &RollMatrix);

	vecEyePt = vec3 + vecLookatPt;

	D3DXMatrixIdentity(&ViewMatrix);
	D3DXMatrixLookAtLH(&CameraPositionMatrix, &vecEyePt, &vecLookatPt, &vecUpVec);

	D3DXMatrixMultiply(&ViewMatrix, &ViewMatrix, &CameraPositionMatrix);
	directX->pDevice->SetTransform(D3DTS_VIEW, &ViewMatrix);
}

void Render::ProjectionTransform(Thing* thing,DirectX* directX)
{
	D3DXMatrixPerspectiveFovLH(&ProjectionMatrix, D3DX_PI / camera.Perspective, 1.0f, 1.0f, 100.0f);
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
		return false;
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

			if (boRenderSphere && thing[i].pSphereMesh != nullptr)
			{
				directX->pDevice->SetMaterial(thing[1].pSphereMeshMaterials);
				thing[i].pSphereMesh->DrawSubset(0);
			}
		}

		if (Impact(&thing[0], &thing[1]) == TRUE)
		{
			RenderString(directX->pFont, "衝突", 100, 100, directX);
		}
		else
		{
			RenderString(directX->pFont, "未衝突", 100, 100, directX);
		};

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

//HRESULT InitSphere(LPDIRECT3DDEVICE9 pDevice,THING* pThing)
//スフィアの計算およびスフィアを視認可能にするためにスフィアメッシュを作成する
HRESULT Render::InitSphere(LPDIRECT3DDEVICE9 pDevice, Thing* thing)
{
	HRESULT hr = NULL;
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	VOID* pVertices = NULL;
	D3DXVECTOR3 center;
	FLOAT radius;

	//メッシュの頂点バッファーをロックする
	if (FAILED(thing->pMesh->GetVertexBuffer(&pVB)))
	{
		return E_FAIL;
	}
	if (FAILED(pVB->Lock(0, 0, &pVertices, 0)))
	{
		pVB->Release();
		pVB = NULL;
		return E_FAIL;
	}

	// メッシュの外接円の中心と半径を計算する
	hr = D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)pVertices,
		thing->pMesh->GetNumVertices(),
		D3DXGetFVFVertexSize(thing->pMesh->GetFVF()),
		&center,
		&radius);
	
	pVB->Unlock();
	pVB->Release();
	pVB = NULL;

	if (FAILED(hr))
	{
		return hr;
	}
	thing->sphere.Center = center;
	thing->sphere.Radius = radius;

	// 得られた中心と半径を基にメッシュとしてのスフィアを作成する
	hr = D3DXCreateSphere(pDevice, radius, 24, 24, &thing->pSphereMesh, NULL);
	if (FAILED(hr))
	{
		return hr;
	}

	//スフィアメッシュのマテリアル　白色、半透明、光沢強
	thing->pSphereMeshMaterials = new D3DMATERIAL9;
	thing->pSphereMeshMaterials->Diffuse.r = 1.0f;
	thing->pSphereMeshMaterials->Diffuse.g = 1.0f;
	thing->pSphereMeshMaterials->Diffuse.b = 1.0f;
	thing->pSphereMeshMaterials->Diffuse.a = 0.5f;
	thing->pSphereMeshMaterials->Ambient = thing->pSphereMeshMaterials->Diffuse;
	thing->pSphereMeshMaterials->Specular.r = 1.0f;
	thing->pSphereMeshMaterials->Specular.g = 1.0f;
	thing->pSphereMeshMaterials->Specular.b = 1.0f;
	thing->pSphereMeshMaterials->Emissive.r = 0.1f;
	thing->pSphereMeshMaterials->Emissive.g = 0.1f;
	thing->pSphereMeshMaterials->Emissive.b = 0.1f;
	thing->pSphereMeshMaterials->Power = 120.0f;

	return S_OK;
}

//衝突判定
BOOL Render::Impact(Thing* thingA, Thing* thingB)
{
	//２つの物体の中心間の距離を求める
	D3DXVECTOR3 vecLength = thingB->Position - thingA->Position;
	FLOAT fLength = D3DXVec3Length(&vecLength);
	// その距離が、2物体の半径を足したものより小さいということは、
	//境界球同士が重なっている（衝突している）ということ
	if (fLength < thingA->sphere.Radius + thingB->sphere.Radius)
	{
		return TRUE;
	}

	return FALSE;
}

//文字列レンダリング関数
void Render::RenderString(LPD3DXFONT pFont,LPCSTR szStr,INT iX,INT iY,DirectX* directX) 
{
	RECT rect = { iX, iY, 100, 100 }; //表示領域

	directX->pDevice->BeginScene();

	//文字列サイズを計算
	pFont->DrawText(
		NULL,
		szStr,
		-1,             //表示サイズ(-1で全部)
		&rect,          //表示範囲
		DT_CALCRECT,    //表示範囲に調整
		NULL);

	//そのサイズでレンダリング
	pFont->DrawText(
		NULL,
		szStr,
		-1,                     //表示サイズ(-1で全部)
		&rect,                  //表示範囲
		DT_LEFT | DT_BOTTOM,    //左詰めで両端揃え
		0xff00ff00);            //色

	directX->pDevice->EndScene();
	directX->pDevice->Present(NULL, NULL, NULL, NULL);

}
