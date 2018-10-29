//*****************************************************************************
//
//		エフェクト（3D）
//												Autohr : Yusuke Seki
//*****************************************************************************
#include "Effect3D.h"
#include "renderer.h"
#include "texture.h"
#include "MainGame.h"


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
Effect3D::Effect3D(EffectManager::TYPE type) :EffectManager(type)
{
	// メンバ変数初期化
	m_halfSize           = D3DXVECTOR3(0, 0, 0);
	m_rotate             = D3DXVECTOR3(0, 0, 0);
	m_scale              = D3DXVECTOR3(0, 0, 0);
	m_radius             = 0;
	m_accelerate         = D3DXVECTOR3(0, 0, 0);
	m_normal[0]          = D3DXVECTOR3(0, 0, 0);
	m_normal[1]          = D3DXVECTOR3(0, 0, 0);
	m_normal[2]          = D3DXVECTOR3(0, 0, 0);
	m_normal[3]          = D3DXVECTOR3(0, 0, 0);
	m_color_R            = 0.f;
	m_color_G            = 0.f;
	m_color_B            = 0.f;
	m_color_A            = 0.f;
	m_UV_StartPoint      = D3DXVECTOR2(0, 0);
	m_UV_Size            = D3DXVECTOR2(0, 0);
	m_pVtxBuff           = nullptr;
	m_pTexture           = nullptr;
	m_bUpdateVertexBuf   = false;
	m_bUpdateWorldMatrix = false;
	m_bInstance          = true;

	m_ChangeRotateByFrame       = D3DXVECTOR3(0, 0, 0);
	m_ChangeScaleByFrame        = D3DXVECTOR3(0, 0, 0);
	m_ChangeAccelerateByFrame   = D3DXVECTOR3(0, 0, 0);
	m_ChangeRedByFrame          = 0.f;
	m_ChangeGreenByFrame        = 0.f;
	m_ChangeBlueByFrame         = 0.f;
	m_ChangeAlphaByFrame        = 0.f;
	m_ChangeUVStartPointByFrame = D3DXVECTOR2(0, 0);
	m_ChangeUVSizeByFrame       = D3DXVECTOR2(0, 0);
	m_ChangeRedByFrame          = 0;
	m_ChangeGreenByFrame        = 0;
	m_ChangeBlueByFrame         = 0;
	m_ChangeAlphaByFrame        = 0;
	m_life                      = 0;

}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
Effect3D::~Effect3D()
{
	// 終了処理
	Uninit();

}


