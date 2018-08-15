#pragma once

class Zeichnen
{
public:
	Zeichnen();
	~Zeichnen();

	void Init();
	void Release();
	void Update(D3DXMATRIX world);
	void Render();

	void CreateBuffer();
	void AddLine(D3DXVECTOR3& start, D3DXVECTOR3& end, D3DXCOLOR color = D3DXCOLOR(0, 0, 0, 1));

	void Clear();
private:
	struct LineData
	{
		D3DXVECTOR3 Start;
		D3DXVECTOR3 End;
		D3DXCOLOR Color;
	};
	vector<LineData *> vecLine;
	vector<LineData *>::iterator iter;

	UINT count;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	VertexColor* vertices;
	UINT* indices;

	Shader* shader;
	WorldBuffer* worldBuffer;

	bool bBuffer;
};