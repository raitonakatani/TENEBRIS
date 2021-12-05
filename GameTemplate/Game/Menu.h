#pragma once

class Player;

class Menu :public IGameObject
{
public:
	Menu();
	~Menu();

	bool Start();
	//�X�V�����B
	void Update();
	//�`�揈���B
	void Render(RenderContext& rc);

private:
	SpriteRender m_menuRender;
	SpriteRender m_mapRender;
	FontRender m_HPRender;
	FontRender m_AttackRender;
	FontRender m_portionRender;
	Player* m_player=nullptr;

	float HP = 0;
	int Attack = 0;
	int portion = 0;
};

