#include "Render.h"

VOID RenderThing(THING* pThing)
{
	D3DXMATRIXA16 LookatRotation;
	D3DXMatrixIdentity(&camera.matView);

	dx.WorldTransform(pThing);
	D3DXMatrixLookAtLH(&LookatRotation, &camera.vecEyePt, &camera.vecLookatPt, &camera.vecUpVec);
	D3DXMatrixMultiply(&camera.matView, &camera.matView, &LookatRotation);
	dx.ViewTransform(&camera);

	D3DXMatrixIdentity(&camera.matProj);
	D3DXMatrixPerspectiveFovLH(&camera.matProj, camera.FOV_Y, camera.aspect, camera.zn, camera.zf);
	dx.ProjectionTransform(&camera);
	dx.SetLight();

	dx.Rendering(pThing);
}

