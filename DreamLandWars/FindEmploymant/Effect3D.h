//*****************************************************************************
//	
//		エフェクト（3D）
//													Author : Yusuke Seki
//*****************************************************************************
#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_

#include "main.h"
#include "EffectManager.h"


class Effect3D : public EffectManager
{
public:
	//----- 列挙型の定義 -----
	enum TYPE {	// 種類
		TYPE_FIRE,	// 炎

		__TYPEMAX,	// ※ 種類の最大数
	};


public:
	//----- コンストラクタ / デストラクタ -----
	Effect3D() {}
	Effect3D(EffectManager::TYPE type);
	virtual ~Effect3D();

	
	//----- 基本的な関数 -----
	static Effect3D* Create(D3DXVECTOR3& position, D3DXVECTOR3& size);
	virtual void Init(D3DXVECTOR3& position, D3DXVECTOR3& size);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);


	//----- 増減処理 -----
	void MovePosition(D3DXVECTOR3& movePosition);																				// 座標の移動
	void MoveSize(D3DXVECTOR3& moveSize);																						// 大きさの増減
	void MoveRotate(D3DXVECTOR3& moveRotate);																					// 回転率の増減
	void MoveScale(D3DXVECTOR3& moveScale);																						// 拡縮率の増減
	void MoveAccelerate(D3DXVECTOR3& moveAccelerate) { m_accelerate += moveAccelerate; }										// 加速度の増減
	void MoveNormal(D3DXVECTOR3& moveNormal0, D3DXVECTOR3& moveNormal1, D3DXVECTOR3& moveNormal2, D3DXVECTOR3& moveNormal3);	// 法線の増減
	void MoveColor(float r, float g, float b, float a);																			// 色の増減
	void MoveUV_StartPoint(D3DXVECTOR2& moveUV_StartPoint);																		// UV開始点の増減
	void MoveUV_Size(D3DXVECTOR2& moveUV_Size);																					// UV大きさの増減


	//----- 設定処理 -----
	static void SetEffect(D3DXVECTOR3& position, D3DXVECTOR3& vecZ, Effect3D::TYPE effectType);								// エフェクトの発生
	void SetPosition(D3DXVECTOR3& position);																				// 座標の設定
	void SetSize(D3DXVECTOR3& size);																						// 大きさの設定
	void SetRotate(D3DXVECTOR3& rotate);																					// 回転率の設定
	void SetScale(D3DXVECTOR3& scale);																						// 拡縮率の設定
	void SetAccelerate(D3DXVECTOR3& accelerate) { m_accelerate = accelerate; }												// 加速度の設定
	void SetNormal(D3DXVECTOR3& moveNormal0, D3DXVECTOR3& moveNormal1, D3DXVECTOR3& moveNormal2, D3DXVECTOR3& moveNormal3);	// 法線の設定
	void SetColor(int r, int g, int b, int a);																				// 色の設定
	void SetUV_StartPoint(D3DXVECTOR2& UV_StartPoint);																		// UV開始点の設定
	void SetUV_Size(D3DXVECTOR2& UV_Size);																					// UV大きさの設定
	void SetTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }													// テクスチャの設定


	//----- データを返す処理 -----
	D3DXMATRIX*        GetWorldMatrix()        { return &m_WorldMatrix; }		// ワールドマトリクスを返す
	D3DXVECTOR3*       GetSize()               { return &(m_halfSize * 2); }	// 大きさを返す
	D3DXVECTOR3*       GetHalfSize()           { return &m_halfSize; }			// 半分の大きさを返す
	D3DXVECTOR3*       GetRotate()             { return &m_rotate; }			// 回転率を返す
	D3DXVECTOR3*       GetScale()              { return &m_scale; }				// 拡縮率を返す
	float              GetRadius()             { return m_radius; }				// 半径を返す
	D3DXVECTOR3*       GetAccelerate()         { return &m_accelerate; }		// 加速度を返す
	D3DXVECTOR3*       GetNormal_LeftTop()     { return &m_normal[0]; }			// 法線を返す
	D3DXVECTOR3*       GetNormal_RightTop()    { return &m_normal[1]; }			// 法線を返す
	D3DXVECTOR3*       GetNormal_LeftBottom()  { return &m_normal[2]; }			// 法線を返す
	D3DXVECTOR3*       GetNormal_RightBottom() { return &m_normal[3]; }			// 法線を返す
	//unsigned int       GetColor()              { return m_color.color; }		// 色を返す
	D3DXVECTOR2*       GetUV_StartPoint()      { return &m_UV_StartPoint; }		// UV開始点を返す
	D3DXVECTOR2*       GetUV_Size()            { return &m_UV_Size; }			// UVの大きさを返す
	LPDIRECT3DTEXTURE9 GetTexture()            { return m_pTexture; }			// テクスチャを返す


