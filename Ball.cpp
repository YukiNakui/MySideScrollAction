#include "Ball.h"
#include"Field.h"
#include <DxLib.h>
#include <assert.h>
#include"Enemy.h"
#include"Player.h"
#include"Camera.h"
#include"ItemBox.h"
#include"GoalObj.h"
#include"Engine/SceneManager.h"

namespace {
	const float BALL_E = 0.8f;
	const float GRAVITY = 9.8f / 60.0f;
	const float MAX_TOSS_HEIGHT = 64.0f * 6.0f;
	const float BALL_WIDTH = 32.0f;
	const float BALL_HEIGHT = 32.0f;
	const float CORRECT_VALUE = 5.0f;
	static const int WINDOW_WIDTH = 1280;
	static const int WINDOW_HEIGHT = 720;
}

Ball::Ball(GameObject* parent)
	:GameObject(parent,"Ball")
{
	hImage = LoadGraph("Assets/ball.png");
	assert(hImage > 0);
	LoadDivGraph("Assets/Effects/ChargeEffects.png", BAF::CHARGE_MAXFRAME * 2, BAF::CHARGE_MAXFRAME, 2, 120, 120, hChargeImg);
	for (int i = 0; i < BAF::CHARGE_MAXFRAME*2; i++) {
		assert(hChargeImg[i] > 0);
	}

	moveVec = XMVectorZero();
	isAlive = true;
	r = 32.0f;
	accsel = 0.0f;
	rot = 0.0f;
	rotSpeed = 0.0f;
	isRight = true;
	animType = 0;
	animFrame = 0;
	frameCounter = 0;
	chargeNow = false;
	isTouchGround = false;

	state = NORMAL;
}

Ball::~Ball()
{
	if (hImage > 0)
	{
		DeleteGraph(hImage);
	}
}

