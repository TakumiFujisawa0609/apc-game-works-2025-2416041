#include  <DxLib.h>
#include "Enemy_Skeleton.h"

void Enemy_Skeleton::SetEnemyParam(void)
{
	// 敵の画像ファイル名
	imgFName += "Skeleton_Walk.png";
	// 敵の画像サイズ
	size = { 95, 50 };
	// 敵の移動速度
	speed = 3.0f;
	hp = hpMax = 1;
}
