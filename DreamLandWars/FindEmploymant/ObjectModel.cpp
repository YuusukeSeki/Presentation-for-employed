// author : yusuke seki
// data   : 20181110
#include "ObjectModel.h"
#include "renderer.h"

ObjectModel::ObjectModel() : Object(Object::TYPE::TYPE_MODEL)
{
	mesh_ = nullptr;

	material_ = nullptr;

	numMaterial_ = 0;

	texture_ = nullptr;

	worldMatrix_ = {};

	rotate_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	scale_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	halfSize_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	radius_ = 0.0f;

	color_.color = 0;

	front_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	isUpdateVertexBuf_ = false;

	isUpdateWorldMatrix_ = false;

	isDraw_ = false;
}

ObjectModel::ObjectModel(const Object::TYPE& type) : Object(type)
{
	mesh_ = nullptr;

	material_ = nullptr;

	numMaterial_ = 0;

	texture_ = nullptr;

	worldMatrix_ = {};

	rotate_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	scale_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	halfSize_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	radius_ = 0.0f;

	color_.color = 0;

	front_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	isUpdateVertexBuf_ = false;

	isUpdateWorldMatrix_ = false;

	isDraw_ = false;
}

ObjectModel::~ObjectModel()
{
	Uninit();
}

ObjectModel* ObjectModel::Create(const D3DXVECTOR3& _position, const std::string& _fileName)
{
	ObjectModel* objectModel = new ObjectModel(Object::TYPE::TYPE_MODEL);
	objectModel->Init(_position, _fileName);

	return objectModel;
}

void ObjectModel::Init(const D3DXVECTOR3& _position, const std::string& _fileName)
{
	LoadMeshModel_DX(_fileName.c_str());

	LoadModelSizeFromX(_fileName.c_str());

	SetPosition(_position);

	SetRotate(D3DXVECTOR3(0, 0, 0));

	SetScale(D3DXVECTOR3(1, 1, 1));

	SetFront(D3DXVECTOR3(0, 0, 1));

	SetColor(255, 255, 255, 255);

	SetIsDraw(true);
}

void ObjectModel::Uninit(void)
{
	for (int i = 0; i < (int)numMaterial_; ++i)
	{
		if (texture_[i] != nullptr)
		{
			texture_[i]->Release();
			texture_[i] = nullptr;
		}
	}

	if (material_ != nullptr)
	{
		delete[] material_;
		material_ = nullptr;
	}

	if (mesh_ != nullptr)
	{
		mesh_->Release();
		mesh_ = nullptr;
	}
}

void ObjectModel::Update(void)
{
}

void ObjectModel::Draw(void)
{
	if (isDraw_ == false)
	{
		return;
	}

	LPDIRECT3DDEVICE9 device = Renderer::GetDevice();
	D3DMATERIAL9 defaultMaterial;

	if (isUpdateVertexBuf_ == true)
	{
		UpdateVertexBuf();

		isUpdateVertexBuf_ = false;
	}

	if (isUpdateWorldMatrix_ == true)
	{
		UpdateWorldMatrix();

		isUpdateWorldMatrix_ = false;
	}

	device->SetTransform(D3DTS_WORLD, &worldMatrix_);

	device->GetMaterial(&defaultMaterial);

	for (int i = 0; i<(int)numMaterial_; i++)
	{
		device->SetMaterial(&material_[i]);

		device->SetTexture(0, texture_[i]);

		mesh_->DrawSubset(i);
	}

	device->SetMaterial(&defaultMaterial);

}

D3DXMATRIX ObjectModel::GetWorldMatrix()
{
	return worldMatrix_;
}

void ObjectModel::SetPosition(const D3DXVECTOR3& _position)
{
	Object::SetPosition(_position);

	isUpdateWorldMatrix_ = true;
}

void ObjectModel::MovePosition(const D3DXVECTOR3& _move)
{
	Object::MovePosition(_move);

	isUpdateWorldMatrix_ = true;
}

void ObjectModel::SetRotate(const D3DXVECTOR3& _rotate)
{
	float angle = D3DXVec3Dot(&rotate_, &_rotate);
	float rotateDirect = _rotate.x * rotate_.z - _rotate.z * rotate_.x;

	rotate_ = _rotate;

	isUpdateWorldMatrix_ = true;
}

void ObjectModel::MoveRotate(const D3DXVECTOR3& _move)
{
	rotate_ += _move;

	isUpdateWorldMatrix_ = true;
}

D3DXVECTOR3 ObjectModel::GetRotate()
{
	return rotate_;
}

void ObjectModel::SetScale(const D3DXVECTOR3& _scale)
{
	scale_ = _scale;

	isUpdateWorldMatrix_ = true;
}

