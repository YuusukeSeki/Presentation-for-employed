//*****************************************************************************
//
//		フィールド
//													Autohr : Yusuke Seki
//*****************************************************************************
#include "field.h"
#include "renderer.h"

//----- マクロ定義
//static const int   NUM_FIELD_HORIZON	= 4;												// 地面の数（横）
//static const int   NUM_FIELD_VERTICAL	= 4;												// 地面の数（縦）
//static const int   NUM_FIELD			= NUM_FIELD_HORIZON * NUM_FIELD_VERTICAL;			// 地面の数（全体）
//static const float FIELD_WIDTH			= ( float )FIELD_WIDTH_ALL / NUM_FIELD_HORIZON;		// １区画の幅
//static const float FIELD_HEIGHT			= ( float )FIELD_HEIGHT_ALL / NUM_FIELD_VERTICAL;	// １区画の高さ
//static const int   NUM_VERTEX			= (2 + 2 * NUM_FIELD_HORIZON) * NUM_FIELD_VERTICAL + (2 * (NUM_FIELD_VERTICAL - 1));	// 頂点の数
//static const int   NUM_POLYGON			= NUM_VERTEX - 2;									// ポリゴンの数

const float Field::FIELD_WIDTH_ALL  = 1000.0f;	// 全体フィールド幅
const float Field::FIELD_HEIGHT_ALL = 1000.0f;	// 全体フィールド高さ


// グローバル変数
float g_FieldHeight[5][5] = {	// フィールドのY座標
	//{ 0.0f,   0.0f,   0.0f,    0.0f, 0.0f },
	//{ 0.0f, 100.0f, 150.0f,  100.0f, 0.0f },
	//{ 0.0f, 200.0f, 300.0f,  200.0f, 0.0f },
	//{ 0.0f, 100.0f, 150.0f,  100.0f, 0.0f },
	//{ 0.0f,   0.0f,   0.0f,    0.0f, 0.0f },
	{ 0.0f,   0.0f,   0.0f,    0.0f, 0.0f },
	{ 0.0f,   0.0f,   0.0f,    0.0f, 0.0f },
	{ 0.0f,   0.0f,   0.0f,    0.0f, 0.0f },
	{ 0.0f,   0.0f,   0.0f,    0.0f, 0.0f },
	{ 0.0f,   0.0f,   0.0f,    0.0f, 0.0f },
};



//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
//Field::Field(int priority, TYPE type):Scene(priority, type)
Field::Field(Object::TYPE type) : Object(type)
{
	// メンバ変数初期化
	D3DXMatrixIdentity( &m_mtxWorld );	// 行列を単位行列にする（全値を１にする）
	m_rot = D3DXVECTOR3( 0, 0, 0);		// 回転率
	m_scl = D3DXVECTOR3( 0, 0, 0);		// 拡縮率
	m_pVtxBuf  = nullptr;				// 頂点バッファ
	m_pIdxBuf  = nullptr;				// インデックスバッファ
	m_pTexture = nullptr;				// テクスチャ
	m_mat.Diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 拡散反射
	m_mat.Ambient.r = m_mat.Diffuse.r * 0.4f;	        // 環境反射
	m_mat.Ambient.g = m_mat.Diffuse.g * 0.4f;	        // 〃
	m_mat.Ambient.b = m_mat.Diffuse.b * 0.4f;	        // 〃
	m_mat.Ambient.a = m_mat.Diffuse.a * 0.4f;	        // 〃
	m_mat.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.3f);	// 鏡面反射
	m_mat.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 自己発光
	m_mat.Power = 1.0f;									// 鏡面反射の鮮明度
	m_size = D3DXVECTOR3(0, 0, 0);
	m_numFieldHorizon = 0;
	m_numFieldVertical = 0;
}


//-----------------------------------------------------------------------------
//	デストラクタ
//-----------------------------------------------------------------------------
Field::~Field()
{
	Uninit();

}


//-----------------------------------------------------------------------------
//	生成
//-----------------------------------------------------------------------------
Field* Field::Create(D3DXVECTOR3 pos , D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXVECTOR3 scl, int numFieldHorizon, int numFieldVertical)
{
	//Field* pField = new Field(1, TYPE_3D_FIELD);
	Field* pField = new Field(Object::TYPE::TYPE_3D_FIELD);
	pField->Init( pos, size, rot, scl, numFieldHorizon, numFieldVertical);

	return pField;
}


