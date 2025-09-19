#pragma once
#include "SceneBase.h"
class SceneManager;

	class TitleScene : public SceneBase
	{

	public:

		// コンストラクタ
		TitleScene(void);

		// デストラクタ
		~TitleScene(void);

		void Init(void) ;
		void Update(void);
		void Draw(void) ;
		void Release(void) ;

	private:

		// 画像
		int imgTitle_;

		bool isGameOver_;		//ゲームオーバー
		bool isGameClear_;		//ゲームクリア
	};


