#pragma once
#include "Engine/GameObject.h"
class Enemy :
    public GameObject
{
public:
	Enemy(GameObject* scene) {}
	virtual ~Enemy() {}
	virtual bool CollideRectToRect(float x, float y, float w, float h) = 0;
	virtual void KillEnemy() = 0;
};

