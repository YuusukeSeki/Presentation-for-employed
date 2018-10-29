//*****************************************************************************
//	
//		兵士の弾
//													Autohr : Yusuke Seki
//*****************************************************************************
// base
#include "SoldierBullet.h"

// fw
#include "camera.h"
#include "collision.h"

// object
#include "ObjectModel.h"
#include "player.h"
#include "Soldier.h"

const float SoldierBullet::bulletDamage_cast = 5.f;		// 標的：キャストの場合の兵士弾のダメージ
const float SoldierBullet::bulletDamage_soldier = 50.f;	// 標的：兵士の場合の兵士弾のダメージ


// コンストラクタ
SoldierBullet::SoldierBullet() : BulletBillboard(Object::TYPE::TYPE_3D_SOLDIERBULLET)
{
	m_pTarget = nullptr;
}

// デストラクタ
SoldierBullet::~SoldierBullet()
{
	Uninit();
}

// 生成処理
SoldierBullet* SoldierBullet::CreateBuffer()
{
	SoldierBullet *pSoldierBullet = new SoldierBullet;
	pSoldierBullet->Init();

	return pSoldierBullet;
}

// 初期化処理
void SoldierBullet::Init()
{
	BulletBillboard::Init(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(6, 6, 0), nullptr);
	m_pTarget = nullptr;
}

// 終了処理
void SoldierBullet::Uninit(void)
{
	BulletBillboard::Uninit();
}

// 更新処理
void SoldierBullet::Update(void)
{
	// 未使用時は無処理
	if (!GetInstance())
		return;

	// 標的はキャスト？
	if (m_pTarget->GetType() == Object::TYPE::TYPE_MODEL_PLAYER) {
		Player* pPlayer = (Player*)m_pTarget;

		// キャストの回避行動で追尾フラグを切る
		if (pPlayer->GetPosture() == Player::POSTURE::POSTURE_AVOID || pPlayer->GetPosture() == Player::POSTURE::POSTURE_EMAVOID) {
			m_bTracking = false;

		}

	}

	// 追尾処理
	if (m_bTracking) {
		m_front = m_pTarget->GetPosition() + D3DXVECTOR3(0, m_pTarget->GetHalfSize().y, 0) - GetPosition();
		D3DXVec3Normalize(&m_front, &m_front);
	}

	// 移動処理
	MovePosition(m_front * 2.f);

	// 当たり判定
	if (Collision_SphereToSphere(GetPosition(), GetRadius(), m_pTarget->GetPosition(), m_pTarget->GetHalfSize().x)) {

		// 標的種類の判別
		if (m_pTarget->GetType() == Object::TYPE::TYPE_MODEL_PLAYER) {
			// 標的：プレイヤー
			Player* pPlayer = (Player*)m_pTarget;
		}
		else {
			// 標的：兵士
			Soldier* pSoldier = (Soldier*)m_pTarget;
			pSoldier->Damage(bulletDamage_soldier);
		}

		// 使用フラグを降ろす
		SetInstance(false);

		return;
	}

	// 寿命の減衰
	m_cntEraseFrame--;

	// 寿命が切れたら使用フラグを降ろす
	if (m_cntEraseFrame <= 0)
		SetInstance(false);

}

// 描画処理
void SoldierBullet::Draw(void)
{
	if (!GetInstance())
		return;

	BulletBillboard::Draw();
}

// 兵士弾の動的生成処理
// position : 生成位置
// pTarget  : ターゲット
void SoldierBullet::SetBullet(D3DXVECTOR3& position, ObjectModel *pTarget, Camera *pCamera)
{
	// 実体持ちを探す
	SoldierBullet* pCurrent = (SoldierBullet*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_3D_SOLDIERBULLET);

	// １つも作られていなかったら生成する
	if (pCurrent == nullptr) {
		// 生成処理
		pCurrent = SoldierBullet::CreateBuffer();

		// 設定処理
		pCurrent->SetBullet_private(position, pTarget, pCamera);

		return;
	}

	// 作られていたら未使用領域を探す
	SoldierBullet* pNext = (SoldierBullet*)pCurrent->GetNextPointer();
	for (;;) {
		// 未使用なら兵士弾を設定して終了
		if (!pCurrent->GetInstance()) {
			// 設定処理
			pCurrent->SetBullet_private(position, pTarget, pCamera);

			return;
		}

		// 未使用領域が見つからなければ、新しく生成する
		if (pNext == nullptr) {
			// 生成処理
			pNext = SoldierBullet::CreateBuffer();

			// 設定処理
			pNext->SetBullet_private(position, pTarget, pCamera);

			return;
		}

		// ポインタをずらす
		pCurrent = pNext;
		pNext = (SoldierBullet*)pCurrent->GetNextPointer();

	}

}

// 兵士弾の設定処理
void SoldierBullet::SetBullet_private(D3DXVECTOR3& position, ObjectModel *pTarget, Camera *pCamera)
{
	SetPosition(position + D3DXVECTOR3(0, pTarget->GetHalfSize().y, 0));

	m_pTarget = pTarget;
	
	SetCamera(*pCamera);

	m_cntEraseFrame = 240;

	m_front = m_pTarget->GetPosition() + D3DXVECTOR3(0, m_pTarget->GetHalfSize().y, 0) - GetPosition();

	D3DXVec3Normalize(&m_front, &m_front);

	m_bTracking = true;

	SetInstance(true);

}


