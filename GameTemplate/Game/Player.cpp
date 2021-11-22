#include "stdafx.h"
#include "Player.h"

#include "Game.h"


//CollisionObjectを使用するために、ファイルをインクルードする。
#include "collision/CollisionObject.h"

//EffectEmitterを使用するために、ファイルをインクルードする。
#include "graphics/effect/EffectEmitter.h"

Player::Player()
{

}

Player::~Player()
{

}

bool Player::Start()
{
	//アニメーションをロードする。
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData2/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData2/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData2/run.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData2/damage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData2/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Attack2].Load("Assets/animData2/attack2.tka");
	m_animationClips[enAnimationClip_Attack2].SetLoopFlag(false);

	// 番号を指定して、efkファイルを読み込む。
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/efk/syoukann.efk");
	
	//剣士のモデルを読み込む。
	m_modelRender.Init("Assets/modelData/kensi.tkm", m_animationClips, enAnimationClip_Num);
	m_modelRender.SetScale(m_scale);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	//キャラクターコントローラーを初期化。
	m_charaCon.Init(
		30.0f,			//半径。
		120.0f,			//高さ。
		m_position		//座標。
	);

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	m_spriteRender.Init("Assets/sprite/HP2.dds", 512.0f, 32.0f);
	//表示する座標を設定する。
	m_spriteRender.SetPosition({ -950.0f,450.0f ,0.0f });
	m_spriteRender.SetPivot({ 0.0f, 0.5f });

	m_game = FindGO<Game>("game");



	//「Sword」ボーンのID(番号)を取得する。
	m_sword_jointBoneId = m_modelRender.FindBoneID(L"sword_joint");

	return true;
}

void Player::Update()
{
	//移動処理。
	Move();
	//回転処理。
	Rotation();
	//攻撃処理。
	Attack();
	//当たり判定。
	Collision();
	//アニメーションの再生。
	PlayAnimation();
	//ステートの遷移処理。
	ManageState();

	//モデルの更新。
	m_modelRender.Update();

	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"HP:%d", int(m_hp));
	m_fontRender.SetText(wcsbuf);
	//表示する座標を設定する。
	m_fontRender.SetPosition({ -950.0f,520.0f,0.0f });
	//文字の大きさを変える。
	//fontRender.SetScale(1.5f);
	//表示する色を設定する。
	m_fontRender.SetColor(g_vec4White);


	wchar_t wcsbuf2[256];
	swprintf_s(wcsbuf2, 256, L"Level:%d", int(level));
	m_fontRender2.SetText(wcsbuf2);
	//表示する座標を設定する。
	m_fontRender2.SetPosition({ -950.0f,420.0f,0.0f });
	//文字の大きさを変える。
	//fontRender.SetScale(1.5f);
	//表示する色を設定する。
	m_fontRender2.SetColor(g_vec4White);

	wchar_t wcsbuf3[256];
	swprintf_s(wcsbuf3, 256, L"カギ:%d", int(m_kagi));
	m_fontRender3.SetText(wcsbuf3);
	//表示する座標を設定する。
	m_fontRender3.SetPosition({ -950.0f,320.0f,0.0f });
	//文字の大きさを変える。
	//fontRender.SetScale(1.5f);
	//表示する色を設定する。
	m_fontRender3.SetColor(g_vec4White);


	//HPバー
	if (m_hp <= 0.0f)
	{
		m_hp = 0.0f;
	}

	life = m_hp/ 100.0f;
	m_spriteRender.SetScale({ life, 1.0f, 0.0f });
	m_spriteRender.Update();
}

void Player::Move()
{
	
	//移動できない状態であれば、移動処理はしない。
	if (IsEnableMove() == false)
	{
		return;
	}

	if (IsEnableMove2() == false)
	{
		return;
	}

	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//このフレームの移動量を求める。
	//左スティックの入力量を受け取る。
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	//カメラの前方方向と右方向を取得。
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//XZ平面での前方方向、右方向に変換する。
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//XZ成分の移動速度をクリア。
	m_moveSpeed += cameraForward * lStick_y * 600.0f;	//奥方向への移動速度を加算。
	m_moveSpeed += cameraRight * lStick_x * 600.0f;		//右方向への移動速度を加算。

	
	timer += g_gameTime->GetFrameDeltaTime();
	if (g_pad[0]->IsTrigger(enButtonB)
		&& dash == false)
	{
		timer = 0.0f;
		dash = true;
	}

	if (dash == true) 
	{
		if (timer < 0.1f)
		{
			m_moveSpeed += cameraForward * lStick_y *  5000.0f;	//奥方向への移動速度を加算。
			m_moveSpeed += cameraRight * lStick_x * 5000.0f;		//右方向への移動速度を加算。
		}
		else if (timer >= 0.1f && timer < 0.3f)
		{
			m_moveSpeed += cameraForward * lStick_y * 500.0f;	//奥方向への移動速度を加算。
			m_moveSpeed += cameraRight * lStick_x * 500.0f;		//右方向への移動速度を加算。	
		}
		else if (timer >= 2.0f)
		{
			timer = 0.0f;
			dash = false;
		}
	}

/*	if (g_pad[0]->IsTrigger(enButtonA) //Aボタンが押されたら
		&& m_charaCon.IsOnGround()  //かつ、地面に居たら
		) {
		m_moveSpeed.y = 350.0f;	//上方向に速度を設定して、
	}
*/
	//キャラクターコントローラーを使用して、座標を更新。
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//地面についた。
		m_moveSpeed.y = 0.0f;
	}

	m_moveSpeed.y -= 500.0f * g_gameTime->GetFrameDeltaTime();

	Vector3 modelPosition = m_position;
	//ちょっとだけモデルの座標を挙げる。
	modelPosition.y += 2.5f;
	m_modelRender.SetPosition(modelPosition);
}

