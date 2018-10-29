//*****************************************************************************
//
//		3Dポリゴン（リスト構造）
//												Autohr : Yusuke Seki
//*****************************************************************************
#include "Object3D_Quaternion.h"
#include "renderer.h"
#include "texture.h"


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
Object3D_Quaternion::Object3D_Quaternion() :Object(Object::TYPE::TYPE_3D)
{
	// メンバ変数初期化
	m_halfSize           = D3DXVECTOR3(0, 0, 0);
	m_quaternion         = D3DXQUATERNION(0, 0, 0, 1);
	m_scale              = D3DXVECTOR3(0, 0, 0);
	m_radius             = 0;
	m_normal[0]          = D3DXVECTOR3(0, 0, 0);
	m_normal[1]          = D3DXVECTOR3(0, 0, 0);
	m_normal[2]          = D3DXVECTOR3(0, 0, 0);
	m_normal[3]          = D3DXVECTOR3(0, 0, 0);
	m_color.color        = 0xffffffff;
	m_UV_StartPoint      = D3DXVECTOR2(0, 0);
	m_UV_Size            = D3DXVECTOR2(0, 0);
	m_pVtxBuff           = nullptr;
	m_pTexture           = nullptr;
	m_bUpdateVertexBuf   = false;
	m_bUpdateWorldMatrix = false;
	m_bDraw              = true;
}


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
Object3D_Quaternion::Object3D_Quaternion(Object::TYPE type) :Object(type)
{
	// メンバ変数初期化
	m_halfSize           = D3DXVECTOR3(0, 0, 0);
	m_quaternion         = D3DXQUATERNION(0, 0, 0, 1);
	m_scale              = D3DXVECTOR3(0, 0, 0);
	m_radius             = 0;
	m_normal[0]          = D3DXVECTOR3(0, 0, 0);
	m_normal[1]          = D3DXVECTOR3(0, 0, 0);
	m_normal[2]          = D3DXVECTOR3(0, 0, 0);
	m_normal[3]          = D3DXVECTOR3(0, 0, 0);
	m_color.color        = 0xffffffff;
	m_UV_StartPoint      = D3DXVECTOR2(0, 0);
	m_UV_Size            = D3DXVECTOR2(0, 0);
	m_pVtxBuff           = nullptr;
	m_pTexture           = nullptr;
	m_bUpdateVertexBuf   = false;
	m_bUpdateWorldMatrix = false;
	m_bDraw              = true;
}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
Object3D_Quaternion::~Object3D_Quaternion()
{
	// 終了処理
	Uninit();

}


//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
Object3D_Quaternion* Object3D_Quaternion::Create(D3DXVECTOR3& pos, D3DXVECTOR3& size)
{
	// 生成
	Object3D_Quaternion* pObject3D_Quaternion = new Object3D_Quaternion(Object::TYPE::TYPE_3D);

	// 初期化
	pObject3D_Quaternion->Init(pos, size);

	return pObject3D_Quaternion;
}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void Object3D_Quaternion::Init(D3DXVECTOR3& position, D3DXVECTOR3& size)
{
	//----- データの設定 -----
	Object::SetPosition(position);																// 座標
	m_halfSize           = size * 0.5f;															// 半分の大きさ
	D3DXQuaternionIdentity(&m_quaternion);														// 回転率
	m_scale              = D3DXVECTOR3(1, 1, 1);												// 拡縮率
	m_radius             = sqrtf(m_halfSize.x * m_halfSize.x + m_halfSize.y * m_halfSize.y);	// 半径
	m_normal[0]          = D3DXVECTOR3(0, 0, -1);												// 法線
	m_normal[1]          = D3DXVECTOR3(0, 0, -1);												// 法線
	m_normal[2]          = D3DXVECTOR3(0, 0, -1);												// 法線
	m_normal[3]          = D3DXVECTOR3(0, 0, -1);												// 法線
	m_color.color        = 0xffffffff;															// 色
	m_UV_StartPoint      = D3DXVECTOR2(0, 0);													// UV開始点
	m_UV_Size            = D3DXVECTOR2(1, 1);													// UV大きさ
	m_bUpdateVertexBuf   = false;																// true で頂点バッファの更新する
	m_bUpdateWorldMatrix = false;																// true でワールドマトリクスの更新する
	m_bDraw              = true;																// true で描画処理を行う

	// 頂点バッファの生成
	this->MakeVertexBuf();

	// 行列を算出
	this->UpdateWorldMatrix();

}


