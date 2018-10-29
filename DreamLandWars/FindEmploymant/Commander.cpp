//*****************************************************************************
//
//		指揮官
//													Author : Yusuke Seki
//*****************************************************************************
#include "Commander.h"
#include "soldier.h"
#include "collision.h"
#include "ObjectModel.h"
#include "ModelData.h"

#include "Etc_Paramaters.h"


// コンストラクタ
Commander::Commander() : Object(Object::TYPE::TYPE_COMMANDER)
{
	m_pSoldier   = nullptr;					// 指揮下の兵士の先頭アドレス
	m_front      = D3DXVECTOR3(0, 0, 0);	// 前方向
}


// デストラクタ
Commander::~Commander()
{

}


// 生成処理
Commander* Commander::CreateBuffer()
{
	Commander* pCommander = new Commander;
	pCommander->Init();

	return pCommander;
}


// 初期化処理
void Commander::Init()
{
	m_pSoldier         = nullptr;				// 指揮下の兵士の先頭アドレス
	m_front            = D3DXVECTOR3(0, 0, 1);	// 前方向
	m_restTime         = 0;						// 次の射撃までの休憩時間
	m_bInstance        = false;					// 使用中フラグ
	m_radius_ViewAngle = 30.f;					// 視野角の半径
	m_bFormLine        = true;					// true で整列している
	m_velocity         = SOLDIER_MOVESPEED;		// 移動速度
	m_bSearchEnemy     = false;					// 敵発見フラグ
	m_pSearchBasePoint = nullptr;				// 拠点の発見フラグ

}


// 終了処理
void Commander::Uninit()
{

}


// 更新処理
void Commander::Update()
{
	// 未使用で無処理
	if (!m_bInstance) return;

	// 拠点を見つけたら突撃
	else if (m_pSearchBasePoint != nullptr)
	{
		AssaultBasePoint();

		SetPosition(m_pSearchBasePoint->GetPosition());
	}

	// 整列していなかったら整列する
	else if (!CheckFormLine_Mine()) {

	}

	// 敵を発見したら止まる
	else if (m_bSearchEnemy) {
		// 休憩時間中でなければ弾を撃つ
		if (!m_restTime) {
			// 指揮下の兵士に弾を撃たせる
			ShotBullet();

			// 休憩し始める
			m_restTime = 300;

		}
	}

	else {

		// 前方の兵士列が整列しているかどうかを検査する
		if (!CheckFormLine_Others()) {
			// 指揮下の兵士の移動
			SoldierMove();

			// 自分も同じだけ、同じ方向に移動
			MovePosition(m_front * m_velocity);

			// 自分の位置が目標地点を超えたかを検査する
			//if () {

			//	// フラグを立て、次のフレームから向きを変える処理を行う

			//}



		}

	}

	// 敵発見フラグを降ろす
	m_bSearchEnemy = false;

	// 休憩時間を減らす
	m_restTime = m_restTime - 1 <= 0 ? 0 : m_restTime - 1;

}


// 指揮官の動的生成処理
// positon ：生成位置
// 返り値  ：生成された指揮官のポインタ
Commander* Commander::SetCommander(D3DXVECTOR3& position, D3DXVECTOR3& front, Object::GROUP group)
{
	// 実体持ちを探す
	Commander* pCommander = (Commander*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_COMMANDER);

	// １つも作られていなかったら生成する
	if (pCommander == nullptr) {
		// 生成処理
		pCommander = Commander::CreateBuffer();

		// 設定処理
		pCommander->SetCommander_private(position, front, group);

		// 兵士を生成する
		pCommander->SpawnSoldier();

		return pCommander;
	}

	// 作られていたら未使用領域を探す
	Commander* pCurrent = (Commander*)pCommander;
	Commander* pNext = (Commander*)pCommander->GetNextPointer();
	for (;;) {
		// 未使用なら兵士を生成して終了
		if (!pCurrent->GetInstance()) {
			// 設定処理
			pCurrent->SetCommander_private(position, front, group);

			// 兵士を生成する
			pCurrent->SpawnSoldier();

			return pCurrent;
		}

		// 未使用領域が見つからなければ、新しく生成する
		if (pNext == nullptr) {
			// 生成処理
			pNext = Commander::CreateBuffer();

			// 設定処理
			pNext->SetCommander_private(position, front, group);

			// 兵士を生成する
			pNext->SpawnSoldier();

			return pNext;
		}

		// ポインタをずらす
		pCurrent = pNext;
		pNext = (Commander*)pCurrent->GetNextPointer();

	}

}


