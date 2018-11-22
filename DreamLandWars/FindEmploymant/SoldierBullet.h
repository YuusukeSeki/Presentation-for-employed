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
	static SoldierBullet* DynamicCreate(Soldier* _parentSoldier, Unit* _target);	// 動的生成
	void Init(Soldier* _parentSoldier, Unit* _target);
	void Uninit();
	void Update();
	void Draw();

	void SetActive(const bool& _isActive);

	void ReceiveDamage(const float& _damage, Unit* _unit = nullptr)
	{
		// 無処理
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
	static const unsigned int kEraseFrame_;	// 消えるまでのフレーム数

	Unit* target_;				// 標的
	D3DXVECTOR3 vector_;		// 進行方向
	bool isChase_;				// 追尾フラグ
	unsigned int remaineFrame_;	// 残りフレーム数

	ObjectBillboard* objectBillboard_;	// 画面表示用オブジェクト
};


#endif

