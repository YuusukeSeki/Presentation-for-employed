//*****************************************************************************
//
//		当たり判定
//												Autohr : Yusuke Seki
//*****************************************************************************
#include "collision.h"
#include "renderer.h"
#include "scene.h"


#define IKD_EPSIRON 0.00001f // 誤差



///////////////////////////////////////////////////
// スクリーン座標をワールド座標へ変換
// out    : ワールド座標（出力）
// pDev   : 描画デバイス
// sx, xy : スクリーン座標
// z      : スクリーン座標の仮想的なZ成分（0.0～1.0）
// view   : ビュー行列
// proj   : 射影変換行列
// 戻り値 : ワールド座標（出力）
D3DXVECTOR3* transScreenToWorld(D3DXVECTOR3* out, IDirect3DDevice9* pDev, int sx, int sy, float z, D3DXMATRIX* view, D3DXMATRIX* proj)
{
	// ビューポート行列を作成
	D3DXMATRIX vpMat;
	D3DXMatrixIdentity(&vpMat);

	D3DVIEWPORT9 vp;
	pDev->GetViewport(&vp);

	vpMat._11 = (float)vp.Width / 2;
	vpMat._22 = -1.0f * (float)(vp.Height / 2);
	vpMat._33 = (float)vp.MaxZ - vp.MinZ;
	vpMat._41 = (float)(vp.X + vp.Width / 2);
	vpMat._42 = (float)(vp.Y + vp.Height / 2);
	vpMat._43 = vp.MinZ;

	// スクリーン位置をワールドへ
	out->x = (float)sx;
	out->y = (float)sy;
	out->z = z;

	D3DXMATRIX invMat, inv_proj, inv_view;
	D3DXMatrixInverse(&invMat, 0, &vpMat);
	D3DXMatrixInverse(&inv_proj, 0, proj);
	D3DXMatrixInverse(&inv_view, 0, view);

	invMat *= inv_proj * inv_view;

	return D3DXVec3TransformCoord(out, out, &invMat);

}


///////////////////////////////////////////////////
// 球と無限レイの衝突判定
// r          : 球の半径
// center     : 球の中心点
// s          : レイの開始点
// v          : レイの方向ベクトル
// pOut_colli : 衝突位置
// pOut_t     : 衝突時刻（出力）
// 戻り値     : 衝突(true), 非衝突(false)
bool CalcSphereRayCollision(float r, D3DXVECTOR3* center, D3DXVECTOR3* s, D3DXVECTOR3* v, D3DXVECTOR3* pOut_colli, float* pOut_t)
{
	D3DXVECTOR3 u = *s - *center;

	float a = D3DXVec3Dot(v, v);
	float b = D3DXVec3Dot(v, &u);
	float c = D3DXVec3Dot(&u, &u) - r * r;

	if (a - IKD_EPSIRON <= 0.0f) {
		// 誤差
		return false;
	}

	float isColli = b * b - a * c;
	if (isColli < 0.0f) {
		// 衝突しない
		return false;
	}

	float t = (-b - sqrtf(b * b - a * c)) / a;

	if (pOut_t) {
		*pOut_t = t;
	}

	if (pOut_colli) {
		*pOut_colli = *s + *v * t;
	}

	// 衝突している！
	return true;
}

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
bool CalcParticlePlaneCollision(FLOAT r, D3DXVECTOR3 *pPre_pos, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pNormal, D3DXVECTOR3 *pPlane_pos, FLOAT *t, D3DXVECTOR3 *pOut_colli)
{
	D3DXVECTOR3 C0 = *pPre_pos - *pPlane_pos; // 平面上の一点から現在位置へのベクトル
	D3DXVECTOR3 D = *pPos - *pPre_pos; // 現在位置から予定位置までのベクトル
	D3DXVECTOR3 N; // 法線
	D3DXVec3Normalize(&N, pNormal); // 法線を標準化

									// 平面と中心点の距離を算出
	FLOAT Dot_C0 = D3DXVec3Dot(&C0, &N);
	FLOAT dist_plane_to_point = (float)fabs(Dot_C0);

	// 進行方向と法線の関係をチェック
	FLOAT Dot = D3DXVec3Dot(&D, &N);

	// 平面と平行に移動してめり込んでいるスペシャルケース
	if ((IKD_EPSIRON - fabs(Dot) > 0.0f) && (dist_plane_to_point < r)) {
		// 一生抜け出せないので最大時刻を返す
		*t = FLT_MAX;
		// 衝突位置は仕方ないので今の位置を指定
		*pOut_colli = *pPre_pos;
		return true;
	}

	// 交差時間の算出
	*t = (r - Dot_C0) / Dot;

	// 衝突位置の算出
	*pOut_colli = *pPre_pos + (*t) * D;

	// めり込んでいたら衝突として処理終了
	if (dist_plane_to_point < r)
		return true;

	// 壁に対して移動が逆向きなら衝突していない
	if (Dot >= 0)
		return false;

	// 時間が0～1の間にあれば衝突
	if ((0 <= *t) && (*t <= 1))
		return true;

	return false;
}

