#include "stdafx.h"
#include "Telop.h"
#include "warp.h"
#include "fastPlayer.h"
#include "Player.h"
#include "Door.h"
#include "Game.h"
#include "WarpCounter.h"

bool Telop::Start()
{
	m_telopRender.Init("Assets/sprite/huti.dds", 1200.0f, 400.0f);
	//表示する座標を設定する。
	m_telopRender.SetPosition({ 0.0f,-350.0f ,0.0f });

	m_kaiwa0Render.Init("Assets/sprite/kaiwa0.dds", 512.0f, 512.0f);
	//表示する座標を設定する。
	m_kaiwa0Render.SetPosition({ -150.0f,-290.0f ,0.0f });

	m_kaiwa1Render.Init("Assets/sprite/kaiwa1.dds", 1024.0f, 512.0f);
	//表示する座標を設定する。
	m_kaiwa1Render.SetPosition({ -160.0f,-350.0f ,0.0f });

	m_kaiwa2Render.Init("Assets/sprite/kaiwa2.dds", 1024.0f, 512.0f);
	//表示する座標を設定する。
	m_kaiwa2Render.SetPosition({ -160.0f,-350.0f ,0.0f });

	m_yazirusiRender.Init("Assets/sprite/yazirusi.dds", 40.0f, 40.0f);
	//表示する座標を設定する。
	m_yazirusiRender.SetPosition({ 0.0f,-350.0f ,0.0f });
	
	m_game = FindGO<Game>("game");

	warpCounter = FindGO<WarpCounter>("WarpCounter");

	return true;
}

void Telop::Update()
{
	if (warpCounter->warpCounter == 0) {
		m_warp = FindGO<warp>("warp");
		m_fastplayer = FindGO<fastPlayer>("fastplayer");
		Vector3 diff = m_fastplayer->GetPosition() - m_warp->GetPosition();

		if (diff.Length() <= 300.0f)
		{
			m_warprange = true;
					//wchar_t wcsbuf[256];
/*						//swprintf_s(wcsbuf, 256, L"HP:%d", int());
						m_fontRender.SetText(L"準備はいいか？ \n \n はい");
						//表示する座標を設定する。
						m_fontRender.SetPosition({ -150.0f,-250.0f,0.0f });
						//文字の大きさを変える。
						//fontRender.SetScale(1.5f);
						//表示する色を設定する。
						m_fontRender.SetColor(g_vec4White);
			

			m_fontRender.SetText(L"先に進んだらもう戻れない \n準備はいいか？\n はい  いいえ");
			m_fontRender.SetPosition({ -200.0f,-250.0f,0.0f });
			m_fontRender.SetColor(g_vec4White);
	*/		
			if (m_kaiwa == 0)
			{
				//話しかける。
	//			m_fontRender.SetText(L"話しかける");
	//			m_fontRender.SetPosition({ -250.0f,-250.0f,0.0f });
	//			m_fontRender.SetColor(g_vec4White);
			}
			if (m_kaiwa == 1)
			{

				if(g_pad[0]->IsTrigger(enButtonRight))
				{
					sentaku = 1;
				}

				if (g_pad[0]->IsTrigger(enButtonLeft))
				{
					sentaku = 0;
				}

				if (sentaku == 0)
				{
					m_yazirusiRender.SetPosition({ -300.0f,-410.0f ,0.0f });
				}

				if (sentaku == 1)
				{
					m_yazirusiRender.SetPosition({ -10.0f,-410.0f ,0.0f });
				}

/*				m_fontRender.SetText(L"先に進んだらもう戻れない\n準備はいいか？\n\nはい");
				m_fontRender.SetPosition({ -250.0f,-250.0f,0.0f });
				m_fontRender.SetColor(g_vec4White);

				m_fontRender2.SetText(L"\n\n\nいいえ");
				m_fontRender2.SetPosition({ 0.0f,-250.0f,0.0f });
				m_fontRender2.SetColor(g_vec4White);
*/			}
			if (m_kaiwa == 2)
			{
	//			m_fontRender.SetText(L"そうか。\n準備ができたら言え。");
	//			m_fontRender.SetPosition({ -250.0f,-250.0f,0.0f });
	//			m_fontRender.SetColor(g_vec4White);
			}

			if (m_kaiwa == 3)
			{
				sentaku = 0;
				m_kaiwa = 0;
			}

		}
		else
		{
			m_warprange = false;
		}
	}
	else {
		m_warprange = false;
	}

	m_telopRender.Update();
	m_kaiwa0Render.Update();
	m_kaiwa1Render.Update();
	m_kaiwa2Render.Update();
	m_yazirusiRender.Update();
}

void Telop::Render(RenderContext& rc)
{
	if (m_warprange == true)
	{
		m_telopRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.8f));
		m_telopRender.Draw(rc);

		if (m_kaiwa ==0)
		{
			m_kaiwa0Render.Draw(rc);
		}

		if (m_kaiwa == 1)
		{
			m_kaiwa1Render.Draw(rc);
			m_yazirusiRender.Draw(rc);
		}

		if (m_kaiwa == 2)
		{
			m_kaiwa2Render.Draw(rc);
		}


	}
	else {
		return;
	}
}