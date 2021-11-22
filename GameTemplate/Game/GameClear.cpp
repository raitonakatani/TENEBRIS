#include "stdafx.h"
#include "GameClear.h"
#include "Title.h"
#include "Fade.h"

GameClear::GameClear()
{

}

GameClear::~GameClear()
{

}

bool GameClear::Start()
{
	//ゲームクリアの画像を読み込む。
	m_spriteRender.Init("Assets/sprite/FRIEDEN.dds", 2100, 1100);
	m_spriteRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));

	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	return true;
}

//更新処理。
void GameClear::Update()
{
	if (m_isWaitFadeout) {
		if (!m_fade->IsFade()) {
			NewGO<Title>(0, "title");
			//自身を削除する。
			DeleteGO(this);
		}
	}
	else {
		//Aボタンを押したら。
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_isWaitFadeout = true;
			m_fade->StartFadeOut();
		}
	}
}

//描画処理。
void GameClear::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}
