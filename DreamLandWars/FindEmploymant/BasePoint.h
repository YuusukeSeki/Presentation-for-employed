// author : yusuke seki
// data   : 20181115
#ifndef BASEPOINT_H_
#define BASEPOINT_H_

#include "Unit.h"
#include "Collider.h"
class Player;
class SoldierGenerator;
class RelayPoint;
//class Hold;
//class LifeGauge;
//class Icon;

class BasePoint : public Unit
{
public:
	BasePoint(const Object::TYPE& _type, const unsigned int& _kNumSoldier_First, const unsigned int& _kNumSoldier_Subsequent);
	virtual ~BasePoint();
	
	virtual void Release();

	virtual void Init(const D3DXVECTOR3& _position, const Object::GROUP& _group, RelayPoint* _nextRelayPoint);
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	void SetBrowedRange(const float& _browedRange);
	Collider* GetBrowedCollider();

	RelayPoint* GetNextRelayPoint();

	virtual void ReceiveDamage(const float& _damage, Unit* _unit) = 0;
	virtual bool IsBreak() = 0;

	bool IsBrowedRange(Player* _player);

protected:
	void SendIsBrakedSoldierGenerator();
	SoldierGenerator* GetSoldierGenerator()
	{
		return soldierGenerator_;
	}

	static const unsigned int kNumFrame_CreateSoldierLine_First_;
	static const unsigned int kNumFrame_CreateSoldierLine_Subsequent_;

private:
	const unsigned int kNumSoldier_First_;
	const unsigned int kNumSoldier_Subsequent_;

	SoldierGenerator * soldierGenerator_;

	float browedRange_;
	Collider* browedCollider_;

	RelayPoint* nextRelayPoint_;

	//Hold* hold_;
	//LifeGauge* lifeGauge_;
	//Icon* icon_;
};

#endif