void ObjectModel::MoveScale(const D3DXVECTOR3& _move)
{
	scale_ += _move;

	isUpdateWorldMatrix_ = true;
}

D3DXVECTOR3 ObjectModel::GetScale()
{
	return scale_;
}

D3DXVECTOR3 ObjectModel::GetSize()
{
	return halfSize_ * 2;
}

D3DXVECTOR3 ObjectModel::GetHalfSize()
{
	return halfSize_;
}

float ObjectModel::GetRadius()
{
	return radius_;
}

void ObjectModel::SetFront(const D3DXVECTOR3& _front)
{
	front_ = _front;
}

D3DXVECTOR3 ObjectModel::GetFront()
{
	return front_;
}

void ObjectModel::SetColor(const unsigned int& _rgba)
{
	color_.color = _rgba;

	isUpdateVertexBuf_ = true;
}

void ObjectModel::SetColor(const unsigned char& _r, const unsigned char& _g, const unsigned char& _b, const unsigned char& _a)
{
	color_.rgba[3] = _r;
	color_.rgba[2] = _g;
	color_.rgba[1] = _b;
	color_.rgba[0] = _a;

	isUpdateVertexBuf_ = true;
}

void ObjectModel::MoveColor(const int& _r, const int& _g, const int& _b, const int& _a)
{
	color_.rgba[3] = color_.rgba[3] + _r > 255 ? 255 : color_.rgba[3] + _r;
	color_.rgba[2] = color_.rgba[2] + _g > 255 ? 255 : color_.rgba[2] + _g;
	color_.rgba[1] = color_.rgba[1] + _b > 255 ? 255 : color_.rgba[1] + _b;
	color_.rgba[0] = color_.rgba[0] + _a > 255 ? 255 : color_.rgba[0] + _a;
	color_.rgba[3] = color_.rgba[3] + _r <   0 ?   0 : color_.rgba[3] + _r;
	color_.rgba[2] = color_.rgba[2] + _g <   0 ?   0 : color_.rgba[2] + _g;
	color_.rgba[1] = color_.rgba[1] + _b <   0 ?   0 : color_.rgba[1] + _b;
	color_.rgba[0] = color_.rgba[0] + _a <   0 ?   0 : color_.rgba[0] + _a;

	isUpdateVertexBuf_ = true;
}

void ObjectModel::SetRotateToPosition(const D3DXVECTOR3& _position)
{
	D3DXVECTOR3 nextFront = _position - GetPosition();

	//// 今回は2DゲームなのでY座標は０で統一
	//targetPosition.y = 0.f;

	D3DXVec3Normalize(&nextFront, &nextFront);

	front_ = nextFront;

	float angle = atan2f(front_.x, front_.z);
	
	SetRotate(D3DXVECTOR3(0, angle, 0));
}

D3DXVECTOR3 ObjectModel::GetRotateToPosition(const D3DXVECTOR3& _position)
{
	D3DXVECTOR3 nextFront = _position - GetPosition();

	D3DXVec3Normalize(&nextFront, &nextFront);

	float angle = atan2f(nextFront.x, nextFront.z);

	return D3DXVECTOR3(0, angle, 0);
}

void ObjectModel::SetTexture(const LPDIRECT3DTEXTURE9& _texture)
{
	*texture_ = _texture;
}

LPDIRECT3DTEXTURE9 ObjectModel::GetTexture()
{
	return *texture_;
}

void ObjectModel::SetIsDraw(const bool& _isDraw)
{
	isDraw_ = _isDraw;
}

void ObjectModel::SetWorldMatrix(const D3DXMATRIX& _worldMatrix)
{
	worldMatrix_ = _worldMatrix;
}

void ObjectModel::SetUpdateWorldMatrix(bool _isUpdate)
{
	isUpdateWorldMatrix_ = _isUpdate;
}

void ObjectModel::UpdateVertexBuf()
{
	VERTEX_3D* vertexBuf;
	int numVertex = (int)mesh_->GetNumVertices();

	mesh_->LockVertexBuffer(0, (void**)&vertexBuf);

	for (int i = 0; i < numVertex; i++)
	{
		vertexBuf[i].color = D3DCOLOR_RGBA(color_.rgba[3], color_.rgba[2], color_.rgba[1], color_.rgba[0]);
	}

	mesh_->UnlockVertexBuffer();
}

