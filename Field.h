#pragma once
#include "Engine/GameObject.h"
class Field :
    public GameObject
{
public:
	Field(GameObject* scene);
	~Field();
	void Reset();
	void Update() override;
	void Draw() override;

	float CollisionRight(float x, float y);
	float CollisionDown(float x, float y);
	float CollisionLeft(float x, float y);
	float CollisionUp(float x, float y);

	void SetNextStage();
	bool CanNextStageChange();
private:
	int hImage;

	bool IsWallBlock(int x, int y);
	int* Map;
	int width;
	int height;
	int stageNum;
};

