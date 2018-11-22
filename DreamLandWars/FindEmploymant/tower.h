// author : yusuke seki
// data   : 20181115
#ifndef TOWER_H_
#define TOWER_H_

#include "BasePoint.h"
class RelayPoint;
class ObjectBillboard;
class Gauge;

class Tower : public BasePoint
{
public:
	Tower(const Object::TYPE& _type);
	~Tower();

	static Tower* Create(const D3DXVECTOR3& _position, const Object::GROUP& _group, RelayPoint* _nextRelayPoint);

	void Init(const D3DXVECTOR3& _position, const Object::GROUP& _group, RelayPoint* _nextRelayPoint);
	void Uninit();
	void Update();
	void Draw();

	void ReceiveDamage(const float& _damage, Unit* _unit);
	bool IsBreak();

	float GetMaxHp();
	float GetCurrentHp();

private:
	static const unsigned int kNumSoldier_First_;
	static const unsigned int kNumSoldier_Subsequent_;
	static const float kMaxHp_;

	float maxHp_;
	float currentHp_;

	unsigned int releaseTimer_;

	ObjectBillboard* icon_;
	Gauge* hpGauge_;

};

#endif
