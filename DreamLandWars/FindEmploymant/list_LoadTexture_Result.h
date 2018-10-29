//*****************************************************************************
//
//		Result : テクスチャ読み込み表
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _LIST_LOADTEXTURE_RESULT_H_
#define _LIST_LOADTEXTURE_RESULT_H_

#include "main.h"
#include "texture.h"


class List_LoadTexture_Result
{
public:
	// 定数
	enum TEXTURE_NAME {
		BACKGROUND_TEXNAME, // 背景

		__LOADTEXTURE_MAX,  // ※ 読込最大数
	};

	// 関数
	static void LoadTextureAll(Texture** ppTexture) {
		// 読込むテクスチャパス列挙
		List texPassList[List_LoadTexture_Result::__LOADTEXTURE_MAX] = {
			"data/TEXTURE/Result/resultBG01.jpg", // 背景

		};


		// 読込
		for (int i = 0; i < List_LoadTexture_Result::__LOADTEXTURE_MAX; i++) {
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


