//*****************************************************************************
//
//		MainGame : テクスチャ読み込み表
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _LIST_LOADTEXTURE_MAINGAME_H_
#define _LIST_LOADTEXTURE_MAINGAME_H_

#include "main.h"
#include "texture.h"


class List_LoadTexture_MainGame
{
public:
	// 定数
	enum TEXTURE_NAME {
		FIELD_TEXNAME,			// 地面
		SKYBOX_TEXNAME,			// スカイボックス
		BULLET_TEXNAME,			// 弾
		NUMBER_TEXNAME,			// 数字
		COLON_TEXNAME,			// コロン
		DRAWLINE_TEXNAME,		// 描画線
		EFFECT_TEXNAME,			// エフェクト
		FRAMEBORDER_TEXNAME,	// 枠線
		LABEL_YOURTEAM_TEXNAME,	// "YOUR TEAM"
		LABEL_ENEMY_TEXNAME,	// "ENEMY"
		TEAMGAUGE_TEXNAME,		// チームゲージ
		LOCKON_INNERLINE,		// 照準：内側
		LOCKON_OUTLINE,			// 照準：外側
		LOCKON_HOLD,			// 照準："HOLD"
		ICON_TOWER,				// アイコン：塔
		ICON_CASTLE,			// アイコン：城
		WALL_TEXNAME,			// 壁
		TUTORIAL,
		SOLDIERBULLET,

		__LOADTEXTURE_MAX,  // ※ 読込最大数
	};


	// 関数
	static void LoadTextureAll(Texture** ppTexture) {
		// 読込むテクスチャパス列挙
		List texPassList[List_LoadTexture_MainGame::__LOADTEXTURE_MAX] = {
			//"data/texture/debugTexture01.jpg",			// 地面
			"data/texture/MainGame/ground03.jpg",			// 地面
			"data/texture/MainGame/skybox001.png",			// スカイボックス
			"data/texture/MainGame/kobeni1.png",			// 弾
			"data/texture/MainGame/numberFile03.png",		// 数字
			"data/texture/MainGame/numberFile04.png",		// コロン
			"data/texture/MainGame/drawLine01.png",			// 描画線
			"data/texture/MainGame/effect000.jpg",			// エフェクト
			"data/texture/MainGame/FrameBorder02.png",		// 枠線
			"data/texture/MainGame/Text_YourTeam01.png",	// "YOUR TEAM"
			"data/texture/MainGame/Text_Enemy01.png",		// "ENEMY"
			"data/texture/MainGame/TeamGauge01.png",		// チームゲージ
			"data/texture/MainGame/Lock-On02.png",			// 照準：内側
			"data/texture/MainGame/Lock-On01.png",			// 照準：外側
			"data/texture/MainGame/Hold01.png",				// 照準："HOLD"
			"data/texture/MainGame/fortress_icon01.png",	// アイコン：塔
			"data/texture/MainGame/castle_icon01.png",		// アイコン：城
			"data/texture/MainGame/wall01.jpg",				// 壁
			"data/texture/tutrorial03.png",					// 文字枠
			"data/texture/MainGame/soldierBullet.png",		// 兵士弾

		};


		// 読込
		for (int i = 0; i < List_LoadTexture_MainGame::__LOADTEXTURE_MAX; i++) {
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


