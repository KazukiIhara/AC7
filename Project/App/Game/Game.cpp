#include "Game.h"

// EngineScenes
#include "Scenes/SampleScene.h"

// GameScene
#include "Scenes/PlayScene.h"

Game::~Game() {}

void Game::Initialize() {
	// 規定システムの初期化
	MAGISYSTEM::Initialize();

	// シーンの追加
	// サンプルシーン
	sceneManager_->AddScene<SampleScene>("Sample");

	sceneManager_->AddScene<PlayScene>("Play");


	// 最初のシーンを設定
	sceneManager_->ChangeScene("Play");
}
