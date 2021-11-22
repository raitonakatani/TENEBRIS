#pragma once


class TIMER :public IGameObject
{
public:
	TIMER();
	~TIMER();
	bool Start();
	//更新処理。
	void Update();
	//描画処理。
	void Render(RenderContext& rc);

	//ここからメンバ変数。
	ModelRender m_modelRender;            //モデルレンダー。
	FontRender fontRender2;               //文字。
	float m_timer = 0.0f;                  //タイマー。
};
