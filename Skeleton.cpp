#include "Skeleton.h"
#include"Field.h"
#include"Camera.h"
#include"ItemBox.h"

namespace {
	const int WALK_MAXFRAME{ 15 };
	const int DEAD_MAXFRAME{ 5 };

	const float SKELETON_WIDTH = 128.0f;
	const float SKELETON_HEIGHT = 128.0f;

	const float MOVE_SPEED = 1.5f;
	const float GRAVITY = 9.8f / 60.0f;

	const float CORRECT_WIDTH = 50.0f;
	const float CORRECT_BOTTOM = 1.0f;
	const float CORRECT_TOP = 35.0f;
	static const int WINDOW_WIDTH = 1280;
	static const int WINDOW_HEIGHT = 720;
	const float COLLIDE_WIDTH = 45.0f;
	const float COLLIDE_HEIGHT = 40.0f;
}

Skeleton::Skeleton(GameObject* parent):Enemy(parent)
{
	hAnimImg = LoadGraph("Assets/Enemy/Skeleton.png");
	assert(hAnimImg > 0);

	animType = 0;
	animFrame = 0;
	frameCounter = 0;

	isRight = false;
	onGround = true;
	jumpSpeed = 0.0f;
	cdTimer = 0.0f;

	state = NORMAL;
}

Skeleton::~Skeleton()
{
	if (hAnimImg > 0) {
		DeleteGraph(hAnimImg);
	}
}

void Skeleton::Update()
{
	Field* pField = GetParent()->FindGameObject<Field>();
	std::list<ItemBox*> pIBoxs = GetParent()->FindGameObjects<ItemBox>();

	float x = (int)transform_.position_.x;
	float y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}
	if (x > WINDOW_WIDTH + SKELETON_WIDTH)
		return;
	else if (x < -SKELETON_WIDTH) {
		return;
	}
	if (y < 0 || y > WINDOW_HEIGHT)
		KillMe();

	if (state == DEAD) {
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

	if (pField != nullptr) {
		float cx = SKELETON_WIDTH / 2.0f - CORRECT_WIDTH;
		float cy = SKELETON_HEIGHT / 2.0f;

		float pushTright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y + cy - CORRECT_TOP - 1.0f);
		float pushBright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y - cy + CORRECT_BOTTOM + 1.0f);
		float pushRight = max(pushBright, pushTright);
		if (pushRight > 0.0f) {
			isRight = false;
			transform_.position_.x -= pushRight - 1.0f;
		}

		float pushTleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y + cy - CORRECT_TOP - 1.0f);
		float pushBleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y - cy + CORRECT_BOTTOM + 1.0f);
		float pushLeft = max(pushBleft, pushTleft);
		if (pushLeft > 0.0f) {
			isRight = true;
			transform_.position_.x += pushLeft - 1.0f;
		}

		jumpSpeed += GRAVITY;
		transform_.position_.y += jumpSpeed;

		int pushRbottom = pField->CollisionDown(transform_.position_.x + cx - 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushLbottom = pField->CollisionDown(transform_.position_.x - cx + 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushBottom = max(pushRbottom, pushLbottom);
		if (pushBottom > 0.0f) {
			transform_.position_.y -= pushBottom - 1.0f;
			jumpSpeed = 0.0f;
		}

		int pushRtop = pField->CollisionUp(transform_.position_.x + cx - 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushLtop = pField->CollisionUp(transform_.position_.x - cx + 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushTop = max(pushRtop, pushLtop);
		if (pushTop > 0.0f) {
			transform_.position_.y += pushTop - 1.0f;
			jumpSpeed = 0.0f;
		}
	}

	for (ItemBox* pIBox : pIBoxs) {
		float cx = SKELETON_WIDTH / 2.0f - CORRECT_WIDTH;
		float cy = SKELETON_HEIGHT / 2.0f;

		float pushTright = pIBox->CollisionRight(transform_.position_.x + cx, transform_.position_.y + cy - CORRECT_TOP - 1.0f);
		float pushBright = pIBox->CollisionRight(transform_.position_.x + cx, transform_.position_.y - cy + CORRECT_BOTTOM + 1.0f);
		float pushRight = max(pushBright, pushTright);
		if (pushRight > 0.0f) {
			isRight = false;
			transform_.position_.x -= pushRight - 1.0f;
		}

		float pushTleft = pIBox->CollisionLeft(transform_.position_.x - cx, transform_.position_.y + cy - CORRECT_TOP - 1.0f);
		float pushBleft = pIBox->CollisionLeft(transform_.position_.x - cx, transform_.position_.y - cy + CORRECT_BOTTOM + 1.0f);
		float pushLeft = max(pushBleft, pushTleft);
		if (pushLeft > 0.0f) {
			isRight = true;
			transform_.position_.x += pushLeft - 1.0f;
		}

		int pushRbottom = pIBox->CollisionDown(transform_.position_.x + cx - 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushLbottom = pIBox->CollisionDown(transform_.position_.x - cx + 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushBottom = max(pushRbottom, pushLbottom);
		if (pushBottom > 0.0f) {
			transform_.position_.y -= pushBottom - 1.0f;
			jumpSpeed = 0.0f;
		}

		int pushRtop = pIBox->CollisionUp(transform_.position_.x + cx - 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushLtop = pIBox->CollisionUp(transform_.position_.x - cx + 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushTop = max(pushRtop, pushLtop);
		if (pushTop > 0.0f) {
			transform_.position_.y += pushTop - 1.0f;
			jumpSpeed = 0.0f;
		}
	}
}

void Skeleton::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}
	DrawRectGraph(x - SKELETON_WIDTH / 2.0, y - SKELETON_HEIGHT / 2.0 - CORRECT_BOTTOM, animFrame * SKELETON_WIDTH, animType * SKELETON_HEIGHT, SKELETON_WIDTH, SKELETON_HEIGHT, hAnimImg, TRUE, !isRight);
}

void Skeleton::SetPosition(int x, int y)
{
	transform_.position_.x = x;
	transform_.position_.y = y;
}

bool Skeleton::CollideRectToRect(float x, float y, float w, float h)
{
	if (state != DEAD) {
		float myRectRight = transform_.position_.x + SKELETON_WIDTH / 2.0f - COLLIDE_WIDTH;
		float myRectLeft = transform_.position_.x - SKELETON_WIDTH / 2.0f + COLLIDE_WIDTH;
		float myRectTop = transform_.position_.y - SKELETON_HEIGHT / 2.0f + COLLIDE_HEIGHT;
		float myRectBottom = transform_.position_.y + SKELETON_HEIGHT / 2.0f - CORRECT_BOTTOM;

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

void Skeleton::KillEnemy()
{
	state = DEAD;
	animType = 1;
	animFrame = 0;
	frameCounter = 0;
}
