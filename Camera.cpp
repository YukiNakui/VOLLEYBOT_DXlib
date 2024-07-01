#include "Camera.h"

Camera::Camera(GameObject* parent)
{
	valueX = 0;
	valueY = 0;
	vibXnow = false;
	vibYnow = false;
}

void Camera::Update()
{
	if (vibXnow) {
		if (vibTimerX > 0.0f) {
			vibTimerX -= 0.1f;
			if (vibTimerX <= 0.0f) {
				vibTimerX = 0.0f;
				vibXnow = false;
			}
		}
		float val = sinf(vibTimerX * 2.0f) * vibTimerX * 2.0f;//時間が0.5に設定しているから1からスタートして盛るために2をかける
		if (val > vibTimerX / 2.0f)
			val = vibTimerX;
		else if (val < -vibTimerX / 2.0f)
			val = -vibTimerX;
		else
			val = 0;
		vibValueX = vibParamX * val;
		SetValueX(vibValueX);
	}

	if (vibYnow) {
		if (vibTimerY > 0.0f) {
			vibTimerY -= 0.1f;
			if (vibTimerY <= 0.0f) {
				vibTimerY = 0.0f;
				vibYnow = false;
			}
		}
		float val = sinf(vibTimerY * 2.0f) * vibTimerY * 2.0f;//時間が0.5に設定しているから1からスタートして盛るために2をかける
		if (val > vibTimerY / 2.0f)
			val = vibTimerY;
		else if (val < -vibTimerY / 2.0f)
			val = -vibTimerY;
		else
			val = 0;
		vibValueY = vibParamY * val;
		SetValueY(vibValueY);
	}
}

void Camera::VibrationX(float v)
{
	vibParamX = v;
	//時間を測る時は減らす方が基本　増やした方がいい理由があるときは増やす
	vibTimerX = 0.5f;//揺れ時間
	vibXnow = true;
}

void Camera::VibrationY(float v)
{
	vibParamY = v;
	//時間を測る時は減らす方が基本　増やした方がいい理由があるときは増やす
	vibTimerY = 0.5f;//揺れ時間
	vibYnow = true;
}
