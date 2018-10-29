//*****************************************************************************
//	
//		テクスチャマネージャー
//												Author : Yusuke Seki
//*****************************************************************************
#include "TextureManager.h"

//----- 静的メンバ変数初期化 -----
TextureManager* TextureManager::m_pTextureArray[TextureManager::NUM_TEXTURE] = {};



//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
TextureManager::TextureManager()
{
	for (int i = 0; i < NUM_TEXTURE; i++) {
		if (m_pTextureArray[i] == nullptr) {
			m_pTextureArray[i] = this;

			return;
		}
	}

	_MSGERROR("テクスチャを格納する空き配列がありません", "");

}


//-----------------------------------------------------------------------------
//	解放処理：個別
//-----------------------------------------------------------------------------
void TextureManager::Release()
{
	for (int i = 0; i < NUM_TEXTURE; i++) {
		if (m_pTextureArray[i] == this) {
			UnLoad();
			delete this;
			m_pTextureArray[i] = nullptr;

			break;
		}
	}
}


//-----------------------------------------------------------------------------
//	終了処理
//-----------------------------------------------------------------------------
void TextureManager::UnLoadAll()
{
	for (int i = 0; i < NUM_TEXTURE; i++) {
		if (m_pTextureArray[i] != nullptr) {
			m_pTextureArray[i]->UnLoad();
		}
	}
}


//-----------------------------------------------------------------------------
//	解放処理
//-----------------------------------------------------------------------------
void TextureManager::ReleaseAll()
{
	for (int i = 0; i < NUM_TEXTURE; i++) {
		if (m_pTextureArray[i] != nullptr) {
			m_pTextureArray[i]->Release();
		}
	}
}


//-----------------------------------------------------------------------------
//	
//-----------------------------------------------------------------------------
TextureManager* TextureManager::GetTexture(int arrayIdx)
{
	// 実体が無ければ nullptr を返す
	if (m_pTextureArray[arrayIdx] == nullptr) return nullptr;

	// 一致したのでポインタを返す
	return m_pTextureArray[arrayIdx];

}


