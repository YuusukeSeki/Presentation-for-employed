//*****************************************************************************
//
//		ƒz[ƒ‹ƒh
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
// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
//-----------------------------------------------------------------------------
Hold::Hold() :ObjectBillboard(Object::TYPE::TYPE_3D_BILLBOARD_HOLD)
{
	// ƒƒ“ƒo•Ï”‰Šú‰»
	m_pParent           = nullptr;
	m_pBackGround       = nullptr;
	m_pLockOn_OutLine   = nullptr;
	m_pLockOn_InnerLine = nullptr;
	m_pLockOn_Hold      = nullptr;
	m_bDraw             = false;
}


//-----------------------------------------------------------------------------
// ƒfƒXƒgƒ‰ƒNƒ^
//-----------------------------------------------------------------------------
Hold::~Hold()
{
	// I—¹ˆ—
	Uninit();

}


//-----------------------------------------------------------------------------
// ŽÀ‘Ì‚Ì¶¬
//-----------------------------------------------------------------------------
Hold* Hold::Create(BasePoint* pObjModel, Camera* pCamera)
{
	// ¶¬
	Hold* pHold = new Hold;

	// ‰Šú‰»
	pHold->Init(pObjModel, pCamera);

	return pHold;
}


//-----------------------------------------------------------------------------
// ‰Šú‰»ˆ—
//-----------------------------------------------------------------------------
void Hold::Init(BasePoint* pObjModel, Camera* pCamera)
{
	//----- ƒf[ƒ^‚ÌÝ’è -----
	//ObjectBillboard::Init(pObjModel->GetPosition() - pCamera->GetVecZ() * 190, D3DXVECTOR3(pObjModel->GetRadius(), pObjModel->GetRadius(), 0), pCamera);	// Œp³ƒf[ƒ^‚Ì‰Šú‰»
	ObjectBillboard::Init(pObjModel->GetPosition(), D3DXVECTOR3(pObjModel->GetRadius(), pObjModel->GetRadius(), 0), pCamera);	// Œp³ƒf[ƒ^‚Ì‰Šú‰»
	m_pParent           = pObjModel;																									// e‚ÌÝ’è
	m_pBackGround       = ObjectBillboard::Create(GetPosition(), D3DXVECTOR3(GetRadius() * 1.65f, GetRadius() * 1.65f, 0), pCamera);	// Æ€A”wŒi
	m_pLockOn_OutLine   = ObjectBillboard::Create(GetPosition(), D3DXVECTOR3(GetRadius() * 1.50f, GetRadius() * 1.50f, 0), pCamera);	// Æ€AŠO˜g
	m_pLockOn_InnerLine = ObjectBillboard::Create(GetPosition(), D3DXVECTOR3(GetRadius() * 0.75f, GetRadius() * 0.75f, 0), pCamera);	// Æ€A“à˜g
	m_pLockOn_Hold      = ObjectBillboard::Create(GetPosition(), D3DXVECTOR3(GetRadius() * 1.2f, GetRadius() * 0.3f, 0), pCamera);		// Æ€F"HOLD"
	MovePosition(D3DXVECTOR3(0, pObjModel->GetSize().y, -pObjModel->GetHalfSize().z));
	m_bDraw             = false;																										// true ‚Å"HOLD"•\Ž¦

	m_pBackGround      ->SetTexture(MainGame::GetTexture(List_LoadTexture_MainGame::DRAWLINE_TEXNAME)->GetTexture());	// ƒeƒNƒXƒ`ƒƒ‚ÌÝ’è
	m_pLockOn_OutLine  ->SetTexture(MainGame::GetTexture(List_LoadTexture_MainGame::LOCKON_OUTLINE)->GetTexture());		// V
	m_pLockOn_InnerLine->SetTexture(MainGame::GetTexture(List_LoadTexture_MainGame::LOCKON_INNERLINE)->GetTexture());	// V
	m_pLockOn_Hold     ->SetTexture(MainGame::GetTexture(List_LoadTexture_MainGame::LOCKON_HOLD)->GetTexture());		// V

	m_pBackGround      ->SetColor(0x0080ff00);	// F‚ÌÝ’è
	m_pLockOn_OutLine  ->SetColor(0xffffff00);	// V
	m_pLockOn_InnerLine->SetColor(0xffffff00);	// V
	m_pLockOn_Hold     ->SetColor(0xffffff00);	// V

	m_pBackGround      ->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));	// Šgk—¦‚ÌÝ’è
	m_pLockOn_OutLine  ->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));	// V
	m_pLockOn_InnerLine->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));	// V
	m_pLockOn_Hold     ->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));	// V


}


//-----------------------------------------------------------------------------
// I—¹ˆ—
//-----------------------------------------------------------------------------
void Hold::Uninit(void)
{
	ObjectBillboard::Uninit();

}


