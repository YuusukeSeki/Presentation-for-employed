// author : yusuke seki
// data   : 20181102
#include "MainGame.h"
#include "GameManager.h"
#include "result.h"
#include "Title.h"
#include "fade.h"
#include "input.h"
#include "renderer.h"

#include "field.h"
#include "camera.h"
#include "light.h"
#include "tower.h"
#include "castle.h"
#include "Wall.h"


#include "player.h"
#include "Soldier.h"
#include "DrawRange.h"
#include "skybox.h"
#include "timer.h"
#include "score.h"
#include "FrameBorder.h"
#include "TeamGaugeManager.h"
#include "TeamGauge.h"
#include "Hold.h"
#include "texture.h"
#include "list_LoadTexture_MainGame.h"
#include "ModelData.h"
#include "BasePoint.h"
#include "ObjectBillboard.h"
#include "EventButton.h"

#include "Object.h"

#include <algorithm>

#include "RelayPoint.h"

#define _INPUT_REGULATION		// コメントアウトでレギュレーション用操作を除外

std::vector<Player*> MainGame::players;
Field* MainGame::m_pField = nullptr;
SkyBox* MainGame::m_pSkyBox = nullptr;
Wall* MainGame::m_pWall[] = {};
Texture* MainGame::m_pTexture[] = {};

Camera* MainGame::camera_ = nullptr;

