#include "Camera.h"

Camera::Camera(GameObject* parent)
{
	valueX = 0;
	valueY = 0;
}

void Camera::Update()
{
	if (vibTimer > 0.0f) {
		vibTimer -= 0.1f;
		if (vibTimer <= 0.0f)
			vibTimer = 0.0f;
	}
	float val = sinf(vibTimer * 2.0f) * vibTimer * 2.0f;//時間が0.5に設定しているから1からスタートして盛るために2をかける
	if (val > vibTimer / 2.0f)
		val = vibTimer;
	else if (val < -vibTimer / 2.0f)
		val = -vibTimer;
	else
		val = 0;
	vibValue = vibParam * val;
	SetValueY(vibValue);
}

void Camera::VibrationY(float v)
{
	vibParam = v;
	//時間を測る時は減らす方が基本　増やした方がいい理由があるときは増やす
	vibTimer = 0.5f;//揺れ時間
}
