// author : yusuke seki
// data   : 20181102
#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <d3dx9.h>
#include "input.h"

class Controller
{
public:
	enum Device
	{
		KEYBOARDANDMOUSE,
		JOYSTICKANDTOUCHPEN,
		ALL,
	};

	enum Key
	{
		RUN,
		AVOID,
		HEAVYAVOID,
		STRAIGHTSHOT,
		DRAWSHOT,
		BREAK,
	};
	
	// コンストラクタ
	// 引数：（キー：前に進む, 後ろに進む, 左に進む, 右に進む, 回避, ストレートショット, ドローショット）
	Controller(const unsigned int& kKeyFront, const unsigned int& kKeyBack, const unsigned int& kKeyLeft, const unsigned int& kKeyRight
			 , const unsigned int& kKeyAvoid, const unsigned int& kKeyStraightShot, const unsigned int& kKeyDrawShot, const unsigned int& kKeyBreak);
	virtual ~Controller();

	static Controller* Create(Controller::Device useDevice);
	void Release();
	virtual bool IsButtonPress(Controller::Key key) = 0;
	virtual bool IsButtonTrigger(Controller::Key key) = 0;
	virtual bool IsButtonRelease(Controller::Key key) = 0;
	virtual D3DXVECTOR2 GetAxis(Controller::Key key) = 0;

protected:
	const int kKeyFront_;
	const int kKeyBack_;
	const int kKeyLeft_;
	const int kKeyRight_;
	const int kKeyAvoid_;
	const int kKeyStraightShot_;
	const int kKeyDrawShot_;
	const int kKeyBreak_;
	Input * input_;
};

#endif
