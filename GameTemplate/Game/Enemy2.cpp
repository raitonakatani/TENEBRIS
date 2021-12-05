#include "stdafx.h"
#include "Enemy2.h"

#include "Game.h"
#include "Magic.h"
#include "Player.h"
#include "Portion.h"

#include <time.h>
#include <stdlib.h>

//CollisionObjectを使用したいため、ファイルをインクルードする。
#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"



Enemy2::Enemy2()
{

}

Enemy2::~Enemy2()
{

}

bool Enemy2::Start()
{
	//アニメーションを読み込む。
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/Enemy2/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/Enemy2/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_MagicAttack].Load("Assets/animData/Enemy2/magic_attack.tka");
	m_animationClips[enAnimationClip_MagicAttack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/Enemy2/damage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/Enemy2/sibou.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	//モデルを読み込む。
	m_modelRender.Init("Assets/modelData/Enemy/wizard.tkm", m_animationClips, enAnimationClip_Num);

	//座標を設定する。
	m_modelRender.SetPosition(m_position);
	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);
	//大きさを設定する。
	m_modelRender.SetScale(m_scale);

	//キャラクターコントローラーを初期化。
	m_charaCon.Init(
		40.0f,			//半径。
		150.0f,			//高さ。
		m_position		//座標。
	);


	//エフェクトを読み込む。
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/enemy.efk");


	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	m_player = FindGO<Player>("player");

	m_spriteRender.Init("Assets/sprite/HP2.dds", 64.0f, 32.0f);
	//表示する座標を設定する。
	m_spriteRender.SetPosition({ 0.0f,1450.0f ,0.0f });


	//乱数を初期化。
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	return true;
}

void Enemy2::Update()
{
	//追跡処理。
	Chase();
	//回転処理。
	Rotation();
	//当たり判定。
	Collision();
	//アニメーションの再生。
	PlayAnimation();
	//ステートの遷移処理。
	ManageState();


	modeltimer += g_gameTime->GetFrameDeltaTime();

	if (m_model == 1)
	{
		if (model == false) {
			//エフェクトのオブジェクトを作成する。
			m_effectEmitter = NewGO <EffectEmitter>(0);
			m_effectEmitter->Init(1);
			m_effectEmitter->SetPosition(m_position);
			//エフェクトの大きさを設定する。
			m_effectEmitter->SetScale(m_scale * 15.0f);
			m_effectEmitter->Play();
			modeltimer = 0.0f;

		}
		model = true;
	}
	else {
		model = false;
	}


	if (m_hp <= 0)
	{
		m_hp = 0;
	}

	//HPバー
	a = m_hp / 2;
	m_spriteRender.SetScale({ a, 0.5f, 0.0f });

	Vector3 diff = m_player->GetPosition() - m_position;

	//ベクトルの長さが1000.0fより小さかったら。
	if (diff.Length() <= 1000.0f)
	{
		//ワールド座標に変換。
		//座標をエネミーの少し上に設定する。
		Vector4 worldPos = Vector4(m_position.x, m_position.y + 250.0f, m_position.z, 1.0f);

		Matrix matrix;
		matrix.Multiply(g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix());

		matrix.Apply(worldPos);

		//カメラのビュー行列を掛ける。
		//カメラ座標に変換。
		worldPos.x = (worldPos.x / worldPos.w);
		worldPos.y = (worldPos.y / worldPos.w);

		//カメラのプロジェクション行列を掛ける。
		//スクリーン座標に変換。
		worldPos.x *= FRAME_BUFFER_W / 2;
		worldPos.y *= FRAME_BUFFER_H / 2;

		//ポジションの設定。
		m_spriteRender.SetPosition(Vector3(worldPos.x, worldPos.y, 0.0f));
		//HPバーを表示する。
		m_isShowHPBar = true;
	}
	else
	{
		//HPバーを表示しない。
		m_isShowHPBar = false;
	}
	m_spriteRender.Update();

	//火力。
	if (m_player->level == 2)
	{
		karyoku = 2;
	}
	if (m_player->level == 3)
	{
		karyoku = 3;
	}
	if (m_player->level == 4)
	{
		karyoku = 4;
	}
	if (m_player->level == 5)
	{
		karyoku = 5;
	}

	//モデルの更新。
	m_modelRender.Update();
}

void Enemy2::Rotation()
{
	if (sibou == false)
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

		Vector3 diff = m_player->GetPosition() - m_position;

		float angle = atan2(-diff.x, diff.z);
		//atanが返してくる角度はラジアン単位なので
		//SetRotationDegではなくSetRotationを使用する。
		m_rotation.SetRotationY(-angle);


		//回転を設定する。
		m_modelRender.SetRotation(m_rotation);

		//プレイヤーの前ベクトルを計算する。
		m_forward = Vector3::AxisZ;
		m_rotation.Apply(m_forward);
	}
}

