// author : yusuke seki
// data   : 20181115
#ifndef CASTLE_H_
#define CASTLE_H_

#include "BasePoint.h"
class SoldierGenerator;

class Castle : public BasePoint
{
public:
	Castle(const Object::TYPE& _type);
	~Castle();

	static Castle* Create(const D3DXVECTOR3& _position, const Object::GROUP& _group
		, RelayPoint* _leftRelayPoint, RelayPoint* _rightRelayPoint);

	void Init(const D3DXVECTOR3& _position, const Object::GROUP& _group
		, RelayPoint* _leftRelayPoint, RelayPoint* _rightRelayPoint);
	void Uninit();
	void Update();
	void Draw();

	void ReceiveDamage(const float& _damage, Unit* _unit);
	bool IsBreak();

private:
	static const unsigned int kNumSoldier_First_;
	static const unsigned int kNumSoldier_Subsequent_;

	SoldierGenerator* leftSoldierGenerator_;
	SoldierGenerator* rightSoldierGenerator_;

};

#endif
