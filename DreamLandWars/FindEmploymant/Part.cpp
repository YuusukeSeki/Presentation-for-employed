// author : yusuke seki
// data   : 20181115
#include "Part.h"

Part::Part() : ObjectModel(Object::TYPE::TYPE_MODEL)
{
	parentPart_ = nullptr;
}

Part::Part(const Object::TYPE& _type) : ObjectModel(_type)
{
	parentPart_ = nullptr;
}

Part::~Part()
{
	Uninit();
}

Part* Part::Create(const D3DXVECTOR3& _position, const std::string& _fileName, D3DXMATRIX* _parentObjectWorldMatrix)
{
	Part* part = new Part(Object::TYPE::TYPE_MODEL);
	part->Init(_position, _fileName, _parentObjectWorldMatrix);

	return part;
}

void Part::Init(const D3DXVECTOR3& _position, const std::string& _fileName, D3DXMATRIX* _parentObjectWorldMatrix)
{
	ObjectModel::Init(_position, _fileName);
	parentObjectWorldMatrix_ = _parentObjectWorldMatrix;
}

void Part::Uninit(void)
{
	ObjectModel::Uninit();
}

void Part::Update(void)
{
	ObjectModel::Update();
}

void Part::Draw(void)
{
	ObjectModel::Draw();
}

void Part::SetParent(Part* _parentPart)
{
	parentPart_ = _parentPart;
	SetUpdateWorldMatrix(true);
}

Part* Part::GetParent()
{
	return parentPart_;
}

void Part::UpdateWorldMatrix()
{
	D3DXMATRIX worldMatrix, translateMatrix, rotateMatrix, scaleMatrix;
	D3DXVECTOR3 position = GetPosition();
	D3DXVECTOR3 rotate = GetRotate();
	D3DXVECTOR3 scale = GetScale();

	D3DXMatrixIdentity(&worldMatrix);

	D3DXMatrixTranslation(&translateMatrix, position.x, position.y, position.z);

	D3DXMatrixRotationYawPitchRoll(&rotateMatrix, rotate.y, rotate.x, rotate.z);

	D3DXMatrixScaling(&scaleMatrix, scale.x, scale.y, scale.z);

	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &scaleMatrix);

	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &rotateMatrix);

	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

	if (parentPart_ != nullptr)
	{
		D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &parentPart_->GetWorldMatrix());
	}
	else
	{
		D3DXMatrixMultiply(&worldMatrix, &worldMatrix, parentObjectWorldMatrix_);
	}

	SetWorldMatrix(worldMatrix);
}

