#pragma once
#include "../Engine/GameObject.h"
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
  /// �J�������U�����J�n����
  /// </summary>
  /// <param name="v">�Ռ��̑傫��</param>
    void VibrationX(float v);
    /// <summary>
    /// �J�����c�U�����J�n����
    /// </summary>
    /// <param name="v">�Ռ��̑傫��</param>
    void VibrationY(float v);

    bool IsVibNow();
private:
    int valueX;
    int valueY;
    //VECTOR lookTarget;
    float vibTimerX; //���Ԃ𑪂�ϐ�
    float vibTimerY; //���Ԃ𑪂�ϐ�
    float vibParamX;//�����Ŏw�肳�ꂽ�l
    float vibParamY;//�����Ŏw�肳�ꂽ�l
    float vibValueX;//�U���ʁA�U����
    float vibValueY;//�U���ʁA�U����
    float firstValue;
    bool vibXnow;
    bool vibYnow;
    bool vibNow;
};

