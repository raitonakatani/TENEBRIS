#pragma once

class fastPlayer;
class Player;
class warp;
class Door;
class Game;
class WarpCounter;

class Telop : public IGameObject
{
public:
	Telop() {}
	~Telop() {}

	bool Start();
	void Update();
	void Render(RenderContext& rc);



	void Pluskaiwa()
	{
		m_kaiwa++;
	}

	int Getkaiwa()
	{
		return m_kaiwa;
	}

	int Getsentaku()
	{
		return sentaku;
	}

private:
	SpriteRender			m_telopRender;
	SpriteRender			m_kaiwa0Render;
	SpriteRender			m_kaiwa1Render;
	SpriteRender			m_kaiwa2Render;
	SpriteRender			m_yazirusiRender;
	FontRender				m_fontRender;			//文字の表示。
	FontRender				m_fontRender2;			//文字の表示。
	FontRender				m_fontRender3;			//文字の表示。
	Player* m_player;
	fastPlayer* m_fastplayer;
	warp* m_warp;
	Door* m_door;
	Game* m_game;
	WarpCounter* warpCounter = nullptr;

	int sentaku = 0;
	int m_kaiwa = 0;
	int DoorCount = 0;
	bool m_warprange = false;
	bool m_doorrange = false;
};

