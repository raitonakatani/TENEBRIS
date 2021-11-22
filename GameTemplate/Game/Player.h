#pragma once

//クラス宣言。
//class Lever;
class Collision;
class Game;

/// <summary>
/// プレイヤー。
/// </summary>
class Player : public IGameObject
{
public:
	//プレイヤーのステート。
	enum EnPlayerState {
		enPlayerState_Idle,					//待機。
		enPlayerState_Run,					//走る。
		enPlayerState_Walk,					//走る。
		enPlayerState_ReceiveDamage,			//被ダメージ。
		enPlayerState_Attack,				//攻撃。
		enPlayerState_Attack2				//攻撃。
	};
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
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
	/// 大きさを設定。
	/// </summary>
	/// <param name="scale">大きさ。</param>
	void SetScale(Vector3& scale)
	{
		m_scale = scale;
	}

	/// <summary>
	/// 回転を設定。
	/// </summary>
	/// <param name="rotation">回転。</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// 動ける状態(ステート)かどうかを取得。
	/// </summary>
	/// <returns>動ける状態(ステート)ならtrue。</returns>
	bool IsEnableMove() const
	{
		return m_playerState != enPlayerState_Attack;
	}

	bool IsEnableMove2() const
	{
		return m_playerState != enPlayerState_ReceiveDamage;
	}

	bool IsEnableMove3() const
	{
		return m_playerState != enPlayerState_Attack2;
	}

	void SetPlayerHP(float)
	{
		m_hp;
	}

	int GetPlayerHP()
	{
		return m_hp;
	}

	void PlusKAGI()
	{
		m_kagi++;
	}

	int GetKAGI()
	{
		return m_kagi;
	}

	void PlusBOSS()
	{
		m_boss++;
	}

	int GetBOSS()
	{
		return m_boss;
	}

	int level = 1;

private:
	/// <summary>
	/// 移動処理。
	/// </summary>
	void Move();
	/// <summary>
	/// 回転処理。
	/// </summary>
	void Rotation();
	/// <summary>
	/// 攻撃中の処理。
	/// </summary>
	void Attack();
	/// <summary>
	/// <summary>
	/// プレイヤーの攻撃との当たり判定処理。
	/// </summary>
	void Collision();
	/// <summary>
	/// 攻撃の際の当たり判定用のコリジョンを作成する。
	/// </summary>
	void MakeAttackCollision();
	/// <summary>
	/// レバーを押す時の、当たり判定用のコリジョンを作成する。
	/// </summary>
	void MakePushLeverCollision();
	/// <summary>
	/// アニメーションの再生。
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// 各ステートの遷移処理。
	/// </summary>
	void ManageState();
	/// <summary>
	/// 共通のステート遷移処理。
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// 待機ステートの遷移処理。
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// 走りステートの遷移処理。
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	 /// 歩きステートの遷移処理。
	/// </summary>
	void ProcessWalkStateTransition();
	/// <summary>
	/// 攻撃ステートの遷移処理。
	/// <summary>
	/// 被ダメージステートの遷移処理。
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	void ProcessAttackStateTransition();
	/// <summary>
	void ProcessAttackState2Transition();
	/// <summary>
	// アニメーションイベント用の関数。
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	
	//アニメーション。
	enum EnAnimationClip {
		enAnimationClip_Idle,				//待機アニメーション。	
		enAnimationClip_Run,				//走りアニメーション。
		enAnimationClip_Walk,				//走りアニメーション。
		enAnimationClip_Attack,				//攻撃アニメーション。
		enAnimationClip_Attack2,				//攻撃アニメーション2。
		enAnimationClip_Damage,				//被ダメージアニメーション。
		enAnimationClip_Num,				//アニメーションの数。
	};
	AnimationClip			m_animationClips[enAnimationClip_Num];		//アニメーションクリップ。
	ModelRender				m_modelRender;								//モデルレンダ―。
	Vector3					m_position;									//座標。
	Vector3					m_moveSpeed;								//移動速度。
	Vector3					m_forward = Vector3::AxisZ;					//プレイヤーの正面ベクトル。
	Vector3					m_scale;									//大きさ。
	Quaternion				m_rotation;									//クォータニオン。
	CharacterController		m_charaCon;									//キャラクターコントローラー。
	EnPlayerState			m_playerState = enPlayerState_Idle;			//プレイヤーステート。
	SpriteRender			m_spriteRender;						        //画像。
	FontRender				m_fontRender;									//文字　HPの表示。
	FontRender				m_fontRender2;					            //文字　Levelの表示。
	FontRender				m_fontRender3;					            //文字　鍵の所持数の表示。
	bool					m_isUnderAttack = false;					//攻撃中ならtrue。
	bool					m_isUnderAttack2 = false;					//攻撃中ならtrue。
	float						m_hp = 100.0f;							//プレイヤーのHP。
	Game*					m_game = nullptr;							//ゲーム。
	int                     m_sword_jointBoneId = -1;                   //「Sword」ボーンのID。
	float					life = 100.0f;								//HPバー
	float					m_timer = 0.0f;								//タイマー。
	int						m_kagi = 0;									//鍵
	int						m_boss = 0;									//ボス。

	bool dash = false;
	float timer = 0.0f;
};

