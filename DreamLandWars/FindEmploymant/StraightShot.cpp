//*****************************************************************************
//	
//		バレット
//													Autohr : Yusuke Seki
//*****************************************************************************
#include "StraightShot.h"
#include "player.h"
#include "soldier.h"
#include "collision.h"
#include "MainGame.h"



//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
StraightShot::StraightShot(Object::TYPE type) : Bullet3D(type)
{
	// データのクリア
	m_uninitPos       = D3DXVECTOR3(0, 0, 0);
	m_vecInitToUninit = D3DXVECTOR3(0, 0, 0);

}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
StraightShot::~StraightShot()
{
	Uninit();
}


//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
StraightShot* StraightShot::Create(D3DXVECTOR3& position, D3DXVECTOR3& size)
{
	// 実体を生成
	StraightShot* pStraightShot = new StraightShot(Object::TYPE::TYPE_3D_STRAIGHTSHOT);

	// 初期化
	pStraightShot->Init(position, size);

	return pStraightShot;
}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void StraightShot::Init(D3DXVECTOR3& position, D3DXVECTOR3& size)
{
	// データの初期化
	Bullet3D::Init(position, size);				// 継承データの初期化
	m_uninitPos       = D3DXVECTOR3(0, 0, 0);	// 終着地点
	m_vecInitToUninit = D3DXVECTOR3(0, 0, 0);	// 発射地点

}


//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void StraightShot::Uninit(void)
{
	Bullet3D::Uninit();
}


//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void StraightShot::Update(void)
{
	// 未使用で処理無し
	if (!Bullet3D::GetIsInstance()) return;

	// 弾の移動
	MovePosition(GetFront() * GetInitSpeed());

	// 兵士、プレイヤー、壁との当たり判定
	if (CollisionSoldier() || CollisionPlayer() || CollisionWall()) {
		// 使用中フラグOFF
		Bullet3D::SetIsInstance(false);
	}
	
	// 消失地点を超えたら消す（内積）
	{
		// 現在地点と消失地点の単位ベクトルを算出
		D3DXVECTOR3 vec = m_uninitPos - GetPosition();
		D3DXVec3Normalize(&vec, &vec);

		// 内積を取り、消失地点を超えていたらバレットを消す
		float result = D3DXVec3Dot(&vec, &m_vecInitToUninit);
		if (result <= 0) Bullet3D::SetIsInstance(false);
	}

}


//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void StraightShot::Draw(void)
{
	Bullet3D::Draw();

}


//	SSの発生
void StraightShot::SetStraightShot(Player& player)
{
	// 実体持ちを探す
	StraightShot* pStraightShot = (StraightShot*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_3D_STRAIGHTSHOT);

	// １つも作られていなかったら生成する
	if (pStraightShot == nullptr) {
		// 生成処理
		pStraightShot = StraightShot::Create(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(10, 10, 0));

		// 設定処理
		pStraightShot->SetStraightShot_Private(player);

		return;
	}

	// 作られていたら未使用のSSを探す
	StraightShot* pCurrent = (StraightShot*)pStraightShot;
	StraightShot* pNext = (StraightShot*)pStraightShot->GetNextPointer();
	for (;;) {
		// 未使用なら設定して終了
		if (!pCurrent->GetIsInstance()) {
			pCurrent->SetStraightShot_Private(player);
			break;
		}

		// 未使用SSが見つからなければ、新しく生成する
		if (pNext == nullptr) {
			// 生成処理
			pNext = StraightShot::Create(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(10, 10, 0));

			// 設定処理
			pNext->SetStraightShot_Private(player);

			break;
		}

		// ポインタをずらす
		pCurrent = pNext;
		pNext = (StraightShot*)pCurrent->GetNextPointer();

	}

}


//=============================================================================
//	private methods
//	SSの設定
void StraightShot::SetStraightShot_Private(Player& player)
{
	// 座標の設定
	this->SetPosition(player.GetPosition());

	// 所属陣営の設定
	this->SetGroup(player.GetGroup());

	// 向きの設定
	this->SetFront(player.GetFront());

	// 初速度の設定
	this->SetInitSpeed(player.GetStraightShotSpeed());

	// 加速度の設定
	this->SetAccelerate(0);

	// 消失地点の設定
	m_uninitPos = GetPosition() + player.GetFront() * player.GetStraightShotRange();

	// 消失用の単位ベクトルの設定（内積で消失）
	m_vecInitToUninit = m_uninitPos - GetPosition();
	D3DXVec3Normalize(&m_vecInitToUninit, &m_vecInitToUninit);

	// 使用フラグON
	this->SetIsInstance(true);

	// 親の設定
	m_pParent = &player;

	// テクスチャの設定
	SetTexture(MainGame::GetTexture(List_LoadTexture_MainGame::TEXTURE_NAME::BULLET_TEXNAME)->GetTexture());

}

//	兵士との当たり判定処理
bool StraightShot::CollisionSoldier()
{
	Soldier* pSoldier = (Soldier*)Object::GetLDATA_HEAD(TYPE_MODEL_SOLDIER);

	if (pSoldier == nullptr) return false;

	Soldier* pCurrent = pSoldier;
	Soldier* pNext = (Soldier*)pSoldier->GetNextPointer();

	for (;;) {

		// 陣営の判定
		if (pCurrent->GetInstance() && pCurrent->GetGroup() != GetGroup())

			// 当たり判定
			if (Collision_SphereToSphere(GetPosition(), GetRadius(), pCurrent->GetPosition(), pCurrent->GetRadius())) {

				// 攻撃処理
				pCurrent->Attack(m_pParent->GetStraightShotDamage(), GetFront(), m_pParent->GetStraightShotSpeed(), m_pParent);

				return true;
			}

		if (pNext == nullptr) return false;

		pCurrent = pNext;
		pNext = (Soldier*)pCurrent->GetNextPointer();

	}

}

//	プレイヤーとの当たり判定処理
bool StraightShot::CollisionPlayer()
{
	Player* pPlayer = (Player*)Object::GetLDATA_HEAD(TYPE_MODEL_PLAYER);

	if (pPlayer == nullptr) return false;

	Player* pCurrent = pPlayer;
	Player* pNext = (Player*)pPlayer->GetNextPointer();

	for (;;) {

		if (pCurrent->GetGroup() != GetGroup())
			if (Collision_SphereToSphere(GetPosition(), GetRadius(), pCurrent->GetPosition(), pCurrent->GetRadius()))
				return true;

		pCurrent = pNext;

		if (pCurrent == nullptr) 
			return false;

		pNext = (Player*)pCurrent->GetNextPointer();

	}

}

//	壁との当たり判定処理
bool StraightShot::CollisionWall()
{
	// ※ 未生成

	return false;
}

