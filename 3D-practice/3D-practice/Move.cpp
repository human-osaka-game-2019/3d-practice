#include "Move.h"



VOID StepMove(THING* pThing)
{
	D3DXMatrixIdentity(&pThing->matWorld);

	D3DXMatrixRotationY(&pThing->matRotation, pThing->fHeading);

	D3DXVECTOR3 vecAxisX(1.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);

	D3DXVec3TransformCoord(&vecAxisX, &vecAxisX, &pThing->matRotation);
	D3DXVec3TransformCoord(&vecAxisZ, &vecAxisZ, &pThing->matRotation);

	switch (pThing->Dir)
	{
	case LEFT:
		pThing->vecPosition -= vecAxisX * 0.1f;
		camera.vecEyePt -= vecAxisX * 0.1f;
		break;
	case RIGHT:
		pThing->vecPosition += vecAxisX * 0.1f;
		camera.vecEyePt += vecAxisX * 0.1f;
		break;
	case FORWARD:
		pThing->vecPosition += vecAxisZ * 0.1f;
		camera.vecEyePt += vecAxisZ * 0.1f;
		break;
	case BACKWARD:
		pThing->vecPosition -= vecAxisZ * 0.1f;
		camera.vecEyePt -= vecAxisZ * 0.1f;
		break;
	case STOP:
		break;
	default:

		break;
	}
	pThing->Dir = STOP;

	D3DXMatrixTranslation(&Thing->matPosition, pThing->vecPosition.x, pThing->vecPosition.y, pThing->vecPosition.z);
	D3DXMatrixMultiply(&pThing->matWorld, &pThing->matRotation, &Thing->matPosition);
}

