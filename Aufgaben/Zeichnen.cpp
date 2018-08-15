#include "stdafx.h"
#include "Zeichnen.h"

Zeichnen::Zeichnen()
	: bBuffer(false), count(0)
	, vertices(NULL), indices(NULL), vertexBuffer(NULL), indexBuffer(NULL)
{
	Init();
}

Zeichnen::~Zeichnen()
{
}

void Zeichnen::Init()
{
	shader = new Shader(Shaders + L"003_Color.hlsl");
	worldBuffer = new WorldBuffer();
}

void Zeichnen::Release()
{
	SAFE_DELETE(worldBuffer);
	if (vertices != NULL)
	{
		SAFE_DELETE_ARRAY(vertices);
		SAFE_DELETE_ARRAY(indices);
	}

	if (indexBuffer != NULL)
	{
		SAFE_RELEASE(indexBuffer);
		SAFE_RELEASE(vertexBuffer);
	}

	for (LineData* vec : vecLine)
		SAFE_DELETE(vec);
}

void Zeichnen::Update(D3DXMATRIX world)
{
	worldBuffer->SetMatrix(world);

	if (bBuffer)
	{
		CreateBuffer();
	}
}

void Zeichnen::Render()
{
	if (count > 0)
	{
		UINT stride = sizeof(VertexColor);
		UINT offset = 0;

		D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		worldBuffer->SetVSBuffer(1);

		shader->Render();

		D3D::GetDC()->DrawIndexed(count, 0, 0);
	}
}

void Zeichnen::CreateBuffer()
{
	if (vertices != NULL)
	{
		SAFE_DELETE_ARRAY(vertices);
		SAFE_DELETE_ARRAY(indices);
	}

	vertices = new VertexColor[count];
	indices = new UINT[count];

	for (UINT i = 0; i < vecLine.size(); i++)
	{
		vertices[i * 2 + 0].Position = vecLine[i]->Start;
		vertices[i * 2 + 1].Position = vecLine[i]->End;

		vertices[i * 2 + 0].Color = vecLine[i]->Color;
		vertices[i * 2 + 1].Color = vecLine[i]->Color;

		indices[i * 2 + 0] = i * 2 + 0;
		indices[i * 2 + 1] = i * 2 + 1;
	}

	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexColor) * count;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	//Create Index Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * count;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = indices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
		assert(SUCCEEDED(hr));
	}
	bBuffer = false;
}

void Zeichnen::AddLine(D3DXVECTOR3& start, D3DXVECTOR3& end, D3DXCOLOR color)
{
	LineData* temp = new LineData();
	temp->Start = start;
	temp->End = end;
	temp->Color = color;

	vecLine.push_back(temp);
	count = vecLine.size() * 2;
	bBuffer = true;
}

void Zeichnen::Clear()
{
	for (LineData* vec : vecLine)
		SAFE_DELETE(vec);
	vecLine.clear();
	count = 0;
}
