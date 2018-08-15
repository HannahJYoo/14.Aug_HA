#include "stdafx.h"
#include "DrawLandscape.h"
#include "DrawNormalLine.h"
#include "./Viewer/OrbitCamera.h"
#include "../Landscape/Terrain.h"
#include "../Objects/MeshSphere.h"
#include "../Objects/GameBullet.h"
#include "../Objects/GameTank.h"
#include "../Aufgaben/Collision.h"
DrawLandscape::DrawLandscape(ExecuteValues * values)
	:Execute(values)
{
	//Create Terrain
	{
		wstring heightMap = Contents + L"HeightMaps/HeightMap256.png";
		Material* material = new Material(Shaders + L"010_Terrain.hlsl");
		material->SetDiffuseMap(Textures + L"Dirt2.png");

		terrain = new Terrain(material, heightMap);
	}
	//Create Tank
	{
		tank = new GameTank();
		
		D3DXMATRIX S;
		D3DXMatrixScaling(&S, 0.5f, 0.5f, 0.5f);
		tank->RootAxis(S);
		tank->Position(D3DXVECTOR3(80, 10, 120));

		world = tank->World();
	}

	//Create Enemies
	{
		
		
	//	objeckten.reserve(256);

		for (int i = 0; i < 10; i++) {
			enemies.push_back(new MeshSphere());
			D3DXVECTOR3& objPos = enemies[i]->Position();
			enemies[i]->Position(D3DXVECTOR3(Math::Random(0, 256), objPos.y, Math::Random(0, 256)));
			if (terrain->Y(&ground, objPos) == true) 
				objPos.y = ground.y;
			enemies[i]->Position(D3DXVECTOR3(Math::Random(0, 256), objPos.y, Math::Random(0, 256)));
		}
		
	}

	
}

DrawLandscape::~DrawLandscape()
{
	for (GameBullet* bullet : bullets)
		SAFE_DELETE(bullet);
	for (MeshSphere* enemy : enemies)
		SAFE_DELETE(enemy);
	SAFE_DELETE(tank);
	SAFE_DELETE(terrain);
}

void DrawLandscape::Update()
{
	D3DXVECTOR3 position = tank->Position();
	D3DXMATRIX matRotation;
	
	for (int i = 0; i < 10; i++) {
		D3DXVECTOR3& objPos = enemies[i]->Position();
		if (terrain->Y(&ground, objPos) == true)
			objPos.y = ground.y;
		enemies[i]->Position(D3DXVECTOR3(objPos.x, objPos.y, objPos.z));
		//enemies[i]->Position(D3DXVECTOR3(Math::Random(0, 256), objPos.y, Math::Random(0, 256)));
	}
	
	
	
	if (Keyboard::Get()->Up(VK_SPACE)) {
		GameBullet* bullet = new GameBullet;
		bullet->SetLaunchMatrix(tank->GetDirection());
		bullets.push_back(bullet);

	}

	
	D3DXVECTOR3 newPosition;
	if (terrain->Y(&newPosition, position) == true) {
		position.y = newPosition.y;
		
	}
	
	//rotation
	
	

	tank->Position(position);


	/*for (GameBullet* bullet : bullets)
		bullet->Update();*/
	terrain->Update();
	tank->Update();

	for (MeshSphere* enemy : enemies)
		enemy->Update();

	vector<GameBullet*>::iterator bIter;
	vector<MeshSphere*>::iterator eIter;
	for (bIter = bullets.begin(); bIter != bullets.end();)
	{
		(*bIter)->Update();
		for (eIter = enemies.begin(); eIter != enemies.end();)
		{
			Manifold m(*bIter, *eIter);
			if (m.Solve() == true)
			{
				//(*bIter)->Destroy(true);
				SAFE_DELETE(*eIter);
				eIter = enemies.erase(eIter);
				SAFE_DELETE(*bIter);
				bIter = bullets.erase(bIter);
			}
			else
			{
				++eIter;
				++bIter;
			}
		}
		
	}
}

void DrawLandscape::PreRender()
{

}

void DrawLandscape::Render()
{
	for (GameBullet* bullet : bullets)
		bullet->Render();

	for (MeshSphere* enemy : enemies)
		enemy->Render();
	terrain->Render();
	tank->Render();
}

void DrawLandscape::PostRender()
{
	
}

void DrawLandscape::ResizeScreen()
{
}
