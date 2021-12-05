#include "stdafx.h"
#include "GameClear.h"
#include "ClearCounter.h"
#include "Title.h"
#include "Fade.h"
#include "Game.h"
#include "WarpCounter.h"

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

	m_pressButton.Init("Assets/sprite/button.dds", 1200, 600);
	m_pressButton.SetPosition(Vector3(-30.0f, -80.0f, 0.0f));


	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	clearCounter = FindGO<ClearCounter>("ClearCounter");
	warpCounter = FindGO<WarpCounter>("WarpCounter");

	return true;
}

//更新処理。
void GameClear::Update()
{
	warpCounter->warpCounter = 0;

	if (clearCounter->clearCounter == 1)
	{
		if (m_isWaitFadeout) {
			if (!m_fade->IsFade()) {
				NewGO<Game>(0, "game");
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

	if (clearCounter->clearCounter == 2)
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


		//α値を変化させる。
		if (m_isWaitFadeout)
		{
			m_alpha += g_gameTime->GetFrameDeltaTime() * 20.5f;
		}
		else
		{
			m_alpha += g_gameTime->GetFrameDeltaTime() * 1.2f;
		}

		m_pressButton.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));


		m_spriteRender.Update();
		m_pressButton.Update();

}

//描画処理。
void GameClear::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
	m_pressButton.Draw(rc);
}