///////////////////////////////////////////
// 壁ずりベクトル
// out : 正規化壁ずりベクトル（戻り値）
// front : 進行ベクトル
// normal: 衝突点での法線ベクトル
D3DXVECTOR3* calcWallScratchVector(D3DXVECTOR3* out, const D3DXVECTOR3& front, const D3DXVECTOR3& normal)
{
	D3DXVECTOR3 normal_n;
	D3DXVec3Normalize(&normal_n, &normal);
	return D3DXVec3Normalize(out, &(front - D3DXVec3Dot(&front, &normal_n) * normal_n));
}

/////////////////////////////////////////
// 反射ベクトル
// out : 正規化反射ベクトル（戻り値）
// front : 進行ベクトル
// normal: 衝突点での法線ベクトル
D3DXVECTOR3* calcReflectVector(D3DXVECTOR3* out, const D3DXVECTOR3& front, const D3DXVECTOR3& normal)
{
	D3DXVECTOR3 normal_n;
	D3DXVec3Normalize(&normal_n, &normal);
	return D3DXVec3Normalize(out, &(front - 2.0f * D3DXVec3Dot(&front, &normal_n) * normal_n));
}

//頂点ABCで作られたポリゴンから法線を計算する。
D3DXVECTOR3* CreatePolygonNormal(D3DXVECTOR3* out, D3DXVECTOR3& posA, D3DXVECTOR3& posB, D3DXVECTOR3& posC)
{
	D3DXVECTOR3 AB(posB - posA);
	D3DXVECTOR3 BC(posC - posB);

	D3DXVec3Cross(out, &AB, &BC);	//AB BCの外積
	D3DXVec3Normalize(out, out);	//単位ベクトルにする

	return out;
}


//-----------------------------------------------------------------------------
// ベクトルの長さの２乗を計算
//-----------------------------------------------------------------------------
float VectorLength(D3DXVECTOR3& vec)
{
	return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

//-----------------------------------------------------------------------------
// 2点間の距離計算
//-----------------------------------------------------------------------------
float Distance3D(const D3DXVECTOR3& p1, const D3DXVECTOR3& p2)
{
	D3DXVECTOR3 sub = D3DXVECTOR3(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);

	return sub.x * sub.x + sub.y * sub.y + sub.z * sub.z;
}

//=============================================================================
//	当たり判定
// 点と線分
bool Collision_PointToSegment(D3DXVECTOR3& pointPosition, D3DXVECTOR3& segmentPosition, D3DXVECTOR3& segmentVector)
{
	return false;
}

// 線分と線分
bool Collision_SegmentToSegment(D3DXVECTOR3& posA, float radiusA, D3DXVECTOR3& posB, float radiusB)
{
	return false;

}

// 線分と平面
bool Collision_SegmentToPlane(D3DXVECTOR3& posA, float radiusA, D3DXVECTOR3& posB, float radiusB)
{
	return false;

}

// 球と球
bool Collision_SphereToSphere(D3DXVECTOR3 posA, float radiusA, D3DXVECTOR3 posB, float radiusB) {
	if ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z)
		<= (radiusA + radiusB) * (radiusA + radiusB)) {
		return true;
	}
	return false;
}

// 筒と筒
bool Collision_CapsuleToCapsule(D3DXVECTOR3& posA, float radiusA, D3DXVECTOR3& posB, float radiusB)
{
	return false;

}

// 線分と板ポリゴン
bool Collision_SegmentToSquare(D3DXVECTOR3& startPos, D3DXVECTOR3& endPos, D3DXVECTOR3& cenPos, D3DXVECTOR3& halfSize)
{
	D3DXVECTOR3 vec = endPos - startPos;
	D3DXVec3Normalize(&vec, &vec);

	float d1 = startPos.y - cenPos.y;
	float d2 = cenPos.y   - endPos.y;

	float a = d1 / (d1 + d2);

	D3DXVECTOR3 point = startPos + vec * a;

	D3DXVECTOR3 p0, p1, p2, p3;
	D3DXVECTOR3 v01, v12, v23, v30, v0p, v1p, v2p, v3p;
	D3DXVECTOR3 c0, c1, c2, c3;

	p0 = D3DXVECTOR3(cenPos.x - halfSize.x, cenPos.y + halfSize.y, cenPos.z + halfSize.z);
	p1 = D3DXVECTOR3(cenPos.x + halfSize.x, cenPos.y + halfSize.y, cenPos.z + halfSize.z);
	p2 = D3DXVECTOR3(cenPos.x - halfSize.x, cenPos.y - halfSize.y, cenPos.z - halfSize.z);
	p3 = D3DXVECTOR3(cenPos.x + halfSize.x, cenPos.y - halfSize.y, cenPos.z - halfSize.z);

	v01 = p1 - p0;
	v12 = p2 - p1;
	v23 = p3 - p2;
	v30 = p0 - p3;
	v0p = point - p0;
	v1p = point - p1;
	v2p = point - p2;
	v3p = point - p3;

	D3DXVec3Cross(&c0, &v01, &v0p);
	D3DXVec3Cross(&c1, &v12, &v1p);
	D3DXVec3Cross(&c2, &v23, &v2p);
	D3DXVec3Cross(&c3, &v30, &v3p);

	if (c0 <= 0 && c1 <= 0 && c2 <= 0 && c3 <= 0) return true;

	return false;

}


