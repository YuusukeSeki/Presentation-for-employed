//*****************************************************************************
//
//		エフェクト（リスト構造）
//												Autohr	:	Yusuke Seki
//*****************************************************************************
#include "EffectBillboard.h"
#include "renderer.h"
#include "MainGame.h"
#include "camera.h"



//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
EffectBillboard::EffectBillboard(EffectManager::TYPE type) :Effect3D(type)
{
	// データの初期化
	m_pCamera = nullptr;
}


//-----------------------------------------------------------------------------
//	デストラクタ
//-----------------------------------------------------------------------------
EffectBillboard::~EffectBillboard()
{
	EffectBillboard::Uninit();
}


//-----------------------------------------------------------------------------
//	生成
//-----------------------------------------------------------------------------
EffectBillboard* EffectBillboard::Create(D3DXVECTOR3& position, D3DXVECTOR3& size, Camera* pCamera)
{
	EffectBillboard* pEffectBillboard = new EffectBillboard(EffectManager::TYPE::TYPE_3D_BILLBOARD);
	pEffectBillboard->Init(position, size, pCamera);

	return pEffectBillboard;
}


//-----------------------------------------------------------------------------
//	初期化
//-----------------------------------------------------------------------------
void EffectBillboard::Init(D3DXVECTOR3& position, D3DXVECTOR3& size, Camera* pCamera)
{
	// データの初期化
	m_pCamera = pCamera;			// カメラ
	Effect3D::Init(position, size);	// 継承データの初期化

}


//-----------------------------------------------------------------------------
//	終了
//-----------------------------------------------------------------------------
void EffectBillboard::Uninit(void)
{
	// 継承データの終了処理
	Effect3D::Uninit();

}


//-----------------------------------------------------------------------------
//	更新
//-----------------------------------------------------------------------------
void EffectBillboard::Update(void)
{
	// 継承データの更新処理
	Effect3D::Update();

}


//-----------------------------------------------------------------------------
//	描画
//-----------------------------------------------------------------------------
void EffectBillboard::Draw(void)
{
	// 描画処理は親クラスに丸投げ
	Effect3D::Draw();

}


//=============================================================================
//	設定
//	エフェクトの発生
void EffectBillboard::SetEffect(D3DXVECTOR3& position, D3DXVECTOR3& vecZ, Camera* pCamera, Effect3D::TYPE effectType)
{
	// 実体持ちを探す
	EffectBillboard* pEffectBillboard = (EffectBillboard*)EffectManager::GetLDATA_HEAD(EffectManager::TYPE::TYPE_3D);

	// １つも作られていなかったら生成する
	if (pEffectBillboard == nullptr) {
		// 生成処理
		pEffectBillboard = EffectBillboard::Create(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), pCamera);

		// 設定処理
		pEffectBillboard->SetEffect_Protected(position, vecZ, effectType);

		return;
	}

	// 作られていたら未使用のエフェクトを探す
	EffectBillboard* pCurrent = (EffectBillboard*)pEffectBillboard;
	EffectBillboard* pNext    = (EffectBillboard*)pEffectBillboard->GetNextPointer();
	for (;;) {
		// 未使用なら設定して終了
		if (!pCurrent->GetInstance()) {
			pCurrent->SetEffect_Protected(position, vecZ, effectType);
			break;
		}

		// 未使用エフェクトが見つからなければ、新しく生成する
		if (pNext == nullptr) {
			// 生成処理
			pNext = EffectBillboard::Create(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), pCamera);

			// 設定処理
			pNext->SetEffect_Protected(position, vecZ, effectType);

			break;
		}

		// ポインタをずらす
		pCurrent = pNext;
		pNext    = (EffectBillboard*)pCurrent->GetNextPointer();

	}
}


//=============================================================================
//	private関数
// ワールドマトリクスの更新
void EffectBillboard::UpdateWorldMatrix()
{
	// 親クラスから行列生成用のデータを取得
	D3DXVECTOR3* pPosition = EffectManager::GetPosition();
	D3DXVECTOR3* pRotate = Effect3D::GetRotate();
	D3DXVECTOR3* pScale = Effect3D::GetScale();

	// カメラからビュー行列を取得
	D3DXMATRIX mtxView = m_pCamera->GetMtxView();


	// 移動、回転、拡縮行列、ビューの逆行列の計算
	D3DXMATRIX mtxWorld, mtxTranslate, mtxRotate, mtxScale, mtxViewInverse;
	D3DXMatrixTranslation(&mtxTranslate, pPosition->x, pPosition->y, pPosition->z);
	D3DXMatrixRotationYawPitchRoll(&mtxRotate, pRotate->y, pRotate->x, pRotate->z);
	D3DXMatrixScaling(&mtxScale, pScale->x, pScale->y, pScale->z);
	D3DXMatrixTranspose(&mtxViewInverse, &mtxView);
	mtxViewInverse._14 = mtxViewInverse._24 = mtxViewInverse._34 = 0.0f;

	// ４行列の合成
	D3DXMatrixIdentity(&mtxWorld);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxViewInverse);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRotate);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ワールドマトリクスの更新フラグOFF
	Effect3D::SetUpdateWorldMatrix(false);

}

