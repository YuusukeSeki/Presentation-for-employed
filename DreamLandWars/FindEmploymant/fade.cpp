//*****************************************************************************
//
// フェード
// Author : Yusuke Seki
//
//*****************************************************************************
#include "fade.h"
#include "renderer.h"


//=============================================================================
//	
//=============================================================================
Fade::Fade()
{
	m_numFrame    = 60;         // フェードイン / アウトにかかるまでのフレーム数
	m_FadeState   = FADE_NONE;	// フェード状態
	m_color.color = 0xffffff00;	// フェード色
	m_pVtxBuff    = nullptr;	// 頂点バッファ
}


//=============================================================================
//	
//=============================================================================
Fade::~Fade()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}


//=============================================================================
//	
//=============================================================================
Fade* Fade::Create()
{
	// フェードの実体を生成
	Fade* pFade = new Fade;

	// 実体の初期化
	pFade->Init();

	// 生成された実体のポインタを返す
	return pFade;
}


//=============================================================================
//	
//=============================================================================
void Fade::Release()
{
	// 実体の終了処理
	Uninit();

	// 実体の破棄
	delete this;
}


//=============================================================================
//	
//=============================================================================
void Fade::Init()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// 頂点バッファの生成
	if (m_pVtxBuff == nullptr) {
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
			D3DPOOL_MANAGED, &m_pVtxBuff, NULL))) {
			MessageBox(NULL, "頂点バッファ生成失敗", "Scene2D::Init( D3DXVECTOR3 pos , D3DXVECTOR3 size )", MB_OK);
		}
	}

	// 2Dポリゴンの位置とサイズの設定
	D3DXVECTOR3 pos ((SCREEN_WIDTH >> 1), (SCREEN_HEIGHT >> 1), 0); // 画面の中心
	D3DXVECTOR3 size((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0); // クライアント領域全域

	// VRAMの仮想アドレス取得
	VERTEX_2D* pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pos.x - size.x * 0.5f, pos.y - size.y * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + size.x * 0.5f, pos.y - size.y * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - size.x * 0.5f, pos.y + size.y * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + size.x * 0.5f, pos.y + size.y * 0.5f, 0.0f);

	// 
	pVtx[0].rhw =
	pVtx[1].rhw =
	pVtx[2].rhw =
	pVtx[3].rhw = 1.0f;

	// 頂点色の設定
	pVtx[0].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
	pVtx[1].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
	pVtx[2].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
	pVtx[3].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);

	// テクスチャUV値の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// VRAMの仮想アドレス解放
	m_pVtxBuff->Unlock();

}


//=============================================================================
//	
//=============================================================================
void Fade::Uninit()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

}


//=============================================================================
//	
//=============================================================================
void Fade::Update()
{
	// 状態が"フェード処理無し"？
	if (m_FadeState == FADE_NONE) { // 処理無し
		// 更新処理を終了する
		return;
	}


	// α値の一時格納領域
	short alpha = 0;

	// 状態遷移
	switch (m_FadeState)
	{
	// フェードアウト
	case FADE_OUT:
		// 既に透過率０なら状態をフェードアウト終了に変更
		if (m_color.rgba[0] == 255) {
			// 状態を"フェードアウト終了"に変更
			m_FadeState = FADE_OUT_FINISH;

			break;
		}

		// α値の増加
		alpha = m_color.rgba[0] + (255 / m_numFrame);

		// α値が255以上になった？
		if (alpha >= 255) { // なった
			// α値を255に修正
			// 透過率０を描画状態に反映してからフェードアウト終了に切り替える
			m_color.rgba[0] = 255;

		}
		else { // なってない
			// α値を更新
			m_color.rgba[0] = (unsigned char)alpha;
		}

		// 頂点バッファに反映
		UpdateVtxBuf();

		// フェードアウト更新処理終了
		break;


	// フェードイン
	case FADE_OUT_FINISH:
		// 状態を"フェードイン"に変更
		m_FadeState = FADE_IN;

		// このままフェードイン処理へ


	// フェードイン
	case FADE_IN:
		// α値の減少
		alpha = m_color.rgba[0] - (255 / m_numFrame);

		// α値が0以下になった？
		if (alpha <= 0) { // なった
			// α値を0に修正
			m_color.rgba[0] = 0;

			// 状態を"フェード処理無し"に変更
			m_FadeState = FADE_NONE;

		}
		else { // なってない
			// α値を更新
			m_color.rgba[0] = (unsigned char)alpha;
		}

		// 頂点バッファに反映
		UpdateVtxBuf();

		// フェードイン更新処理終了
		break;


	}


}


//=============================================================================
//	
//=============================================================================
void Fade::Draw()
{
	// 状態が"フェード処理無し"？
	if (m_FadeState == FADE_NONE) { // 処理無し
		// 描画処理を終了する
		return;
	}


	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource( 0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_2D );

	// テクスチャの設定
	pDevice->SetTexture( 0 , nullptr );

	// ポリゴンの描画
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP , 0 , 2 );				
}


//=============================================================================
//	色設定
//=============================================================================
void Fade::SetColor(unsigned char r, unsigned char g, unsigned char b)
{
	// 色の更新
	m_color.rgba[3] = r;
	m_color.rgba[2] = g;
	m_color.rgba[1] = b;

	// 頂点バッファに設定
	UpdateVtxBuf();

}


//=============================================================================
//	フェードアウト開始
//=============================================================================
void Fade::Start_FadeOut(unsigned int numFrame)
{
	// 現在フェードイン / アウト状態？
	if (m_FadeState != FADE_NONE) { // フェードイン / アウト状態
		// フェード設定を行わずに処理を終了する
		return;
	}

	// フレーム数が0？
	if (numFrame == 0) numFrame = 1; // 値がバグるので1に変更

	// フェードイン / アウトにかかるフレーム数の設定
	m_numFrame = numFrame;

	// 状態を"フェードアウト"に変更
	m_FadeState = FADE_OUT;


}


//=============================================================================
//	フェードアウト終わった？
//=============================================================================
bool Fade::Finish_FadeOut()
{
	// フェードアウト終わった？
	if(m_FadeState == FADE_OUT_FINISH){ // はい
		// 終わってるよ
		return true;
	}

	// 終わってないよ
	return false;
}


//=============================================================================
//	頂点バッファの更新
//=============================================================================
void Fade::UpdateVtxBuf()
{
	// VRAMの仮想アドレス取得
	VERTEX_2D* pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点色の設定
	pVtx[0].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
	pVtx[1].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
	pVtx[2].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
	pVtx[3].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);

	// テクスチャUV値の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// VRAMの仮想アドレス解放
	m_pVtxBuff->Unlock();


}
