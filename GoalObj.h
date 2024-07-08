#pragma once
#include "Engine/GameObject.h"

class GoalObj :
    public GameObject
{
	int hAnimImage;
	
	float gRectTop;
	float gRectBottom;
	float gRectLeft;
	float gRectRight;

	int animFrame;
	int frameCounter;

	float cdTimer;

	enum State {
		NOTBRAKE,
		BRAKE,
	};
	State state;
public:
	GoalObj(GameObject* scene);
	~GoalObj();
	void Update() override;
	void Draw() override;

	void SetPosition(int x, int y);
	bool CollideRectToRect(float x, float y, float w, float h);
	void Goal();
	void UpdateRect();
};

