//*****************************************************************************
//
//		ホールド
//												Autohr : Yusuke Seki
//*****************************************************************************
#include "Hold.h"
#include "camera.h"
#include "BasePoint.h"
#include "tower.h"
#include "castle.h"

#include "MainGame.h"
#include "list_LoadTexture_MainGame.h"


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
Hold::Hold() :ObjectBillboard(Object::TYPE::TYPE_3D_BILLBOARD_HOLD)
{
	// メンバ変数初期化
	m_pParent           = nullptr;
	m_pBackGround       = nullptr;
	m_pLockOn_OutLine   = nullptr;
	m_pLockOn_InnerLine = nullptr;
	m_pLockOn_Hold      = nullptr;
	m_bDraw             = false;
}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
Hold::~Hold()
{
	// 終了処理
	Uninit();

}


//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
Hold* Hold::Create(BasePoint* pObjModel, Camera* pCamera)
{
	// 生成
	Hold* pHold = new Hold;

	// 初期化
	pHold->Init(pObjModel, pCamera);

	return pHold;
}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void Hold::Init(BasePoint* pObjModel, Camera* pCamera)
{
	//----- データの設定 -----
	//ObjectBillboard::Init(pObjModel->GetPosition() - pCamera->GetVecZ() * 190, D3DXVECTOR3(pObjModel->GetRadius(), pObjModel->GetRadius(), 0), pCamera);	// 継承データの初期化
	ObjectBillboard::Init(pObjModel->GetPosition(), D3DXVECTOR3(pObjModel->GetRadius(), pObjModel->GetRadius(), 0), pCamera);	// 継承データの初期化
	m_pParent           = pObjModel;																									// 親の設定
	m_pBackGround       = ObjectBillboard::Create(GetPosition(), D3DXVECTOR3(GetRadius() * 1.65f, GetRadius() * 1.65f, 0), pCamera);	// 照準、背景
	m_pLockOn_OutLine   = ObjectBillboard::Create(GetPosition(), D3DXVECTOR3(GetRadius() * 1.50f, GetRadius() * 1.50f, 0), pCamera);	// 照準、外枠
	m_pLockOn_InnerLine = ObjectBillboard::Create(GetPosition(), D3DXVECTOR3(GetRadius() * 0.75f, GetRadius() * 0.75f, 0), pCamera);	// 照準、内枠
	m_pLockOn_Hold      = ObjectBillboard::Create(GetPosition(), D3DXVECTOR3(GetRadius() * 1.2f, GetRadius() * 0.3f, 0), pCamera);		// 照準："HOLD"
	MovePosition(D3DXVECTOR3(0, pObjModel->GetSize().y, -pObjModel->GetHalfSize().z));
	m_bDraw             = false;																										// true で"HOLD"表示

	m_pBackGround      ->SetTexture(MainGame::GetTexture(List_LoadTexture_MainGame::DRAWLINE_TEXNAME)->GetTexture());	// テクスチャの設定
	m_pLockOn_OutLine  ->SetTexture(MainGame::GetTexture(List_LoadTexture_MainGame::LOCKON_OUTLINE)->GetTexture());		// 〃
	m_pLockOn_InnerLine->SetTexture(MainGame::GetTexture(List_LoadTexture_MainGame::LOCKON_INNERLINE)->GetTexture());	// 〃
	m_pLockOn_Hold     ->SetTexture(MainGame::GetTexture(List_LoadTexture_MainGame::LOCKON_HOLD)->GetTexture());		// 〃

	m_pBackGround      ->SetColor(0x0080ff00);	// 色の設定
	m_pLockOn_OutLine  ->SetColor(0xffffff00);	// 〃
	m_pLockOn_InnerLine->SetColor(0xffffff00);	// 〃
	m_pLockOn_Hold     ->SetColor(0xffffff00);	// 〃

	m_pBackGround      ->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));	// 拡縮率の設定
	m_pLockOn_OutLine  ->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));	// 〃
	m_pLockOn_InnerLine->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));	// 〃
	m_pLockOn_Hold     ->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));	// 〃


}


//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void Hold::Uninit(void)
{
	ObjectBillboard::Uninit();

}


