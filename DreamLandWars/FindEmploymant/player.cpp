//*****************************************************************************
//
//		プレイヤー
//												Autohr : Yusuke Seki
//*****************************************************************************
#include "player.h"
#include "list_CharaParameters.h"
#include "MainGame.h"
#include "list_LoadTexture_MainGame.h"
#include "ModelData.h"
#include "Part.h"

// 当たり判定
#include "collision.h"
#include "renderer.h"
#include "field.h"

// 入力状況の取得
#include "GameManager.h"
#include "input.h"

// 移動と攻撃
#include "camera.h"
#include "StraightShot.h"
#include "DrawLine.h"

// コリジョン対象
#include "tower.h"
#include "castle.h"
#include "Hold.h"
#include "Wall.h"


#define KEY_MAX		2

Player::KEY_FRAME g_KeyFrameWalk[KEY_MAX] =
{
	// 0 key
	{
		30,	// num frame
		{
			{ D3DXVECTOR3(0.f, 20.f, -100.f), D3DXVECTOR3( 0.54f,  0.09f,  0.00f) },	// BODY
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3(-0.48f, -0.03f,  0.00f) },	// HEAD
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.00f, -0.36f,  0.96f) },	// RARM
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.00f,  0.00f, -0.87f) },	// REDGE
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.00f, -0.60f, -0.03f) },	// LARM
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.00f,  0.60f,  0.57f) },	// LEDGE
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.99f,  0.00f,  0.00f) },	// RLEG
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.60f,  0.00f,  0.00f) },	// REDGE
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3(-1.74f,  0.00f,  0.00f) },	// LLEG
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.51f,  0.00f,  0.00f) },	// LEDGE
		}
	},

	// 1 key
	{
		30,	// num frame
		{
			{ D3DXVECTOR3(0.f, 20.f, -100.f), D3DXVECTOR3( 0.54f, -0.09f,  0.00f) },	// BODY
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3(-0.48f, +0.03f,  0.00f) },	// HEAD
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.00f, +0.36f, -0.96f) },	// RARM
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.00f,  0.00f, +0.87f) },	// REDGE
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.00f, +0.60f, +0.03f) },	// LARM
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3( 0.00f, -0.60f, -0.57f) },	// LEDGE
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3(-0.99f,  0.00f,  0.00f) },	// RLEG
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3(-0.60f,  0.00f,  0.00f) },	// REDGE
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3(+1.74f,  0.00f,  0.00f) },	// LLEG
			{ D3DXVECTOR3(0.f,  0.f, -100.f), D3DXVECTOR3(-0.51f,  0.00f,  0.00f) },	// LEDGE
		}
	},

	//// 2 key
	//{
	//	30,	// num frame
	//	{
	//		{ D3DXVECTOR3(0.f, 10.f, 100.f), D3DXVECTOR3(0.f, 0.f, 0.f) },	// BODY
	//		{ D3DXVECTOR3(0.f, 10.f, 100.f), D3DXVECTOR3(0.f, 0.f, 0.f) },	// HEAD
	//		{ D3DXVECTOR3(0.f, 10.f, 100.f), D3DXVECTOR3(0.f, 0.f, 0.f) },	// RARM
	//		{ D3DXVECTOR3(0.f, 10.f, 100.f), D3DXVECTOR3(0.f, 0.f, 0.f) },	// REDGE
	//		{ D3DXVECTOR3(0.f, 10.f, 100.f), D3DXVECTOR3(0.f, 0.f, 0.f) },	// LARM
	//		{ D3DXVECTOR3(0.f, 10.f, 100.f), D3DXVECTOR3(0.f, 0.f, 0.f) },	// LEDGE
	//		{ D3DXVECTOR3(0.f, 10.f, 100.f), D3DXVECTOR3(0.f, 0.f, 0.f) },	// RLEG
	//		{ D3DXVECTOR3(0.f, 10.f, 100.f), D3DXVECTOR3(0.f, 0.f, 0.f) },	// REDGE
	//		{ D3DXVECTOR3(0.f, 10.f, 100.f), D3DXVECTOR3(0.f, 0.f, 0.f) },	// LLEG
	//		{ D3DXVECTOR3(0.f, 10.f, 100.f), D3DXVECTOR3(0.f, 0.f, 0.f) },	// LEDGE
	//	}
	//},

};


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
Player::Player(Object::TYPE type, Player::CHARACTER character) : ObjectModel(type)
{
	// キャラクターを設定
	m_character = character;

	// データのクリア
	m_life              = 0;					// 体力
	m_movePosition	    = D3DXVECTOR3(0, 0, 0);	// 移動量バッファ
	m_InitialVelosity   = 0.f;					// 初速
	m_Acceleration      = 0.f;					// 加速度
	m_avDist            = 0.f;					// 回避距離
	m_straightShotSpeed = 0.f;					// SSの速度
	m_straightShotRange = 0.f;					// SSの射程
	m_drawShotSpeed     = 0.f;					// DSの速度
	m_drawShotRange     = 0.f;					// DSの射程
	m_drawShotLength    = 0.f;					// DSの描ける長さ
	m_cntCooldownFrame  = 0;					// 硬直時間計測用カウンター
	m_cooldown_Avoid    = 0;					// 【硬直時間】避ける
	m_cooldown_EmAvoid  = 0;					// 【硬直時間】緊急回避
	m_cooldown_StShot   = 0;					// 【硬直時間】SS攻撃
	m_cooldown_DrShot   = 0;					// 【硬直時間】DS攻撃
	m_maxLife           = 0;					// 【最大値】体力
	m_maxMoveSpeed      = 0.f;					// 【最大値】速度
	m_pDL_StartPos      = nullptr;				// 描画線の始点のバッファ
	m_DL_Length         = 0;					// 描画線の長さのバッファ
	m_DL_PrePos         = D3DXVECTOR3(0, 0, 0);	// 描画線の前座標のバッファ
	m_DL_PreLength      = 0;					// 描画線の更新前の長さのバッファ
	m_pCamera           = nullptr;				// 追従してくるカメラ

	// スコア
	_score_crushSoldier = 0;	// 兵士撃破数

	// パラメータ参照
	List_CharacterParameters::InitParameter(this);



}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
Player::~Player()
{
	// 継承データの終了処理
	ObjectModel::Uninit();

}


