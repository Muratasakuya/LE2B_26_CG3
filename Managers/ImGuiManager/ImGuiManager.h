#pragma once

// DirectX
#include "DXInclude.h"

// imgui
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

class WinApp;
class DXCommon;

/*////////////////////////////////////////////////////////////////////////////////
*
*							ImGuiManager Class
*
////////////////////////////////////////////////////////////////////////////////*/
class ImGuiManager {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	void Initialize(WinApp* winApp,DXCommon* dxCommon);

	void Begin();
	void End();
	void Draw();

	void Finalize();

	// singleton
	static ImGuiManager* GetInstance();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/


	ImGuiManager() = default;
	~ImGuiManager() = default;
	// コピー禁止
	ImGuiManager(const ImGuiManager&) = delete;
	const ImGuiManager& operator=(const ImGuiManager&) = delete;
};