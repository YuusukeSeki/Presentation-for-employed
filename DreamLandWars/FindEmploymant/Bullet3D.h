// author : yusuke seki
// data   : 20181111
#ifndef BULLET3D_H_
#define BULLET3D_H_

#include "Object3D.h"

class Bullet3D : public Object3D
{
public:
	Bullet3D();
	Bullet3D(const Object::TYPE& type);
	virtual ~Bullet3D();

	static Bullet3D* Create(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size);
	virtual void Init(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size);
	virtual void Uninit();
	virtual void Update() {}
	virtual void Draw();

	void SetFront(const D3DXVECTOR3& _front);
	D3DXVECTOR3& GetFront();

	void SetMaxSpeed(const float& _maxSpeed);
	float GetMaxSpeed();

	void SetInitSpeed(const float& _initSpeed);
	float GetInitSpeed();

	void SetAccelerate(const float& _accelerate);
	float GetAccelerate();

	void SetIsInstance(const bool& _isInstanse);
	bool GetIsInstance();

private:
	D3DXVECTOR3 front_;
	float initSpped_;
	float accelerate_;
	float inirtia_;
	float maxSpeed_;
	bool isInstance_;

};


#endif

#pragma once
