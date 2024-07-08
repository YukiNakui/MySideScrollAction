#pragma once
#include "Engine/GameObject.h"
class GameOverScene :
    public GameObject
{
	int hImage;
public:
	GameOverScene(GameObject* parent);

	//‰Šú‰»
	void Initialize() override;

	//XV
	void Update() override;

	//•`‰æ
	void Draw() override;

	//ŠJ•ú
	void Release() override;
};

