//*****************************************************************************
//
//		3Dポリゴン
//												Autohr	:	Yusuke Seki
//*****************************************************************************
#include "scene3D.h"
#include "renderer.h"


//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
Scene3D::Scene3D(int priority, TYPE type):Scene(priority, type)
{
	// メンバ変数初期化
	D3DXMatrixIdentity( &m_mtxWorld );
	m_rot  = D3DXVECTOR3(0, 0, 0);
	m_scl  = D3DXVECTOR3(0, 0, 0);
	m_size = D3DXVECTOR3(0, 0, 0);
	m_pVtxBuff = nullptr;
	m_pTexture = nullptr;
	m_bDraw = true;

}


//-----------------------------------------------------------------------------
//	デストラクタ
//-----------------------------------------------------------------------------
Scene3D::~Scene3D()
{
	// 頂点バッファの破棄
	if( m_pVtxBuff != nullptr){
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

}


//-----------------------------------------------------------------------------
//	生成
//-----------------------------------------------------------------------------
Scene3D* Scene3D::Create( D3DXVECTOR3 pos , D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXVECTOR3 scl )
{
	Scene3D* pScene3D = new Scene3D(1, TYPE_3D);
	pScene3D->Init( pos, size, rot, scl );

	return pScene3D;
}


//-----------------------------------------------------------------------------
//	初期化
//-----------------------------------------------------------------------------
HRESULT Scene3D::Init( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXVECTOR3 scl )
{
	D3DXMATRIX mtxTrans, mtxRot, mtxScall;
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();
	VERTEX_3D* pVtx;
	D3DXVECTOR3 halfSize = size * 0.5f;


	// メンバ変数設定
	SetPos(pos);
	m_size = size;
	m_rot  = rot;
	m_scl  = scl;
	m_bDraw = true;


	// 頂点バッファ領域の生成
	if( m_pVtxBuff == nullptr ){
		if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D ) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,
										  D3DPOOL_MANAGED, &m_pVtxBuff, nullptr ) )){
			_MSGERROR("Failed Create Vertex Buf!!", "HRESULT Scene3D::Init( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXVECTOR3 scl )");
		}
	}


	// VRAMの仮想アドレス取得
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx, 0 );

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-halfSize.x,  halfSize.y,  halfSize.z);	// 左上
	pVtx[1].pos = D3DXVECTOR3( halfSize.x,  halfSize.y,  halfSize.z);	// 右上
	pVtx[2].pos = D3DXVECTOR3(-halfSize.x, -halfSize.y, -halfSize.z);	// 左下
	pVtx[3].pos = D3DXVECTOR3( halfSize.x, -halfSize.y, -halfSize.z);	// 右下

	// 法線の設定
	pVtx[0].normal = D3DXVECTOR3( 0.0f , 0.0f , 1.0f );
	pVtx[1].normal = D3DXVECTOR3( 0.0f , 0.0f , 1.0f );
	pVtx[2].normal = D3DXVECTOR3( 0.0f , 0.0f , 1.0f );
	pVtx[3].normal = D3DXVECTOR3( 0.0f , 0.0f , 1.0f );

	// 頂点色の設定
	pVtx[0].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	pVtx[1].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	pVtx[2].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	pVtx[3].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );

	// テクスチャUV値の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	// VRAMの仮想アドレス解放
	m_pVtxBuff->Unlock();


	// 各行列計算
	D3DXMatrixScaling(&mtxScall, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	// 行列の合成
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScall);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);


	return S_OK;
}


//-----------------------------------------------------------------------------
//	終了
//-----------------------------------------------------------------------------
void Scene3D::Uninit(void)
{
	// 頂点バッファの破棄
	if( m_pVtxBuff != nullptr ){
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

}


//-----------------------------------------------------------------------------
//	更新
//-----------------------------------------------------------------------------
void Scene3D::Update(void)
{
}


//-----------------------------------------------------------------------------
//	描画
//-----------------------------------------------------------------------------
void Scene3D::Draw(void)
{
	if (!m_bDraw) return;

	D3DXVECTOR3 pos = GetPos();
	D3DXMATRIX mtxTrans, mtxRot, mtxScall;


	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// ワールド行列を求める
	D3DXMatrixScaling(&mtxScall, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScall);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// デバイスにワールド変換行列を設定
	pDevice->SetTransform( D3DTS_WORLD , &m_mtxWorld );

	// ストリームの設定
	pDevice->SetStreamSource( 0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_3D );

	// テクスチャ貼り付け
	if (m_pTexture != nullptr) pDevice->SetTexture(0, m_pTexture->GetTexture());
	else                       pDevice->SetTexture(0, nullptr);

	// 描画
	pDevice->DrawPrimitive(	D3DPT_TRIANGLESTRIP, 0, 2);
}



//=============================================================================
//	増減
// 大きさ
void Scene3D::MoveSize(D3DXVECTOR3 move)
{
	D3DXVECTOR3 halfSize;
	VERTEX_3D* pVtx;


	// 大きさを更新
	m_size  += move;
	halfSize = m_size * 0.5f;

	// VRAMの仮想アドレス取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-halfSize.x,  halfSize.y,  halfSize.z);	// 左上
	pVtx[1].pos = D3DXVECTOR3( halfSize.x,  halfSize.y,  halfSize.z);	// 右上
	pVtx[2].pos = D3DXVECTOR3(-halfSize.x, -halfSize.y, -halfSize.z);	// 左下
	pVtx[3].pos = D3DXVECTOR3( halfSize.x, -halfSize.y, -halfSize.z);	// 右下

	// VRAMの仮想アドレス解放
	m_pVtxBuff->Unlock();
}


//=============================================================================
//	設定
// 大きさ
void Scene3D::SetSize(D3DXVECTOR3 size)
{
	D3DXVECTOR3 halfSize;
	VERTEX_3D* pVtx;


	// 大きさを更新
	m_size   = size;
	halfSize = m_size * 0.5f;

	// VRAMの仮想アドレス取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-halfSize.x,  halfSize.y,  halfSize.z);	// 左上
	pVtx[1].pos = D3DXVECTOR3( halfSize.x,  halfSize.y,  halfSize.z);	// 右上
	pVtx[2].pos = D3DXVECTOR3(-halfSize.x, -halfSize.y, -halfSize.z);	// 左下
	pVtx[3].pos = D3DXVECTOR3( halfSize.x, -halfSize.y, -halfSize.z);	// 右下

	// VRAMの仮想アドレス解放
	m_pVtxBuff->Unlock();

}


// 法線
void Scene3D::SetNormal(D3DXVECTOR3 normalLT, D3DXVECTOR3 normalRT, D3DXVECTOR3 normalLB, D3DXVECTOR3 normalRB)
{
	// 頂点情報を設定
	VERTEX_3D* pVtx;

	// VRAMの仮想アドレス取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点色の設定
	pVtx[0].normal = normalLT;
	pVtx[1].normal = normalRT;
	pVtx[2].normal = normalLB;
	pVtx[3].normal = normalRB;

	// VRAMの仮想アドレス解放
	m_pVtxBuff->Unlock();

}


// 色
void Scene3D::SetColor(unsigned int color)
{
	m_color.color = color;

	// 頂点情報を設定
	VERTEX_3D* pVtx;

	// VRAMの仮想アドレス取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点色の設定
	pVtx[0].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
	pVtx[1].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
	pVtx[2].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
	pVtx[3].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);

	// VRAMの仮想アドレス解放
	m_pVtxBuff->Unlock();


}


