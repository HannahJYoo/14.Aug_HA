#pragma once
#include "GameModel.h"

class GameTank : public GameModel
{
public:
	GameTank();
	~GameTank();

	void Update() override;
	void Render() override;

private:
	ModelBone* lbWheelBone;
	ModelBone* rbWheelBone;
	ModelBone* lfWheelBone;
	ModelBone* rfWheelBone;
	D3DXMATRIX lbWheel;
	D3DXMATRIX rbWheel;
	D3DXMATRIX lfWheel;
	D3DXMATRIX rfWheel;

	ModelBone* turretBone;
	D3DXMATRIX turret;

	D3DXMATRIX tanklocal;

	float angle;

public:
	D3DXMATRIX GetDirection();

	////////////////////////////////////////////////
public:
	void Center(D3DXVECTOR3& val) { val = center; }
	void BoundSize(D3DXVECTOR3& val) { val = boundSize; }
	void Radius(float& val) { val = radius; }

	void GetAABB(std::vector<D3DXVECTOR3>& aabbBox);

	D3DXVECTOR3 MinVertice() { return vecMin; }
	D3DXVECTOR3 MaxVertice() { return vecMax; }


protected:
	std::vector<D3DXVECTOR3> boundSpace;
	D3DXVECTOR3 center;
	D3DXVECTOR3 boundSize;
	D3DXVECTOR3 vecMin, vecMax;
	float radius;

	void SetBoundSpace();
	///////////////////////////////////////////////

};