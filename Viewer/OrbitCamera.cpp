#include "stdafx.h"
#include "OrbitCamera.h"

OrbitCamera::OrbitCamera(float moveSpeed, float rotationSpeed)
	:moveSpeed(moveSpeed), rotationSpeed(rotationSpeed)
	, forcusP(0, 0, 0), orbitDistance(10)
{
	Position(0, 0, -10);
	ViewUpdate();
}

OrbitCamera::~OrbitCamera()
{
}

void OrbitCamera::Update()
{
	D3DXVECTOR3 position;
	Position(&position);

	{
		if (Mouse::Get()->Press(1))
		{
			//	회전
			D3DXVECTOR2 rotation;
			Rotation(&rotation);

			D3DXVECTOR3 val = Mouse::Get()->GetMoveValue();

			rotation.x += val.y * rotationSpeed * Time::Delta();
			rotation.y += val.x * rotationSpeed * Time::Delta();

			Rotation(rotation.x, rotation.y);
		}

		if (Keyboard::Get()->Press(VK_OEM_MINUS))
			orbitDistance += 10.0f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_OEM_PLUS))
			orbitDistance -= 10.0f * Time::Delta();

		if (orbitDistance < 0.1f)
			orbitDistance = 0.1f;
	}

	AxisUpdate();

	D3DXVECTOR3 forward;
	Forward(&forward);

	position = forcusP - forward * orbitDistance;
	Position(position.x, position.y, position.z);

	ViewUpdate();
}