void MainGame::Init()
{
	List_LoadTexture_MainGame::LoadTextureAll(m_pTexture);

	CreateField();

	CreateWall();

	CreateRelayPoint();

	CreateCamera();

	CreateBasePoint();

	CreateUnit();

	CreateUI();

	CreateLight();

	//CreateBackGround();

//		// Create Sky_Box
//		{
//			m_pSkyBox = SkyBox::Create(D3DXVECTOR3(0, 700, 0), 3500.f);
//			m_pSkyBox->SetTexture(m_pTexture[List_LoadTexture_MainGame::TEXTURE_NAME::SKYBOX_TEXNAME]);
//		}
//
//		// Create Cast
//		{
//			//Player* player = Player::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pCamera[0], MODELDATA_PLAYER, Player::CHARACTER::FIGHTER);
//			Player* player = Player::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), Player::Character::FIGHTER);
//			player->SetGroup(Object::GROUP::GROUP_A);
//
//			players.push_back(player);
//		}
//
//		// Create Range of Draw
//		{
//			for each (auto player in players)
//			{
//				m_pDrawRange = DrawRange::Create(player);
//				m_pDrawRange->SetTexture(m_pTexture[List_LoadTexture_MainGame::DRAWLINE_TEXNAME]->GetTexture());
//				m_pDrawRange->SetColor(0xffffff80);
//			}
//		}
//
//		// Create Point of Healing
//		//Object3D* pObject3D = Object3D::Create(D3DXVECTOR3(0, 50, -7500), D3DXVECTOR3(750, 0, 750));
//		//pObject3D->SetNormal(D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 1, 0));
//		//pObject3D->SetColor(0x00ff0080);
//		//pObject3D->SetTexture(m_pTexture[List_LoadTexture_MainGame::DRAWLINE_TEXNAME]->GetTexture());
//
//		//pObject3D = Object3D::Create(D3DXVECTOR3(-2000, 50, -6000), D3DXVECTOR3(750, 0, 750));
//		//pObject3D->SetNormal(D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 1, 0));
//		//pObject3D->SetColor(0x00ff0080);
//		//pObject3D->SetTexture(m_pTexture[List_LoadTexture_MainGame::DRAWLINE_TEXNAME]->GetTexture());
//
//		//pObject3D = Object3D::Create(D3DXVECTOR3(2000, 50, -6000), D3DXVECTOR3(750, 0, 750));
//		//pObject3D->SetNormal(D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 1, 0));
//		//pObject3D->SetColor(0x00ff0080);
//		//pObject3D->SetTexture(m_pTexture[List_LoadTexture_MainGame::DRAWLINE_TEXNAME]->GetTexture());
//
//		// Create Point of Speedy
//
//		// Create Forest
//
//	}
//
//
	// 左上UIの生成
	D3DXVECTOR3 size = D3DXVECTOR3((SCREEN_WIDTH * 0.42f), (SCREEN_HEIGHT * 0.08f), 0);
	D3DXVECTOR3 pos  = D3DXVECTOR3((SCREEN_WIDTH * 0.22f), (SCREEN_HEIGHT * 0.06f), 0);
	m_pFrameBorder = FrameBorder::Create(pos, size);
	m_pFrameBorder->SetTexture(m_pTexture[List_LoadTexture_MainGame::FRAMEBORDER_TEXNAME]->GetTexture());
//	
//	// スコアの生成
//	size = D3DXVECTOR3(600.f * 0.4f, 100.f * 0.4f, 0.f);
//	pos  = D3DXVECTOR3(SCREEN_WIDTH - size.x * 0.55f, SCREEN_HEIGHT - size.y * 0.75f, 0.f);
//	m_pScore = Score::Create(pos, size, 6);
//	m_pScore->SetTexture(m_pTexture[List_LoadTexture_MainGame::TEXTURE_NAME::NUMBER_TEXNAME]->GetTexture());
//
//	_tutorial = Object2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.8f, 0), D3DXVECTOR3(SCREEN_WIDTH * 0.15f, SCREEN_HEIGHT * 0.15f, 0));
//	_tutorial->SetTexture(m_pTexture[List_LoadTexture_MainGame::TUTORIAL]->GetTexture());
//	
//	// デバッグ用切り替えスイッチ（カメラ ⇔ プレイヤー）
//	_switchControl = 1;
//
//
//	m_pFrameBorder->GetTimer()->SwitchTimer();
//
//
//	//BasePoint* objModel;
//	//pos = D3DXVECTOR3(0, 10, 10);
//	//objModel = BasePoint::Create(pos, MODELDATA_HEAD);
//	//pos.z += 20;
//	//objModel = BasePoint::Create(pos, MODELDATA_BODY);
//	//pos.z += 20;
//	//objModel = BasePoint::Create(pos, MODELDATA_RARM);
//	//pos.z += 20;
//	//objModel = BasePoint::Create(pos, MODELDATA_RARM_EDGE);
//	//pos.z += 20;
//	//objModel = BasePoint::Create(pos, MODELDATA_LARM);
//	//pos.z += 20;
//	//objModel = BasePoint::Create(pos, MODELDATA_LARM_EDGE);
//	//pos.z += 20;
//	//objModel = BasePoint::Create(pos, MODELDATA_RLEG);
//	//pos.z += 20;
//	//objModel = BasePoint::Create(pos, MODELDATA_RLEG_EDGE);
//	//pos.z += 20;
//	//objModel = BasePoint::Create(pos, MODELDATA_LLEG);
//	//pos.z += 20;
//	//objModel = BasePoint::Create(pos, MODELDATA_LLEG_EDGE);
//
}

void MainGame::Uninit()
{
	if (camera_ != nullptr)
	{
		camera_->Release();
		camera_ = nullptr;
	}

	for each(auto light in lights_)
	{
		light.second->Release();
	}
	lights_.clear();

	towers_.clear();
	castles_.clear();

	for each(auto relayPoint in relayPoints_)
	{
		relayPoint.second->Release();
	}
	relayPoints_.clear();

	wallList_.clear();
}

