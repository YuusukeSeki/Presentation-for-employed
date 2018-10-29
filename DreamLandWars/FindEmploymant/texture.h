//*****************************************************************************
//	
//		テクスチャー
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"
#include "TextureManager.h"



class Texture : public TextureManager
{
public:
	// コンストラクタ / デストラクタ
	~Texture();

	// 基本的な関数
	static Texture* Create(const char* texPass);
	void Release();
	void LoadTexture(const char* texPass);
	void UnLoad();

	// 取得
	LPDIRECT3DTEXTURE9 GetTexture() { return m_pTexture; }


protected:
	Texture(const char* texPass);


private:
	// コンストラクタ / デストラクタ
	Texture(){}

	// 変数
	LPDIRECT3DTEXTURE9 m_pTexture;


};

#endif