//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
Player* Player::Create(D3DXVECTOR3& position, Camera* pCamera, const char* modelPass, Player::CHARACTER character)
{
	// 実体の生成
	Player* pPlayer = new Player(Object::TYPE_MODEL_PLAYER, character);

	// 初期化
	pPlayer->Init(position, pCamera, modelPass);

	// 生成したポインタを返す
	return pPlayer;

}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void Player::Init(D3DXVECTOR3& position, Camera* pCamera, const char* modelPass)
{
	// 継承データの初期化処理
	ObjectModel::Init(position, modelPass);

	// DS
	m_pDL_StartPos = nullptr;				// 描画線の始点のバッファ
	m_DL_Length    = 0;						// 描画線の長さのバッファ
	m_DL_PrePos    = D3DXVECTOR3(0, 0, 0);	// 描画線の前座標のバッファ
	m_DL_PreLength = 0;						// 描画線の更新前の長さのバッファ

	// 追従してくるカメラの取得
	m_pCamera = pCamera;


	currentFront = 0;
	targetFront = currentFront;



	m_Part[0] = Part::Create(D3DXVECTOR3(0, 0, 0), MODELDATA_BODY);
	m_Part[1] = Part::Create(D3DXVECTOR3(0, 0, 0), MODELDATA_HEAD);
	m_Part[2] = Part::Create(D3DXVECTOR3(0, 0, 0), MODELDATA_RARM);
	m_Part[3] = Part::Create(D3DXVECTOR3(0, 0, 0), MODELDATA_RARM_EDGE);
	m_Part[4] = Part::Create(D3DXVECTOR3(0, 0, 0), MODELDATA_LARM);
	m_Part[5] = Part::Create(D3DXVECTOR3(0, 0, 0), MODELDATA_LARM_EDGE);
	m_Part[6] = Part::Create(D3DXVECTOR3(0, 0, 0), MODELDATA_RLEG);
	m_Part[7] = Part::Create(D3DXVECTOR3(0, 0, 0), MODELDATA_RLEG_EDGE);
	m_Part[8] = Part::Create(D3DXVECTOR3(0, 0, 0), MODELDATA_LLEG);
	m_Part[9] = Part::Create(D3DXVECTOR3(0, 0, 0), MODELDATA_LLEG_EDGE);

	m_Part[0]->SetParent(nullptr);		// BODY
	m_Part[1]->SetParent(m_Part[0]);	// HEAD
	m_Part[2]->SetParent(m_Part[0]);	// RARM
	m_Part[3]->SetParent(m_Part[2]);	// REDGE
	m_Part[4]->SetParent(m_Part[0]);	// LARM
	m_Part[5]->SetParent(m_Part[4]);	// LEDGE
	m_Part[6]->SetParent(m_Part[0]);	// RLEG
	m_Part[7]->SetParent(m_Part[6]);	// REDGE
	m_Part[8]->SetParent(m_Part[0]);	// LLEG
	m_Part[9]->SetParent(m_Part[8]);	// LEDGE

	// BODY
	D3DXVECTOR3 pos = D3DXVECTOR3(0, 20, 0);
	//D3DXVECTOR3 rotate = D3DXVECTOR3(-0.21f, 3.1415f, 0);
	D3DXVECTOR3 rotate = D3DXVECTOR3(0, 0, 0);
	m_Part[0]->SetPosition(pos);
	m_Part[0]->SetRotate(rotate);

	// HEAD
	pos = D3DXVECTOR3(0, m_Part[1]->GetParent()->GetSize().y * 0.75f, m_Part[1]->GetHalfSize().z * 0.5f);
	m_Part[1]->SetPosition(pos);

	// RARM
	pos = D3DXVECTOR3(m_Part[2]->GetParent()->GetHalfSize().x, m_Part[2]->GetParent()->GetHalfSize().y, 0);
	//rotate = D3DXVECTOR3(0, 0, 0.81f);
	m_Part[2]->SetPosition(pos);
	m_Part[2]->SetRotate(rotate);

	// REDGE
	pos = D3DXVECTOR3(m_Part[3]->GetParent()->GetSize().x, 0, 0);
	//rotate = D3DXVECTOR3(0, 0, 1.68f);
	m_Part[3]->SetPosition(pos);
	m_Part[3]->SetRotate(rotate);

	// LARM
	pos = D3DXVECTOR3(-m_Part[4]->GetParent()->GetHalfSize().x, m_Part[4]->GetParent()->GetHalfSize().y, 0);
	//rotate = D3DXVECTOR3(0, 0, 0.9f);
	m_Part[4]->SetPosition(pos);
	m_Part[4]->SetRotate(rotate);

	// LEDGE
	pos = D3DXVECTOR3(-m_Part[5]->GetParent()->GetSize().x, 0, 0);
	//rotate = D3DXVECTOR3(0, 0, 1.56f);
	m_Part[5]->SetPosition(pos);
	m_Part[5]->SetRotate(rotate);

	// RLEG
	pos = D3DXVECTOR3(m_Part[6]->GetParent()->GetHalfSize().x, -m_Part[6]->GetParent()->GetHalfSize().y * 0.5f, 0);
	//rotate = D3DXVECTOR3(0, 0, 0.33f);
	m_Part[6]->SetPosition(pos);
	m_Part[6]->SetRotate(rotate);

	// REDGE
	pos = D3DXVECTOR3(0, -m_Part[7]->GetParent()->GetSize().y, 0);
	m_Part[7]->SetPosition(pos);

	// LLEG
	pos = D3DXVECTOR3(-m_Part[8]->GetParent()->GetHalfSize().x, -m_Part[8]->GetParent()->GetHalfSize().y * 0.5f, 0);
	//rotate = D3DXVECTOR3(0, 0, -0.15f);
	m_Part[8]->SetPosition(pos);
	m_Part[8]->SetRotate(rotate);

	// LEDGE
	pos = D3DXVECTOR3(0, -m_Part[9]->GetParent()->GetSize().y, 0);
	m_Part[9]->SetPosition(pos);


	// Initialize Key Frame
	g_KeyFrameWalk[0].Frame = 20;
	g_KeyFrameWalk[1].Frame = 20;

	for (int j = 0; j < KEY_MAX; j++) {
		for (int i = 0; i < 10; i++) {
			g_KeyFrameWalk[j].Key[i].Position = m_Part[i]->GetPosition();
			//g_KeyFrameWalk[j].Key[i].Rotation = m_Part[i]->GetRotate();
		}
	}

	m_KeyFrame = g_KeyFrameWalk;
	m_Key = 0;
	m_Frame = 0;

}


//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void Player::Uninit(void)
{
	// 継承データの終了処理
	ObjectModel::Uninit();

}


