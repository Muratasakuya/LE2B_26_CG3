#include "Engine.h"

#include "SceneManager.h"

// 解放忘れのチェック
struct LeakChecker {

	~LeakChecker() {

		ComPtr<IDXGIDebug1> debug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {

			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		}
	}
};

LeakChecker leakCheck_;

// WindowSize
static const uint32_t windowWidth = 1280;
static const uint32_t windowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// メインシステムの初期化
	Engine::Initialize(windowWidth, windowHeight);

	// シーン管理クラスのインスタンス
	std::unique_ptr<SceneManager> sceneManager = std::make_unique<SceneManager>();

	// メインループ
	sceneManager->Run();

	// ライブラリ終了
	Engine::Finalize();

	return 0;
}