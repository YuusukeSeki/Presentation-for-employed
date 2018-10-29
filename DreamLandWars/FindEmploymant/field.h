//*****************************************************************************
//
//		フィールド
//													Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"
#include "Object.h"
#include "texture.h"


class Field : public Object	// 3Dポリゴン
{
public:
	// 定数
	static const float FIELD_WIDTH_ALL;	 // 全体フィールド幅
	static const float FIELD_HEIGHT_ALL; // 全体フィールド高さ

	// コンストラクタ / デストラクタ
	virtual ~Field();

	// 基本的な関数
	static	Field*	Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXVECTOR3 scl, int numFieldHorizon, int numFieldVertical);
	virtual HRESULT	Init( D3DXVECTOR3 pos , D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXVECTOR3 scl, int numFieldHorizon, int numFieldVertical);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	// 設定
	void SetMtxWorld( D3DXMATRIX mtxWorld )	{ m_mtxWorld = mtxWorld; }
	void SetTexture(Texture* pTexture) { m_pTexture = pTexture; }

	// 取得
	const D3DXVECTOR3* GetRotate (void) const { return &m_rot; }
	const D3DXVECTOR3* GetScall (void) const { return &m_scl; }
	float GetHeight(D3DXVECTOR3 pos);
	D3DXVECTOR3& GetSize() { return m_size; }
	D3DXVECTOR3& GetHalfSize() { return m_halfSize; }


private:
	// コンストラクタ / デストラクタ
	Field() {}
	//Field(int priority, TYPE type);
	Field(Object::TYPE type);

	// 関数
	void MakeVertexBuf(void); // 頂点バッファ生成
	void MakeIdxBuf(void);    // インデックスバッファ生成

	// 変数
	D3DXMATRIX				m_mtxWorld;	   // ワールド行列
	D3DXVECTOR3             m_size;        // 全体の大きさ
	D3DXVECTOR3				m_rot;		   // 回転率
	D3DXVECTOR3				m_scl;		   // 拡縮率
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuf;	   // 頂点バッファ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuf;	   // インデックスバッファ
	D3DMATERIAL9			m_mat;		   // マテリアル情報（光）
	int                     m_numFieldHorizon;  // 横のフィールド数
	int                     m_numFieldVertical; // 縦のフィールド数

	D3DXVECTOR3 m_halfSize;	// 半分の大きさ

	Texture* m_pTexture;

};

#endif
