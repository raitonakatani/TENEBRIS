#include "stdafx.h"
#include "Menu.h"
#include "Player.h"


bool Menu::Start()
{

	m_menuRender.Init("Assets/sprite/menu1.dds",1920.0f, 1080.0f);
//	m_mapRender.Init("Assets/sprite/map.dds", 900.0f, 1050.0f);
//	m_mapRender.SetPosition({ -450.0f,0.0f,0.0f });

	m_player = FindGO<Player>("player");

	return true;
}

void Menu::Update()
{
	
	HP = m_player->GetPlayerHP();
	Stamina = m_player->GetPlayerStamina();
	Attack = m_player->level * 10;
	portion = m_player->GetPortion();

	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"%d", int(HP));
	m_HPRender.SetText(wcsbuf);
	//文字の大きさを変える。
	//m_HPRender.SetScale(5.0f);
	//表示する座標を設定する。
	m_HPRender.SetPosition(Vector3{ 240.0f,415.0f,0.0f });

	wchar_t wcsbuf2[256];
	swprintf_s(wcsbuf2, 256, L"%d", int(Stamina/1.5));
	m_StaminaRender.SetText(wcsbuf2);
	//文字の大きさを変える。
	//m_HPRender.SetScale(5.0f);
	//表示する座標を設定する。
	m_StaminaRender.SetPosition(Vector3{ 440.0f,290.0f,0.0f });

	wchar_t wcsbuf3[256];
	swprintf_s(wcsbuf3, 256, L"%d", int(Attack));
	m_AttackRender.SetText(wcsbuf3);
	//文字の大きさを変える。
	//m_HPRender.SetScale(5.0f);
	//表示する座標を設定する。
	m_AttackRender.SetPosition(Vector3{ 380.0f,160.0f,0.0f });

	wchar_t wcsbuf4[256];
	swprintf_s(wcsbuf4, 256, L"%d", int(portion));
	m_portionRender.SetText(wcsbuf4);
	//文字の大きさを変える。
	//m_HPRender.SetScale(5.0f);
	//表示する座標を設定する。
	m_portionRender.SetPosition(Vector3{ 290.0f,40.0f,0.0f });

	m_menuRender.Update();
//	m_mapRender.Update();
}

void Menu::Render(RenderContext&rc)
{
	m_menuRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.9f));
	m_menuRender.Draw(rc);
//	m_mapRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.9f));
//	m_mapRender.Draw(rc);


	m_HPRender.Draw(rc);
	m_StaminaRender.Draw(rc);
	m_AttackRender.Draw(rc);
	m_portionRender.Draw(rc);
}