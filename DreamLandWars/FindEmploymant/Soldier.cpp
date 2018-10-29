//*****************************************************************************
//
//		兵士
//												Autohr : Yusuke Seki
//*****************************************************************************
// base
#include "Soldier.h"
#include "Commander.h"

// fw
#include "collision.h"

// Object
#include "player.h"
#include "tower.h"
#include "castle.h"
#include "SoldierBullet.h"

// resource
#include "MainGame.h"


// Initialize ID
int Soldier::m_cntID = 0;



// コンストラクタ
Soldier::Soldier() : ObjectModel(Object::TYPE::TYPE_MODEL_SOLDIER)
{
	// データのクリア
	m_bInstance = false;

}

// デストラクタ
Soldier::~Soldier()
{
	Uninit();

}

// 生成
Soldier* Soldier::Create(D3DXVECTOR3& position, const char* modelPass)
{
	Soldier* pSoldier = new Soldier;
	pSoldier->Init(position, modelPass);

	return pSoldier;

}

// 初期化
void Soldier::Init(D3DXVECTOR3& position, const char* modelPass)
{
	// データの初期化
	ObjectModel::Init(position, modelPass);
	m_searchRange = 45;
	m_breakPower = 10;

	m_ID = m_cntID;

	m_bInstance = false;

	m_cntID++;

}

// 終了
void Soldier::Uninit(void)
{
	ObjectModel::Uninit();

	m_cntID = 0;
}

// 更新
void Soldier::Update(void)
{
	if (!m_bInstance) return;

	// 敵の塔に向かって突撃する
	if (m_pAssaultBasePoint != nullptr)
	{
		// 塔に当たったら死ぬ
		if (CollisionBasePoint())
		{
			BreakBasePoint();
		}
	}

	// 感知範囲に敵の拠点を発見したら指揮官に報告する
	else if ((m_pAssaultBasePoint = SearchTower()) != nullptr || (m_pAssaultBasePoint = SearchCastle()) != nullptr)
		m_pParent->ReceiveReport_SearchBasePoint(m_pAssaultBasePoint);

	// 感知範囲に敵を発見したら指揮官に報告する
	else if (SearchEnemy())
		m_pParent->ReceiveReport_SearchEnemy();

}

// 描画
void Soldier::Draw(void)
{
	if (!m_bInstance) return;

	ObjectModel::Draw();

}

// 走る
void Soldier::Run(D3DXVECTOR3& vecZ, float velocity)
{
	SetRotateToObj((D3DXVECTOR3(vecZ * velocity) + GetPosition()));
	MovePosition(vecZ * velocity);

}

// 攻撃される
void Soldier::Attack(float damage, D3DXVECTOR3& vector, float accelerate, Player* pPlayer)
{
	// ダメージを受ける
	if (Damage(damage)) {
		// 死んだ

		// プレイヤーの撃破数上昇
		pPlayer->PlusScore_CrushSoldier();

	}
	else {
		// 生きてる

		// 飛ばされる
		MovePosition(vector * accelerate);
	}
	
}

// ダメージを受ける
// damage：与えるダメージ数
// 返り値： true  死んだよ
//			false 生きてるよ
bool Soldier::Damage(float damage)
{
	m_life -= damage;

	if (m_life <= 0) {
		m_pParent->ReceiveReport_Death(this);

		m_bInstance = false;
		return true;
	}

	return false;
}

// 目標拠点を設定する
void Soldier::SetTargetBasePoint(ObjectModel* pTarget)
{
	m_pAssaultBasePoint = pTarget;
}

// 敵に攻撃をする
void Soldier::ShotBullet()
{
	ObjectModel* pTarget = SearchNearEnemy();

	if (pTarget == nullptr)
		return;

	SetRotateToObj(D3DXVECTOR3(pTarget->GetPosition()));
	SoldierBullet::SetBullet(GetPosition(), pTarget, MainGame::GetCamera(0));
	
}

