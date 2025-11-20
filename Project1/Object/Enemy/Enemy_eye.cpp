#include <DxLib.h>

#include"Enemy_eye.h"

void Eenmy_eye::SetEnemyParam(void)
{
	// 敵の画像ファイル名
	imgFName += "Enemy_eye.png";
	// 敵の画像サイズ
	size = { 124, 35 };
	// 敵の移動速度
	speed = 1.0f;
	hp = hpMax = 2    ;
}
