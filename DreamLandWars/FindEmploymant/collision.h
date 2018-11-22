//*****************************************************************************
//
//		当たり判定
//													Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "main.h"


class Collision_History
{
public:
	Collision_History() { m_pNext = nullptr; }
	void SetNextPointer(void* pNext) { m_pNext = (Collision_History*)pNext; }
	void* GetNextPointer() { return m_pNext; }

private:
	Collision_History * m_pNext;

};



// ベクトルの長さの２乗を計算
float VectorLength(D3DXVECTOR3& vec);

// 2点間の距離の2乗を計算
float Distance3D(const D3DXVECTOR3& p1, const D3DXVECTOR3& p2);

 // カーソル座標を3D座標に変換
D3DXVECTOR3* transScreenToWorld(D3DXVECTOR3* out, IDirect3DDevice9* pDev, int sx, int sy, float z, D3DXMATRIX* view, D3DXMATRIX* proj);

// 【当たり判定】球と無限レイ
bool CalcSphereRayCollision(float r, D3DXVECTOR3* center, D3DXVECTOR3* s, D3DXVECTOR3* v, D3DXVECTOR3* pOut_colli, float* pOut_t);

// 【当たり判定】点と線分
bool Collision_PointToSegment(D3DXVECTOR3& pointPosition, D3DXVECTOR3& segmentPosition, D3DXVECTOR3& segmentVector);

// 【当たり判定】線分と線分
bool Collision_SegmentToSegment(D3DXVECTOR3& posA, float radiusA, D3DXVECTOR3& posB, float radiusB);

// 【当たり判定】線分と平面
bool Collision_SegmentToPlane(D3DXVECTOR3& posA, float radiusA, D3DXVECTOR3& posB, float radiusB);

// 【当たり判定】球と球
bool Collision_SphereToSphere(D3DXVECTOR3 posA, float radiusA, D3DXVECTOR3 posB, float radiusB);

// 【当たり判定】筒と筒
bool Collision_CapsuleToCapsule(D3DXVECTOR3& posA, float radiusA, D3DXVECTOR3& posB, float radiusB);

// 【当たり判定】線分と板ポリゴン
bool Collision_SegmentToSquare(D3DXVECTOR3& startPos, D3DXVECTOR3& endPos, D3DXVECTOR3& cenPos, D3DXVECTOR3& halfSize);

///////////////////////////////////////////////////
// 平面パーティクル衝突判定・時刻・位置算出関数
// r : パーティクルの半径
// pPre_pos : パーティクルの前の位置
// pPos : パーティクルの次の到達位置
// pNormal : 平面の法線
// pPlane_pos : 平面上の1点
// pOut_t : 衝突時間を格納するFLOAT型へのポインタ
// pOut_colli : パーティクルの衝突位置を格納するD3DXVECTOR型へのポインタ
// 戻り値 : 衝突(true), 非衝突(false)
bool CalcParticlePlaneCollision(FLOAT r, D3DXVECTOR3 *pPre_pos, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pNormal, D3DXVECTOR3 *pPlane_pos, FLOAT *t, D3DXVECTOR3 *pOut_colli);

///////////////////////////////////////////
// 壁ずりベクトル
// out : 正規化壁ずりベクトル（戻り値）
// front : 進行ベクトル
// normal: 衝突点での法線ベクトル
D3DXVECTOR3* calcWallScratchVector(D3DXVECTOR3* out, const D3DXVECTOR3& front, const D3DXVECTOR3& normal);

/////////////////////////////////////////
// 反射ベクトル
// out : 正規化反射ベクトル（戻り値）
// front : 進行ベクトル
// normal: 衝突点での法線ベクトル
D3DXVECTOR3* calcReflectVector(D3DXVECTOR3* out, const D3DXVECTOR3& front, const D3DXVECTOR3& normal);

//頂点ABCで作られたポリゴンから法線を計算する。
D3DXVECTOR3* CreatePolygonNormal(D3DXVECTOR3* out, D3DXVECTOR3& posA, D3DXVECTOR3& posB, D3DXVECTOR3& posC);

#endif