// 兵士の動的生成処理
Soldier* Soldier::SetSoldier(D3DXVECTOR3& position, D3DXVECTOR3& front, const char* modelPass, Object::GROUP group, Commander* pCommnader)
{
	// 実体持ちを探す
	Soldier* pSoldier = (Soldier*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_MODEL_SOLDIER);

	// １つも作られていなかったら生成する
	if (pSoldier == nullptr) {
		// 生成処理
		pSoldier = Soldier::Create(position, modelPass);
		pSoldier->SetSoldier_private(position, front, group, pCommnader);

		return pSoldier;
	}

	// 作られていたら未使用領域を探す
	Soldier* pCurrent = (Soldier*)pSoldier;
	Soldier* pNext = (Soldier*)pSoldier->GetNextPointer();
	for (;;) {
		// 未使用なら兵士を初期化して終了
		if (!pCurrent->GetInstance()) {
			pCurrent->SetSoldier_private(position, front, group, pCommnader);
			return pCurrent;
		}

		// 未使用領域が見つからなければ、新しく生成する
		if (pNext == nullptr) {
			// 生成処理
			pNext = Soldier::Create(position, modelPass);
			pNext->SetSoldier_private(position, front, group, pCommnader);

			return pNext;
		}

		// ポインタをずらす
		pCurrent = pNext;
		pNext = (Soldier*)pCurrent->GetNextPointer();

	}

}

// この兵士を殺す
void Soldier::KillMe()
{
	Damage(m_maxLife);
}


//=============================================================================
//	private 関数
// 兵士の設定処理
void Soldier::SetSoldier_private(D3DXVECTOR3& position, D3DXVECTOR3& front, Object::GROUP group, Commander* pCommnader)
{
	// 位置
	SetPosition(position);

	// 向き
	SetFront(front);

	// 所属グループ
	SetGroup(group);

	// 直属の指揮官
	m_pParent = pCommnader;

	// 色、向き
	if (group == Object::GROUP_A) {
		SetColor(0xff0000ff);
		SetRotate(D3DXVECTOR3(0, 0, 0));
	}
	else if (group == Object::GROUP_B) {
		SetColor(0x0000ffff);
		SetRotate(D3DXVECTOR3(0, D3DXToRadian(180), 0));
	}

	// 体力の最大値
	m_maxLife = 100;

	// 現在の体力
	m_life    = m_maxLife;

	// 発見した拠点
	m_pAssaultBasePoint = nullptr;

	// 使用フラグON
	m_bInstance = true;
	
}

// 索敵
// 返り値： true  発見
//			false いないよ
bool Soldier::SearchEnemy()
{
	if (SearchEnemy_Soldier() || SearchEnemy_Cast()) return true;

	return false;
}

// 索敵（兵士）
// 返り値： true  発見
//			false いないよ
bool Soldier::SearchEnemy_Soldier()
{
	Soldier* pSoldier = (Soldier*)Object::GetLDATA_HEAD(TYPE_MODEL_SOLDIER);

	if (pSoldier == nullptr) return false;

	Soldier* pCurrent = pSoldier;
	Soldier* pNext = (Soldier*)pSoldier->GetNextPointer();

	for (;;) {

		if (pCurrent->GetGroup() != GetGroup() && pCurrent->GetInstance())
			if (Collision_SphereToSphere(GetPosition(), m_searchRange, pCurrent->GetPosition(), pCurrent->GetRadius())) return true;

		pCurrent = pNext;

		if (pCurrent == nullptr) return false;

		pNext = (Soldier*)pCurrent->GetNextPointer();

	}

}

// 索敵（キャスト）
// 返り値： true  発見
//			false いないよ
bool Soldier::SearchEnemy_Cast()
{
	Player* pPlayer = (Player*)Object::GetLDATA_HEAD(TYPE_MODEL_PLAYER);

	if (pPlayer == nullptr) return false;

	Player* pCurrent = pPlayer;
	Player* pNext = (Player*)pPlayer->GetNextPointer();

	for (;;) {

		if(pCurrent->GetGroup() != GetGroup())
			if (Collision_SphereToSphere(GetPosition(), m_searchRange, pCurrent->GetPosition(), pCurrent->GetRadius())) return true;

		pCurrent = pNext;

		if (pCurrent == nullptr) return false;

		pNext = (Player*)pCurrent->GetNextPointer();

	}

}

