#pragma once
#include "Engine/GameObject.h"

namespace BALL_ANIMFRAME{
	const int CHARGE_MAXFRAME = 10;
}
namespace BAF = BALL_ANIMFRAME;

class Ball :
    public GameObject
{
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Ball(GameObject* parent);
	~Ball();

	//更新
	void Update() override;

	//描画
	void Draw() override;

	void SetPosition(float x,float y);
	
	void Spike(bool isRight);
	void FirstToss();
	void SecondToss();
	
	bool IsBallAlive();
	XMFLOAT3 GetPos();
	bool IsBallCatch(float x,float y);
	void SetIsAlive(bool isalive) { isAlive = isalive; }
	int GetBallSize();
	void SetIsRight(bool right);

	void SetCharge(bool chargenow,int tosscount);
	bool IsTouchGround();
private:
	int hImage;
	int hChargeImg[BAF::CHARGE_MAXFRAME*2];

	XMVECTOR moveVec;
	bool isAlive;
	float r;
	float accsel;
	float rot;
	float rotSpeed;
	bool isRight;

	bool chargeNow;
	bool isTouchGround;

	int animType;
	int animFrame;
	int frameCounter;

	enum State {
		NORMAL,
		TOSS,
		SPIKE,
	};
	State state;
};

