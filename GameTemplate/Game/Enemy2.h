#pragma once

//クラス宣言。
class Player;
class Game;

/// <summary>
/// エネミー。
/// </summary>
class Enemy2 : public IGameObject
{
public:
	//エネミーステート。
	enum enEnemy2State {
		enEnemy2State_Idle,					//待機。
		enEnemy2State_Chase,					//追跡。
		enEnemy2State_MagicAttack,				//攻撃。
		enEnemy2State_ReceiveDamage,			//被ダメージ。
		enEnemy2State_Down,					//ダウン。
		enEnemy2State_model
	};
public:
	Enemy2();
	~Enemy2();
	bool Start();
	void Update();
	void MODEL();
	void Render(RenderContext& rc);
	/// <summary>
	/// 座標を設定する。
	/// </summary>
	/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 座標を取得する。
	/// </summary>
	/// <returns>座標。</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// HPを設定する。
	/// </summary>
	/// <param name="hp">HP。</param>
	void SetHP(const float hp)
	{
		m_hp = hp;
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
	/// 魔法使いの番号を設定する。
	/// </summary>
	/// <param name="wizardNumber">	魔法使いの番号。</param>
	void SetwizardNumber(const int wizardNumber)
	{
		m_wizardNumber = wizardNumber;
	}
	/// <summary>
	/// ドアの番号を取得する。
	/// </summary>
	/// <returns>ドアの番号。</returns>
	const int GetwizardNumber() const
	{
		return m_wizardNumber;
	}

private:
	/// <summary>
	/// 追跡処理。
	/// </summary>
	void Chase();
	/// <summary>
	/// 回転処理。
	/// </summary>
	void Rotation();
	/// <summary>
	/// プレイヤーの攻撃との当たり判定処理。
	/// </summary>
	void Collision();
	/// <summary>
	/// プレイヤーを探索する。
	/// </summary>
	/// <returns>プレイヤーが見つかったらtrue。</returns>
	const bool SearchPlayer() const;
	/// <summary>
	/// ファイヤーボールを作成する。
	/// </summary>
	void MakeFireBall();
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
	/// 歩きステートの遷移処理。
	/// </summary>
	void ProcessWalkStateTransition();
	/// <summary>
	/// 追跡ステートの背遷移処理。
	/// </summary>
	void ProcessChaseStateTransition();
	/// <summary>
	/// 魔法攻撃ステートの遷移処理。
	/// </summary>
	void ProcessMagicAttackStateTransition();
	/// <summary>
	/// 被ダメージステートの遷移処理。
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// ダウンステートの遷移処理。
	/// </summary>
	void ProcessDownStateTransition();
	/// <summary>
	/// 攻撃できる距離かどうか調べる。
	/// </summary>
	/// <returns>攻撃できるならtrue。</returns>
	const bool IsCanAttack() const;
	/// <summary>
   /// アニメーションイベント用の関数。
   /// </summary>
   /// <param name="clipName">アニメーションの名前。</param>
   /// <param name="eventName">アニメーションイベントのキーの名前。</param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	enum EnAnimationClip {						//アニメーション。
		enAnimationClip_Idle,					//待機アニメーション。
		enAnimationClip_Walk,					//歩きアニメーション。
		enAnimationClip_MagicAttack,			//攻撃アニメーション。
		enAnimationClip_Damage,					//被ダメージアニメーション。
		enAnimationClip_Down,					//ダウンアニメーション。
		enAnimationClip_Num,					//アニメーションの数。
	};
	AnimationClip				m_animationClips[enAnimationClip_Num];		//アニメーションクリップ。
	ModelRender					m_modelRender;								//モデルレンダー。
	Vector3						m_position;									//座標。
	Vector3						m_moveSpeed;								//移動速度。
	Vector3						m_forward = Vector3::AxisZ;					//エネミーの正面ベクトル。
	Quaternion					m_rotation;									//回転。
	Vector3						m_scale = Vector3(3.0f, 3.0f, 3.0f);		//大きさ。
	CharacterController			m_charaCon;									//キャラコン。
	EffectEmitter*				m_effectEmitter = nullptr;					//エフェクト。
	enEnemy2State				m_enemy2State = enEnemy2State_Idle;			//エネミーステート。
	Game*						m_game = nullptr;
	SpriteRender				m_spriteRender;								//画像。
	bool						m_isUnderAttack = false;					//攻撃中か？
	float						m_hp = 3;									//HP。
	int							m_swordBoneId = -1;							//剣のボーンのID。
	Player*						m_player = nullptr;							//プレイヤー。
	float						m_chaseTimer = 0.0f;						//追跡タイマー。
	float						m_idleTimer = 0.0f;							//待機タイマー。
	float						m_sibouTimer = 0.0f;						//待機タイマー。
	float						a = 0.0f;			//拡大率
	bool						m_isShowHPBar = false;
	bool model = false;
	float modeltimer = 0.0f;
	int m_model = 0;
	bool sibou = false;
	int karyoku = 1;
	int m_wizardNumber = 0;
};