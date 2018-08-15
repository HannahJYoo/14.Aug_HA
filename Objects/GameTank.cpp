#include "stdafx.h"
#include "GameTank.h"
#include "../Model/ModelMeshPart.h"


GameTank::GameTank()
	: GameModel(Models + L"Tank/", L"Tank.material", Models + L"Tank/", L"Tank.mesh")
{
	 angle=0;

	shader = new Shader(Shaders + L"HA003_Model.hlsl");
	for (Material* material : model->Materials())
	{
		material->SetShader(shader);
	}

	lbWheelBone = model->BoneByName(L"l_back_wheel_geo");
	lbWheel = lbWheelBone->Local();
	
	rbWheelBone = model->BoneByName(L"r_back_wheel_geo");
	rbWheel = rbWheelBone->Local();
	
	lfWheelBone = model->BoneByName(L"l_front_wheel_geo");
	lfWheel = lfWheelBone->Local();
	
	rfWheelBone = model->BoneByName(L"r_front_wheel_geo");
	rfWheel = rfWheelBone->Local();

	turretBone = model->BoneByName(L"turret_geo");
	turret = turretBone->Local();

	tanklocal = this->World();

}

GameTank::~GameTank()
{
	SAFE_DELETE(shader);
}

void GameTank::Update()
{
	__super::Update();	//__super ; 바로 위 부모호출

						//Tank Bewegung
	{
		D3DXVECTOR3 pos = this->Position();
		D3DXVECTOR3 dir = this->Direction();
		float speed = 10.0f;
		float rotSpeed = 100.0f;
		float radian = 90.0f;
		D3DXVECTOR3 up = this->Up();
		D3DXVECTOR3 direction = this->Right();
		D3DXMATRIX matRotation;
		D3DXMATRIX R;

		if (Keyboard::Get()->Press('I')) {
			this->Position(pos - (dir * speed * Time::Get()->Delta()));
			//D3DXMatrixRotationX(&R, sinf(Time::Get()->Running()*2.0f)*0.25f);	//각도*속도
			D3DXMatrixRotationX(&R, Math::ToRadian(Time::Get()->Running()*-rotSpeed));	//각도*속도

			lbWheelBone->Local(R*lbWheel);
			rbWheelBone->Local(R*rbWheel);
			lfWheelBone->Local(R*lfWheel);
			rfWheelBone->Local(R*rfWheel);

		}
		else if (Keyboard::Get()->Press('K')) {
			this->Position(pos + (dir * speed * Time::Get()->Delta()));
			//D3DXMatrixRotationX(&R, sinf(Time::Get()->Running()*2.0f)*0.25f);	//각도*속도
			D3DXMatrixRotationX(&R, Math::ToRadian(Time::Get()->Running()*rotSpeed));	//각도*속도

			lbWheelBone->Local(R*lbWheel);
			rbWheelBone->Local(R*rbWheel);
			lfWheelBone->Local(R*lfWheel);
			rfWheelBone->Local(R*rfWheel);


		}
		if (Keyboard::Get()->Press('J')) {
			D3DXMatrixRotationAxis(&matRotation, &-up, Math::ToRadian(radian)*Time::Get()->Delta());
			//rotate wheel
			//wheelAngle += 0.01f;
			//D3DXMatrixRotationY(&R, wheelAngle);	//각도*속도
			//lbWheelBone->Local(R*lbWheel);

			this->World(matRotation* this->World());
		}
		else if (Keyboard::Get()->Press('L')) {
			D3DXMatrixRotationAxis(&matRotation, &up, Math::ToRadian(radian)*Time::Get()->Delta());
			//rotate wheel
			//wheelAngle -= 0.01f;
			//D3DXMatrixRotationY(&R, wheelAngle);	//각도*속도
			//lbWheelBone->Local(R*lbWheel);

			this->World(matRotation* this->World());
		}

		if (Keyboard::Get()->Press('U')) {
			angle -= 0.01f;
			D3DXMatrixRotationY(&R, angle);
			turretBone->Local(R*turret);
		}
		else if (Keyboard::Get()->Press('O')) {
			angle += 0.01f;
			D3DXMatrixRotationY(&R, angle);
			turretBone->Local(R*turret);
		}

		if (Keyboard::Get()->Press('Z')) {
			D3DXMatrixRotationX(&matRotation, Math::ToRadian(Time::Get()->Running()*0.01f));
			//turretBone->Local(*turret);
			this->World(matRotation* this->World());
			
		}
		else if (Keyboard::Get()->Press('X')) {
			angle -= 0.001f;
			D3DXMatrixRotationX(&matRotation, Math::ToRadian(Time::Get()->Running()*-0.01f));
			//turretBone->Local(*turret);
			this->World(matRotation* this->World());
			
		}

	
	}


}

