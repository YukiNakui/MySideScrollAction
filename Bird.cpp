#include "Bird.h"
#include"Field.h"
#include"Camera.h"
#include"ItemBox.h"
#include"Player.h"

namespace {
	const int WALK_MAXFRAME{ 15 };
	const int DEAD_MAXFRAME{ 5 };

	const float BIRD_WIDTH = 128.0f;
	const float BIRD_HEIGHT = 128.0f;

	const float MOVE_SPEED = 1.5f;
	const float GRAVITY = 9.8f / 60.0f;

	const float CORRECT_WIDTH = 35.0f;
	const float CORRECT_BOTTOM = 1.0f;
	const float CORRECT_TOP = 35.0f;
	static const int WINDOW_WIDTH = 1280;
	static const int WINDOW_HEIGHT = 720;
	const float COLLIDE_WIDTH = 45.0f;
	const float COLLIDE_HEIGHT = 40.0f;
	const float CAN_ATTACK_LENGTH = 64.0f;
}

Bird::Bird(GameObject* parent):Enemy(parent)
{
	hAnimImg = LoadGraph("Assets/Enemy/Bird.png");
	assert(hAnimImg > 0);

	animType = 0;
	animFrame = 0;
	frameCounter = 0;

	isRight = false;
	onGround = true;
	jumpSpeed = 0.0f;
	cdTimer = 0.0f;

	targetPos = { 0.0f,0.0f,0.0f };

	state = NORMAL;
}

Bird::~Bird()
{
	if (hAnimImg > 0) {
		DeleteGraph(hAnimImg);
	}
}

