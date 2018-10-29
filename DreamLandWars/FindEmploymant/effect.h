//*****************************************************************************
//	
//		エフェクト
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _EFFECT_H_
#define _EFFECT_H_

//----- インクルードファイル -----
#include "main.h"
#include "scene.h"


//----- 前方宣言 -----
class Texture;


//----- クラスの定義 -----
class Effect : public Scene
{
public:
	// コンストラクタ / デストラクタ
	virtual ~Effect();

	// 基本的な関数
	static	Effect* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXVECTOR3 scl); // 実体の生成
	virtual HRESULT	Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXVECTOR3 scl);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	// 設定
	bool SetEffect(D3DXVECTOR3 pos);
	void SetTexture(Texture* pTexture) { m_pTexture = pTexture; }

	// 取得


protected:
	// コンストラクタ / デストラクタ
	Effect() {}
	Effect(int priority, TYPE type);


private:
	// 定数
	static const int ELASE_TIME = 240;

	// 変数
	D3DXMATRIX m_mtxWorld;
	D3DXVECTOR3 m_size, m_rot, m_scl, m_move;
	Scene::Color m_color;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	Texture* m_pTexture;

	float m_radius, m_angle, m_colorAlphaValue, m_radiusValue;
	int   m_life;
	bool  m_bUse;

};

#endif