void Ball::Update()
{
	Field* pField = GetParent()->FindGameObject<Field>();
	std::list<ItemBox*> pIBoxs = GetParent()->FindGameObjects<ItemBox>();
	
	XMVECTOR vBall = XMLoadFloat3(&transform_.position_);
	XMFLOAT3 move;
	XMStoreFloat3(&move, moveVec);

	XMVECTOR nextPos = vBall + moveVec + XMVectorSet(0.0f, GRAVITY, 0.0f, 0.0f);
	XMFLOAT3 nextPosFloat;
	XMStoreFloat3(&nextPosFloat, nextPos);

	if (chargeNow) {
		if (animFrame < BAF::CHARGE_MAXFRAME - 1) {
			frameCounter++;
			if (frameCounter >= 5) {
				frameCounter = 0;
				animFrame++;
			}
		}
		else
			animFrame = 0;
	}
	else {
		animFrame = 0;
		frameCounter = 0;
	}

	if (pField != nullptr) {
		float cx = 13.0f;
		float cy = 13.0f;
		float pushRTop = pField->CollisionRight(nextPosFloat.x + cx, nextPosFloat.y - cy + CORRECT_VALUE);
		float pushRBottom = pField->CollisionRight(nextPosFloat.x + cx, nextPosFloat.y + cy - CORRECT_VALUE);
		float pushR = max(pushRBottom, pushRTop);
		if (pushR > 0.0f) {
			transform_.position_.x -= pushR - 1.0f;
			move.x = -move.x * BALL_E;
			rotSpeed = rotSpeed * BALL_E;
			isRight = false;
		}
		float pushLTop = pField->CollisionLeft(nextPosFloat.x - cx, nextPosFloat.y - cy + CORRECT_VALUE);
		float pushLBottom = pField->CollisionLeft(nextPosFloat.x - cx, nextPosFloat.y + cy - CORRECT_VALUE);
		float pushL = max(pushLBottom, pushLTop);
		if (pushL > 0.0f) {
			transform_.position_.x += pushL - 1.0f;
			move.x = -move.x * BALL_E;
			rotSpeed = rotSpeed * BALL_E;
			isRight = true;
		}

		float pushTRight = pField->CollisionUp(nextPosFloat.x + cx - CORRECT_VALUE, nextPosFloat.y - cy);
		float pushTLeft = pField->CollisionUp(nextPosFloat.x - cx + CORRECT_VALUE, nextPosFloat.y - cy);
		float pushT = max(pushTRight, pushTLeft);
		if (pushT > 0.0f) {
			transform_.position_.y += pushT - 1.0f;
			move.y = -move.y * BALL_E;
			rotSpeed = rotSpeed * BALL_E;
		}
		float pushBRight = pField->CollisionDown(nextPosFloat.x + cx - CORRECT_VALUE, nextPosFloat.y + cy);
		float pushBLeft = pField->CollisionDown(nextPosFloat.x - cx + CORRECT_VALUE, nextPosFloat.y + cy);
		float pushB = max(pushBRight, pushBLeft);
		if (pushB > 0.0f) {
			transform_.position_.y -= pushB - 1.0f;
			move.y = -move.y * BALL_E;
			rotSpeed = rotSpeed * BALL_E;
			isTouchGround = true;
			state = NORMAL;
			XMVECTOR v = XMVector3Length(moveVec);
			float length = XMVectorGetX(v);
			if (length < 5.0f)
			{
				isAlive = false;
				transform_.position_.y = 720.0f;
			}
		}
	}
	for (ItemBox* pIBox : pIBoxs) {
		float cx = BALL_WIDTH / 2.0f;
		float cy = BALL_HEIGHT / 2.0f;
		float pushRTop = pIBox->CollisionRight(nextPosFloat.x + cx, nextPosFloat.y - cy + CORRECT_VALUE);
		float pushRBottom = pIBox->CollisionRight(nextPosFloat.x + cx, nextPosFloat.y + cy - CORRECT_VALUE);
		float pushR = max(pushRBottom, pushRTop);
		if (pushR > 0.0f) {
			transform_.position_.x -= pushR - 1.0f;
			move.x = -move.x * BALL_E;
			rotSpeed = rotSpeed * BALL_E;
			isRight = false;
		}
		float pushLTop = pIBox->CollisionLeft(nextPosFloat.x - cx, nextPosFloat.y - cy + CORRECT_VALUE);
		float pushLBottom = pIBox->CollisionLeft(nextPosFloat.x - cx, nextPosFloat.y + cy - CORRECT_VALUE);
		float pushL = max(pushLBottom, pushLTop);
		if (pushL > 0.0f) {
			transform_.position_.x += pushL - 1.0f;
			move.x = -move.x * BALL_E;
			rotSpeed = rotSpeed * BALL_E;
			isRight = true;
		}

		float pushTRight = pIBox->CollisionUp(nextPosFloat.x + cx - CORRECT_VALUE, nextPosFloat.y - cy);
		float pushTLeft = pIBox->CollisionUp(nextPosFloat.x - cx + CORRECT_VALUE, nextPosFloat.y - cy);
		float pushT = max(pushTRight, pushTLeft);
		if (pushT > 0.0f) {
			transform_.position_.y += pushT - 1.0f;
			move.y = -move.y * BALL_E;
			rotSpeed = rotSpeed * BALL_E;
			isTouchGround = true;
		}
		float pushBRight = pIBox->CollisionDown(nextPosFloat.x + cx - CORRECT_VALUE, nextPosFloat.y + cy);
		float pushBLeft = pIBox->CollisionDown(nextPosFloat.x - cx + CORRECT_VALUE, nextPosFloat.y + cy);
		float pushB = max(pushBRight, pushBLeft);
		if (pushB > 0.0f) {
			transform_.position_.y -= pushB - 1.0f;
			move.y = -move.y * BALL_E;
			rotSpeed = rotSpeed * BALL_E;
			isTouchGround = true;
			XMVECTOR v = XMVector3Length(moveVec);
			float length = XMVectorGetX(v);
			if (length < 5.0f)
			{
				isAlive = false;
				transform_.position_.y = 720.0f;
			}
		}
	}

	moveVec = XMLoadFloat3(&move);
	moveVec += XMVectorSet(0.0f, GRAVITY, 0.0f, 0.0f);
	vBall += moveVec;
	XMStoreFloat3(&transform_.position_, vBall);
	if (rotSpeed <= 0)
		rotSpeed = 0;
	if (isRight)
		rot += rotSpeed;
	else
		rot -= rotSpeed;

	std::list<Enemy*> pEnemies = GetParent()->FindGameObjects<Enemy>();
	for (Enemy* pEnemy : pEnemies) {
		if (pEnemy->CollideRectToRect(transform_.position_.x, transform_.position_.y, BALL_WIDTH / 2.0f, BALL_HEIGHT / 2.0f)) {
			pEnemy->KillEnemy();
			isAlive = false;
			transform_.position_.y = 720.0f;
		}
	}

	GoalObj* pGoal = GetParent()->FindGameObject<GoalObj>();
	if (pGoal->CollideRectToRect(transform_.position_.x, transform_.position_.y, BALL_WIDTH / 2.0f, BALL_HEIGHT / 2.0f)) {
		if (state == SPIKE) {
			pGoal->Goal();
			isAlive = false;
			transform_.position_.y = 720.0f;
			
		}
	}

	float x = (int)transform_.position_.x;
	float y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}
	if ((y < 0 || y > WINDOW_HEIGHT) ||
		(x < 0 || x > WINDOW_WIDTH)) {
		isAlive = false;
		KillMe();
	}
}

