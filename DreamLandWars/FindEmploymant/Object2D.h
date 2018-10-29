//*****************************************************************************
//
//		2Dポリゴン（リスト構造）
//												Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

#include "main.h"
#include "object.h"


class Object2D : public Object	// 2Dポリゴン
{
public:
	//----- コンストラクタ / デストラクタ -----
	Object2D();
	Object2D(Object::TYPE type);
	virtual ~Object2D();

	
	//----- 基本的な関数 -----
	static Object2D* Create(D3DXVECTOR3& position, D3DXVECTOR3& size);
	virtual void Init(D3DXVECTOR3& position, D3DXVECTOR3& size);
	virtual void Uninit(void);
	virtual void Update(void) {}
	virtual void Draw(void);


	//----- 増減処理 -----
	virtual void MovePosition(D3DXVECTOR3& movePosition);			// 座標の移動
	virtual void MoveSize(D3DXVECTOR3& moveSize);					// 大きさの増減
	virtual void MoveRotate(float moveRotate);						// 回転率の増減
	virtual void MoveColor(int r, int g, int b, int a);				// 色の増減
	virtual void MoveUV_StartPoint(D3DXVECTOR2& moveUV_StartPoint);	// UV開始点の増減
	virtual void MoveUV_Size(D3DXVECTOR2& moveUV_Size);				// UV大きさの増減


	//----- 設定処理 -----
	virtual void SetPosition(D3DXVECTOR3& position);								// 座標の設定
	virtual void SetSize(D3DXVECTOR3& size);										// 大きさの設定
	virtual void SetRotate(float rotate);											// 回転率の設定
	virtual void SetColor(unsigned int rgba);										// 色の設定
	virtual void SetUV_StartPoint(D3DXVECTOR2& UV_StartPoint);						// UV開始点の設定
	virtual void SetUV_Size(D3DXVECTOR2& UV_Size);									// UV大きさの設定
	        void SetTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }	// テクスチャの設定
	        void SetDraw(bool bDraw) { m_bDraw = bDraw; }							// 描画処理の有無の設定


	//----- データを返す処理 -----
	D3DXVECTOR3        GetSize()          { return (m_halfSize * 2); }	// 大きさを返す
	D3DXVECTOR3&       GetHalfSize()      { return m_halfSize; }		// 半分の大きさを返す
	float              GetRadius()        { return m_radius; }			// 半径を返す
	float              GetAngle()         { return m_angle; }			// atanを返す
	float              GetRotate()        { return m_rotate; }			// 回転率を返す
	unsigned int       GetColor()         { return m_color.color; }		// 色を返す
	D3DXVECTOR2&       GetUV_StartPoint() { return m_UV_StartPoint; }	// UV開始点を返す
	D3DXVECTOR2&       GetUV_Size()       { return m_UV_Size; }			// UVの大きさを返す
	LPDIRECT3DTEXTURE9 GetTexture()       { return m_pTexture; }		// テクスチャを返す


protected:
	//----- 共用体の定義 -----
	union Color { // 色
		unsigned int color;
		unsigned char rgba[4];
	};


private:
	//----- 関数 -----
	void MakeVertexBuf();		// 頂点バッファの生成
	void UpdateVertexBuf();		// 頂点バッファの更新


	//----- データ -----
	D3DXVECTOR3 m_halfSize;			// 半分の大きさ
	float       m_radius;			// 半径
	float       m_angle;			// atan
	float       m_rotate;			// 回転率
	Color       m_color;			// 色
	D3DXVECTOR2 m_UV_StartPoint;	// UV開始点
	D3DXVECTOR2 m_UV_Size;			// UV大きさ

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	LPDIRECT3DTEXTURE9      m_pTexture;	// テクスチャ

	bool m_bUpdateVertexBuf;	// true で頂点バッファの更新をする
	bool m_bDraw;				// true で描画処理を行う

};

#endif
