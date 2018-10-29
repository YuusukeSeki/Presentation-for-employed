//*****************************************************************************
//	
//		バレット
//													Autohr : Yusuke Seki
//*****************************************************************************
#include "BulletBillboard.h"
#include "camera.h"


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
BulletBillboard::BulletBillboard() : Bullet3D(Object::TYPE::TYPE_3D_BILLBOARD_BULLET)
{
	m_pCamera = nullptr;
}

BulletBillboard::BulletBillboard(Object::TYPE type) : Bullet3D(type)
{
	m_pCamera = nullptr;
}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
BulletBillboard::~BulletBillboard()
{
	Uninit();
}


//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
BulletBillboard* BulletBillboard::Create(D3DXVECTOR3& position, D3DXVECTOR3& size, Camera* pCamera)
{
	// 実体を生成
	BulletBillboard* pBullet = new BulletBillboard;

	// 初期化
	pBullet->Init(position, size, pCamera);

	return pBullet;
}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void BulletBillboard::Init(D3DXVECTOR3& position, D3DXVECTOR3& size, Camera* pCamera)
{
	// 継承データの初期化
	Bullet3D::Init(position, size);

	// カメラの取得
	m_pCamera = pCamera;

}


//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void BulletBillboard::Uninit(void)
{
	Bullet3D::Uninit();
}


//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void BulletBillboard::Draw(void)
{
	Bullet3D::Draw();
}


//=============================================================================
//	private関数
// ワールドマトリクスの更新
void BulletBillboard::UpdateWorldMatrix()
{
	if (m_pCamera == nullptr) {
		Object3D::UpdateWorldMatrix();
		return;
	}


	// 親クラスから行列生成用のデータを取得
	D3DXVECTOR3    position = Object::GetPosition();
	D3DXQUATERNION rotate   = Bullet3D::GetRotate();
	D3DXVECTOR3    scale    = Bullet3D::GetScale();

	// カメラからビュー行列を取得
	D3DXMATRIX pMtxView = m_pCamera->GetMtxView();


	// 移動、回転、拡縮行列、ビューの逆行列の計算
	D3DXMATRIX mtxWorld, mtxTranslate, mtxRotate, mtxScale, mtxViewInverse;
	D3DXMatrixTranslation(&mtxTranslate, position.x, position.y, position.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRotate, rotate.y, rotate.x, rotate.z);
	D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
	D3DXMatrixTranspose(&mtxViewInverse, &pMtxView);
	mtxViewInverse._14 = mtxViewInverse._24 = mtxViewInverse._34 = 0.0f;

	// ４行列の合成
	D3DXMatrixIdentity(&mtxWorld);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRotate);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxViewInverse);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// 生成した行列を設定
	Bullet3D::SetWorldMatrix(mtxWorld);

	// ワールドマトリクスの更新フラグOFF
	Bullet3D::SetUpdateWorldMatrix(false);

}