//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void Player::Update(void)
{
	TempInput();

	// 逆行列の更新
	UpdateInverseMatrix();

	// 座標の移動
	ObjectModel::MovePosition(m_movePosition);

	// 向き求めちゃうよ～
	D3DXVECTOR3 targetVector = GetFront();
	D3DXVECTOR3 targetPosition = GetPosition();

	// ステート遷移
	switch (m_posture) {
		case POSTURE_NONE:
		{
			// 向きの変更
			if (m_movePosition != D3DXVECTOR3(0, 0, 0)) SetRotateToObj(m_movePosition + this->GetPosition());

			SetColor(0xffffffff); // 白
			break;
		}

		case POSTURE_RUN:
		{
			// 向きの変更
			//if (m_movePosition != D3DXVECTOR3(0, 0, 0)) SetRotateToObj(m_movePosition + this->GetPosition());

			D3DXVECTOR3 nowPos = GetPosition();
			targetPosition = nowPos + m_movePosition;
			//targetPosition.y = 0.f;

			targetVector = targetPosition - nowPos;
			
			D3DXVec3Normalize(&targetVector, &targetVector);

			//D3DXVec3Normalize(&targetPosition, &targetPosition);
			//targetFront = atan2f(targetPosition.x, targetPosition.z);

			// 現在の角度と目的の角度の差分を求める
			float cos_sita = D3DXVec3Dot(&targetVector, &GetFront());
			float sita = acosf(cos_sita);

			// 回転方向を求める
			float c0 = GetFront().x * targetVector.z - GetFront().z * targetVector.x;
			if (c0 > 0)
				sita *= -1;

			// 回転させる
			SetRotate(D3DXVECTOR3(0, sita, 0));
			//SetFront(targetVector);

			OX::DebugFont::print(450, 300, 0xff00ff00, "cos_sita : %f", cos_sita);
			OX::DebugFont::print(450, 320, 0xff00ff00, "sita     : %f", sita);
			OX::DebugFont::print(450, 340, 0xff00ff00, "tagFront x:%f y:%f z:%f", targetVector.x, targetVector.y, targetVector.z);
			OX::DebugFont::print(450, 360, 0xff00ff00, "nowFront x:%f y:%f z:%f", GetFront().x, GetFront().y, GetFront().z);
			OX::DebugFont::print(450, 380, 0xff00ff00, "tagPos   x:%f y:%f z:%f", targetPosition.x, targetPosition.y, targetPosition.z);
			OX::DebugFont::print(450, 400, 0xff00ff00, "noePos   x:%f y:%f z:%f", GetPosition().x, GetPosition().y, GetPosition().z);


			SetColor(0x00ff00ff); // 緑

			// 移動量に慣性を掛ける
			m_movePosition *= 0.0f;
			if (fabs(m_movePosition.x) < 0.05f && fabs(m_movePosition.z) < 0.05f) {
				m_movePosition.x = 0;
				m_movePosition.z = 0;
				if (m_posture == POSTURE_RUN) m_posture = POSTURE_NONE;
			}

			break;

		}

		case POSTURE_AVOID:
		{
			// 向きの変更
			if (m_movePosition != D3DXVECTOR3(0, 0, 0)) SetRotateToObj(m_movePosition + this->GetPosition());

			// 硬直時間の発生
			m_cntCooldownFrame--;
			if (m_cntCooldownFrame == 0) m_posture = POSTURE_NONE;

			SetColor(0xffff00ff); // 黄

			// 移動量に慣性を掛ける
			m_movePosition *= INIRTIA;
			if (fabs(m_movePosition.x) < 0.05f && fabs(m_movePosition.z) < 0.05f) {
				m_movePosition.x = 0;
				m_movePosition.z = 0;
				if (m_posture == POSTURE_RUN) m_posture = POSTURE_NONE;
			}

			break;

		}

		case POSTURE_EMAVOID:
		{
			// 向きの変更
			//if (m_movePosition != D3DXVECTOR3(0, 0, 0)) SetRotateToObj(m_movePosition + this->GetPosition());


			// 硬直時間の発生
			m_cntCooldownFrame--;
			if (m_cntCooldownFrame == 0) m_posture = POSTURE_NONE;

			SetColor(0x000000ff); // 黒

								  // 移動量に慣性を掛ける
			m_movePosition *= INIRTIA;
			if (fabs(m_movePosition.x) < 0.05f && fabs(m_movePosition.z) < 0.05f) {
				m_movePosition.x = 0;
				m_movePosition.z = 0;
				if (m_posture == POSTURE_RUN) m_posture = POSTURE_NONE;
			}

			break;

		}

		case POSTURE_BACKWORD:
			break;

		case POSTURE_TUMBLE:
			break;

		case POSTURE_DEATH:
			break;

		case POSTURE_REVIVE:
			break;

		case POSTURE_STREADY:
		{
			// 止まる
			m_movePosition = D3DXVECTOR3(0, 0, 0);

			SetColor(0xff00ffff); // 紫
			break;

		}

		case POSTURE_STSHOT:
		{
			// 硬直時間の発生
			m_cntCooldownFrame--;
			if (m_cntCooldownFrame == 0) m_posture = POSTURE_NONE;

			SetColor(0xff0000ff); // 赤
			break;

		}

		case POSTURE_DRREADY:
		{
			// 止まる
			m_movePosition = D3DXVECTOR3(0, 0, 0);

			SetColor(0x00ffffff); // 
			break;

		}

		case POSTURE_DRSHOT:
		{
			// 止まる
			m_movePosition = D3DXVECTOR3(0, 0, 0);

			// 硬直時間の発生
			m_cntCooldownFrame--;
			if (m_cntCooldownFrame == 0) m_posture = POSTURE_NONE;

			SetColor(0x0000ffff); // 
			break;

		}

		case POSTURE_BREAK:
		{
			// 止まる
			m_movePosition = D3DXVECTOR3(0, 0, 0);

			// 硬直時間の発生
			m_cntCooldownFrame--;
			if (m_cntCooldownFrame == 0) m_posture = POSTURE_NONE;

			SetColor(0x808080ff); // 
			break;

		}

	}

	// 塔との当たり判定
	CollisionTower();
	
	// 城との当たり判定
	CollisionCastle();

	// 壁との当たり判定
	CollisionWall();

	// フィールドとの当たり判定
	Field* pField = MainGame::GetField();

	// 座標を地面の上に戻す
	SetPosition(D3DXVECTOR3(GetPosition().x, pField->GetHeight(GetPosition()), GetPosition().z));

	// Save Present Position
	m_prePosion = GetPosition();


	for (int i = 0; i < 10; i++)
	{
		float rate = (float)m_Frame / m_KeyFrame[m_Key].Frame;
		D3DXVECTOR3 curPos  = m_KeyFrame[m_Key].Key[i].Position;

		D3DXVECTOR3 nextPos = m_KeyFrame[m_Key + 1 >= KEY_MAX ? 0 : m_Key + 1].Key[i].Position;
		D3DXVECTOR3 curRot  = m_KeyFrame[m_Key].Key[i].Rotation;
		D3DXVECTOR3 nextRot = m_KeyFrame[m_Key + 1 >= KEY_MAX ? 0 : m_Key + 1].Key[i].Rotation;

		m_Part[i]->SetPosition(curPos * (1.f - rate) + nextPos * rate);
		m_Part[i]->SetRotate(curRot * (1.f - rate) + nextRot * rate);

	}

	m_Frame++;

	if (m_Frame >= m_KeyFrame[m_Key].Frame)
	{
		m_Key = m_Key + 1 >= KEY_MAX ? 0 : m_Key + 1;
		m_Frame = 0;
	}

	
	//// 現在の角度と目的の角度の差分を求める
	//float cos_sita = D3DXVec3Dot(&targetVector, &GetFront());
	//float sita = acosf(cos_sita);

	//// 回転方向を求める
	//float c0 = GetFront().x * targetVector.z - GetFront().z * targetVector.x;
	//if (c0 > 0)
	//	sita *= -1;

	//// 回転させる
	//SetRotate(D3DXVECTOR3(0, sita, 0));
	//SetFront(targetVector);

	//OX::DebugFont::print(500, 300, 0xff00ff00, "cos_sita   : %f", cos_sita);
	//OX::DebugFont::print(500, 320, 0xff00ff00, "sita       : %f", sita);
	//OX::DebugFont::print(450, 340, 0xff00ff00, "tagFront x:%f y:%f z:%f", targetVector.x, targetVector.y, targetVector.z);
	//OX::DebugFont::print(450, 360, 0xff00ff00, "nowFront x:%f y:%f z:%f", GetFront().x, GetFront().y, GetFront().z);

	// 180度を越えているかをチェックする
	


	//SetFront(targetVector);

	//// 
	//if (a > D3DX_PI) {
	//	currentFront -= a * 0.1f;
	//}
	//else {
	//	currentFront += a * 0.1f;
	//}
	
	//if (a > 0 && a <= D3DX_PI) {
	//	// 時計回り
	//	currentFront += a * 0.1f;
	//	//currentFront += (targetFront - currentFront) * 0.1f;
	//	
	//}
	//else if (a < 0 && a >= -D3DX_PI){
	//	// 反時計回り
	//	currentFront -= a * 0.1f;
	//	//currentFront += (targetFront + currentFront) * 0.1f;
	//	
	//}
	//else {
	//	currentFront = targetFront;
	//}

	//SetRotate(D3DXVECTOR3(0, currentFront, 0));


	// 回転を試行錯誤した残骸（失敗に終わる）
	//float a = fabs(targetFront - currentFront);

	//if (fabs(targetFront - currentFront) > 0.01f )
	//{
	//	float b = fabs(targetFront) + fabs(currentFront);

	//	float c = sinf(targetFront) * D3DX_PI;
	//	float d = sinf(currentFront) * D3DX_PI;
	//	float e = c - d;

	//	//if ((fabs(targetFront) - fabs(currentFront)) <= D3DX_PI)
	//	if (fabs(targetFront - currentFront) <= D3DX_PI)
	//	{
	//		//currentFront += b * 0.1f;
	//		currentFront += (targetFront - currentFront) * 0.1f;

	//	}
	//	else
	//	{
	//		b = D3DX_PI * 2 - b;

	//		//currentFront -= b * 0.1f;
	//		currentFront += (targetFront + currentFront) * 0.1f;

	//	}

	//	SetRotate(D3DXVECTOR3(0, currentFront, 0));

	//	if (currentFront > D3DX_PI || currentFront <= -D3DX_PI)
	//	{
	//		currentFront *= -1;
	//	}

	//}
	//else
	//{
	//	currentFront = targetFront;
	//	SetRotate(D3DXVECTOR3(0, currentFront, 0));
	//}

	//static unsigned int cnt;
	//cnt++;

	//static float bufCur, bufTag, bufSab;
	//
	//static float sub;
	//sub = targetFront - currentFront;

	//if (cnt / 60 == 1) {
	//	bufCur = currentFront;
	//	bufTag = targetFront;
	//	bufSab = sub;
	//
	//	cnt = 0;
	//}

	//OX::DebugFont::print(500,   0, 0xffffffff, "currentFront : %f", currentFront);
	//OX::DebugFont::print(500,  20, 0xffffffff, "targetFront  : %f", targetFront);
	//OX::DebugFont::print(500,  40, 0xffffffff, "sabun        : %f", sub);
	//OX::DebugFont::print(500,  60, 0xffffffff, "bufCur       : %f", bufCur);
	//OX::DebugFont::print(500,  80, 0xffffffff, "bufTag       : %f", bufTag);
	//OX::DebugFont::print(500, 100, 0xffffffff, "bufSab       : %f", bufSab);

}


