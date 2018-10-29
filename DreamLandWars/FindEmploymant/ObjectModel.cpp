//*****************************************************************************
//
//		モデル（リスト構造）
//													Autohr : Yusuke Seki
//*****************************************************************************
#include "ObjectModel.h"
#include "renderer.h"



//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
ObjectModel::ObjectModel() : Object(Object::TYPE::TYPE_MODEL)
{
	// メンバ変数初期化
	m_pMesh = nullptr;
	m_pMeshMat = nullptr;
	m_numMat = 0;
	ZeroMemory(&m_WorldMatrix, sizeof(m_WorldMatrix));
	m_halfSize = D3DXVECTOR3(0, 0, 0);
	m_rotate = D3DXVECTOR3(0, 0, 0);
	m_scale = D3DXVECTOR3(0, 0, 0);
	m_radius = 0.f;
	m_color.color = 0xffffffff;
	m_front = D3DXVECTOR3(0, 0, 1);
	m_pVtxBuff = nullptr;
	m_pTexture = nullptr;
	m_bUpdateVertexBuf = false;
	m_bUpdateWorldMatrix = false;
	m_bDraw = true;

}


//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
ObjectModel::ObjectModel(Object::TYPE type) : Object(type)
{
	// メンバ変数初期化
	m_pMesh              = nullptr;
	m_pMeshMat           = nullptr;
	m_numMat             = 0;
	ZeroMemory(&m_WorldMatrix, sizeof(m_WorldMatrix));
	m_halfSize           = D3DXVECTOR3(0, 0, 0);
	m_rotate             = D3DXVECTOR3(0, 0, 0);
	m_scale              = D3DXVECTOR3(0, 0, 0);
	m_radius             = 0.f;
	m_color.color        = 0xffffffff;
	m_front               = D3DXVECTOR3(0, 0, 1);
	m_pVtxBuff           = nullptr;
	m_pTexture           = nullptr;
	m_bUpdateVertexBuf   = false;
	m_bUpdateWorldMatrix = false;
	m_bDraw              = true;

}


//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
ObjectModel::~ObjectModel()
{
	Uninit();

}


//-----------------------------------------------------------------------------
// 実体の生成
//-----------------------------------------------------------------------------
ObjectModel* ObjectModel::Create(D3DXVECTOR3& position, const char* FileName)
{
	ObjectModel* pObjectModel = new ObjectModel(Object::TYPE::TYPE_MODEL);
	pObjectModel->Init(position, FileName);

	return pObjectModel;
}


//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void ObjectModel::Init(D3DXVECTOR3& position, const char* FileName)
{
	// データの設定
	Object::SetPosition(position);					// 座標
	m_rotate             = D3DXVECTOR3(0, 0, 0);	// 回転率
	m_scale              = D3DXVECTOR3(1, 1, 1);	// 拡縮率
	m_front               = D3DXVECTOR3(0, 0, 1);	// 向いてる方向
	m_pTexture           = nullptr;					// テクスチャ
	m_bUpdateWorldMatrix = true;					// true でワールドマトリクスの更新をする
	m_bDraw              = true;					// true で描画処理を行う

	// ※※ 以下現在適当に設定せざるを得ない状態 ※※
	m_color.color        = 0xffffffff;				// 色
	m_pVtxBuff           = nullptr;					// 頂点バッファ
	m_bUpdateVertexBuf   = false;					// true で頂点バッファの更新をする


	// モデルの読み込み
	LoadMeshModel_DX(FileName);

	// 大きさの取得
	LoadModelSizeFromX(FileName);

}


//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void ObjectModel::Uninit(void)
{
	// テクスチャの開放
	for (int i = 0; i < (int)m_numMat; ++i) {
		if (m_pTexture[i] != nullptr)
		{
			m_pTexture[i]->Release();
			m_pTexture[i] = nullptr;
		}
	}

	// メッシュの開放
	if (m_pMeshMat != nullptr) {
		delete[] m_pMeshMat;
		m_pMeshMat = nullptr;
	}

	// メッシュインターフェース情報の開放
	if (m_pMesh != nullptr) {
		m_pMesh->Release();
		m_pMesh = nullptr;
	}
}


