#include "stdafx.h"
#include "fastPlayer.h"

#include "Game.h"
#include "Menu.h"
#include"WarpCounter.h"


//CollisionObjectを使用するために、ファイルをインクルードする。
#include "collision/CollisionObject.h"

//EffectEmitterを使用するために、ファイルをインクルードする。
#include "graphics/effect/EffectEmitter.h"

fastPlayer::fastPlayer()
{

}

fastPlayer::~fastPlayer()
{

}

bool fastPlayer::Start()
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

	// 番号を指定して、efkファイルを読み込む。
	//EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/efk/syoukann.efk");

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

	m_game = FindGO<Game>("game");

	//エフェクトを読み込む。
	EffectEngine::GetInstance()->ResistEffect(2, u"Assets/effect/efk/Heal.efk");

	//「Sword」ボーンのID(番号)を取得する。
	m_sword_jointBoneId = m_modelRender.FindBoneID(L"sword_joint");

	return true;
}

void fastPlayer::Update()
{
	warpCounter = FindGO<WarpCounter>("WarpCounter");

	if (warpCounter->warpCounter == 0) {

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
	}

	if (warpCounter->warpCounter != 0) {
		DeleteGO(this);
	}
	//モデルの更新。
	m_modelRender.Update();




}

void fastPlayer::Move()
{

	//移動できない状態であれば、移動処理はしない。
	if (IsEnableMove() == false)
	{
		//攻撃
		return;
	}

	if (IsEnableMove2() == false)
	{
		//ダメージ
		return;
	}

	
	if (IsEnableMove() == true || IsEnableMove2() == true) {
		if (m_game->IsMove() == true)
		{
			m_playerState = enPlayerState_Idle;
			m_moveSpeed.x = 0.0f;
			m_moveSpeed.z = 0.0f;
			return;
		}
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

/*	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_moveSpeed.y += 1000.0f;
	}
*/
	if (COOLtime == false)
	{
		if (g_pad[0]->IsPress(enButtonLB1) && m_sutamina > 0)
		{
			m_moveSpeed += cameraForward * lStick_y * 600.0f;	//奥方向への移動速度を加算。
			m_moveSpeed += cameraRight * lStick_x * 600.0f;		//右方向への移動速度を加算。
		}
		else
		{
			m_moveSpeed += cameraForward * lStick_y * 400.0f;	//奥方向への移動速度を加算。
			m_moveSpeed += cameraRight * lStick_x * 400.0f;		//右方向への移動速度を加算。
		}
	}

	
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

void fastPlayer::Rotation()
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


void fastPlayer::Collision()
{

	//音を読み込む。
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/3damage.wav");
	//効果音を再生する。
	SoundSource* damagese = NewGO<SoundSource>(0);
	damagese->Init(3);

}

void fastPlayer::Attack()
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


void fastPlayer::MakeAttackCollision()
{
	//コリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);

	Vector3 collisionPosition = m_position;
	//座標をプレイヤーの少し前に設定する。
	collisionPosition += m_forward * 50.0f;
	collisionPosition.y += 100.0f;
	//ボックス状のコリジョンを作成する。
	collisionObject->CreateBox(collisionPosition,				   //座標。
		Quaternion::Identity,                                       //回転。
		Vector3(220.0f, 30.0f, 30.0f)                               //大きさ。
	);
	collisionObject->SetName("fastPlayer_attack");

	//「Sword」ボーンのワールド行列を取得する。
	Matrix matrix = m_modelRender.GetBone(m_sword_jointBoneId)->GetWorldMatrix();
	//「Sword」ボーンのワールド行列をコリジョンに適用する。
	collisionObject->SetWorldMatrix(matrix);
}

void fastPlayer::ProcessCommonStateTransition()
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


	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//移動速度が一定以上だったら。
		if (g_pad[0]->IsPress(enButtonLB2))
		{
			//m_sutamina--;
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

//待機。
void fastPlayer::ProcessIdleStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}

//走り。
void fastPlayer::ProcessRunStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}

//歩く。
void fastPlayer::ProcessWalkStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}

//攻撃。
void fastPlayer::ProcessAttackStateTransition()
{
	//攻撃アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//ステートを遷移する。
		ProcessCommonStateTransition();
	}
}

//ダメージ。
void fastPlayer::ProcessDamageStateTransition()
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

void fastPlayer::ManageState()
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
		//歩きステートの時。
	case enPlayerState_Walk:
		//走りステートのステート遷移処理。
		ProcessWalkStateTransition();
		break;
	case enPlayerState_ReceiveDamage:
		//被ダメージステートのステート遷移処理。
		ProcessDamageStateTransition();
		break;
		//攻撃ステートの時。
	case enPlayerState_Attack:
		//攻撃ステートのステート遷移処理。
		ProcessAttackStateTransition();
		break;
	}
}

void fastPlayer::PlayAnimation()
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
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.5f);
		break;
		//攻撃ステートの時。
	case enPlayerState_Attack:
		m_modelRender.SetAnimationSpeed(1.0f);
		//攻撃アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.5f);
		break;
	default:
		break;
	}
}

void fastPlayer::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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

void fastPlayer::Render(RenderContext& rc)
{
	//モデルを描画する。
	m_modelRender.Draw(rc);
}
