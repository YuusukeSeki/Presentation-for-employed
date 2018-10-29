//*****************************************************************************
//	
//		オブジェクト基底クラス（リスト構造）
//													Author : Yusuke Seki
//*****************************************************************************
#include "Object.h"


//----- 静的メンバ変数初期化 -----
Object* Object::m_pHEAD[Object::TYPE::TYPE_MAX] = {};
Object* Object::m_pTAIL[Object::TYPE::TYPE_MAX] = {};



//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
Object::Object(Object::TYPE type)
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
		this->m_pNext    = nullptr;
		this->m_pPrev    = nullptr;

	}
	// ２個目以上に生成された種類なら、末尾に追加。
	else {
		// テイルの次のポインタを"これ"にする
		m_pTAIL[type]->m_pNext = this;

		// 前後のポインタの初期化
		this->m_pNext    = nullptr;
		this->m_pPrev    = m_pTAIL[type];

		// テイルの更新
		m_pTAIL[type] = this;

	}


	// メンバ変数の初期化
	this->m_position = D3DXVECTOR3(0, 0, 0);
	this->m_type = type;

#ifdef _WLW
	this->m_group = Object::GROUP::GROUP_NONE;
#endif

}


//-----------------------------------------------------------------------------
//	解放処理：個別
//-----------------------------------------------------------------------------
void Object::Release()
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
void Object::UpdateAll()
{
	int cnt[Object::TYPE::TYPE_MAX] = {};


	for (int i = 0; i < Object::TYPE::TYPE_MAX; i++) {
		Object* pCurrent = m_pHEAD[i];


		for (;;) {
			if (pCurrent == nullptr) break;
			else {
				pCurrent->Update();
				pCurrent = pCurrent->m_pNext;

				cnt[i]++;
			}
		}
	}

	OX::DebugFont::print(0, SCREEN_HEIGHT - 420, 0xffff8080, "___Create Object___");
	OX::DebugFont::print(0, SCREEN_HEIGHT - 380, 0xffff0000, "TYPE_COMMANDER           : %d", cnt[TYPE_COMMANDER]);
	OX::DebugFont::print(0, SCREEN_HEIGHT - 360, 0xffff0000, "TYPE_MODEL_SOLDIER       : %d", cnt[TYPE_MODEL_SOLDIER]);
	OX::DebugFont::print(0, SCREEN_HEIGHT - 300, 0xffff0000, "TYPE_MODEL_TOWER         : %d", cnt[TYPE_MODEL_TOWER]);
	OX::DebugFont::print(0, SCREEN_HEIGHT - 260, 0xffff0000, "TYPE_3D_BILLBOARD        : %d", cnt[TYPE_3D_BILLBOARD]);
	OX::DebugFont::print(0, SCREEN_HEIGHT - 240, 0xffff0000, "TYPE_3D_SOLDIERBULLET    : %d", cnt[TYPE_3D_SOLDIERBULLET]);
	OX::DebugFont::print(0, SCREEN_HEIGHT - 220, 0xffff0000, "TYPE_3D_BILLBOARD_BULLET : %d", cnt[TYPE_3D_BILLBOARD_BULLET]);
	OX::DebugFont::print(0, SCREEN_HEIGHT - 200, 0xffff0000, "TYPE_3D_BILLBOARD_EFFECT : %d", cnt[TYPE_3D_BILLBOARD_EFFECT]);
	OX::DebugFont::print(0, SCREEN_HEIGHT - 180, 0xffff0000, "TYPE_3D_STRAIGHTSHOT     : %d", cnt[TYPE_3D_STRAIGHTSHOT]);
	OX::DebugFont::print(0, SCREEN_HEIGHT - 160, 0xffff0000, "TYPE_3D_DRAWLINE         : %d", cnt[TYPE_3D_DRAWLINE]);
	OX::DebugFont::print(0, SCREEN_HEIGHT - 140, 0xffff0000, "TYPE_3D_DRAWSHOT         : %d", cnt[TYPE_3D_DRAWSHOT]);

}


//-----------------------------------------------------------------------------
//	描画処理：全体
//-----------------------------------------------------------------------------
void Object::DrawAll()
{
	for (int i = 0; i < Object::TYPE::TYPE_MAX; i++) {
		Object* pCurrent = m_pHEAD[i];

		for (;;) {
			if (pCurrent == nullptr) break;
			else {
				pCurrent->Draw();
				pCurrent = pCurrent->m_pNext;

			}

		}
	}

}


//-----------------------------------------------------------------------------
//	解放処理：全体
//-----------------------------------------------------------------------------
void Object::ReleaseAll()
{
	for (int i = 0; i < Object::TYPE::TYPE_MAX; i++) {
		if (m_pHEAD[i] == nullptr) continue;

		Object* pCurrent = m_pHEAD[i];
		Object* pNext    = m_pHEAD[i]->m_pNext;

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


