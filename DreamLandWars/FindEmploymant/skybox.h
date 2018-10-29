//*****************************************************************
//
//		スカイボックス
//										Autohr : Yusuke Seki
//
//*****************************************************************
#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "main.h"
#include "scene.h"
#include "Object.h"
#include "texture.h"



class SkyBox : public Object
{
public:
	// コンストラクタ / デストラクタ
	virtual ~SkyBox();

	// 基本的な関数
	static SkyBox* Create(D3DXVECTOR3 pos, float width);
	HRESULT Init(D3DXVECTOR3 pos, float width);
	void Uninit();
	void Update();
	void Draw();

	// 増減
	void MovePos(D3DXVECTOR3 move);

	// 設定
	void SetPos(D3DXVECTOR3 pos);
	void SetTexture(Texture* pTexture) { m_pTexture = pTexture; }
	

protected:


private:
	// コンストラクタ / デストラクタ
	SkyBox(){}
	SkyBox(Object::TYPE type);

	// 関数
	void MakeVertexBuf();

	// 変数
	D3DXMATRIX m_mtxWorld; // ワールド行列
	float m_width;         // 一辺の長さ

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	Texture* m_pTexture;                // テクスチャ


};

#endif
