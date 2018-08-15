#include "stdafx.h"
#include "ExportModel.h"
#include "../FBX/Exporter.h"
#include "../Objects/GameModel.h"
#include "../Objects/GameTank.h"
ExportModel::ExportModel(ExecuteValues * values)
	:Execute(values)
{
	//Fbx::Exporter* exporter = NULL;


	tank = new GameTank();

	/*D3DXMATRIX R;
	D3DXMatrixRotationY(&R, Math::ToRadian(90));

	tank->RootAxis(R);*/
}

ExportModel::~ExportModel()
{
	SAFE_DELETE(tank);
}

void ExportModel::Update()
{


	tank->Update();
}

void ExportModel::PreRender()
{

}

void ExportModel::Render()
{
	tank->Render();
}

void ExportModel::PostRender()
{

}

void ExportModel::ResizeScreen()
{
}