//-----------------------------------------------------------------------------
//	初期化
//-----------------------------------------------------------------------------
HRESULT Field::Init( D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXVECTOR3 scl, int numFieldHorizon, int numFieldVertical)
{
	// メンバ変数設定
	//Scene::SetPos(pos);
	Object::SetPosition(pos);
	m_size = size;                         // 大きさ
	m_rot = rot;						   // 回転率
	m_scl = scl;						   // 拡縮率
	m_numFieldHorizon  = numFieldHorizon;  // 横のフィールド数
	m_numFieldVertical = numFieldVertical; // 縦のフィールド数

	m_halfSize = m_size * 0.5f;

	// ワールドマトリクスの生成
	D3DXMATRIX mtxTrans;
	D3DXMATRIX mtxRot;
	D3DXMATRIX mtxScall;
	
	// 各行列計算
	D3DXMatrixIdentity( &m_mtxWorld );	// 行列を単位行列にする（全値を１にする）
	D3DXMatrixScaling( &mtxScall, m_scl.x, m_scl.y, m_scl.z );					// 拡縮
	D3DXMatrixTranslation( &mtxTrans , pos.x , pos.y , pos.z );					// 移動
	D3DXMatrixRotationYawPitchRoll( &mtxRot , m_rot.y , m_rot.x , m_rot.z );	// 回転（y,x,zの順）

	// 行列の合成
	D3DXMatrixMultiply( &m_mtxWorld , &m_mtxWorld , &mtxScall );
	D3DXMatrixMultiply( &m_mtxWorld , &m_mtxWorld , &mtxRot );
	D3DXMatrixMultiply( &m_mtxWorld , &m_mtxWorld , &mtxTrans );

	// 頂点バッファの生成
	MakeVertexBuf();

	// インデックスバッファの生成
	MakeIdxBuf();

	return S_OK;
}


//-----------------------------------------------------------------------------
//	終了
//-----------------------------------------------------------------------------
void Field::Uninit(void)
{
	// 頂点バッファの破棄
	if( m_pVtxBuf != nullptr){
		m_pVtxBuf->Release();
		m_pVtxBuf = nullptr;
	}

	// インデックスバッファの開放
	if( m_pIdxBuf != nullptr){
		m_pIdxBuf->Release();
		m_pIdxBuf = nullptr;
	}

}


//-----------------------------------------------------------------------------
//	更新
//-----------------------------------------------------------------------------
void Field::Update(void)
{
}


//-----------------------------------------------------------------------------
//	描画
//-----------------------------------------------------------------------------
void Field::Draw(void)
{
	// 変数宣言
	int numVtx = (2 + 2 * m_numFieldHorizon) * m_numFieldVertical + (2 * (m_numFieldVertical - 1)); // 頂点数
	int numPolygon = numVtx - 2; // ポリゴン数

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// デバイスにワールド変換行列を設定
	pDevice->SetTransform( D3DTS_WORLD , &m_mtxWorld );

	// ストリームの設定
	pDevice->SetStreamSource( 0, m_pVtxBuf, 0, sizeof(VERTEX_3D));

	// デバイスへインデックスバッファを設定
	pDevice->SetIndices( m_pIdxBuf );

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_3D );

	// マテリアル情報設定
	D3DMATERIAL9 defaultMaterial;

	pDevice->GetMaterial(&defaultMaterial);

	pDevice->SetMaterial(&m_mat);

	pDevice->SetMaterial(&defaultMaterial);

	// テクスチャ貼り付け
	if (m_pTexture != nullptr)
	{
		pDevice->SetTexture(0, m_pTexture->GetTexture());
	}
	else
	{
		pDevice->SetTexture(0, nullptr);
	}

	// 描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, numVtx, 0, numPolygon);
}


