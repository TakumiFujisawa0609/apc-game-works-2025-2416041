#include "Enemy_Goblin.h"

void Enemy_Goblin::SetEnemyParam(void)
{
	// 敵の画像ファイル名
	imgFName += "Goblin_Run.png";
	// 敵の画像サイズ
	size = { 80, 80 };
	// 敵の移動速度
	speed = 3.0f;
	hp = hpMax = 8;
}
