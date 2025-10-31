#include "EnemyMushroom.h"

void EnemyMushroom::SetEnemyParam(void)
{
	// 敵の画像ファイル名
	imgFName += "Mushroom_Run.png";
	// 敵の画像サイズ
	size = { 119, 39 };
	// 敵の移動速度
	speed = 3.0f;
	hp = hpMax = 5;	
}
