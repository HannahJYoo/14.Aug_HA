#pragma once
#include "Execute.h"

class DrawNormalLine;
class ExportMesh : public Execute
{

public:
	ExportMesh(ExecuteValues* values);
	~ExportMesh();

	// Execute��(��) ���� ��ӵ�
	 void Update() ;
	 void PreRender() ;
	 void Render() ;
	 void PostRender() ;
	 void ResizeScreen() ;


private:
	vector<class GameModel*> models;
};