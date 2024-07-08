#include "Player.h"
#include <DxLib.h>
#include <assert.h>
#include"PlayScene.h"
#include"Ball.h"
#include"Enemy.h"
#include"Field.h"
#include"ItemBox.h"
#include"Camera.h"
#include"Engine/SceneManager.h"

namespace {
	const int IDLE_MAXFRAME{ 6 };
	const int WALK_MAXFRAME{ 6 };
	const int TOSS_MAXFRAME{ 4 };
	const int SPIKE_MAXFRAME{ 4 };
	const int DEAD_MAXFRAME{ 5 };

	const float MAX_MOVE_SPEED = 3.5f;
	const float GROUND = 600.0f;
	const float JUMP_HIGHT = 64.0f * 1.5f;
	const float GRAVITY = 8.0f / 60.0f;
	
	const float PLAYER_WIDTH = 128.0f;
	const float PLAYER_HEIGHT = 128.0f;
	const float CORRECT_WIDTH = 50.0f;
	const float CORRECT_HEIGHT = 3.0f;
	const float CORRECT_TOP = 55.0f;
	const float CORRECT_BOTTOM = 3.0f;
};

Player::Player(GameObject* parent) : GameObject(sceneTop)
{
	hAnimImage = LoadGraph("Assets/Player/Player.png");
	assert(hAnimImage > 0);
	hBallImg = LoadGraph("Assets/Ball.png");
	assert(hBallImg > 0);

	transform_.position_.x = 128.0f;
	transform_.position_.y = GROUND;

	moveSpeed = 0.0;
	jumpSpeed = 0.0f;
	onGround = true;
	isRight = true;
	canMove = true;

	pBall = nullptr;
	isBallAlive = false;
	canSpike = false;
	tossCount = 0;

	animFrame = 0;
	animType = 0;
	frameCounter = 0;

	cdTimer = 0.0f;
	
	state = NORMAL;
}

Player::~Player()
{
	if (hAnimImage > 0)
	{
		DeleteGraph(hAnimImage);
	}
	if (hBallImg > 0) {
		DeleteGraph(hBallImg);
	}
}