//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void Object3D_Quaternion::Uninit(void)
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
void Object3D_Quaternion::Draw(void)
{
	// false で描画処理を行わない
	if (!m_bDraw) return;

	// true で頂点バッファの更新処理をする
	if (m_bUpdateVertexBuf) this->UpdateVertexBuf();

	// true でワールドマトリクスの更新処理をする
	if (m_bUpdateWorldMatrix) this->UpdateWorldMatrix();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// デバイスにワールド変換行列を設定
	pDevice->SetTransform( D3DTS_WORLD , &m_WorldMatrix );

	// ストリームの設定
	pDevice->SetStreamSource( 0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_3D );

	// テクスチャ貼り付け
	pDevice->SetTexture(0, m_pTexture);

	// 描画
	pDevice->DrawPrimitive(	D3DPT_TRIANGLESTRIP, 0, 2);

}



//=============================================================================
//	増減処理
// 座標の移動
void Object3D_Quaternion::MovePosition(D3DXVECTOR3& movePosition)
{
	// 座標の移動
	Object::MovePosition(movePosition);

	// ワールドマトリクスの更新フラグON
	m_bUpdateWorldMatrix = true;
}

// 大きさの増減
void Object3D_Quaternion::MoveSize(D3DXVECTOR3& moveSize)
{
	// 大きさの増減
	m_halfSize += moveSize * 0.5f;
	m_radius    = sqrtf(m_halfSize.x * m_halfSize.x + m_halfSize.y * m_halfSize.y);	// 半径
	
	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// 回転率の増減
void Object3D_Quaternion::MoveRotate(D3DXVECTOR3& vec, float rotate)
{
	// 回転率の増減
	D3DXQUATERNION quaternion;
	D3DXQuaternionRotationAxis(&quaternion, &vec, rotate);
	D3DXQuaternionMultiply(&m_quaternion, &quaternion, &m_quaternion);

	// ワールドマトリクスの更新フラグON
	m_bUpdateWorldMatrix = true;
}

// 拡縮率の増減
void Object3D_Quaternion::MoveScale(D3DXVECTOR3& moveScale)
{
	// 拡縮率の増減
	m_scale.x = m_scale.x + moveScale.x < 0 ? 0 : m_scale.x + moveScale.x;
	m_scale.y = m_scale.y + moveScale.y < 0 ? 0 : m_scale.y + moveScale.y;
	m_scale.z = m_scale.z + moveScale.z < 0 ? 0 : m_scale.z + moveScale.z;

	// ワールドマトリクスの更新フラグON
	m_bUpdateWorldMatrix = true;
}

// 法線の増減
void Object3D_Quaternion::MoveNormal(D3DXVECTOR3& moveNormal0, D3DXVECTOR3& moveNormal1, D3DXVECTOR3& moveNormal2, D3DXVECTOR3& moveNormal3)
{
	// 法線の増減
	m_normal[0] += moveNormal0;
	m_normal[1] += moveNormal1;
	m_normal[2] += moveNormal2;
	m_normal[3] += moveNormal3;
	
	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;
}

// 色の増減
void Object3D_Quaternion::MoveColor(int r, int g, int b, int a)
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
void Object3D_Quaternion::MoveUV_StartPoint(D3DXVECTOR2& moveUV_StartPoint)
{
	// UV開始点の増減
	m_UV_StartPoint += moveUV_StartPoint;

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// UV大きさの増減
void Object3D_Quaternion::MoveUV_Size(D3DXVECTOR2& moveUV_Size)
{
	// UV開始点の増減
	m_UV_Size += moveUV_Size;

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}



//=============================================================================
// 設定処理
// 座標の設定
void Object3D_Quaternion::SetPosition(D3DXVECTOR3& position)
{
	// 座標の移動
	Object::SetPosition(position);

	// ワールドマトリクスの更新フラグON
	m_bUpdateWorldMatrix = true;
}

// 大きさの設定
void Object3D_Quaternion::SetSize(D3DXVECTOR3& size)
{
	// 大きさの設定
	m_halfSize = size * 0.5f;
	m_radius   = sqrtf(m_halfSize.x * m_halfSize.x + m_halfSize.y * m_halfSize.y);	// 半径

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// 回転率の設定
void Object3D_Quaternion::SetRotate(D3DXVECTOR3& vec, float rotate)
{
	// 回転率の設定
	D3DXQuaternionIdentity(&m_quaternion);
	D3DXQuaternionRotationAxis(&m_quaternion, &vec, rotate);

	// ワールドマトリクスの更新フラグON
	m_bUpdateWorldMatrix = true;
}

// 拡縮率の設定
void Object3D_Quaternion::SetScale(D3DXVECTOR3& scale)
{
	// 拡縮率の設定
	m_scale.x = scale.x < 0 ? 0 : scale.x;
	m_scale.y = scale.y < 0 ? 0 : scale.y;
	m_scale.z = scale.z < 0 ? 0 : scale.z;


	// ワールドマトリクスの更新フラグON
	m_bUpdateWorldMatrix = true;
}

// 法線の設定
void Object3D_Quaternion::SetNormal(D3DXVECTOR3& moveNormal0, D3DXVECTOR3& moveNormal1, D3DXVECTOR3& moveNormal2, D3DXVECTOR3& moveNormal3)
{
	// 法線の増減
	m_normal[0] = moveNormal0;
	m_normal[1] = moveNormal1;
	m_normal[2] = moveNormal2;
	m_normal[3] = moveNormal3;

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;
}

// 色の設定
void Object3D_Quaternion::SetColor(unsigned int rgba)
{
	// 色設定
	m_color.color = rgba;

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// UV開始点の設定
void Object3D_Quaternion::SetUV_StartPoint(D3DXVECTOR2& UV_StartPoint)
{
	// UV開始点の設定
	m_UV_StartPoint = UV_StartPoint;

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// UVの大きさの設定
void Object3D_Quaternion::SetUV_Size(D3DXVECTOR2& UV_Size)
{
	// UVの大きさの設定
	m_UV_Size = UV_Size;

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}


//=============================================================================
//	private関数
// 頂点バッファの生成
void Object3D_Quaternion::MakeVertexBuf()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// 頂点バッファを生成
	if( m_pVtxBuff == nullptr ){
		// 頂点生成に失敗した？
		if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D ) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, nullptr ) )){
			// エラーメッセージの表示
			_MSGERROR("Failed Create Vertex Buffer!!", "void Object3D_Quaternion::MakeVertexBuf()");

			// このオブジェクトを破棄する
			this->Release();

			return;
		}
	}

	// VRAMの仮想アドレス取得
	VERTEX_3D* pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_halfSize.x,  m_halfSize.y,  m_halfSize.z);	// 左上
	pVtx[1].pos = D3DXVECTOR3( m_halfSize.x,  m_halfSize.y,  m_halfSize.z);	// 右上
	pVtx[2].pos = D3DXVECTOR3(-m_halfSize.x, -m_halfSize.y, -m_halfSize.z);	// 左下
	pVtx[3].pos = D3DXVECTOR3( m_halfSize.x, -m_halfSize.y, -m_halfSize.z);	// 右下

	// 法線の設定
	pVtx[0].normal = m_normal[0];
	pVtx[1].normal = m_normal[1];
	pVtx[2].normal = m_normal[2];
	pVtx[3].normal = m_normal[3];

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
void Object3D_Quaternion::UpdateVertexBuf()
{
	// VRAMの仮想アドレス取得
	VERTEX_3D* pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_halfSize.x,  m_halfSize.y,  m_halfSize.z);	// 左上
	pVtx[1].pos = D3DXVECTOR3( m_halfSize.x,  m_halfSize.y,  m_halfSize.z);	// 右上
	pVtx[2].pos = D3DXVECTOR3(-m_halfSize.x, -m_halfSize.y, -m_halfSize.z);	// 左下
	pVtx[3].pos = D3DXVECTOR3( m_halfSize.x, -m_halfSize.y, -m_halfSize.z);	// 右下

	// 法線の設定
	pVtx[0].normal = m_normal[0];
	pVtx[1].normal = m_normal[1];
	pVtx[2].normal = m_normal[2];
	pVtx[3].normal = m_normal[3];

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

// ワールドマトリクスの更新
void Object3D_Quaternion::UpdateWorldMatrix()
{
	// 移動、回転、拡縮行列の計算
	D3DXMATRIX mtxTranslate, mtxRotate, mtxScale;
	D3DXVECTOR3 position = Object::GetPosition();
	D3DXMatrixTranslation(&mtxTranslate, position.x, position.y, position.z);
	D3DXMatrixRotationQuaternion(&mtxRotate, &m_quaternion);
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);

	// ３行列の合成
	D3DXMatrixIdentity(&m_WorldMatrix);
	D3DXMatrixMultiply(&m_WorldMatrix, &m_WorldMatrix, &mtxScale);
	D3DXMatrixMultiply(&m_WorldMatrix, &m_WorldMatrix, &mtxRotate);
	D3DXMatrixMultiply(&m_WorldMatrix, &m_WorldMatrix, &mtxTranslate);

	// ワールドマトリクスの更新フラグOFF
	m_bUpdateWorldMatrix = false;

}