void Enemy2::Chase()
{
	//追跡ステートでないなら、追跡処理はしない。
	if (m_enemy2State != enEnemy2State_Chase)
	{
		return;
	}
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//地面についた。
		m_moveSpeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	//ちょっとだけモデルの座標を挙げる。
	modelPosition.y += 2.5f;
	m_modelRender.SetPosition(modelPosition);
}

void Enemy2::Collision()
{
	//被ダメージ、あるいはダウンステートの時は。
	//当たり判定処理はしない。
	if (m_enemy2State == enEnemy2State_ReceiveDamage ||
		m_enemy2State == enEnemy2State_Down)
	{
		return;
	}

	//プレイヤーの攻撃用のコリジョンを取得する。
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//コリジョンの配列をfor文で回す。
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_charaCon))
		{
			//HPを1減らす。
			m_hp -= karyoku;

			//音を読み込む。
			g_soundEngine->ResistWaveFileBank(3, "Assets/sound/3damage.wav");
			//効果音を再生する。
			SoundSource* damagese = NewGO<SoundSource>(0);
			damagese->Init(3);
			damagese->Play(false);
			damagese->SetVolume(1.5f);

			//もしHPが0より上なら。
			if (m_hp > 0)
			{
				sibou = false;
				//被ダメージステートに遷移する。
				m_enemy2State = enEnemy2State_ReceiveDamage;
			}
			//HPが0なら。
			else if (m_hp <= 0)
			{
				sibou = true;
				//ダウンステートに遷移する。
				m_enemy2State = enEnemy2State_Down;
			}

			return;
		}
	}

}



const bool Enemy2::SearchPlayer() const
{
	Vector3 diff = m_player->GetPosition() - m_position;

	//プレイヤーにある程度近かったら.。
	if (diff.LengthSq() <= 1000.0f * 1000.0f)
	{
		//エネミーからプレイヤーに向かうベクトルを正規化する。
		diff.Normalize();
		//エネミーの正面のベクトルと、エネミーからプレイヤーに向かうベクトルの。
		//内積(cosθ)を求める。
		float cos = m_forward.Dot(diff);
		//内積(cosθ)から角度(θ)を求める。
		float angle = acosf(cos);
		//角度(θ)が120°より小さければ。
		if (angle <= (Math::PI / 180.0f) * 135.0f)
		{
			//プレイヤーを見つけた！
			return true;
		}
	}
	//プレイヤーを見つけられなかった。
	return false;
}



void Enemy2::MakeFireBall()
{
	//ファイヤーボールを作成する。
	Magic* fireBall = NewGO<Magic>(0);
	Vector3 fireBallPosition = m_position;
	//座標を少し上にする。
	fireBallPosition.y += 70.0f;
	//座標を設定する。
	fireBall->SetPosition(fireBallPosition);
	fireBall->SetRotation(m_rotation);
	//術者はプレイヤーにする。
	fireBall->SetEnMagician(Magic::enMagician_Enemy2);

}

void Enemy2::ProcessCommonStateTransition()
{
	//各タイマーを初期化。
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	//エネミーからプレイヤーに向かうベクトルを計算する。
	Vector3 diff = m_player->GetPosition() - m_position;
//	Vector3 diff = m_position - m_player->GetPosition();
	

//	m_rotation.SetRotationY(atan2(diff.x , diff.z));

	//回転を設定する。
//	m_modelRender.SetRotation(m_rotation);

	//プレイヤーを見つけたら。
	if (SearchPlayer() == true)
	{
		//ベクトルを正規化する。
		diff.Normalize();
		//移動速度を設定する。
		m_moveSpeed -= diff * 100.0f;


		//攻撃できる距離なら。
		if (IsCanAttack() == true)
		{
			//乱数によって、攻撃するか待機させるかを決定する。	
			int ram = rand() % 100;
			if (ram > 50)
			{
				//攻撃ステートに遷移する。
				m_enemy2State = enEnemy2State_MagicAttack;
				m_isUnderAttack = false;
				return;
			}
			else
			{
				//追跡ステートに遷移する。
				m_enemy2State = enEnemy2State_Chase;
				return;
			}

		}
		//攻撃できない距離なら。
		else
		{
			//乱数によって、追跡させるか魔法攻撃をするか決定する。	
			int ram = rand() % 100;
			if (ram > 30)
			{
				//追跡ステートに遷移する。
				m_enemy2State = enEnemy2State_Chase;
				return;
			}
			else {
				//現在が魔法攻撃ステートなら。
				if (m_enemy2State == enEnemy2State_MagicAttack)
				{
					//連続で魔法を撃たせないように。
					//追跡ステートに遷移する。
					m_enemy2State = enEnemy2State_Chase;
					return;
				}
				//現在が魔法攻撃ステートでないなら。
				else
				{
					//魔法攻撃ステートに遷移する。
					m_enemy2State = enEnemy2State_MagicAttack;
					return;
				}
			}
		}
	}
	//プレイヤーを見つけられなければ。
	else
	{
		//待機ステートに遷移する。
		m_enemy2State = enEnemy2State_Idle;
		return;

	}
}

