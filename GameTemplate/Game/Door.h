#pragma once

/// <summary>
/// ドア。
/// </summary>

class Player;
class Game;

class Door : public IGameObject
{
public:
	//ドアステート。
	enum EnDoorState {
		enDoorState_Idle,				//待機。
		enDoorState_Open,				//オープン。
		enDoorState_Open_Idle,			//オープンし終わった。
		enDoorState_Close,				//クローズ。
		enDoorState_Close_Idle			//クローズし終わった。
	};
public:
	Door();
	~Door();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// オープンを通知する。
	/// </summary>
	void NotifyOpen();
	/// <summary>
	/// クローズを通知する。
	/// </summary>
	void NotifyClose();
	/// <summary>
	/// 座標を設定。
	/// </summary>
	/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 座標を取得。
	/// </summary>
	/// <returns>座標。</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// 回転を設定する。
	/// </summary>
	/// <param name="rotation">回転。</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// 大きさを設定する。
	/// </summary>
	/// <param name="scale">大きさ。</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// ドアの番号を設定する。
	/// </summary>
	/// <param name="doorNumber">ドアの番号。</param>
	void SetDoorNumber(const int doorNumber)
	{
		m_doorNumber = doorNumber;
	}
	/// <summary>
	/// ドアの番号を取得する。
	/// </summary>
	/// <returns>ドアの番号。</returns>
	const int GetDoorNumber() const
	{
		return m_doorNumber;
	}

	bool IsRange() const
	{
		return  m_range;
	}
private:
	/// <summary>
	/// アニメーションを再生する。
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// フィジックスオブジェクトの当たり判定を開放する。
	/// </summary>
	void ReleasePhysicsObject();
	/// <summary>
	/// フィジックスオブジェクトの当たり判定を作成する。
	/// </summary>
	void CreatePhysicsObject();
	/// <summary>
	/// ステート管理。
	/// </summary>
	void ManageState();
	/// <summary>
	/// オープンステートのステート遷移処理。
	/// </summary>
	void ProcessOpenStateTransition();
	/// <summary>
	/// クローズステートのステート遷移処理。
	/// </summary>
	void ProcessCloseStateTransition();
private:
	ModelRender					m_modelRender;							//モデルレンダ―。
	Vector3						m_position;								//座標。
	Quaternion					m_rotation;								//回転。
	Vector3						m_scale = Vector3::One;					//大きさ。
	FontRender					m_fontRender;							//カギを所持しているとき。
	SpriteRender				m_telopRender;							//テロップ
	SpriteRender				m_kaiwa1Render;							//文字
	enum EnAnimationClip {												//アニメーション。
		enAnimationClip_Idle,											//待機。	
		enAnimationClip_Open,											//オープン。
		enAnimationClip_Close,											//クローズ。
		enAnimationClip_Num,											//アニメーションの数。
	};
	AnimationClip				m_animationClips[enAnimationClip_Num];	//アニメーションクリップ。
	PhysicsStaticObject			m_physicsStaticObject;					//フィジクススタティックオブジェクト。	
	EnDoorState					m_doorState = enDoorState_Idle;			//ドアステート。
	Player*						m_player;								//プレイヤー。
	Game*						m_game;
	bool						door = false;							//ドアとの距離。
	int							m_doorNumber = 0;						//ドアの番号。
	bool model = false;



	bool m_range = false;

	int m_DoorCount = 0;
};

