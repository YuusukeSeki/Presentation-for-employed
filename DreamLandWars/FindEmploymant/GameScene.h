//*****************************************************************************
//
// ◆ 基底クラス ◆
// シーン遷移
// Author : Yusuke Seki
// 
//*****************************************************************************
#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include "main.h"

class GameScene
{
public:
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual GameScene* Update() = 0;
	virtual void Draw() = 0;
	virtual void Release() = 0;
private:
};

#endif
