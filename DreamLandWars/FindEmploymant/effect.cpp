////*****************************************************************************
////
////		エフェクト
////												Autohr	:	Yusuke Seki
////*****************************************************************************
//#include "effect.h"
//#include "renderer.h"
//#include "MainGame.h"
//#include "camera.h"
//
//
//
////-----------------------------------------------------------------------------
////	コンストラクタ
////-----------------------------------------------------------------------------
//Effect::Effect(int priority, TYPE type) :Scene(priority, type)
//{
//	m_bUse = false;
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
//Effect::~Effect()
//{
//	Effect::Uninit();
//}
//
//
////-----------------------------------------------------------------------------
////	生成
////-----------------------------------------------------------------------------
//Effect* Effect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
//{
//	Effect* pEffect = new Effect(1, TYPE_3D);
//	pEffect->Init(pos, size, rot, scl);
//
//	return pEffect;
//}
//
//
////-----------------------------------------------------------------------------
////	初期化
////-----------------------------------------------------------------------------
//HRESULT Effect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
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
//	m_color.color = 0xffffffff;
//
//	m_radius = sqrtf((m_size.x * 0.5f) * (m_size.x * 0.5f) + (m_size.y * 0.5f) * (m_size.y * 0.5f));
//	m_angle = atan2f(m_size.y, m_size.x);
//
//	m_life = 0;
//	m_radiusValue = m_colorAlphaValue = 0;
//
//	m_move = D3DXVECTOR3(0, 0, 0);
//
//	m_bUse = false;
//
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
//	pVtx[0].pos = D3DXVECTOR3(-halfSize.x,  halfSize.y,  halfSize.z);	// 左上
//	pVtx[1].pos = D3DXVECTOR3( halfSize.x,  halfSize.y,  halfSize.z);	// 右上
//	pVtx[2].pos = D3DXVECTOR3(-halfSize.x, -halfSize.y, -halfSize.z);	// 左下
//	pVtx[3].pos = D3DXVECTOR3( halfSize.x, -halfSize.y, -halfSize.z);	// 右下
//
//	// 法線の設定
//	pVtx[0].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//	pVtx[1].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//	pVtx[2].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//	pVtx[3].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
//
//	// 頂点色の設定
//	pVtx[0].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
//	pVtx[1].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
//	pVtx[2].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
//	pVtx[3].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
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
//void Effect::Uninit(void)
//{
//	// 頂点バッファの破棄
//	if (m_pVtxBuff != nullptr) {
//		m_pVtxBuff->Release();
//		m_pVtxBuff = nullptr;
//	}
//}
//
//
////-----------------------------------------------------------------------------
////	更新
////-----------------------------------------------------------------------------
//void Effect::Update(void)
//{
//	// 未使用で処理無し
//	if (!m_bUse) return;
//
//
//
//	// 位置の更新
//	this->MovePos(m_move);
//
//	// 色の更新
//	m_color.rgba[3] = m_color.rgba[3] - 1 <= 0 ? 0 : m_color.rgba[3] - 1;
//	m_color.rgba[2] = m_color.rgba[2] - 1 <= 0 ? 0 : m_color.rgba[2] - 1;
//	m_color.rgba[1] = m_color.rgba[1] - 1 <= 0 ? 0 : m_color.rgba[1] - 1;
//	m_color.rgba[0] = m_color.rgba[0] -	m_colorAlphaValue * 2 <= 0 ? 0 : (unsigned char)(m_color.rgba[0] - m_colorAlphaValue * 2);
//
//	// 大きさの縮小
//	m_radius = m_radius - m_radiusValue < 0 ? 0 : m_radius - m_radiusValue;
//
//	// 寿命の減衰
//	m_life--;
//
//
//	// VRAMの仮想アドレス取得
//	VERTEX_3D* pVtx;
//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//	// 頂点座標の設定
//	pVtx[0].pos = D3DXVECTOR3(-m_radius,  m_radius, 0);	// 左上
//	pVtx[1].pos = D3DXVECTOR3( m_radius,  m_radius, 0);	// 右上
//	pVtx[2].pos = D3DXVECTOR3(-m_radius, -m_radius, 0);	// 左下
//	pVtx[3].pos = D3DXVECTOR3( m_radius, -m_radius, 0);	// 右下
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
//	// 寿命が尽きたら、使用フラグOFF
//	if (m_life <= 0) m_bUse = false;
//
//}
//
//
////-----------------------------------------------------------------------------
////	描画
////-----------------------------------------------------------------------------
//void Effect::Draw(void)
//{
//	// 未使用で処理無し
//	if (!m_bUse) return;
//
//
//
//	D3DXVECTOR3 pos = GetPos();
//	D3DXMATRIX mtxTrans, mtxRot, mtxScall, mtxView, mtxViewInverse;
//	Camera* pCamera = MainGame::GetCamera();
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
//	// ブレンド方法変更
//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
//
//	// Zバッファを書き込まない
//	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
//
//	// 描画
//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
//
//	// Zバッファを書き込む
//	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
//
//	// 描画方法を戻す
//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//
//	// ライトを有効に戻す
//	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//
//	// αテスト終了
//	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
//
//
//}
//
//
////-----------------------------------------------------------------------------
////	設定
////-----------------------------------------------------------------------------
//bool Effect::SetEffect(D3DXVECTOR3 pos)
//{
//	if (m_bUse) return false;
//
//	pos.x += rand() % 100 - 50;
//	pos.y += rand() % 100 - 20;
//
//	this->SetPos(pos);
//
//	m_color.color = 0xff8040ff;
//
//	m_move.x = 0;
//	m_move.y = (float)(rand() % 10001 / 2000);
//	m_radius = 120.0f;
//	m_angle = atan2f(m_radius, m_radius);
//	m_colorAlphaValue = 255 / ELASE_TIME;
//	m_radiusValue = m_radius / ELASE_TIME;
//	m_life = ELASE_TIME;
//
//	m_bUse = true;
//
//
//	return true;
//
//}
//
