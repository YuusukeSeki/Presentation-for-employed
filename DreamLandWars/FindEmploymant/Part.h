//*****************************************************************************
//
//		Part
//													Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _PART_H_
#define _PART_H_

#include "main.h"
#include "Object.h"


class Part : public Object
{
public:
	//----- コンストラクタ / デストラクタ -----
	Part();
	Part(Object::TYPE type);
	virtual ~Part();


	//----- 基本的な関数 -----
	static Part* Create(D3DXVECTOR3& position, const char* FileName);
	virtual void Init(D3DXVECTOR3& position, const char* FileName);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);


	//----- 増減処理 -----
	virtual void MovePosition(D3DXVECTOR3& movePosition);																				// 座標の移動
	void MoveSize(D3DXVECTOR3& moveSize);																						// 大きさの増減
	void MoveRotate(D3DXVECTOR3& moveRotate);																					// 回転率の増減
	void MoveScale(D3DXVECTOR3& moveScale);																						// 拡縮率の増減
	void MoveNormal(D3DXVECTOR3& moveNormal0, D3DXVECTOR3& moveNormal1, D3DXVECTOR3& moveNormal2, D3DXVECTOR3& moveNormal3);	// 法線の増減
	void MoveColor(int r, int g, int b, int a);																					// 色の増減
	void MoveUV_StartPoint(D3DXVECTOR2& moveUV_StartPoint);																		// UV開始点の増減
	void MoveUV_Size(D3DXVECTOR2& moveUV_Size);																					// UV大きさの増減


	//----- 設定処理 -----
	void SetPosition(D3DXVECTOR3& position);																				// 座標の設定
	void SetSize(D3DXVECTOR3& size);																						// 大きさの設定
	void SetRotate(D3DXVECTOR3& rotate);																					// 回転率の設定
	void SetScale(D3DXVECTOR3& scale);																						// 拡縮率の設定
	void SetNormal(D3DXVECTOR3& moveNormal0, D3DXVECTOR3& moveNormal1, D3DXVECTOR3& moveNormal2, D3DXVECTOR3& moveNormal3);	// 法線の設定
	void SetColor(unsigned int rgba);																						// 色の設定
	void SetUV_StartPoint(D3DXVECTOR2& UV_StartPoint);																		// UV開始点の設定
	void SetUV_Size(D3DXVECTOR2& UV_Size);																					// UV大きさの設定
	void SetTexture(LPDIRECT3DTEXTURE9 pTexture) { *m_pTexture = pTexture; }												// テクスチャの設定
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }																			// 描画処理の有無の設定
	void SetVecZ(D3DXVECTOR3& vecZ) { m_vecZ = vecZ; }

	// 怪しい
	void SetRotateToObj(D3DXVECTOR3& ObjectPostion);	// 対象の方向を向くように設定する
	void SetRadius(float radius) { m_radius = radius; }	// 半径の設定

	// 親のポインタ設定
	void SetParent(Part *pParent) {
		m_pParent = pParent;
		m_bUpdateWorldMatrix = true;
	}


	//----- データを返す処理 -----
	D3DXMATRIX&        GetWorldMatrix() { return m_WorldMatrix; }	// ワールドマトリクスを返す
	D3DXVECTOR3        GetSize() { return (m_halfSize * 2); }		// 大きさを返す
	D3DXVECTOR3&       GetHalfSize() { return m_halfSize; }			// 半分の大きさを返す
	D3DXVECTOR3&       GetRotate() { return m_rotate; }				// 回転率を返す
	D3DXVECTOR3&       GetScale() { return m_scale; }				// 拡縮率を返す
	float              GetRadius() { return m_radius; }				// 半径を返す
	D3DXVECTOR3&       GetVecZ() { return m_vecZ; }					// 向いてる方向を返す
	LPDIRECT3DTEXTURE9 GetTexture() { return *m_pTexture; }			// テクスチャを返す

	Part* GetParent() { return m_pParent; }

private:
	//----- 共用体の定義 -----
	union Color { // 色
		unsigned int color;
		unsigned char rgba[4];
	};


	//----- 構造体の定義 -----
	struct MATERIAL {	// マテリアル構造体
		Color		Ambient;
		Color		Diffuse;
		Color		Specular;
		Color		Emission;
		float		Shininess;
	};

	//----- マテリアル構造体 -----
	struct MODEL_MATERIAL {
		char Name[256];
		MATERIAL Material;
		unsigned int Texture;
	};

	//----- 描画サブセット構造体 -----
	struct SUBSET {
		unsigned int StartIndex;
		unsigned int IndexNum;
		MODEL_MATERIAL Material;
	};

	//----- モデル構造体 -----
	struct MODEL {
		VERTEX_3D    *VertexArray;
		unsigned int *IndexArray;
		unsigned int IndexNum;
		SUBSET       *Subset;
		unsigned int SubsetNum;
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
	D3DXVECTOR3 m_vecZ;			// 向いてる方向

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	LPDIRECT3DTEXTURE9*     m_pTexture;	// テクスチャ

	bool m_bUpdateVertexBuf;	// true で頂点バッファの更新をする
	bool m_bUpdateWorldMatrix;	// true でワールドマトリクスの更新をする
	bool m_bDraw;				// true で描画処理を行う

	Part *m_pParent;	// 親のポインタ

							
	//LPD3DXMESH   mesh;		// メッシュ情報
	//D3DMATERIAL9 *meshMat;	// マテリアル情報
	//DWORD        numMat;	// マテリアル数
	//D3DXVECTOR3  position;	// 座標
	//D3DXVECTOR3  rotation;	// 回転率
	//D3DXMATRIX   matrix;	// ワールドマトリクス
	//LPDIRECT3DTEXTURE9* texture;	// テクスチャ

};

#endif

