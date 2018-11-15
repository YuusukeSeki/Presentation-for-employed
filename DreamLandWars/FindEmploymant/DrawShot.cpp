//*****************************************************************************
//	
//		ドローショット
//													Autohr : Yusuke Seki
//*****************************************************************************
// base
#include "DrawShot.h"
#include "DrawLine.h"
#include "player.h"

// fw
#include "collision.h"
#include "camera.h"

// resource
#include "MainGame.h"
#include "list_LoadTexture_MainGame.h"

// effect
#include "EffectBillboard.h"

// object
#include "Soldier.h"
#include "tower.h"


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
DrawShot::DrawShot(Object::TYPE type) : Bullet3D(type)
{
	// データのクリア
	m_pMoveWay      = nullptr;				// 移動経路
	m_vecPrevToNext = D3DXVECTOR3(0, 0, 0);	// 経路毎の単位ベクトル
	m_pCamera       = nullptr;				// カメラ
	m_pPlayer       = nullptr;				// 親プレイヤー

}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
DrawShot::~DrawShot()
{
	Uninit();
}


//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
DrawShot* DrawShot::Create(D3DXVECTOR3& position, D3DXVECTOR3& size)
{
	// 実体を生成
	DrawShot* pDrawShot = new DrawShot(Object::TYPE::TYPE_3D_DRAWSHOT);

	// 初期化
	pDrawShot->Init(position, size);

	return pDrawShot;
}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void DrawShot::Init(D3DXVECTOR3& position, D3DXVECTOR3& size)
{
	// データの初期化
	Bullet3D::Init(position, size);							// 継承データ
	m_pMoveWay      = nullptr;								// 移動経路
	m_vecPrevToNext = D3DXVECTOR3(0, 0, 0);					// 経路毎の単位ベクトル
	m_pCamera       = nullptr;								// カメラ
	m_pPlayer       = nullptr;								// 親プレイヤー

	ClearCollisionID();	// 当たった兵士の初期化

}


//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void DrawShot::Uninit(void)
{
	Bullet3D::Uninit();

}


//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void DrawShot::Update(void)
{
	// 【更新判定】
	if (GetIsInstance() == false) return;	// 未使用で処理無し

	//移動
	{
		D3DXVECTOR3 move, position, vec;
		float result;

		// 移動量の算出
		move     = GetFront() * GetInitSpeed();
		position = GetPosition() + move;
		vec      = m_pMoveWay->GetNextPointer_DrawLine()->GetPosition() - GetPosition();

		// 目標地点を超えていないかの判定
		D3DXVec3Normalize(&vec, &vec);
		result = D3DXVec3Dot(&vec, &m_vecPrevToNext);
		if (result <= 0) { // 超えている

			// 移動量の修正
			move = m_pMoveWay->GetNextPointer_DrawLine()->GetPosition() - GetPosition();

			// 経路を次の地点へ変更
			{
				DrawLine* pNextWay;
				D3DXVECTOR3 currentPos, nextPos, vec;

				// 現在のポインターを次のポインターに変更
				pNextWay   = m_pMoveWay->GetNextPointer_DrawLine();
				m_pMoveWay->ClearParameter();
				m_pMoveWay = pNextWay;

				// 次の地点が無ければDS終了
				if (m_pMoveWay->GetNextPointer_DrawLine() == nullptr) {
					m_pMoveWay->ClearParameter();
					ClearCollisionID();
					SetIsInstance(false);
					return;
				}

				// 弾の向きを算出
				currentPos = m_pMoveWay->GetPosition();
				nextPos    = m_pMoveWay->GetNextPointer_DrawLine()->GetPosition();
				vec        = nextPos - currentPos;
				D3DXVec3Normalize(&vec, &vec);
				
				// 弾の向きを更新
				SetFront(vec);
				m_vecPrevToNext = vec;

			}

		}// End.. 超過判定

		// 移動
		MovePosition(move);

		// エフェクトの発生
		Effect3D::SetEffect(GetPosition(), GetFront(), EffectBillboard::TYPE::TYPE_FIRE);
		Effect3D::SetEffect(GetPosition(), GetFront(), EffectBillboard::TYPE::TYPE_FIRE);
		//EffectBillboard::SetEffect(GetPosition(), GetVecZ(), MainGame::GetCamera(0), EffectBillboard::TYPE::TYPE_FIRE);
		//EffectBillboard::SetEffect(GetPosition(), GetVecZ(), MainGame::GetCamera(0), EffectBillboard::TYPE::TYPE_FIRE);

	}

	// 兵士との当たり判定
	CollisionSoldier();

	// 塔との当たり判定
	if (Collision_Tower()) {
		DrawLine::ClearParameter_List(m_pMoveWay);
		ClearCollisionID();
		SetIsInstance(false);
	}

}


//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void DrawShot::Draw(void)
{
	// 未使用で処理無し
	if (!Bullet3D::GetIsInstance()) return;


	// 描くだけ
	Bullet3D::Draw();
}