// 指揮下の兵士から死亡報告を受け取る
// pSoldier：死んだ兵士
void Commander::ReceiveReport_Death(Soldier* pSoldier)
{
	// 先頭の兵士だった場合は次の兵士に入れ替える
	if (m_pSoldier == pSoldier) {
		m_pSoldier = m_pSoldier->GetNextSoldier();

		// 次の兵士がいなかった場合は指揮官の使用フラグを降ろし、待機状態にする
		if (m_pSoldier == nullptr) {
			m_bInstance = false;
			return;
		}

	}
	// まだ兵士が残っている場合は、死んだ兵士の前後のリストを修正する
	else {
		if (pSoldier->GetNextSoldier() != nullptr) pSoldier->GetNextSoldier()->SetPrevSoldier(pSoldier->GetPrevSoldier());
		if (pSoldier->GetPrevSoldier() != nullptr) pSoldier->GetPrevSoldier()->SetNextSoldier(pSoldier->GetNextSoldier());
	}

	// 兵士の位置を修正する
	{
		D3DXVECTOR3 vec;
		float move = 7.5f;
		Soldier* tagSoldier = m_pSoldier;

		// 死んだ兵士の方向に動かす
		for (;;) {
			vec = pSoldier->GetPosition() - tagSoldier->GetPosition();
			D3DXVec3Normalize(&vec, &vec);
			tagSoldier->MovePosition(vec * move);

			tagSoldier = tagSoldier->GetNextSoldier();

			if (tagSoldier == nullptr)
				break;
		}
	}

}


// 指揮下の兵士から拠点破壊報告を受け取る
void Commander::ReceiveReport_BreakBasePoint()
{
	// 指揮下の兵士を全て殺す
	Soldier* pCurrent = m_pSoldier;

	if (pCurrent == nullptr) return;

	Soldier* pNext = pCurrent->GetNextSoldier();

	for (;;) {
		m_pSoldier->KillMe();

		if (m_pSoldier == nullptr)
			break;

	}


}


//=============================================================================
//	private 関数
// 指揮官の設定処理
void Commander::SetCommander_private(D3DXVECTOR3& position, D3DXVECTOR3& front, Object::GROUP group)
{
	// 位置
	SetPosition(position);

	// 前方向
	m_front = front;

	// 所属グループ
	SetGroup(group);

	// 休憩時間
	m_restTime = 0;

	// 使用中フラグON
	m_bInstance = true;

	// 発見フラグOFF
	m_bSearchEnemy     = false;		// 敵発見フラグ
	m_pSearchBasePoint = nullptr;	// 拠点発見フラグ


}


// 指揮下の兵士を生成
void Commander::SpawnSoldier()
{
	D3DXVECTOR3 vecZ, vecX;
	vecZ = GetPosition() + m_front - GetPosition();

	D3DXVec3Cross(&vecX, &vecZ, &D3DXVECTOR3(0, 1, 0));
	vecX.y = 0.f;

	D3DXVec3Normalize(&vecX, &vecX);

	m_pSoldier = Soldier::SetSoldier(GetPosition() + vecX * *D3DXVECTOR3(-60, 0, 0), m_front, MODELDATA_SOLDIER, GetGroup(), this);

	m_pSoldier->SetPrevSoldier(nullptr);
	m_pSoldier->SetNextSoldier(nullptr);

	Soldier* pPrev    = m_pSoldier;
	Soldier* pCurrent = nullptr;

	D3DXVECTOR3 space(-60.f, 0, 0);

	for (int i = 1; i < 9; i++) {

		space.x += i * 15;
		D3DXVECTOR3 a(vecX.x * space.x, vecX.y * space.y, vecX.z * space.z);
		D3DXVECTOR3 b = vecX * * D3DXVECTOR3(-60.f + i * 15, 0, 0);
		float c = D3DXVec3Dot(&a, &b);

		//pCurrent = Soldier::SetSoldier(GetPosition() + a, m_front, MODELDATA_SOLDIER, GetGroup(), this);
		pCurrent = Soldier::SetSoldier(GetPosition() + vecX * * D3DXVECTOR3(-60.f + i * 15, 0, 0), m_front, MODELDATA_SOLDIER, GetGroup(), this);

		pPrev->SetNextSoldier(pCurrent);
		pCurrent->SetPrevSoldier(pPrev);
		pCurrent->SetNextSoldier(nullptr);

		pPrev = pCurrent;

	}

}