//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void Hold::Update(void)
{
	// 近づく
	if (m_bDraw) {
		// α値の増加
		m_pBackGround->MoveColor(0, 0, 0, (int)(0x60 * 0.05f));
		m_pLockOn_OutLine->MoveColor(0, 0, 0, (int)(0xb0 * 0.05f));
		m_pLockOn_InnerLine->MoveColor(0, 0, 0, (int)(0xb0 * 0.05f));
		m_pLockOn_Hold->MoveColor(0, 0, 0, (int)(0xff * 0.05f));

		// 回転させる
		m_pLockOn_OutLine->MoveRotate(D3DXVECTOR3(0, 0, 1), 0.01f);
		m_pLockOn_InnerLine->MoveRotate(D3DXVECTOR3(0, 0, 1), -0.03f);

		// 拡縮率の減少
		m_pBackGround->MoveScale(D3DXVECTOR3(-0.05f, -0.05f, -0.05f));
		m_pLockOn_OutLine->MoveScale(D3DXVECTOR3(-0.05f, -0.05f, -0.05f));
		m_pLockOn_InnerLine->MoveScale(D3DXVECTOR3(-0.05f, -0.05f, -0.05f));
		m_pLockOn_Hold->MoveScale(D3DXVECTOR3(-0.05f, -0.05f, -0.05f));

		if (m_pLockOn_OutLine->GetScale().x <= 1.0f) {
			// α値の設定
			m_pBackGround->SetColor(0x0080ff60);
			m_pLockOn_OutLine->SetColor(0xffffffb0);
			m_pLockOn_InnerLine->SetColor(0xffffffb0);
			m_pLockOn_Hold->SetColor(0xffffffff);

			// 拡縮率の設定
			m_pBackGround->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			m_pLockOn_OutLine->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			m_pLockOn_InnerLine->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			m_pLockOn_Hold->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

		}
	}

	// 離れる
	else {
		// 透過
		m_pBackGround->MoveColor(0, 0, 0, (int)(0x60 * -0.05f));
		m_pLockOn_OutLine->MoveColor(0, 0, 0, (int)(0xb0 * -0.05f));
		m_pLockOn_InnerLine->MoveColor(0, 0, 0, (int)(0xb0 * -0.05f));
		m_pLockOn_Hold->MoveColor(0, 0, 0, (int)(0xff * -0.05f));

		// 回転させる
		m_pLockOn_OutLine->MoveRotate(D3DXVECTOR3(0, 0, 1), -0.01f);
		m_pLockOn_InnerLine->MoveRotate(D3DXVECTOR3(0, 0, 1), 0.03f);

		// 拡縮率の減少
		m_pBackGround->MoveScale(D3DXVECTOR3(0.05f, 0.05f, 0.05f));
		m_pLockOn_OutLine->MoveScale(D3DXVECTOR3(0.05f, 0.05f, 0.05f));
		m_pLockOn_InnerLine->MoveScale(D3DXVECTOR3(0.05f, 0.05f, 0.05f));
		m_pLockOn_Hold->MoveScale(D3DXVECTOR3(0.05f, 0.05f, 0.05f));

		if (m_pLockOn_OutLine->GetScale().x >= 1.5f) {
			// α値の設定
			m_pBackGround->SetColor(0x0080ff00);
			m_pLockOn_OutLine->SetColor(0xffffff00);
			m_pLockOn_InnerLine->SetColor(0xffffff00);
			m_pLockOn_Hold->SetColor(0xffffff00);

			// 拡縮率の設定
			m_pBackGround->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));
			m_pLockOn_OutLine->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));
			m_pLockOn_InnerLine->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));
			m_pLockOn_Hold->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));

		}
	}


}


//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void Hold::Draw(void)
{
}



//=============================================================================
//	増減処理
void Hold::MovePosition(D3DXVECTOR3& movePosition)
{
	ObjectBillboard::MovePosition(movePosition);

	m_pBackGround      ->MovePosition(movePosition);
	m_pLockOn_OutLine  ->MovePosition(movePosition);
	m_pLockOn_InnerLine->MovePosition(movePosition);
	m_pLockOn_Hold     ->MovePosition(movePosition);

}



//=============================================================================
//	設定処理
// 解放処理
void Hold::ReleaseThis()
{
	m_pBackGround      ->Release();
	m_pLockOn_OutLine  ->Release();	
	m_pLockOn_InnerLine->Release();
	m_pLockOn_Hold     ->Release();	

	Release();
}
