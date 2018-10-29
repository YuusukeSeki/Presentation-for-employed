//*****************************************************************************
//
// 入力処理 [input.cpp]
// Author : Yusuke Seki
//
//*****************************************************************************
#include "main.h"
#include "input.h"
#include "renderer.h"


// 静的変数の初期化
LPDIRECTINPUT8       Input::m_pInput         = nullptr; // DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 Input::m_pDevJoystick   = nullptr; // 入力デバイス(ジョイスティック)へのポインタ
DIDEVCAPS			 Input::m_diJoystickCaps = {};	    // ジョイスティックの能力



//=============================================================================
// 入力処理
// 生成
Input* Input::Create(HINSTANCE hInstance, HWND hWnd)
{
	Input* pInput = new Input;
	pInput->Init( hInstance, hWnd );

	return pInput;
}


// 初期化
HRESULT Input::Init(HINSTANCE hInstance, HWND hWnd)
{
	if(m_pInput == NULL){
		// DirectInputオブジェクトの作成
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
										IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			MessageBox(hWnd, "DirectInputオブジェクトが作れねぇ！", "警告！", MB_ICONWARNING);
			return E_FAIL;
		}
	}

#ifdef _USE_KEYBOARD
	// キーボードの入力制御
	if(FAILED(InitKeyboard(hInstance,hWnd))){
		MessageBox(hWnd,"キーボードの入力制御の初期化に失敗しました","警告！",MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
#endif

	// マウスの入力制御
#ifdef _USE_MOUSE
	if(FAILED(InitMouse(hInstance,hWnd))){
		MessageBox(hWnd,"マウスの入力制御の初期化に失敗しました","警告！",MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
#endif

	// ジョイスティックの入力制御
#ifdef _USE_JOYSTICK
	if(FAILED(InitJoystick(hInstance,hWnd))){
		//MessageBox(hWnd,"ジョイスティックの入力制御の初期化に失敗しました", "警告！" ,MB_OK | MB_ICONEXCLAMATION);
		return E_FAIL;
	}
#endif

	return S_OK;
}


// 終了
void Input::Uninit(void)
{
#ifdef _USE_JOYSTICK
	// ジョイスティックの入力制御
	UninitJoystick();
#endif

#ifdef _USE_MOUSE
	// マウスの入力制御
	UninitMouse();
#endif

#ifdef _USE_KEYBOARD
	// キーボードの入力制御
	UninitKeyboard();
#endif

	// DirectInputオブジェクトの開放
	if(m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}

	delete this;
}


// 更新
void Input::Update(void)
{
#ifdef _USE_KEYBOARD
	// キーボード更新
	UpdateKeyboard();
#endif

#ifdef _USE_MOUSE
	// マウス更新
	UpdateMouse();
#endif

#ifdef _USE_JOYSTICK
	// ジョイスティック更新
	UpdateJoystick();
#endif
}



#ifdef _USE_KEYBOARD
//=============================================================================
// キーボード
// 初期化
HRESULT Input::InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// デバイスの作成
	if(FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevKeyboard, NULL)))
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// データフォーマットを設定
	if(FAILED(m_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if(FAILED(m_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevKeyboard->Acquire();

	//　変数の初期化
	for (int i = 0; i < NUM_KEY_MAX; i++) {
		m_aKeyState[i]            = 0;
		m_aKeyStateTrigger[i]     = 0;
		m_aKeyStateWTrigger[i]    = 0;
		m_aKeyStateRelease[i]     = 0;
		m_aKeyStateRepeat[i]      = 0;
		m_aKeyStateWTriggerCnt[i] = 0;
		m_aKeyStateRepeatCnt[i]   = 0;
	}

	return S_OK;
}


// 終了
void Input::UninitKeyboard(void)
{
	if(m_pDevKeyboard != NULL)
	{// 入力デバイス(キーボード)の開放
		// キーボードへのアクセス権を開放(入力制御終了)
		m_pDevKeyboard->Unacquire();

		m_pDevKeyboard->Release();
		m_pDevKeyboard = NULL;
	}
}


// 更新
void Input::UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	// デバイスからデータを取得
	if(SUCCEEDED(m_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for(int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			// キー トリガー情報の生成
			m_aKeyStateTrigger[nCnKey] = (m_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];

			// キー リリース情報の生成
			m_aKeyStateRelease[nCnKey] = (m_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & m_aKeyState[nCnKey];

			// キー リピート情報の生成
			if(aKeyState[nCnKey]){
				if(m_aKeyStateRepeatCnt[nCnKey] < LIMIT_COUNT_REPEAT){
					m_aKeyStateRepeatCnt[nCnKey]++;

					if(m_aKeyStateRepeatCnt[nCnKey] == 1 || m_aKeyStateRepeatCnt[nCnKey] >= LIMIT_COUNT_REPEAT){
						m_aKeyStateRepeat[nCnKey] = aKeyState[nCnKey];
					}
					else{
						m_aKeyStateRepeat[nCnKey] = 0;
					}
				}
			}
			else{
				m_aKeyStateRepeat[nCnKey] = 0;
				m_aKeyStateRepeatCnt[nCnKey] = 0;
			}
			//----- ボタンのダブルトリガー情報の生成 -----
			// カウンター回ってる？
			if (m_aKeyStateWTriggerCnt[nCnKey]) {
				m_aKeyStateWTriggerCnt[nCnKey]++;

				// もう一度トリガーが押されたらダブルトリガーとして判定する
				if (m_aKeyStateTrigger[nCnKey]) {
					m_aKeyStateWTrigger[nCnKey] = aKeyState[nCnKey];

					m_aKeyStateWTriggerCnt[nCnKey] = 0;
				}

				// カウンターが閾値を上回ったらフラグを降ろす
				if (m_aKeyStateWTriggerCnt[nCnKey] >= LIMIT_COUNT_WTRIGGER) {
					m_aKeyStateWTriggerCnt[nCnKey] = 0;
				}

			}
			// トリガーが押されたらカウンターを回し始める
			else if (m_aKeyStateTrigger[nCnKey]) m_aKeyStateWTriggerCnt[nCnKey]++;

			// カウンターをリセットする
			else m_aKeyStateWTrigger[nCnKey] = 0;


			// キープレス情報を保存
			m_aKeyState[nCnKey] = aKeyState[nCnKey];
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		m_pDevKeyboard->Acquire();
	}
}


// 取得 : 押しているか
bool Input::GetKeyboardPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true: false;
	//return (m_aKeyState[nKey] & 0x01) ? true: false;	// 前のフレームで押されたキーを取得
}


// 取得 : 押した瞬間か
bool Input::GetKeyboardTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true: false;
}


// 取得 : 離した瞬間か
bool Input::GetKeyboardRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}


// 取得 : ドラクエ的なアレ
bool Input::GetKeyboardRepeat(int nKey)
{
	return (m_aKeyStateRepeat[nKey] & 0x80) ? true: false;
}


// 取得 : 2回連続で押された瞬間のみ
bool Input::GetKeyboardWTrigger(int nKey)
{
	return (m_aKeyStateWTrigger[nKey] & 0x80) ? true : false;

}

#endif



#ifdef _USE_MOUSE
//=============================================================================
// マウス
// 初期化
HRESULT Input::InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	// デバイスの作成
	if(FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevMouse, NULL)))
	{
		MessageBox(hWnd, "マウスがねぇ！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// データフォーマットを設定
	if(FAILED(m_pDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		MessageBox(hWnd, "マウスのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if(FAILED(m_pDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "マウスの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// ※※	下記はバッファリング入力を読み取る場合の設定
	//		上記でイベント通知による読み取り設定を行う場合は不要。
	// バッファーサイズの設定
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize		= sizeof(dipdw);
	dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
	dipdw.diph.dwObj		= 0;
	dipdw.diph.dwHow		= DIPH_DEVICE;
	dipdw.dwData			= 100;

	// ※メモ書き
	//diprop.dwData = DIPROPAXISMODE_REL; // 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）    
	//pDIMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	if(FAILED(m_pDevMouse->SetProperty(DIPROP_BUFFERSIZE,&dipdw.diph)))
	{
		MessageBox(hWnd, "マウスのバッファーサイズを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// マウスへのアクセス権を獲得(入力制御開始)
	m_pDevMouse->Acquire();

	// 変数の初期化
	for (int i = 0; i < NUM_MOUSE_MAX; i++) {
		m_diMouseState.rgbButtons[i]         = 0;
		m_diMouseStateTrigger.rgbButtons[i]  = 0;
		m_diMouseStateWTrigger.rgbButtons[i] = 0;
		m_diMouseStateRelease.rgbButtons[i]  = 0;
		m_diMouseStateRepeat.rgbButtons[i]   = 0;
		m_diMouseStateWTriggerCnt[i]         = 0;
		m_diMouseStateRepeatCnt[i]           = 0;
	}


	return S_OK;
}


// 終了
void Input::UninitMouse(void)
{
	if(m_pDevMouse != NULL)
	{// 入力デバイスの開放
		// マウスへのアクセス権を開放(入力制御終了)
		m_pDevMouse->Unacquire();

		m_pDevMouse->Release();
		m_pDevMouse = NULL;
	}
}


// 更新
void Input::UpdateMouse(void)
{
	DIMOUSESTATE2 diMouseState;

	// デバイスからデータを取得
	if(SUCCEEDED(m_pDevMouse->GetDeviceState(sizeof(m_diMouseState), &diMouseState)))
	{
		for(int nCnKey = 0; nCnKey < NUM_MOUSE_MAX; nCnKey++)
		{
			m_diMouseState.lX = diMouseState.lX;
			m_diMouseState.lY = diMouseState.lY;
			m_diMouseState.lZ = diMouseState.lZ;

			// マウスのトリガー情報の生成
			m_diMouseStateTrigger.rgbButtons[nCnKey] =
				(m_diMouseState.rgbButtons[nCnKey] ^ diMouseState.rgbButtons[nCnKey]) & diMouseState.rgbButtons[nCnKey];

			// マウスのリリース情報の生成
			m_diMouseStateRelease.rgbButtons[nCnKey] =
				(m_diMouseState.rgbButtons[nCnKey] ^ diMouseState.rgbButtons[nCnKey]) & m_diMouseState.rgbButtons[nCnKey];

			// マウスのリピート情報の生成
			if(diMouseState.rgbButtons[nCnKey])
			{
				if(m_diMouseStateRepeatCnt[nCnKey] < LIMIT_COUNT_REPEAT)
				{
					m_diMouseStateRepeatCnt[nCnKey]++;

					if(m_diMouseStateRepeatCnt[nCnKey] == 1 || m_diMouseStateRepeatCnt[nCnKey] >= LIMIT_COUNT_REPEAT)
					{
						m_diMouseStateRepeat.rgbButtons[nCnKey] = diMouseState.rgbButtons[nCnKey];
					}
					else
					{
						m_diMouseStateRepeat.rgbButtons[nCnKey] = 0;
					}
				}
			}
			else
			{
				m_diMouseStateRepeat.rgbButtons[nCnKey] = 0;
				m_diMouseStateRepeatCnt[nCnKey] = 0;
			}
		
			//----- ボタンのダブルトリガー情報の生成 -----
			// カウンター回ってる？
			if (m_diMouseStateWTriggerCnt[nCnKey]) {
				m_diMouseStateWTriggerCnt[nCnKey]++;

				// もう一度トリガーが押されたらダブルトリガーとして判定する
				if (m_diMouseStateTrigger.rgbButtons[nCnKey]) {
					m_diMouseStateWTrigger.rgbButtons[nCnKey] = diMouseState.rgbButtons[nCnKey];

					m_diMouseStateWTriggerCnt[nCnKey] = 0;
				}

				// カウンターが閾値を上回ったらフラグを降ろす
				if (m_diMouseStateWTriggerCnt[nCnKey] >= LIMIT_COUNT_WTRIGGER) {
					m_diMouseStateWTriggerCnt[nCnKey] = 0;
				}

			}
			// トリガーが押されたらカウンターを回し始める
			else if (m_diMouseStateTrigger.rgbButtons[nCnKey]) m_diMouseStateWTriggerCnt[nCnKey]++;
			else m_diMouseStateWTrigger.rgbButtons[nCnKey] = 0;


			// マウスのプレス情報を保存
			m_diMouseState.rgbButtons[nCnKey] = diMouseState.rgbButtons[nCnKey];
		}
	}
	else
	{
		// マウスへのアクセス権を取得
		m_pDevMouse->Acquire();
	}

}


// 取得 : 押しているか
bool Input::GetMousePress(int nKey)
{
	return (m_diMouseState.rgbButtons[nKey] & 0x80) ? true: false;
	//return (m_diMouseState.rgbButtons[nKey] & 0x01) ? true: false;	// 前のフレームで押されたボタンを取得
}


// 取得 : 押した瞬間か
bool Input::GetMouseTrigger(int nKey)
{
	return (m_diMouseStateTrigger.rgbButtons[nKey] & 0x80) ? true: false;
}


// 取得 : 離した瞬間か
bool Input::GetMouseRelease(int nKey)
{
	return (m_diMouseStateRelease.rgbButtons[nKey] & 0x80) ? true: false;
}


// 取得 : ドラクエ的なアレ
bool Input::GetMouseRepeat(int nKey)
{
	return (m_diMouseStateRepeat.rgbButtons[nKey] & 0x80) ? true : false;
}


// 取得 : 2回連続で押された瞬間のみ
bool Input::GetMouseWTrigger(int nKey)
{
	return (m_diMouseStateWTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}

#endif



#ifdef _USE_JOYSTICK
//=============================================================================
// ジョイスティック
// 初期化
HRESULT Input::InitJoystick(HINSTANCE hInstance, HWND hWnd)
{
	// 各値0クリア
	ZeroMemory(&m_diJoystickState,sizeof(m_diJoystickState));
	ZeroMemory(&m_diJoystickStateTrigger,sizeof(m_diJoystickStateTrigger));
	ZeroMemory(&m_diJoystickStateRelease,sizeof(m_diJoystickStateRelease));
	ZeroMemory(&m_diJoystickStateRepeat,sizeof(m_diJoystickStateRepeat));
	ZeroMemory(&m_diJoystickStateWTrigger, sizeof(m_diJoystickStateWTrigger));

	for(int i = 0; i<NUM_JOYSTICK_MAX; i++)
	{
		m_diJoystickStateRepeatCnt[i] = 0;
		m_diJoystickStateWTriggerCnt[i] = 0;
	}


	// デバイスの作成
	if (FAILED(m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))
	{
		MessageBox(hWnd, "ジョイスティックの判別ができませんでした", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}
	
	// 接続確認
	if(m_pDevJoystick == NULL)
	{
		// ジョイスティックの接続フラグ設定
		m_bConnectJoystick = false;
		//MessageBox(hWnd,"ジョイスティックの接続が確認できませんでした\n"
		//				"ジョイスティックを使用したい場合は一度ゲームを終了し、接続してから再び起動し直してください", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}
	else
	{
		// ジョイスティックの接続フラグ設定
		m_bConnectJoystick = true;
	}

	// 軸の範囲設定
	if(FAILED(m_pDevJoystick->EnumObjects( EnumAxesCallback, NULL, DIDFT_AXIS )))
	{
		MessageBox(hWnd, "ジョイスティックの入力値の設定ができませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// データフォーマットを設定
	if(FAILED(m_pDevJoystick->SetDataFormat(&c_dfDIJoystick2)))
	{
		MessageBox(hWnd, "ジョイスティックのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if(FAILED(m_pDevJoystick->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "ジョイスティックの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// バッファーサイズの設定
	DIPROPDWORD	dipdwJoystick;
	dipdwJoystick.diph.dwSize		= sizeof(dipdwJoystick);
	dipdwJoystick.diph.dwHeaderSize = sizeof(dipdwJoystick.diph);
	dipdwJoystick.diph.dwObj		= 0;
	dipdwJoystick.diph.dwHow		= DIPH_DEVICE;
	dipdwJoystick.dwData			= 100;

	if(FAILED(m_pDevJoystick->SetProperty(DIPROP_BUFFERSIZE,&dipdwJoystick.diph)))
	{
		MessageBox(hWnd, "ジョイスティックのバッファーサイズを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// ジョイスティックへのアクセス権を獲得(入力制御開始)
	m_pDevJoystick->Acquire();

	return S_OK;
}


// 終了
void Input::UninitJoystick(void)
{
	if(m_pDevJoystick != NULL)
	{// 入力デバイスの開放
		// ジョイスティックへのアクセス権を開放(入力制御終了)
		m_pDevJoystick->Unacquire();

		m_pDevJoystick->Release();
		m_pDevJoystick = NULL;
	}
}


// 更新
void Input::UpdateJoystick(void)
{
	// 未接続で更新処理無し
	if (!m_bConnectJoystick) return;

	DIJOYSTATE2 diJoystickState;

	// デバイスに値の取得準備を通知
	m_pDevJoystick->Poll();

	// デバイスの値を取得
	if(SUCCEEDED(m_pDevJoystick->GetDeviceState( sizeof(DIJOYSTATE2), &diJoystickState )))
	{
		for(int nCnKey = 0; nCnKey < NUM_JOYSTICK_MAX; nCnKey++)
		{
			// ボタンのトリガー情報の生成
			m_diJoystickStateTrigger.rgbButtons[nCnKey] =
				(m_diJoystickState.rgbButtons[nCnKey] ^ diJoystickState.rgbButtons[nCnKey]) & diJoystickState.rgbButtons[nCnKey];

			// ボタンのリリース情報の生成
			m_diJoystickStateRelease.rgbButtons[nCnKey] =
				(m_diJoystickState.rgbButtons[nCnKey] ^ diJoystickState.rgbButtons[nCnKey]) & m_diJoystickState.rgbButtons[nCnKey];

			// ボタンのリピート情報の生成
			if(diJoystickState.rgbButtons[nCnKey])
			{
				if(m_diJoystickStateRepeatCnt[nCnKey] < LIMIT_COUNT_REPEAT)
				{
					m_diJoystickStateRepeatCnt[nCnKey]++;

					if(m_diJoystickStateRepeatCnt[nCnKey] == 1 || m_diJoystickStateRepeatCnt[nCnKey] >= LIMIT_COUNT_REPEAT)
					{
						m_diJoystickStateRepeat.rgbButtons[nCnKey] = diJoystickState.rgbButtons[nCnKey];
					}
					else
					{
						m_diJoystickStateRepeat.rgbButtons[nCnKey] = 0;
					}
				}
			}
			else{
				m_diJoystickStateRepeat.rgbButtons[nCnKey] = 0;
				m_diJoystickStateRepeatCnt[nCnKey] = 0;
			}

			//----- ボタンのダブルトリガー情報の生成 -----
			// カウンター回ってる？
			if (m_diJoystickStateWTriggerCnt[nCnKey]) {
				m_diJoystickStateWTriggerCnt[nCnKey]++;

				// もう一度トリガーが押されたらダブルトリガーとして判定する
				if (m_diJoystickStateTrigger.rgbButtons[nCnKey]) {
					m_diJoystickStateWTrigger.rgbButtons[nCnKey] = diJoystickState.rgbButtons[nCnKey];

					m_diJoystickStateWTriggerCnt[nCnKey] = 0;
				}

				// カウンターが閾値を上回ったらフラグを降ろす
				if (m_diJoystickStateWTriggerCnt[nCnKey] >= LIMIT_COUNT_WTRIGGER) {
					m_diJoystickStateWTriggerCnt[nCnKey] = 0;
				}

			}
			// トリガーが押されたらカウンターを回し始める
			else if (m_diJoystickStateTrigger.rgbButtons[nCnKey]) m_diJoystickStateWTriggerCnt[nCnKey]++;
			else m_diJoystickStateWTrigger.rgbButtons[nCnKey] = 0;


			// ボタンのプレス情報を保存
			m_diJoystickState.rgbButtons[nCnKey] = diJoystickState.rgbButtons[nCnKey];
		}
	}
	else
	{
		// ジョイスティックへのアクセス権を取得
		m_pDevJoystick->Acquire();
	}

}


// 取得 : 左スティックの傾き
D3DXVECTOR2 Input::GetJoystickLeftAxis(void)
{
	if(m_bConnectJoystick == true)
	{
		DIJOYSTATE2 DevJoystick;

		m_pDevJoystick->GetDeviceState( sizeof(DIJOYSTATE2), &DevJoystick );

		return D3DXVECTOR2( (float)DevJoystick.lX, (float)DevJoystick.lY );
	}

	return D3DXVECTOR2(0,0);

}


// 取得 : 右スティックの傾き
D3DXVECTOR2 Input::GetJoystickRightAxis(void)
{
	if(m_bConnectJoystick == true)
	{
		DIJOYSTATE2 DevJoystick;

		m_pDevJoystick->GetDeviceState( sizeof(DIJOYSTATE2), &DevJoystick );

		return D3DXVECTOR2( (float)DevJoystick.lZ, (float)DevJoystick.lRz );
	}

	return D3DXVECTOR2(0,0);
}


// 取得 : 押しているか
bool Input::GetJoystickPress(int nKey)
{
	return (m_diJoystickState.rgbButtons[nKey] & 0x80) ? true: false;
	//return (m_diJoystickState.rgbButtons[nKey] & 0x01) ? true: false;	// 前のフレームで押されたキーを取得
}


// 取得 : 押した瞬間か
bool Input::GetJoystickTrigger(int nKey)
{
	return (m_diJoystickStateTrigger.rgbButtons[nKey] & 0x80) ? true: false;
}


// 取得 : 離した瞬間か
bool Input::GetJoystickRelease(int nKey)
{
	return (m_diJoystickStateRelease.rgbButtons[nKey] & 0x80) ? true : false;
}


// 取得 : ドラクエ的なアレ
bool Input::GetJoystickRepeat(int nKey)
{
	return (m_diJoystickStateRepeat.rgbButtons[nKey] & 0x80) ? true: false;
}


// 取得 : 2回連続で押された瞬間のみ
bool Input::GetJoystickWTrigger(int nKey)
{
	return (m_diJoystickStateWTrigger.rgbButtons[nKey] & 0x80) ? true : false;

}


// 接続されているジョイスティックの判別
BOOL CALLBACK Input::EnumJoysticksCallback(const DIDEVICEINSTANCE* pDevInstance, LPVOID lpContext )
{
	UNREFERENCED_PARAMETER(pDevInstance);
	UNREFERENCED_PARAMETER(lpContext);

	// 列挙されたジョイスティックへのインターフェイスを取得
	if(FAILED(m_pInput->CreateDevice(pDevInstance->guidInstance, &m_pDevJoystick, NULL)))
	{
		MessageBox(NULL, "ジョイスティックの接続が確認できませんでした。", "警告！", MB_ICONWARNING);
		return DIENUM_CONTINUE;
	}
	
	// ジョイスティックの能力を取得
	m_diJoystickCaps.dwSize = sizeof(DIDEVCAPS);
	if(FAILED(m_pDevJoystick->GetCapabilities(&m_diJoystickCaps)))
	{
		// ジョイスティック能力の取得に失敗
		MessageBox(NULL, "ジョイスティックの能力の取得ができませんでした。", "警告！", MB_ICONWARNING);
		UninitJoystick();
		return DIENUM_CONTINUE;
	}
	
	return DIENUM_STOP;
}


// スティックの各軸の設定
BOOL CALLBACK Input::EnumAxesCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID lpvRef )
{
	// 軸の値の範囲を設定（-1000～1000）
	DIPROPRANGE diprg;
	ZeroMemory( &diprg, sizeof(diprg) );
	diprg.diph.dwSize       = sizeof(diprg); 
	diprg.diph.dwHeaderSize = sizeof(diprg.diph); 
	diprg.diph.dwObj        = lpddoi->dwType;
	diprg.diph.dwHow        = DIPH_BYID;
	diprg.lMin              = -1000;
	diprg.lMax              = +1000;

	// 設定値をジョイスティックデバイスへ反映
	if(FAILED(m_pDevJoystick->SetProperty( DIPROP_RANGE, &diprg.diph )))
	{
		MessageBox(NULL, "取得したジョイスティックに値を設定することができませんでした。", "警告！", MB_ICONWARNING);
		return DIENUM_STOP;
	}
	
	return DIENUM_CONTINUE;
}

#endif