//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
Effect3D* Effect3D::Create(D3DXVECTOR3& pos, D3DXVECTOR3& size)
{
	// 生成
	Effect3D* pEffect3D = new Effect3D(EffectManager::TYPE::TYPE_3D);

	// 初期化
	pEffect3D->Init(pos, size);

	return pEffect3D;
}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void Effect3D::Init(D3DXVECTOR3& position, D3DXVECTOR3& size)
{
	//----- データの設定 -----
	EffectManager::SetPosition(position);														// 座標
	m_halfSize           = size * 0.5f;															// 半分の大きさ
	m_rotate             = D3DXVECTOR3(0, 0, 0);												// 回転率
	m_scale              = D3DXVECTOR3(1, 1, 1);												// 拡縮率
	m_radius             = sqrtf(m_halfSize.x * m_halfSize.x + m_halfSize.y * m_halfSize.y);	// 半径
	m_accelerate         = D3DXVECTOR3(0, 0, 0);												// 加速度
	m_normal[0]          = D3DXVECTOR3(0, 0, 1);												// 法線
	m_normal[1]          = D3DXVECTOR3(0, 0, 1);												// 法線
	m_normal[2]          = D3DXVECTOR3(0, 0, 1);												// 法線
	m_normal[3]          = D3DXVECTOR3(0, 0, 1);												// 法線
	m_color_R            = 0.f;																	// 頂点色 - 赤
	m_color_G            = 0.f;																	// 頂点色 - 緑
	m_color_B            = 0.f;																	// 頂点色 - 青
	m_color_A            = 0.f;																	// 頂点色 - 透過
	m_UV_StartPoint      = D3DXVECTOR2(0, 0);													// UV開始点
	m_UV_Size            = D3DXVECTOR2(1, 1);													// UV大きさ
	m_bUpdateVertexBuf   = false;																// true で頂点バッファの更新する
	m_bUpdateWorldMatrix = false;																// true でワールドマトリクスの更新する
	m_bInstance          = true;																// true で使用中

	m_ChangeRotateByFrame       = D3DXVECTOR3(0, 0, 0);	// フレーム間の回転率変化量
	m_ChangeScaleByFrame        = D3DXVECTOR3(0, 0, 0);	// フレーム間の拡縮率変化量
	m_ChangeAccelerateByFrame   = D3DXVECTOR3(0, 0, 0);	// フレーム間の加速度変化量
	m_ChangeRedByFrame          = 0;					// フレーム間の頂点色・赤、変化量
	m_ChangeGreenByFrame        = 0;					// フレーム間の頂点色・緑、変化量
	m_ChangeBlueByFrame         = 0;					// フレーム間の頂点色・青、変化量
	m_ChangeAlphaByFrame        = 0;					// フレーム間の頂点色・透過値、変化量
	m_ChangeUVStartPointByFrame = D3DXVECTOR2(0, 0);	// フレーム間のUV開始点変化量
	m_ChangeUVSizeByFrame       = D3DXVECTOR2(0, 0);	// フレーム間のUVサイズ変化量
	m_life                      = 0;					// 寿命

	// 頂点バッファの生成
	Effect3D::MakeVertexBuf();

	// 行列を算出
	this->UpdateWorldMatrix();

}


//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void Effect3D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

}


//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void Effect3D::Update(void)
{
	// 未使用で処理無し
	if (!m_bInstance) return;

	// 寿命が尽きたら、使用フラグOFF
	if (m_life <= 0) {
		m_bInstance = false;
		return;
	}

	// 寿命の減衰
	m_life--;

	// 位置の更新
	Effect3D::MovePosition(m_accelerate);

	// 回転率の更新
	Effect3D::MoveRotate(m_ChangeRotateByFrame);

	// 拡縮率の更新
	Effect3D::MoveScale(m_ChangeScaleByFrame);

	// 加速度の更新
	Effect3D::MoveAccelerate(m_ChangeAccelerateByFrame);

	// 頂点色の更新
	Effect3D::MoveColor(m_ChangeRedByFrame, m_ChangeGreenByFrame, m_ChangeBlueByFrame, m_ChangeAlphaByFrame);
	
	// UV開始点の更新
	Effect3D::MoveUV_StartPoint(m_ChangeUVStartPointByFrame);

	// UVサイズの更新
	Effect3D::MoveUV_Size(m_ChangeUVSizeByFrame);

}


