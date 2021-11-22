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
	//経過時間の取得。
	m_timer += g_gameTime->GetFrameDeltaTime();
	swprintf_s(wcsbuf, 256, L"%d秒", int(m_timer));
	//テキストの表示
	fontRender2.SetText(wcsbuf);
	//表示する座標を設定する。
	fontRender2.SetPosition({ -600.0f,350.0f,0.0f });
	//文字の大きさを変える。
	fontRender2.SetScale(2.0f);
	//表示する色を設定する。
	fontRender2.SetColor(g_vec4White);
}

void TIMER::Render(RenderContext& rc)
{
	fontRender2.Draw(rc);
}