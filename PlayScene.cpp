#include"PlayScene.h"
#include"Player.h"
#include"GoalObj.h"
#include"Field.h"
#include"Camera.h"
#include"BackGround.h"

//�R���X�g���N�^
PlayScene::PlayScene(GameObject * parent)
	: GameObject(parent, "PlayScene")
{
}

//������ 
void PlayScene::Initialize()
{
	Instantiate<Camera>(this);
	Instantiate<BackGround>(this);
	Field* pField = Instantiate<Field>(this);
	Instantiate<GoalObj>(this);
	Instantiate<Player>(this);
	pField->Reset();
}

//�X�V
void PlayScene::Update()
{
	switch (state) {
	case s_Ready:
		UpdateReady();
		break;
	case s_Play:
		UpdatePlay();
		break;
	case s_Clear:
		UpdateClear();
		break;
	case s_Dead:
		UpdateDead();
		break;
	}
}

//�`��
void PlayScene::Draw()
{
}

//�J��
void PlayScene::Release()
{
}

bool PlayScene::CanMove()
{
	return (state == s_Play);
}

void PlayScene::StartReady()
{
	state = s_Ready;
	readyTimer = 2.0f;
}

void PlayScene::UpdateReady()
{
	readyTimer -= 1.0f / 60.0f;
	if (readyTimer <= 0.0f) {
		StartPlay();
	}
}

void PlayScene::StartPlay()
{
	state = s_Play;
}

void PlayScene::UpdatePlay()
{
}

void PlayScene::UpdateClear()
{
}

void PlayScene::StartDead()
{
	state = s_Dead;
}

void PlayScene::UpdateDead()
{
}
