// author : yusuke seki
// data   : 20181111
#ifndef SOLDIERCOMMANDER_H_
#define SOLDIERCOMMANDER_H_

#include "Object.h"
#include "SoldierGenerator.h"
#include "Soldier.h"

class RelayPoint;

#include <vector>

class SoldierCommander : public Object
{
public:
	enum Order
	{
		STOP,
		FORM,
		RUN,
		TURN,
		ATTACK_ENEMY_UNIT,
		ASSAULT_ENEMY_BASEPOINT,
		DEATH,
	};

	SoldierCommander();
	~SoldierCommander();
	
	static void CreateBuffer(const unsigned int& _numCreate);
	static SoldierCommander* Create(SoldierGenerator* _soldierGenerator);
	static SoldierCommander* DynamicCreate(SoldierGenerator* _soldierGenerator);	// ìÆìIê∂ê¨
	void Init(SoldierGenerator* _soldierGenerator);
	void Uninit();
	void Update();
	void Draw() {}

	void AddSoldier();
	void MarchedStart();

	void ReceiveReport(Soldier::Report _report, Soldier* _reporter = nullptr, BasePoint* _findBasePoint = nullptr);

	D3DXVECTOR3 GetFront();
	float GetSpeed();
	RelayPoint* GetTargetRelayPoint();
	
private:
	static SoldierCommander* FindNonActiveSoldierCommander();

	void ExecOrder();
	void Order_Stop();
	void Order_Form();
	void Order_Run();
	void Order_Turn();
	void Order_AttackEnemyUnit();
	void Order_AssaultEnemyBasePoint();
	void Order_Death();

	void SoldierLine_Stop();
	void SoldierLine_FormLine();
	void SoldierLine_Run();
	void SoldierLine_Turn();
	void SoldierLine_ShotBullet();
	void SoldierLine_AssaultBasePoint();
	void SoldierLine_Kill();

	void ReceiveReport_Death(Soldier* _soldier);
	void ReceiveReport_FindFriendUnit();
	void ReceiveReport_FindEnemyUnit();
	void ReceiveReport_FindEnemyBasePoint(BasePoint* _enemyBasePoint);
	void ReceiveReport_BreakBasePoint();

	bool IsChangeOrder(const Order& _changeOrder);
	void ChangeOrder(const Order& _changeOrder);

	void UpdateBulletTimer();
	bool IsAlartBulletTimer();
	void ResetBulletTimer(const unsigned int& _time);

	void RemoveSoldier(Soldier* _soldier);

	void ResetDestinationToSoldier();
	bool IsCorrectFormLine();
	void SetDestinationToSoldier();

	bool IsPathedTurnPoint();
	D3DXVECTOR3 FindPositionAfterTurned();

	D3DXVECTOR3 FindFront();
	D3DXVECTOR3 FindRight();

	static const unsigned int kInterval_ShotBullet_;
	static const float kLength_SoldierToSoldier_;
	static const float kSpeed_SoldierRun_;

	Order currentOrder_;
	D3DXVECTOR3 front_;
	float speed_;
	RelayPoint* targetRelayPoint_;
	BasePoint* targetBasePoint_;
	unsigned int bulletTimer_;
	bool isMarchedStart_;

	SoldierGenerator* soldierGenerator_;
	std::vector<Soldier*> soldierList_;
	std::vector<D3DXVECTOR3> soldierDestinationList_;

	D3DXVECTOR3 right_;
	D3DXVECTOR3 vectorToReleyPoint_;

};


#endif
