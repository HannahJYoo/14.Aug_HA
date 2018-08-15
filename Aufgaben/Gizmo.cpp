#include "stdafx.h"
#include "Gizmo.h"
#include "../Objects/GameModel.h"
#include "../Aufgaben/Collision.h"
#include "Zeichnen.h"

Gizmo::Gizmo(GameModel* model, int GIZMO_Flag)
	: model(model), flag(GIZMO_Flag)
{
	Init();
}

Gizmo::~Gizmo()
{
	SAFE_DELETE(aabbLine);
	SAFE_DELETE(line);
}

void Gizmo::Init()
{
	float intervalAngle = (D3DX_PI * 2.0f) / CIRCLEGIZMO_SEGMENTS;
	for (int i = 0; i <= CIRCLEGIZMO_SEGMENTS; i++) {
		float angle = i * intervalAngle;
		circlePos[i] = D3DXVECTOR2(cos(angle), sin(angle));
	}

	this->model->GetCollider()->GetAABBBox(aabb);
	D3DXVECTOR3 center = (aabb[0] + aabb[1]) / 2;
	float radius = this->model->GetCollider()->GetRadius();
	line = new Zeichnen();
	aabbLine = new Zeichnen();

	if (flag & GIZMO_BOUNDINGBOX)
		BoundingBox(aabb[0], aabb[1]);

	if (flag & GIZMO_AABBBOX)
		AABBBox(aabb[0], aabb[1]);

	if (flag & GIZMO_CIRCLE)
		Circle(center, radius);

	if (flag & GIZMO_DIRECTION)
		Line(center, center + this->model->Direction() * radius * 2, D3DXCOLOR(0, 0, 1, 1));

	if (flag & GIZMO_RIGHT)
		Line(center, center + this->model->Right() * radius * 2, D3DXCOLOR(1, 0, 0, 1));

	if (flag & GIZMO_UP)
		Line(center, center + this->model->Up() * radius * 2, D3DXCOLOR(0, 1, 0, 1));
}

void Gizmo::Update()
{
	line->Update(model->Transformed());
	if (flag & GIZMO_AABBBOX)
	{
		this->model->GetCollider()->GetAABBBox(aabb);
		AABBBox(aabb[0], aabb[1]);
		D3DXMATRIX mat;
		D3DXMatrixIdentity(&mat);
		aabbLine->Update(mat);
	}
}

void Gizmo::Render()
{
	line->Render();
	if (flag & GIZMO_AABBBOX)
		aabbLine->Render();
}

void Gizmo::Line(D3DXVECTOR3 & startPos, D3DXVECTOR3 & endPos, D3DXCOLOR color)
{
	line->AddLine(startPos, endPos, color);
}

void Gizmo::Circle(D3DXVECTOR3 & centerPos, float radius, D3DXCOLOR color)
{
	D3DXVECTOR3 circle[2];
	for (int i = 0; i < CIRCLEGIZMO_SEGMENTS; i++)
	{
		line->AddLine(centerPos + D3DXVECTOR3(circlePos[i].x, circlePos[i].y, 0.0f) * radius, centerPos + D3DXVECTOR3(circlePos[i + 1].x, circlePos[i + 1].y, 0.0f) * radius, color);
		line->AddLine(centerPos + D3DXVECTOR3(circlePos[i].x, 0.0f, circlePos[i].y) * radius, centerPos + D3DXVECTOR3(circlePos[i + 1].x, 0.0f, circlePos[i + 1].y) * radius, color);
		line->AddLine(centerPos + D3DXVECTOR3(0.0f, circlePos[i].x, circlePos[i].y) * radius, centerPos + D3DXVECTOR3(0.0f, circlePos[i + 1].x, circlePos[i + 1].y) * radius, color);
	}
}