protected:
	//----- データを返す処理 -----
	const bool GetInstance() { return m_bInstance; }	// 使用中フラグを返す：true で使用中


protected:
	//----- 設定処理 -----
	virtual void SetEffect_Protected(D3DXVECTOR3& position, D3DXVECTOR3& vecZ, Effect3D::TYPE effectType);							// エフェクトの設定
	        void SetUpdateVertexBuf(bool bUpdateVertexBuf)     { m_bUpdateVertexBuf = bUpdateVertexBuf; }		// true で頂点バッファの更新処理をする
	        void SetUpdateWorldMatrix(bool bUpdateWorldMatrix) { m_bUpdateWorldMatrix = bUpdateWorldMatrix; }	// true でワールドマトリクスの更新処理をする


private:
	//----- 関数 -----
	        void MakeVertexBuf();		// 頂点バッファの生成
	        void UpdateVertexBuf();		// 頂点バッファの更新
	virtual void UpdateWorldMatrix();	// ワールドマトリクスの更新

	//----- データ -----
	D3DXMATRIX  m_WorldMatrix;		// ワールドマトリクス
	D3DXVECTOR3 m_halfSize;			// 半分の大きさ
	D3DXVECTOR3 m_rotate;			// 回転率
	D3DXVECTOR3 m_scale;			// 拡縮率
	float       m_radius;			// 半径
	D3DXVECTOR3 m_accelerate;		// 加速度
	D3DXVECTOR3 m_normal[4];		// 法線
	float       m_color_R;			// 赤色
	float       m_color_G;			// 緑色
	float       m_color_B;			// 青色
	float       m_color_A;			// 透過色
	D3DXVECTOR2 m_UV_StartPoint;	// UV開始点
	D3DXVECTOR2 m_UV_Size;			// UV大きさ

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	LPDIRECT3DTEXTURE9      m_pTexture;	// テクスチャ

	bool m_bUpdateVertexBuf;	// true で頂点バッファの更新をする
	bool m_bUpdateWorldMatrix;	// true でワールドマトリクスの更新をする
	bool m_bInstance;			// true で使用中


	//----- データ -----
	unsigned int m_life;							// 寿命
	D3DXVECTOR3  m_ChangeRotateByFrame;			// フレーム間の回転率変化量
	D3DXVECTOR3  m_ChangeScaleByFrame;			// フレーム間の拡縮率変化量
	D3DXVECTOR3  m_ChangeAccelerateByFrame;		// フレーム間の加速度変化量
	float        m_ChangeRedByFrame;			// フレーム間の頂点色・赤、変化量
	float        m_ChangeGreenByFrame;			// フレーム間の頂点色・緑、変化量
	float        m_ChangeBlueByFrame;			// フレーム間の頂点色・青、変化量
	float        m_ChangeAlphaByFrame;			// フレーム間の頂点色・透過値、変化量
	D3DXVECTOR2  m_ChangeUVStartPointByFrame;	// フレーム間のUV開始点変化量
	D3DXVECTOR2  m_ChangeUVSizeByFrame;			// フレーム間のUVサイズ変化量

};

#endif