GameScene* MainGame::Update()
{
	camera_->Update();

	// 入力制御
	InputManage();

	//// カメラをプレイヤーに追従させる：要列挙体
	//for each (auto player in players)
	//{
	//	if (_switchControl)
	//	{
	//		D3DXVECTOR3 pos = player->GetPosition();
	//		pos.z += 0.f;
	//		//D3DXVECTOR3 move = m_pCamera[0]->SetPosAt(pos);
	//		D3DXVECTOR3 move = player->GetCamera()->SetPosAt(pos);
	//	}

	//	// スカイボックスをカメラに追従させる：要列挙体
	//	//m_pSkyBox->MovePos(move);

	//	// スコア
	//	m_pScore->Update();

	//	// カメラ
	//	player->GetCamera()->Update();
	//}

	//// ライト
	//m_pLight->Update();

	//m_pDrawRange->Update();

	//// 次のシーンに行く？
	//if (GameManager::GetFade()->Finish_FadeOut()) { // 行く
	//												// ゲームシーンをリザルトへ
	//												//return new Result;
	//	return new Title;
	//}
	//else {	// 行かない
	//		// メインゲームの処理を続行する
	//	return this;
	//}

	return this;
}

void MainGame::Draw()
{

}

bool MainGame::Start_NextScene()
{
	// 入力デバイスの取得
	Input* pInput = GameManager::GetInput();

	// フェード状態の取得
	Fade* pFade = GameManager::GetFade();


	// スペースーキー押された？
	if (pInput->GetKeyboardTrigger(DIK_SPACE)) {
		// フェードアウト開始
		pFade->Start_FadeOut();
	}

	// フェードアウト終了した？
	if (pFade->Finish_FadeOut()) {
		// ゲームシーンをリザルトへ
		return true;
	}

	return false;
}

void MainGame::Release()
{
	Uninit();

	delete this;
}

