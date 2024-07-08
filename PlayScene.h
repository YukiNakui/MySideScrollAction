#pragma once
#include "Engine/GameObject.h"

//テストシーンを管理するクラス
class PlayScene : public GameObject
{
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	PlayScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	bool CanMove();
private:
	enum State {
		s_Ready,
		s_Play,
		s_Clear,
		s_Dead,
	};
	State state;
	
	void StartReady();
	void UpdateReady();
	void StartPlay();
	void UpdatePlay();
	void UpdateClear();
public:
	void StartDead();
private:
	void UpdateDead();
	float readyTimer;
};