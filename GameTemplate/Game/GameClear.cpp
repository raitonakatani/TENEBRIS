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
	//�Q�[���N���A�̉摜��ǂݍ��ށB
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

//�X�V�����B
void GameClear::Update()
{
	warpCounter->warpCounter = 0;

	if (clearCounter->clearCounter == 1)
	{
		if (m_isWaitFadeout) {
			if (!m_fade->IsFade()) {
				NewGO<Game>(0, "game");
				//���g���폜����B
				DeleteGO(this);
			}
		}
		else {
			//A�{�^������������B
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
				//���g���폜����B
				DeleteGO(this);
			}
		}
		else {
			//A�{�^������������B
			if (g_pad[0]->IsTrigger(enButtonA)) {
				m_isWaitFadeout = true;
				m_fade->StartFadeOut();
			}
		}

	}


		//���l��ω�������B
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

//�`�揈���B
void GameClear::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
	m_pressButton.Draw(rc);
}