//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void Player::Draw(void)
{
	// 描画処理
	ObjectModel::Draw();

}



void Player::TempInput()
{
	Input *pInput = GameManager::GetInput();
	static int iterator_part, iterator_rotate;
	static char str[3] = {'X', 'Y', 'Z'};
	float addValue = 0.03f;

	if (pInput->GetKeyboardTrigger(DIK_UP))
	{
		iterator_part = iterator_part - 1 < 0 ? 9 : iterator_part - 1;
	}
	if (pInput->GetKeyboardTrigger(DIK_DOWN))
	{
		iterator_part = iterator_part + 1 > 9 ? 0 : iterator_part + 1;
	}

	D3DXVECTOR3 rotate;
	rotate = m_Part[iterator_part]->GetRotate();


	if (pInput->GetKeyboardRepeat(DIK_LEFT))
	{
		if (iterator_part >= 0 && iterator_part <= 9) {
			switch (iterator_rotate) {
				case 0:
					rotate.x -= addValue;
					break;
				case 1:
					rotate.y -= addValue;
					break;
				case 2:
					rotate.z -= addValue;
					break;
			}
		}
	}
	if (pInput->GetKeyboardRepeat(DIK_RIGHT))
	{
		if (iterator_part >= 0 && iterator_part <= 9) {
			switch (iterator_rotate) {
			case 0:
				rotate.x += addValue;
				break;
			case 1:
				rotate.y += addValue;
				break;
			case 2:
				rotate.z += addValue;
				break;
			}
		}
	}

	if (pInput->GetKeyboardTrigger(DIK_X))
		iterator_rotate = 0;
	if (pInput->GetKeyboardTrigger(DIK_Y))
		iterator_rotate = 1;
	if (pInput->GetKeyboardTrigger(DIK_Z))
		iterator_rotate = 2;


	m_Part[iterator_part]->SetRotate(rotate);

	OX::DebugFont::print(400, 20* 0, 0xff00ff00, "select rotate = %c", str[iterator_rotate]);
	OX::DebugFont::print(400, 20* 1, 0xff00ff00, "BODY _ X:%f, Y:%f, Z:%f", m_Part[0]->GetRotate().x, m_Part[0]->GetRotate().y, m_Part[0]->GetRotate().z);
	OX::DebugFont::print(400, 20* 2, 0xff00ff00, "HEAD _ X:%f, Y:%f, Z:%f", m_Part[1]->GetRotate().x, m_Part[1]->GetRotate().y, m_Part[1]->GetRotate().z);
	OX::DebugFont::print(400, 20* 3, 0xff00ff00, "RARM _ X:%f, Y:%f, Z:%f", m_Part[2]->GetRotate().x, m_Part[2]->GetRotate().y, m_Part[2]->GetRotate().z);
	OX::DebugFont::print(400, 20* 4, 0xff00ff00, "REDGE_ X:%f, Y:%f, Z:%f", m_Part[3]->GetRotate().x, m_Part[3]->GetRotate().y, m_Part[3]->GetRotate().z);
	OX::DebugFont::print(400, 20* 5, 0xff00ff00, "LARM _ X:%f, Y:%f, Z:%f", m_Part[4]->GetRotate().x, m_Part[4]->GetRotate().y, m_Part[4]->GetRotate().z);
	OX::DebugFont::print(400, 20* 6, 0xff00ff00, "LEDGE_ X:%f, Y:%f, Z:%f", m_Part[5]->GetRotate().x, m_Part[5]->GetRotate().y, m_Part[5]->GetRotate().z);
	OX::DebugFont::print(400, 20* 7, 0xff00ff00, "RLEG _ X:%f, Y:%f, Z:%f", m_Part[6]->GetRotate().x, m_Part[6]->GetRotate().y, m_Part[6]->GetRotate().z);
	OX::DebugFont::print(400, 20* 8, 0xff00ff00, "REDGE_ X:%f, Y:%f, Z:%f", m_Part[7]->GetRotate().x, m_Part[7]->GetRotate().y, m_Part[7]->GetRotate().z);
	OX::DebugFont::print(400, 20* 9, 0xff00ff00, "LLEG _ X:%f, Y:%f, Z:%f", m_Part[8]->GetRotate().x, m_Part[8]->GetRotate().y, m_Part[8]->GetRotate().z);
	OX::DebugFont::print(400, 20*10, 0xff00ff00, "LEDGE_ X:%f, Y:%f, Z:%f", m_Part[9]->GetRotate().x, m_Part[9]->GetRotate().y, m_Part[9]->GetRotate().z);
	OX::DebugFont::print(390, 20 + iterator_part * 20, 0xff00ff00, "-> ");


}


