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

    /// <summary>
    /// カメラ縦振動を開始する
    /// </summary>
    /// <param name="v">衝撃の大きさ</param>
    void VibrationY(float v);
private:
    int valueX;
    int valueY;
    //VECTOR lookTarget;
    float vibTimer; //時間を測る変数
    float vibParam;//引数で指定された値
    float vibValue;//振動量、振動幅
};

