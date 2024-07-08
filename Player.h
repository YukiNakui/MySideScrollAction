#pragma once
#include "Engine/GameObject.h"
#include"Ball.h"

class Player :
    public GameObject
{
public:
	Player(GameObject* scene);
	~Player();
	void Update() override;
	void Draw() override;

	void SetPosition(int x, int y);
	XMFLOAT3 GetPosition();
private:
	int hAnimImage;
	int hBallImg;
	
	enum State {
		NORMAL = 0,
		TOSS,
		SPIKE,
		DAMAGE,
		DEAD,
	};
	State state;

	GameObject* sceneTop;
	bool prevSpaceKey;
	bool prevAttackKey;
	float moveSpeed;
	float jumpSpeed;
	bool onGround;

	int animType;
	int animFrame;
	int frameCounter;
	
	bool isRight;
	bool canMove;

	Ball* pBall;

	bool isBallAlive;
	bool canSpike;
	int tossCount;
	float cdTimer;
	
	bool IsTouchBall(XMFLOAT3 pos);
};