//-----------------------------------------------------------------------------
// 頂点バッファ生成
//-----------------------------------------------------------------------------
void Field::MakeVertexBuf(void)
{
	// 変数宣言
	VERTEX_3D* pVtx;
	int indexNum = 0;
	float fieldWidth  = m_size.x / m_numFieldHorizon;  // フィールド１つ分の幅
	float fieldHeight = m_size.z / m_numFieldVertical; // フィールド１つ分の高さ
	int numVtx = (m_numFieldHorizon + 1) * (m_numFieldVertical + 1); // 頂点数
	int numPolygon = numVtx - 2; // ポリゴン数
	float leftEdgeVertex_X = -m_size.x * 0.5f;	// 左端頂点のX座標
	float topEdgeVertex_Z = m_size.z * 0.5f;	// 上端頂点のZ座標

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// 頂点バッファ領域の生成
	if (m_pVtxBuf == NULL) {
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * numVtx, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,
			D3DPOOL_MANAGED, &m_pVtxBuf, NULL))) {
			MessageBox(NULL, "頂点バッファ生成失敗", "Field::MakeVertexBuf", MB_OK);
		}
	}

	// VRAMの仮想アドレス取得
	m_pVtxBuf->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	for (int i = 0; i < m_numFieldVertical + 1; i++)
	{
		for (int j = 0; j < m_numFieldHorizon + 1; j++)
		{
			pVtx[indexNum].pos.x = leftEdgeVertex_X + fieldWidth  * j;
			//pVtx[indexNum].pos.y = g_FieldHeight[i][j];
			pVtx[indexNum].pos.y = 0;
			pVtx[indexNum].pos.z = topEdgeVertex_Z - fieldHeight * i;
			indexNum++;
		}
	}

	// 配列番号の初期化
	indexNum = 0;

	// テクスチャ座標の設定
	for (int i = 0; i < m_numFieldVertical + 1; i++)
	{
		for (int j = 0; j < m_numFieldHorizon + 1; j++)
		{
			pVtx[indexNum].tex = D3DXVECTOR2((float)j, (float)i);
			indexNum++;
		}
	}

	// 法線と頂点色の設定
	for (int i = 0; i < numVtx; i++)
	{
		// 法線の設定
		pVtx[i].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点色の設定
		pVtx[i].color = D3DCOLOR_RGBA(255, 255, 255, 255);
		//pVtx[i].color = D3DCOLOR_RGBA(255, 255, 255, 64);
	}

	// 法線の設定
	for(int z = 1; z < m_numFieldVertical - 1; z++)
	{
		for(int x = 1; x < m_numFieldHorizon - 1; x++)
		{
			// X方向の計算
			//int indexA = z * (m_numFieldVertical + 1) + x + 1;
			//int indexB = z * (m_numFieldVertical + 1) + x - 1;
			int indexA = z * (m_numFieldHorizon + 1) + x + 1;
			int indexB = z * (m_numFieldHorizon + 1) + x - 1;

			D3DXVECTOR3 vx = pVtx[indexA].pos - pVtx[indexB].pos;
			D3DXVECTOR3 nx(-vx.y, vx.x, 0.0f);

			// Z方向の計算
			indexA = z * (m_numFieldHorizon + 1) + x - z * (m_numFieldHorizon + 1);
			indexB = z * (m_numFieldHorizon + 1) + x + z * (m_numFieldHorizon + 1);

			if (indexB > numVtx)
			{
				indexB = numVtx;
			}

			D3DXVECTOR3 vz = pVtx[indexA].pos - pVtx[indexB].pos;
			D3DXVECTOR3 nz(0.0f, vz.z, -vz.y);

			// Y方向の算出
			D3DXVECTOR3 n = nx + nz;
			D3DXVec3Normalize(&n, &n);
			pVtx[z * 5 + x].normal = n;
		}
	}

	//// 法線の設定
	//D3DXVECTOR3 n(0,0,0);
	//int cnt = 0;

	//for (int z = 1; z < 4; z++) {
	//	for (int x = 1; x < 4; x++) {
	//		// X方向の計算
	//		D3DXVECTOR3 vx = pVtx[z * 5 + x + 1].pos - pVtx[z * 5 + x].pos;
	//		// Z方向の計算
	//		D3DXVECTOR3 vz = pVtx[z * 5 + x - z * 5].pos - pVtx[z * 5 + x].pos;
	//		// Y方向の算出
	//		D3DXVECTOR3 n1;
	//		D3DXVec3Cross(&n1, &vz, &vx);

	//		//// X方向の計算
	//		//D3DXVECTOR3 vx = pVtx[z * 5 + x + 1].pos - pVtx[z * 5 + x].pos;
	//		//// Z方向の計算
	//		//D3DXVECTOR3 vz = pVtx[z * 5 + x - z * 5].pos - pVtx[z * 5 + x].pos;
	//		//// Y方向の算出
	//		//D3DXVECTOR3 n1;
	//		//D3DXVec3Cross(&n1, &vz, &vx);
	//		// 計算結果を足す
	//		n += n1;
	//		cnt++;

	//		pVtx[z * 5 + x].normal = n;
	//	}
	//}

	// VRAMの仮想アドレス解放
	m_pVtxBuf->Unlock();
}


