//*****************************************************************************
//	
//		テクスチャマネージャー
//												Author : Yusuke Seki
//*****************************************************************************
#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

#include "main.h"


class TextureManager
{
public:
	// コンストラクタ / デストラクタ
	TextureManager();
	~TextureManager() {}

	// 基本的な関数
	void Release(void);
	virtual void LoadTexture(const char* texPass) = 0;
	virtual void UnLoad() = 0;

	// ALL
	static void UnLoadAll();
	static void ReleaseAll();

	// 取得
	static TextureManager* GetTexture(int arrayIdx);


protected:


private:
	// 定数
	static const int NUM_TEXTURE = 0xff;

	// 変数
	static TextureManager* m_pTextureArray[NUM_TEXTURE];

};

#endif

