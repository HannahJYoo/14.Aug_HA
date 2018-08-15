#pragma once

#define CIRCLEGIZMO_SEGMENTS 36

#define GIZMO_BOUNDINGBOX	0b00000001
#define GIZMO_AABBBOX		0b00000010
#define GIZMO_CIRCLE		0b00000100
#define GIZMO_DIRECTION		0b00001000
#define GIZMO_RIGHT			0b00010000
#define GIZMO_UP			0b00100000

class Zeichnen;
class Gizmo
{
public:
	Gizmo(class GameModel* model, int GIZMO_Flag);
	~Gizmo();

	void Init();
	void Update();
	void Render();

private:
	void BoundingBox(D3DXVECTOR3& minPos, D3DXVECTOR3& maxPos, D3DXCOLOR color = D3DXCOLOR(1, 1, 1, 1));
	void AABBBox(D3DXVECTOR3& minPos, D3DXVECTOR3& maxPos, D3DXCOLOR color = D3DXCOLOR(1, 1, 1, 1));
	void Line(D3DXVECTOR3& startPos, D3DXVECTOR3& endPos, D3DXCOLOR color = D3DXCOLOR(1, 1, 1, 1));
	void Circle(D3DXVECTOR3& centerPos, float radius, D3DXCOLOR color = D3DXCOLOR(1, 1, 1, 1));

private:
	class GameModel* model;
	D3DXVECTOR3 aabb[2];
	Zeichnen* line;
	Zeichnen* aabbLine;
	int flag;
	D3DXVECTOR2 circlePos[CIRCLEGIZMO_SEGMENTS + 1];
};