//=============================================================================
//	増減
// 座標の移動
void Player::MovePosition(D3DXVECTOR3& movePosition)
{
	movePosition.x *= -1;
	m_movePosition += m_pCamera->GetVecX() * movePosition.x;
	m_movePosition += m_pCamera->GetVecZ() * movePosition.z;

	if (fabs(m_movePosition.x) + fabs(m_movePosition.z) > m_maxMoveSpeed) {
		D3DXVec3Normalize(&m_movePosition, &m_movePosition);
		m_movePosition *= m_maxMoveSpeed;
	}

}


// 体力
void Player::MoveLife(int move)
{
	m_life += move;
	if (m_life >= m_maxLife) m_life = m_maxLife;

}



//=============================================================================
//	設定
// 体力
void Player::SetLife(float life)
{
	m_life = life;
	if (m_life >= m_maxLife) m_life = m_maxLife;

}


// 体勢
void Player::SetPosture(Player::POSTURE posture)
{
	m_posture = posture;
}


//=============================================================================
//	特殊
// 走る
void Player::Run(float moveX, float moveZ)
{
	// 【発生判定】体勢が NONE, RUN 以外では無処理で返す
	if ((m_posture != POSTURE_NONE && m_posture != POSTURE_RUN) || (moveX == 0 && moveZ == 0)) return;

	// 移動行動
	m_movePosition += m_pCamera->GetVecX() * moveX;
	m_movePosition += m_pCamera->GetVecZ() * moveZ;
	D3DXVec3Normalize(&m_movePosition, &m_movePosition);
	m_movePosition *= m_maxMoveSpeed;

	// 体勢の設定
	m_posture = POSTURE_RUN;

}


// 避ける
void Player::Avoid(float moveX, float moveZ)
{
	// 【発生判定】体勢が NONE, RUN, STREADY, DSREADY 以外では無処理で返す
	if (m_posture != POSTURE_NONE && m_posture != POSTURE_RUN && m_posture != POSTURE_STREADY && m_posture != POSTURE_DRREADY) return;

	
	// 移動力が全く無い場合は向いてる方向に回避する
	if (moveX == 0 && moveZ == 0) {
		moveX = this->GetFront().x * m_avDist;
		moveZ = this->GetFront().z * m_avDist;
	}


	// 回避行動
	m_movePosition += m_pCamera->GetVecX() * moveX;
	m_movePosition += m_pCamera->GetVecZ() * moveZ;
	D3DXVec3Normalize(&m_movePosition, &m_movePosition);
	m_movePosition *= m_avDist;

	// 【描画線データのクリア】
	if (m_posture == Player::POSTURE::POSTURE_DRREADY && m_pDL_StartPos != nullptr)
		m_pDL_StartPos->ClearParameter_List(m_pDL_StartPos);

	// 体勢の設定
	m_posture = POSTURE_AVOID;

	// 硬直時間の設定
	m_cntCooldownFrame = m_cooldown_Avoid;

}


// 緊急回避
void Player::EmAvoid(float moveX, float moveZ)
{
	// 【発生判定】体勢が STSHOT, DRSHOT 以外では無処理で返す
	if (m_posture != POSTURE_STSHOT && m_posture != POSTURE_DRSHOT) return;


	// 移動力が全く無い場合は向いてる方向に回避する
	if (moveX == 0 && moveZ == 0) {
		moveX = this->GetFront().x * m_avDist;
		moveZ = this->GetFront().z * m_avDist;
	}

	// 回避行動
	moveX *= -1;
	m_movePosition += m_pCamera->GetVecX() * moveX;
	m_movePosition += m_pCamera->GetVecZ() * moveZ;
	D3DXVec3Normalize(&m_movePosition, &m_movePosition);
	m_movePosition *= m_avDist;

	// 体勢の設定
	m_posture = POSTURE_EMAVOID;

	// 硬直時間の設定
	m_cntCooldownFrame = m_cooldown_EmAvoid;

}


