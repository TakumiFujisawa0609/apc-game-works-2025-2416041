#pragma once
#include <array>
#include "../Common/Vector2.h"

// DxLib を使う前提のスロットクラス
class Slot
{
public:
    Slot();
    ~Slot();

    // 最初に一回呼ぶ
    void Init();
    // 毎フレーム呼ぶ
    void Update();
    // 毎フレーム呼ぶ（描画）
    void Draw(const Vector2& screenPos);
    // 解放
    void Relese();

    // スロットを回し始める
    void Start();

    // スロットが回転中か？
    bool IsSpinning() const;
    // 結果待ち状態か？（止まって結果が出ている）
    bool IsResult() const;

    // 出目を取得（0?SYMBOL_COUNT-1 の数字）
    std::array<int, 3> GetResult() const;

    // 当たり系
    bool IsAnyHit() const;      // 小当たり or 大当たり
    bool IsSmallHit() const;    // 小当たり
    bool IsBigHit() const;      // 大当たり

    // 確変モード中か？
    bool IsInKakuhen() const;

    enum class HitType
    {
        None,
        Small,
        Big,
    };
    HitType FetchHitType();

private:
    // スロットの状態
    enum class State
    {
        Idle,      // 待機（Start待ち）
        Spinning,  // 回転中
        Result     // 止まって結果表示中
    };

    // 通常 or 確変
    enum class Mode
    {
		Normal,    // 通常
		Kakuhen,   // 確変
    };

    // 内部処理
    void DecideResult();  // 最終的な出目を決める
    void JudgeHit();      // 当たり判定を行う

    // 定数
    static constexpr int REEL_COUNT = 3; // リール数
    static constexpr int SYMBOL_COUNT = 4; // シンボルの種類数（0?3）
    static constexpr int SPIN_DURATION = 60; // 何フレーム回すか

    // 描画用（シンボル画像サイズと間隔）
    static constexpr int SYMBOL_W = 64;  // 画像幅（実際の画像に合わせて調整）
    static constexpr int SYMBOL_H = 64;  // 画像高さ
    static constexpr int SYMBOL_MARGIN = 8; // リール間の隙間


    State state_;
    Mode  mode_;

    std::array<int, REEL_COUNT> result_;   // 出目（各リールのシンボル番号）

    int spinTimer_;    // 何フレーム回っているか

    bool isSmallHit_;//小当たり
	bool isBigHit_;//大当たり

    bool isVisible_;// 表示中か？
    int  resultTimer_;// 結果表示用タイマー
    static constexpr int RESULT_DISPLAY_TIME = 180; // 約3秒

    //確変の残りゲーム数
    int kakuhenGamesLeft_;
    static constexpr int KAKUHEN_GAME_MAX = 3;

    // シンボルごとの画像ハンドル
    int symbolImg_[SYMBOL_COUNT];

    HitType lastHitType_;
    bool hitUsed_;
};