void Player::Update()
{
	Field* pField = GetParent()->FindGameObject<Field>();
	std::list<ItemBox*> pIBoxs = GetParent()->FindGameObjects<ItemBox>();
	Camera* cam = GetParent()->FindGameObject<Camera>();

	if (state == DEAD) {
		if (animFrame < DEAD_MAXFRAME - 1) {
			frameCounter++;
			if (frameCounter >= 10) {
				frameCounter = 0;
				animFrame++;
			}
		}
		else {
			cdTimer += 1.0f / 60.0f;
			if (cdTimer > 2.0f) {
				cdTimer = 0.0f;
				SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
				pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
			}
		}
	}

	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	if (!scene->CanMove()) {
		canMove = false;
	}

	if (state == TOSS) {
		if (animFrame < TOSS_MAXFRAME - 1) {
			frameCounter++;
			if (frameCounter >= 10) {
				frameCounter = 0;
				animFrame++;
			}
			if (animFrame == 1) {
				if (tossCount == 1) {
					if (pBall != nullptr) {
						pBall->FirstToss();
					}
				}
			}
		}
		else {
			canMove = true;
			animType = 0;
			animFrame = 0;
			frameCounter = 0;
			state = NORMAL;
		}
	}

	float nextPos_x = transform_.position_.x;
	float nextPos_y = transform_.position_.y;

	if (state == SPIKE) {
		if (animFrame < SPIKE_MAXFRAME - 1) {
			frameCounter++;
			if (frameCounter >= 10) {
				frameCounter = 0;
				animFrame++;
			}
			if (animFrame == 2) {
				if (pBall != nullptr) {
					if (isRight)
						nextPos_x = pBall->GetPos().x - 10.0f;
					else
						nextPos_x = pBall->GetPos().x + 10.0f;
					nextPos_y = pBall->GetPos().y + PLAYER_HEIGHT / 2.0f - CORRECT_TOP;
					pBall->Spike(isRight);
					if (cam != nullptr)
						cam->VibrationY(10.0f);
				}
			}
		}
		else {
			canMove = true;
			animType = 0;
			animFrame = 0;
			frameCounter = 0;
			state = NORMAL;
		}
	}

	float len = 0.0f;
	if (pBall != nullptr) {
		float lenX = pBall->GetPos().x - transform_.position_.x;
		float lenY = pBall->GetPos().y - transform_.position_.y;
		len = sqrt(lenX * lenX + lenY * lenY);
	}

	if (CheckHitKey(KEY_INPUT_K))
	{
		if (!prevAttackKey) {
			if (!isBallAlive || (pBall != nullptr && !pBall->IsBallAlive())) {
				pBall = nullptr;
				pBall = Instantiate<Ball>(GetParent());
				isBallAlive = true;
				pBall->SetPosition(transform_.position_.x, transform_.position_.y - PLAYER_HEIGHT / 2.0f + 30.0f);
				pBall->SetIsRight(isRight);
				canMove = false;
				tossCount += 1;
				animType = 2;
				animFrame = 0;
				frameCounter = 0;
				state = TOSS;
			}
			else {
				if (tossCount > 0) {
					if (IsTouchBall(pBall->GetPos())) {
						if (tossCount == 1) {
							pBall->SetIsRight(isRight);
							pBall->SecondToss();
							canMove = false;
							tossCount += 1;
							animType = 2;
							animFrame = 0;
							state = TOSS;
						}
					}
					else {
						if (len > PLAYER_HEIGHT && pBall->GetPos().y <= transform_.position_.y - PLAYER_HEIGHT) {
							if (!pBall->IsTouchGround()) {
								canMove = false;
								tossCount = 0;
								animType = 3;
								animFrame = 0;
								pBall->SetIsRight(isRight);
								state = SPIKE;
							}
						}
					}
				}
			}
			prevAttackKey = true;
		}
	}
	else {
		prevAttackKey = false;
	}

	if (pBall != nullptr) {
		if (tossCount > 0) {
			if (len > PLAYER_HEIGHT && pBall->GetPos().y <= transform_.position_.y - PLAYER_HEIGHT)
				if (!pBall->IsTouchGround())
					canSpike = true;
				else
					canSpike = false;
			else {
				canSpike = false;
			}
		}
		pBall->SetCharge(canSpike, tossCount);
		if (len > 600) {
			pBall->SetPosition(0.0f, 800.0f);
			isBallAlive = false;
			pBall = nullptr;
			tossCount = 0;
		}
	}

	if (pBall!=nullptr){ 
		if (!pBall->IsBallAlive() || pBall->IsBallCatch(transform_.position_.x - 10.0f, transform_.position_.y + PLAYER_HEIGHT / 4.0f)) {
			pBall->SetPosition(0.0f, 800.0f);
			isBallAlive = false;
			pBall = nullptr;
			tossCount = 0;
		}
	}
	
	
	if (canMove) {
		if (CheckHitKey(KEY_INPUT_D))
		{
			if (onGround)
				animType = 1;
			if (moveSpeed < 0)
				moveSpeed = 0.0f;
			moveSpeed += 0.1f;
			if (moveSpeed >= MAX_MOVE_SPEED)
				moveSpeed = MAX_MOVE_SPEED;
			nextPos_x += moveSpeed;
			float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH;
			float cy = PLAYER_HEIGHT / 2.0f;
			if (++frameCounter >= WALK_MAXFRAME) {
				animFrame = (animFrame + 1) % WALK_MAXFRAME;
				frameCounter = 0;
			}
			if (pField != nullptr) {
				float pushTright = pField->CollisionRight(nextPos_x + cx, nextPos_y - (cy - CORRECT_TOP) + 1.0f);
				float pushBright = pField->CollisionRight(nextPos_x + cx, nextPos_y + (cy - CORRECT_BOTTOM) - 1.0f);
				float pushRight = max(pushBright, pushTright);
				if (pushRight > 0.0f) {
					nextPos_x -= pushRight - 1.0f;
				}
			}
			for (ItemBox* pIBox : pIBoxs) {
				if (pIBox != nullptr) {
					float pushTright = pIBox->CollisionRight(nextPos_x + cx, nextPos_y - (cy - CORRECT_TOP) + 1.0f);
					float pushBright = pIBox->CollisionRight(nextPos_x + cx, nextPos_y + (cy - CORRECT_BOTTOM) - 1.0f);
					float pushRight = max(pushBright, pushTright);
					if (pushRight > 0.0f) {
						nextPos_x -= pushRight - 1.0f;
					}
				}
			}
			isRight = true;
		}
		else if (CheckHitKey(KEY_INPUT_A))
		{
			if (onGround)
				animType = 1;
			if (moveSpeed > 0)
				moveSpeed = 0.0f;
			moveSpeed -= 0.1f;
			if (moveSpeed <= -MAX_MOVE_SPEED)
				moveSpeed = -MAX_MOVE_SPEED;
			nextPos_x += moveSpeed;
			float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH;
			float cy = PLAYER_HEIGHT / 2.0f;
			if (++frameCounter >= WALK_MAXFRAME) {
				animFrame = (animFrame + 1) % WALK_MAXFRAME;
				frameCounter = 0;
			}
			if (pField != nullptr) {
				float pushTleft = pField->CollisionLeft(nextPos_x - cx, nextPos_y - (cy - CORRECT_TOP) + 1.0f);
				float pushBleft = pField->CollisionLeft(nextPos_x - cx, nextPos_y + (cy - CORRECT_BOTTOM) - 1.0f);
				float pushLeft = max(pushBleft, pushTleft);
				if (pushLeft > 0.0f) {
					nextPos_x += pushLeft - 1.0f;
				}
			}
			for (ItemBox* pIBox : pIBoxs) {
				if (pIBox != nullptr) {
					float pushTleft = pIBox->CollisionLeft(nextPos_x - cx, nextPos_y - (cy - CORRECT_TOP) + 1.0f);
					float pushBleft = pIBox->CollisionLeft(nextPos_x - cx, nextPos_y + (cy - CORRECT_BOTTOM) - 1.0f);
					float pushLeft = max(pushBleft, pushTleft);
					if (pushLeft > 0.0f) {
						nextPos_x += pushLeft - 1.0f;
					}
				}
			}
			isRight = false;
		}
		else {
			moveSpeed = 0.0f;
			if (onGround) {
				animType = 0;
				animFrame = 0;
				frameCounter = 0;
			}
		}

		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			if (prevSpaceKey == false) {
				if (onGround) {
					jumpSpeed = -sqrt(2 * GRAVITY * JUMP_HIGHT);
					onGround = false;
					animType = 3;
				}
			}
			prevSpaceKey = true;
		}
		else {
			prevSpaceKey = false;
		}

		if (!onGround) {
			if (state == NORMAL && animType == 3)
			{
				animFrame = 3;
			}
		}
	}
	

	jumpSpeed += GRAVITY; 
	nextPos_y += jumpSpeed;

	if (pField != nullptr) {
		float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH - 5.0f;
		float cy = PLAYER_HEIGHT / 2.0f;

		float pushRbottom = pField->CollisionDown(nextPos_x + cx, nextPos_y + (cy - CORRECT_BOTTOM));
		float pushLbottom = pField->CollisionDown(nextPos_x - cx, nextPos_y + (cy - CORRECT_BOTTOM));
		float pushBottom = max(pushRbottom, pushLbottom);
		if (pushBottom > 0.0f) {
			nextPos_y -= pushBottom - 1.0f;
			jumpSpeed = 0.0f;
			onGround = true;
		}
		else {
			onGround = false;
		}
		float pushRtop = pField->CollisionUp(nextPos_x + cx, nextPos_y - (cy - CORRECT_TOP));
		float pushLtop = pField->CollisionUp(nextPos_x - cx, nextPos_y - (cy - CORRECT_TOP));
		float pushTop = max(pushRtop, pushLtop);
		if (pushTop > 0.0f) {
			nextPos_y += pushTop - 1.0f;
			jumpSpeed = 0.0f;
		}
	}

	for (ItemBox* pIBox : pIBoxs) {
		if (pIBox != nullptr) {
			float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH - 5.0f;
			float cy = PLAYER_HEIGHT / 2.0f;

			float pushRbottom = pIBox->CollisionDown(nextPos_x + cx, nextPos_y + (cy - CORRECT_BOTTOM));
			float pushLbottom = pIBox->CollisionDown(nextPos_x - cx, nextPos_y + (cy - CORRECT_BOTTOM));
			float pushBottom = max(pushRbottom, pushLbottom);
			if (pushBottom > 0.0f) {
				nextPos_y -= pushBottom - 1.0f;
				jumpSpeed = 0.0f;
				onGround = true;
			}
			float pushRtop = pIBox->CollisionUp(nextPos_x + cx, nextPos_y - (cy - CORRECT_TOP));
			float pushLtop = pIBox->CollisionUp(nextPos_x - cx, nextPos_y - (cy - CORRECT_TOP));
			float pushTop = max(pushRtop, pushLtop);
			if (pushTop > 0.0f) {
				nextPos_y += pushTop;
				jumpSpeed = 0.0f;
			}
		}
	}

	transform_.position_ = { nextPos_x,nextPos_y,0.0f };

	if (transform_.position_.y > 720.0f) {
		if (pBall != nullptr) {
			pBall->KillMe();
		}
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
	}
	
	if (state != DEAD) {
		std::list<Enemy*> pEnemies = GetParent()->FindGameObjects<Enemy>();
		for (Enemy* pEnemy : pEnemies) {
			if (pEnemy->CollideRectToRect(transform_.position_.x, transform_.position_.y, PLAYER_WIDTH / 2.0f, PLAYER_HEIGHT / 2.0f)) {
				canMove = false;
				animType = 5;
				animFrame = 0;
				state = DEAD;
				if (cam != nullptr)
					cam->VibrationX(100.0f);
				scene->StartDead();
				if (pBall != nullptr)
					pBall->KillMe();
			}
		}
	}

	if (cam != nullptr) {
		int x = (int)transform_.position_.x - cam->GetValueX();
		if (x > 960) {
			x = 960;
			cam->SetValueX((int)transform_.position_.x - x);
		}
		else if(x<320){
			x = 320;
			cam->SetValueX((int)transform_.position_.x - x);
		}
	}
	
}