void Gizmo::BoundingBox(D3DXVECTOR3 & minPos, D3DXVECTOR3 & maxPos, D3DXCOLOR color)
{
	Line(D3DXVECTOR3(minPos.x, minPos.y, minPos.z), D3DXVECTOR3(maxPos.x, minPos.y, minPos.z), color);
	Line(D3DXVECTOR3(maxPos.x, minPos.y, minPos.z), D3DXVECTOR3(maxPos.x, minPos.y, maxPos.z), color);
	Line(D3DXVECTOR3(maxPos.x, minPos.y, maxPos.z), D3DXVECTOR3(minPos.x, minPos.y, maxPos.z), color);
	Line(D3DXVECTOR3(minPos.x, minPos.y, maxPos.z), D3DXVECTOR3(minPos.x, minPos.y, minPos.z), color);

	Line(D3DXVECTOR3(minPos.x, maxPos.y, minPos.z), D3DXVECTOR3(maxPos.x, maxPos.y, minPos.z), color);
	Line(D3DXVECTOR3(maxPos.x, maxPos.y, minPos.z), D3DXVECTOR3(maxPos.x, maxPos.y, maxPos.z), color);
	Line(D3DXVECTOR3(maxPos.x, maxPos.y, maxPos.z), D3DXVECTOR3(minPos.x, maxPos.y, maxPos.z), color);
	Line(D3DXVECTOR3(minPos.x, maxPos.y, maxPos.z), D3DXVECTOR3(minPos.x, maxPos.y, minPos.z), color);

	Line(D3DXVECTOR3(minPos.x, minPos.y, minPos.z), D3DXVECTOR3(minPos.x, maxPos.y, minPos.z), color);
	Line(D3DXVECTOR3(maxPos.x, minPos.y, minPos.z), D3DXVECTOR3(maxPos.x, maxPos.y, minPos.z), color);
	Line(D3DXVECTOR3(maxPos.x, minPos.y, maxPos.z), D3DXVECTOR3(maxPos.x, maxPos.y, maxPos.z), color);
	Line(D3DXVECTOR3(minPos.x, minPos.y, maxPos.z), D3DXVECTOR3(minPos.x, maxPos.y, maxPos.z), color);
}

void Gizmo::AABBBox(D3DXVECTOR3 & minPos, D3DXVECTOR3 & maxPos, D3DXCOLOR color)
{
	aabbLine->Clear();
	aabbLine->AddLine(D3DXVECTOR3(minPos.x, minPos.y, minPos.z), D3DXVECTOR3(maxPos.x, minPos.y, minPos.z), color);
	aabbLine->AddLine(D3DXVECTOR3(maxPos.x, minPos.y, minPos.z), D3DXVECTOR3(maxPos.x, minPos.y, maxPos.z), color);
	aabbLine->AddLine(D3DXVECTOR3(maxPos.x, minPos.y, maxPos.z), D3DXVECTOR3(minPos.x, minPos.y, maxPos.z), color);
	aabbLine->AddLine(D3DXVECTOR3(minPos.x, minPos.y, maxPos.z), D3DXVECTOR3(minPos.x, minPos.y, minPos.z), color);

	aabbLine->AddLine(D3DXVECTOR3(minPos.x, maxPos.y, minPos.z), D3DXVECTOR3(maxPos.x, maxPos.y, minPos.z), color);
	aabbLine->AddLine(D3DXVECTOR3(maxPos.x, maxPos.y, minPos.z), D3DXVECTOR3(maxPos.x, maxPos.y, maxPos.z), color);
	aabbLine->AddLine(D3DXVECTOR3(maxPos.x, maxPos.y, maxPos.z), D3DXVECTOR3(minPos.x, maxPos.y, maxPos.z), color);
	aabbLine->AddLine(D3DXVECTOR3(minPos.x, maxPos.y, maxPos.z), D3DXVECTOR3(minPos.x, maxPos.y, minPos.z), color);

	aabbLine->AddLine(D3DXVECTOR3(minPos.x, minPos.y, minPos.z), D3DXVECTOR3(minPos.x, maxPos.y, minPos.z), color);
	aabbLine->AddLine(D3DXVECTOR3(maxPos.x, minPos.y, minPos.z), D3DXVECTOR3(maxPos.x, maxPos.y, minPos.z), color);
	aabbLine->AddLine(D3DXVECTOR3(maxPos.x, minPos.y, maxPos.z), D3DXVECTOR3(maxPos.x, maxPos.y, maxPos.z), color);
	aabbLine->AddLine(D3DXVECTOR3(minPos.x, minPos.y, maxPos.z), D3DXVECTOR3(minPos.x, maxPos.y, maxPos.z), color);
}