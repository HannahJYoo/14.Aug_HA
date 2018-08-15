#pragma once
#include "Execute.h"

class DrawNormalLine;
class ExportModel : public Execute
{

public:
	ExportModel(ExecuteValues* values);
	~ExportModel();

	// Execute��(��) ���� ��ӵ�
	void Update();
	void PreRender();
	void Render();
	void PostRender();
	void ResizeScreen();


private:
	class GameTank* tank;

};