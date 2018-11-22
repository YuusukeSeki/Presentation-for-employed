// author : yusuke seki
// data   : 20181111
#ifndef SOLDIER_GENERATOR_
#define SOLDIER_GENERATOR_

#include "Object.h"
class BasePoint;
class RelayPoint;
class SoldierCommander;

class SoldierGenerator : public Object
{
public:
	SoldierGenerator(const unsigned int _kInterval_CreateSoldierLine_First, const unsigned int _kNumSoldier_First
		, const unsigned int _kInterval_CreateSoldierLine_Subsequent, const unsigned int _kNumSoldier_Subsequent);
	~SoldierGenerator();

	static SoldierGenerator* Create(BasePoint* _parentObject
		, const unsigned int _kInterval_CreateSoldierLine_First, const unsigned int _kNumSoldier_First
		, const unsigned int _kInterval_CreateSoldierLine_Subsequent, const unsigned int _kNumSoldier_Subsequent);
	void Init(BasePoint* _parentObject);
	void Uninit();
	void Update();
	void Draw() {}

	void SetNextRelayPoint(RelayPoint* _relayPoint);
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

	BasePoint* parentObject_;
	unsigned int timer_;
	unsigned int countCreateSoldier_;
	RelayPoint* nextRelayPoint_;
	bool isFirstCreate_;
	bool isUpdateTimer_;

	SoldierCommander* soldierCommander_;
};

#endif
