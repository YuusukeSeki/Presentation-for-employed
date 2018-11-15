// author : yusuke seki
// data   : 20181102
#include "MainGame.h"
#include "GameManager.h"
#include "result.h"
#include "Title.h"
#include "fade.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "renderer.h"
#include "player.h"
#include "Soldier.h"
#include "DrawRange.h"
#include "tower.h"
#include "castle.h"
#include "field.h"
#include "skybox.h"
#include "Wall.h"
#include "timer.h"
#include "score.h"
#include "FrameBorder.h"
#include "TeamGaugeManager.h"
#include "TeamGauge.h"
#include "Hold.h"
#include "texture.h"
#include "list_LoadTexture_MainGame.h"
#include "ModelData.h"
#include "ObjectModel.h"
#include "Object2D.h"
#include "EventButton.h"

#include <algorithm>

#define _INPUT_REGULATION		// コメントアウトでレギュレーション用操作を除外

std::vector<Player*> MainGame::players;
Field* MainGame::m_pField = nullptr;
SkyBox* MainGame::m_pSkyBox = nullptr;
Wall* MainGame::m_pWall[] = {};
Texture* MainGame::m_pTexture[] = {};

void MainGame::Init()
{
	List_LoadTexture_MainGame::LoadTextureAll(m_pTexture);

	//m_pCamera[0] = Camera::Create(D3DXVECTOR3(0.f, 200.f, -110.f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 1.f, 0.f), 1.f, 1000.f);

	// Create Light
	{
		m_pLight = Light::Create();
		m_pLight->SetVectorDirection(D3DXVECTOR3(0.f, -1, 0.f));
	}

	//----- Create Object -----
	{
		// Create Field
		{
			m_pField = Field::Create(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(750.f, 0, 3000.f), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 1, 1), 4, 4);
			m_pField->SetTexture(m_pTexture[List_LoadTexture_MainGame::TEXTURE_NAME::FIELD_TEXNAME]);
		}

		// Create Sky_Box
		{
			m_pSkyBox = SkyBox::Create(D3DXVECTOR3(0, 700, 0), 3500.f);
			m_pSkyBox->SetTexture(m_pTexture[List_LoadTexture_MainGame::TEXTURE_NAME::SKYBOX_TEXNAME]);
		}

		// Create Wall
		{
			// LeftWall
			D3DXVECTOR3 setSize = D3DXVECTOR3(	m_pField->GetSize().x * 0.05f, m_pField->GetSize().z * 0.03f, m_pField->GetSize().z);
			D3DXVECTOR3 setPos  = D3DXVECTOR3(	m_pField->GetPosition().x - m_pField->GetHalfSize().x + setSize.x * 0.5f,
												m_pField->GetPosition().y + setSize.y * 0.5f,
												m_pField->GetPosition().z);
			m_pWall[0] = Wall::Create(setPos, setSize);
			m_pWall[0]->SetTexture(m_pTexture[List_LoadTexture_MainGame::TEXTURE_NAME::WALL_TEXNAME]->GetTexture());

			// RightWall
			setPos  = D3DXVECTOR3(	m_pField->GetPosition().x + m_pField->GetHalfSize().x - setSize.x * 0.5f,
									m_pField->GetPosition().y + setSize.y * 0.5f,
									m_pField->GetPosition().z);
			m_pWall[1] = Wall::Create(setPos, setSize);
			m_pWall[1]->SetTexture(m_pTexture[List_LoadTexture_MainGame::TEXTURE_NAME::WALL_TEXNAME]->GetTexture());

			// FrontWall
			setSize = D3DXVECTOR3(	m_pField->GetSize().x, m_pField->GetSize().z * 0.03f, m_pField->GetSize().x * 0.05f);
			setPos  = D3DXVECTOR3(	m_pField->GetPosition().x,
									m_pField->GetPosition().y + setSize.y * 0.5f,
									m_pField->GetPosition().z - m_pField->GetHalfSize().z + setSize.z * 0.5f);
			m_pWall[2] = Wall::Create(setPos, setSize);
			m_pWall[2]->SetTexture(m_pTexture[List_LoadTexture_MainGame::TEXTURE_NAME::WALL_TEXNAME]->GetTexture());

			// BackWall
			setPos  = D3DXVECTOR3(	m_pField->GetPosition().x + m_pField->GetHalfSize().x - setSize.x * 0.5f,
									m_pField->GetPosition().y + setSize.y * 0.5f,
									m_pField->GetPosition().z + m_pField->GetHalfSize().z - setSize.z * 0.5f);
			m_pWall[3] = Wall::Create(setPos, setSize);
			m_pWall[3]->SetTexture(m_pTexture[List_LoadTexture_MainGame::TEXTURE_NAME::WALL_TEXNAME]->GetTexture());
		}

		// Create Cast
		{
			//Player* player = Player::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pCamera[0], MODELDATA_PLAYER, Player::CHARACTER::FIGHTER);
			Player* player = Player::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), Player::Character::FIGHTER);
			player->SetGroup(Object::GROUP::GROUP_A);

			players.push_back(player);
		}

		// Create Range of Draw
		{
			for each (auto player in players)
			{
				m_pDrawRange = DrawRange::Create(player);
				m_pDrawRange->SetTexture(m_pTexture[List_LoadTexture_MainGame::DRAWLINE_TEXNAME]->GetTexture());
				m_pDrawRange->SetColor(0xffffff80);
			}
		}

		// Create Tower
		{
			// Side Enemy
			D3DXVECTOR3 pos = m_pField->GetPosition();											// フィールド座標基準に補正
			float halfX = m_pField->GetHalfSize().x * 0.5f;										// フィールドサイズ基準に補正
			float halfZ = m_pField->GetHalfSize().z * 0.5f;										// フィールドサイズ基準に補正
			D3DXVECTOR3 setPos = D3DXVECTOR3(pos.x - halfX, 0, pos.z + halfZ - halfZ * 0.5f);	// 塔を設定する座標

			for (int i = 0; i < NUM_TOWER; i++)
			{
				m_pTower = Tower::Create(setPos, MODELDATA_TOWER, Object::GROUP_B);
				m_pTower->SetRotate(D3DXVECTOR3(0, D3DXToRadian(180), 0));
				//m_pTower->SetVecZ(D3DXVECTOR3(0, 0, -1));

				// 塔をちょっと前に出す
				if (i <= 1)
					m_pTower->MovePosition(D3DXVECTOR3(0, 0, -m_pField->GetSize().z * 0.05f));
				else
					m_pTower->MovePosition(D3DXVECTOR3(0, 0, -m_pField->GetSize().z * 0.12f));

				setPos.x *= -1;

				if (i == 1)
					setPos.z += halfZ;

			}

			// Side Your Team
			setPos = D3DXVECTOR3(pos.x - halfX, 0, pos.z - halfZ + halfZ * 0.5f);	// 塔を設定する座標

			for (int i = 0; i < NUM_TOWER; i++)
			{
				m_pTower = Tower::Create(setPos, MODELDATA_TOWER, Object::GROUP_A);
				if (i <= 1)
					m_pTower->MovePosition(D3DXVECTOR3(0, 0, m_pField->GetSize().z * 0.05f));
				else
					m_pTower->MovePosition(D3DXVECTOR3(0, 0, m_pField->GetSize().z * 0.12f));

				setPos.x *= -1;
				if (i == 1) setPos.z -= halfZ;
			}
		}

		// Create Castle
		{
			// Side Enemy
			m_pCastle = Castle::Create(MODELDATA_CASTLE, Object::GROUP_B);

			// Side Your Team
			m_pCastle = Castle::Create(MODELDATA_CASTLE, Object::GROUP_A);

		}

		// Create Buffer of Soldier
		{
			for (int i = 0; i < 256; i++)
				Soldier::Create(D3DXVECTOR3(0, 0, 0), MODELDATA_SOLDIER);
		}

		// Create Point of Healing
		//Object3D* pObject3D = Object3D::Create(D3DXVECTOR3(0, 50, -7500), D3DXVECTOR3(750, 0, 750));
		//pObject3D->SetNormal(D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 1, 0));
		//pObject3D->SetColor(0x00ff0080);
		//pObject3D->SetTexture(m_pTexture[List_LoadTexture_MainGame::DRAWLINE_TEXNAME]->GetTexture());

		//pObject3D = Object3D::Create(D3DXVECTOR3(-2000, 50, -6000), D3DXVECTOR3(750, 0, 750));
		//pObject3D->SetNormal(D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 1, 0));
		//pObject3D->SetColor(0x00ff0080);
		//pObject3D->SetTexture(m_pTexture[List_LoadTexture_MainGame::DRAWLINE_TEXNAME]->GetTexture());

		//pObject3D = Object3D::Create(D3DXVECTOR3(2000, 50, -6000), D3DXVECTOR3(750, 0, 750));
		//pObject3D->SetNormal(D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 1, 0));
		//pObject3D->SetColor(0x00ff0080);
		//pObject3D->SetTexture(m_pTexture[List_LoadTexture_MainGame::DRAWLINE_TEXNAME]->GetTexture());


		// Create Point of Speedy

		// Create Forest

		// example
		//float scall = 10;

		//ObjectModel* pObj = ObjectModel::Create(D3DXVECTOR3(0, 0, 30), MODELDATA_SOLDIER02);
		//pObj->SetScale(D3DXVECTOR3(scall, scall, scall));
		//pObj->SetColor(0xff0000ff);

	}


	// 左上UIの生成
	D3DXVECTOR3 size = D3DXVECTOR3((SCREEN_WIDTH * 0.42f), (SCREEN_HEIGHT * 0.08f), 0);
	D3DXVECTOR3 pos  = D3DXVECTOR3((SCREEN_WIDTH * 0.22f), (SCREEN_HEIGHT * 0.06f), 0);
	m_pFrameBorder = FrameBorder::Create(pos, size);
	m_pFrameBorder->SetTexture(m_pTexture[List_LoadTexture_MainGame::FRAMEBORDER_TEXNAME]->GetTexture());
	
	// スコアの生成
	size = D3DXVECTOR3(600.f * 0.4f, 100.f * 0.4f, 0.f);
	pos  = D3DXVECTOR3(SCREEN_WIDTH - size.x * 0.55f, SCREEN_HEIGHT - size.y * 0.75f, 0.f);
	m_pScore = Score::Create(pos, size, 6);
	m_pScore->SetTexture(m_pTexture[List_LoadTexture_MainGame::TEXTURE_NAME::NUMBER_TEXNAME]->GetTexture());

	_tutorial = Object2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.8f, 0), D3DXVECTOR3(SCREEN_WIDTH * 0.15f, SCREEN_HEIGHT * 0.15f, 0));
	_tutorial->SetTexture(m_pTexture[List_LoadTexture_MainGame::TUTORIAL]->GetTexture());
	
	// デバッグ用切り替えスイッチ（カメラ ⇔ プレイヤー）
	_switchControl = 1;


	m_pFrameBorder->GetTimer()->SwitchTimer();


	//ObjectModel* objModel;
	//pos = D3DXVECTOR3(0, 10, 10);
	//objModel = ObjectModel::Create(pos, MODELDATA_HEAD);
	//pos.z += 20;
	//objModel = ObjectModel::Create(pos, MODELDATA_BODY);
	//pos.z += 20;
	//objModel = ObjectModel::Create(pos, MODELDATA_RARM);
	//pos.z += 20;
	//objModel = ObjectModel::Create(pos, MODELDATA_RARM_EDGE);
	//pos.z += 20;
	//objModel = ObjectModel::Create(pos, MODELDATA_LARM);
	//pos.z += 20;
	//objModel = ObjectModel::Create(pos, MODELDATA_LARM_EDGE);
	//pos.z += 20;
	//objModel = ObjectModel::Create(pos, MODELDATA_RLEG);
	//pos.z += 20;
	//objModel = ObjectModel::Create(pos, MODELDATA_RLEG_EDGE);
	//pos.z += 20;
	//objModel = ObjectModel::Create(pos, MODELDATA_LLEG);
	//pos.z += 20;
	//objModel = ObjectModel::Create(pos, MODELDATA_LLEG_EDGE);

}

