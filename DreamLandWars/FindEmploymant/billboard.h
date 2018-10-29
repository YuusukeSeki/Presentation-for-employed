//*****************************************************************************
//
//		ビルボード
//												Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"
#include "scene.h"
#include "texture.h"

class Camera;


class Billboard : public Scene
{
public:
	// コンストラクタ / デストラクタ
	~Billboard();

	// 基本的な関数
	static	Billboard* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXVECTOR3 scl); // 実体の生成
	virtual HRESULT	Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXVECTOR3 scl);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	// 増減
	void MoveSize(D3DXVECTOR3 move);
	void MoveRot(D3DXVECTOR3 move) { m_rot += move; }
	void MoveScl(D3DXVECTOR3 move) { m_scl += move; }

	// 設定
	void SetSize(D3DXVECTOR3 size);
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetScl(D3DXVECTOR3 scl) { m_scl = scl; }
	void SetTexture(Texture* pTexture) { m_pTexture = pTexture; }
	void SetColor(unsigned int color);

	// 取得
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3 GetRotate(void) { return m_rot; }
	D3DXVECTOR3 GetScall(void) { return m_scl; }
	Scene::Color GetColor(int* r = nullptr, int* g = nullptr, int* b = nullptr, int* a = nullptr) { return m_color; }
	Texture* GetTexture() { return m_pTexture; }


protected:
	// コンストラクタ / デストラクタ
	Billboard() {}
	Billboard(int priority, TYPE type);


private:
	// 変数
	D3DXMATRIX m_mtxWorld;
	D3DXVECTOR3 m_size;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_scl;
	Scene::Color m_color;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	Texture* m_pTexture;

};

#endif