// SS準備
void Player::STReady()
{
	// 【発生判定】体勢が NONE, RUN, STREADY 以外では無処理で返す
	if (m_posture != Player::POSTURE::POSTURE_NONE && m_posture != Player::POSTURE::POSTURE_RUN && m_posture != Player::POSTURE::POSTURE_STREADY)
		return;

	// カーソル座標のバッファ
	D3DXVECTOR3 mousePos(0, 0, 0);

	// 【当たり判定】カーソル
	if (CollisionCursor_Player(&mousePos)) {
		// 【体勢の設定】→ "STREADY"に
		m_posture = Player::POSTURE::POSTURE_STREADY;

		// 【向きの設定】
		SetRotateToObj(mousePos);

	}

}


// SS攻撃
void Player::STShot()
{
	// 【発生判定】体勢が"STREADY"以外では無処理で返す
	if (m_posture != POSTURE_STREADY) return;


	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// カーソル座標のバッファの用意
	D3DXVECTOR3 mousePos(0, 0, 0);


	// 【当たり判定】カーソル
	if (CollisionCursor_Player(&mousePos)) {
		// 【体勢の設定】→ "NONE"に
		m_posture = Player::POSTURE::POSTURE_NONE;

		// SS撃たずに終了
		return;

	}

	// 【向きの設定】カーソルの方向へ
	SetRotateToObj(mousePos);

	// 【SS発射】
	StraightShot::SetStraightShot(*this);

	// 【体勢の設定】→ "STSHOT"に
	m_posture = POSTURE_STSHOT;

	// 【硬直時間の設定】
	m_cntCooldownFrame = m_cooldown_StShot;


}


// DS準備
void Player::DRReady(bool bTrigger)
{
	// 【発生判定】体勢が NONE, RUN, DRREADY 以外では無処理で返す
	if (m_posture != Player::POSTURE::POSTURE_NONE && m_posture != Player::POSTURE::POSTURE_RUN && m_posture != Player::POSTURE::POSTURE_DRREADY)
		return;


	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// カーソル座標のバッファの用意
	D3DXVECTOR3 mousePos(0, 0, 0);


	// --- ステート遷移 ---
	switch (m_posture) {
	// 体勢が NONE, RUN → 【カーソルとキャラクタの当たり判定】
	case Player::POSTURE::POSTURE_NONE:
	case Player::POSTURE::POSTURE_RUN:
		// 【当たり判定の発生判定】左クリック押した瞬間じゃなかったら処理無し
		if (!bTrigger) return;

		// 【当たり判定】カーソル
		if (CollisionCursor_Player(&mousePos)) {
			// 【体勢の設定】→ "DRREADY"に
			m_posture = Player::POSTURE::POSTURE_DRREADY;
		}
		else {
			// 無処理で返す
			return;			
		}


		// THRU
	case Player::POSTURE::POSTURE_DRREADY:

		// 【カーソルの座標を取得】
		CollisionCursor_Player(&mousePos);

		// 【始点判定】
		if (bTrigger) {
			// １、始点を送る（始点だよ！という情報を忘れずに）
			m_pDL_StartPos = DrawLine::SetStartLine(mousePos, *this);
			//m_pDL_StartPos = DrawLine::SetStartLine(GetPosition(), *this);

			// ２、描画線の長さを初期化
			m_DL_Length = 0;

			// ３、描画線の前の点の座標を初期化
			m_DL_PrePos = mousePos;
			//m_DL_PrePos = GetPosition();

			// ３、描画線の更新前の長さを初期化
			m_DL_PreLength = m_DL_Length;

			// 始点が mousePos の場合
			break;
		}

		// 【射程外判定】
		if (Distance3D(mousePos, GetPosition()) > m_drawShotRange * m_drawShotRange) {
			// ※ 修正を行う
			// 【座標の設定】中継点
			D3DXVECTOR3 vec = mousePos - GetPosition();
			D3DXVec3Normalize(&vec, &vec);
			vec *= Player::m_drawShotRange;
			mousePos = vec + GetPosition();
		}

		// 【中継点の保存判定】
		//		※ 現在の条件：前の点との距離
		//	※要 条件を距離と角度への変更
		if (Distance3D(mousePos, this->m_DL_PrePos) <= Player::NON_DRAW * Player::NON_DRAW) break;	// switch

		// 距離を見る


		// 角度を見る


		// 描画線の長さの更新
		//m_DL_Length += (mousePos.x - m_DL_PrePos.x) * (mousePos.x - m_DL_PrePos.x)
		//			 + (mousePos.z - m_DL_PrePos.z) * (mousePos.z - m_DL_PrePos.z);
		m_DL_Length += sqrtf((mousePos.x - m_DL_PrePos.x) * (mousePos.x - m_DL_PrePos.x) + (mousePos.z - m_DL_PrePos.z) * (mousePos.z - m_DL_PrePos.z));


		// 【描画線の長さ判定】上限を超えているか
		//if (m_DL_Length >= m_drawShotLength * m_drawShotLength) {
		if (m_DL_Length >= m_drawShotLength) {
			// ★ 修正した次の点を送り、DSを撃つ ★

			// 描画線の長さの修正
			//float length = sqrtf(m_drawShotLength * m_drawShotLength - m_DL_PreLength);	// 限界値までの長さ
			float length = sqrtf(m_drawShotLength - m_DL_PreLength);	// 限界値までの長さ

			// 向きの取得
			D3DXVECTOR3 vec = mousePos - m_DL_PrePos;
			D3DXVec3Normalize(&vec, &vec);

			// 修正された点の座標を設定
			mousePos = vec * length + m_DL_PrePos;

			// 中継点を送る
			DrawLine::SetViaPoint(mousePos, m_pDL_StartPos);

			// DSを撃つ
			Player::DRShot(false);
			
			// 終了
			return;
		}

		// 描画線の更新前の長さを最新に更新する
		m_DL_PreLength = m_DL_Length;

		// 中継点を送る
		DrawLine::SetViaPoint(mousePos, m_pDL_StartPos);

		// 中継点の前の座標を更新
		m_DL_PrePos = mousePos;


		// DRREADY 終了
		break;

	}

#ifdef _DEBUG
	// デバック表示
	OX::DebugFont::print(0,  20, 0xff00ff00, "PlayerPos.x         : %f", ObjectModel::GetPosition().x);
	OX::DebugFont::print(0,  40, 0xff00ff00, "PlayerPos.y         : %f", ObjectModel::GetPosition().y);
	OX::DebugFont::print(0,  60, 0xff00ff00, "PlayerPos.z         : %f", ObjectModel::GetPosition().z);
	OX::DebugFont::print(0,  80, 0xff00ff00, "mousePos.x          : %f", mousePos.x);
	OX::DebugFont::print(0, 100, 0xff00ff00, "mousePos.y          : %f", mousePos.y);
	OX::DebugFont::print(0, 120, 0xff00ff00, "mousePos.z          : %f", mousePos.z);
	OX::DebugFont::print(0, 140, 0xff00ff00, "m_drawLine_PrePos.x : %f", m_DL_PrePos.x);
	OX::DebugFont::print(0, 160, 0xff00ff00, "m_drawLine_PrePos.y : %f", m_DL_PrePos.y);
	OX::DebugFont::print(0, 180, 0xff00ff00, "m_drawLine_PrePos.z : %f", m_DL_PrePos.z);
	OX::DebugFont::print(0, 200, 0xff00ff00, "m_drawLine_Length   : %f", m_DL_Length);
#endif

}


