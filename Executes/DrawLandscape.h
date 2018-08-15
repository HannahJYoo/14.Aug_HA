#pragma once
#include "Execute.h"

class DrawNormalLine;
class DrawLandscape : public Execute
{

public:
	DrawLandscape(ExecuteValues* values);
	~DrawLandscape();

	// Execute��(��) ���� ��ӵ�
	void Update();
	void PreRender();
	void Render();
	void PostRender();
	void ResizeScreen();


private:
	class Terrain* terrain;
	class GameTank* tank;
	vector<class GameBullet*> bullets;
	vector<class MeshSphere*> enemies;

	
	D3DXVECTOR3 ground;
	D3DXMATRIX world;
	float normalAngle;
};