//*****************************************************************************
//
//		2Dポリゴン
//												Autohr : Yusuke Seki
//*****************************************************************************
#include "scene2D.h"
#include "renderer.h"
#include "texture.h"


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
Scene2D::Scene2D(int priority, TYPE type):Scene(priority, type)
{
	// メンバ変数初期化
	ZeroMemory( &m_halfSize , sizeof(D3DXVECTOR3) );	// 半分の大きさ
	m_pVtxBuff = NULL;									// 頂点バッファ
	m_pTexture = NULL;									// テクスチャポインタ
	m_color.color = 0xffffffff;

	m_pTexture = nullptr;
}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
Scene2D::~Scene2D()
{
	// 頂点バッファの破棄
	if( m_pVtxBuff != NULL ){
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

}


//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
Scene2D* Scene2D::Create( D3DXVECTOR3 pos , D3DXVECTOR3 size )
{
	Scene2D* pScene2D = new Scene2D(2, TYPE_2D);
	pScene2D->Init( pos , size );

	return pScene2D;
}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
HRESULT Scene2D::Init( D3DXVECTOR3 pos , D3DXVECTOR3 size )
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// 位置を設定
	Scene::SetPos(pos);

	// 大きさを設定
	m_halfSize = size * 0.5f;

	// 頂点バッファの生成
	if( m_pVtxBuff == NULL ){
		if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_2D ) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
										  D3DPOOL_MANAGED, &m_pVtxBuff, NULL ) )){
			MessageBox( NULL , "頂点バッファ生成失敗" , "Scene2D::Init( D3DXVECTOR3 pos , D3DXVECTOR3 size )" , MB_OK );
		}
	}

	// 頂点情報を設定
	VERTEX_2D* pVtx;

	// VRAMの仮想アドレス取得
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx, 0 );

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3( pos.x - m_halfSize.x , pos.y - m_halfSize.y , 0.0f);
	pVtx[1].pos = D3DXVECTOR3( pos.x + m_halfSize.x , pos.y - m_halfSize.y , 0.0f);
	pVtx[2].pos = D3DXVECTOR3( pos.x - m_halfSize.x , pos.y + m_halfSize.y , 0.0f);
	pVtx[3].pos = D3DXVECTOR3( pos.x + m_halfSize.x , pos.y + m_halfSize.y , 0.0f);

	// 
	pVtx[0].rhw = 
	pVtx[1].rhw = 
	pVtx[2].rhw = 
	pVtx[3].rhw = 1.0f;

	// 頂点色の設定
	pVtx[0].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	pVtx[1].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	pVtx[2].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	pVtx[3].color = D3DCOLOR_RGBA( 255, 255, 255, 255 );

	// テクスチャUV値の設定
	pVtx[0].tex = D3DXVECTOR2( 0.0f , 0.0f );
	pVtx[1].tex = D3DXVECTOR2( 1.0f , 0.0f );
	pVtx[2].tex = D3DXVECTOR2( 0.0f , 1.0f );
	pVtx[3].tex = D3DXVECTOR2( 1.0f , 1.0f );

	// VRAMの仮想アドレス解放
	m_pVtxBuff->Unlock();

	return S_OK;
}


//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void Scene2D::Uninit(void)
{
	// 頂点バッファの破棄
	if( m_pVtxBuff != NULL ){
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

}


//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void Scene2D::Update(void)
{
}


//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void Scene2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource( 0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_2D );

	// テクスチャの設定
	if (m_pTexture != nullptr) pDevice->SetTexture(0, m_pTexture->GetTexture());
	else                       pDevice->SetTexture(0, nullptr);

	// ポリゴンの描画
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP , 0 , 2 );				
}


//-----------------------------------------------------------------------------
// 座標の移動
//-----------------------------------------------------------------------------
void Scene2D::MovePos( D3DXVECTOR3 move )
{
	// 座標の移動
	Scene::MovePos(move);
	D3DXVECTOR3 newPos = Scene::GetPos();

	// 頂点バッファの書き換え
	VERTEX_2D* pVtx;

	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx, 0 );

	pVtx[0].pos = D3DXVECTOR3( newPos.x - m_halfSize.x , newPos.y - m_halfSize.y , 0.0f);
	pVtx[1].pos = D3DXVECTOR3( newPos.x + m_halfSize.x , newPos.y - m_halfSize.y , 0.0f);
	pVtx[2].pos = D3DXVECTOR3( newPos.x - m_halfSize.x , newPos.y + m_halfSize.y , 0.0f);
	pVtx[3].pos = D3DXVECTOR3( newPos.x + m_halfSize.x , newPos.y + m_halfSize.y , 0.0f);

	m_pVtxBuff->Unlock();
}


//-----------------------------------------------------------------------------
// 座標の設定
//-----------------------------------------------------------------------------
void Scene2D::SetPos( D3DXVECTOR3 pos )
{
	// 座標の移動
	Scene::SetPos(pos);
	D3DXVECTOR3 newPos = Scene::GetPos();

	// 頂点バッファの書き換え
	VERTEX_2D* pVtx;

	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx, 0 );

	pVtx[0].pos = D3DXVECTOR3( newPos.x - m_halfSize.x , newPos.y - m_halfSize.y , 0.0f);
	pVtx[1].pos = D3DXVECTOR3( newPos.x + m_halfSize.x , newPos.y - m_halfSize.y , 0.0f);
	pVtx[2].pos = D3DXVECTOR3( newPos.x - m_halfSize.x , newPos.y + m_halfSize.y , 0.0f);
	pVtx[3].pos = D3DXVECTOR3( newPos.x + m_halfSize.x , newPos.y + m_halfSize.y , 0.0f);

	m_pVtxBuff->Unlock();
}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void Scene2D::SetColor(unsigned int rgba)
{
	// 色設定
	m_color.color = rgba;

	// 頂点情報を設定
	VERTEX_2D* pVtx;

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


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void Scene2D::SetUV(float U, float V, float width, float height)
{
	// 頂点情報を設定
	VERTEX_2D* pVtx;

	// VRAMの仮想アドレス取得
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx, 0 );

	// テクスチャUV値の設定
	pVtx[0].tex = D3DXVECTOR2( U        , V          );
	pVtx[1].tex = D3DXVECTOR2( U + width, V          );
	pVtx[2].tex = D3DXVECTOR2( U        , V + height );
	pVtx[3].tex = D3DXVECTOR2( U + width, V + height );

	// VRAMの仮想アドレス解放
	m_pVtxBuff->Unlock();
}


