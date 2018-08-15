#include "stdafx.h"
#include "Model.h"
#include "ModelBone.h"
#include "ModelMesh.h"
#include "ModelMeshPart.h"

Model::Model()
{
	buffer = new BoneBuffer();
}

Model::~Model()
{
	SAFE_DELETE(buffer);

	for (Material* material : materials)
		SAFE_DELETE(material);
	
	for (ModelBone* bone : bones)
		SAFE_DELETE(bone);

	for (ModelMesh* mesh : meshes)
		SAFE_DELETE(mesh);

}

 Material * Model::MaterialByName(wstring name) {
	for (Material* material : materials) {
		if (material->Name() == name)
			return material;
	}
	return NULL;
}

ModelBone * Model::BoneByName(wstring name) {
	for (ModelBone* bone : bones) {
		if (bone->Name() == name)
			return bone;
	}
	return NULL;

}

 ModelMesh * Model::MeshByName(wstring name) {
	for (ModelMesh* mesh : meshes) {
		if (mesh->Name() == name)
			return mesh;
	}
	return NULL;

}

 void Model::BoundingBox(D3DXVECTOR3 * boundingBox)
 {
	 D3DXVECTOR3 min, max;
	 min = { 10000, 10000, 10000 };
	 max = { -10000, -10000, -10000 };

	 D3DXVECTOR3 result[8];

	 for (ModelMesh* mesh : meshes)
	 {
		 ModelBone* bone = mesh->ParentBone();
		 D3DXMATRIX boneWorld = bone->Global();

		 for (ModelMeshPart* part : mesh->meshParts)
		 {
			 for (ModelVertexType vertex : part->vertices)
			 {
				 D3DXVECTOR3 position = vertex.Position;
				 D3DXVec3TransformCoord(&position, &position, &boneWorld);

				 min.x = (min.x < position.x) ? min.x : position.x;
				 min.y = (min.y < position.y) ? min.y : position.y;
				 min.z = (min.z < position.z) ? min.z : position.z;
				 max.x = (max.x > position.x) ? max.x : position.x;
				 max.y = (max.y > position.y) ? max.y : position.y;
				 max.z = (max.z > position.z) ? max.z : position.z;
			 }
		 }
	 }

	 boundingBox[0] = D3DXVECTOR3(min.x, max.y, min.z);
	 boundingBox[1] = D3DXVECTOR3(min.x, max.y, max.z);
	 boundingBox[2] = D3DXVECTOR3(max.x, max.y, max.z);
	 boundingBox[3] = D3DXVECTOR3(max.x, max.y, min.z);
	 boundingBox[4] = D3DXVECTOR3(min.x, min.y, min.z);
	 boundingBox[5] = D3DXVECTOR3(min.x, min.y, max.z);
	 boundingBox[6] = D3DXVECTOR3(max.x, min.y, max.z);
	 boundingBox[7] = D3DXVECTOR3(max.x, min.y, min.z);
 }

void Model::CopyGlobalBoneTo(vector<D3DXMATRIX>& transforms)
{
	D3DXMATRIX w;
	D3DXMatrixIdentity(&w);

	CopyGlobalBoneTo(transforms, w);
}

void Model::CopyGlobalBoneTo(vector<D3DXMATRIX>& transforms, D3DXMATRIX& w)
{
	transforms.clear();
	transforms.assign(bones.size(), D3DXMATRIX());

	for (size_t i = 0; i < bones.size(); i++) {	//size_t = UINT
		ModelBone* bone = bones[i];

		if (bone->Parent() != NULL) {
			int index = bone->parent->index;
			transforms[i] = bone->local* transforms[index];
		}
		else {
			transforms[i] = bone->local*w;
		}
	}//for(i)

}

void Models::Create()
{
}

void Models::Delete()
{
	for (pair<wstring, vector<Material*>> temp : materialMap) {
		for (Material* material : temp.second)
			SAFE_DELETE(material);
	}

	for (pair<wstring, MeshData> temp : meshDataMap)
	{
		MeshData data = temp.second;

		for (ModelBone* bone : data.Bones)
			SAFE_DELETE(bone);

		for (ModelMesh* mesh : data.Meshes)
			SAFE_DELETE(mesh);
	}

}
