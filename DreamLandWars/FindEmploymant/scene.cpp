//*****************************************************************************
//	
//		ポリゴンマネージャー
//													Author : Yusuke Seki
//*****************************************************************************
#include "scene.h"

//----- 静的メンバ変数初期化 -----
Scene* Scene::m_pScene[Scene::NUM_TYPE][Scene::MAX_SCENE] = {};



//-----------------------------------------------------------------------------
//	コンストラクタ
//-----------------------------------------------------------------------------
Scene::Scene(int priority, Scene::TYPE type)
{
	for(int i=0; i<MAX_SCENE; i++){
		if(m_pScene[priority][i] == nullptr){
			m_pScene[priority][i] = this;
			m_pos = D3DXVECTOR3(0,0,0);
			m_sceneType = type;

			m_group = GROUP_NONE;
			break;
		}
	}
}


//-----------------------------------------------------------------------------
//	解放処理：個別
//-----------------------------------------------------------------------------
void Scene::Release()
{
	for( int j=0; j<NUM_TYPE; j++ ){
		for(int i=0; i<MAX_SCENE; i++){
			if(m_pScene[j][i] == this){
				Uninit();
				delete this;
				m_pScene[j][i] = nullptr;
				break;
			}
		}
	}
}


//-----------------------------------------------------------------------------
//	終了処理
//-----------------------------------------------------------------------------
void Scene::UninitAll()
{
	for( int j=0; j<NUM_TYPE; j++ ){
		for(int i=0; i<MAX_SCENE; i++){
			if(m_pScene[j][i] != nullptr){
				m_pScene[j][i]->Uninit();
			}
		}
	}
}


//-----------------------------------------------------------------------------
//	更新処理
//-----------------------------------------------------------------------------
void Scene::UpdateAll()
{
	for( int j=0; j<NUM_TYPE; j++ ){
		for(int i=0; i<MAX_SCENE; i++){
			if(m_pScene[j][i] != nullptr){
				m_pScene[j][i]->Update();
			}
		}
	}
}


//-----------------------------------------------------------------------------
//	描画処理
//-----------------------------------------------------------------------------
void Scene::DrawAll()
{
	for( int j=0; j < NUM_TYPE; j++ ){
		for(int i=0; i < MAX_SCENE; i++){
			if(m_pScene[j][i] != nullptr){
				m_pScene[j][i]->Draw();
			}
		}
	}
}


//-----------------------------------------------------------------------------
//	解放処理
//-----------------------------------------------------------------------------
void Scene::ReleaseAll()
{
	for( int j=0; j<NUM_TYPE; j++ ){
		for(int i=0; i<MAX_SCENE; i++){
			if(m_pScene[j][i] != nullptr){
				m_pScene[j][i]->Release();
			}
		}
	}
}


//-----------------------------------------------------------------------------
//	
//-----------------------------------------------------------------------------
Scene* Scene::GetScene(Scene::TYPE type, int arrayIdx1, int arrayIdx2)
{
	// 実体が無ければ nullptr を返す
	if (m_pScene[arrayIdx2][arrayIdx1] == nullptr) return nullptr;

	// タイプが違えば nullptr を返す
	if (m_pScene[arrayIdx2][arrayIdx1]->m_sceneType != type) return nullptr;

	// 一致したのでポインタを返す
	return m_pScene[arrayIdx2][arrayIdx1];

}