void Bird::Update()
{
	Player* pPlayer = GetParent()->FindGameObject<Player>();
	Field* pField = GetParent()->FindGameObject<Field>();
	std::list<ItemBox*> pIBoxs = GetParent()->FindGameObjects<ItemBox>();

	float x = (int)transform_.position_.x;
	float y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}
	if (x > WINDOW_WIDTH + BIRD_WIDTH)
		return;
	else if (x < -BIRD_WIDTH) {
		return;
	}
	if (y < 0 || y > WINDOW_HEIGHT)
		KillMe();

	if (state == DEAD) {
		jumpSpeed += GRAVITY;
		transform_.position_.y += jumpSpeed;
		if (animFrame < DEAD_MAXFRAME - 1) {
			frameCounter++;
			if (frameCounter > 10) {
				animFrame = (animFrame + 1) % DEAD_MAXFRAME;
				frameCounter = 0;
			}
		}
		else
			cdTimer++;
		if (cdTimer > 100.0f)
			KillMe();
	}

	if (state == NORMAL) {
		frameCounter++;
		if (frameCounter > 6) {
			animFrame = (animFrame + 1) % WALK_MAXFRAME;
			frameCounter = 0;
		}
		if (isRight) {
			transform_.position_.x += MOVE_SPEED;
		}
		else {
			transform_.position_.x -= MOVE_SPEED;
		}
	}

	if (pField != nullptr) {
		float cx = BIRD_WIDTH / 2.0f - CORRECT_WIDTH;
		float cy = BIRD_HEIGHT / 2.0f;

		float pushTright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y + cy - CORRECT_TOP - 1.0f);
		float pushBright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y - cy + CORRECT_BOTTOM + 1.0f);
		float pushRight = max(pushBright, pushTright);//‰E‘¤‚Ì“ª‚Æ‘«Œ³‚Å“–‚½‚è”»’è
		if (pushRight > 0.0f) {
			isRight = false;
			transform_.position_.x -= pushRight - 1.0f;
		}

		float pushTleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y + cy - CORRECT_TOP - 1.0f);
		float pushBleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y - cy + CORRECT_BOTTOM + 1.0f);
		float pushLeft = max(pushBleft, pushTleft);//¶‘¤‚Ì“ª‚Æ‘«Œ³‚Å“–‚½‚è”»’è
		if (pushLeft > 0.0f) {
			isRight = true;
			transform_.position_.x += pushLeft - 1.0f;
		}

		int pushRbottom = pField->CollisionDown(transform_.position_.x + cx - 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushLbottom = pField->CollisionDown(transform_.position_.x - cx + 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushBottom = max(pushRbottom, pushLbottom);//2‚Â‚Ì‘«Œ³‚Ì‚ß‚èž‚Ý‚Ì‘å‚«‚¢‚Ù‚¤
		if (pushBottom > 0.0f) {
			transform_.position_.y -= pushBottom - 1.0f;
			jumpSpeed = 0.0f;
		}

		int pushRtop = pField->CollisionUp(transform_.position_.x + cx - 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushLtop = pField->CollisionUp(transform_.position_.x - cx + 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushTop = max(pushRtop, pushLtop);//2‚Â‚Ì“ª‚Ì‚ß‚èž‚Ý‚Ì‘å‚«‚¢‚Ù‚¤
		if (pushTop > 0.0f) {
			transform_.position_.y += pushTop - 1.0f;
			jumpSpeed = 0.0f;
		}
	}

	for (ItemBox* pIBox : pIBoxs) {
		float cx = BIRD_WIDTH / 2.0f - CORRECT_WIDTH;
		float cy = BIRD_HEIGHT / 2.0f;

		float pushTright = pIBox->CollisionRight(transform_.position_.x + cx, transform_.position_.y + cy - CORRECT_TOP - 1.0f);
		float pushBright = pIBox->CollisionRight(transform_.position_.x + cx, transform_.position_.y - cy + CORRECT_BOTTOM + 1.0f);
		float pushRight = max(pushBright, pushTright);//‰E‘¤‚Ì“ª‚Æ‘«Œ³‚Å“–‚½‚è”»’è
		if (pushRight > 0.0f) {
			isRight = false;
			transform_.position_.x -= pushRight - 1.0f;
		}

		float pushTleft = pIBox->CollisionLeft(transform_.position_.x - cx, transform_.position_.y + cy - CORRECT_TOP - 1.0f);
		float pushBleft = pIBox->CollisionLeft(transform_.position_.x - cx, transform_.position_.y - cy + CORRECT_BOTTOM + 1.0f);
		float pushLeft = max(pushBleft, pushTleft);//¶‘¤‚Ì“ª‚Æ‘«Œ³‚Å“–‚½‚è”»’è
		if (pushLeft > 0.0f) {
			isRight = true;
			transform_.position_.x += pushLeft - 1.0f;
		}

		int pushRbottom = pIBox->CollisionDown(transform_.position_.x + cx - 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushLbottom = pIBox->CollisionDown(transform_.position_.x - cx + 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushBottom = max(pushRbottom, pushLbottom);//2‚Â‚Ì‘«Œ³‚Ì‚ß‚èž‚Ý‚Ì‘å‚«‚¢‚Ù‚¤
		if (pushBottom > 0.0f) {
			transform_.position_.y -= pushBottom - 1.0f;
			jumpSpeed = 0.0f;
		}

		int pushRtop = pIBox->CollisionUp(transform_.position_.x + cx - 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushLtop = pIBox->CollisionUp(transform_.position_.x - cx + 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushTop = max(pushRtop, pushLtop);//2‚Â‚Ì“ª‚Ì‚ß‚èž‚Ý‚Ì‘å‚«‚¢‚Ù‚¤
		if (pushTop > 0.0f) {
			transform_.position_.y += pushTop - 1.0f;
			jumpSpeed = 0.0f;
		}
	}
}

void Bird::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}
	DrawRectGraph(x - BIRD_WIDTH / 2.0, y - BIRD_HEIGHT / 2.0 - CORRECT_BOTTOM, animFrame * BIRD_WIDTH, animType * BIRD_HEIGHT, BIRD_WIDTH, BIRD_HEIGHT, hAnimImg, TRUE, !isRight);
}

void Bird::SetPosition(int x, int y)
{
	transform_.position_.x = x;
	transform_.position_.y = y;
}

bool Bird::CollideRectToRect(float x, float y, float w, float h)
{
	if (state != DEAD) {
		float myRectRight = transform_.position_.x + BIRD_WIDTH / 2.0f - COLLIDE_WIDTH;
		float myRectLeft = transform_.position_.x - BIRD_WIDTH / 2.0f + COLLIDE_WIDTH;
		float myRectTop = transform_.position_.y - BIRD_HEIGHT / 2.0f + COLLIDE_HEIGHT;
		float myRectBottom = transform_.position_.y + BIRD_HEIGHT / 2.0f - CORRECT_BOTTOM;

		if ((x - w / 2.0f < myRectRight && x + w / 2.0f > myRectLeft) &&
			(y - h / 2.0f < myRectBottom && y + h / 2.0f > myRectTop)) {
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

void Bird::KillEnemy()
{
	state = DEAD;
	animType = 1;
	animFrame = 0;
	frameCounter = 0;
}

bool Bird::IsCanAttack(XMFLOAT3 pos)
{
	float len = sqrt((pos.x - transform_.position_.x) * (pos.x - transform_.position_.x))+ 
				sqrt((pos.y - transform_.position_.y) * (pos.y - transform_.position_.y));
	if (len < CAN_ATTACK_LENGTH) {
		if (pos.y > transform_.position_.y) {
			if (isRight) {
				if (pos.x <= transform_.position_.x + 50.0f && pos.x > transform_.position_.x){
					targetPos = pos;
					return true;
				}
			}
			else {
				if (pos.x >= transform_.position_.x - 50.0f && pos.x < transform_.position_.x) {
					targetPos = pos;
					return true;
				}
			}
		}
	}
	return false;
}
