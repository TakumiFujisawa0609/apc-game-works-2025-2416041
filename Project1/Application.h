#pragma once
#include <string>
class Application
{
public:

	// スクリーンサイズ
	static constexpr int SCREEN_SIZE_X = 1024;
	static constexpr int SCREEN_SIZE_Y = 640;

	static constexpr float FRAME_RATE = 1000.0f / 60.0f;

	int currentTime = 0;
	int lastFrameTime = 0;

	int frameCnt = 0;
	int updateFrameRateTime = 0;

	float frameRate = 0.0f;

	// データパス関連
	//-------------------------------------------
	static const std::string PATH_DATA;
	static const std::string PATH_OBJECT;
	static const std::string PATH_SCENE;
	static const std::string PATH_STAGE;
	static const std::string PATH_UI;
	static const std::string PATH_IMAGE;
	static const std::string PATH_SOUND;
	static const std::string PATH_MOVIE; // 動画のパス（必要なら追加）
	//static const std::string PATH_EFFECT;
	//static const std::string PATH_MAP_DATA;
	//------------------------------------------

	// インスタンスを明示的に生成
	static void CreateInstance(void);

	// インスタンスの取得
	static Application& GetInstance(void);

	// 初期化
	void Init(void);

	// ゲームループの開始
	void Run(void);

	// リソースの破棄
	void Destroy(void);

	// 初期化成功／失敗の判定
	bool IsInitFail(void) const;

	// 解放成功／失敗の判定
	bool IsReleaseFail(void) const;

	void CalcFrameRate(void);					// フレームレート計算
	void DrawFrameRate();

	void DrawPauseOverlay(void);

private:

	bool isPauseMenuActive_ = false; // ポーズメニュー中かどうか
	bool prevEscapePressed_ = false; // 前フレームのESC入力

	// 静的インスタンス
	static Application* instance_;

	// 初期化失敗
	bool isInitFail_;

	// 解放失敗
	bool isReleaseFail_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	Application(void);

	// コピーコンストラクタも同様
	Application(const Application& instance) = default;

	// デストラクタも同様
	~Application(void) = default;
};