void Player::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		//m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
		//このフレームではキャラは移動していないので旋回する必要はない。
		return;
	}
	//atan2はtanθの値を角度(ラジアン単位)に変換してくれる関数。
	//m_moveSpeed.x / m_moveSpeed.zの結果はtanθになる。
	//atan2を使用して、角度を求めている。
	//これが回転角度になる。
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atanが返してくる角度はラジアン単位なので
	//SetRotationDegではなくSetRotationを使用する。
	m_rotation.SetRotationY(-angle);

	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);

	//プレイヤーの正面ベクトルを計算する。
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}


void Player::Collision()
{

	//被ダメージの時は。
	//当たり判定処理はしない。
	if (m_playerState == enPlayerState_ReceiveDamage)
	{
		return;
	}

	//音を読み込む。
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/3damage.wav");
	//効果音を再生する。
	SoundSource* damagese = NewGO<SoundSource>(0);
	damagese->Init(3);


	//エネミー（knight）の攻撃用のコリジョンを取得する。
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	//コリジョンの配列をfor文で回す。
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_charaCon))
		{
			//HPを1減らす。
			m_hp -= 10;

			//音を再生する。
			damagese->Play(false);
			damagese->SetVolume(1.5f);

			//もしHPが0より上なら。
			if (m_hp > 0)
			{
				//被ダメージステートに遷移する。
				m_playerState = enPlayerState_ReceiveDamage;
			}
			//HPが0なら。
			else if (m_hp <= 0)
			{
				//被ダメージステートに遷移する。
				m_playerState = enPlayerState_ReceiveDamage;
			}
		
			return;
		}
	}

	//エネミーのファイヤーボール用のコリジョンを取得する。
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("enemy_Magic");
	//for文で配列を回す。
	for (auto collision : collisions2)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_charaCon))
		{
			//HPを10減らす。
			m_hp -= 10;

			//音を再生する。
			damagese->Play(false);
			damagese->SetVolume(1.5f);

			//もしHPが0より上なら。
			if (m_hp > 0)
			{
				//被ダメージステートに遷移する。
				m_playerState = enPlayerState_ReceiveDamage;
			}
			//HPが0なら。
			else if (m_hp <= 0)
			{
				//被ダメージステートに遷移する。
				m_playerState = enPlayerState_ReceiveDamage;
			}
			return;
		}
	}

	//エネミー（ボス）の攻撃用のコリジョンを取得する。
	const auto& collisions3 = g_collisionObjectManager->FindCollisionObjects("enemy3_attack");
	//コリジョンの配列をfor文で回す。
	for (auto collision : collisions3)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_charaCon))
		{
			//HPを1減らす。
			m_hp -= 10;

			//音を再生する。
			damagese->Play(false);
			damagese->SetVolume(1.5f);

			//もしHPが0より上なら。
			if (m_hp > 0)
			{
				//被ダメージステートに遷移する。
				m_playerState = enPlayerState_ReceiveDamage;
			}
			//HPが0なら。
			else if (m_hp <= 0)
			{
				//被ダメージステートに遷移する。
				m_playerState = enPlayerState_ReceiveDamage;
			}

			return;
		}
	}

	//エネミー（カギ持ち）の攻撃用のコリジョンを取得する。
	const auto& collisions4 = g_collisionObjectManager->FindCollisionObjects("enemy4_attack");
	//コリジョンの配列をfor文で回す。
	for (auto collision : collisions4)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_charaCon))
		{
			//HPを1減らす。
			m_hp -= 10;

			//音を再生する。
			damagese->Play(false);
			damagese->SetVolume(1.5f);

			//もしHPが0より上なら。
			if (m_hp > 0)
			{
				//被ダメージステートに遷移する。
				m_playerState = enPlayerState_ReceiveDamage;
			}
			//HPが0なら。
			else if (m_hp <= 0)
			{
				//被ダメージステートに遷移する。
				m_playerState = enPlayerState_ReceiveDamage;
			}

			return;
		}
	}
}

void Player::Attack()
{
	//攻撃中でないなら、処理をしない。
	if (m_playerState != enPlayerState_Attack)
	{
		return;
	}

	//攻撃判定中であれば。
	if (m_isUnderAttack == true)
	{
		//攻撃用のコリジョンを作成する。
		MakeAttackCollision();
	}
}