//-----------------------------------------------------------------------------
// インデックスバッファ生成
//-----------------------------------------------------------------------------
void Field::MakeIdxBuf(void)
{
	// 変数宣言
	WORD* pIdx;
	float fieldWidth       = m_size.x / m_numFieldHorizon;  // フィールド１つ分の幅
	float fieldHeight      = m_size.z / m_numFieldVertical; // フィールド１つ分の高さ
	int numVtx             = (2 + 2 * m_numFieldHorizon) * m_numFieldVertical + (2 * (m_numFieldVertical - 1)); // 頂点数
	int numPolygon         = numVtx - 2;                    // ポリゴン数
	int firstLineNumber    = m_numFieldHorizon * 2 + 3;	    // 2行目の一つ目の頂点番号
	int addFirstLineNumber = m_numFieldHorizon * 2 + 4;	    // 3行目以降で上記変数に足される数
	int lastLineNumber     = m_numFieldHorizon * 2 + 2;	    // 行毎のの最後の頂点番号
	int addLastLineNumber  = m_numFieldHorizon * 2 + 4;	    // 2行目以降で上記変数に足される数
	//float leftEdgeVertex_X = -m_size.x * 0.5f;	            // 左端頂点のX座標
	//float topEdgeVertex_Z  = m_size.z * 0.5f;	            // 上端頂点のZ座標
	int firstLineIndex     = m_numFieldHorizon + 1;	        // 折り返し時の頂点番号 

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// インデックスバッファ領域の生成
	if(m_pIdxBuf == NULL){
		if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * numVtx, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
			D3DPOOL_MANAGED, &m_pIdxBuf, NULL))) {
			MessageBox(NULL, "インデックスバッファの生成に失敗しました。", "Field::Init", MB_OK);
		}
	}

	// インデックスバッファ引き渡し開始
	m_pIdxBuf->Lock(0, 0, (void**)&pIdx, 0);

	// インデックス順序の設定
	{
		// 1行目の左端2頂点
		pIdx[0] = firstLineIndex;
		pIdx[1] = 0;

		// 縦
		for (int i = 0; i<m_numFieldVertical; i++)
		{
			// 1行目じゃなければ点を打つ
			if (i != 0)
			{
				// 2行目の場合
				if (i == 1)
				{
					pIdx[firstLineNumber] = firstLineIndex * 2;
					pIdx[firstLineNumber + 1] = firstLineIndex * 2;
					pIdx[firstLineNumber + 2] = firstLineIndex;
				}
				// 3行目以降
				else
				{
					pIdx[firstLineNumber + addFirstLineNumber * (i - 1)] = firstLineIndex * (i + 1);
					pIdx[firstLineNumber + addFirstLineNumber * (i - 1) + 1] = firstLineIndex * (i + 1);
					pIdx[firstLineNumber + addFirstLineNumber * (i - 1) + 2] = firstLineIndex * i;
				}
			}// End -- 1行目じゃない場合

			 // 横
			for (int j = 0; j<m_numFieldHorizon; j++)
			{
				// 1行目の場合
				if (i == 0)
				{
					pIdx[j * 2 + 2] = firstLineIndex + j + 1;
					pIdx[j * 2 + 3] = j + 1;
				}
				// 2行目の場合
				else if (i == 1)
				{
					pIdx[firstLineNumber + j * 2 + 3] = firstLineIndex * 2 + 1 + j;
					pIdx[firstLineNumber + j * 2 + 4] = firstLineIndex + 1 + j;
				}
				// 3行目以降
				else
				{
					pIdx[firstLineNumber + addFirstLineNumber * (i - 1) + j * 2 + 3] = firstLineIndex * (i + 1) + 1 + j;
					pIdx[firstLineNumber + addFirstLineNumber * (i - 1) + j * 2 + 4] = firstLineIndex * i + 1 + j;
				}
			}// End -- 横

			 // 最後の行じゃなければ点を打つ
			if (i != m_numFieldVertical - 1)
			{
				// 1行目の場合
				if (i == 0)
				{
					pIdx[lastLineNumber] = pIdx[lastLineNumber - 1];
				}
				// 2行目以降
				else
				{
					pIdx[lastLineNumber + addLastLineNumber * i] = pIdx[lastLineNumber + addLastLineNumber * i - 1];
				}
			}// End -- 最後の行じゃない場合
		}// End -- 縦
	}// End -- 頂点座標の設定

	 // インデックスバッファ引き渡し終了
	m_pIdxBuf->Unlock();
}