void GameTank::Render()
{
	__super::Render();
}

D3DXMATRIX GameTank::GetDirection()
{
	D3DXMATRIX temp;

	temp = turretBone->Global() * this->World();

	return temp;
}
void GameTank::GetAABB(std::vector<D3DXVECTOR3>& aabbBox)
{
	float min = -std::numeric_limits<float>().infinity(), max = std::numeric_limits<float>().infinity();
	D3DXVECTOR3 dMin(max, max, max), dMax(min, min, min);
	aabbBox.clear();

	for (D3DXVECTOR3 vec : boundSpace)
	{
		D3DXVec3TransformCoord(&vec, &vec, &World());
		aabbBox.push_back(vec);
	}

	for (D3DXVECTOR3 vec : aabbBox)
	{
		if (dMin.x > vec.x) dMin.x = vec.x;
		if (dMin.y > vec.y) dMin.y = vec.y;
		if (dMin.z > vec.z) dMin.z = vec.z;

		if (dMax.x < vec.x) dMax.x = vec.x;
		if (dMax.y < vec.y) dMax.y = vec.y;
		if (dMax.z < vec.z) dMax.z = vec.z;
	}

	aabbBox.clear();
	aabbBox.reserve(8);
	aabbBox.push_back(D3DXVECTOR3(dMin.x, dMin.y, dMin.z));
	aabbBox.push_back(D3DXVECTOR3(dMin.x, dMax.y, dMin.z));
	aabbBox.push_back(D3DXVECTOR3(dMax.x, dMin.y, dMin.z));
	aabbBox.push_back(D3DXVECTOR3(dMax.x, dMax.y, dMin.z));
	aabbBox.push_back(D3DXVECTOR3(dMin.x, dMin.y, dMax.z));
	aabbBox.push_back(D3DXVECTOR3(dMin.x, dMax.y, dMax.z));
	aabbBox.push_back(D3DXVECTOR3(dMax.x, dMin.y, dMax.z));
	aabbBox.push_back(D3DXVECTOR3(dMax.x, dMax.y, dMax.z));

	return;
}

void GameTank::SetBoundSpace()
{
	float min = -std::numeric_limits<float>().infinity(), max = std::numeric_limits<float>().infinity();
	D3DXVECTOR3 vecMin(max, max, max), vecMax(min, min, min);

	for (ModelMesh* modelMesh : model->Meshes()) {
		ModelBone* bone = modelMesh->ParentBone();
		D3DXMATRIX w = bone->Global();

		for (ModelMeshPart* part : modelMesh->MeshPart()) {
			vector<ModelVertexType> vertices = part->Vertices();

			for (ModelVertexType data : vertices) {
				D3DXVECTOR3 pos = data.Position;
				D3DXVec3TransformCoord(&pos, &pos, &w);
				if (vecMin.x > pos.x) vecMin.x = pos.x;
				if (vecMax.x < pos.x) vecMax.x = pos.x;

				if (vecMin.y > pos.y) vecMin.y = pos.y;
				if (vecMax.y < pos.y) vecMax.y = pos.y;

				if (vecMin.z > pos.z) vecMin.z = pos.z;
				if (vecMax.z < pos.z) vecMax.z = pos.z;
			}
		}
	}


	this->vecMin = vecMin;
	this->vecMax = vecMax;
	center = (vecMax + vecMin) / 2.0f;

	//x,y,z 가장 큰 값으로
	D3DXVECTOR3 temp = vecMax - center;
	if (temp.x < 0) temp.x *= -1;
	if (temp.y < 0) temp.y *= -1;
	if (temp.z < 0) temp.z *= -1;
	radius = max(temp.x, temp.y);
	radius = max(radius, temp.z);

	boundSize = temp * 2.0f;

	boundSpace.reserve(8);
	boundSpace.push_back(D3DXVECTOR3(vecMin.x, vecMin.y, vecMin.z));
	boundSpace.push_back(D3DXVECTOR3(vecMin.x, vecMax.y, vecMin.z));
	boundSpace.push_back(D3DXVECTOR3(vecMax.x, vecMin.y, vecMin.z));
	boundSpace.push_back(D3DXVECTOR3(vecMax.x, vecMax.y, vecMin.z));
	boundSpace.push_back(D3DXVECTOR3(vecMin.x, vecMin.y, vecMax.z));
	boundSpace.push_back(D3DXVECTOR3(vecMin.x, vecMax.y, vecMax.z));
	boundSpace.push_back(D3DXVECTOR3(vecMax.x, vecMin.y, vecMax.z));
	boundSpace.push_back(D3DXVECTOR3(vecMax.x, vecMax.y, vecMax.z));
}