void Player::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}
	DrawRectGraph(x - PLAYER_WIDTH/2.0, y-PLAYER_HEIGHT/2.0-CORRECT_BOTTOM, animFrame * PLAYER_WIDTH, animType * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT, hAnimImage, TRUE,!isRight);
	
	if (!isBallAlive && state!=DEAD) {
		int crrX = 0;
		if (isRight)
			crrX = 15;
		else
			crrX = -15;
		DrawRotaGraph(x + crrX, y + 20, 0.9, 0, hBallImg, TRUE, !isRight);
	}
	unsigned int Cr = GetColor(0, 0, 255);
	DrawCircle(10, 10, 10,Cr, isBallAlive);
}

void Player::SetPosition(int x, int y)
{
	transform_.position_.x = x;
	transform_.position_.y = y;
}

XMFLOAT3 Player::GetPosition()
{
	return transform_.position_;
}

bool Player::IsTouchBall(XMFLOAT3 pos)
{
	float cx = PLAYER_WIDTH / 2.0f;
	float cy = 50.0f;
	if ((pos.x <= transform_.position_.x + cx) && (pos.x >= transform_.position_.x - cx)) {
		if ((pos.y <= transform_.position_.y) && (pos.y >= transform_.position_.y - cy)) {
			return true;
		}
	}
	return false;
}
