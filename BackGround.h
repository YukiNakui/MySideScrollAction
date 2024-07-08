#pragma once
#include "Engine/GameObject.h"

class BackGround :
    public GameObject
{
    int hImage;
public:
	BackGround(GameObject* scene);
	~BackGround();
	void Update() override;
	void Draw() override;
};

