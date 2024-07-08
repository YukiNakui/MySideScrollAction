#include "TitleScene.h"
#include"Engine/SceneManager.h"

TitleScene::TitleScene(GameObject* parent)
{
	hImage = LoadGraph("Assets/Scenes/Title.png");
	assert(hImage > 0);
}

void TitleScene::Initialize()
{
}

void TitleScene::Update()
{
	if (CheckHitKey(KEY_INPUT_C)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
}

void TitleScene::Draw()
{
	DrawGraph(0, 0, hImage, TRUE);
}

void TitleScene::Release()
{
}
