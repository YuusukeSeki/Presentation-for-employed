// author : yusuke seki
// data   : 20181102
#ifndef CONTROLLER_KEYBOARDANDMOUSE_H_
#define CONTROLLER_KEYBOARDANDMOUSE_H_

#include "Controller.h"
#include "input.h"

class KeyboardAndMouse : public Controller
{
public:
	KeyboardAndMouse();
	~KeyboardAndMouse();
	bool IsButtonPress(Controller::Key key);
	bool IsButtonTrigger(Controller::Key key);
	bool IsButtonRelease(Controller::Key key);
	D3DXVECTOR2 GetAxis(Controller::Key key);

private:
	static const int kKeyFront_;
	static const int kKeyBack_;
	static const int kKeyLeft_;
	static const int kKeyRight_;
	static const int kKeyAvoid_;
	static const int kKeyStraightShot_;
	static const int kKeyDrawShot_;
	static const int kKeyBreak_;
};

#endif
