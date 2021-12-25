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
	m_telopRender.Init("Assets/sprite/huti.dds", 800.0f, 200.0f);
	//表示する座標を設定する。
	m_telopRender.SetPosition({ 0.0f,-300.0f ,0.0f });

	
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
			//swprintf_s(wcsbuf, 256, L"HP:%d", int());
			m_fontRender.SetText(L"準備はいいか？ \n \n はい");
			//表示する座標を設定する。
			m_fontRender.SetPosition({ -150.0f,-250.0f,0.0f });
			//文字の大きさを変える。
			//fontRender.SetScale(1.5f);
			//表示する色を設定する。
			m_fontRender.SetColor(g_vec4White);
		}
		else
		{
			m_warprange = false;
		}
	}
	else {
		m_warprange = false;
	}

/*	if (warpCounter->warpCounter != 0)
	{
		m_player = FindGO<Player>("player");
		m_door = FindGO<Door>("door");

		if (m_door->IsRange()==true)
		{
			m_doorrange = true;
		
			if (m_game->GetsibouEnemy() == 0 && DoorCount == 0 || m_game->GetsibouEnemy() == 0 && DoorCount == 1 ||
				m_game->GetsibouEnemy() == 4 && DoorCount == 2 ||
				m_game->GetsibouEnemy() == 5 && DoorCount == 3 ||
				m_game->GetsibouEnemy() == 10 && DoorCount == 4 ||
				m_game->GetsibouEnemy() == 11 && DoorCount == 5 ||
				m_game->GetsibouEnemy() == 17 && DoorCount == 6 || m_game->GetsibouEnemy() == 17 && DoorCount == 7
				)
			{
				m_fontRender2.SetText(L"扉を開ける。\n \n Aボタン");
				//表示する座標を設定する。
				m_fontRender2.SetPosition({ -150.0f,-250.0f,0.0f });
				//文字の大きさを変える。
				//m_fontRender.SetScale(1.5f);
				//表示する色を設定する。
				m_fontRender2.SetColor(g_vec4White);

				if (g_pad[0]->IsTrigger(enButtonA))
				{
					DoorCount += 1;
				}
			}
			else
			{
				m_fontRender2.SetText(L"カギが閉まっている");
				//表示する座標を設定する。
				m_fontRender2.SetPosition({ -150.0f,-250.0f,0.0f });
				//文字の大きさを変える。
				//m_fontRender.SetScale(1.5f);
				//表示する色を設定する。
				m_fontRender2.SetColor(g_vec4White);
			}

		}
		else
		{
			m_doorrange = false;
		}
	
	}
	else {
		m_doorrange = false;
	}
	*/
	m_telopRender.Update();
}

void Telop::Render(RenderContext& rc)
{
	if (m_warprange == true)
	{
		m_telopRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.8f));
		m_telopRender.Draw(rc);
		m_fontRender.Draw(rc);
	}
	if (m_doorrange == true)
	{
		m_telopRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.8f));
		m_telopRender.Draw(rc);
		m_fontRender2.Draw(rc);
	}
	else {
		return;
	}
}