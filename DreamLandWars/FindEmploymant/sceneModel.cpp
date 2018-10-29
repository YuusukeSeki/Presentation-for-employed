//*****************************************************************************
//
//		モデル
//													Autohr : Yusuke Seki
//*****************************************************************************
#include "sceneModel.h"
#include "renderer.h"



//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
SceneModel::SceneModel(int priority, TYPE type) : Scene(priority, type)
{
	// メンバ変数初期化
	D3DXMatrixIdentity( &m_mtxWorld );
	D3DXQuaternionIsIdentity(&m_quaternion);
	m_rot = m_scl = D3DXVECTOR3( 0, 0, 0 );
	m_pMesh		= nullptr;
	m_pBufMat	= nullptr;
	m_numMat	= 0;
	m_pMeshMat	= nullptr;
	m_pTexture	= nullptr;

	m_vecZ = D3DXVECTOR3(0.f, 0.f, 1.f);

}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
SceneModel::~SceneModel()
{
	// テクスチャの開放
	for( int i = 0; i < (int)m_numMat; ++i ){
		if( m_pTexture[i] != nullptr )
		{
			m_pTexture[i]->Release();
			m_pTexture[i] = nullptr;
		}
	}

	// メッシュの開放
	if( m_pMeshMat != nullptr ){
		delete [] m_pMeshMat;
		m_pMeshMat = nullptr;
	}

	// マテリアルの開放
	if( m_pBufMat != nullptr ){
		m_pBufMat->Release();
		m_pBufMat = nullptr;
	}

	// メッシュインターフェース情報の開放
	if( m_pMesh != nullptr ){
		m_pMesh->Release();
		m_pMesh = nullptr;
	}
}


//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
SceneModel* SceneModel::Create( D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, const char* modelPass )
{
	SceneModel* pSceneModel = new SceneModel(0, TYPE_MODEL);
	pSceneModel->Init( pos, rot, scl, modelPass );

	return pSceneModel;
}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
HRESULT SceneModel::Init( D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, const char* modelPass )
{
	Scene::SetPos(pos);
	D3DXQuaternionIdentity(&m_quaternion);
	m_rot = rot;
	m_scl = scl;

	
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// Xファイルの読み込み
	if(FAILED(D3DXLoadMeshFromXA(modelPass, D3DXMESH_MANAGED, pDevice, nullptr, &m_pBufMat, nullptr, &m_numMat, &m_pMesh)))
	{
		MessageBox( nullptr , modelPass , "Xfile OpenError!" , MB_OK );
		return E_FAIL;
	}

	// Xファイルに法線がない場合は、法線を書き込む
	if ( !( m_pMesh->GetFVF() & D3DFVF_NORMAL ) )
	{
		ID3DXMesh* pTempMesh = nullptr;
		
		m_pMesh->CloneMeshFVF( m_pMesh->GetOptions(), m_pMesh->GetFVF() | D3DFVF_NORMAL, pDevice, &pTempMesh );
	
		D3DXComputeNormals(pTempMesh, nullptr);
		m_pMesh->Release();
		m_pMesh = pTempMesh;
	}
	
	//// 半径の算出
	//LPDIRECT3DVERTEXBUFFER9 pVtxBuf;
	//float max = 0.f;
	//float min = 0.f;
	//VERTEX_3D* pVtx;
	//int numVtx = m_pMesh->GetNumVertices();
	//m_pMesh->GetVertexBuffer(&pVtxBuf);

	////pVtxBuf->Lock(0, 0, (void**)pVtx, 0);

	////for (int i = 0; i < numVtx; i++) {
	////	D3DXVECTOR3 pos = pVtx[i].pos;
	////	if (min > pVtx->pos.x) min = pVtx[i].pos.x;
	////	if (max < pVtx->pos.x) max = pVtx[i].pos.x;
	////}

	////pVtxBuf->Unlock();

	////m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)pVtx);

	////for (int i = 0; i < numVtx; i++) {
	////	D3DXVECTOR3 pos = pVtx[i].pos;
	////	if (min > pVtx->pos.x) min = pVtx[i].pos.x;
	////	if (max < pVtx->pos.x) max = pVtx[i].pos.x;
	////}

	////m_pMesh->UnlockVertexBuffer();

	//m_radius = (max - min) / 2;


	// マテリアル情報の取得
	D3DXMATERIAL* pMat = ( D3DXMATERIAL* )m_pBufMat->GetBufferPointer();
	m_pMeshMat = new D3DMATERIAL9[ m_numMat ];
	m_pTexture = new LPDIRECT3DTEXTURE9[ m_numMat ];


	for( int i=0; i<(int)m_numMat; i++ )
	{
		m_pMeshMat[i] = pMat[i].MatD3D;
		m_pTexture[i] = nullptr;

		if (pMat[i].pTextureFilename == nullptr) continue;

		if( FAILED( D3DXCreateTextureFromFile( pDevice, pMat[i].pTextureFilename, &m_pTexture[i] )))
		{
			MessageBox( nullptr, pMat[i].pTextureFilename, "テクスチャの読み込みに失敗しました" , MB_OK );
		}
	}

	return S_OK;
}


