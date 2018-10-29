//*****************************************************************************
//
//		モデル（リスト構造）
//													Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _OBJECTMODEL_H_
#define _OBJECTMODEL_H_

#include "main.h"
#include "Object.h"


class ObjectModel : public Object
{
public:
	//----- コンストラクタ / デストラクタ -----
	ObjectModel();
	ObjectModel(Object::TYPE type);
	virtual ~ObjectModel();

	
	//----- 基本的な関数 -----
	static ObjectModel* Create(D3DXVECTOR3& position, const char* FileName);
	virtual void Init(D3DXVECTOR3& position, const char* FileName);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);


	//----- 増減処理 -----
	virtual void MovePosition(D3DXVECTOR3& movePosition);	// 座標の移動
	        void MoveSize(D3DXVECTOR3& moveSize);			// 大きさの増減
	        void MoveRotate(D3DXVECTOR3& moveRotate);		// 回転率の増減
	        void MoveScale(D3DXVECTOR3& moveScale);			// 拡縮率の増減
	        void MoveColor(int r, int g, int b, int a);		// 色の増減

	//----- 設定処理 -----
	void SetPosition(D3DXVECTOR3& position);									// 座標の設定
	void SetSize(D3DXVECTOR3& size);											// 大きさの設定
	void SetRotate(D3DXVECTOR3& rotate);										// 回転率の設定
	void SetScale(D3DXVECTOR3& scale);											// 拡縮率の設定
	void SetColor(unsigned int rgba);											// 色の設定
	void SetTexture(LPDIRECT3DTEXTURE9 pTexture) { *m_pTexture = pTexture; }	// テクスチャの設定
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }								// 描画処理の有無の設定
	void SetFront(D3DXVECTOR3& front) { m_front = front; }

	// 怪しい
	void SetRotateToObj(D3DXVECTOR3& ObjectPostion);	// 対象の方向を向くように設定する
	void SetRadius(float radius) { m_radius = radius; }	// 半径の設定


	//----- データを返す処理 -----
	D3DXMATRIX&        GetWorldMatrix() { return m_WorldMatrix; }		// ワールドマトリクスを返す
	D3DXVECTOR3        GetSize()        { return (m_halfSize * 2); }	// 大きさを返す
	D3DXVECTOR3&       GetHalfSize()    { return m_halfSize; }			// 半分の大きさを返す
	D3DXVECTOR3&       GetRotate()      { return m_rotate; }			// 回転率を返す
	D3DXVECTOR3&       GetScale()       { return m_scale; }				// 拡縮率を返す
	float              GetRadius()      { return m_radius; }			// 半径を返す
	D3DXVECTOR3&       GetFront()       { return m_front; }				// 前方向を返す
	LPDIRECT3DTEXTURE9 GetTexture()     { return *m_pTexture; }			// テクスチャを返す


private:
	//----- 共用体の定義 -----
	union Color { // 色
		unsigned int color;
		unsigned char rgba[4];
	};


	//----- 関数 -----
	void MakeVertexBuf();							// 頂点バッファの生成
	void UpdateVertexBuf();							// 頂点バッファの更新
	void UpdateWorldMatrix();						// ワールドマトリクスの更新
	void LoadModel(const char* FileName);			// モデルデータの読み込み
	void LoadModelSizeFromX(const char* FileName);	// Xファイルからサイズを読み込む
	void LoadMaterial();							// マテリアル情報の読み込み
	void LoadMeshModel_DX(const char* FileName);	// モデルデータの読み込み（directXの便利関数使用）


	//----- データ -----
	LPD3DXMESH	  m_pMesh;		// メッシュ情報
	D3DMATERIAL9* m_pMeshMat;	// マテリアル情報
	DWORD		  m_numMat;		// マテリアル数

	D3DXMATRIX	m_WorldMatrix;	// ワールドマトリクス
	D3DXVECTOR3 m_halfSize;		// 半分の大きさ
	D3DXVECTOR3 m_rotate;		// 回転率
	D3DXVECTOR3	m_scale;		// 拡縮率
	float       m_radius;		// 半径
	Color       m_color;		// 色
	D3DXVECTOR3 m_front;		// 前方向

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	LPDIRECT3DTEXTURE9*     m_pTexture;	// テクスチャ

	bool m_bUpdateVertexBuf;	// true で頂点バッファの更新をする
	bool m_bUpdateWorldMatrix;	// true でワールドマトリクスの更新をする
	bool m_bDraw;				// true で描画処理を行う

};

#endif