void MainGame::InputManage()
{
	// 入力デバイスの取得
	Input* pInput = GameManager::GetInput();

	// スイッチの切り替え
	if (pInput->GetKeyboardTrigger(BUTTON_DEBUG_SWITCH_CTRL)) _switchControl ^= 1;


	// プレイヤー
	{
		// ジョイスティック、左軸傾きの取得
		D3DXVECTOR3 lAxis = pInput->GetJoystickLeftAxis();
		lAxis.y *= -1;
		// 誤差修正
		if (fabs(lAxis.x) < 50 && fabs(lAxis.y) < 50) {
			lAxis.x = 0;
			lAxis.y = 0;
		}

		const float MOVE = 0.6f;
		const float ROT = 0.03f;

		// 移動
		if (pInput->GetKeyboardPress(DIK_W))
			camera_->MovePosEye_Z(MOVE);

		if (pInput->GetKeyboardPress(DIK_S))
			camera_->MovePosEye_Z(-MOVE);

		if (pInput->GetKeyboardPress(DIK_A))
			camera_->MovePosEye_X(-MOVE);

		if (pInput->GetKeyboardPress(DIK_D))
			camera_->MovePosEye_X(MOVE);

		if (pInput->GetKeyboardPress(DIK_R))
			camera_->MovePosEye_Y(MOVE * 3);

		if (pInput->GetKeyboardPress(DIK_F))
			camera_->MovePosEye_Y(-MOVE * 3);

		// 回転
		if (pInput->GetKeyboardPress(DIK_Q))
			camera_->MoveRotEye(-ROT);
		
		if (pInput->GetKeyboardPress(DIK_E))
			camera_->MoveRotEye(ROT);

		// 旋回
		if (pInput->GetKeyboardPress(DIK_Z))
			camera_->MoveTurnEye(-ROT);
		
		if (pInput->GetKeyboardPress(DIK_C))
			camera_->MoveTurnEye(ROT);

#ifdef _DEBUG
		// collider switch
		if (pInput->GetKeyboardTrigger(DIK_P))
		{
			if (Object::GetIsDrawCollider() == true)
			{
				Object::SetIsDrawCollider(false);
			}
			else
			{
				Object::SetIsDrawCollider(true);
			}
		}
#endif


		OX::DebugFont::print(0,  0, 0xffffffff, "camera    : %f, %f, %f ", camera_->GetPosEye().x, camera_->GetPosEye().y, camera_->GetPosEye().z);

		for each (auto player in players)
		{
			// カメラ 
			if (!_switchControl)
			{
				const float MOVE = 10.0f;
				const float ROT = 0.03f;

				// 移動
				if (pInput->GetKeyboardPress(DIK_W)) player->GetCamera()->MovePosEye_Z(MOVE);	// 前
				if (pInput->GetKeyboardPress(DIK_S)) player->GetCamera()->MovePosEye_Z(-MOVE);	// 後
				if (pInput->GetKeyboardPress(DIK_A)) player->GetCamera()->MovePosEye_X(-MOVE);	// 左
				if (pInput->GetKeyboardPress(DIK_D)) player->GetCamera()->MovePosEye_X(MOVE);	// 右

				// 回転
				if (pInput->GetKeyboardPress(DIK_Q)) player->GetCamera()->MoveRotEye(-ROT);		// 反時計周り
				if (pInput->GetKeyboardPress(DIK_E)) player->GetCamera()->MoveRotEye(ROT);		// 時計周り

				// 旋回
				if (pInput->GetKeyboardPress(DIK_Z)) player->GetCamera()->MoveTurnEye(-ROT);	// 反時計周り
				if (pInput->GetKeyboardPress(DIK_C)) player->GetCamera()->MoveTurnEye(ROT);		// 時計周り
			}

//			player->Run(lAxis.x, lAxis.y);
//			if (pInput->GetJoystickTrigger(Input::JOYSTICKBUTTON005)) player->Avoid(lAxis.x, lAxis.y);
//			if (pInput->GetJoystickWTrigger(Input::JOYSTICKBUTTON005)) player->EmAvoid(lAxis.x, lAxis.y);
//			if (pInput->GetMouseTrigger(Input::MOUSEBUTTON_LEFT)) player->STReady();
//			if (pInput->GetMouseRelease(Input::MOUSEBUTTON_LEFT)) player->STShot();
//			if (pInput->GetMousePress(Input::MOUSEBUTTON_RIGHT)) player->DRReady(pInput->GetMouseTrigger(Input::MOUSEBUTTON_RIGHT));
//			if (pInput->GetMouseRelease(Input::MOUSEBUTTON_RIGHT)) player->DRShot(true);
//			if (pInput->GetMousePress(Input::MOUSEBUTTON_LEFT)) player->BreakBasePoint();
//
//#ifdef _INPUT_REGULATION
//			//----- ▼▼以下レギュレーション用キーボード操作▼▼ -----
//			// 移動
//			if (_switchControl)
//			{
//				if (pInput->GetKeyboardPress(DIK_W) && pInput->GetKeyboardPress(DIK_A)) player->Run(-1, +1);
//				else if (pInput->GetKeyboardPress(DIK_W) && pInput->GetKeyboardPress(DIK_D)) player->Run(+1, +1);
//				else if (pInput->GetKeyboardPress(DIK_S) && pInput->GetKeyboardPress(DIK_A)) player->Run(-1, -1);
//				else if (pInput->GetKeyboardPress(DIK_S) && pInput->GetKeyboardPress(DIK_D)) player->Run(+1, -1);
//				else if (pInput->GetKeyboardPress(DIK_W))                                    player->Run(0, +1);
//				else if (pInput->GetKeyboardPress(DIK_S))                                    player->Run(0, -1);
//				else if (pInput->GetKeyboardPress(DIK_A))                                    player->Run(-1, 0);
//				else if (pInput->GetKeyboardPress(DIK_D))                                    player->Run(+1, 0);
//			}
//
//			// 回避
//			if (pInput->GetKeyboardTrigger(DIK_E))
//			{
//				if (pInput->GetKeyboardPress(DIK_W) && pInput->GetKeyboardPress(DIK_A)) player->Avoid(-1, +1);
//				else if (pInput->GetKeyboardPress(DIK_W) && pInput->GetKeyboardPress(DIK_D)) player->Avoid(+1, +1);
//				else if (pInput->GetKeyboardPress(DIK_S) && pInput->GetKeyboardPress(DIK_A)) player->Avoid(-1, -1);
//				else if (pInput->GetKeyboardPress(DIK_S) && pInput->GetKeyboardPress(DIK_D)) player->Avoid(+1, -1);
//				else if (pInput->GetKeyboardPress(DIK_W))                                    player->Avoid(0, +1);
//				else if (pInput->GetKeyboardPress(DIK_S))                                    player->Avoid(0, -1);
//				else if (pInput->GetKeyboardPress(DIK_A))                                    player->Avoid(-1, 0);
//				else if (pInput->GetKeyboardPress(DIK_D))                                    player->Avoid(+1, 0);
//				else                                                                         player->Avoid(0, 0);
//			}
//
//			// 緊急回避
//			if (pInput->GetKeyboardWTrigger(DIK_E))
//			{
//				if (pInput->GetKeyboardPress(DIK_W) && pInput->GetKeyboardPress(DIK_A)) player->EmAvoid(-1, +1);
//				else if (pInput->GetKeyboardPress(DIK_W) && pInput->GetKeyboardPress(DIK_D)) player->EmAvoid(+1, +1);
//				else if (pInput->GetKeyboardPress(DIK_S) && pInput->GetKeyboardPress(DIK_A)) player->EmAvoid(-1, -1);
//				else if (pInput->GetKeyboardPress(DIK_S) && pInput->GetKeyboardPress(DIK_D)) player->EmAvoid(+1, -1);
//				else if (pInput->GetKeyboardPress(DIK_W))                                    player->EmAvoid(0, +1);
//				else if (pInput->GetKeyboardPress(DIK_S))                                    player->EmAvoid(0, -1);
//				else if (pInput->GetKeyboardPress(DIK_A))                                    player->EmAvoid(-1, 0);
//				else if (pInput->GetKeyboardPress(DIK_D))                                    player->EmAvoid(+1, 0);
//				else                                                                         player->EmAvoid(0, 0);
//
//			}
//#endif
		}
	}

	return;

	// タイマーを動かす / 止める
	if (pInput->GetKeyboardTrigger(DIK_T)) m_pFrameBorder->GetTimer()->SwitchTimer();

	// 点数を...
	if (pInput->GetKeyboardPress(DIK_P)) m_pScore->MoveScore(+50000); // 上げる
	if (pInput->GetKeyboardPress(DIK_O)) m_pScore->MoveScore(-50000); // 下げる

	// 塗りつぶしモードを...
	if (pInput->GetKeyboardPress(DIK_F1)) Renderer::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);	    // "点" に変更
	if (pInput->GetKeyboardPress(DIK_F2)) Renderer::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); // "ワイヤーフレーム" に変更
	if (pInput->GetKeyboardPress(DIK_F3)) Renderer::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	    // "面" に変更

	// シーン遷移
	if (pInput->GetKeyboardTrigger(BUTTON_NEXT_SCENE)) GameManager::GetFade()->Start_FadeOut();

}

