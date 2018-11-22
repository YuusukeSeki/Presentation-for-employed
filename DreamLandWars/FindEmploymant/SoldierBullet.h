// author : yusuke seki
// data   : 20181121
#ifndef SOLDIERBULLET_H_
#define SOLDIERBULLET_H_

#include "Unit.h"
class Soldier;
class Camera;
class ObjectBillboard;

class SoldierBullet : public Unit
{
public:
	SoldierBullet();
	virtual ~SoldierBullet();

	static void CreateBuffer(const unsigned int& _numCreate);
	static SoldierBullet* Create(Soldier* _parentSoldier, Unit* _target);
	static SoldierBullet* DynamicCreate(Soldier* _parentSoldier, Unit* _target);	// ���I����
	void Init(Soldier* _parentSoldier, Unit* _target);
	void Uninit();
	void Update();
	void Draw();

	void SetActive(const bool& _isActive);

	void ReceiveDamage(const float& _damage, Unit* _unit = nullptr)
	{
		// ������
	}
	
private:
	static SoldierBullet* FindNonActiveSoldierBullet();
	void CheckTargetBehave();
	void CollisionTarget();
	void ChangeVector();
	void Move();
	void UpdateTimer();
	bool IsTargetBahave_Avoid();
	void SetVectorToTarget();
	bool IsCollisionTarget();

	static const float kDamage_;
	static const float kSpeed_;
	static const unsigned int kEraseFrame_;	// ������܂ł̃t���[����

	Unit* target_;				// �W�I
	D3DXVECTOR3 vector_;		// �i�s����
	bool isChase_;				// �ǔ��t���O
	unsigned int remaineFrame_;	// �c��t���[����

	ObjectBillboard* objectBillboard_;	// ��ʕ\���p�I�u�W�F�N�g
};


#endif