//-----------------------------------------------------------------------------
// XVˆ—
//-----------------------------------------------------------------------------
void Hold::Update(void)
{
	// ‹ß‚Ã‚­
	if (m_bDraw) {
		// ƒ¿’l‚Ì‘‰Á
		m_pBackGround->MoveColor(0, 0, 0, (int)(0x60 * 0.05f));
		m_pLockOn_OutLine->MoveColor(0, 0, 0, (int)(0xb0 * 0.05f));
		m_pLockOn_InnerLine->MoveColor(0, 0, 0, (int)(0xb0 * 0.05f));
		m_pLockOn_Hold->MoveColor(0, 0, 0, (int)(0xff * 0.05f));

		// ‰ñ“]‚³‚¹‚é
		m_pLockOn_OutLine->MoveRotate(D3DXVECTOR3(0, 0, 1), 0.01f);
		m_pLockOn_InnerLine->MoveRotate(D3DXVECTOR3(0, 0, 1), -0.03f);

		// Šgk—¦‚ÌŒ¸­
		m_pBackGround->MoveScale(D3DXVECTOR3(-0.05f, -0.05f, -0.05f));
		m_pLockOn_OutLine->MoveScale(D3DXVECTOR3(-0.05f, -0.05f, -0.05f));
		m_pLockOn_InnerLine->MoveScale(D3DXVECTOR3(-0.05f, -0.05f, -0.05f));
		m_pLockOn_Hold->MoveScale(D3DXVECTOR3(-0.05f, -0.05f, -0.05f));

		if (m_pLockOn_OutLine->GetScale().x <= 1.0f) {
			// ƒ¿’l‚ÌÝ’è
			m_pBackGround->SetColor(0x0080ff60);
			m_pLockOn_OutLine->SetColor(0xffffffb0);
			m_pLockOn_InnerLine->SetColor(0xffffffb0);
			m_pLockOn_Hold->SetColor(0xffffffff);

			// Šgk—¦‚ÌÝ’è
			m_pBackGround->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			m_pLockOn_OutLine->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			m_pLockOn_InnerLine->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			m_pLockOn_Hold->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

		}
	}

	// —£‚ê‚é
	else {
		// “§‰ß
		m_pBackGround->MoveColor(0, 0, 0, (int)(0x60 * -0.05f));
		m_pLockOn_OutLine->MoveColor(0, 0, 0, (int)(0xb0 * -0.05f));
		m_pLockOn_InnerLine->MoveColor(0, 0, 0, (int)(0xb0 * -0.05f));
		m_pLockOn_Hold->MoveColor(0, 0, 0, (int)(0xff * -0.05f));

		// ‰ñ“]‚³‚¹‚é
		m_pLockOn_OutLine->MoveRotate(D3DXVECTOR3(0, 0, 1), -0.01f);
		m_pLockOn_InnerLine->MoveRotate(D3DXVECTOR3(0, 0, 1), 0.03f);

		// Šgk—¦‚ÌŒ¸­
		m_pBackGround->MoveScale(D3DXVECTOR3(0.05f, 0.05f, 0.05f));
		m_pLockOn_OutLine->MoveScale(D3DXVECTOR3(0.05f, 0.05f, 0.05f));
		m_pLockOn_InnerLine->MoveScale(D3DXVECTOR3(0.05f, 0.05f, 0.05f));
		m_pLockOn_Hold->MoveScale(D3DXVECTOR3(0.05f, 0.05f, 0.05f));

		if (m_pLockOn_OutLine->GetScale().x >= 1.5f) {
			// ƒ¿’l‚ÌÝ’è
			m_pBackGround->SetColor(0x0080ff00);
			m_pLockOn_OutLine->SetColor(0xffffff00);
			m_pLockOn_InnerLine->SetColor(0xffffff00);
			m_pLockOn_Hold->SetColor(0xffffff00);

			// Šgk—¦‚ÌÝ’è
			m_pBackGround->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));
			m_pLockOn_OutLine->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));
			m_pLockOn_InnerLine->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));
			m_pLockOn_Hold->SetScale(D3DXVECTOR3(1.5f, 1.5f, 1.5f));

		}
	}


}


//-----------------------------------------------------------------------------
// •`‰æˆ—
//-----------------------------------------------------------------------------
void Hold::Draw(void)
{
}



//=============================================================================
//	‘Œ¸ˆ—
void Hold::MovePosition(D3DXVECTOR3& movePosition)
{
	ObjectBillboard::MovePosition(movePosition);

	m_pBackGround      ->MovePosition(movePosition);
	m_pLockOn_OutLine  ->MovePosition(movePosition);
	m_pLockOn_InnerLine->MovePosition(movePosition);
	m_pLockOn_Hold     ->MovePosition(movePosition);

}



//=============================================================================
//	Ý’èˆ—
// ‰ð•úˆ—
void Hold::ReleaseThis()
{
	m_pBackGround      ->Release();
	m_pLockOn_OutLine  ->Release();	
	m_pLockOn_InnerLine->Release();
	m_pLockOn_Hold     ->Release();	

	Release();
}
