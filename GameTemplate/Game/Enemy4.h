#pragma once

//クラス宣言。
class Player;

/// <summary>
/// エネミー。
/// </summary>
class Enemy4 : public IGameObject
{
public:
	//エネミーステート。
	enum EnEnemy4State {
		enEnemy4State_Idle,					//待機。
		enEnemy4State_Chase,				//追跡。
		enEnemy4State_Attack,				//攻撃。
		enEnemy4State_MagicAttack,			//魔法攻撃。
		enEnemy4State_ReceiveDamage,		//被ダメージ。
		enEnemy4State_Down,					//ダウン。
		enEnemy4State_model
	};
public:
	Enemy4();
	~Enemy4();
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
	void SetHP(const int hp)
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
	///中ボスの番号を設定する。
	/// </summary>
	/// <param name="tyuubossNumber">中ボスの番号。</param>
	void SettyuubossNumber(const int tyuubossNumber)
	{
		m_tyuubossNumber = tyuubossNumber;
	}
	/// <summary>
	/// ドアの番号を取得する。
	/// </summary>
	/// <returns>ドアの番号。</returns>
	const int GettyuubossNumber() const
	{
		return m_tyuubossNumber;
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
	/// 攻撃処理。
	/// </summary>
	void Attack();
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
	/// 攻撃用の当たり判定コリジョンを作成する。
	/// </summary>
	void MakeAttackCollision();
	/// <summary>
	/// ファイヤーボールを作成する。
	/// </summary>
	void MakeFireBall();
	/// <summary>
	/// アニメーションの再生。
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// アニメーションイベント用の関数。
	/// </summary>
	/// <param name="clipName">アニメーションの名前。</param>
	/// <param name="eventName">アニメーションイベントのキーの名前。</param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
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
	/// 攻撃ステートの遷移処理。
	/// </summary>
	void ProcessAttackStateTransition();
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

	enum EnAnimationClip {						//アニメーション。
		enAnimationClip_Idle,					//待機アニメーション。
		enAnimationClip_Walk,					//歩きアニメーション。
		enAnimationClip_Attack,					//攻撃アニメーション。
		enAnimationClip_MagicAttack,			//魔法攻撃アニメーション。
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
	Vector3						m_scale = Vector3::One;						//大きさ。
	CharacterController			m_charaCon;									//キャラコン。
	EnEnemy4State				m_Enemy4State = enEnemy4State_Idle;			//エネミーステート。
	SpriteRender				m_spriteRender;								//画像。
	bool						m_isUnderAttack = false;					//攻撃中か？
	int							m_swordBoneId = -1;							//剣のボーンのID。
	float						m_hp = 10;									//HP。
	Player* m_player = nullptr;												//プレイヤー。
	float						m_chaseTimer = 0.0f;						//追跡タイマー。
	float						m_idleTimer = 0.0f;							//待機タイマー。
	float						a = 0.0f;			//拡大率
	bool						m_isShowHPBar = false;
	bool model = false;
	int karyoku = 1;
	int m_tyuubossNumber = 0;
};



