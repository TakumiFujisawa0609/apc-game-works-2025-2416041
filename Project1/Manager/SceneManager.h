#pragma once

#include <chrono>

class SceneBase;

class StageBase;

class Fader;

class Camera;

class Application; // ←もし未定義であれば追加しておく

class SceneManager
{
public:

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		SELECT,
		STAGE01,
		STAGE02,
		STAGE03,
		GAMEOVER,
		GAMECLEAR,
		END,
		MAX,
	};

	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SceneManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// リソースの破棄
	void Destroy(void);

	// 状態遷移
	void ChangeScene(SCENE_ID nextId);

	// シーンIDの取得
	SCENE_ID GetSceneID(void);

	// デルタタイムの取得
	float GetDeltaTime(void) const;

	// ★ 追加：前ステージIDをセット
	void SetPrevStageID(SCENE_ID id);  // ← これが必要！
	SCENE_ID GetPrevStageID() const { return prevStageId_; }

private:

	// 静的インスタンス
	static SceneManager* instance_;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;
	SCENE_ID prevStageId_; // ★ 追加：前のステージを保持する変数

	// フェード
	Fader* fader_;

	// カメラ
	Camera* camera_;

	// 各種シーン
	SceneBase* scene_;

	StageBase* stage_;

	Application* application_;

	// シーン遷移中判定
	bool isSceneChanging_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;

	// デフォルトコンストラクタをprivateに
	SceneManager(void);
	SceneManager(const SceneManager& instance) = default;
	~SceneManager(void) = default;

	void ResetDeltaTime(void);
	void DoChangeScene(SCENE_ID sceneId);
	void Fade(void);
};