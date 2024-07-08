#pragma once
#include "Engine/GameObject.h"
#include"Enemy.h"

class Wolf :
    public Enemy
{
	int hAnimImg;
	int animFrame;
	int frameCounter;
	int animType;
	bool isRight;
	bool onGround;

	enum State {
		NORMAL,
		DEAD,
	};
	State state;

	float jumpSpeed;

	float cdTimer;
public:
	Wolf(GameObject* parent);
	~Wolf();
	void Update() override;
	void Draw() override;
	void SetPosition(int x, int y);
	bool CollideRectToRect(float x, float y, float w, float h) override;
	void KillEnemy() override;
};

