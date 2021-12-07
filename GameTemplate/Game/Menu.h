#pragma once

class Player;

class Menu :public IGameObject
{
public:
	Menu() {}
	~Menu() {}

	bool Start();
	//更新処理。
	void Update();
	//描画処理。
	void Render(RenderContext& rc);

private:
	SpriteRender m_menuRender;
	SpriteRender m_mapRender;
	FontRender m_HPRender;
	FontRender m_StaminaRender;
	FontRender m_AttackRender;
	FontRender m_portionRender;
	Player* m_player=nullptr;

	int HP = 0;
	float Stamina = 0.0f;
	int Attack = 0;
	int portion = 0;
};