void Ball::Draw()
{
	float x = (int)transform_.position_.x;
	float y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}

	if (isAlive) {
		DrawRotaGraph(x, y, 1.0, rot, hImage, TRUE);
	}

	if (chargeNow) {
		DrawRotaGraph(x, y, 0.8, 0, hChargeImg[BAF::CHARGE_MAXFRAME * animType + animFrame], TRUE);
	}
}

void Ball::SetPosition(float x,float y)
{
	transform_.position_.x = x;
	transform_.position_.y = y;
}

void Ball::Spike(bool isRight)
{
	isAlive = true;
	float spikeSpeed = 10.0f;
	rotSpeed = 1.0f;

	state = SPIKE;

	if (isRight)
	{
		moveVec = { spikeSpeed,spikeSpeed,0.0f,0.0f };
	}
	else
	{
		moveVec = { -spikeSpeed,spikeSpeed,0.0f,0.0f };
	}
}

void Ball::FirstToss()
{
	isAlive = true;
	rotSpeed = 0.4f;
	float tossSpeed = -sqrt(2 * GRAVITY * MAX_TOSS_HEIGHT / 2.0f);
	moveVec = { 0.0f,  tossSpeed, 0.0f,0.0f};
	state = TOSS;
}

void Ball::SecondToss()
{
	isAlive = true;
	rotSpeed = 0.4f;
	float tossSpeed = -sqrt(2 * GRAVITY * MAX_TOSS_HEIGHT);
	moveVec = { 0.0f,  tossSpeed, 0.0f,0.0f };
	state = TOSS;
}

bool Ball::IsBallAlive()
{
	return isAlive;
}

XMFLOAT3 Ball::GetPos()
{
	return transform_.position_;
}

bool Ball::IsBallCatch(float x,float y)
{
	float lenX = x - transform_.position_.x;
	float lenY = y - transform_.position_.y;
	float len = sqrt(lenX * lenX + lenY * lenY);
	if (len < 20.0f) {
		isAlive = false;
		return true;
	}
	return false;
}

int Ball::GetBallSize()
{
	return BALL_WIDTH;
}

void Ball::SetIsRight(bool right)
{
	isRight = right;
}

void Ball::SetCharge(bool chargenow,int tosscount)
{
	chargeNow = chargenow;
	animType = tosscount - 1;
}

bool Ball::IsTouchGround()
{
	return isTouchGround;
}
