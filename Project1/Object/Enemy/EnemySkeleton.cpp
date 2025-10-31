#include <DxLib.h>
#include "EnemySkeleton.h"

void EnemySkeleton::SetEnemyParam(void)
{
	// 敵の画像ファイル名
	imgFName += "Skeleton_Walk.png";
	// 敵の画像サイズ
	size = { 125, 53 };
	// 敵の移動速度
	speed = 5.0f;

	hp = hpMax =1;
}
