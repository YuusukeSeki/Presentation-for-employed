// author : yusuke seki
// data   : 20181115
#include "Unit.h"
#include "collision.h"
#include "Part.h"
#include "Collider.h"

Unit::Unit(const Object::TYPE& _type) : Object(_type)
{
	objectParts_.clear();

	worldMatrix_ = {};
	rotate_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	radius_ = 0.0f;
	color_.color = D3DCOLOR_RGBA(255, 255, 255, 255);
	front_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	isUpdateWorldMatrix_ = false;

	objectCollider_ = Collider::Create(this);
}

Unit::~Unit()
{

}

void Unit::Release()
{
	objectParts_.clear();

	Object::Release();
}

void Unit::ReleaseParts()
{
	for each (auto part in objectParts_)
	{
		part.second->Release();
	}

	objectParts_.clear();
}

void Unit::Init(const D3DXVECTOR3& _position, const Object::GROUP& _group)
{
	SetPosition(_position);

	SetGroup(_group);

	rotate_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale_ = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	color_.color = D3DCOLOR_RGBA(255, 255, 255, 255);
	front_ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	ResetRadius();

	UpdateWorldMatrix();

	if (objectCollider_ == nullptr)
	{
		objectCollider_ = Collider::Create(this);
	}
	else
	{
		objectCollider_->SetParentUnit(this);
	}

#ifdef _DEBUG
	objectCollider_->SetColor(1.0f, 1.0f, 1.0f, 0.75f);
#endif
}

void Unit::Uninit()
{
	SetActive(false);
}

void Unit::Draw(void)
{
	if (isUpdateWorldMatrix_ == true)
	{
		UpdateWorldMatrix();

		for each (auto part in objectParts_)
		{
			part.second->SetUpdateWorldMatrix(true);
		}

		isUpdateWorldMatrix_ = false;
	}
}

Part* Unit::AddPart(const std::string& _key, const std::string& _fileName)
{
	float halfSize;
	Part* part = Part::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), _fileName, &worldMatrix_);

	part->SetFront(front_);
	part->SetColor(color_.color);
	part->SetGroup(GetGroup());
	part->SetActive(GetActive());

	objectParts_.emplace(_key, part);

	ResetRadius();

	halfSize = GetHalfSize().x < GetHalfSize().z ? GetHalfSize().x : GetHalfSize().z;

	objectCollider_->SetRadius(halfSize);

	return part;
}

void Unit::SetPosition(const D3DXVECTOR3& _position)
{
	Object::SetPosition(_position);

	isUpdateWorldMatrix_ = true;
}

void Unit::SetPosition(const D3DXVECTOR3& _position, const D3DXVECTOR3& _front)
{
	Object::SetPosition(_position);

	front_ = _front;

	isUpdateWorldMatrix_ = true;
}

void Unit::MovePosition(const D3DXVECTOR3& _move)
{
	Object::MovePosition(_move);

	isUpdateWorldMatrix_ = true;
}

void Unit::MovePosition(const D3DXVECTOR3& _moveVector, const float& _speed)
{
	Object::MovePosition(_moveVector * _speed);

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

Unit::Color Unit::GetColor()
{
	return color_;
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
		part.second->SetActive(_isActive);
	}

	objectCollider_->SetActive(_isActive);
}

Collider* Unit::GetObjectCollider()
{
	return objectCollider_;
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

std::map<std::string, Part*>* Unit::GetParts()
{
	return &objectParts_;
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
			minPosition = part.second->GetMinVertex();
			maxPosition = part.second->GetMaxVertex();

			first = false;
		}
		else
		{
			minPosition.x = part.second->GetMinVertex().x < minPosition.x ? part.second->GetMinVertex().x : minPosition.x;
			minPosition.y = part.second->GetMinVertex().y < minPosition.y ? part.second->GetMinVertex().y : minPosition.y;
			minPosition.z = part.second->GetMinVertex().z < minPosition.z ? part.second->GetMinVertex().z : minPosition.z;

			maxPosition.x = part.second->GetMaxVertex().x > maxPosition.x ? part.second->GetMaxVertex().x : maxPosition.x;
			maxPosition.y = part.second->GetMaxVertex().y > maxPosition.y ? part.second->GetMaxVertex().y : maxPosition.y;
			maxPosition.z = part.second->GetMaxVertex().z > maxPosition.z ? part.second->GetMaxVertex().z : maxPosition.z;
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

	for each (auto part in objectParts_)
	{
		part.second->SetUpdateWorldMatrix(true);
	}
}