//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void ObjectModel::Update(void)
{
}


//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void ObjectModel::Draw(void)
{
	// false で描画処理を行わない
	if (!m_bDraw) return;

	// true で頂点バッファの更新処理をする
	if (m_bUpdateVertexBuf) this->UpdateVertexBuf();

	// true でワールドマトリクスの更新処理をする
	if (m_bUpdateWorldMatrix) this->UpdateWorldMatrix();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// デバイスにワールド変換行列を設定
	pDevice->SetTransform(D3DTS_WORLD, &m_WorldMatrix);

	// 現在デバイスに設定されているマテリアル情報の取得
	D3DMATERIAL9 matDef;
	pDevice->GetMaterial(&matDef);

	// マテリアル情報のポインタとしてバッファアドレスを取得
	for (int i = 0; i<(int)m_numMat; i++)
	{
		// デバイスにマテリアルを設定
		pDevice->SetMaterial(&m_pMeshMat[i]);

		// テクスチャの貼り付け
		pDevice->SetTexture(0, m_pTexture[i]);

		// 描画
		m_pMesh->DrawSubset(i);
	}

	// マテリアル情報を初期状態へ戻す
	pDevice->SetMaterial(&matDef);

}



//=============================================================================
//	増減処理
// 座標の移動
void ObjectModel::MovePosition(D3DXVECTOR3& movePosition)
{
	// 座標の移動
	Object::MovePosition(movePosition);

	// ワールドマトリクスの更新フラグON
	m_bUpdateWorldMatrix = true;
}

