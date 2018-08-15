#pragma once

class Collision
{
public:
	Collision(class GameModel* model);
	~Collision();

	void Update();

	void SetScale(D3DXVECTOR3& scale);

	void GetBoundingBox(D3DXVECTOR3* boundingBox);
	void GetAABBBox(D3DXVECTOR3* aabbBox);
	float GetRadius() { return radius; }

private:
	void CalAABBBox();

	class GameModel* model;

	D3DXMATRIX postWorld;

	D3DXVECTOR3 boundingBox[8];
	D3DXVECTOR3 aabbBox[2];

	float radius;
};



class GameModel;
class Manifold
{
public:
	Manifold(GameModel* a, GameModel* b);
	~Manifold();

	bool Solve();

private:
	bool AABBCollision();

	GameModel* a;
	GameModel* b;
};