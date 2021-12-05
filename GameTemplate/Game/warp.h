#pragma once

class fastPlayer;
class WarpCounter;
//class Player;

class warp : public IGameObject
{
public:
	warp();
	~warp();
	bool Start();
	//更新処理。
	void Update();
	//描画処理。
	void Render(RenderContext& rc);
	//回転処理。
	void Rotation();

	/// <summary>
	/// 座標を設定。
	/// </summary>
	/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

private:
	//ここからメンバ変数。
	ModelRender m_modelRender;            //モデルレンダー。
	Vector3 m_position;                  //座標。
	Vector3 m_firstPosition;             //最初の座標。
	int moveState = 0;                   //上に移動か下に移動か。
	Quaternion rotation;                //クォータニオン。
	fastPlayer* m_fastplayer;                   //プレイヤー。
	WarpCounter* warpCounter = nullptr;
	//Player* m_player;
	SoundSource* se;                    //se
};


