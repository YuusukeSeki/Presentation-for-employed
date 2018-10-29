////*****************************************************************************
////
////		体力ゲージ（拠点）
////													Author : Yusuke Seki
////*****************************************************************************
//#include "LifeGauge.h"
//#include "MainGame.h"
//#include "list_LoadTexture_MainGame.h"
//#include "Effect3D.h"
//#include "tower.h"
//
//
//
////-----------------------------------------------------------------------------
//// コンストラクタ
////-----------------------------------------------------------------------------
//LifeGauge::LifeGauge(Object::TYPE type) : ObjectBillboard(type)
//{
//	// データのクリア
//	m_pParent = nullptr;	// 親モデル
//
//	m_initLife = 0;	// 現在のチームゲージ量
//	m_initWidth = 0.f;	// ポリゴンの初期幅
//
//	m_pBG = nullptr;	// 背景
//
//}
//
//
////-----------------------------------------------------------------------------
//// デストラクタ
////-----------------------------------------------------------------------------
//LifeGauge::~LifeGauge()
//{
//	// 終了処理
//	Uninit();
//
//}
//
//
////-----------------------------------------------------------------------------
//// 実体の生成
////-----------------------------------------------------------------------------
//LifeGauge* LifeGauge::Create(ObjectModel* pObjModel, Camera* pCamera)
//{
//	// 実体の生成
//	LifeGauge* pLifeGauge = new LifeGauge;
//
//	// 実体の初期化
//	pLifeGauge->Init(pObjModel, pCamera);
//
//	// 生成されたタイマーのポインタを返す
//	return pLifeGauge;
//}
//
//
////-----------------------------------------------------------------------------
//// 初期化処理
////-----------------------------------------------------------------------------
//void LifeGauge::Init(ObjectModel* pObjModel, Camera* pCamera)
//{
//	// データの初期化
//	{
//		// 親の取得
//		m_pParent = pObjModel;
//		Tower* pTower = (Tower*)m_pParent;
//
//		// 継承データの初期化
//		ObjectBillboard::Init(m_pParent->GetPosition(), D3DXVECTOR3(300, 75, 0), pCamera);
//		MovePosition(D3DXVECTOR3(0, pTower->GetSize().y + 50, 0));
//
//		// 初期体力の設定
//		m_initLife = pTower->GetLife();
//
//		// ポリゴンの初期幅の設定
//		m_initWidth = GetSize().x;
//
//		// 所属陣営
//		SetGroup(pTower->GetGroup());
//
//		if (GetGroup() == Object::GROUP::GROUP_A) SetColor(0xff0000b0);
//		else if (GetGroup() == Object::GROUP::GROUP_B) SetColor(0x00ffffb0);
//
//		// 背景の生成
//		//m_pBG = ObjectBillboard::Create(GetPosition() + D3DXVECTOR3(0, 0, 5), GetSize() + D3DXVECTOR3(30, 7.5f, 0), pCamera);
//		//m_pBG->SetColor(0x00000080);
//
//	}
//
//}
//
//
////-----------------------------------------------------------------------------
//// 終了処理
////-----------------------------------------------------------------------------
//void LifeGauge::Uninit(void)
//{
//	// 継承データの終了処理
//	ObjectBillboard::Uninit();
//
//}
//
//
////-----------------------------------------------------------------------------
//// 更新処理
////-----------------------------------------------------------------------------
//void LifeGauge::Update(void)
//{
//
//}
//
////-----------------------------------------------------------------------------
//// 描画処理
////-----------------------------------------------------------------------------
//void LifeGauge::Draw(void)
//{
//	// 描画処理
//	ObjectBillboard::Draw();
//
//}
//
//
//
////=============================================================================
//// 増減
//// ゲージ量の増減処理
//void LifeGauge::MoveLife(int moveLife)
//{
//	Tower* pTower = (Tower*)m_pParent;
//
//	int life = pTower->GetLife();
//
//	// ゲージ量が0以下で無処理
//	if (life <= 0) return;
//
//	// 現在のゲージ量の更新
//	life += moveLife;
//
//	// 最大値からの割合の算出
//	float percentage = (float)life / m_initLife;
//
//	// ゲージの中心点と大きさの更新
//	D3DXVECTOR3 size = GetSize();
//	size.x = m_initWidth * percentage;
//	D3DXVECTOR3 position = GetPosition();
//	position.x -= (GetSize().x - size.x) * 0.5f;
//	SetPosition(position);
//	SetSize(size);
//
//	// ゲージのUVサイズの更新
//	D3DXVECTOR2 UVsize = GetUV_Size();
//	UVsize.x = 1.0f * percentage;
//	SetUV_Size(UVsize);
//
//}
//
//
////=============================================================================
//// 設定