Wall* MainGame::GetWall(int index)
{
	if (index < 0 || index >= NUM_WALL)
	{
		return nullptr;
	}

	return m_pWall[index];
}

Player* MainGame::GetPlayer(unsigned int index)
{
	if (index < 0 || index >= players.size())
	{
		return nullptr;
	}

	return players[index];
}

void MainGame::CreateField()
{
	const D3DXVECTOR3 kSize(100.0f, 0.0f, 300.0f);
	const D3DXVECTOR3 kPosition = kSize * 0.5f;
	const D3DXVECTOR3 kRotate(0.0f, 0.0f, 0.0f);
	const D3DXVECTOR3 kScale(1.0f, 1.0f, 1.0f);
	const int kNumHorizon = 16;
	const int kNumVertical = kNumHorizon * (int)(kSize.z / kSize.x);

	field_ = Field::Create(kPosition, kSize, kRotate, kScale, kNumHorizon, kNumVertical);
	field_->SetTexture(m_pTexture[List_LoadTexture_MainGame::TEXTURE_NAME::FIELD_TEXNAME]);
}

void MainGame::CreateWall()
{
	D3DXVECTOR3 position, halfSize;

	wallList_.emplace("front", Wall::Create(field_->GetPosition(), "data/model/MainGame/wall.x", Wall::Collide::BACK));

	halfSize = wallList_["front"]->GetHalfSize();

	position = field_->GetPosition();
	position.z += field_->GetHalfSize().z - halfSize.z;
	wallList_["front"]->SetPosition(position);

	position = field_->GetPosition();
	position.z -= field_->GetHalfSize().z - halfSize.z;
	wallList_.emplace("back", Wall::Create(position, "data/model/MainGame/wall.x", Wall::Collide::FRONT));

	position = field_->GetPosition();
	position.x -= field_->GetHalfSize().x - halfSize.z;
	position.z += field_->GetHalfSize().z - halfSize.x;
	wallList_.emplace("left1", Wall::Create(position, "data/model/MainGame/wall.x", Wall::Collide::RIGHT));
	wallList_["left1"]->SetRotate(D3DXVECTOR3(0, D3DXToRadian(-90), 0));

	position.z -= halfSize.x * 2;
	wallList_.emplace("left2", Wall::Create(position, "data/model/MainGame/wall.x", Wall::Collide::RIGHT));
	wallList_["left2"]->SetRotate(D3DXVECTOR3(0, D3DXToRadian(-90), 0));

	position.z -= halfSize.x * 2;
	wallList_.emplace("left3", Wall::Create(position, "data/model/MainGame/wall.x", Wall::Collide::RIGHT));
	wallList_["left3"]->SetRotate(D3DXVECTOR3(0, D3DXToRadian(-90), 0));

	position = field_->GetPosition();
	position.x += field_->GetHalfSize().x - halfSize.z;
	position.z += field_->GetHalfSize().z - halfSize.x;
	wallList_.emplace("right1", Wall::Create(position, "data/model/MainGame/wall.x", Wall::Collide::LEFT));
	wallList_["right1"]->SetRotate(D3DXVECTOR3(0, D3DXToRadian(90), 0));

	position.z -= halfSize.x * 2;
	wallList_.emplace("right2", Wall::Create(position, "data/model/MainGame/wall.x", Wall::Collide::LEFT));
	wallList_["right2"]->SetRotate(D3DXVECTOR3(0, D3DXToRadian(90), 0));

	position.z -= halfSize.x * 2;
	wallList_.emplace("right3", Wall::Create(position, "data/model/MainGame/wall.x", Wall::Collide::LEFT));
	wallList_["right3"]->SetRotate(D3DXVECTOR3(0, D3DXToRadian(90), 0));

}

