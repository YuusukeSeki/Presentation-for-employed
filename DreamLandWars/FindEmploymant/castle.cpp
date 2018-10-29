//*****************************************************************************
//
//		城
//													Autohr : Yusuke Seki
//*****************************************************************************
#include "castle.h"

#include "MainGame.h"	// 自身の座標設定のため
#include "field.h"		// 自身の座標設定のため
#include "Wall.h"		// 自身の座標設定のため

#include "Hold.h"		// 殴れる範囲を可視化するため
#include "player.h"		// 殴れる範囲を可視化するため

#include "collision.h"	// 3D当たり判定用の便利関数

#include "Commander.h"	// 兵士生成のため

#include "TeamGaugeManager.h"	// チームゲージに影響を与えるため
#include "TeamGauge.h"			// チームゲージに影響を与えるため

#include "Etc_Paramaters.h"



//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
Castle::Castle() : ObjectModel(Object::TYPE::TYPE_MODEL_CASTLE)
{
	// メンバ変数初期化
	m_browRange = 0.f;	// 殴れる範囲
	m_pHold = nullptr;	// 殴れる範囲のGUI
	m_frameCounter = 0;	// 兵士生成用フレームカウンター
	vector_LEFT  = D3DXVECTOR3(0, 0, 0);	// 兵士生成時の初期目標座標
	vector_RIGHT = D3DXVECTOR3(0, 0, 0);	// 兵士生成時の初期目標座標

}


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
Castle::Castle(Object::TYPE type) : ObjectModel(type)
{
	// メンバ変数初期化
	m_browRange = 0.f;	// 殴れる範囲
	m_pHold = nullptr;	// 殴れる範囲のGUI
	m_frameCounter = 0;	// 兵士生成用フレームカウンター
	vector_LEFT = D3DXVECTOR3(0, 0, 0);	// 兵士生成時の初期目標座標
	vector_RIGHT = D3DXVECTOR3(0, 0, 0);	// 兵士生成時の初期目標座標

}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
Castle::~Castle()
{
	Uninit();

}


//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
Castle* Castle::Create(const char* FileName, Object::GROUP group)
{
	Castle* pCastle = new Castle(Object::TYPE::TYPE_MODEL_CASTLE);
	pCastle->Init(FileName, group);

	return pCastle;
}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void Castle::Init(const char* FileName, Object::GROUP group)
{
	// 継承データの初期化
	ObjectModel::Init(D3DXVECTOR3(0, 0, 0), FileName);

	// グループの設定
	SetGroup(group);

	// グループ判定でデータを修正
	// ※　修正対象：座標、色、向き、兵士の初期目標座標
	{
		Field* pField = MainGame::GetField();
		Wall* pWall = MainGame::GetWall(3);
		D3DXVECTOR3 length(0, 0, pField->GetPosition().z + pField->GetHalfSize().z - pWall->GetHalfSize().z * 2 - GetHalfSize().z);
		D3DXVECTOR3 setPos(pField->GetPosition().x, 0, 0);
		vector_LEFT  = D3DXVECTOR3(-1, 0, 1);
		vector_RIGHT = D3DXVECTOR3(1, 0, 1);

		// Side Your Team
		if (GetGroup() == Object::GROUP_A) {
			SetPosition(setPos - length);
			SetColor(0xff0000a0);
			SetFront(D3DXVECTOR3(0, 0, 1));

		}

		// Side Enemy
		else if(GetGroup() == Object::GROUP_B) {
			SetPosition(setPos + length);
			SetColor(0x0000ffa0);
			SetRotate(D3DXVECTOR3(0, D3DXToRadian(180), 0));
			SetFront(D3DXVECTOR3(0, 0, -1));
			vector_LEFT  *= -1;
			vector_RIGHT *= -1;

		}
		// Error Command
		else {
			_MSGERROR("Castle cannot be 'GROUP_NONE'!!", "Error!!");
			Release();

			return;
		}

	}

	// 殴れる範囲の設定
	m_browRange = GetSize().x * 1.05f;

	// 殴れる範囲のGUI生成
	m_pHold = Hold::Create(this, MainGame::GetCamera(0));

	// 兵士生成用カウンターの初期値設定
	m_frameCounter = 1500;

}


//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void Castle::Uninit(void)
{
	// 継承データの終了処理
	ObjectModel::Uninit();

}


//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void Castle::Update(void)
{
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

	// 殴れる範囲の可視化判定
	{
		// プレイヤーリストの先頭データを取得
		Player* pCurrent = (Player*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_MODEL_PLAYER);

		// Error Command
		if (pCurrent == nullptr)
			return;

		// 敵キャストと自身との距離が規定値以内で、殴れる状態を可視化する
		for (;;) {

			// 敵キャストかどうかを判定
			if (pCurrent->GetGroup() != GetGroup()) {

				// 距離が規定値以内なら殴れる状態であることを可視化する
				if (Distance3D(pCurrent->GetPosition(), GetPosition()) <= m_browRange * m_browRange) {
					m_pHold->SetDrawHold(true);
				}

				// 範囲外なら非表示
				else {
					m_pHold->SetDrawHold(false);
				}

			}

			// 次のキャストへ
			pCurrent = (Player*)pCurrent->GetNextPointer();

			// 次のキャストがいなければ判定終了
			if (pCurrent == nullptr) {
				break;
			}

		}

	}


}


//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void Castle::Draw(void)
{
	// 描画処理は親元に丸投げ
	ObjectModel::Draw();

}


// ダメージを与える
// breakPower : 与えるダメージ
void Castle::BrowCastle(float breakPower)
{
	TeamGaugeManager::GetTeamGauge_ENEMY()->MoveLife(-breakPower * CASTLE_RETOUCH_BREAKPOWER);

}

// 殴れる範囲との当たり判定
// position : 対象キャストの位置
// 【返り値】	true  : 殴れる
//				false : 殴れない
bool Castle::CollisionBrowRange(D3DXVECTOR3& position)
{
	if (Distance3D(GetPosition(), position) <= m_browRange * m_browRange)
		return true;

	return false;

}


