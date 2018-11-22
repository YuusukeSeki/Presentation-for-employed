//*****************************************************************************
//
//		ライト
//													Autohr : Yusuke Seki
//
//*****************************************************************************
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"


class Light
{
public:
	// コンストラクタ / デストラクタ
	Light();
	~Light();

	// 基本的な関数
	static Light* Create(void);
	void Release(void);
	void Init(void);
	void Uninit(void);
	void Update(void);

	// 設定
	void SetDiffuse(float r, float g, float b, float a);
	void SetAmbient(float r, float g, float b, float a);
	void SetSpecular(float r, float g, float b, float a);

	// 向きの設定
	// vecDirection : 設定したい向き
	void SetVectorDirection(const D3DXVECTOR3& _vecDirection);



private:
	DWORD index_;
	D3DLIGHT9	m_light;
	D3DXVECTOR3	m_vecDir;	// 光の向き


};

#endif