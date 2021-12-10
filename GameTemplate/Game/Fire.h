#pragma once

class Player;
class fastPlayer;
class WarpCounter;

//☆クラス。
class Fire : public IGameObject
{
public:
	Fire();
	~Fire();
	bool Start();
	//更新処理。
	void Update();
	//描画処理。
	void Render(RenderContext& rc);
	void CreateFireEffect();
	/// <summary>
	/// 座標を設定。
	/// </summary>
	/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	//回転を設定。
	void SetRotation(const Quaternion rotation)
	{
		m_rotation = rotation;
	}

	//大きさを設定。
	void SetScale(Vector3 scale)
	{
		m_scale = scale;
	}

private:
	//ここからメンバ変数。
	ModelRender m_modelRender;            //モデルレンダー。
	Vector3 m_position;                  //座標。
	int moveState = 0;                   //上に移動か下に移動か。
	Player* m_player;                   //プレイヤー。
	fastPlayer* m_fastplayer;
	SoundSource* se;                    //se
	EffectEmitter* m_effectEmitter = nullptr;					//エフェクト。
	Quaternion		m_rotation;
	PhysicsStaticObject m_physicsStaticObject;      //静的物理オブジェクト。
	Vector3			m_scale;
	PointLight* m_pointLight = nullptr;
	WarpCounter* warpCounter = nullptr;
	float m_timer = 0.0f;
	bool model = true;
};



