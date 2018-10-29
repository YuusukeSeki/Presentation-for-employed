//*****************************************************************************
//
//		入力処理 
//												Author : Yusuke Seki
//*****************************************************************************
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"


#define _USE_KEYBOARD	// キーボードを使う場合はコメントアウトを外してください。
#define _USE_MOUSE		// マウスを使う場合はコメントアウトを外してください。
#define _USE_JOYSTICK	// ジョイスティックを使う場合はコメントアウトを外してください。



class Input
{
public:
	Input(){ m_pInput = NULL; }
	~Input(){}
	static Input* Create(HINSTANCE hInstance, HWND hWnd);	// 生成
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);			// 初期化
	void Uninit(void);										// 終了
	void Update(void);										// 更新

private:
	static const int LIMIT_COUNT_WTRIGGER = 30;	// ダブルクリックと判定するまでのフレーム数
	static const int LIMIT_COUNT_REPEAT   = 60;	// リピート状態に移行するまでのフレーム数
	static LPDIRECTINPUT8 m_pInput; // DirectInputオブジェクトへのポインタ


#ifdef _USE_KEYBOARD
// キーボード
public:
	// 関数
	bool GetKeyboardPress(int nKey);    // 押している間
	bool GetKeyboardTrigger(int nKey);  // 押された瞬間のみ
	bool GetKeyboardWTrigger(int nKey); // ２回連続で押された瞬間のみ
	bool GetKeyboardRepeat(int nKey);   // ドラクエ的なアレ
	bool GetKeyboardRelease(int nKey);  // 離された瞬間のみ


private:
	// 定数
	static const int NUM_KEY_MAX = 256;	// キーの最大数

	// 関数
	HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
	void UninitKeyboard(void);
	void UpdateKeyboard(void);

	// 変数
	LPDIRECTINPUTDEVICE8 m_pDevKeyboard;		// 入力デバイス(キーボード)へのポインタ
	BYTE m_aKeyState[NUM_KEY_MAX];				// キーボードの入力情報ワーク
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];		// キーボードのトリガー情報ワーク
	BYTE m_aKeyStateWTrigger[NUM_KEY_MAX];		// キーボードのダブルトリガー情報ワーク
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];		// キーボードのリリース情報ワーク
	BYTE m_aKeyStateRepeat[NUM_KEY_MAX];		// キーボードのリピート情報ワーク
	int	 m_aKeyStateWTriggerCnt[NUM_KEY_MAX];	// キーボードのダブルトリガーカウンタ
	int	 m_aKeyStateRepeatCnt[NUM_KEY_MAX];		// キーボードのリピートカウンタ


#endif


#ifdef _USE_MOUSE
// マウス
public:
	// 定数
	static const int MOUSEBUTTON_LEFT    = 0;
	static const int MOUSEBUTTON_RIGHT   = 1;
	static const int MOUSEBUTTON_CENTER  = 2;
	static const int MOUSEBUTTON_RETURN  = 3;
	static const int MOUSEBUTTON_ADVANCE = 4;

	// 関数
	bool GetMousePress(int nKey);		// 押している間
	bool GetMouseTrigger(int nKey);		// 押された瞬間のみ
	bool GetMouseWTrigger(int nKey);	// ２回連続で押された瞬間のみ
	bool GetMouseRepeat(int nKey);		// ドラクエ的なアレ
	bool GetMouseRelease(int nKey);		// 離された瞬間のみ


private:
	// 定数
	static const int NUM_MOUSE_MAX		 = 8;	// マウスボタンの最大数

	// 関数
	HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd);
	void UninitMouse(void);
	void UpdateMouse(void);

	// 変数
	LPDIRECTINPUTDEVICE8 m_pDevMouse;						// 入力デバイス(マウス)へのポインタ
	DIMOUSESTATE2 m_diMouseState;							// マウスのプレス情報ワーク
	DIMOUSESTATE2 m_diMouseStateTrigger;					// マウスのトリガー情報ワーク
	DIMOUSESTATE2 m_diMouseStateWTrigger;					// マウスのダブルトリガー情報ワーク
	DIMOUSESTATE2 m_diMouseStateRelease;					// マウスのリリース情報ワーク
	DIMOUSESTATE2 m_diMouseStateRepeat;						// マウスのリピート情報ワーク
	int           m_diMouseStateWTriggerCnt[NUM_MOUSE_MAX];	// マウスのダブルトリガーカウンタ
	int           m_diMouseStateRepeatCnt[NUM_MOUSE_MAX];	// マウスのリピートカウンタ


#endif


#ifdef _USE_JOYSTICK
// ジョイスティック
public:
	// 定数
	static const int JOYSTICKBUTTON001 = 0;
	static const int JOYSTICKBUTTON002 = 1;
	static const int JOYSTICKBUTTON003 = 2;
	static const int JOYSTICKBUTTON004 = 3;
	static const int JOYSTICKBUTTON005 = 4;
	static const int JOYSTICKBUTTON006 = 5;
	static const int JOYSTICKBUTTON007 = 6;
	static const int JOYSTICKBUTTON008 = 7;
	static const int JOYSTICKBUTTON009 = 8;
	static const int JOYSTICKBUTTON010 = 9;
	static const int JOYSTICKBUTTON011 = 10;
	static const int JOYSTICKBUTTON012 = 11;

	// 関数
	D3DXVECTOR2 GetJoystickLeftAxis(void);	// 左スティックの傾き値
	D3DXVECTOR2 GetJoystickRightAxis(void);	// 右スティックの傾き値
	bool GetJoystickPress(int nKey);		// 押している間
	bool GetJoystickTrigger(int nKey);		// 押された瞬間のみ
	bool GetJoystickWTrigger(int nKey);		// 2回連続で押された瞬間のみ
	bool GetJoystickRepeat(int nKey);		// ドラクエ的なアレ
	bool GetJoystickRelease(int nKey);		// 離された瞬間のみ


private:
	// 定数
	static const int NUM_JOYSTICK_MAX     = 128;	// ジョイスティックボタンの最大数
	static const int JOYSTICK_BUFFER_SIZE = 100;	// ジョイスティック入力のバッファーサイズ

	// 関数
	HRESULT InitJoystick(HINSTANCE hInstance, HWND hWnd);
	static void UninitJoystick(void);
	void UpdateJoystick(void);
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pDevInstance, LPVOID lpContext);
	static BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID lpvRef);

	// 変数
	static LPDIRECTINPUTDEVICE8 m_pDevJoystick;			        // 入力デバイス(ジョイスティック)へのポインタ
	static DIDEVCAPS m_diJoystickCaps;			                // ジョイスティックの能力
	DIJOYSTATE2	m_diJoystickState;			                    // ジョイスティックのプレス情報ワーク
	DIJOYSTATE2	m_diJoystickStateTrigger;	                    // ジョイスティックのトリガー情報ワーク
	DIJOYSTATE2	m_diJoystickStateWTrigger;	                    // ジョイスティックのダブルトリガー情報ワーク
	DIJOYSTATE2	m_diJoystickStateRelease;	                    // ジョイスティックのリリース情報ワーク
	DIJOYSTATE2	m_diJoystickStateRepeat;	                    // ジョイスティックのリピート情報ワーク
	int			m_diJoystickStateWTriggerCnt[NUM_JOYSTICK_MAX]; // ジョイスティックのダブルトリガーカウンタ
	int			m_diJoystickStateRepeatCnt[NUM_JOYSTICK_MAX];   // ジョイスティックのリピートカウンタ
	bool		m_bConnectJoystick;		                        // trueの場合、ジョイスティック接続有り



#endif

};

#endif
