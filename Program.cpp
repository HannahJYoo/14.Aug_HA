#include "stdafx.h"
#include "Program.h"
#include "./Viewer/Freedom.h"
#include "./Viewer/OrbitCamera.h"

#include "./Executes/ExportModel.h"
#include "./Executes/ExeSphere.h"
#include "./Executes/ExportMesh.h"

#include "./Executes/DrawLandscape.h"

Program::Program()
{
	States::Create();

	D3DDesc desc;
	D3D::GetDesc(&desc);

	values = new ExecuteValues();
	values->ViewProjection = new ViewProjectionBuffer();
	values->GlobalLight = new LightBuffer();
	values->Perspective = new Perspective(desc.Width, desc.Height);
	values->Viewport = new Viewport(desc.Width, desc.Height);

	orbitCamera = new OrbitCamera();
	freeCamera = new Freedom(100.0f,2.5f);

	values->MainCamera = freeCamera;
	values->MainCamera->Position(0, 0, -100.0f);

	freeCamera->RotationDegree(33, 0);
	freeCamera->Position(137, 97, -100.0f);
	orbitCamera->Position(0, 0, -100.0f);

	executes.push_back(new DrawLandscape(values));
	//executes.push_back(new ExportModel(values));

	isOrbit = false;

}

Program::~Program()
{
	for (Execute* exe : executes)
		SAFE_DELETE(exe);

	/*SAFE_DELETE(freeCamera);
	SAFE_DELETE(orbitCamera);*/
	SAFE_DELETE(values->MainCamera);
	SAFE_DELETE(values->GlobalLight);
	SAFE_DELETE(values->ViewProjection);
	SAFE_DELETE(values->Perspective);
	SAFE_DELETE(values->Viewport);
	SAFE_DELETE(values);

	States::Delete();
}

void Program::Update()
{
	
	if (isOrbit == true)
		values->MainCamera = orbitCamera;
	else
		values->MainCamera = freeCamera;

	values->MainCamera->Update();

	for (Execute* exe : executes)
		exe->Update();
}

void Program::PreRender()
{

}

void Program::Render()
{
	D3DXMATRIX view, projection;
	values->MainCamera->Matrix(&view);
	values->Perspective->GetMatrix(&projection);

	values->ViewProjection->SetView(view);
	values->ViewProjection->SetProjection(projection);
	values->ViewProjection->SetVSBuffer(0);
	values->GlobalLight->SetPSBuffer(0);

	for (Execute* exe : executes)
		exe->Render();
}

void Program::PostRender()
{
	for (Execute* exe : executes)
		exe->PostRender();
	ImGui::Begin("Info");
	{
		ImGui::Text("FPS : %f", Time::Get()->FPS());
		ImGui::Separator();
		ImGui::Checkbox("Orbit_Sphere", &isOrbit);
		ImGui::Separator();

		ImGui::SliderFloat3("LightDirection", (float*)&values->GlobalLight->Data.Direction, -1, 1);
		ImGui::Separator();
		D3DXVECTOR3 vec;
		values->MainCamera->Position(&vec);
		ImGui::LabelText("CameraPos", "%.2f, %.2f, %.2f", vec.x, vec.y, vec.z);

		D3DXVECTOR2 rot;
		values->MainCamera->RotationDegree(&rot);
		ImGui::LabelText("CameraRot", "%.2f, %.2f, %.2f", rot.x, rot.y);

	}
	ImGui::End();
}

void Program::ResizeScreen()
{
	D3DDesc desc;
	D3D::GetDesc(&desc);

	values->Perspective->Set(desc.Width, desc.Height);
	values->Viewport->Set(desc.Width, desc.Height);

	for (Execute* exe : executes)
		exe->ResizeScreen();
}