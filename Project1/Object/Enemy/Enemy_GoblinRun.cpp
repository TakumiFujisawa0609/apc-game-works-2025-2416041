#include  <DxLib.h>
#include "Enemy_GoblinRun.h"

void Enemy_GoblinRun::SetEnemyParam(void)
{
	// 敵の画像ファイル名
	imgFName += "GoblinRun.png";
	// 敵の画像サイズ
	size = { 111, 35 };
	// 敵の移動速度
	speed = 2.5f;
	hp = hpMax = 1;
}
