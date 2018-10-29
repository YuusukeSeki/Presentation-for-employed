//*****************************************************************************
//
//		2Dポリゴン（リスト構造）
//												Autohr : Yusuke Seki
//*****************************************************************************
#include "Object2D.h"
#include "renderer.h"
#include "texture.h"


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
Object2D::Object2D() :Object(Object::TYPE::TYPE_2D)
{
	// メンバ変数初期化
	m_halfSize = D3DXVECTOR3(0, 0, 0);
	m_color.color = 0xffffffff;
	m_UV_StartPoint = D3DXVECTOR2(0, 0);
	m_UV_Size = D3DXVECTOR2(0, 0);
	m_radius = 0;
	m_angle = 0;
	m_rotate = 0;
	m_pVtxBuff = nullptr;
	m_pTexture = nullptr;
	m_bUpdateVertexBuf = false;
	m_bDraw = true;
}


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
Object2D::Object2D(Object::TYPE type) :Object(type)
{
	// メンバ変数初期化
	m_halfSize         = D3DXVECTOR3(0, 0, 0);
	m_color.color      = 0xffffffff;
	m_UV_StartPoint    = D3DXVECTOR2(0, 0);
	m_UV_Size          = D3DXVECTOR2(0, 0);
	m_radius           = 0;
	m_angle            = 0;
	m_rotate           = 0;
	m_pVtxBuff         = nullptr;
	m_pTexture         = nullptr;
	m_bUpdateVertexBuf = false;
	m_bDraw            = true;
}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
Object2D::~Object2D()
{
	// 終了処理
	Uninit();

}


//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
Object2D* Object2D::Create(D3DXVECTOR3& pos, D3DXVECTOR3& size)
{
	// 生成
	Object2D* pObject2D = new Object2D(Object::TYPE::TYPE_2D);

	// 初期化
	pObject2D->Init(pos, size);

	return pObject2D;
}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void Object2D::Init(D3DXVECTOR3& position, D3DXVECTOR3& size)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// データを設定
	Object::SetPosition(position);															// 座標
	m_halfSize         = size * 0.5f;														// 半分の大きさ
	m_radius           = sqrtf(m_halfSize.x * m_halfSize.x + m_halfSize.y * m_halfSize.y);	// 半径
	m_angle            = atan2f(size.y, size.x);											// atan
	m_rotate           = 0;																	// 回転率
	m_color.color      = 0xffffffff;														// 色
	m_UV_StartPoint    = D3DXVECTOR2(0, 0);													// UV開始点
	m_UV_Size          = D3DXVECTOR2(1, 1);													// UV大きさ
	m_bUpdateVertexBuf = false;																// true で頂点バッファの更新処理をする
	m_bDraw            = true;																// true で描画処理を行う

	// 頂点バッファの生成
	this->MakeVertexBuf();

}


//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void Object2D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

}


//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void Object2D::Draw(void)
{
	// false で描画処理を行わない
	if (!m_bDraw) return;

	// true で頂点バッファの更新処理を行う
	if (m_bUpdateVertexBuf) this->UpdateVertexBuf();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}