void MainGame::CreateRelayPoint()
{
	D3DXVECTOR3 castlePosition = field_->GetPosition();
	D3DXVECTOR3 leftPosition = field_->GetPosition();
	D3DXVECTOR3 rightPosition = field_->GetPosition();

	castlePosition.z = field_->GetPosition().z - field_->GetHalfSize().z;
	relayPoints_.emplace("friendCastleLeft", RelayPoint::Create(castlePosition));
	relayPoints_.emplace("friendCastleRight", RelayPoint::Create(castlePosition));

	castlePosition.z = field_->GetPosition().z + field_->GetHalfSize().z;
	relayPoints_.emplace("enemyCastleLeft", RelayPoint::Create(castlePosition));
	relayPoints_.emplace("enemyCastleRight", RelayPoint::Create(castlePosition));

	leftPosition.x -= field_->GetHalfSize().x * 0.5f;
	leftPosition.z -= field_->GetHalfSize().z * 0.75f;
	relayPoints_.emplace("friendLeft", RelayPoint::Create(leftPosition));

	rightPosition.x += field_->GetHalfSize().x * 0.5f;
	rightPosition.z -= field_->GetHalfSize().z * 0.75f;
	relayPoints_.emplace("friendRight", RelayPoint::Create(rightPosition));

	leftPosition.z = field_->GetPosition().z + field_->GetHalfSize().z * 0.75f;
	relayPoints_.emplace("enemyLeft", RelayPoint::Create(leftPosition));

	rightPosition.z = field_->GetPosition().z + field_->GetHalfSize().z * 0.75f;
	relayPoints_.emplace("enemyRight", RelayPoint::Create(rightPosition));

	relayPoints_.at("friendCastleLeft")->SetFrontRelayPoint(relayPoints_.at("friendLeft"));
	relayPoints_.at("friendCastleLeft")->SetBackRelayPoint(nullptr);
	relayPoints_.at("friendCastleRight")->SetFrontRelayPoint(relayPoints_.at("friendRight"));
	relayPoints_.at("friendCastleRight")->SetBackRelayPoint(nullptr);

	relayPoints_.at("enemyCastleLeft")->SetFrontRelayPoint(nullptr);
	relayPoints_.at("enemyCastleLeft")->SetBackRelayPoint(relayPoints_.at("enemyLeft"));
	relayPoints_.at("enemyCastleRight")->SetFrontRelayPoint(nullptr);
	relayPoints_.at("enemyCastleRight")->SetBackRelayPoint(relayPoints_.at("enemyRight"));

	relayPoints_.at("friendLeft")->SetFrontRelayPoint(relayPoints_.at("enemyLeft"));
	relayPoints_.at("friendLeft")->SetBackRelayPoint(relayPoints_.at("friendCastleLeft"));

	relayPoints_.at("enemyLeft")->SetFrontRelayPoint(relayPoints_.at("enemyCastleLeft"));
	relayPoints_.at("enemyLeft")->SetBackRelayPoint(relayPoints_.at("friendLeft"));

	relayPoints_.at("friendRight")->SetFrontRelayPoint(relayPoints_.at("enemyRight"));
	relayPoints_.at("friendRight")->SetBackRelayPoint(relayPoints_.at("friendCastleRight"));

	relayPoints_.at("enemyRight")->SetFrontRelayPoint(relayPoints_.at("enemyCastleRight"));
	relayPoints_.at("enemyRight")->SetBackRelayPoint(relayPoints_.at("friendRight"));

}

