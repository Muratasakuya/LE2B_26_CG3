#include "ImGuiManager.h"

// imgui
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

#include "DXCommon.h"
#include "WinApp.h"



/*////////////////////////////////////////////////////////////////////////////////

*								singleton

////////////////////////////////////////////////////////////////////////////////*/
ImGuiManager* ImGuiManager::GetInstance() {

	static ImGuiManager instance;

	return &instance;
}



/*////////////////////////////////////////////////////////////////////////////////

*							  フレームの開始

////////////////////////////////////////////////////////////////////////////////*/
void ImGuiManager::Begin() {

#ifdef _DEBUG

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif
}



/*////////////////////////////////////////////////////////////////////////////////

*							 内部コマンドの生成

////////////////////////////////////////////////////////////////////////////////*/
void ImGuiManager::End() {

#ifdef _DEBUG

	ImGui::Render();
#endif
}



/*////////////////////////////////////////////////////////////////////////////////

*								 終了処理

////////////////////////////////////////////////////////////////////////////////*/
void ImGuiManager::Finalize() {

#ifdef _DEBUG
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif
}



/*////////////////////////////////////////////////////////////////////////////////

*								  描画

////////////////////////////////////////////////////////////////////////////////*/
void ImGuiManager::Draw() {
#ifdef _DEBUG

	DXCommon* dxCommon = DXCommon::GetInstance();

	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon->GetCommandList();
	ID3D12DescriptorHeap* descriptorHeaps[] = { dxCommon->GetSRVDescriptorHeap() };
	commandList->SetDescriptorHeaps(1, descriptorHeaps);

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.Get());
#endif
}



/*////////////////////////////////////////////////////////////////////////////////

*								  初期化

////////////////////////////////////////////////////////////////////////////////*/
void ImGuiManager::Initialize(WinApp* winApp, DXCommon* dxCommon) {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApp->GetHwnd());
	ImGui_ImplDX12_Init(
		dxCommon->GetDevice(),
		dxCommon->GetSwapChainDesc().BufferCount,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		dxCommon->GetSRVDescriptorHeap(),
		dxCommon->GetSRVDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		dxCommon->GetSRVDescriptorHeap()->GetGPUDescriptorHandleForHeapStart()
	);
}