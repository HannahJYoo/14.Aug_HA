#pragma once
#include "Execute.h"

class DrawNormalLine;
class ExportMesh : public Execute
{

public:
	ExportMesh(ExecuteValues* values);
	~ExportMesh();

	// Execute을(를) 통해 상속됨
	 void Update() ;
	 void PreRender() ;
	 void Render() ;
	 void PostRender() ;
	 void ResizeScreen() ;


private:
	vector<class GameModel*> models;
};