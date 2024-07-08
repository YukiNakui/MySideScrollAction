#include "GameOverScene.h"
#include"Engine/SceneManager.h"

GameOverScene::GameOverScene(GameObject* parent)
{
	hImage = LoadGraph("Assets/Scenes/GameOver.png");
	assert(hImage > 0);
}

void GameOverScene::Initialize()
{
}

void GameOverScene::Update()
{
	if (CheckHitKey(KEY_INPUT_C)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
}

void GameOverScene::Draw()
{
	DrawGraph(0, 0, hImage, TRUE);
}

void GameOverScene::Release()
{
}
