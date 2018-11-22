// author : yusuke seki
// data   : 20181115
#include "Collider.h"
#include "Unit.h"
#include "renderer.h"
#include "collision.h"

Collider::Collider(Unit* _parentUnit) : Object(TYPE::COLLIDER)
{
	radius_ = 0.0f;

	parentUnit_ = _parentUnit;
	previewParentUnitPosition_ = D3DXVECTOR3(0, 0, 0);

#ifdef _DEBUG
	mesh_ = nullptr;
	material_ = {};
	worldMatrix_ = {};
	isDraw_ = false;
#endif
}

Collider::~Collider()
{
	Uninit();
}

Collider* Collider::Create(Unit* _parentUnit)
{
	Collider* collider = new Collider(_parentUnit);
	collider->Init();

	return collider;
}

void Collider::Release()
{
#ifdef _DEBUG
	if (mesh_ != nullptr)
	{
		mesh_->Release();
		mesh_ = nullptr;
	}
#endif

	Object::Release();
}

void Collider::Init()
{
	SetPosition(parentUnit_->GetPosition());
	radius_ = parentUnit_->GetRadius();

	previewParentUnitPosition_ = parentUnit_->GetPosition();

#ifdef _DEBUG
	CreateSphere();
	SetColor(1.0f, 1.0f, 1.0f, 0.6f);
	UpdateWorldMatrix();
	isDraw_ = true;
#endif
}

void Collider::Uninit()
{
	SetActive(false);
}

void Collider::Update()
{
	if (IsMovePositionParentUnit() == true)
	{
		ChaseParentUnit();
	}

	previewParentUnitPosition_ = parentUnit_->GetPosition();

#ifdef _DEBUG
	UpdateWorldMatrix();
#endif
}

void Collider::Draw()
{
#ifdef _DEBUG
	if (isDraw_ == false)
	{
		return;
	}

	LPDIRECT3DDEVICE9 device = Renderer::GetDevice();
	D3DMATERIAL9 defaultMaterial;

	device->SetTransform(D3DTS_WORLD, &worldMatrix_);

	device->GetMaterial(&defaultMaterial);

	device->SetMaterial(&material_);

	mesh_->DrawSubset(0);

	device->SetMaterial(&defaultMaterial);
#endif
}

void Collider::SetRadius(const float& _radius)
{
	radius_ = _radius;

#ifdef _DEBUG
	CreateSphere();
#endif
}

float Collider::GetRadius()
{
	return radius_;
}

void Collider::SetParentUnit(Unit* _parentUnit)
{
	parentUnit_ = _parentUnit;
	previewParentUnitPosition_ = parentUnit_->GetPosition();

	SetPosition(parentUnit_->GetPosition());
	radius_ = parentUnit_->GetRadius();
}

bool Collider::Collision(Collider* _targetCollider)
{
	float distance = Distance3D(GetPosition(), _targetCollider->GetPosition());
	float length = (radius_ + _targetCollider->GetRadius()) * (radius_ + _targetCollider->GetRadius());

	if (distance <= length)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Collider::Collision(const D3DXVECTOR3& _point)
{
	float distance = Distance3D(GetPosition(), _point);
	float length = radius_ * radius_;

	if (distance <= length)
	{
		return true;
	}
	else
	{
		return false;
	}
}

#ifdef _DEBUG
void Collider::SetColor(const float& _r, const float& _g, const float& _b, const float& _a)
{
	material_.Diffuse = D3DXCOLOR(_r, _g, _b, _a);
}

void Collider::SetDraw(bool _isDraw)
{
	isDraw_ = _isDraw;
}

#endif

bool Collider::IsMovePositionParentUnit()
{
	if (parentUnit_->GetPosition() != previewParentUnitPosition_)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Collider::ChaseParentUnit()
{
	D3DXVECTOR3 moveVector = parentUnit_->GetPosition() - previewParentUnitPosition_;
	Object::MovePosition(moveVector);
}

#ifdef _DEBUG
void Collider::UpdateWorldMatrix()
{
	D3DXMATRIX translateMatrix, rotateMatrix, scaleMatrix;
	D3DXVECTOR3 position = GetPosition();

	D3DXMatrixIdentity(&worldMatrix_);

	D3DXMatrixTranslation(&translateMatrix, position.x, position.y, position.z);

	D3DXMatrixRotationYawPitchRoll(&rotateMatrix, 0, 0, 0);

	D3DXMatrixScaling(&scaleMatrix, 1, 1, 1);

	D3DXMatrixMultiply(&worldMatrix_, &worldMatrix_, &scaleMatrix);

	D3DXMatrixMultiply(&worldMatrix_, &worldMatrix_, &rotateMatrix);

	D3DXMatrixMultiply(&worldMatrix_, &worldMatrix_, &translateMatrix);
}

void Collider::CreateSphere()
{
	LPDIRECT3DDEVICE9 device = Renderer::GetDevice();

	if (mesh_ != nullptr)
	{
		mesh_->Release();
		mesh_ = nullptr;
	}

	D3DXCreateSphere(device, radius_, 16, 8, &mesh_, nullptr);
}
#endif
