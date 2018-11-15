// author : yusuke seki
// data   : 20181115
#include "Unit.h"
#include "collision.h"

Unit::Unit(const Object::TYPE& _type)
{
	objectParts_.clear();

	worldMatrix_ = {};
	rotate_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	radius_ = 0.0f;
	color_.color = D3DCOLOR_RGBA(255, 255, 255, 255);
	front_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

Unit::~Unit()
{
	Uninit();
}

void Unit::Release()
{
	for each (auto part in objectParts_)
	{
		if (part.second != nullptr)
		{
			part.second->Release();
			part.second = nullptr;
		}
	}

	objectParts_.clear();
}

void Unit::Init(const D3DXVECTOR3& _position)
{
	SetPosition(_position);

	rotate_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale_ = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	color_.color = D3DCOLOR_RGBA(255, 255, 255, 255);
	front_ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	ResetRadius();

	UpdateWorldMatrix();
}

void Unit::Uninit(void)
{

}

void Unit::Update(void)
{

}

void Unit::Draw(void)
{
	if (isUpdateWorldMatrix_ == true)
	{
		UpdateWorldMatrix();

		isUpdateWorldMatrix_ = false;
	}
}

void Unit::SetPosition(const D3DXVECTOR3& _position)
{
	Object::SetPosition(_position);

	isUpdateWorldMatrix_ = true;
}

void Unit::MovePosition(const D3DXVECTOR3& _move)
{
	Object::MovePosition(_move);

	isUpdateWorldMatrix_ = true;
}

void Unit::SetRotate(const D3DXVECTOR3& _rotate)
{
	float angle = D3DXVec3Dot(&rotate_, &_rotate);
	float rotateDirect = _rotate.x * rotate_.z - _rotate.z * rotate_.x;

	rotate_ = _rotate;

	isUpdateWorldMatrix_ = true;
}

void Unit::MoveRotate(const D3DXVECTOR3& _move)
{
	rotate_ += _move;

	isUpdateWorldMatrix_ = true;
}

D3DXVECTOR3 Unit::GetRotate()
{
	return rotate_;
}

void Unit::SetScale(const D3DXVECTOR3& _scale)
{
	scale_ = _scale;

	isUpdateWorldMatrix_ = true;
}

void Unit::MoveScale(const D3DXVECTOR3& _move)
{
	scale_ += _move;

	isUpdateWorldMatrix_ = true;
}

D3DXVECTOR3 Unit::GetScale()
{
	return scale_;
}

D3DXVECTOR3 Unit::GetSize()
{
	return halfSize_ * 2;
}

D3DXVECTOR3 Unit::GetHalfSize()
{
	return halfSize_;
}

float Unit::GetRadius()
{
	return radius_;
}

void Unit::SetFront(const D3DXVECTOR3& _front)
{
	front_ = _front;

	for each (auto part in objectParts_)
	{
		part.second->SetFront(front_);
	}
}

D3DXVECTOR3 Unit::GetFront()
{
	return front_;
}

void Unit::SetColor(const unsigned int& _rgba)
{
	color_.color = _rgba;

	for each (auto part in objectParts_)
	{
		part.second->SetColor(color_.color);
	}
}

void Unit::SetColor(const unsigned char& _r, const unsigned char& _g, const unsigned char& _b, const unsigned char& _a)
{
	color_.rgba[3] = _r;
	color_.rgba[2] = _g;
	color_.rgba[1] = _b;
	color_.rgba[0] = _a;

	for each (auto part in objectParts_)
	{
		part.second->SetColor(color_.color);
	}
}

void Unit::MoveColor(const int& _r, const int& _g, const int& _b, const int& _a)
{
	color_.rgba[3] = color_.rgba[3] + _r > 255 ? 255 : color_.rgba[3] + _r;
	color_.rgba[2] = color_.rgba[2] + _g > 255 ? 255 : color_.rgba[2] + _g;
	color_.rgba[1] = color_.rgba[1] + _b > 255 ? 255 : color_.rgba[1] + _b;
	color_.rgba[0] = color_.rgba[0] + _a > 255 ? 255 : color_.rgba[0] + _a;
	color_.rgba[3] = color_.rgba[3] + _r <   0 ? 0 : color_.rgba[3] + _r;
	color_.rgba[2] = color_.rgba[2] + _g <   0 ? 0 : color_.rgba[2] + _g;
	color_.rgba[1] = color_.rgba[1] + _b <   0 ? 0 : color_.rgba[1] + _b;
	color_.rgba[0] = color_.rgba[0] + _a <   0 ? 0 : color_.rgba[0] + _a;

	for each (auto part in objectParts_)
	{
		part.second->SetColor(color_.color);
	}
}

void Unit::SetRotateToPosition(const D3DXVECTOR3& _position)
{
	float angle;
	D3DXVECTOR3 nextFront = _position - GetPosition();

	//// ¡‰ñ‚Í2DƒQ[ƒ€‚È‚Ì‚ÅYÀ•W‚Í‚O‚Å“ˆê
	//targetPosition.y = 0.f;

	D3DXVec3Normalize(&nextFront, &nextFront);

	front_ = nextFront;

	angle = atan2f(front_.x, front_.z);

	SetRotate(D3DXVECTOR3(0, angle, 0));
}

D3DXVECTOR3 Unit::GetRotateToPosition(const D3DXVECTOR3& _position)
{
	float angle;
	D3DXVECTOR3 nextFront = _position - GetPosition();

	D3DXVec3Normalize(&nextFront, &nextFront);

	angle = atan2f(nextFront.x, nextFront.z);

	return D3DXVECTOR3(0, angle, 0);
}

void Unit::SetGroup(const Object::GROUP& _group)
{
	Object::SetGroup(_group);

	for each (auto part in objectParts_)
	{
		part.second->SetGroup(GetGroup());
	}
}

void Unit::SetActive(const bool& _isActive)
{
	Object::SetActive(_isActive);

	for each (auto part in objectParts_)
	{
		part.second->SetActive(GetActive());
	}
}

void Unit::AddPart(const std::string& _key, const std::string& _fileName)
{
	Part* part = Part::Create(GetPosition(), _fileName, &worldMatrix_);

	objectParts_.emplace(_key, part);
}

void Unit::ErasePart(const std::string& _key)
{
	objectParts_.erase(_key);
}

void Unit::SetPartPosition(const std::string& _key, const D3DXVECTOR3& _position)
{
	objectParts_[_key]->SetPosition(_position);
}

void Unit::SetPartRotate(const std::string& _key, const D3DXVECTOR3& _rotate)
{
	objectParts_[_key]->SetPosition(_rotate);
}

void Unit::SetPartScale(const std::string& _key, const D3DXVECTOR3& _scale)
{
	objectParts_[_key]->SetPosition(_scale);
}

void Unit::SetParent(const std::string& _childKey, const std::string& _parentKey)
{
	objectParts_[_childKey]->SetParent(objectParts_[_parentKey]);
}

Part* Unit::GetPart(const std::string& _key)
{
	return objectParts_[_key];
}

void Unit::ResetRadius()
{
	float radius = 0.0f;
	D3DXVECTOR3 minPosition(0, 0, 0);
	D3DXVECTOR3 maxPosition(0, 0, 0);
	bool first = true;

	for each (auto part in objectParts_)
	{
		if (first == true)
		{
			minPosition = part.second->GetPosition();
			maxPosition = part.second->GetPosition();

			first = false;
		}
		else
		{
			minPosition.x = part.second->GetPosition().x < minPosition.x ? part.second->GetPosition().x : minPosition.x;
			minPosition.y = part.second->GetPosition().y < minPosition.y ? part.second->GetPosition().y : minPosition.y;
			minPosition.z = part.second->GetPosition().z < minPosition.z ? part.second->GetPosition().z : minPosition.z;

			maxPosition.x = part.second->GetPosition().x > maxPosition.x ? part.second->GetPosition().x : maxPosition.x;
			maxPosition.y = part.second->GetPosition().y > maxPosition.y ? part.second->GetPosition().y : maxPosition.y;
			maxPosition.z = part.second->GetPosition().z > maxPosition.z ? part.second->GetPosition().z : maxPosition.z;
		}
	}

	halfSize_ = (maxPosition - minPosition) * 0.5f;
	radius_ = sqrtf(halfSize_.x * halfSize_.x + halfSize_.y * halfSize_.y + halfSize_.z * halfSize_.z);
}

void Unit::UpdateWorldMatrix()
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