//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void Effect3D::Draw(void)
{
	// 未使用なら描画処理を行わない
	if (!m_bInstance) return;

	// true で頂点バッファの更新処理をする
	if (m_bUpdateVertexBuf) Effect3D::UpdateVertexBuf();

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
void Effect3D::MovePosition(D3DXVECTOR3& movePosition)
{
	// 座標の移動
	EffectManager::MovePosition(movePosition);

	// ワールドマトリクスの更新フラグON
	m_bUpdateWorldMatrix = true;
}

// 大きさの増減
void Effect3D::MoveSize(D3DXVECTOR3& moveSize)
{
	// 大きさの増減
	m_halfSize += moveSize * 0.5f;
	m_radius    = sqrtf(m_halfSize.x * m_halfSize.x + m_halfSize.y * m_halfSize.y);	// 半径
	
	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// 回転率の増減
void Effect3D::MoveRotate(D3DXVECTOR3& moveRotate)
{
	// 回転率の増減
	m_rotate += moveRotate;
	
	// ワールドマトリクスの更新フラグON
	m_bUpdateWorldMatrix = true;
}

// 拡縮率の増減
void Effect3D::MoveScale(D3DXVECTOR3& moveScale)
{
	// 拡縮率の増減
	m_scale.x = m_scale.x + moveScale.x < 0 ? 0 : m_scale.x + moveScale.x;
	m_scale.y = m_scale.y + moveScale.y < 0 ? 0 : m_scale.y + moveScale.y;
	m_scale.z = m_scale.z + moveScale.z < 0 ? 0 : m_scale.z + moveScale.z;

	// ワールドマトリクスの更新フラグON
	m_bUpdateWorldMatrix = true;
}

// 法線の増減
void Effect3D::MoveNormal(D3DXVECTOR3& moveNormal0, D3DXVECTOR3& moveNormal1, D3DXVECTOR3& moveNormal2, D3DXVECTOR3& moveNormal3)
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
void Effect3D::MoveColor(float r, float g, float b, float a)
{
	// 色の増減
	m_color_R = m_color_R + r >= 255 ? 255 : m_color_R + r;
	m_color_G = m_color_G + g >= 255 ? 255 : m_color_G + g;
	m_color_B = m_color_B + b >= 255 ? 255 : m_color_B + b;
	m_color_A = m_color_A + a >= 255 ? 255 : m_color_A + a;
	m_color_R = m_color_R + r <=   0 ?   0 : m_color_R + r;
	m_color_G = m_color_G + g <=   0 ?   0 : m_color_G + g;
	m_color_B = m_color_B + b <=   0 ?   0 : m_color_B + b;
	m_color_A = m_color_A + a <=   0 ?   0 : m_color_A + a;

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// UV開始点の増減
void Effect3D::MoveUV_StartPoint(D3DXVECTOR2& moveUV_StartPoint)
{
	// UV開始点の増減
	m_UV_StartPoint += moveUV_StartPoint;

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// UV大きさの増減
void Effect3D::MoveUV_Size(D3DXVECTOR2& moveUV_Size)
{
	// UV開始点の増減
	m_UV_Size += moveUV_Size;

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}



//=============================================================================
// 設定処理
// エフェクトの発生
void Effect3D::SetEffect(D3DXVECTOR3& position, D3DXVECTOR3& vecZ, Effect3D::TYPE effectType)
{
	// 実体持ちを探す
	Effect3D* pEffect3D = (Effect3D*)EffectManager::GetLDATA_HEAD(EffectManager::TYPE::TYPE_3D);

	// １つも作られていなかったら生成する
	if (pEffect3D == nullptr) {
		// 生成処理
		pEffect3D = Effect3D::Create(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));

		// 設定処理
		pEffect3D->SetEffect_Protected(position, vecZ, effectType);

		return;
	}

	// 作られていたら未使用のエフェクトを探す
	Effect3D* pCurrent = (Effect3D*)pEffect3D;
	Effect3D* pNext    = (Effect3D*)pEffect3D->GetNextPointer();
	for (;;) {
		// 未使用なら設定して終了
		if (!pCurrent->m_bInstance) {
			pCurrent->SetEffect_Protected(position, vecZ, effectType);
			break;
		}

		// 未使用エフェクトが見つからなければ、新しく生成する
		if (pNext == nullptr) {
			// 生成処理
			pNext = Effect3D::Create(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));

			// 設定処理
			pNext->SetEffect_Protected(position, vecZ, effectType);

			break;
		}

		// ポインタをずらす
		pCurrent = pNext;
		pNext    = (Effect3D*)pCurrent->GetNextPointer();

	}
}

// 座標の設定
void Effect3D::SetPosition(D3DXVECTOR3& position)
{
	// 座標の移動
	EffectManager::SetPosition(position);

	// ワールドマトリクスの更新フラグON
	m_bUpdateWorldMatrix = true;
}

// 大きさの設定
void Effect3D::SetSize(D3DXVECTOR3& size)
{
	// 大きさの設定
	m_halfSize = size * 0.5f;
	m_radius   = sqrtf(m_halfSize.x * m_halfSize.x + m_halfSize.y * m_halfSize.y);	// 半径

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// 回転率の設定
void Effect3D::SetRotate(D3DXVECTOR3& rotate)
{
	// 回転率の増減
	m_rotate = rotate;

	// ワールドマトリクスの更新フラグON
	m_bUpdateWorldMatrix = true;
}

// 拡縮率の設定
void Effect3D::SetScale(D3DXVECTOR3& scale)
{
	// 拡縮率の設定
	m_scale.x = scale.x < 0 ? 0 : scale.x;
	m_scale.y = scale.y < 0 ? 0 : scale.y;
	m_scale.z = scale.z < 0 ? 0 : scale.z;


	// ワールドマトリクスの更新フラグON
	m_bUpdateWorldMatrix = true;
}

// 法線の設定
void Effect3D::SetNormal(D3DXVECTOR3& moveNormal0, D3DXVECTOR3& moveNormal1, D3DXVECTOR3& moveNormal2, D3DXVECTOR3& moveNormal3)
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
void Effect3D::SetColor(int r, int g, int b, int a)
{
	// 色設定
	m_color_R = (float)r;
	m_color_G = (float)g;
	m_color_B = (float)b;
	m_color_A = (float)a;

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// UV開始点の設定
void Effect3D::SetUV_StartPoint(D3DXVECTOR2& UV_StartPoint)
{
	// UV開始点の設定
	m_UV_StartPoint = UV_StartPoint;

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// UVの大きさの設定
void Effect3D::SetUV_Size(D3DXVECTOR2& UV_Size)
{
	// UVの大きさの設定
	m_UV_Size = UV_Size;

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}


//=============================================================================
//	private関数
// 頂点バッファの生成
void Effect3D::MakeVertexBuf()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// 頂点バッファを生成
	if( m_pVtxBuff == nullptr ){
		// 頂点生成に失敗した？
		if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D ) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, nullptr ) )){
			// エラーメッセージの表示
			_MSGERROR("Failed Create Vertex Buffer!!", "void Effect3D::MakeVertexBuf()");

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
	pVtx[0].color = D3DCOLOR_RGBA((int)m_color_R, (int)m_color_G, (int)m_color_B, (int)m_color_A);
	pVtx[1].color = D3DCOLOR_RGBA((int)m_color_R, (int)m_color_G, (int)m_color_B, (int)m_color_A);
	pVtx[2].color = D3DCOLOR_RGBA((int)m_color_R, (int)m_color_G, (int)m_color_B, (int)m_color_A);
	pVtx[3].color = D3DCOLOR_RGBA((int)m_color_R, (int)m_color_G, (int)m_color_B, (int)m_color_A);

	// テクスチャUV値の設定
	pVtx[0].tex = D3DXVECTOR2(m_UV_StartPoint.x              , m_UV_StartPoint.y              );
	pVtx[1].tex = D3DXVECTOR2(m_UV_StartPoint.x + m_UV_Size.x, m_UV_StartPoint.y              );
	pVtx[2].tex = D3DXVECTOR2(m_UV_StartPoint.x              , m_UV_StartPoint.y + m_UV_Size.y);
	pVtx[3].tex = D3DXVECTOR2(m_UV_StartPoint.x + m_UV_Size.x, m_UV_StartPoint.y + m_UV_Size.y);

	// VRAMの仮想アドレス解放
	m_pVtxBuff->Unlock();
}

// 頂点バッファの更新
void Effect3D::UpdateVertexBuf()
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
	pVtx[0].color = D3DCOLOR_RGBA((int)m_color_R, (int)m_color_G, (int)m_color_B, (int)m_color_A);
	pVtx[1].color = D3DCOLOR_RGBA((int)m_color_R, (int)m_color_G, (int)m_color_B, (int)m_color_A);
	pVtx[2].color = D3DCOLOR_RGBA((int)m_color_R, (int)m_color_G, (int)m_color_B, (int)m_color_A);
	pVtx[3].color = D3DCOLOR_RGBA((int)m_color_R, (int)m_color_G, (int)m_color_B, (int)m_color_A);

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
void Effect3D::UpdateWorldMatrix()
{
	// 移動、回転、拡縮行列の計算
	D3DXMATRIX mtxTranslate, mtxRotate, mtxScale;
	D3DXVECTOR3* pPosition = EffectManager::GetPosition();
	D3DXMatrixTranslation(&mtxTranslate, pPosition->x, pPosition->y, pPosition->z);
	D3DXMatrixRotationYawPitchRoll(&mtxRotate, m_rotate.y, m_rotate.x, m_rotate.z);
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);

	// ３行列の合成
	D3DXMatrixIdentity(&m_WorldMatrix);
	D3DXMatrixMultiply(&m_WorldMatrix, &m_WorldMatrix, &mtxScale);
	D3DXMatrixMultiply(&m_WorldMatrix, &m_WorldMatrix, &mtxRotate);
	D3DXMatrixMultiply(&m_WorldMatrix, &m_WorldMatrix, &mtxTranslate);

	// ワールドマトリクスの更新フラグOFF
	m_bUpdateWorldMatrix = false;

}

// エフェクトの設定
void Effect3D::SetEffect_Protected(D3DXVECTOR3& position, D3DXVECTOR3& vecZ, Effect3D::TYPE effectType)
{
	// 発生起点取得
	D3DXVECTOR3 pos = position;

	// エフェクトの種類で初期化内容を変更
	switch (effectType)
	{
	// 炎
	case Effect3D::TYPE::TYPE_FIRE:
		// 寿命の設定
		m_life = 60;
		//m_life = 240;

		// 出現位置を調整
		pos.x += rand() % 10 - 5;
		pos.y += rand() % 10 - 2;
		EffectManager::SetPosition(pos);

		// 半分の大きさの設定
		m_halfSize = D3DXVECTOR3(10, 10, 0);

		// 回転率の設定
		m_rotate = D3DXVECTOR3(0, 0, 0);

		// 拡縮率の設定
		m_scale = D3DXVECTOR3(1, 1, 1);

		// 加速度の設定
		vecZ *= -1;
		m_accelerate = D3DXVECTOR3(vecZ.x * 3, (float)(rand() % 10001 / 2000), vecZ.z * 3);

		// 頂点色の設定
		m_color_R = 255;	// 赤
		m_color_G = 128;	// 緑
		m_color_B =  64;	// 青
		m_color_A = 255;	// 透過

		// UV開始点の設定
		m_UV_StartPoint = D3DXVECTOR2(0, 0);

		// UVサイズの設定
		m_UV_Size = D3DXVECTOR2(1, 1);


		// 回転率変化量の設定
		m_ChangeRotateByFrame = D3DXVECTOR3(0, 0, 0.1f);

		// 拡縮率変化量の設定
		m_ChangeScaleByFrame = D3DXVECTOR3(-1.f / m_life, -1.f / m_life, -1.f / m_life);

		// 加速度変化量の設定
		m_ChangeAccelerateByFrame = D3DXVECTOR3(0, 0, 0);

		// 頂点色変化量の設定
		m_ChangeRedByFrame   = -m_color_R / m_life;
		m_ChangeGreenByFrame = -m_color_G / m_life;
		m_ChangeBlueByFrame  = -m_color_B / m_life;
		m_ChangeAlphaByFrame = 0;
		//m_ChangeAlphaByFrame = -m_color_A / m_life;

		// UV開始点変化量の設定
		m_ChangeUVStartPointByFrame = D3DXVECTOR2(0, 0);

		// UVサイズ変化量の設定
		m_ChangeUVSizeByFrame = D3DXVECTOR2(0, 0);


		// 頂点バッファの更新フラグON
		m_bUpdateVertexBuf = true;

		// ワールドマトリクスの更新フラグON
		m_bUpdateWorldMatrix = true;

		// 使用中フラグON
		m_bInstance = true;

		// テクスチャの設定
		SetTexture(MainGame::GetTexture(List_LoadTexture_MainGame::TEXTURE_NAME::EFFECT_TEXNAME)->GetTexture());

		break;


	// 例外
	default:
		// エラーメッセージの表示
		_MSGERROR("Cannot Create This Effect Type!!", "void ObjectEffect::SetEffect_Private(D3DXVECTOR3& position, ObjectEffect::TYPE effectType)");
		break;

	}
}

