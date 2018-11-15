// author : yusuke seki
// data   : 20181114
#ifndef SOLDIER_H_
#define SOLDIER_H_

#include "Unit.h"
class Player;
class BasePoint;
class Tower;
class Castle;
class Collider;
class SoldierCommander;

class Soldier : public Unit
{
public:
	enum Report
	{
		NONE,
		DEATH,
		FIND_FRIEND_UNIT,
		FIND_ENEMY_UNIT,
		FIND_ENEMY_BASEPOINT,
		BREAK_ENEMY_BASEPOINT,
	};

	enum Behave
	{
		STOP,
		RUN,
		ATTACK,
		ASSAULT,
		BENDING_BACKWORD,	// ã¬ÇØîΩÇÈ
		DOWN,
		RISEUP,
		DEAD,
	};

public:
	Soldier();
	~Soldier();

	static void CreateBuffer(const unsigned int& _numCreate);
	static Soldier* Create(SoldierCommander* _soldierCommander);
	static Soldier* DynamicCreate(SoldierCommander* _soldierCommander);	// ìÆìIê∂ê¨
	void Init(SoldierCommander* _soldierCommander);
	void Uninit();
	void Update();
	void Draw();

	void Stop();
	void Run();
	void ShotBullet();
	void AssaultBasePoint();
	void SelfDelete();

	void FormLine();

	void SetDestination(const D3DXVECTOR3& _destinationPoint);
	void SetEndFront(const D3DXVECTOR3& _endFront);
	void SetTargetBasePoint(ObjectModel* _targetBasePoint);

	void ReceiveDamage(const float& _damage, const D3DXVECTOR3& _bulletPosition);

	void SetPosition(const D3DXVECTOR3& _position);
	void SetPosition(const D3DXVECTOR3& _position, const D3DXVECTOR3& _front);
	void MovePosition(const D3DXVECTOR3& _moveVector, const float& _speed);
	D3DXVECTOR3 GetPosition();

	Behave GetBehave();

	Collider* GetObjectCollider();
	Collider* GetSearchCollider();

private:
	static Soldier* FindNonActiveSoldier();

	Unit* FindNearEnemy();
	Soldier* FindNearEnemySoldier();
	Player* FindNearEnemyPlayer();
	BasePoint* SearchEnemyBasePoint();
	Tower* SearchEnemyTower();
	Castle* SearchEnemyCastle();

	bool IsSearchEnemyUnit();
	bool IsSearchEnemySoldier();
	bool IsSearchEnemyPlayer();
	bool IsSearchFriendSoldier();

	bool CollidedBasePoint();

	void BreakBasePoint();

	static const std::string kSoldierModelPath_;
	static const float kBreakPower_;
	static const float kAttackPower_;
	static const float kMaxHp_;
	static const float kSearchRange_;

	float speed_;
	float currentHp_;
	float maxHp_;
	float attackPower_;
	float defencePower_;
	float breakPower_;
	float searchRange_;

	D3DXVECTOR3 destination_;
	D3DXVECTOR3 frontAfterArriveDestination_;
	ObjectModel* targetBasePoint_;

	SoldierCommander* parentCommander_;

	Collider* objectCollider_;
	Collider* searchCollider_;
};


#endif
