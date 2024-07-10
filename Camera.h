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
  /// カメラ横振動を開始する
  /// </summary>
  /// <param name="v">衝撃の大きさ</param>
    void VibrationX(float v);
    /// <summary>
    /// カメラ縦振動を開始する
    /// </summary>
    /// <param name="v">衝撃の大きさ</param>
    void VibrationY(float v);

    bool IsVibNow();
private:
    int valueX;
    int valueY;
    //VECTOR lookTarget;
    float vibTimerX; //時間を測る変数
    float vibTimerY; //時間を測る変数
    float vibParamX;//引数で指定された値
    float vibParamY;//引数で指定された値
    float vibValueX;//振動量、振動幅
    float vibValueY;//振動量、振動幅
    bool vibXnow;
    bool vibYnow;
    bool vibNow;
};

