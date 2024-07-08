#pragma once
#include "Engine/GameObject.h"
class ClearScene :
    public GameObject
{
	int hImage;
public:
	ClearScene(GameObject* parent);

	//‰Šú‰»
	void Initialize() override;

	//XV
	void Update() override;

	//•`‰æ
	void Draw() override;

	//ŠJ•ú
	void Release() override;
};