//-----------------------------------------------------------------------------
//	終了処理
//-----------------------------------------------------------------------------
void MainGame::Uninit()
{
	// ライト
	m_pLight->Release();
	m_pLight = nullptr;

	// カメラ
	for each(auto player in players)
	{
		//player->GetCamera()->Release();
		//player->GetCamera() = nullptr;
	}

	// スコア
	m_pScore->Release();

}

//-----------------------------------------------------------------------------
//	更新処理
//-----------------------------------------------------------------------------
GameScene* MainGame::Update()
{
	// 入力制御
	InputManage();

	// カメラをプレイヤーに追従させる：要列挙体
	for each (auto player in players)
	{
		if (_switchControl)
		{
			D3DXVECTOR3 pos = player->GetPosition();
			pos.z += 0.f;
			//D3DXVECTOR3 move = m_pCamera[0]->SetPosAt(pos);
			D3DXVECTOR3 move = player->GetCamera()->SetPosAt(pos);
		}

		// スカイボックスをカメラに追従させる：要列挙体
		//m_pSkyBox->MovePos(move);

		// スコア
		m_pScore->Update();

		// カメラ
		player->GetCamera()->Update();
	}

	// ライト
	m_pLight->Update();

	m_pDrawRange->Update();

	// 次のシーンに行く？
	if (GameManager::GetFade()->Finish_FadeOut()) { // 行く
													// ゲームシーンをリザルトへ
													//return new Result;
		return new Title;
	}
	else {	// 行かない
			// メインゲームの処理を続行する
		return this;
	}

}

//-----------------------------------------------------------------------------
//	描画処理
//-----------------------------------------------------------------------------
void MainGame::Draw()
{
}

//-----------------------------------------------------------------------------
//	trueで次のシーンに遷移
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//	
//-----------------------------------------------------------------------------
void MainGame::Release()
{
	// 終了処理
	Uninit();

	// メモリの解放
	delete this;
}

//-----------------------------------------------------------------------------
//	
//-----------------------------------------------------------------------------
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

// 壁情報の取得
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
