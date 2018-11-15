// author : yusuke seki
// data   : 20181115
#ifndef BASEPOINT_H_
#define BASEPOINT_H_

#include "Unit.h"
#include "Collider.h"

class BasePoint : public Unit
{
public:
	BasePoint(const Object::TYPE& _type, const float& _maxHp);
	virtual ~BasePoint();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void ReceiveDamage(const float& _damage, Unit* _unit);

	float GetBrowedRange();

	Collider* GetObjectCollider();
	Collider* GetBrowedCollider();

private:
	float maxHp_;
	float currentHp_;
	float browedRange_;

	Collider* objectCollider_;
	Collider* browedCollider_;
};

#endif
