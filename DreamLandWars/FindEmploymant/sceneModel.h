//*****************************************************************************
//
//		モデル
//													Autohr : Yusuke Seki
//*****************************************************************************
#ifndef _SCENEMODEL_H_
#define _SCENEMODEL_H_

#include "main.h"
#include "scene.h"


class SceneModel : public Scene
{
public:
	// コンストラクタ / デストラクタ
	virtual ~SceneModel();

	// 基本的な関数
	static SceneModel* Create( D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, const char* modelPass );	// 実体の生成
	virtual HRESULT Init( D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, const char* modelPass );
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	// 増減
	void MoveQuaternion(float move);
	void MoveQuaternion(D3DXVECTOR3 vec, float move);
	void MoveRotate(D3DXVECTOR3 move) { m_rot += move; }
	void MoveScale(D3DXVECTOR3 move) { m_scl += move; }

	// 設定
	void SetQuaternion(float rot) { D3DXQuaternionRotationAxis(&m_quaternion, &D3DXVECTOR3(0.f, 1.f, 0.f), rot); }
	void SetQuaternion(D3DXVECTOR3 vec, float rot){ D3DXQuaternionRotationAxis(&m_quaternion, &vec, rot); }
	void SetRotate(D3DXVECTOR3 rotate) { m_rot = rotate; }
	void SetRotateToObj(D3DXVECTOR3 objPos);
	void SetScale(D3DXVECTOR3 scale) { m_scl = scale; }
	void SetColor(int a = 255, int r = 255, int g = 255, int b = 255);
	void SetRadius(float radius) { m_radius = radius; }

	// 取得
	D3DXMATRIX GetMatrixWorld() { return m_mtxWorld; }
	D3DXVECTOR3 GetRotate() { return m_rot; }
	D3DXVECTOR3 GetVecZ() { return m_vecZ; }
	float GetRadius() { return m_radius; }

protected:
	// コンストラクタ / デストラクタ
	SceneModel() {}
	SceneModel(int priority, TYPE type);


private:
	// 変数
	LPD3DXMESH			m_pMesh;		// メッシュインターフェースのポインタ
	LPD3DXBUFFER		m_pBufMat;		// マテリアル情報
	DWORD				m_numMat;		// マテリアル数
	D3DMATERIAL9*		m_pMeshMat;		// メッシュ情報
	D3DXMATRIX			m_mtxWorld;		// ワールド行列
	D3DXQUATERNION		m_quaternion;	// 回転率
	D3DXVECTOR3         m_rot;			// 回転率
	D3DXVECTOR3			m_scl;			// 拡縮率
	LPDIRECT3DTEXTURE9* m_pTexture;		// テクスチャ
	float               m_radius;

	D3DXVECTOR3 m_vecZ;

};

#endif