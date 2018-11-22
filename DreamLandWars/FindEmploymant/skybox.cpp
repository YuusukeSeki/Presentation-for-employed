//*****************************************************************************
//
//		スカイボックス
//													Autohr : Yusuke Seki
//
//*****************************************************************************
#include "skybox.h"
#include "renderer.h"

// 座標いじってた残骸
//#define _USE_JPEG_



//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
SkyBox::SkyBox(Object::TYPE type) : Object(type)
{
	// メンバ変数初期化
	D3DXMatrixIdentity(&m_mtxWorld); // 行列を単位行列にする（全値を１にする）
	m_width = 0.f;                   // 一辺の長さ
	m_pVtxBuff = nullptr;			 // 頂点バッファ
	m_pTexture = nullptr;			 // テクスチャ
}

//-----------------------------------------------------------------------------
//	デストラクタ
//-----------------------------------------------------------------------------
SkyBox::~SkyBox()
{
	Uninit();

}

//-----------------------------------------------------------------------------
//	生成
//-----------------------------------------------------------------------------
SkyBox* SkyBox::Create(D3DXVECTOR3 pos, float width)
{
	SkyBox* pSkyBox = new SkyBox(Object::TYPE::TYPE_3D_SKYBOX);
	pSkyBox->Init(pos, width);

	return pSkyBox;
}

//-----------------------------------------------------------------------------
//	初期化
//-----------------------------------------------------------------------------
HRESULT SkyBox::Init(D3DXVECTOR3 pos, float width)
{
	// 変数宣言
	D3DXMATRIX mtxTrans;
	D3DXMATRIX mtxRot;
	D3DXMATRIX mtxScall;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// メンバ変数設定
	Object::SetPosition(pos);
	m_width = width;

	// 行列の生成
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixScaling(&mtxScall, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.f, 0.f, 0.f);

	// 行列の合成
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScall);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 頂点バッファ領域の生成
	MakeVertexBuf();

	// 正常に初期化完了
	return S_OK;
}

//-----------------------------------------------------------------------------
//	終了
//-----------------------------------------------------------------------------
void SkyBox::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL) {
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

}

//-----------------------------------------------------------------------------
//	更新
//-----------------------------------------------------------------------------
void SkyBox::Update(void)
{
}

//-----------------------------------------------------------------------------
//	描画
//-----------------------------------------------------------------------------
void SkyBox::Draw(void)
{
	// 変数宣言
#ifdef _USE_JPEG_
	const int numPolygon = 2 * 5;
#else
	const int numPolygon = 2 * 6;
#endif

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// デバイスにワールド変換行列を設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ストリームの設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ貼り付け
	pDevice->SetTexture(0, m_pTexture->GetTexture());

	// ライトOFF
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, numPolygon);

	// ライトON
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//-----------------------------------------------------------------------------
// 座標の移動
//-----------------------------------------------------------------------------
void SkyBox::MovePos(D3DXVECTOR3 move)
{
	// 変数宣言
	D3DXVECTOR3 pos;
	D3DXMATRIX mtxTrans;
	D3DXMATRIX mtxRot;
	D3DXMATRIX mtxScall;

	// 座標の移動
	Object::MovePosition(move);
	pos = Object::GetPosition();

	// 行列の生成
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixScaling(&mtxScall, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.f, 0.f, 0.f);

	// 行列の合成
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScall);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
}

//-----------------------------------------------------------------------------
// 座標の設定
//-----------------------------------------------------------------------------
void SkyBox::SetPos(D3DXVECTOR3 pos)
{
	// 変数宣言
	D3DXMATRIX mtxTrans;
	D3DXMATRIX mtxRot;
	D3DXMATRIX mtxScall;

	// 座標の設定
	Object::SetPosition(pos);

	// 行列の生成
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixScaling(&mtxScall, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.f, 0.f, 0.f);

	// 行列の合成
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScall);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
}