void MainGame::CreateBasePoint()
{
	// tower
	D3DXVECTOR3 leftFrontPosition = field_->GetPosition();
	leftFrontPosition.x -= field_->GetHalfSize().x * 0.5f;
	leftFrontPosition.z -= field_->GetHalfSize().z * 0.25f;

	D3DXVECTOR3 leftBackPosition = field_->GetPosition();
	leftBackPosition.x -= field_->GetHalfSize().x * 0.5f;
	leftBackPosition.z -= field_->GetHalfSize().z * 0.6f;

	D3DXVECTOR3 rightFrontPosition = field_->GetPosition();
	rightFrontPosition.x += field_->GetHalfSize().x * 0.5f;
	rightFrontPosition.z -= field_->GetHalfSize().z * 0.25f;

	D3DXVECTOR3 rightBackPosition = field_->GetPosition();
	rightBackPosition.x += field_->GetHalfSize().x * 0.5f;
	rightBackPosition.z -= field_->GetHalfSize().z * 0.6f;

	towers_.emplace("friendLeftFront", Tower::Create(leftFrontPosition, Object::GROUP::GROUP_A, relayPoints_.at("enemyLeft")));
	towers_.emplace("friendLeftBack", Tower::Create(leftBackPosition, Object::GROUP::GROUP_A, relayPoints_.at("enemyLeft")));
	towers_.emplace("friendRightFront", Tower::Create(rightFrontPosition, Object::GROUP::GROUP_A, relayPoints_.at("enemyRight")));
	towers_.emplace("friendRightBack", Tower::Create(rightBackPosition, Object::GROUP::GROUP_A, relayPoints_.at("enemyRight")));

	leftFrontPosition.z = field_->GetPosition().z + field_->GetHalfSize().z * 0.25f;
	leftBackPosition.z = field_->GetPosition().z + field_->GetHalfSize().z * 0.6f;
	rightFrontPosition.z = field_->GetPosition().z + field_->GetHalfSize().z * 0.25f;
	rightBackPosition.z = field_->GetPosition().z + field_->GetHalfSize().z * 0.6f;

	towers_.emplace("enemyLeftFront", Tower::Create(leftFrontPosition, Object::GROUP::GROUP_B, relayPoints_.at("friendLeft")));
	towers_.emplace("enemyLeftBack", Tower::Create(leftBackPosition, Object::GROUP::GROUP_B, relayPoints_.at("friendLeft")));
	towers_.emplace("enemyRightFront", Tower::Create(rightFrontPosition, Object::GROUP::GROUP_B, relayPoints_.at("friendRight")));
	towers_.emplace("enemyRightBack", Tower::Create(rightBackPosition, Object::GROUP::GROUP_B, relayPoints_.at("friendRight")));

	// castle
	D3DXVECTOR3 castlePosition = field_->GetPosition();

	castlePosition.z = wallList_["back"]->GetPosition().z + wallList_["back"]->GetHalfSize().z + 7;
	castles_.emplace("friend", Castle::Create(castlePosition, Object::GROUP::GROUP_A
		, relayPoints_.at("friendLeft"), relayPoints_.at("friendRight")));

	castlePosition.z = wallList_["front"]->GetPosition().z - wallList_["front"]->GetHalfSize().z - 7;
	castles_.emplace("enemy", Castle::Create(castlePosition, Object::GROUP::GROUP_B
		, relayPoints_.at("enemyLeft"), relayPoints_.at("enemyRight")));

}