//=============================================================================
//	増減処理
// 座標の移動
void Object2D::MovePosition(D3DXVECTOR3& movePosition)
{
	// 座標の移動
	Object::MovePosition(movePosition);

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// 大きさの増減
void Object2D::MoveSize(D3DXVECTOR3& moveSize)
{
	// 大きさの増減
	m_halfSize += moveSize * 0.5f;
	m_radius    = sqrtf(m_halfSize.x * m_halfSize.x + m_halfSize.y * m_halfSize.y);	// 半径
	m_angle     = atan2f(m_halfSize.y, m_halfSize.x);								// atan
	
	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// 回転率の増減
void Object2D::MoveRotate(float moveRotate)
{
	// 回転率の増減
	m_rotate += moveRotate;
	
	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// 色の増減
void Object2D::MoveColor(int r, int g, int b, int a)
{
	// 色の増減
	m_color.rgba[3] = m_color.rgba[3] + r >= 255 ? 255 : m_color.rgba[3] + r;
	m_color.rgba[2] = m_color.rgba[2] + g >= 255 ? 255 : m_color.rgba[2] + g;
	m_color.rgba[1] = m_color.rgba[1] + b >= 255 ? 255 : m_color.rgba[1] + b;
	m_color.rgba[0] = m_color.rgba[0] + a >= 255 ? 255 : m_color.rgba[0] + a;
	m_color.rgba[3] = m_color.rgba[3] + r <=   0 ?   0 : m_color.rgba[3] + r;
	m_color.rgba[2] = m_color.rgba[2] + g <=   0 ?   0 : m_color.rgba[2] + g;
	m_color.rgba[1] = m_color.rgba[1] + b <=   0 ?   0 : m_color.rgba[1] + b;
	m_color.rgba[0] = m_color.rgba[0] + a <=   0 ?   0 : m_color.rgba[0] + a;

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// UV開始点の増減
void Object2D::MoveUV_StartPoint(D3DXVECTOR2& moveUV_StartPoint)
{
	// UV開始点の増減
	m_UV_StartPoint += moveUV_StartPoint;

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// UV大きさの増減
void Object2D::MoveUV_Size(D3DXVECTOR2& moveUV_Size)
{
	// UV開始点の増減
	m_UV_Size += moveUV_Size;

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}



//=============================================================================
// 設定処理
// 座標の設定
void Object2D::SetPosition(D3DXVECTOR3& position)
{
	// 座標の移動
	Object::SetPosition(position);

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// 大きさの設定
void Object2D::SetSize(D3DXVECTOR3& size)
{
	// 大きさの設定
	m_halfSize = size * 0.5f;
	m_radius   = sqrtf(m_halfSize.x * m_halfSize.x + m_halfSize.y * m_halfSize.y);	// 半径
	m_angle    = atan2f(m_halfSize.y, m_halfSize.x);								// atan

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// 回転率の設定
void Object2D::SetRotate(float rotate)
{
	// 回転率の増減
	m_rotate = rotate;

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// 色の設定
void Object2D::SetColor(unsigned int rgba)
{
	// 色設定
	m_color.color = rgba;

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// UV開始点の設定
void Object2D::SetUV_StartPoint(D3DXVECTOR2& UV_StartPoint)
{
	// UV開始点の設定
	m_UV_StartPoint = UV_StartPoint;

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// UVの大きさの設定
void Object2D::SetUV_Size(D3DXVECTOR2& UV_Size)
{
	// UVの大きさの設定
	m_UV_Size = UV_Size;

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}


//=============================================================================
//	private関数
// 頂点バッファの生成
void Object2D::MakeVertexBuf()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// 頂点バッファを生成
	if (m_pVtxBuff == nullptr) {
		// 頂点生成に失敗した？
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL))) {
			// エラーメッセージの表示
			_MSGERROR("Failed Create Vertex Buffer!!", "void Object2D::MakeVertexBuf()");

			// このオブジェクトを破棄する
			this->Release();

			return;
		}
	}

	// VRAMの仮想アドレス取得
	VERTEX_2D* pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	D3DXVECTOR3 position = Object::GetPosition();
	pVtx[0].pos = D3DXVECTOR3(position.x - m_halfSize.x, position.y - m_halfSize.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(position.x + m_halfSize.x, position.y - m_halfSize.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(position.x - m_halfSize.x, position.y + m_halfSize.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(position.x + m_halfSize.x, position.y + m_halfSize.y, 0.0f);

	// 
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点色の設定
	pVtx[0].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
	pVtx[1].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
	pVtx[2].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
	pVtx[3].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);

	// テクスチャUV値の設定
	pVtx[0].tex = D3DXVECTOR2(m_UV_StartPoint.x              , m_UV_StartPoint.y              );
	pVtx[1].tex = D3DXVECTOR2(m_UV_StartPoint.x + m_UV_Size.x, m_UV_StartPoint.y              );
	pVtx[2].tex = D3DXVECTOR2(m_UV_StartPoint.x              , m_UV_StartPoint.y + m_UV_Size.y);
	pVtx[3].tex = D3DXVECTOR2(m_UV_StartPoint.x + m_UV_Size.x, m_UV_StartPoint.y + m_UV_Size.y);

	// VRAMの仮想アドレス解放
	m_pVtxBuff->Unlock();
}

// 頂点バッファの更新
void Object2D::UpdateVertexBuf()
{
	// 変数の定義
	D3DXVECTOR3 position = Object::GetPosition();
	VERTEX_2D* pVtx;

	// VRAMの仮想アドレス取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3( position.x + cosf(m_rotate + m_angle + D3DX_PI) * m_radius
							 , position.y + sinf(m_rotate + m_angle + D3DX_PI) * m_radius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3( position.x + cosf(m_rotate - m_angle          ) * m_radius
							 , position.y + sinf(m_rotate - m_angle          ) * m_radius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3( position.x + cosf(m_rotate - m_angle + D3DX_PI) * m_radius
							 , position.y + sinf(m_rotate - m_angle + D3DX_PI) * m_radius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3( position.x + cosf(m_rotate + m_angle          ) * m_radius
							 , position.y + sinf(m_rotate + m_angle          ) * m_radius, 0.0f);

	// 頂点色の設定
	pVtx[0].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
	pVtx[1].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
	pVtx[2].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
	pVtx[3].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);

	// テクスチャUV値の設定
	pVtx[0].tex = D3DXVECTOR2(m_UV_StartPoint.x              , m_UV_StartPoint.y              );
	pVtx[1].tex = D3DXVECTOR2(m_UV_StartPoint.x + m_UV_Size.x, m_UV_StartPoint.y              );
	pVtx[2].tex = D3DXVECTOR2(m_UV_StartPoint.x              , m_UV_StartPoint.y + m_UV_Size.y);
	pVtx[3].tex = D3DXVECTOR2(m_UV_StartPoint.x + m_UV_Size.x, m_UV_StartPoint.y + m_UV_Size.y);

	// VRAMの仮想アドレス解放
	m_pVtxBuff->Unlock();


	// 頂点バッファの更新フラグOFF
	m_bUpdateVertexBuf = false;

}

