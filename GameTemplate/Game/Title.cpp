#include "stdafx.h"
#include "Title.h"

#include "Game.h"
#include "Fade.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

Title::Title()
{

}

Title::~Title()
{
	//BGMを削除する。
	DeleteGO(m_titleBGM);
}

bool Title::Start()
{
	//画像を読み込む。
	m_spriteRender.Init("Assets/sprite/TENEBRIS.dds", 2100, 1100);
	m_spriteRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_pressButton.Init("Assets/sprite/button.dds", 1200, 600);
	m_pressButton.SetPosition(Vector3(-30.0f, -80.0f, 0.0f));


	//音を読み込む。
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/2titleBGM.wav");
	//BGMを再生する。
	m_titleBGM = NewGO<SoundSource>(0);
	m_titleBGM->Init(2);
	m_titleBGM->Play(true);
	m_titleBGM->SetVolume(0.3f);


	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();
	return true;
}

void Title::Update()
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

			//音を読み込む。
			g_soundEngine->ResistWaveFileBank(0, "Assets/sound/0titlebutton.wav");
			//効果音を再生する。
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(0);
			se->Play(false);
			se->SetVolume(0.5f);

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

	//画像の更新。
	m_spriteRender.Update();
	m_pressButton.Update();
}

void Title::Render(RenderContext& rc)
{
	//画像の描画。
	m_spriteRender.Draw(rc);
	m_pressButton.Draw(rc);
}