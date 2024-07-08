#include "ItemBox.h"
#include"Camera.h"
#include"Ball.h"

namespace {
	const int BOX_WIDTH = 64;
	const int BOX_HEIGHT = 64;
}

ItemBox::ItemBox(GameObject* parent)
{
	hImage = LoadGraph("Assets/Objects/Crate.png");
	assert(hImage > 0);
	isAfterHit = false;
	itemInTheBox = 0;
}

ItemBox::~ItemBox()
{
	if (hImage > 0)
	{
		DeleteGraph(hImage);
	}
}

void ItemBox::Update()
{
}

void ItemBox::Draw()
{
	float x = (int)transform_.position_.x;
	float y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}
	DrawRotaGraph(x, y, 1.0, 0, hImage, TRUE);
}

void ItemBox::SetPosition(int x, int y)
{
	transform_.position_.x = x;
	transform_.position_.y = y;
}

bool ItemBox::CollideRectToRect(float x, float y, float w, float h)
{
	float myRectRight = transform_.position_.x + BOX_WIDTH / 2.0f;
	float myRectLeft = transform_.position_.x - BOX_WIDTH / 2.0f;
	float myRectTop = transform_.position_.y - BOX_HEIGHT / 2.0f;
	float myRectBottom = transform_.position_.y  + BOX_HEIGHT / 2.0f;
	if (((x - w / 2.0f > myRectLeft && x - w / 2.0f < myRectRight) || (myRectLeft > x - w / 2.0f && myRectLeft < x + w / 2.0f)) &&
		((y - h / 2.0f > myRectTop && y - h / 2.0f < myRectBottom) || (myRectTop > y - h / 2.0f && myRectTop < y + h / 2.0f))) {
		return true;
	}
	else {
		return false;
	}
	return false;
}

float ItemBox::CollisionRight(float x, float y)
{
	if (y > transform_.position_.y - BOX_HEIGHT / 2.0f && y < transform_.position_.y + BOX_HEIGHT / 2.0f) {
		if (x > transform_.position_.x - BOX_WIDTH / 2.0f&& x < transform_.position_.x + BOX_WIDTH / 2.0f) {
			return x - (transform_.position_.x - BOX_WIDTH / 2.0f);
		}
	}
	return 0.0f;
}

float ItemBox::CollisionDown(float x, float y)
{
	if (x > transform_.position_.x - BOX_WIDTH / 2.0f && x < transform_.position_.x + BOX_WIDTH / 2.0f) {
		if (y > transform_.position_.y - BOX_HEIGHT / 2.0f && y < transform_.position_.y + BOX_HEIGHT / 2.0f) {
			return y - (transform_.position_.y - BOX_HEIGHT / 2.0f);
		}
	}
	return 0.0f;
}

float ItemBox::CollisionLeft(float x, float y)
{
	if (y > transform_.position_.y - BOX_HEIGHT / 2.0f && y < transform_.position_.y + BOX_HEIGHT / 2.0f) {
		if (x > transform_.position_.x - BOX_WIDTH / 2.0f && x < transform_.position_.x + BOX_WIDTH / 2.0f) {
			return (transform_.position_.x + BOX_WIDTH / 2.0f) - x;
		}
	}
	return 0.0f;
}

float ItemBox::CollisionUp(float x, float y)
{
	if (x > transform_.position_.x - BOX_WIDTH / 2.0f && x < transform_.position_.x + BOX_WIDTH / 2.0f) {
		if (y > transform_.position_.y - BOX_HEIGHT / 2.0f && y < transform_.position_.y + BOX_HEIGHT / 2.0f) {
			return (transform_.position_.y + BOX_HEIGHT / 2.0f) - y;
		}
	}
	return 0.0f;
}
