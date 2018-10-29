//*****************************************************************************
//
//		2Dポリゴン
//												Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include "scene.h"
#include "texture.h"



class Scene2D : public Scene	// 2Dポリゴン
{
public:
	// コンストラクタ / デストラクタ
	virtual ~Scene2D();

	// 基本的な関数
	static Scene2D* Create( D3DXVECTOR3 pos , D3DXVECTOR3 size );
	virtual HRESULT Init( D3DXVECTOR3 pos , D3DXVECTOR3 size );
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	// 増減
	void MovePos(D3DXVECTOR3 move);
	//void MoveSize(D3DXVECTOR3 move);
	//void MoveColor(int r, int g, int b, int a);
	//void MoveUV(D3DXVECTOR2 move);

	// 設定
	void SetPos( D3DXVECTOR3 pos );
	//void SetSize(D3DXVECTOR3 size);
	void SetColor(unsigned int rgba);
	void SetUV(float U, float V, float width, float height);
	void SetTexture(Texture* pTexture) { m_pTexture = pTexture; }
	Texture* GetTexture() { return m_pTexture; }


protected:
	Scene2D() {}
	Scene2D(int priority, TYPE type);


private:
	D3DXVECTOR3 m_halfSize;
	Color m_color;
	D3DXVECTOR2 m_UV_StartPoint;
	D3DXVECTOR2 m_UV_Size;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	Texture* m_pTexture;


};

#endif