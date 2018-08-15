#include "stdafx.h"
#include "Collision.h"
#include "../Objects/GameModel.h"

Collision::Collision(GameModel * model)
	: model(model)
{
	model->GetModel()->BoundingBox(boundingBox);
	postWorld = model->Transformed();
	CalAABBBox();
}

Collision::~Collision()
{
}

void Collision::Update()
{
	if (postWorld != model->Transformed())
	{
		postWorld = model->Transformed();

		CalAABBBox();
	}
}

void Collision::SetScale(D3DXVECTOR3 &scale)
{
	for (UINT i = 0; i < 8; i++)
	{
		this->boundingBox[i].x *= scale.x;
		this->boundingBox[i].y *= scale.y;
		this->boundingBox[i].z *= scale.z;
	}
}

void Collision::GetBoundingBox(D3DXVECTOR3 * boundingBox)
{
	for (UINT i = 0; i < 8; i++)
		boundingBox[i] = this->boundingBox[i];
}

void Collision::GetAABBBox(D3DXVECTOR3 * aabbBox)
{
	aabbBox[0] = this->aabbBox[0];
	aabbBox[1] = this->aabbBox[1];
}

void Collision::CalAABBBox()
{
	aabbBox[0] = D3DXVECTOR3(10000, 10000, 10000);
	aabbBox[1] = D3DXVECTOR3(-10000, -10000, -10000);

	D3DXVECTOR3 worldBox[8];
	D3DXMATRIX world = model->Transformed();

	for (int i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&worldBox[i], &boundingBox[i], &world);

		if (aabbBox[0].x > worldBox[i].x)
			aabbBox[0].x = worldBox[i].x;
		else if (aabbBox[1].x < worldBox[i].x)
			aabbBox[1].x = worldBox[i].x;

		if (aabbBox[0].y > worldBox[i].y)
			aabbBox[0].y = worldBox[i].y;
		else if (aabbBox[1].y < worldBox[i].y)
			aabbBox[1].y = worldBox[i].y;

		if (aabbBox[0].z > worldBox[i].z)
			aabbBox[0].z = worldBox[i].z;
		else if (aabbBox[1].z < worldBox[i].z)
			aabbBox[1].z = worldBox[i].z;
	}

	D3DXVECTOR3 distance = aabbBox[1] - aabbBox[0];

	radius = distance.x - distance.x;
	radius = (radius > distance.y) ? radius : distance.y;
	radius = (radius > distance.z) ? radius : distance.z;
}

Manifold::Manifold(GameModel* a, GameModel* b)
	: a(a), b(b)
{
}

Manifold::~Manifold()
{
}

bool Manifold::Solve()
{
	return AABBCollision();
}

bool Manifold::AABBCollision()
{
	D3DXVECTOR3 aBox[2];
	D3DXVECTOR3 bBox[2];
	a->GetCollider()->GetAABBBox(aBox);
	b->GetCollider()->GetAABBBox(bBox);

	if (aBox[1].x < bBox[0].x || aBox[0].x > bBox[1].x) return false;
	if (aBox[1].y < bBox[0].y || aBox[0].y > bBox[1].y) return false;
	if (aBox[1].z < bBox[0].z || aBox[0].z > bBox[1].z) return false;
	return true;
}