void Enemy2::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//待機時間がある程度経過したら。
	if (m_idleTimer >= 0.6f)
	{
		//他のステートへ遷移する。
		ProcessCommonStateTransition();
	}

}

void Enemy2::ProcessWalkStateTransition()
{
	//他のステートに遷移する。
	ProcessCommonStateTransition();
}

void Enemy2::ProcessChaseStateTransition()
{
	//攻撃できる距離なら。
	if (IsCanAttack() == true)
	{
		//他のステートに遷移する。
		ProcessCommonStateTransition();
		return;
	}
	m_chaseTimer += g_gameTime->GetFrameDeltaTime();
	//追跡時間がある程度経過したら。
	if (m_chaseTimer >= 0.5f)
	{
		//他のステートに遷移する。
		ProcessCommonStateTransition();
	}
}



void Enemy2::ProcessMagicAttackStateTransition()
{

	//魔法攻撃アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する。
		ProcessCommonStateTransition();
	}
}

void Enemy2::ProcessReceiveDamageStateTransition()
{
	//被ダメージアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//攻撃されたら距離関係無しに、取り敢えず追跡させる。
		m_enemy2State = enEnemy2State_Chase;
		Vector3 diff = m_player->GetPosition() - m_position;
		diff.Normalize();
		//移動速度を設定する。
		m_moveSpeed -= diff * 100.0f;
	}
}

void Enemy2::ProcessDownStateTransition()
{
	//ダウンアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		Game* game = FindGO<Game>("game");
		int radm = rand() % 100;
		if (radm >= 70) {
			Portion* portion = NewGO<Portion>(0,"portion");
			portion->SetPosition(m_position);
		}
		//音を読み込む。
		g_soundEngine->ResistWaveFileBank(1, "Assets/sound/1sibouzi.wav");
		//効果音を再生する。
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(1);
		se->Play(false);
		se->SetVolume(1.5f);
		//倒されたエネミーの数を+1する。
		game->AddDefeatedEnemyNumber();
		game->SetnumEnemydesu();
		
/*		int ram = rand() % 100;

		if (ram > 20)
		{
			Portion* portion = NewGO<Portion>(0, "portion");
		}
*/

		//自身を削除する。
		DeleteGO(this);
	}
}



void Enemy2::ManageState()
{
	switch (m_enemy2State)
	{
		//待機ステートの時。
	case enEnemy2State_Idle:
		//待機ステートのステート遷移処理。
		ProcessIdleStateTransition();
		break;
		//追跡ステートの時。
	case enEnemy2State_Chase:
		//追跡ステートのステート遷移処理。
		ProcessChaseStateTransition();
		break;
		//魔法攻撃ステートの時。
	case enEnemy2State_MagicAttack:
		//魔法攻撃ステートのステート遷移処理。
		ProcessMagicAttackStateTransition();
		break;
		//被ダメージステートの時。
	case enEnemy2State_ReceiveDamage:
		//被ダメージステートのステート遷移処理。
		ProcessReceiveDamageStateTransition();
		break;
		//ダウンステートの時。
	case enEnemy2State_Down:
		//ダウンステートのステート遷移処理。
		ProcessDownStateTransition();
		break;
	}
}

void Enemy2::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_enemy2State)
	{
		//待機ステートの時。
	case enEnemy2State_Idle:
		//待機アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//追跡ステートの時。
	case enEnemy2State_Chase:
		m_modelRender.SetAnimationSpeed(1.2f);
		//走りアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
		//魔法攻撃ステートの時。
	case enEnemy2State_MagicAttack:
		m_modelRender.SetAnimationSpeed(1.5f);
		//魔法攻撃アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_MagicAttack, 0.1f);
		break;
		//被ダメージステートの時。
	case enEnemy2State_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(1.5f);
		//被ダメージアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
		//ダウンステートの時。
	case enEnemy2State_Down:
		//ダウンアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	default:
		break;
	}
}

void Enemy2::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//キーの名前が「magic_attack」の時。
	if (wcscmp(eventName, L"magic_attack") == 0) {
		//ファイヤーボールのオブジェクトを作成する。
		MakeFireBall();
	}
}

const bool Enemy2::IsCanAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//エネミーとプレイヤーの距離が近かったら。
	if (diff.LengthSq() <= 600.0f * 600.0f)
	{
		//攻撃できる！
		return true;
	}
	return false;
}


void Enemy2::MODEL()
{
	m_model = 1;

}

void Enemy2::Render(RenderContext& rc)
{
	if (model == true && modeltimer > 2.5f)
	{
		//モデルを描画する。
		m_modelRender.Draw(rc);
	}
	else {
		return;
	}

	//画像を描画する。
	if (m_isShowHPBar == true) {
		m_spriteRender.Draw(rc);
	}
	else {
		return;
	}
}