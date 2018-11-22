////*****************************************************************************
////
////		ビルボード
////												Autohr : Yusuke Seki
////*****************************************************************************
//#include "billboard.h"
//#include "renderer.h"
//#include "camera.h"
//#include "MainGame.h"
//
//
////-----------------------------------------------------------------------------
////	コンストラクタ
////-----------------------------------------------------------------------------
//Billboard::Billboard(int priority, TYPE type) :Scene(priority, type)
//{
//	// メンバ変数初期化
//	D3DXMatrixIdentity(&m_mtxWorld);
//	m_rot = D3DXVECTOR3(0, 0, 0);
//	m_scl = D3DXVECTOR3(0, 0, 0);
//	m_size = D3DXVECTOR3(0, 0, 0);
//	m_color.color = 0xffffffff;
//
//	m_pVtxBuff = nullptr;
//	m_pTexture = nullptr;
//
//}
//
//
////-----------------------------------------------------------------------------
////	デストラクタ
////-----------------------------------------------------------------------------
//Billboard::~Billboard()
//{
//	Uninit();
//
//}
//
//
////-----------------------------------------------------------------------------
////	生成
////-----------------------------------------------------------------------------
//Billboard* Billboard::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
//{
//	Billboard* pBillboard = new Billboard(1, TYPE_3D);
//	pBillboard->Init(pos, size, rot, scl);
//
//	return pBillboard;
//}
//
//
////-----------------------------------------------------------------------------
////	初期化
////-----------------------------------------------------------------------------
//HRESULT Billboard::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
//{
//	D3DXMATRIX mtxTrans, mtxRot, mtxScall;
//	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();
//	VERTEX_3D* pVtx;
//	D3DXVECTOR3 halfSize = size * 0.5f;
//
//
//	// メンバ変数設定
//	SetPos(pos);
//	m_size = size;
//	m_rot = rot;
//	m_scl = scl;
//
//
//	// 頂点バッファ領域の生成
//	if (m_pVtxBuff == nullptr) {
//		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,
//			D3DPOOL_MANAGED, &m_pVtxBuff, nullptr))) {
//			MessageBox(nullptr, "頂点バッファ生成失敗", "Billboard::Init", MB_OK);
//		}
//	}
//
//
//	// VRAMの仮想アドレス取得
//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//	// 頂点座標の設定
//	pVtx[0].pos = D3DXVECTOR3(-halfSize.x, halfSize.y, halfSize.z);	// 左上
//	pVtx[1].pos = D3DXVECTOR3(halfSize.x, halfSize.y, halfSize.z);	// 右上
//	pVtx[2].pos = D3DXVECTOR3(-halfSize.x, -halfSize.y, -halfSize.z);	// 左下
//	pVtx[3].pos = D3DXVECTOR3(halfSize.x, -halfSize.y, -halfSize.z);	// 右下
//
//	// 法線の設定
//	pVtx[0].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//	pVtx[1].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//	pVtx[2].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//	pVtx[3].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//
//	// 頂点色の設定
//	pVtx[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
//	pVtx[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
//	pVtx[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
//	pVtx[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);
//
//	// テクスチャUV値の設定
//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//	// VRAMの仮想アドレス解放
//	m_pVtxBuff->Unlock();
//
//
//	// 各行列計算
//	D3DXMatrixScaling(&mtxScall, m_scl.x, m_scl.y, m_scl.z);
//	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
//
//	// 行列の合成
//	D3DXMatrixIdentity(&m_mtxWorld);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScall);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
//
//
//	return S_OK;
//}
//
//
////-----------------------------------------------------------------------------
////	終了
////-----------------------------------------------------------------------------
//void Billboard::Uninit(void)
//{
//	// 頂点バッファの破棄
//	if (m_pVtxBuff != nullptr) {
//		m_pVtxBuff->Release();
//		m_pVtxBuff = nullptr;
//	}
//
//}
//
//
////-----------------------------------------------------------------------------
////	更新
////-----------------------------------------------------------------------------
//void Billboard::Update(void)
//{
//}
//
//
////-----------------------------------------------------------------------------
////	描画
////-----------------------------------------------------------------------------
//void Billboard::Draw(void)
//{
//	D3DXVECTOR3 pos = GetPos();
//	D3DXMATRIX mtxTrans, mtxRot, mtxScall, mtxView, mtxViewInverse;
//	Camera* pCamera = MainGame::GetCamera(0);
//	mtxView = pCamera->GetMtxView();
//
//
//	// デバイスの取得
//	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();
//
//	// ワールド行列を求める
//	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
//	D3DXMatrixScaling(&mtxScall, m_scl.x, m_scl.y, m_scl.z);
//	D3DXMatrixTranspose(&mtxViewInverse, &mtxView);
//	mtxViewInverse._14 = mtxViewInverse._24 = mtxViewInverse._34 = 0.0f;
//
//	D3DXMatrixIdentity(&m_mtxWorld);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxViewInverse);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScall);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
//	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
//
//	// デバイスにワールド変換行列を設定
//	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
//
//	// ストリームの設定
//	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
//
//	// 頂点フォーマットの設定
//	pDevice->SetFVF(FVF_VERTEX_3D);
//
//	// テクスチャ貼り付け
//	pDevice->SetTexture(0, m_pTexture->GetTexture());
//
//	// αテストの設定
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// αテストＯＮ
//	pDevice->SetRenderState(D3DRS_ALPHAREF, 128);				// 判定値
//	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 演算子
//
//	// ライトを無効にする
//	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//
//	// 描画
//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
//
//	// ライトを有効に戻す
//	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//
//	// αテスト終了
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// αテストOFF
//
//}
//
//
////-----------------------------------------------------------------------------
//// 大きさの増減
////-----------------------------------------------------------------------------
//void Billboard::MoveSize(D3DXVECTOR3 move)
//{
//	D3DXVECTOR3 halfSize;
//	VERTEX_3D* pVtx;
//
//
//	// 大きさを更新
//	m_size += move;
//	halfSize = m_size * 0.5f;
//
//	// VRAMの仮想アドレス取得
//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//	// 頂点座標の設定
//	pVtx[0].pos = D3DXVECTOR3(-halfSize.x, halfSize.y, halfSize.z);	// 左上
//	pVtx[1].pos = D3DXVECTOR3(halfSize.x, halfSize.y, halfSize.z);	// 右上
//	pVtx[2].pos = D3DXVECTOR3(-halfSize.x, -halfSize.y, -halfSize.z);	// 左下
//	pVtx[3].pos = D3DXVECTOR3(halfSize.x, -halfSize.y, -halfSize.z);	// 右下
//
//																		// VRAMの仮想アドレス解放
//	m_pVtxBuff->Unlock();
//}
//
//
////-----------------------------------------------------------------------------
//// 大きさの設定
////-----------------------------------------------------------------------------
//void Billboard::SetSize(D3DXVECTOR3 size)
//{
//	D3DXVECTOR3 halfSize;
//	VERTEX_3D* pVtx;
//
//
//	// 大きさを更新
//	m_size = size;
//	halfSize = m_size * 0.5f;
//
//	// VRAMの仮想アドレス取得
//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//	// 頂点座標の設定
//	pVtx[0].pos = D3DXVECTOR3(-halfSize.x, halfSize.y, halfSize.z);		// 左上
//	pVtx[1].pos = D3DXVECTOR3(halfSize.x, halfSize.y, halfSize.z);		// 右上
//	pVtx[2].pos = D3DXVECTOR3(-halfSize.x, -halfSize.y, -halfSize.z);	// 左下
//	pVtx[3].pos = D3DXVECTOR3(halfSize.x, -halfSize.y, -halfSize.z);	// 右下
//
//	// VRAMの仮想アドレス解放
//	m_pVtxBuff->Unlock();
//
//}
//
//
//// 色
//void Billboard::SetColor(unsigned int color)
//{
//	m_color.color = color;
//
//	// 頂点情報を設定
//	VERTEX_3D* pVtx;
//
//	// VRAMの仮想アドレス取得
//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//	// 頂点色の設定
//	pVtx[0].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
//	pVtx[1].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
//	pVtx[2].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
//	pVtx[3].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
//
//	// VRAMの仮想アドレス解放
//	m_pVtxBuff->Unlock();
//
//
//}
//
//
