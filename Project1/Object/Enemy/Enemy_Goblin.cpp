#include "Enemy_Goblin.h"

void Enemy_Goblin::SetEnemyParam(void)
{
	// 敵の画像ファイル名
	imgFName += "GoblinRun.png";
	// 敵の画像サイズ
	size = { 122, 39 };
	// 敵の移動速度
	speed = 4.0f;
	hp = hpMax = 1;
}
