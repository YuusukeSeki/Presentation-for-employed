// author : yusuke seki
// data   : 20181111
#ifndef SOLDIER_GENERATOR_
#define SOLDIER_GENERATOR_

#include "ObjectModel.h"
class RelayPoint;
class SoldierCommander;

class SoldierGenerator
{
public:
	SoldierGenerator(const unsigned int _kInterval_CreateSoldierLine_First, const unsigned int _kNumSoldier_First
		, const unsigned int _kInterval_CreateSoldierLine_Subsequent, const unsigned int _kNumSoldier_Subsequent);
	~SoldierGenerator();

	static SoldierGenerator* Create(ObjectModel* _parentObject
		, const unsigned int _kInterval_CreateSoldierLine_First, const unsigned int _kNumSoldier_First
		, const unsigned int _kInterval_CreateSoldierLine_Subsequent, const unsigned int _kNumSoldier_Subsequent);
	void Release();
	void Init(ObjectModel* _parentObject);
	void Uninit();
	void Update();

	D3DXVECTOR3 GetPosition();

	Object::GROUP GetGroup();

	RelayPoint* GetNextRelayPoint();

	const unsigned int GetMaxNumSoldierInOneSoldierLine();

	void SetIsUpdateTimer(const bool& _isUpdateTimer);

	void ReceiveReport_BreakedParent();

private:
	void UpdateTimer();
	bool IsAlertTimer();
	void CreateSoldier();
	bool IsFilledSoldier();
	void ResetTimer(const unsigned int _timer);
	void FindSoldierCommander();

	static const unsigned int kInterval_CreateOneSoldier_;
	const unsigned int kInterval_CreateSoldierLine_First_;
	const unsigned int kNumSoldier_First_;
	const unsigned int kInterval_CreateSoldierLine_Subsequent_;
	const unsigned int kNumSoldier_Subsequent_;

	ObjectModel* parentObject_;
	unsigned int timer_;
	unsigned int countCreateSoldier_;
	bool isFirstCreate_;
	bool isUpdateTimer_;
	Object::GROUP group_;

	SoldierCommander* soldierCommander_;
	RelayPoint* nextRelayPoint_;
};

#endif