//-----------------------------------------------------------------------------
// 頂点バッファの生成
//-----------------------------------------------------------------------------
void SkyBox::MakeVertexBuf()
{
	// 変数宣言
	D3DXVECTOR3 pos = GetPosition();
	float halfWidth = m_width * 0.5f;
#ifdef _USE_JPEG_
	const int numPolygon = 2 * 5;
	const int numVertex = 3 * 2 * 5;
	float texWidth = 1.0f / 4;
	float texHeight = 1.0f / 2;
#else
	const int numPolygon = 2 * 6;
	const int numVertex = 3 * 2 * 6;
	float texWidth = 1.0f / 4;
	float texHeight = 1.0f / 3;
#endif

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// 頂点バッファ領域の生成
	if (m_pVtxBuff == NULL) {
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * numVertex, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,
			D3DPOOL_MANAGED, &m_pVtxBuff, NULL))) {
			MessageBox(NULL, "頂点バッファ生成失敗", "SkyBox::Init", MB_OK);
		}
	}

	// 頂点バッファの設定
	VERTEX_3D* pVtx;

	// VRAMの仮想アドレス取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

#ifdef _USE_JPEG_
	//----- 頂点座標の設定 -----
	// 上面
	pVtx[0].pos = D3DXVECTOR3(pos.x + halfWidth, pos.y + halfWidth, pos.z - halfWidth); // 右上
	pVtx[1].pos = D3DXVECTOR3(pos.x + halfWidth, pos.y + halfWidth, pos.z + halfWidth); // 右下
	pVtx[2].pos = D3DXVECTOR3(pos.x - halfWidth, pos.y + halfWidth, pos.z - halfWidth); // 左上
	pVtx[3].pos = D3DXVECTOR3(pos.x - halfWidth, pos.y + halfWidth, pos.z - halfWidth); // 左上
	pVtx[4].pos = D3DXVECTOR3(pos.x + halfWidth, pos.y + halfWidth, pos.z + halfWidth); // 右下
	pVtx[5].pos = D3DXVECTOR3(pos.x - halfWidth, pos.y + halfWidth, pos.z + halfWidth); // 左下

	// 背面
	pVtx[6].pos = D3DXVECTOR3(pos.x + halfWidth, pos.y + halfWidth, pos.z + halfWidth); // 右上
	pVtx[7].pos = D3DXVECTOR3(pos.x + halfWidth, pos.y - halfWidth, pos.z + halfWidth); // 右下
	pVtx[8].pos = D3DXVECTOR3(pos.x - halfWidth, pos.y + halfWidth, pos.z + halfWidth); // 左上
	pVtx[9].pos = D3DXVECTOR3(pos.x - halfWidth, pos.y + halfWidth, pos.z + halfWidth); // 左上
	pVtx[10].pos = D3DXVECTOR3(pos.x + halfWidth, pos.y - halfWidth, pos.z + halfWidth); // 右下
	pVtx[11].pos = D3DXVECTOR3(pos.x - halfWidth, pos.y - halfWidth, pos.z + halfWidth); // 左下

	// 左面
	pVtx[12].pos = D3DXVECTOR3(pos.x - halfWidth, pos.y + halfWidth, pos.z + halfWidth); // 右上
	pVtx[13].pos = D3DXVECTOR3(pos.x - halfWidth, pos.y - halfWidth, pos.z + halfWidth); // 右下
	pVtx[14].pos = D3DXVECTOR3(pos.x - halfWidth, pos.y + halfWidth, pos.z - halfWidth); // 左上
	pVtx[15].pos = D3DXVECTOR3(pos.x - halfWidth, pos.y + halfWidth, pos.z - halfWidth); // 左上
	pVtx[16].pos = D3DXVECTOR3(pos.x - halfWidth, pos.y - halfWidth, pos.z + halfWidth); // 右下
	pVtx[17].pos = D3DXVECTOR3(pos.x - halfWidth, pos.y - halfWidth, pos.z - halfWidth); // 左下

	// 右面
	pVtx[18].pos = D3DXVECTOR3(pos.x + halfWidth, pos.y + halfWidth, pos.z - halfWidth); // 右上
	pVtx[19].pos = D3DXVECTOR3(pos.x + halfWidth, pos.y - halfWidth, pos.z - halfWidth); // 右下
	pVtx[20].pos = D3DXVECTOR3(pos.x + halfWidth, pos.y + halfWidth, pos.z + halfWidth); // 左上
	pVtx[21].pos = D3DXVECTOR3(pos.x + halfWidth, pos.y + halfWidth, pos.z + halfWidth); // 左上
	pVtx[22].pos = D3DXVECTOR3(pos.x + halfWidth, pos.y - halfWidth, pos.z - halfWidth); // 右下
	pVtx[23].pos = D3DXVECTOR3(pos.x + halfWidth, pos.y - halfWidth, pos.z + halfWidth); // 左下

	// 正面
	pVtx[24].pos = D3DXVECTOR3(pos.x - halfWidth, pos.y + halfWidth, pos.z - halfWidth); // 右上
	pVtx[25].pos = D3DXVECTOR3(pos.x - halfWidth, pos.y - halfWidth, pos.z - halfWidth); // 右下
	pVtx[26].pos = D3DXVECTOR3(pos.x + halfWidth, pos.y + halfWidth, pos.z - halfWidth); // 左上
	pVtx[27].pos = D3DXVECTOR3(pos.x + halfWidth, pos.y + halfWidth, pos.z - halfWidth); // 左上
	pVtx[28].pos = D3DXVECTOR3(pos.x - halfWidth, pos.y - halfWidth, pos.z - halfWidth); // 右下
	pVtx[29].pos = D3DXVECTOR3(pos.x + halfWidth, pos.y - halfWidth, pos.z - halfWidth); // 左下

	//----- 法線と頂点色の設定 -----
	for (int i = 0; i < numVertex; i++) {
		pVtx[i].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[i].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//----- テクスチャUV値の設定 -----
	// 上面
	pVtx[0].tex = D3DXVECTOR2(texWidth * 2, texHeight * 0); // 右上
	pVtx[1].tex = D3DXVECTOR2(texWidth * 2, texHeight * 1); // 右下
	pVtx[2].tex = D3DXVECTOR2(texWidth * 1, texHeight * 0); // 左上
	pVtx[3].tex = D3DXVECTOR2(texWidth * 1, texHeight * 0); // 左上
	pVtx[4].tex = D3DXVECTOR2(texWidth * 2, texHeight * 1); // 右下
	pVtx[5].tex = D3DXVECTOR2(texWidth * 1, texHeight * 1); // 左下

	// 背面
	pVtx[6].tex  = D3DXVECTOR2(texWidth * 2, texHeight * 1); // 右上
	pVtx[7].tex  = D3DXVECTOR2(texWidth * 2, texHeight * 2); // 右下
	pVtx[8].tex  = D3DXVECTOR2(texWidth * 1, texHeight * 1); // 左上
	pVtx[9].tex  = D3DXVECTOR2(texWidth * 1, texHeight * 1); // 左上
	pVtx[10].tex = D3DXVECTOR2(texWidth * 2, texHeight * 2); // 右下
	pVtx[11].tex = D3DXVECTOR2(texWidth * 1, texHeight * 2); // 左下

	// 左面
	pVtx[12].tex = D3DXVECTOR2(texWidth * 1, texHeight * 1); // 右上
	pVtx[13].tex = D3DXVECTOR2(texWidth * 1, texHeight * 2); // 右下
	pVtx[14].tex = D3DXVECTOR2(texWidth * 0, texHeight * 1); // 左上
	pVtx[15].tex = D3DXVECTOR2(texWidth * 0, texHeight * 1); // 左上
	pVtx[16].tex = D3DXVECTOR2(texWidth * 1, texHeight * 2); // 右下
	pVtx[17].tex = D3DXVECTOR2(texWidth * 0, texHeight * 2); // 左下

	// 右面
	pVtx[18].tex = D3DXVECTOR2(texWidth * 3, texHeight * 1); // 右上
	pVtx[19].tex = D3DXVECTOR2(texWidth * 3, texHeight * 2); // 右下
	pVtx[20].tex = D3DXVECTOR2(texWidth * 2, texHeight * 1); // 左上
	pVtx[21].tex = D3DXVECTOR2(texWidth * 2, texHeight * 1); // 左上
	pVtx[22].tex = D3DXVECTOR2(texWidth * 3, texHeight * 2); // 右下
	pVtx[23].tex = D3DXVECTOR2(texWidth * 2, texHeight * 2); // 左下

	// 正面
	pVtx[24].tex = D3DXVECTOR2(texWidth * 4, texHeight * 1); // 右上
	pVtx[25].tex = D3DXVECTOR2(texWidth * 4, texHeight * 2); // 右下
	pVtx[26].tex = D3DXVECTOR2(texWidth * 3, texHeight * 1); // 左上
	pVtx[27].tex = D3DXVECTOR2(texWidth * 3, texHeight * 1); // 左上
	pVtx[28].tex = D3DXVECTOR2(texWidth * 4, texHeight * 2); // 右下
	pVtx[29].tex = D3DXVECTOR2(texWidth * 3, texHeight * 2); // 左下
#else
	//----- 頂点座標の設定 -----
	// 上面
	pVtx[0].pos = D3DXVECTOR3(+ halfWidth, + halfWidth, - halfWidth); // 右上
	pVtx[1].pos = D3DXVECTOR3(+ halfWidth, + halfWidth, + halfWidth); // 右下
	pVtx[2].pos = D3DXVECTOR3(- halfWidth, + halfWidth, - halfWidth); // 左上
	pVtx[3].pos = D3DXVECTOR3(- halfWidth, + halfWidth, - halfWidth); // 左上
	pVtx[4].pos = D3DXVECTOR3(+ halfWidth, + halfWidth, + halfWidth); // 右下
	pVtx[5].pos = D3DXVECTOR3(- halfWidth, + halfWidth, + halfWidth); // 左下

	// 背面
	pVtx[6].pos  = D3DXVECTOR3(+ halfWidth, + halfWidth, + halfWidth); // 右上
	pVtx[7].pos  = D3DXVECTOR3(+ halfWidth, - halfWidth, + halfWidth); // 右下
	pVtx[8].pos  = D3DXVECTOR3(- halfWidth, + halfWidth, + halfWidth); // 左上
	pVtx[9].pos  = D3DXVECTOR3(- halfWidth, + halfWidth, + halfWidth); // 左上
	pVtx[10].pos = D3DXVECTOR3(+ halfWidth, - halfWidth, + halfWidth); // 右下
	pVtx[11].pos = D3DXVECTOR3(- halfWidth, - halfWidth, + halfWidth); // 左下

	// 左面
	pVtx[12].pos = D3DXVECTOR3(- halfWidth, + halfWidth, + halfWidth); // 右上
	pVtx[13].pos = D3DXVECTOR3(- halfWidth, - halfWidth, + halfWidth); // 右下
	pVtx[14].pos = D3DXVECTOR3(- halfWidth, + halfWidth, - halfWidth); // 左上
	pVtx[15].pos = D3DXVECTOR3(- halfWidth, + halfWidth, - halfWidth); // 左上
	pVtx[16].pos = D3DXVECTOR3(- halfWidth, - halfWidth, + halfWidth); // 右下
	pVtx[17].pos = D3DXVECTOR3(- halfWidth, - halfWidth, - halfWidth); // 左下

	// 右面
	pVtx[18].pos = D3DXVECTOR3(+ halfWidth, + halfWidth, - halfWidth); // 右上
	pVtx[19].pos = D3DXVECTOR3(+ halfWidth, - halfWidth, - halfWidth); // 右下
	pVtx[20].pos = D3DXVECTOR3(+ halfWidth, + halfWidth, + halfWidth); // 左上
	pVtx[21].pos = D3DXVECTOR3(+ halfWidth, + halfWidth, + halfWidth); // 左上
	pVtx[22].pos = D3DXVECTOR3(+ halfWidth, - halfWidth, - halfWidth); // 右下
	pVtx[23].pos = D3DXVECTOR3(+ halfWidth, - halfWidth, + halfWidth); // 左下

	// 正面
	pVtx[24].pos = D3DXVECTOR3(- halfWidth, + halfWidth, - halfWidth); // 右上
	pVtx[25].pos = D3DXVECTOR3(- halfWidth, - halfWidth, - halfWidth); // 右下
	pVtx[26].pos = D3DXVECTOR3(+ halfWidth, + halfWidth, - halfWidth); // 左上
	pVtx[27].pos = D3DXVECTOR3(+ halfWidth, + halfWidth, - halfWidth); // 左上
	pVtx[28].pos = D3DXVECTOR3(- halfWidth, - halfWidth, - halfWidth); // 右下
	pVtx[29].pos = D3DXVECTOR3(+ halfWidth, - halfWidth, - halfWidth); // 左下

	// 下面
	pVtx[30].pos = D3DXVECTOR3(+ halfWidth, - halfWidth, + halfWidth); // 右上
	pVtx[31].pos = D3DXVECTOR3(+ halfWidth, - halfWidth, - halfWidth); // 右下
	pVtx[32].pos = D3DXVECTOR3(- halfWidth, - halfWidth, + halfWidth); // 左上
	pVtx[33].pos = D3DXVECTOR3(- halfWidth, - halfWidth, + halfWidth); // 左上
	pVtx[34].pos = D3DXVECTOR3(+ halfWidth, - halfWidth, - halfWidth); // 右下
	pVtx[35].pos = D3DXVECTOR3(- halfWidth, - halfWidth, - halfWidth); // 左下

	//----- 法線と頂点色の設定 -----
	for (int i = 0; i < numVertex; i++) {
		pVtx[i].normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[i].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//----- テクスチャUV値の設定 -----
	// 上面
	pVtx[0].tex = D3DXVECTOR2(texWidth * 2, texHeight * 0); // 右上
	pVtx[1].tex = D3DXVECTOR2(texWidth * 2, texHeight * 1); // 右下
	pVtx[2].tex = D3DXVECTOR2(texWidth * 1, texHeight * 0); // 左上
	pVtx[3].tex = D3DXVECTOR2(texWidth * 1, texHeight * 0); // 左上
	pVtx[4].tex = D3DXVECTOR2(texWidth * 2, texHeight * 1); // 右下
	pVtx[5].tex = D3DXVECTOR2(texWidth * 1, texHeight * 1); // 左下

	// 背面
	pVtx[6].tex  = D3DXVECTOR2(texWidth * 2, texHeight * 1); // 右上
	pVtx[7].tex  = D3DXVECTOR2(texWidth * 2, texHeight * 2); // 右下
	pVtx[8].tex  = D3DXVECTOR2(texWidth * 1, texHeight * 1); // 左上
	pVtx[9].tex  = D3DXVECTOR2(texWidth * 1, texHeight * 1); // 左上
	pVtx[10].tex = D3DXVECTOR2(texWidth * 2, texHeight * 2); // 右下
	pVtx[11].tex = D3DXVECTOR2(texWidth * 1, texHeight * 2); // 左下

	// 左面
	pVtx[12].tex = D3DXVECTOR2(texWidth * 1, texHeight * 1); // 右上
	pVtx[13].tex = D3DXVECTOR2(texWidth * 1, texHeight * 2); // 右下
	pVtx[14].tex = D3DXVECTOR2(texWidth * 0, texHeight * 1); // 左上
	pVtx[15].tex = D3DXVECTOR2(texWidth * 0, texHeight * 1); // 左上
	pVtx[16].tex = D3DXVECTOR2(texWidth * 1, texHeight * 2); // 右下
	pVtx[17].tex = D3DXVECTOR2(texWidth * 0, texHeight * 2); // 左下

	// 右面
	pVtx[18].tex = D3DXVECTOR2(texWidth * 3, texHeight * 1); // 右上
	pVtx[19].tex = D3DXVECTOR2(texWidth * 3, texHeight * 2); // 右下
	pVtx[20].tex = D3DXVECTOR2(texWidth * 2, texHeight * 1); // 左上
	pVtx[21].tex = D3DXVECTOR2(texWidth * 2, texHeight * 1); // 左上
	pVtx[22].tex = D3DXVECTOR2(texWidth * 3, texHeight * 2); // 右下
	pVtx[23].tex = D3DXVECTOR2(texWidth * 2, texHeight * 2); // 左下

	// 正面
	pVtx[24].tex = D3DXVECTOR2(texWidth * 4, texHeight * 1); // 右上
	pVtx[25].tex = D3DXVECTOR2(texWidth * 4, texHeight * 2); // 右下
	pVtx[26].tex = D3DXVECTOR2(texWidth * 3, texHeight * 1); // 左上
	pVtx[27].tex = D3DXVECTOR2(texWidth * 3, texHeight * 1); // 左上
	pVtx[28].tex = D3DXVECTOR2(texWidth * 4, texHeight * 2); // 右下
	pVtx[29].tex = D3DXVECTOR2(texWidth * 3, texHeight * 2); // 左下

	// 下面
	pVtx[30].tex = D3DXVECTOR2(texWidth * 2, texHeight * 2); // 右上
	pVtx[31].tex = D3DXVECTOR2(texWidth * 2, texHeight * 3); // 右下
	pVtx[32].tex = D3DXVECTOR2(texWidth * 1, texHeight * 2); // 左上
	pVtx[33].tex = D3DXVECTOR2(texWidth * 1, texHeight * 2); // 左上
	pVtx[34].tex = D3DXVECTOR2(texWidth * 2, texHeight * 3); // 右下
	pVtx[35].tex = D3DXVECTOR2(texWidth * 1, texHeight * 3); // 左下
#endif

	// VRAMの仮想アドレス解放
	m_pVtxBuff->Unlock();
}