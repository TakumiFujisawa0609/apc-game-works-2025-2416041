#include <sstream>
#include "AsoUtility.h"
/*
 *----------------------------------------------------
 * 文字列の分割
 * Input:
 * std:string& line : 分割する文字列
 * char delimiter : 区切りとして使用する文字
 * Output:
 * 取出した文字列の配列テーブル
 *----------------------------------------------------
 */
	std::vector<std::string> AsoUtility::Split(std::string& line, char delimiter)
{
	// 動的配列の返り値を準備
	std::vector<std::string> result;
	// 文字操作用のクラスに置き換える
	std::istringstream stream(line);
	std::string field;
	// streamを順に読み込み、
	// 第３引数で指定された文字までを第２引数に返す
	while (getline(stream, field, delimiter)) {
		// 動的配列に追加
		result.push_back(field);
	}
	return result;
}
int AsoUtility::Round(float val)
{
	return static_cast<int>(round(val));
}

/*
 *----------------------------------------------------
 * 四捨五入
 * Input:
 * 　　　flot val :int型に四捨五入するfloat値
 * Output:
 * 　　int : 四捨五入した値
 *----------------------------------------------------
 */
Vector2 AsoUtility::Round(Vector2F val)
{
	Vector2 rv;
	rv.x = Round(val.x);
	rv.y = Round(val.y);

	return rv;
}