// 索敵（塔）
// 返り値： true  発見
//			false いないよ
Tower* Soldier::SearchTower()
{
	Tower* pTower = (Tower*)Object::GetLDATA_HEAD(TYPE_MODEL_TOWER);

	if (pTower == nullptr)
		return nullptr;

	Tower* pCurrent = pTower;
	Tower* pNext = (Tower*)pTower->GetNextPointer();

	for (;;) {

		if (pCurrent->GetGroup() != GetGroup())
			if (Collision_SphereToSphere(GetPosition(), m_searchRange, pCurrent->GetPosition(), pCurrent->GetRadius()))
				return pCurrent;

		pCurrent = pNext;

		if (pCurrent == nullptr)
			return nullptr;

		pNext = (Tower*)pCurrent->GetNextPointer();

	}

}

// 索敵（城）
// 返り値： true  発見
//			false いないよ
Castle* Soldier::SearchCastle()
{
	Castle* pCastle = (Castle*)Object::GetLDATA_HEAD(TYPE_MODEL_CASTLE);

	if (pCastle == nullptr)
		return nullptr;

	Castle* pCurrent = pCastle;
	Castle* pNext = (Castle*)pCastle->GetNextPointer();

	for (;;) {

		if (pCurrent->GetGroup() != GetGroup())
			if (Collision_SphereToSphere(GetPosition(), m_searchRange, pCurrent->GetPosition(), pCurrent->GetRadius()))
				return pCurrent;

		pCurrent = pNext;

		if (pCurrent == nullptr)
			return nullptr;

		pNext = (Castle*)pCurrent->GetNextPointer();

	}

}

// 一番近い敵を探す
// 返り値：	一番近い敵のポインタ
ObjectModel* Soldier::SearchNearEnemy()
{
	Soldier *pCurrentA = (Soldier*)Object::GetLDATA_HEAD(Object::TYPE_MODEL_SOLDIER);

	if (pCurrentA == nullptr)
		return nullptr;

	Soldier *pNextA = (Soldier*)pCurrentA->GetNextPointer();
	Soldier *pTargetA = nullptr;

	float minA, minB;
	minA = minB = 99999999.f;

	// 一番近い兵士を探す
	for (;;) {
		if (pCurrentA->GetGroup() != GetGroup() && pCurrentA->GetInstance()) {
			float buf = Distance3D(GetPosition(), pCurrentA->GetPosition());

			if (buf < minA) {
				minA = buf;
				pTargetA = pCurrentA;
			}
		}

		pCurrentA = pNextA;

		if (pCurrentA == nullptr)
			break;

		pNextA = (Soldier*)pCurrentA->GetNextPointer();

	}

	
	Player *pCurrentB = (Player*)Object::GetLDATA_HEAD(Object::TYPE_MODEL_PLAYER);

	if (pCurrentB == nullptr)
		return nullptr;

	Player *pNextB = (Player*)pCurrentB->GetNextPointer();
	Player *pTargetB = nullptr;

	// 兵士よりも近いキャストを探す
	for (;;) {
		if (pCurrentB->GetGroup() != GetGroup()) {
			float buf = Distance3D(GetPosition(), pCurrentB->GetPosition());

			if (buf < minB) {
				minB = buf;
				pTargetB = pCurrentB;
			}
		}

		pCurrentB = pNextB;

		if (pCurrentB == nullptr)
			break;

		pNextB = (Player*)pCurrentB->GetNextPointer();

	}

	if (minA < minB)
		return (ObjectModel*)pTargetA;
	else
		return (ObjectModel*)pTargetB;

}

// 拠点との当たり判定
bool Soldier::CollisionBasePoint()
{
	if (Collision_SphereToSphere(GetPosition(), 0.00001f, m_pAssaultBasePoint->GetPosition(), m_pAssaultBasePoint->GetRadius() - m_pAssaultBasePoint->GetRadius() * 0.3f))
		return true;

	return false;
}

// 拠点を殴る
void Soldier::BreakBasePoint()
{
	if (m_pAssaultBasePoint->GetType() == Object::TYPE::TYPE_MODEL_CASTLE)
	{
		//dynamic_cast<Castle*>(m_pAssaultBasePoint)->
	}
	else
	{
		// 壊れたら指揮官に報告する
		if (dynamic_cast<Tower*>(m_pAssaultBasePoint)->BrowTower(m_breakPower))
		{
			m_pParent->ReceiveReport_BreakBasePoint();
			return;
		}
	}

	// Death
	Damage(m_maxLife);

}

