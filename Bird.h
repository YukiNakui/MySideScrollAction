#pragma once
#include "Enemy.h"

class Bird :
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
		ATTACK,
		DEAD,
	};
	State state;

	float jumpSpeed;

	float cdTimer;

	XMFLOAT3 targetPos;
public:
	Bird(GameObject* parent);
	~Bird();
	void Update() override;
	void Draw() override;
	void SetPosition(int x, int y);
	bool CollideRectToRect(float x, float y, float w, float h) override;
	void KillEnemy() override;
private:
	bool IsCanAttack(XMFLOAT3 pos);
};