// 大きさの増減
void ObjectModel::MoveSize(D3DXVECTOR3& moveSize)
{
	// 大きさの増減
	m_halfSize += moveSize * 0.5f;
	m_radius    = sqrtf(m_halfSize.x * m_halfSize.x + m_halfSize.y * m_halfSize.y);	// 半径
	
	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// 回転率の増減
void ObjectModel::MoveRotate(D3DXVECTOR3& moveRotate)
{
	// 回転率の増減
	m_rotate += moveRotate;
	
	// ワールドマトリクスの更新フラグON
	m_bUpdateWorldMatrix = true;
}

// 拡縮率の増減
void ObjectModel::MoveScale(D3DXVECTOR3& moveScale)
{
	// 拡縮率の増減
	m_scale += moveScale;

	// ワールドマトリクスの更新フラグON
	m_bUpdateWorldMatrix = true;
}

// 色の増減
void ObjectModel::MoveColor(int r, int g, int b, int a)
{
	// 色の増減
	m_color.rgba[3] = m_color.rgba[3] + r >= 255 ? 255 : m_color.rgba[3] + r;
	m_color.rgba[2] = m_color.rgba[2] + g >= 255 ? 255 : m_color.rgba[2] + g;
	m_color.rgba[1] = m_color.rgba[1] + b >= 255 ? 255 : m_color.rgba[1] + b;
	m_color.rgba[0] = m_color.rgba[0] + a >= 255 ? 255 : m_color.rgba[0] + a;
	m_color.rgba[3] = m_color.rgba[3] + r <=   0 ?   0 : m_color.rgba[3] + r;
	m_color.rgba[2] = m_color.rgba[2] + g <=   0 ?   0 : m_color.rgba[2] + g;
	m_color.rgba[1] = m_color.rgba[1] + b <=   0 ?   0 : m_color.rgba[1] + b;
	m_color.rgba[0] = m_color.rgba[0] + a <=   0 ?   0 : m_color.rgba[0] + a;

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}



//=============================================================================
// 設定処理
// 座標の設定
void ObjectModel::SetPosition(D3DXVECTOR3& position)
{
	// 座標の移動
	Object::SetPosition(position);

	// ワールドマトリクスの更新フラグON
	m_bUpdateWorldMatrix = true;
}

// 大きさの設定
void ObjectModel::SetSize(D3DXVECTOR3& size)
{
	// 大きさの設定
	m_halfSize = size * 0.5f;
	m_radius   = sqrtf(m_halfSize.x * m_halfSize.x + m_halfSize.y * m_halfSize.y);	// 半径

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// 回転率の設定
void ObjectModel::SetRotate(D3DXVECTOR3& rotate)
{
	// 回転率の増減
	m_rotate = rotate;

	// ワールドマトリクスの更新フラグON
	m_bUpdateWorldMatrix = true;
}

// 拡縮率の設定
void ObjectModel::SetScale(D3DXVECTOR3& scale)
{
	// 拡縮率の設定
	m_scale = scale;

	// ワールドマトリクスの更新フラグON
	m_bUpdateWorldMatrix = true;
}

// 色の設定
void ObjectModel::SetColor(unsigned int rgba)
{
	// 色設定
	m_color.color = rgba;

	// 頂点バッファの更新フラグON
	m_bUpdateVertexBuf = true;

}

// 対象の方向を向くように設定する
void ObjectModel::SetRotateToObj(D3DXVECTOR3& ObjectPostion)
{
	D3DXVECTOR3 targetPosition = ObjectPostion;

	// 自分から対象へのベクトルを取る
	targetPosition -= Object::GetPosition();

	// 今回は2DゲームなのでY座標は０で統一
	targetPosition.y = 0.f;

	// 力が要らないので単位ベクトル化
	D3DXVec3Normalize(&targetPosition, &targetPosition);

	// 向きを設定
	m_front = targetPosition;

	// Y軸の傾きを算出して設定
	float angle = atan2f(targetPosition.x, targetPosition.z);
	SetRotate(D3DXVECTOR3(0, angle, 0));

}



//=============================================================================
//	private関数
// 頂点バッファの生成　※未完成
void ObjectModel::MakeVertexBuf()
{

}

// 頂点バッファの更新　※未完成
void ObjectModel::UpdateVertexBuf()
{
	// 頂点数取得
	int numVtx = (int)m_pMesh->GetNumVertices();

	// VRAMの仮想アドレス取得
	VERTEX_3D* pVtx;
	m_pMesh->LockVertexBuffer(0, (void**)&pVtx);

	// 頂点座標の設定


	// 法線の設定


	// 頂点色の設定
	for (int i = 0; i < numVtx; i++) {
		pVtx[i].color = D3DCOLOR_RGBA(m_color.rgba[3], m_color.rgba[2], m_color.rgba[1], m_color.rgba[0]);
	}

	// テクスチャUV値の設定


	// VRAMの仮想アドレス解放
	m_pMesh->UnlockVertexBuffer();

	// 頂点バッファの更新フラグOFF
	m_bUpdateVertexBuf = false;

}

// ワールドマトリクスの更新
void ObjectModel::UpdateWorldMatrix()
{
	// 移動、回転、拡縮行列の計算
	D3DXMATRIX mtxTranslate, mtxRotate, mtxScale;
	D3DXVECTOR3 position = Object::GetPosition();
	D3DXMatrixTranslation(&mtxTranslate, position.x, position.y, position.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRotate, m_rotate.y, m_rotate.x, m_rotate.z);
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);

	// ３行列の合成
	D3DXMatrixIdentity(&m_WorldMatrix);
	D3DXMatrixMultiply(&m_WorldMatrix, &m_WorldMatrix, &mtxScale);
	D3DXMatrixMultiply(&m_WorldMatrix, &m_WorldMatrix, &mtxRotate);
	D3DXMatrixMultiply(&m_WorldMatrix, &m_WorldMatrix, &mtxTranslate);

	// ワールドマトリクスの更新フラグOFF
	m_bUpdateWorldMatrix = false;

}

// モデルデータの読み込み　※未完成
void ObjectModel::LoadModel(const char* FileName)
{
	//D3DXVECTOR3* positionArray;
	//D3DXVECTOR2* texcoordArray;
	//D3DXVECTOR3* normalArray;
	//unsigned int positionNum = 0;
	//unsigned int texcoordlNum = 0;
	//unsigned int normalNum = 0;
	//unsigned int vertexNum = 0;
	//unsigned int indexNum = 0;
	//unsigned int in = 0;
	//unsigned int subsetNum = 0;
	//char str[256];
	//char* s;
	//char c;
	//FILE* file;
	//file = fopen(modelPass, "rt");

	//// ファイルが無ければ終了
	//if (file == nullptr) {
	//	_MSGERROR("Failed Load Model!!", modelPass);
	//	return;
	//}

	//// モデルの実体生成
	//m_pModel = new MODEL;


	//// 要素数カウント
	//while (true) {
	//	fscanf(file, "%s", str);

	//	// 行がなければ要素数のカウントを終了する
	//	if (feof(file) != 0) break;

	//	// 頂点座標データ？
	//	if (strcmp(str, "v") == 0) positionNum++;
	//	// 法線データ？
	//	else if (strcmp(str, "vn") == 0) normalNum++;
	//	// SV値データ？
	//	else if (strcmp(str, "vt") == 0) texcoordlNum++;
	//	// 描画サブセットデータ？
	//	else if (strcmp(str, "usemtl") == 0) subsetNum++;
	//	// 頂点指定データ？
	//	else if (strcmp(str, "f") == 0) {
	//		in = 0;

	//		do {
	//			fscanf(file, "%s", str);

	//			// 頂点数増加
	//			vertexNum++;
	//			in++;

	//			// 一文字取得
	//			c = fgetc(file);

	//		} while (c != '\n' && c != '\r'); // 改行でループ終了

	//										  // 四角は三角に分割
	//		if (in == 4) in = 6;

	//		// インデックス数増加
	//		indexNum += in;

	//	}
	//}

	//// メモリ確保
	//positionArray = new VECTOR3D[positionNum];
	//normalArray = new VECTOR3D[normalNum];
	//texcoordArray = new VECTOR2D[texcoordlNum];
	//m_pModel->VertexArray = new VERTEX_3D[vertexNum];
	//m_pModel->IndexArray = new unsigned int[indexNum];
	//m_pModel->IndexNum = indexNum;
	//m_pModel->Subset = new SUBSET[subsetNum];
	//m_pModel->SubsetNum = subsetNum;

	//// 要素読込
	//VECTOR3D* position = positionArray;
	//VECTOR3D* normal = normalArray;
	//VECTOR2D* texcoord = texcoordArray;
	//unsigned int vc = 0;
	//unsigned int ic = 0;
	//unsigned int sc = 0;
	//fseek(file, 0, SEEK_SET);
	//while (true) {
	//	// 一行取得
	//	fscanf(file, "%s", str);

	//	// 終了
	//	if (feof(file) != 0) break;

	//	// 
	//	if (strcmp(str, "mtllib") == 0) {
	//		// マテリアルファイル
	//		fscanf(file, "%s", str);

	//		char path[256];
	//		strcpy(path, "data/model/");
	//		strcat(path, str);

	//		LoadMaterial(path);
	//	}
	//	// 
	//	else if (strcmp(str, "o") == 0) {
	//		// オブジェクト名
	//		fscanf(file, "%s", str);
	//	}
	//	// 座標データの入った頂点配列を形成
	//	else if (strcmp(str, "v") == 0) {
	//		// 頂点座標
	//		fscanf(file, "%f", &position->x);
	//		fscanf(file, "%f", &position->y);
	//		fscanf(file, "%f", &position->z);
	//		position->x *= SCALE_MODEL;
	//		position->y *= SCALE_MODEL;
	//		position->z *= SCALE_MODEL;
	//		position++;

	//	}
	//	// 法線データ
	//	else if (strcmp(str, "vn") == 0) {
	//		// 法線
	//		fscanf(file, "%f", &normal->x);
	//		fscanf(file, "%f", &normal->y);
	//		fscanf(file, "%f", &normal->z);
	//		normal++;

	//	}
	//	// テクスチャデータ
	//	else if (strcmp(str, "vt") == 0) {
	//		// SV値
	//		fscanf(file, "%f", &texcoord->x);
	//		fscanf(file, "%f", &texcoord->y);
	//		texcoord++;

	//	}
	//	// 
	//	else if (strcmp(str, "usemtl") == 0) {
	//		// マテリアル
	//		fscanf(file, "%s", str);

	//		if (sc != 0) m_pModel->Subset[sc - 1].IndexNum = ic - m_pModel->Subset[sc - 1].StartIndex;
	//		m_pModel->Subset[sc].StartIndex = ic;

	//		for (unsigned int i = 0; i < m_MaterialNum; i++) {
	//			if (strcmp(str, m_pMaterialArray[i].Name) == 0) {
	//				m_pModel->Subset[sc].Material.Material = m_pMaterialArray[i].Material;
	//				strcpy(m_pModel->Subset[sc].Material.Name, m_pMaterialArray[i].Name);

	//				break;
	//			}
	//		}

	//		sc++;

	//	}
	//	// 頂点番号が入ったインデックス配列の形成
	//	else if (strcmp(str, "f") == 0) {
	//		// 面
	//		in = 0;

	//		do {
	//			fscanf(file, "%s", str);

	//			s = strtok(str, "/");
	//			m_pModel->VertexArray[vc].Position = positionArray[atoi(s) - 1];

	//			if (s[strlen(s) + 1] != '/')
	//			{
	//				s = strtok(NULL, "/");
	//				m_pModel->VertexArray[vc].TexturePos = texcoordArray[atoi(s) - 1];
	//			}

	//			s = strtok(NULL, "/");
	//			m_pModel->VertexArray[vc].Normal = normalArray[atoi(s) - 1];

	//			m_pModel->IndexArray[ic] = vc;
	//			ic++;
	//			vc++;

	//			in++;
	//			c = fgetc(file);

	//		} while (c != '\n' && c != '\r');

	//		// 四角は三角に分割
	//		if (in == 4) {
	//			m_pModel->IndexArray[ic] = vc - 4;
	//			ic++;
	//			m_pModel->IndexArray[ic] = vc - 2;
	//			ic++;

	//		}
	//	}
	//}

	//if (sc != 0) m_pModel->Subset[sc - 1].IndexNum = ic - m_pModel->Subset[sc - 1].StartIndex;

	//if (m_pMaterialArray) {
	//	delete[] m_pMaterialArray;
	//	m_pMaterialArray = NULL;
	//}

	//m_MaterialNum = 0;

}

// Xファイルからモデルデータのサイズを読み込む
void ObjectModel::LoadModelSizeFromX(const char* FileName)
{
	// 変数の定義
	unsigned int positionNum = 0;
	D3DXVECTOR3 positionMax(0, 0, 0);
	D3DXVECTOR3 positionMin(0, 0, 0);
	char str[256];
	FILE* file;
	int cntHeader = 0;
	char* s;

	// ファイルを開く
	file = fopen(FileName, "rt");

	// ファイルが無ければ終了
	if (file == nullptr) {
		_MSGERROR("Failed Load Model Size From X File!!", FileName);
		return;
	}

	// 最大値と最小値の取得
	while (true) {
		fscanf(file, "%s", str);

		// 行がなければ要素数のカウントを終了する
		if (feof(file) != 0) break;

		// 頂点データの探知
		if (strcmp(str, "Header{") == 0) {
			cntHeader++;
		}

		// 頂点データの探知
		if (strcmp(str, "Mesh") == 0 && cntHeader == 1) {
			// 読み飛ばす
			fscanf(file, "%s", str);

			// 要素数の取得
			fscanf(file, "%d", &positionNum);

			// 読み飛ばす
			fscanf(file, "%s", str);

			// 頂点データの読込
			for (unsigned int i = 0; i < positionNum; i++) {
				fscanf(file, "%s", str);

				D3DXVECTOR3 buf;
				s = strtok(str, ";");
				buf.x = (float)atof(s);
				s = strtok(NULL, ";");
				buf.y = (float)atof(s);
				s = strtok(NULL, ";");
				buf.z = (float)atof(s);

				// 最大値の更新
				if (positionMax.x < buf.x) positionMax.x = buf.x;
				if (positionMax.y < buf.y) positionMax.y = buf.y;
				if (positionMax.z < buf.z) positionMax.z = buf.z;

				// 最小値の更新
				if (positionMin.x > buf.x) positionMin.x = buf.x;
				if (positionMin.y > buf.y) positionMin.y = buf.y;
				if (positionMin.z > buf.z) positionMin.z = buf.z;

			}

			break;

		}
	}

	// ファイルを閉じる
	fclose(file);

	// 大きさを取得
	m_halfSize = (positionMax - positionMin) * 0.5f;																// 半分の大きさ
	//m_radius   = sqrtf(m_halfSize.x * m_halfSize.x + m_halfSize.z * m_halfSize.z);									// 半径
	m_radius   = sqrtf(m_halfSize.x * m_halfSize.x + m_halfSize.y * m_halfSize.y + m_halfSize.z * m_halfSize.z);	// 半径
	//m_radius = m_halfSize.x;

}

// マテリアル情報の読み込み　※未完成
void ObjectModel::LoadMaterial()
{

}

// モデルデータの読み込み（directXの便利関数使用）
void ObjectModel::LoadMeshModel_DX(const char* modelPass)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	// Xファイルの読み込み
	LPD3DXBUFFER pBufMat = nullptr;	// マテリアル情報
	if (FAILED(D3DXLoadMeshFromXA(modelPass, D3DXMESH_MANAGED, pDevice, nullptr, &pBufMat, nullptr, &m_numMat, &m_pMesh))){
		_MSGERROR("Failed XFile Open!!", "modelPass");
	}

	// Xファイルに法線がない場合は、法線を書き込む
	if (!(m_pMesh->GetFVF() & D3DFVF_NORMAL)){
		ID3DXMesh* pTempMesh = nullptr;
		m_pMesh->CloneMeshFVF(m_pMesh->GetOptions(), m_pMesh->GetFVF() | D3DFVF_NORMAL, pDevice, &pTempMesh);
		D3DXComputeNormals(pTempMesh, nullptr);
		m_pMesh->Release();
		m_pMesh = pTempMesh;
	}

	// マテリアル情報の取得
	D3DXMATERIAL* pMat = (D3DXMATERIAL*)pBufMat->GetBufferPointer();
	m_pMeshMat = new D3DMATERIAL9[m_numMat];
	m_pTexture = new LPDIRECT3DTEXTURE9[m_numMat];

	// マテリアル情報とテクスチャ情報の保存
	for (int i = 0; i<(int)m_numMat; i++){
		m_pMeshMat[i] = pMat[i].MatD3D;
		m_pTexture[i] = nullptr;

		if (pMat[i].pTextureFilename == nullptr) continue;

		if (FAILED(D3DXCreateTextureFromFile(pDevice, pMat[i].pTextureFilename, &m_pTexture[i]))){
			_MSGERROR("Failed Load Texture!!", "pMat[i].pTextureFilename");
		}
	}

	// メモリの解放
	if (pBufMat != nullptr) pBufMat->Release();

}

