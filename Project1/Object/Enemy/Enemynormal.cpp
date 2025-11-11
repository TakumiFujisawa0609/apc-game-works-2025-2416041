#include <DxLib.h>
#include "Enemynormal.h"

void Enemynormal::SetEnemyParam(void)
{
	// 敵の画像ファイル名
	imgFName += "FLYING.png";
	// 敵の画像サイズ
	size = { 81, 71 };
	// 敵の移動速度
	speed =3.0f;

	hp = hpMax = 1;
}
