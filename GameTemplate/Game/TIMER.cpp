#include "stdafx.h"
#include "TIMER.h"

TIMER::TIMER()
{

}

TIMER::~TIMER()
{

}

bool TIMER::Start()
{
	return true;
}

void TIMER::Update()
{

	wchar_t wcsbuf[256];
	//�o�ߎ��Ԃ̎擾�B
	m_timer += g_gameTime->GetFrameDeltaTime();
	swprintf_s(wcsbuf, 256, L"%d�b", int(m_timer));
	//�e�L�X�g�̕\��
	fontRender2.SetText(wcsbuf);
	//�\��������W��ݒ肷��B
	fontRender2.SetPosition({ -600.0f,350.0f,0.0f });
	//�����̑傫����ς���B
	fontRender2.SetScale(2.0f);
	//�\������F��ݒ肷��B
	fontRender2.SetColor(g_vec4White);
}

void TIMER::Render(RenderContext& rc)
{
	fontRender2.Draw(rc);
}