void Player::MakeAttackCollision()
{
	//コリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);

	Vector3 collisionPosition = m_position;
	//座標をプレイヤーの少し前に設定する。
	collisionPosition += m_forward * 50.0f;
	collisionPosition.y += 100.0f;
	//ボックス状のコリジョンを作成する。
	collisionObject->CreateBox(collisionPosition,               //座標。
		Quaternion::Identity,                                       //回転。
		Vector3(200.0f, 20.0f, 20.0f)                               //大きさ。
	);
	collisionObject->SetName("player_attack");

	//「Sword」ボーンのワールド行列を取得する。
	Matrix matrix = m_modelRender.GetBone(m_sword_jointBoneId)->GetWorldMatrix();
	//「Sword」ボーンのワールド行列をコリジョンに適用する。
	collisionObject->SetWorldMatrix(matrix);
}

void Player::MakePushLeverCollision()
{
	//コリジョンオブジェクトを作成する。
	CollisionObject* collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	//座標をプレイヤーの少し前に設定する。
	collisionPosition += m_forward * 50.0f;
	//球状のコリジョンを作成する。
	collisionObject->CreateSphere(collisionPosition,       //座標。
		Quaternion::Identity,                                  //回転。
		70.0f);                                                //球の大きさ(半径)。
		//名前を付ける。
	collisionObject->SetName("player_lever");
}

void Player::ProcessCommonStateTransition()
{
	
	//Yボタンが押されたら。
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		//攻撃ステートに移行する。
		m_playerState = enPlayerState_Attack;
		//フラグをfalseにする。
		m_isUnderAttack = false;
		return;
	}
	
/*
	
	if (g_pad[0]->IsTrigger(enButtonX))
	{
		MakePushLeverCollision();
	}
*/
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//移動速度が一定以上だったら。
		if (g_pad[0]->IsPress(enButtonLB2))
		{
			//ステートを走りにする。
			m_playerState = enPlayerState_Run;
			return;
		}
		else
		{
			//歩きにする。
			m_playerState = enPlayerState_Walk;
			return;
		}

	}
	//xとzの移動速度が無かったら(スティックの入力が無かったら)。
	else
	{
		//ステートを待機にする。
		m_playerState = enPlayerState_Idle;
		return;
	}
}

void Player::ProcessIdleStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}

void Player::ProcessRunStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}

void Player::ProcessWalkStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}

void Player::ProcessAttackStateTransition()
{
	//攻撃アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//ステートを遷移する。
		ProcessCommonStateTransition();
	}
}


void Player::ProcessReceiveDamageStateTransition()
{
	m_timer += g_gameTime->GetFrameDeltaTime();

	//被ダメージアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false
		&& m_timer >= 0.3f)
	{
		//攻撃されたら、待機アニメーションにさせる。
		m_playerState = enPlayerState_Idle;
		m_timer = 0.0f;
	}
}

void Player::ManageState()
{
	switch (m_playerState)
	{
		//待機ステートの時。
	case enPlayerState_Idle:
		//待機ステートのステート遷移処理。
		ProcessIdleStateTransition();
		break;
		//走りステートの時。
	case enPlayerState_Run:
		//走りステートのステート遷移処理。
		ProcessRunStateTransition();
		break;
	case enPlayerState_Walk:
		//走りステートのステート遷移処理。
		ProcessWalkStateTransition();
		break;
	case enPlayerState_ReceiveDamage:
		//被ダメージステートのステート遷移処理。
		ProcessReceiveDamageStateTransition();
		break;
		//攻撃ステートの時。
	case enPlayerState_Attack:
		//攻撃ステートのステート遷移処理。
		ProcessAttackStateTransition();
		break;
	}
}

void Player::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_playerState)
	{
		//待機ステートの時。
	case enPlayerState_Idle:
		//待機アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//走りステートの時。
	case enPlayerState_Run:
		m_modelRender.SetAnimationSpeed(1.2f);
		//走りアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.5f);
		break;
	case enPlayerState_Walk:
		m_modelRender.SetAnimationSpeed(1.0f);
		//走りアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.5f);
		break;
		//被ダメージステートの時。
	case enPlayerState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(0.8f);
		//被ダメージアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
		//攻撃ステートの時。
	case enPlayerState_Attack:
		m_modelRender.SetAnimationSpeed(1.0f);
		//攻撃アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
	default:
		break;
	}
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

	//キーの名前が「attack_start」の時。
	if (wcscmp(eventName, L"attack_start") == 0) {
		//攻撃中にする。
		m_isUnderAttack = true;
	}
	//キーの名前が「attack_end」の時。
	else if (wcscmp(eventName, L"attack_end") == 0) {
		//攻撃を終わる。
		m_isUnderAttack = false;
	}

}

void Player::Render(RenderContext& rc)
{
	//モデルを描画する。
	m_modelRender.Draw(rc);
	//画像を描写する。
	m_spriteRender.Draw(rc);
	//文字を描写する。
	m_fontRender.Draw(rc);
	m_fontRender2.Draw(rc);
	m_fontRender3.Draw(rc);
}
