#include "stdafx.h"
#include "Menu.h"
#include "Player.h"

Menu::Menu()
{

}

Menu::~Menu()
{

}

bool Menu::Start()
{

	m_menuRender.Init("Assets/sprite/menu.dds",1800.0f, 1050.0f);
	m_mapRender.Init("Assets/sprite/map.dds", 900.0f, 1050.0f);
	m_mapRender.SetPosition({ -450.0f,0.0f,0.0f });
	return true;
}

void Menu::Update()
{
	m_player = FindGO<Player>("player");
	
	HP = m_player->GetPlayerHP();
	Attack = m_player->level * 10;
	portion = m_player->GetPortion();

	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"%d", int(HP));
	m_HPRender.SetText(wcsbuf);
	//文字の大きさを変える。
	//m_HPRender.SetScale(5.0f);
	//表示する座標を設定する。
	m_HPRender.SetPosition(Vector3{ 500.0f,430.0f,0.0f });

	wchar_t wcsbuf2[256];
	swprintf_s(wcsbuf2, 256, L"%d", int(Attack));
	m_AttackRender.SetText(wcsbuf2);
	//文字の大きさを変える。
	//m_HPRender.SetScale(5.0f);
	//表示する座標を設定する。
	m_AttackRender.SetPosition(Vector3{ 600.0f,290.0f,0.0f });

	wchar_t wcsbuf3[256];
	swprintf_s(wcsbuf3, 256, L"%d", int(portion));
	m_portionRender.SetText(wcsbuf3);
	//文字の大きさを変える。
	//m_HPRender.SetScale(5.0f);
	//表示する座標を設定する。
	m_portionRender.SetPosition(Vector3{ 645.0f,185.0f,0.0f });

	m_menuRender.Update();
	m_mapRender.Update();
}

void Menu::Render(RenderContext&rc)
{
	m_menuRender.SetMulColor(Vector4(0.5f, 0.5f, 0.5f, 0.6f));
	m_menuRender.Draw(rc);
	m_mapRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.7f));
	m_mapRender.Draw(rc);
	m_HPRender.Draw(rc);
	m_AttackRender.Draw(rc);
	m_portionRender.Draw(rc);
}