#pragma once

// Windows
#include <Windows.h>

// Lib
#include "Logger.h"

/*////////////////////////////////////////////////////////////////////////////////
* 
*							Windows Application Class 
* 
////////////////////////////////////////////////////////////////////////////////*/

class WinApp {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	bool ProcessMessage();
	void CreateMainWindow(uint32_t width, uint32_t height);

	// singleton
	static WinApp* GetInstance();

	// getter

	HWND GetHwnd() const;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	// windowHandle
	HWND hwnd_;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void RegisterWindowClass();

	WinApp() = default;
	~WinApp() = default;
	// コピー禁止
	WinApp(const WinApp&) = delete;
	const WinApp& operator=(const WinApp&) = delete;
};