//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void SceneModel::Uninit(void)
{
	// テクスチャの開放
	for( int i = 0; i < (int)m_numMat; ++i ){
		if( m_pTexture[i] != nullptr )
		{
			m_pTexture[i]->Release();
			m_pTexture[i] = nullptr;
		}
	}

	// メッシュの開放
	if( m_pMeshMat != nullptr ){
		delete [] m_pMeshMat;
		m_pMeshMat = nullptr;
	}

	// マテリアルの開放
	if( m_pBufMat != nullptr ){
		m_pBufMat->Release();
		m_pBufMat = nullptr;
	}

	// メッシュインターフェース情報の開放
	if( m_pMesh != nullptr ){
		m_pMesh->Release();
		m_pMesh = nullptr;
	}
}


//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void SceneModel::Update(void)
{
}


//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void SceneModel::Draw(void)
{
	// ワールドマトリクスの生成
	D3DXMATRIX mtxTrans, mtxRot, mtxScl;
	D3DXVECTOR3 pos = GetPos();

	// 行列の計算
	D3DXMatrixScaling(&mtxScl, m_scl.x, m_scl.y, m_scl.z);
	//D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);

	// 行列の合成
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// デバイスにワールド変換行列を設定
	Renderer::GetDevice()->SetTransform( D3DTS_WORLD , &m_mtxWorld );

	// 現在デバイスに設定されているマテリアル情報の取得
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial( &matDef );

	// マテリアル情報のポインタとしてバッファアドレスを取得
	for( int i=0; i<(int)m_numMat; i++ )
	{
		// デバイスにマテリアルを設定
		pDevice->SetMaterial( &m_pMeshMat[i] );

		// テクスチャの貼り付け
		pDevice->SetTexture( 0 , m_pTexture[i] );

		// 描画
		m_pMesh->DrawSubset( i );
	}

	// マテリアル情報を初期状態へ戻す
	pDevice->SetMaterial( &matDef );

}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void SceneModel::MoveQuaternion(float move)
{
	D3DXQUATERNION quaternion;

	D3DXQuaternionRotationAxis(&quaternion, &D3DXVECTOR3(0.f, 1.f, 0.f), move);
	D3DXQuaternionMultiply(&m_quaternion, &quaternion, &m_quaternion);

}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void SceneModel::MoveQuaternion(D3DXVECTOR3 vec, float move)
{
	D3DXQUATERNION quaternion;

	D3DXQuaternionRotationAxis(&quaternion, &vec, move);
	D3DXQuaternionMultiply(&m_quaternion, &quaternion, &m_quaternion);

}


//-----------------------------------------------------------------------------
// 色の設定
//-----------------------------------------------------------------------------
void SceneModel::SetColor(int a, int r, int g, int b)
{
	VERTEX_3D* pVtx;
	int numVtx = (int)m_pMesh->GetNumVertices();

	m_pMesh->LockVertexBuffer(0, (void**)&pVtx);

	for (int i = 0; i < numVtx; i++) {
		pVtx[i].color = D3DCOLOR_RGBA(r, g, b, a);
	}

	m_pMesh->UnlockVertexBuffer();

}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void SceneModel::SetRotateToObj(D3DXVECTOR3 objPos)
{
	float angle;

	objPos -= GetPos();
	objPos.y = 0.f;
	D3DXVec3Normalize(&objPos, &objPos);
	m_vecZ = objPos;
	angle = atan2f(objPos.x, objPos.z);
	SetRotate(D3DXVECTOR3(0, angle, 0));

}


