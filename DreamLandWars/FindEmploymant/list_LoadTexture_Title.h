//*****************************************************************************
//
//		タイトル : テクスチャ読み込み表
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _LIST_LOADTEXTURE_TITLE_H_
#define _LIST_LOADTEXTURE_TITLE_H_

#include "main.h"
#include "texture.h"


class List_LoadTexture_Title
{
public:
	// 定数
	enum TEXTURE_NAME {
		TEXT_TEXNAME,				// 大文字
		TEXTBORDER_TEXNAME,			// 大文字枠
		TEXTSMALL_TEXNAME,			// 小文字
		TEXTSMALLBORDER_TEXNAME,	// 小文字枠
		TUTORIAL01,
		TUTORIAL02,

		__LOADTEXTURE_MAX,	// ※ 読込最大数
	};

	// 関数
	static void LoadTextureAll(Texture** ppTexture) {
		// 読込むテクスチャパス列挙
		List texPassList[List_LoadTexture_Title::__LOADTEXTURE_MAX] = {
			"data/texture/text_alphabet02.png", // 文字
			"data/texture/text_alphabet04.png", // 文字枠
			"data/texture/text_alphabet_S02.png", // 文字
			"data/texture/text_alphabet_S03.png", // 文字枠
			"data/texture/tutrorial01.png", // 文字枠
			"data/texture/tutrorial02.png", // 文字枠
		};


		// 読込
		for (int i = 0; i < List_LoadTexture_Title::__LOADTEXTURE_MAX; i++) {
			ppTexture[i] = Texture::Create(texPassList[i].texPass);

		}

	}



private:
	// 読込用の構造体定義
	struct List {
		char texPass[256];
	};

};

#endif


