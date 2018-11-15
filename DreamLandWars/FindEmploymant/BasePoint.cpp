// author : yusuke seki
// data   : 20181115
#include "BasePoint.h"

BasePoint::BasePoint(const Object::TYPE& _type) : Unit(_type)
{
	maxHp_;
	currentHp_;
	browedRange_;

	objectCollider_;
	browedCollider_;
}

BasePoint::~BasePoint()
{

}

void BasePoint::Init()
{

}

void BasePoint::Uninit()
{

}

void BasePoint::Update()
{

}

void BasePoint::Draw()
{

}

void BasePoint::ReceiveDamage(const float& _damage, Unit* _unit)
{

}

float BasePoint::GetBrowedRange()
{

}

Collider* BasePoint::GetObjectCollider()
{

}

Collider* BasePoint::GetBrowedCollider()
{

}
