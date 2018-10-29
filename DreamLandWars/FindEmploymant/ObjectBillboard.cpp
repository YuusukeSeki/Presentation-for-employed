//*****************************************************************************
//
//		ビルボード（リスト構造）
//												Autohr : Yusuke Seki
//*****************************************************************************
#include "ObjectBillboard.h"
#include "renderer.h"
#include "camera.h"


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
ObjectBillboard::ObjectBillboard(Object::TYPE type) :Object3D_Quaternion(type)
{
	// メンバ変数初期化
	m_pCamera = nullptr;

}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
ObjectBillboard::~ObjectBillboard()
{
	// 終了処理
	Uninit();

}


//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
ObjectBillboard* ObjectBillboard::Create(D3DXVECTOR3& position, D3DXVECTOR3& size, Camera* pCamera)
{
	// 生成
	ObjectBillboard* pObjectBillboard = new ObjectBillboard(Object::TYPE::TYPE_3D_BILLBOARD);

	// 初期化
	pObjectBillboard->Init(position, size, pCamera);

	return pObjectBillboard;
}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void ObjectBillboard::Init(D3DXVECTOR3& position, D3DXVECTOR3& size, Camera* pCamera)
{
	//----- データの設定 -----
	m_pCamera = pCamera;			// カメラ
	Object3D_Quaternion::Init(position, size);	// その他、行列関係や頂点バッファ関係、フラグなど

	// 行列を算出
	//this->UpdateWorldMatrix();

}


//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void ObjectBillboard::Uninit(void)
{
	// 継承データの終了処理
	Object3D_Quaternion::Uninit();

	// カメラポインタを手放す
	m_pCamera = nullptr;

}


//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void ObjectBillboard::Draw(void)
{
	// 描画機能は親クラス持ち
	Object3D_Quaternion::Draw();

}



//=============================================================================
//	増減処理


//=============================================================================
// 設定処理


//=============================================================================
//	private関数
// ワールドマトリクスの更新
void ObjectBillboard::UpdateWorldMatrix()
{
	// 親クラスから行列生成用のデータを取得
	D3DXVECTOR3    position   = Object::GetPosition();
	D3DXQUATERNION quaternion = Object3D_Quaternion::GetQuaternion();
	D3DXVECTOR3    scale      = Object3D_Quaternion::GetScale();

	// カメラからビュー行列を取得
	D3DXMATRIX pMtxView = m_pCamera->GetMtxView();

	// 移動、回転、拡縮行列、ビューの逆行列の計算
	D3DXMATRIX mtxWorld, mtxTranslate, mtxRotate, mtxScale, mtxViewInverse;
	D3DXMatrixTranslation(&mtxTranslate, position.x, position.y, position.z);
	D3DXMatrixRotationQuaternion(&mtxRotate, &quaternion);
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
	Object3D_Quaternion::SetWorldMatrix(mtxWorld);

	// ワールドマトリクスの更新フラグOFF
	Object3D_Quaternion::SetUpdateWorldMatrix(false);

}
