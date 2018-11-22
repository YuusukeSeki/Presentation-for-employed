// author : yusuke seki
// data   : 20181116
#include "castle.h"
#include "collision.h"
#include "SoldierGenerator.h"
#include "SoldierCommander.h"
#include "Part.h"

const unsigned int Castle::kNumSoldier_First_ = 9;
const unsigned int Castle::kNumSoldier_Subsequent_ = 9;

Castle::Castle(const Object::TYPE& _type) : BasePoint(_type, kNumSoldier_First_, kNumSoldier_Subsequent_)
{
	leftSoldierGenerator_ = nullptr;
	rightSoldierGenerator_ = nullptr;
}

Castle::~Castle()
{
	Uninit();
}

Castle* Castle::Create(const D3DXVECTOR3& _position, const Object::GROUP& _group
	, RelayPoint* _leftRelayPoint, RelayPoint* _rightRelayPoint)
{
	Castle* castle = new Castle(Object::TYPE::TYPE_MODEL_CASTLE);
	castle->Init(_position, _group, _leftRelayPoint, _rightRelayPoint);

	return castle;
}

void Castle::Init(const D3DXVECTOR3& _position, const Object::GROUP& _group
	, RelayPoint* _leftRelayPoint, RelayPoint* _rightRelayPoint)
{
	BasePoint::Init(_position, _group, _leftRelayPoint);

	AddPart("castle", "data/model/MainGame/castle03.x");

	SetBrowedRange(GetHalfSize().z + 3.5f);

	if (rightSoldierGenerator_ == nullptr)
	{
		rightSoldierGenerator_ = SoldierGenerator::Create(this
			, kNumFrame_CreateSoldierLine_First_, kNumSoldier_First_
			, kNumFrame_CreateSoldierLine_Subsequent_, kNumSoldier_Subsequent_);
	}
	else
	{
		rightSoldierGenerator_->Init(this);
	}

	rightSoldierGenerator_->SetNextRelayPoint(_rightRelayPoint);
	rightSoldierGenerator_->SetIsUpdateTimer(true);

}

void Castle::Uninit(void)
{
	BasePoint::Uninit();
}

void Castle::Update(void)
{
	BasePoint::Update();

	//// 兵士生成用フレームカウンターを回す
	//m_frameCounter++;

	//// フレームカウンターが規定値を超えたら兵士を生成する
	//if (m_frameCounter >= Castle::_INTERVAL_CREATE_SOLDIER) {
	//	D3DXVECTOR3 vecX;
	//	D3DXVec3Cross(&vecX, &vector_LEFT, &D3DXVECTOR3(0, 1, 0));
	//	vecX.y = 0.f;
	//	vecX.z = 0;

	//	D3DXVec3Normalize(&vecX, &vecX);

	//	vecX *= 20;

	//	// 左レーン
	//	Commander::SetCommander(GetPosition() + vecX, vector_LEFT, GetGroup());

	//	// 右レーン
	//	Commander::SetCommander(GetPosition() + vecX * -1, vector_RIGHT, GetGroup());

	//	// カウンターを巻き戻す
	//	m_frameCounter = 0;

	//}

	//// 殴れる範囲の可視化判定
	//{
	//	// プレイヤーリストの先頭データを取得
	//	Player* pCurrent = (Player*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_MODEL_PLAYER);

	//	// Error Command
	//	if (pCurrent == nullptr)
	//		return;

	//	// 敵キャストと自身との距離が規定値以内で、殴れる状態を可視化する
	//	for (;;) {

	//		// 敵キャストかどうかを判定
	//		if (pCurrent->GetGroup() != GetGroup()) {

	//			// 距離が規定値以内なら殴れる状態であることを可視化する
	//			if (Distance3D(pCurrent->GetPosition(), GetPosition()) <= m_browRange * m_browRange) {
	//				m_pHold->SetDrawHold(true);
	//			}

	//			// 範囲外なら非表示
	//			else {
	//				m_pHold->SetDrawHold(false);
	//			}

	//		}

	//		// 次のキャストへ
	//		pCurrent = (Player*)pCurrent->GetNextPointer();

	//		// 次のキャストがいなければ判定終了
	//		if (pCurrent == nullptr) {
	//			break;
	//		}

	//	}

	//}


}

void Castle::Draw(void)
{
	BasePoint::Draw();
}

void Castle::ReceiveDamage(const float& _damage, Unit* _unit)
{
	// チームゲージと紐づけて減らす
}

//void Castle::BrowCastle(float breakPower)
//{
//	TeamGaugeManager::GetTeamGauge_ENEMY()->MoveLife(-breakPower * CASTLE_RETOUCH_BREAKPOWER);
//}
//
//// 殴れる範囲との当たり判定
//// position : 対象キャストの位置
//// 【返り値】	true  : 殴れる
////				false : 殴れない
//bool Castle::CollisionBrowRange(D3DXVECTOR3& position)
//{
//	if (Distance3D(GetPosition(), position) <= m_browRange * m_browRange)
//		return true;
//
//	return false;
//
//}
//
//

bool Castle::IsBreak()
{
	return false;
}
