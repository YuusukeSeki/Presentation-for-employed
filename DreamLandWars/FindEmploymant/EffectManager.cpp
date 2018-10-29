//*****************************************************************************
//	
//		エフェクト基底クラス（リスト構造）
//													Author : Yusuke Seki
//*****************************************************************************
#include "EffectManager.h"
#include "renderer.h"


//----- 静的メンバ変数初期化 -----
EffectManager* EffectManager::m_pHEAD[EffectManager::TYPE::TYPE_MAX] = {};
EffectManager* EffectManager::m_pTAIL[EffectManager::TYPE::TYPE_MAX] = {};



//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
EffectManager::EffectManager(EffectManager::TYPE type)
{
	// 初めて生成された種類なら、先頭にポインタを代入。
	if (m_pHEAD[type] == nullptr) {
		// ヘッドの更新
		m_pHEAD[type] = this;
		m_pHEAD[type]->m_pNext = nullptr;
		m_pHEAD[type]->m_pPrev = nullptr;

		// テイルの更新
		m_pTAIL[type] = m_pHEAD[type];

		// 前後のポインタの初期化
		this->m_pNext = nullptr;
		this->m_pPrev = nullptr;

	}
	// ２個目以上に生成された種類なら、末尾に追加。
	else {
		// テイルの次のポインタを"これ"にする
		m_pTAIL[type]->m_pNext = this;

		// 前後のポインタの初期化
		this->m_pNext = nullptr;
		this->m_pPrev = m_pTAIL[type];

		// テイルの更新
		m_pTAIL[type] = this;

	}


	// メンバ変数の初期化
	this->m_position = D3DXVECTOR3(0, 0, 0);
	this->m_type = type;

}


//-----------------------------------------------------------------------------
//	解放処理：個別
//-----------------------------------------------------------------------------
void EffectManager::Release()
{
	// 前後のオブジェクトが持っている、前後のポインタを更新
	if (this->m_pNext != nullptr) m_pNext->m_pPrev = this->m_pPrev;
	if (this->m_pPrev != nullptr) m_pPrev->m_pNext = this->m_pNext;

	// ヘッドのポインタを更新
	if (m_pHEAD[m_type] == this) m_pHEAD[m_type] = this->m_pNext;

	// テイルのポインタを更新
	if (m_pTAIL[m_type] == this) m_pTAIL[m_type] = this->m_pPrev;

	// 終了処理
	this->Uninit();

	// メモリの解放
	delete this;

}


//-----------------------------------------------------------------------------
//	更新処理：全体
//-----------------------------------------------------------------------------
void EffectManager::UpdateAll()
{
	int cnt[EffectManager::TYPE::TYPE_MAX] = {};


	for (int i = 0; i < EffectManager::TYPE::TYPE_MAX; i++) {
		EffectManager* pCurrent = m_pHEAD[i];

		for (;;) {
			if (pCurrent == nullptr) break;
			else {
				pCurrent->Update();
				pCurrent = pCurrent->m_pNext;

				cnt[i]++;
			}

		}
	}

	OX::DebugFont::print(0, SCREEN_HEIGHT - 80, 0xffff8080, "___Create Effect___");
	OX::DebugFont::print(0, SCREEN_HEIGHT - 60, 0xffff0000, "TYPE_3D                  : %d", cnt[TYPE_3D]);
	OX::DebugFont::print(0, SCREEN_HEIGHT - 40, 0xffff0000, "TYPE_3D_BILLBOARD        : %d", cnt[TYPE_3D_BILLBOARD]);
	OX::DebugFont::print(0, SCREEN_HEIGHT - 20, 0xffff0000, "TYPE_2D                  : %d", cnt[TYPE_2D]);

}


//-----------------------------------------------------------------------------
//	描画処理：全体
//-----------------------------------------------------------------------------
void EffectManager::DrawAll()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	//// αテストの設定
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// αテストＯＮ
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 128);				// 判定値
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 演算子
	
	// カリングを切る
	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ブレンド方法変更
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Zバッファを書き込まない
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// 描画処理
	for (int i = 0; i < EffectManager::TYPE::TYPE_MAX; i++) {
		EffectManager* pCurrent = m_pHEAD[i];

		for (;;) {
			if (pCurrent == nullptr) break;
			else {
				pCurrent->Draw();
				pCurrent = pCurrent->m_pNext;
			}

		}
	}

	// レンダーステートを元に戻す
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				// Zバッファを書き込む
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ブレンド方法を戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);					// ライトを有効に戻す
	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			// カリングモードを反時計回りにします

	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			// αテスト終了

}


//-----------------------------------------------------------------------------
//	解放処理：全体
//-----------------------------------------------------------------------------
void EffectManager::ReleaseAll()
{
	for (int i = 0; i < EffectManager::TYPE::TYPE_MAX; i++) {
		if (m_pHEAD[i] == nullptr) continue;

		EffectManager* pCurrent = m_pHEAD[i];
		EffectManager* pNext = m_pHEAD[i]->m_pNext;

		for (;;) {
			if (pCurrent == nullptr) break;
			else {
				pNext = pCurrent->m_pNext;
				pCurrent->Release();
				pCurrent = pNext;
			}

		}
	}

}


