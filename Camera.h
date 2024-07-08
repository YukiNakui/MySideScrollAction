#pragma once
#include "Engine/GameObject.h"
class Camera :
    public GameObject
{
public:
    Camera(GameObject* parent);
    ~Camera() {}

    void Update();

    void SetValueX(int v) { valueX = v; };
    int GetValueX() { return valueX; }
    void SetValueY(int v) { valueY = v; };
    int GetValueY() { return valueY; }

    void VibrationX(float v);
    void VibrationY(float v);
private:
    int valueX;
    int valueY;
    float vibTimerX;
    float vibTimerY;
    float vibParamX;
    float vibParamY;
    float vibValueX;
    float vibValueY;
    bool vibXnow;
    bool vibYnow;
};

