// author : yusuke seki
// data   : 20181115
#include "Collider.h"
#include "Unit.h"
#include "renderer.h"
#include "collision.h"

Collider::Collider(Unit* _parentUnit)
{
	position_ = D3DXVECTOR3(0, 0, 0);
	radius_ = 0.0f;

	parentUnit_ = _parentUnit;
	previewParentUnitPosition_ = D3DXVECTOR3(0, 0, 0);

	isActive_ = false;

#ifdef _DEBUG
	mesh_ = nullptr;
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

	delete this;
}

void Collider::Init()
{
	position_ = parentUnit_->GetPosition();
	radius_ = parentUnit_->GetRadius();

	previewParentUnitPosition_ = parentUnit_->GetPosition();

	isActive_ = true;

#ifdef _DEBUG
	CreateSphere();
	UpdateWorldMatrix();
	UpdateVertexBuffer();
	isDraw_ = true;
#endif
}

void Collider::Uninit()
{

}

void Collider::Update()
{
	if (isActive_ == false)
	{
		return;
	}

	if (IsMovePositionParentUnit() == true)
	{
		ChaseParentUnit();
	}

	previewParentUnitPosition_ = parentUnit_->GetPosition();

#ifdef _DEBUG
	UpdateWorldMatrix();
#endif
}

void Collider::SetPosition(const D3DXVECTOR3& _position)
{
	position_ = _position;
}

D3DXVECTOR3 Collider::GetPosition()
{
	return position_;
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

	position_ = parentUnit_->GetPosition();
	radius_ = parentUnit_->GetRadius();
}

void Collider::SetActive(bool _isActive)
{
	isActive_ = _isActive;
}

bool Collider::Collision(Collider* _targetCollider)
{
	float distance = Distance3D(position_, _targetCollider->GetPosition());
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

#ifdef _DEBUG
void Collider::Draw()
{
	if (isDraw_ == false)
	{
		return;
	}

	LPDIRECT3DDEVICE9 device = Renderer::GetDevice();
	D3DMATERIAL9 defaultMaterial;

	device->SetTransform(D3DTS_WORLD, &worldMatrix_);

	device->GetMaterial(&defaultMaterial);

	mesh_->DrawSubset(0);

	device->SetMaterial(&defaultMaterial);
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
	float distance = Distance3D(parentUnit_->GetPosition(), previewParentUnitPosition_);

	D3DXVec3Normalize(&moveVector, &moveVector);
	sqrtf(distance);

	MovePosition(moveVector, distance);
}

void Collider::MovePosition(const D3DXVECTOR3& _moveVector, const float& _distance)
{
	position_ += _moveVector * _distance;
}

#ifdef _DEBUG
void Collider::UpdateWorldMatrix()
{
	D3DXMATRIX translateMatrix, rotateMatrix, scaleMatrix;

	D3DXMatrixIdentity(&worldMatrix_);

	D3DXMatrixTranslation(&translateMatrix, position_.x, position_.y, position_.z);

	D3DXMatrixRotationYawPitchRoll(&rotateMatrix, 0, 0, 0);

	D3DXMatrixScaling(&scaleMatrix, 1, 1, 1);

	D3DXMatrixMultiply(&worldMatrix_, &worldMatrix_, &scaleMatrix);

	D3DXMatrixMultiply(&worldMatrix_, &worldMatrix_, &rotateMatrix);

	D3DXMatrixMultiply(&worldMatrix_, &worldMatrix_, &translateMatrix);
}

void Collider::UpdateVertexBuffer()
{
	VERTEX_3D* vertexBuf;
	int numVertex = (int)mesh_->GetNumVertices();

	mesh_->LockVertexBuffer(0, (void**)&vertexBuf);

	for (int i = 0; i < numVertex; i++)
	{
		vertexBuf[i].color = D3DCOLOR_RGBA(255, 255, 255, 128);
	}

	mesh_->UnlockVertexBuffer();
}

void Collider::CreateSphere()
{
	LPDIRECT3DDEVICE9 device = Renderer::GetDevice();

	if (mesh_ != nullptr)
	{
		mesh_->Release();
		mesh_ = nullptr;
	}

	D3DXCreateSphere(device, radius_, 32, 16, &mesh_, nullptr);
}
#endif