// DS攻撃
void Player::DRShot(bool bRelease)
{
	// 【発生判定】体勢が DRREADY 以外では無処理で返す
	if (m_posture != Player::POSTURE::POSTURE_DRREADY) return;


	// 【キャンセル判定】
	if(bRelease){
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

		// カーソル座標のバッファ用意
		D3DXVECTOR3 mousePos(0, 0, 0);

		// 【当たり判定】カーソル → プレイヤー
		if (CollisionCursor_Player(&mousePos)) {
			// 【体勢の設定】→ "NONE"に
			m_posture = Player::POSTURE::POSTURE_NONE;

			// 【描画線のクリア】
			m_pDL_StartPos->ClearParameter_List(m_pDL_StartPos);

			// DS撃たずに終了
			return;
		}

	}

	// 【DSの発生】
	DrawLine::SetDrawShot(m_pDL_StartPos);

	// 【向きの設定】プレイヤーの向き → 発射した方向を向く
	SetRotateToObj(m_pDL_StartPos->GetNextPointer_DrawLine()->GetPosition());

	// 【体勢の設定】→ "DRSHOT"に
	m_posture = Player::POSTURE::POSTURE_DRSHOT;

	// 【硬直時間の設定】
	m_cntCooldownFrame = m_cooldown_DrShot;

}


// 拠点を殴る
void Player::BreakBasePoint()
{
	// 【発生判定】体勢が NONE, RUN 以外では無処理で返す
	if (m_posture != Player::POSTURE::POSTURE_NONE && m_posture != Player::POSTURE::POSTURE_RUN) return;

	// 【当たり判定】カーソル → 拠点
	Tower* pTower = CollisionCursor_BasePoint();
	if (pTower == nullptr) return;

	D3DXVECTOR3 position = pTower->GetPosition();

	// 【破壊判定】
	if (pTower->BrowTower(m_breakPower)) {
		// 壊した時の処理

	}

	// 【体勢の設定】破壊
	m_posture = Player::POSTURE::POSTURE_BREAK;

	// 【向きの設定】
	SetRotateToObj(position);

	// 【硬直時間の設定】
	m_cntCooldownFrame = 10;

}


//=============================================================================
//	private関数
void Player::UpdateInverseMatrix()
{
	if (m_pCamera == nullptr) return;

	D3DXMATRIX vpMat;
	D3DXMatrixIdentity(&vpMat);

	D3DVIEWPORT9 vp;
	Renderer::GetDevice()->GetViewport(&vp);

	vpMat._11 = (float)vp.Width / 2;
	vpMat._22 = -1.0f * (float)(vp.Height / 2);
	vpMat._33 = (float)vp.MaxZ - vp.MinZ;
	vpMat._41 = (float)(vp.X + vp.Width / 2);
	vpMat._42 = (float)(vp.Y + vp.Height / 2);
	vpMat._43 = vp.MinZ;

	D3DXMATRIX inv_proj, inv_view;
	D3DXMatrixInverse(&m_InverseMatrix, 0, &vpMat);
	D3DXMatrixInverse(&inv_proj, 0, &m_pCamera->GetMtxProj());
	D3DXMatrixInverse(&inv_view, 0, &m_pCamera->GetMtxView());

	m_InverseMatrix *= inv_proj * inv_view;

}

// 【当たり判定】カーソル → プレイヤー
bool Player::CollisionCursor_Player(D3DXVECTOR3* pOut)
{
	// カーソル座標の取得
	POINT cursorPos;
	D3DXVECTOR3 mousePos;
	GetCursorPos(&cursorPos);
	ScreenToClient(GetHWnd(), &cursorPos);

	// 座標変換のために逆行列を掛ける
	for (int i = 9960000; i < 10000000; i++) {

		// 【座標の変換】カーソル座標 → 3D座標
		//transScreenToWorld(&mousePos, pDevice, cursorPos.x, cursorPos.y, (1.f / 10000000) * i, &m_pCamera->GetMtxView(), &m_pCamera->GetMtxProj());
		mousePos.x = (float)cursorPos.x;
		mousePos.y = (float)cursorPos.y;
		mousePos.z = (float)(1. / 10000000) * i;
		D3DXVec3TransformCoord(&mousePos, &mousePos, &m_InverseMatrix);

		if (mousePos.y < 0) {
			mousePos.y = 0;
			*pOut = mousePos;

			// 【当たり判定】カーソルとキャラクター
			if (CalcSphereRayCollision(GetRadius(), &this->GetPosition(), &mousePos, &m_pCamera->GetVecZ_UnNormal(), nullptr, nullptr)) {
				return true;
			}
			else { return false; }
		}
	}

	return false;

}

