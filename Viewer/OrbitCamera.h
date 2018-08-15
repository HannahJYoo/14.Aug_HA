#pragma once
#include "Camera.h"

class OrbitCamera : public Camera
{
public:
	OrbitCamera(float moveSpeed = 20.0f, float rotationSpeed = 2.5f);
	~OrbitCamera();

	void Update();

	void SetForcusPosition(D3DXVECTOR3 position) { forcusP = position; }
	D3DXVECTOR3 GetForcusPosition() const { return forcusP; }

	float GetOrbitDistance() const { return orbitDistance; }

private:
	float moveSpeed;
	float rotationSpeed;

	float orbitDistance;
	D3DXVECTOR3 forcusP;
};