void ObjectModel::UpdateWorldMatrix()
{
	D3DXMATRIX translateMatrix, rotateMatrix, scaleMatrix;
	D3DXVECTOR3 position = GetPosition();

	D3DXMatrixIdentity(&worldMatrix_);

	D3DXMatrixTranslation(&translateMatrix, position.x, position.y, position.z);

	D3DXMatrixRotationYawPitchRoll(&rotateMatrix, rotate_.y, rotate_.x, rotate_.z);

	D3DXMatrixScaling(&scaleMatrix, scale_.x, scale_.y, scale_.z);

	D3DXMatrixMultiply(&worldMatrix_, &worldMatrix_, &scaleMatrix);

	D3DXMatrixMultiply(&worldMatrix_, &worldMatrix_, &rotateMatrix);

	D3DXMatrixMultiply(&worldMatrix_, &worldMatrix_, &translateMatrix);
}

// Xファイルからモデルデータのサイズを読み込む
void ObjectModel::LoadModelSizeFromX(const char* _fileName)
{
	// 変数の定義
	unsigned int positionNum = 0;
	D3DXVECTOR3 positionMax(0, 0, 0);
	D3DXVECTOR3 positionMin(0, 0, 0);
	char str[256];
	FILE* file;
	int cntHeader = 0;
	char* s;

	// ファイルを開く
	file = fopen(_fileName, "rt");

	// ファイルが無ければ終了
	if (file == nullptr) {
		_MSGERROR("Failed Load Model Size From X File!!", _fileName);
		return;
	}

	// 最大値と最小値の取得
	while (true) {
		fscanf(file, "%s", str);

		// 行がなければ要素数のカウントを終了する
		if (feof(file) != 0) break;

		// 頂点データの探知
		if (strcmp(str, "Header{") == 0) {
			cntHeader++;
		}

		// 頂点データの探知
		if (strcmp(str, "Mesh") == 0 && cntHeader == 1) {
			// 読み飛ばす
			fscanf(file, "%s", str);

			// 要素数の取得
			fscanf(file, "%d", &positionNum);

			// 読み飛ばす
			fscanf(file, "%s", str);

			// 頂点データの読込
			for (unsigned int i = 0; i < positionNum; i++) {
				fscanf(file, "%s", str);

				D3DXVECTOR3 buf;
				s = strtok(str, ";");
				buf.x = (float)atof(s);
				s = strtok(NULL, ";");
				buf.y = (float)atof(s);
				s = strtok(NULL, ";");
				buf.z = (float)atof(s);

				// 最大値の更新
				if (positionMax.x < buf.x) positionMax.x = buf.x;
				if (positionMax.y < buf.y) positionMax.y = buf.y;
				if (positionMax.z < buf.z) positionMax.z = buf.z;

				// 最小値の更新
				if (positionMin.x > buf.x) positionMin.x = buf.x;
				if (positionMin.y > buf.y) positionMin.y = buf.y;
				if (positionMin.z > buf.z) positionMin.z = buf.z;

			}

			break;

		}
	}

	fclose(file);

	halfSize_ = (positionMax - positionMin) * 0.5f;
	radius_   = sqrtf(halfSize_.x * halfSize_.x + halfSize_.y * halfSize_.y + halfSize_.z * halfSize_.z);

}

// モデルデータの読み込み（directXの便利関数使用）
void ObjectModel::LoadMeshModel_DX(const char* _fileName)
{
	LPDIRECT3DDEVICE9 device = Renderer::GetDevice();
	LPD3DXBUFFER materialBuf = nullptr;
	HRESULT hr;

	hr = D3DXLoadMeshFromXA(_fileName, D3DXMESH_MANAGED, device, nullptr, &materialBuf, nullptr, &numMaterial_, &mesh_);

	if (FAILED(hr))
	{
		_MSGERROR("Failed XFile Open!!", _fileName);
	}

	if (!(mesh_->GetFVF() & D3DFVF_NORMAL))
	{
		ID3DXMesh* tempMesh = nullptr;
		mesh_->CloneMeshFVF(mesh_->GetOptions(), mesh_->GetFVF() | D3DFVF_NORMAL, device, &tempMesh);
		D3DXComputeNormals(tempMesh, nullptr);
		mesh_->Release();
		mesh_ = tempMesh;
	}

	D3DXMATERIAL* material = (D3DXMATERIAL*)materialBuf->GetBufferPointer();
	material_ = new D3DMATERIAL9[numMaterial_];
	texture_ = new LPDIRECT3DTEXTURE9[numMaterial_];

	for (int i = 0; i<(int)numMaterial_; i++)
	{
		material_[i] = material[i].MatD3D;
		texture_[i] = nullptr;

		if (material[i].pTextureFilename != nullptr)
		{
			hr = D3DXCreateTextureFromFile(device, material[i].pTextureFilename, &texture_[i]);

			if (FAILED(hr))
			{
				_MSGERROR("Failed Load Texture!!", material[i].pTextureFilename);
			}
		}
	}

	if (materialBuf != nullptr)
	{
		materialBuf->Release();
	}
}
