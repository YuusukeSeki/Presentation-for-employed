// author : yusuke seki
// data   : 20181111
#ifndef OBJECTCUBE_H_
#define OBJECTCUBE_H_

#include "Object.h"
#include "Object3D.h"

class ObjectCube : public Object
{
public:
	//----- コンストラクタ / デストラクタ -----
	ObjectCube();
	ObjectCube(Object::TYPE type);
	virtual ~ObjectCube();

	
	//----- 基本的な関数 -----
	static ObjectCube* Create(D3DXVECTOR3& position, D3DXVECTOR3& size);
	virtual void Init(D3DXVECTOR3& position, D3DXVECTOR3& size);
	virtual void Uninit(void);
	virtual void Update(void) {}
	virtual void Draw(void) {}


	//----- 増減処理 -----
	void MovePosition(D3DXVECTOR3& movePosition);																				// 座標の移動
	void MoveSize(D3DXVECTOR3& moveSize);																						// 大きさの増減
	void MoveRotate(D3DXVECTOR3& moveRotate);																					// 回転率の増減
	void MoveScale(D3DXVECTOR3& moveScale);																						// 拡縮率の増減
	void MoveColor(int r, int g, int b, int a);																					// 色の増減
	void MoveUV_StartPoint(D3DXVECTOR2& moveUV_StartPoint);																		// UV開始点の増減
	void MoveUV_Size(D3DXVECTOR2& moveUV_Size);																					// UV大きさの増減


	//----- 設定処理 -----
	void SetPosition(D3DXVECTOR3& position);																					// 座標の設定
	void SetSize(D3DXVECTOR3& size);																							// 大きさの設定
	void SetRotate(D3DXVECTOR3& rotate);																						// 回転率の設定
	void SetScale(D3DXVECTOR3& scale);																							// 拡縮率の設定
	void SetColor(unsigned int rgba);																							// 色の設定
	void SetUV_StartPoint(D3DXVECTOR2& UV_StartPoint);																			// UV開始点の設定
	void SetUV_Size(D3DXVECTOR2& UV_Size);																						// UV大きさの設定
	void SetTexture(LPDIRECT3DTEXTURE9 pTexture);																				// テクスチャの設定
	void SetDraw(bool bDraw);																									// 描画処理の有無の設定


	//----- データを返す処理 -----
	D3DXVECTOR3 GetSize() { return (m_halfSize * 2); }
	D3DXVECTOR3& GetHalfSize() { return m_halfSize; }
	D3DXVECTOR3& GetRotate() { return m_rotate; }
	D3DXVECTOR3& GetScale() { return m_scale; }
	float GetRadius() { return m_radius; }
	D3DXVECTOR3& GetNormal_LeftTop() { return m_normal[0]; }
	D3DXVECTOR3& GetNormal_RightTop() { return m_normal[1]; }
	D3DXVECTOR3& GetNormal_LeftBottom() { return m_normal[2]; }
	D3DXVECTOR3& GetNormal_RightBottom() { return m_normal[3]; }
	unsigned int GetColor() { return m_color.color; }
	D3DXVECTOR2& GetUV_StartPoint() { return m_UV_StartPoint; }
	D3DXVECTOR2& GetUV_Size() { return m_UV_Size; }
	LPDIRECT3DTEXTURE9 GetTexture() { return m_pTexture; }

	//D3DXVECTOR3& GetNormal_TOP()	{ return _surface_TOP->GetNormal_Suface(); }
	//D3DXVECTOR3& GetNormal_BOTTOM()	{ return _surface_BOTTOM->GetNormal_Suface(); }
	//D3DXVECTOR3& GetNormal_LEFT()	{ return _surface_LEFT->GetNormal_Suface(); }
	//D3DXVECTOR3& GetNormal_RIGHT()	{ return _surface_RIGHT->GetNormal_Suface(); }
	//D3DXVECTOR3& GetNormal_FRONT()	{ return _surface_FRONT->GetNormal_Suface(); }
	//D3DXVECTOR3& GetNormal_BACK()	{ return _surface_BACK->GetNormal_Suface(); }

	D3DXVECTOR3& GetPosition_TOP()		{ return _surface_TOP->GetPosition(); }
	D3DXVECTOR3& GetPosition_BOTTOM()	{ return _surface_BOTTOM->GetPosition(); }
	D3DXVECTOR3& GetPosition_LEFT()		{ return _surface_LEFT->GetPosition(); }
	D3DXVECTOR3& GetPosition_RIGHT()	{ return _surface_RIGHT->GetPosition(); }
	D3DXVECTOR3& GetPosition_FRONT()	{ return _surface_FRONT->GetPosition(); }
	D3DXVECTOR3& GetPosition_BACK()		{ return _surface_BACK->GetPosition(); }

protected:
	//----- 共用体の定義 -----
	union Color { // 色
		unsigned int color;
		unsigned char rgba[4];
	};


private:
	//----- データ -----
	Object3D* _surface_TOP;		// Surface of Top
	Object3D* _surface_BOTTOM;	// Surface of Bottom
	Object3D* _surface_LEFT;	// Surface of Left
	Object3D* _surface_RIGHT;	// Surface of Right
	Object3D* _surface_FRONT;	// Surface of Front
	Object3D* _surface_BACK;	// Surface of Back

	D3DXVECTOR3 m_halfSize;			// 半分の大きさ
	D3DXVECTOR3 m_rotate;			// 回転率
	D3DXVECTOR3 m_scale;			// 拡縮率
	float       m_radius;			// 半径
	D3DXVECTOR3 m_normal[4];		// 法線
	Color       m_color;			// 色
	D3DXVECTOR2 m_UV_StartPoint;	// UV開始点
	D3DXVECTOR2 m_UV_Size;			// UV大きさ

	LPDIRECT3DTEXTURE9      m_pTexture;	// テクスチャ

	bool m_bDraw;				// true で描画処理を行う

};

#endif