// 【当たり判定】カーソル → 拠点
Tower* Player::CollisionCursor_BasePoint()
{
	D3DXVECTOR3 P1, P2;

	// カーソル座標の取得
	POINT cursorPos;
	D3DXVECTOR3 mousePos;
	GetCursorPos(&cursorPos);
	ScreenToClient(GetHWnd(), &cursorPos);


	Tower* pTower = (Tower*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_MODEL_TOWER);

	if (pTower == nullptr) return nullptr;

	Tower* pCurrent = pTower;
	Tower* pNext    = (Tower*)pTower->GetNextPointer();

	for (;;) {
		// 敵グループの塔かどうか
		if (pCurrent->GetGroup() != GetGroup()) {

			// 殴れる範囲内にいるかどうか
			if (pCurrent->CollisionBrowRange(GetPosition())) {

				// 座標変換のために逆行列を掛ける
				for (int i = 9900000; i < 10000000; i++) {

					// 【座標の変換】カーソル座標 → 3D座標
					//transScreenToWorld(&mousePos, pDevice, cursorPos.x, cursorPos.y, (1.f / 10000000) * i, &m_pCamera->GetMtxView(), &m_pCamera->GetMtxProj());
					mousePos.x = (float)cursorPos.x;
					mousePos.y = (float)cursorPos.y;
					mousePos.z = (float)(1. / 10000000) * i;
					D3DXVec3TransformCoord(&mousePos, &mousePos, &m_InverseMatrix);

					// 【当たり判定】カーソルと"HOLD"
					if (CalcSphereRayCollision(pCurrent->GetHold()->GetRadius(), &pCurrent->GetHold()->GetPosition(), &mousePos, &m_pCamera->GetVecZ_UnNormal(), nullptr, nullptr)) {
						return pCurrent;
					}
				}

				return nullptr;

			}
		}

		pCurrent = pNext;

		if (pCurrent == nullptr) return nullptr;

		pNext = (Tower*)pCurrent->GetNextPointer();
	}

}


// 塔との当たり判定
void Player::CollisionTower()
{
	Tower* pTower = (Tower*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_MODEL_TOWER);

	if (pTower == nullptr) return;

	Tower* pCurrent = pTower;
	Tower* pNext    = (Tower*)pTower->GetNextPointer();

	for (;;) {
		if (Collision_SphereToSphere(GetPosition(), GetHalfSize().x, pCurrent->GetPosition(), pCurrent->GetHalfSize().x)) {
			D3DXVECTOR3 vec = GetPosition() - pCurrent->GetPosition();
			D3DXVec3Normalize(&vec, &vec);
			SetPosition(D3DXVECTOR3(pCurrent->GetPosition().x + vec.x * (GetHalfSize().x + pCurrent->GetHalfSize().x),
									0,
									pCurrent->GetPosition().z + vec.z * (GetHalfSize().x + pCurrent->GetHalfSize().x)));

			return;
		}

		pCurrent = pNext;

		if (pCurrent == nullptr)
				return;

		pNext = (Tower*)pCurrent->GetNextPointer();
	}

}

// 城との当たり判定
void Player::CollisionCastle()
{
	Castle* pCastle = (Castle*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_MODEL_CASTLE);

	if (pCastle == nullptr) return;

	Castle* pCurrent = pCastle;
	Castle* pNext = (Castle*)pCastle->GetNextPointer();

	for (;;) {
		if (Collision_SphereToSphere(GetPosition(), GetHalfSize().x, pCurrent->GetPosition(), pCurrent->GetHalfSize().x)) {

			D3DXVECTOR3 vec = GetPosition() - pCurrent->GetPosition();
			D3DXVec3Normalize(&vec, &vec);
			SetPosition(D3DXVECTOR3(pCurrent->GetPosition().x + vec.x * (GetHalfSize().x + pCurrent->GetHalfSize().x),
									0,
									pCurrent->GetPosition().z + vec.z * (GetHalfSize().x + pCurrent->GetHalfSize().x)));

			return;
		}

		pCurrent = pNext;

		if (pCurrent == nullptr) return;

		pNext = (Castle*)pCurrent->GetNextPointer();
	}

}

// 壁との当たり判定
void Player::CollisionWall()
{
	Wall* pCurrent = (Wall*)Object::GetLDATA_HEAD(Object::TYPE::TYPE_3D_WALL);

	if (pCurrent == nullptr) return;

	Wall* pNext = (Wall*)pCurrent->GetNextPointer();

	for (int i = 0; i < 4; i++)
	{
		// Left Wall
		if (GetPosition().x - GetRadius() < pCurrent->GetPosition_RIGHT().x)
		{
			D3DXVECTOR3 pos = GetPosition();
			SetPosition(D3DXVECTOR3(pCurrent->GetPosition_RIGHT().x + GetRadius(), pos.y, pos.z));

			return;
		}

		pCurrent = pNext;
		pNext = (Wall*)pCurrent->GetNextPointer();

		// Right Wall
		if (GetPosition().x + GetRadius() > pCurrent->GetPosition_LEFT().x)
		{
			D3DXVECTOR3 pos = GetPosition();
			SetPosition(D3DXVECTOR3(pCurrent->GetPosition_LEFT().x - GetRadius(), pos.y, pos.z));

			return;
		}

		pCurrent = pNext;
		pNext = (Wall*)pCurrent->GetNextPointer();


		// Front Wall
		if (GetPosition().z - GetRadius() < pCurrent->GetPosition_BACK().z)
		{
			D3DXVECTOR3 pos = GetPosition();
			SetPosition(D3DXVECTOR3(pos.x, pos.y, pCurrent->GetPosition_BACK().z + GetRadius()));

			return;
		}

		pCurrent = pNext;
		pNext = (Wall*)pCurrent->GetNextPointer();

		// Back Wall
		if (GetPosition().z + GetRadius() > pCurrent->GetPosition_FRONT().z)
		{
			D3DXVECTOR3 pos = GetPosition();
			SetPosition(D3DXVECTOR3(pos.x, pos.y, pCurrent->GetPosition_FRONT().z - GetRadius()));

			return;
		}
		else
			return;

		//D3DXVECTOR3 collisionPoint;

		//if (CalcParticlePlaneCollision(GetRadius(), &m_prePosion, &GetPosition(), &pCurrent->GetNormal_RIGHT(), &pCurrent->GetPosition_RIGHT(), nullptr, &collisionPoint))
		//{
		//	D3DXVECTOR3 moveVec = GetPosition() - m_prePosion;
		//	D3DXVECTOR3 vec;
		//	calcWallScratchVector(&vec, moveVec, pCurrent->GetNormal_RIGHT());

		//	D3DXVECTOR3 vec = GetPosition() - pCurrent->GetPosition();
		//	D3DXVec3Normalize(&vec, &vec);
		//	SetPosition(D3DXVECTOR3(pCurrent->GetPosition().x + vec.x * (GetHalfSize().x + pCurrent->GetHalfSize().x),
		//							0,
		//							pCurrent->GetPosition().z + vec.z * (GetHalfSize().x + pCurrent->GetHalfSize().x)));

		//	return;
		//}

		//if (pNext == nullptr) return;

		//pCurrent = pNext;
		//pNext = (Wall*)pCurrent->GetNextPointer();
	}

	return;
}