// 指揮下の兵士を進ませる
void Commander::SoldierMove()
{
	if (m_pSoldier == nullptr) return;

	Soldier* pCurrent = m_pSoldier;

	for (;;) {
		pCurrent->Run(m_front, m_velocity);

		if (pCurrent->GetNextSoldier() == nullptr) break;

		pCurrent = pCurrent->GetNextSoldier();

	}

}


// 前方の兵士列が整列しているかどうかを検査する
// 返り値：	true  整列している
//			false 整列していない、存在しない
bool Commander::CheckFormLine_Others()
{
	Commander* pCurrent = (Commander*)Object::GetLDATA_HEAD(TYPE_COMMANDER);

	if (pCurrent == nullptr) return false;

	Commander* pNext = (Commander*)pCurrent->GetNextPointer();

	for (;;) {

		// 自分ではない、使用中、同じグループ
		if (pCurrent != this && pCurrent->GetInstance() && pCurrent->GetGroup() == GetGroup()) {
			// 当たり判定
			if (Collision_SphereToSphere(GetPosition() + m_front * *D3DXVECTOR3(0, 0, m_radius_ViewAngle), m_radius_ViewAngle, pCurrent->GetPosition(), 1.f)) {
				// 整列しているかどうかを検査する
				if (pCurrent->GetFormLine()) {
					// 自分は相手よりも前にいるか
					if ((pCurrent->GetPosition().z - GetPosition().z) * m_front.z > 0) {
						return true;

					}
				}
			}
		}

		pCurrent = pNext;

		if (pCurrent == nullptr) return false;

		pNext = (Commander*)pCurrent->GetNextPointer();

	}

}


// 指揮下の兵士を塔に突撃させる
void Commander::AssaultBasePoint()
{
	Soldier* pCurrent = m_pSoldier;

	if (pCurrent == nullptr) return;

	Soldier* pNext = pCurrent->GetNextSoldier();

	for (;;)
	{
		// 目標拠点を設定する
		pCurrent->SetTargetBasePoint(m_pSearchBasePoint);

		// 向きを拠点に向ける
		pCurrent->SetRotateToObj(m_pSearchBasePoint->GetPosition());

		// 拠点に向かって進ませる
		D3DXVECTOR3 vec = m_pSearchBasePoint->GetPosition() - pCurrent->GetPosition();
		D3DXVec3Normalize(&vec, &vec);
		pCurrent->Run(vec, m_velocity);

		if (pNext == nullptr)
			break;

		pCurrent = pNext;
		pNext = pCurrent->GetNextSoldier();

	}

}


// 指揮下の兵士に弾を撃たせる
void Commander::ShotBullet()
{
	Soldier* pCurrent = m_pSoldier;

	if (pCurrent == nullptr) return;

	Soldier* pNext = pCurrent->GetNextSoldier();

	int i = 0;

	for (;;) {
		i++;
		pCurrent->ShotBullet();

		pCurrent = pNext;

		if (pCurrent == nullptr)
			break;

		pNext = pCurrent->GetNextSoldier();

	}

}


// 指揮下の兵士が整列しているかどうかを検査する
// 返り値：	true  整列している
//			false 整列していない
bool Commander::CheckFormLine_Mine()
{
	Soldier* pBase = m_pSoldier;

	// 兵士の右方向にレイを飛ばす
	


	// 全兵士のpositionが閾値内ならば true を返す



	return true;
	
}





