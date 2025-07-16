#include <Windows.h>
#include "kamataEngine.h"
#include "GameScene.h"

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	KamataEngine::Initialize(L"LE3D_12_ヒガ_ワタル");

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	ImGuiManager* imGuiManager = ImGuiManager::GetInstance();

	std::unique_ptr<GameScene> gameScene = std::make_unique<GameScene>();
	gameScene->Initialize();

	while (true) {
		if (KamataEngine::Update()) {
			break;
		}

		imGuiManager->Begin();

		gameScene->Update();

		imGuiManager->End();

		dxCommon->PreDraw();

		gameScene->Draw();

		imGuiManager->Draw();

		dxCommon->PostDraw();
	}

	KamataEngine::Finalize();

	return 0;
}