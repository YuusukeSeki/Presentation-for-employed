//*****************************************************************************
//	
//		バレット（3D）
//													Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _OL3D_BULLET_H_
#define _OL3D_BULLET_H_

#include "main.h"
#include "Object3D.h"



class Bullet3D : public Object3D
{
public:
	//----- コンストラクタ / デストラクタ -----
	Bullet3D() {}
	Bullet3D(Object::TYPE type);
	virtual ~Bullet3D();


	//----- 基本的な関数 -----
	static Bullet3D* Create(D3DXVECTOR3& position, D3DXVECTOR3& size);
	virtual void Init(D3DXVECTOR3& position, D3DXVECTOR3& size);
	virtual void Uninit(void);
	virtual void Update(void) {}
	virtual void Draw(void);

	
	//----- 設定処理 -----
	//void SetBullet(); // ※未使用


	//----- データを返す処理 -----
	D3DXVECTOR3& GetVecZ()     { return m_vecZ; }		// 向いてる方向を返す
	bool         GetInstance() { return m_bInstance; }	// 使用中フラグを返す


protected:
	//----- 設定処理 -----
	void SetVecZ(D3DXVECTOR3& vecZ);								// 向いている方向の設定
	void SetMove(float move) { m_move = move; }							// 初速度の設定
	void SetAccelerate(float accelerate) { m_accelerate = accelerate; }	// 加速度の設定
	void SetInstance(bool bInstanse) { m_bInstance = bInstanse; }		// 使用中フラグの設定

	//----- データを返す処理 -----
	float GetMove()       { return m_move; }		// 初速度を返す
	float GetAccelerate() { return m_accelerate; }	// 加速度を返す


private:
	//----- データ -----
	D3DXVECTOR3 m_vecZ;			// 向き
	float       m_move;			// 初速度
	float       m_accelerate;	// 加速度
	bool        m_bInstance;	// 使用中フラグ

	// 寿命で消えるタイプのバレット用
	//int m_life;

	// 距離で消えるタイプのバレット用
	//D3DXVECTOR3 m_uninitPos;	   // 消失地点
	//D3DXVECTOR3 m_vecInitToUninit; // 消失用の単位ベクトル


};


#endif

#pragma once
