#pragma once
#include "../StDefineData.h"
class Fader
{
public:
	static constexpr float FADE_SPEED_ALPHA = 2; // フェードの速さ

	Fader(void); // コンストラクタ
	~Fader(void); // デストラクタ

	bool SystemInit(void); // 初期化処理(最初の１回のみ実行)
	void GameInit(void); // ゲーム起動・再開時に必ず呼び出す処理
	void Update(void); // 更新処理
	void Draw(void); // 描画処理
	bool Release(void); // 解放処理(最後の１回のみ実行)

	// ゲッター・セッター関数
	E_FADE_STAT_ID GetNowState(void) { return stat; }
	bool IsEnd(void) { return endFlg; }
	void SetFade(E_FADE_STAT_ID id);
private:
	E_FADE_STAT_ID stat; // ステータス
	float alpha; // アルファ値用の変数
	bool endFlg; // フェード処理終了フラグ(true=未実行/false=実行中)
};

