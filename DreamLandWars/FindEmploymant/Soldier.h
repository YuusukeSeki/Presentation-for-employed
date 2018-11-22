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
class SoldierGauge;

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
	void Death();

	void FormLine();

	void SetDestination(const D3DXVECTOR3& _destinationPoint);
	void SetEndFront(const D3DXVECTOR3& _endFront);
	void SetTargetBasePoint(BasePoint* _targetBasePoint);

	//Behave GetBehave();

	Collider* GetSearchCollider();

	void ReceiveDamage(const float& _damage, Unit* _unit = nullptr);

	void SetActive(const bool& _isActive);

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

	static const float kBreakPower_;
	static const float kAttackPower_;
	static const float kMaxHp_;
	static const float kSearchEnemyRange_;
	static const float kSearchFriendRange_;

	float speed_;
	float currentHp_;
	float maxHp_;
	float attackPower_;
	float defencePower_;
	float breakPower_;
	float searchEnemyRange_;
	float searchFriendRange_;

	D3DXVECTOR3 destination_;
	D3DXVECTOR3 frontAfterArriveDestination_;
	BasePoint* targetBasePoint_;

	SoldierCommander* parentCommander_;

	Collider* searchEnemyCollider_;
	Collider* searchFriendCollider_;

	SoldierGauge* hpGauge_;
};


#endif