void MainGame::CreateUnit()
{
	Soldier::CreateBuffer(256);
}

void MainGame::CreateUI()
{

}

void MainGame::CreateCamera()
{
	const D3DXVECTOR3 kLookAtPosition = field_->GetPosition();
	//const D3DXVECTOR3 kLookAtPosition = D3DXVECTOR3(0, 0, 0);
	const D3DXVECTOR3 kEyePosition(kLookAtPosition.x, 25.0f, kLookAtPosition.z - 12.5f);
	//const D3DXVECTOR3 kEyePosition(kLookAtPosition.x, 300.0f, kLookAtPosition.z - 7.5f);
	const D3DXVECTOR3 kVectorUp(0.0f, 1.0f, 0.0f);
	const float kNear = 0.1f;
	//const float kFar = 100.0f;
	const float kFar = 1000.0f;

	camera_ = Camera::Create(kEyePosition, kLookAtPosition, kVectorUp, kNear, kFar);
}

void MainGame::CreateLight()
{
	D3DXVECTOR3 kVector(0.0f, -1.0f, 0.0f);

	lights_.emplace("down", Light::Create());
	lights_.at("down")->SetVectorDirection(kVector);

	kVector = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	lights_.emplace("front", Light::Create());
	lights_.at("front")->SetVectorDirection(kVector);

	kVector = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	lights_.emplace("back", Light::Create());
	lights_.at("back")->SetVectorDirection(kVector);

}

void MainGame::CreateBackGround()
{
	skyBox_ = SkyBox::Create(field_->GetPosition(), field_->GetSize().z);
	skyBox_->SetTexture(m_pTexture[List_LoadTexture_MainGame::TEXTURE_NAME::SKYBOX_TEXNAME]);
}