//-----------------------------------------------------------------------------
// フィールドとの当たり判定
// 返り値：true  当たっているフィールドの高さ
//         false -1.f
//-----------------------------------------------------------------------------
float Field::GetHeight(D3DXVECTOR3 pos)
{
	// 変数宣言
	D3DXVECTOR3 v01, v12, v20, v0p, v1p, v2p, n;
	float c0, c1, c2;
	int indexNum = 0;
	VERTEX_3D pVertex[256];
	float leftVertex_X = -m_size.x * 0.5f;	// 左端頂点のX座標
	float topVertex_Z  = m_size.z * 0.5f;	// 上端頂点のZ座標
	float fieldWidth = m_size.x / m_numFieldHorizon;  // フィールド１つ分の幅
	float fieldHeight = m_size.z / m_numFieldVertical; // フィールド１つ分の高さ

	// フィールドの頂点座標を取得
	for (int i = 0; i <= m_numFieldVertical; i++)
	{
		for (int j = 0; j <= m_numFieldHorizon; j++)
		{
			pVertex[indexNum].pos.x = leftVertex_X + fieldWidth * j;
			//pVertex[indexNum].pos.y = g_FieldHeight[i][j];
			pVertex[indexNum].pos.y = 0;
			pVertex[indexNum].pos.z = topVertex_Z - fieldHeight * i;
			indexNum++;
		}
	}

	// 当たり判定
	for (int z = 0; z < m_numFieldVertical + 1; z++) {
		for (int x = 0; x < m_numFieldHorizon + 1; x++) {
			// 左側の三角形
			v01 = pVertex[z * (m_numFieldHorizon + 1) + x].pos - pVertex[(z + 1) * (m_numFieldHorizon + 1) + x].pos;
			v12 = pVertex[z * (m_numFieldHorizon + 1) + (x + 1)].pos - pVertex[z * (m_numFieldHorizon + 1) + x].pos;
			v20 = pVertex[(z + 1) * (m_numFieldHorizon + 1) + x].pos - pVertex[z * (m_numFieldHorizon + 1) + (x + 1)].pos;
			v0p = pos - pVertex[(z + 1) * (m_numFieldHorizon + 1) + x].pos;
			v1p = pos - pVertex[z * (m_numFieldHorizon + 1) + x].pos;
			v2p = pos - pVertex[z * (m_numFieldHorizon + 1) + (x + 1)].pos;
			c0 = v01.x * v0p.z - v01.z * v0p.x;
			c1 = v12.x * v1p.z - v12.z * v1p.x;
			c2 = v20.x * v2p.z - v20.z * v2p.x;

			// 当たっていたら高さを求める
			if (c0 <= 0.f && c1 <= 0.f && c2 <= 0.f) {
				D3DXVec3Cross(&n, &v01, &v12);
				pos.y = pVertex[(z + 1) * (m_numFieldHorizon + 1) + x].pos.y
					  - (n.x * (pos.x - pVertex[(z + 1) * (m_numFieldHorizon + 1) + x].pos.x)
					  +  n.z * (pos.z - pVertex[(z + 1) * (m_numFieldHorizon + 1) + x].pos.z)) / n.y;
				return pos.y;
			}// 高さを求める

			 // 右側の三角形
			v01 = pVertex[z * (m_numFieldHorizon + 1) + (x + 1)].pos - pVertex[(z + 1) * (m_numFieldHorizon + 1) + x].pos;
			v12 = pVertex[(z + 1) * (m_numFieldHorizon + 1) + (x + 1)].pos - pVertex[z * (m_numFieldHorizon + 1) + (x + 1)].pos;
			v20 = pVertex[(z + 1) * (m_numFieldHorizon + 1) + x].pos - pVertex[(z + 1) * (m_numFieldHorizon + 1) + (x + 1)].pos;
			v0p = pos - pVertex[(z + 1) * (m_numFieldHorizon + 1) + x].pos;
			v1p = pos - pVertex[z * (m_numFieldHorizon + 1) + (x + 1)].pos;
			v2p = pos - pVertex[(z + 1) * (m_numFieldHorizon + 1) + (x + 1)].pos;
			c0 = v01.x * v0p.z - v01.z * v0p.x;
			c1 = v12.x * v1p.z - v12.z * v1p.x;
			c2 = v20.x * v2p.z - v20.z * v2p.x;

			// 当たっていたら高さを求める
			if (c0 <= 0.f && c1 <= 0.f && c2 <= 0.f) {
				D3DXVec3Cross(&n, &v01, &v12);
				pos.y = pVertex[(z + 1) * (m_numFieldHorizon + 1) + x].pos.y
					  - (n.x * (pos.x - pVertex[(z + 1) * (m_numFieldHorizon + 1) + x].pos.x)
					  +  n.z * (pos.z - pVertex[(z + 1) * (m_numFieldHorizon + 1) + x].pos.z)) / n.y;
				return pos.y;
			}// 高さを求める
		}// x
	}// z

	// 当たってない場合、負の数を返す
	return -1.f;
}