//=============================================================================
//	設定処理
//	DSの発生
void DrawShot::SetDrawShot(DrawLine* pStartPoint)
{
	// 始点のNULLチェック
	if (pStartPoint == nullptr) {
		// エラーメッセージ
		_MSGERROR("pStartPoint cannnot be nullptr!!", "void DrawShot::SetDrawShot(DrawLine* pStartPoint)");
	}


	// 実体持ちを探す
	DrawShot* pDrawShot = (DrawShot*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_3D_DRAWSHOT);

	// １つも作られていなかったら生成する
	if (pDrawShot == nullptr) {
		// 生成処理
		pDrawShot = DrawShot::Create(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(5, 5, 0));

		// 設定処理
		pDrawShot->SetDrawShot_private(pStartPoint);

		return;
	}

	// 作られていたら未使用領域を探す
	DrawShot* pCurrent = (DrawShot*)pDrawShot;
	DrawShot* pNext    = (DrawShot*)pDrawShot->GetNextPointer();
	for (;;) {
		// 未使用なら設定して終了
		if (!pCurrent->GetIsInstance()) {
			pCurrent->SetDrawShot_private(pStartPoint);
			break;
		}

		// 未使用領域が見つからなければ、新しく生成する
		if (pNext == nullptr) {
			// 生成処理
			pNext = DrawShot::Create(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(5, 5, 0));

			// 設定処理
			pNext->SetDrawShot_private(pStartPoint);

			break;
		}

		// ポインタをずらす
		pCurrent = pNext;
		pNext    = (DrawShot*)pCurrent->GetNextPointer();

	}

}


//=============================================================================
//	private関数
// 兵士との当たり判定
void DrawShot::CollisionSoldier()
{
	Soldier* pSoldier = (Soldier*)Object::GetLDATA_HEAD(TYPE_MODEL_SOLDIER);

	if (pSoldier == nullptr) return;

	Soldier* pCurrent = pSoldier;
	Soldier* pNext = (Soldier*)pSoldier->GetNextPointer();

	for (;;) {

		// 実体があり、自分と違う陣営の兵士かを判定
		if (pCurrent->GetInstance() && pCurrent->GetGroup() != GetGroup())
			
			// 当たってる？
			if (Collision_SphereToSphere(GetPosition(), GetRadius(), pCurrent->GetPosition(), pCurrent->GetRadius())) {

				// まだ当たっていない対象かを判定
				for (int i = 0; i <= m_cntCollisionSoldier; i++) {
					if (m_CollisionID_Soldier[i] == pCurrent->GetID()) {
						// 当たっている
						break;
					}
					else if (m_CollisionID_Soldier[i] == CLEAR_ID) {
						// まだ当たっていない

						// 当たった相手のIDを記憶
						m_CollisionID_Soldier[m_cntCollisionSoldier] = pCurrent->GetID();
						m_cntCollisionSoldier++;

						// ダメージを与える
						pCurrent->Attack(m_pPlayer->GetDrawShotDamage(), m_vecPrevToNext, m_pPlayer->GetDrawShotSpeed(), m_pPlayer);
						//pCurrent->Damage(m_pPlayer->GetDrawShotDamage());

						break;

					}
				}
			}

		pCurrent = pNext;

		if (pCurrent == nullptr) return;

		pNext = (Soldier*)pCurrent->GetNextPointer();

	}

}

// 塔との当たり判定
// 返り値：	true  当たった
//			false 当たってない
bool DrawShot::Collision_Tower()
{
	Tower* pTower = (Tower*)Object::GetLDATA_HEAD(TYPE_MODEL_TOWER);

	if (pTower == nullptr) return false;

	Tower* pCurrent = pTower;
	Tower* pNext = (Tower*)pTower->GetNextPointer();

	for (;;) {

		if (pCurrent->GetGroup() != GetGroup())
			if (Collision_SphereToSphere(GetPosition(), GetRadius(), pCurrent->GetPosition(), pCurrent->GetHalfSize().x)) return true;

		pCurrent = pNext;

		if (pCurrent == nullptr) return false;

		pNext = (Tower*)pCurrent->GetNextPointer();

	}

}

// DSの設定
void DrawShot::SetDrawShot_private(DrawLine* pStartPoint)
{
	// 親プレイヤーの取得
	m_pPlayer = pStartPoint->GetPlayer_DrawLine();

	// 経路の取得
	m_pMoveWay = pStartPoint;

	// 座標の設定
	SetPosition(pStartPoint->GetPosition());

	// 所属陣営の設定
	SetGroup(pStartPoint->GetPlayer_DrawLine()->GetGroup());

	// 向きの設定
	{
		// スタート地点を取得
		D3DXVECTOR3 startPos = m_pMoveWay->GetPosition();

		// 次の目標地点を取得
		D3DXVECTOR3 nextPos = m_pMoveWay->GetNextPointer_DrawLine()->GetPosition();

		// 向きを設定
		D3DXVECTOR3 vec = nextPos - startPos;
		D3DXVec3Normalize(&vec, &vec);
		Bullet3D::SetFront(vec);

		// 次の地点までのベクトルを保存
		m_vecPrevToNext = vec;

	}
	//SetVecZ(pStartPoint->GetPlayer_DrawLine()->GetVecZ());

	// 初速度の設定
	SetInitSpeed(pStartPoint->GetPlayer_DrawLine()->GetDrawShotSpeed());

	// 加速度の設定
	SetAccelerate(0);

	// カメラの取得
	m_pCamera = pStartPoint->GetPlayer_DrawLine()->GetCamera();

	// 使用フラグON
	SetIsInstance(true);

}

// 当たった相手を初期化
void DrawShot::ClearCollisionID()
{
	for (int i = 0; i < NUM_COLLISION_ID_SOLDIER; i++)
	{
		m_CollisionID_Soldier[i] = CLEAR_ID;
	}

	m_cntCollisionSoldier = 0;

}
