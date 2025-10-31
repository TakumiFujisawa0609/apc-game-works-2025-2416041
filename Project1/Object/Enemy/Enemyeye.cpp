#include "Enemyeye.h"

void Enemyeye::SetEnemyParam(void)
{
	// 敵の画像ファイル名
	imgFName += "Enemy_eye.png";
	// 敵の画像サイズ
	size = { 124, 36 };
	// 敵の移動速度
	speed = 5.0f;

	hp = hpMax =4;
}
