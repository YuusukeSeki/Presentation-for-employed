// author : yusuke seki
// data   : 20181111
#include "Bullet3D.h"
#include "renderer.h"
#include "player.h"
#include "collision.h"

Bullet3D::Bullet3D() : Object3D(Object::TYPE::TYPE_3D_BULLET)
{
	front_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	initSpped_ = 0.0f;
	accelerate_ = 0.0f;
	inirtia_ = 0.0f;
	maxSpeed_ = 0.0f;
	isInstance_ = false;
}

Bullet3D::Bullet3D(const Object::TYPE& type) : Object3D(type)
{
	front_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	initSpped_ = 0.0f;
	accelerate_ = 0.0f;
	inirtia_ = 0.0f;
	maxSpeed_ = 0.0f;
	isInstance_ = false;
}

Bullet3D::~Bullet3D()
{
	Uninit();
}

Bullet3D* Bullet3D::Create(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size)
{
	Bullet3D* bullet = new Bullet3D(Object::TYPE::TYPE_3D_BULLET);
	bullet->Init(_position, _size);

	return bullet;
}

void Bullet3D::Init(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size)
{
	Object3D::Init(_position, _size);

	front_ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	initSpped_ = 0.0f;
	accelerate_ = 0.0f;
	inirtia_ = 0.0f;
	maxSpeed_ = 0.0f;
	isInstance_ = true;
}

void Bullet3D::Uninit()
{
	Object3D::Uninit();
}

void Bullet3D::Draw()
{
	if (isInstance_ == false)
	{
		return;
	}

	Object3D::Draw();
}

void Bullet3D::SetFront(const D3DXVECTOR3& _front)
{
	front_ = _front;
}

D3DXVECTOR3& Bullet3D::GetFront()
{
	return front_;
}

void Bullet3D::SetMaxSpeed(const float& _maxSpeed)
{
	maxSpeed_ = _maxSpeed;
}

float Bullet3D::GetMaxSpeed()
{
	return maxSpeed_;
}

void Bullet3D::SetInitSpeed(const float& _initSpeed)
{
	initSpped_ = _initSpeed;
}

float Bullet3D::GetInitSpeed()
{
	return initSpped_;
}

void Bullet3D::SetAccelerate(const float& _accelerate)
{
	accelerate_ = _accelerate;
}

float Bullet3D::GetAccelerate()
{
	return accelerate_;
}

void Bullet3D::SetIsInstance(const bool& _isInstanse)
{
	isInstance_ = _isInstanse;
}

bool Bullet3D::GetIsInstance()
{
	return isInstance_;
}
