#include "BackGround.h"
#include "Camera.h"

namespace {
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;
}

BackGround::BackGround(GameObject* scene)
{
    hImage = LoadGraph("Assets/BackGround/BG.png");
    assert(hImage > 0);
    transform_.position_.x = 0;
    transform_.position_.y = SCREEN_HEIGHT / 2.0;
}

BackGround::~BackGround()
{
    if (hImage > 0)
    {
        DeleteGraph(hImage);
    }
}

void BackGround::Update()
{
}

void BackGround::Draw()
{
    int x = (int)transform_.position_.x;
    int y = (int)transform_.position_.y;
    Camera* cam = GetParent()->FindGameObject<Camera>();
    if (cam != nullptr) {
        x -= cam->GetValueX()/2;
        y -= cam->GetValueY()/2;
    }

    while (x < 0) {
        x += SCREEN_WIDTH;
    }
    while (x >= SCREEN_WIDTH) {
        x -= SCREEN_WIDTH;
    }

    DrawRotaGraph(x, y, 1.0f, 0, hImage, TRUE);
    DrawRotaGraph(x + SCREEN_WIDTH, y, 1.0f, 0, hImage, TRUE);
    DrawRotaGraph(x - SCREEN_WIDTH, y, 1.0f, 0, hImage, TRUE);
}
