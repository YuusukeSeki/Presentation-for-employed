////*****************************************************************************
////
////		ゲージ
////													Author : Yusuke Seki
////*****************************************************************************
//#include "Gauge.h"
//#include "ObjectBillboard.h"
//
//
//// コンストラクタ
//Gauge::Gauge()
//{
//	_bDraw     = false;	// 描画する？
//	_value      = 0;		// 現在のゲージ量
//	_initWidth = 0;		// ゲージの初期幅
//
//	_pGauge      = nullptr;	// ゲージ
//	_pBackGround = nullptr;	// ゲージの背景
//
//}
//
//// デストラクタ
//Gauge::~Gauge()
//{
//	Uninit();
//}
//
//// 生成処理
//// position : 座標
//// size : 大きさ
//Gauge* Gauge::Create(D3DXVECTOR3& position, D3DXVECTOR3& size, Camera* pCamera)
//{
//	Gauge* pGauge = new Gauge();
//	pGauge->Init(position, size, pCamera);
//
//	return pGauge;
//}
//
//// 初期化処理
//void Gauge::Init(D3DXVECTOR3& position, D3DXVECTOR3& size, Camera* pCamera)
//{
//	D3DXVECTOR3 sizePlus(size * 0.1f);	// 背景大きさの補正値
//
//	_pBackGround = ObjectBillboard::Create(position, size + sizePlus, pCamera);
//	_pBackGround->SetColor(0x000000a0);
//	_pGauge = ObjectBillboard::Create(position, size, pCamera);
//
//	_initWidth = size.x;
//	_bDraw = false;
//
//}
//
//// 終了処理
//void Gauge::Uninit(void)
//{
//
//}
//
//// 更新処理
//void Gauge::Update(void)
//{
//
//}
//
//// 解放処理
//void Gauge::Release(void)
//{
//	delete this;
//}
//
//// ゲージ量の増減処理
//void Gauge::MoveValue(float moveLife)
//{
//	_value += moveLife;
//
//}
//
//// 現在のゲージ量の取得
//float Gauge::GetValue(void)
//{
//	return _value;
//}
//
//// 描画の有無スイッチ
//void Gauge::SetDraw(bool bDraw)
//{
//	// 所有ポリゴンの描画フラグを切り替える
//	_pGauge->SetDraw(bDraw);
//	_pBackGround->SetDraw(bDraw);
//
//}
//
