#include  <DxLib.h>
#include "Enemy_Mushroom.h"

void Enemy_Mushroom::SetEnemyParam(void)
{
	// 敵の画像ファイル名
	imgFName += "Mushroom_Run.png";
	// 敵の画像サイズ
	size = { 64, 64 };
	// 敵の移動速度
	speed = 1.0f;
	hp = hpMax = 4;
}
