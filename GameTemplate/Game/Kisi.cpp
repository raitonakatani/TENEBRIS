#include "stdafx.h"
#include "Kisi.h"

#include "Game.h"
#include "Player.h"
#include"Portion.h"
#include <stdlib.h>

//CollisionObjectを使用したいため、ファイルをインクルードする。
#include "collision/CollisionObject.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//EffectEmitterを使用するために、ファイルをインクルードする。
#include "graphics/effect/EffectEmitter.h"



/// <summary>
/// エネミー１。
/// 騎士。
/// 物理攻撃のみ。
/// 防御も入れるかも。（今後の進み具合による）
/// </summary>

Kisi::Kisi()
{

}

Kisi::~Kisi()
{

}

bool Kisi::Start()
{
	//アニメーションを読み込む。
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/enemy3/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/enemy3/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/enemy3/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/enemy3/damage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/enemy3/sibou.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);

	//モデルを読み込む。
	m_modelRender.Init("Assets/modelData/Enemy/enemy2.tkm", m_animationClips, enAnimationClip_Num);

	//エフェクトを読み込む。
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/enemy.efk");

	//エフェクトを読み込む。
	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/effect/efk/Magic.efk");

	//座標を設定する。
	Vector3 position = m_position;
	//position.y += 2.5f;
	m_modelRender.SetPosition(position);
	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);
	//大きさを設定する。
	m_modelRender.SetScale(m_scale);

	//キャラクターコントローラーを初期化。
	m_charaCon.Init(
		40.0f,			//半径。
		140.0f,			//高さ。
		m_position		//座標。
	);


	m_player = FindGO<Player>("player");

	m_spriteRender.Init("Assets/sprite/HP.dds", 64.0f, 32.0f);
	//表示する座標を設定する。
	m_spriteRender.SetPosition({ 0.0f,1450.0f ,0.0f });

	//乱数を初期化。
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);


	//剣のボーンのIDを取得する。
	m_swordBoneId = m_modelRender.FindBoneID(L"Sword_joint");

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	return true;
}

void Kisi::Update()
{
	if (m_player->GetPlayerHP() != 0)
	{
		//追跡処理。
		Chase();
		//回転処理。
		Rotation();
		//当たり判定。
		Collision();
		//攻撃処理。
		Attack();
		//アニメーションの再生。
		PlayAnimation();
		//ステートの遷移処理。
		ManageState();
	}

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

	//ベクトルの長さが700.0fより小さかったら。
	if (diff.LengthSq() <= 800.0f * 800.0f)
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

	//モデルの更新。
	m_modelRender.Update();
}

void Kisi::Rotation()
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

	//プレイヤーの前ベクトルを計算する。
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Kisi::Chase()
{
	//追跡ステートでないなら、追跡処理はしない。
	if (m_enemyState != enEnemyState_Chase)
	{
		return;
	}

	//エネミーを移動させる。
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//地面についた。
		//重力を0にする。
		m_moveSpeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	//ちょっとだけモデルの座標を挙げる。
	modelPosition.y += 2.5f;
	//座標を設定する。
	m_modelRender.SetPosition(modelPosition);

}

void Kisi::Collision()
{
	//被ダメージ、あるいはダウンステートの時は。
	//当たり判定処理はしない。
	if (m_enemyState == enEnemyState_ReceiveDamage ||
		m_enemyState == enEnemyState_Down)
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
				//被ダメージステートに遷移する。
				m_enemyState = enEnemyState_ReceiveDamage;
			}
			//HPが0なら。
			else if (m_hp <= 0)
			{
				//ダウンステートに遷移する。
				m_enemyState = enEnemyState_Down;
			}
			return;
		}
	}

}

void Kisi::Attack()
{
	//攻撃ステートでないなら処理をしない。
	if (m_enemyState != enEnemyState_Attack)
	{
		return;
	}

	//攻撃中であれば。
	if (m_isUnderAttack == true)
	{
		//攻撃用のコリジョンを作成する。
		MakeAttackCollision();
	}
}

const bool Kisi::SearchPlayer() const
{
	//エネミーからプレイヤーに向かうベクトルを求める。
	Vector3 diff = m_player->GetPosition() - m_position;

	//プレイヤーにある程度近かったら。
	if (diff.LengthSq() <= 800.0 * 800.0f)
	{
		//エネミーからプレイヤーに向かうベクトルを正規化(大きさを1)する。
		diff.Normalize();
		//エネミーの正面のベクトルと、エネミーからプレイヤーに向かうベクトルの。
		//内積(cosθ)を求める。
		float cos = m_forward.Dot(diff);
		//内積(cosθ)から角度(θ)を求める。
		float angle = acosf(cos);
		//角度(θ)が120°(視野角)より小さければ。
		if (angle <= (Math::PI / 180.0f) * 135.0f)
		{
			//プレイヤーを見つけた！
			return true;
		}
	}
	//プレイヤーを見つけられなかった。
	return false;
}

void Kisi::MakeAttackCollision()
{
	//攻撃当たり判定用のコリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);
	//剣のボーンのワールド行列を取得する。
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
	//ボックス状のコリジョンを作成する。
	collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(200.0f, 15.0f, 15.0f));
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy_attack");
}

void Kisi::ProcessCommonStateTransition()
{
	//各タイマーを初期化。
   //待機時間と追跡時間を制限するため。
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	//エネミーからプレイヤーに向かうベクトルを計算する。
	Vector3 diff = m_player->GetPosition() - m_position;

	//プレイヤーを見つけたら。
	if (SearchPlayer() == true)
	{
		//ベクトルを正規化する。
		diff.Normalize();
		//移動速度を設定する。
		m_moveSpeed = diff * 250.0f;

		Vector3 toPlayerDir = diff;
		m_forward = toPlayerDir;

		m_rotation.SetRotationY(atan2(m_forward.x, m_forward.z));
		m_modelRender.SetRotation(m_rotation);

		if (m_position.y >= 0.001f)
		{
			m_moveSpeed.y = -1000.0f;
		}
		if (m_position.y <= 0.0f)
		{
			m_moveSpeed.y = 0.0f;
		}
		//通常攻撃できない距離なら。
		if (IsCanAttack() == false)
		{
			//乱数によって、追跡させるか魔法攻撃をするか決定する。	
			int ram = rand() % 100;
			//乱数が40以上なら。
			if (ram > 40)
			{
				//追跡ステートに遷移する。
				m_enemyState = enEnemyState_Chase;
				return;
			}
			//乱数が40未満なら。
			else {
				//魔法攻撃ステートに遷移する。
				//追跡
				m_enemyState = enEnemyState_Chase;
				//m_enemyState = enEnemyState_Attack;
				return;
			}
		}
		//攻撃できる距離なら。
		else
		{
			//乱数によって、待機させるか通常攻撃をするか決定する。	
			int ram = rand() % 100;
			if (ram > 30)
			{
				//攻撃ステートに遷移する。
				m_enemyState = enEnemyState_Attack;
				return;
			}
			else
			{
				//待機ステートに遷移する。
				m_enemyState = enEnemyState_Idle;
				return;
			}
		}
	}
	//プレイヤーを見つけられなければ。
	else
	{
		//待機ステートに遷移する。
		m_enemyState = enEnemyState_Idle;
		return;

	}
}

void Kisi::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//待機時間がある程度経過したら。
	if (m_idleTimer >= 0.5f)
	{
		//他のステートへ遷移する。
		ProcessCommonStateTransition();
	}

}

void Kisi::ProcessWalkStateTransition()
{
	//他のステートに遷移する。
	ProcessCommonStateTransition();
}

void Kisi::ProcessRunStateTransition()
{
	//他のステートに遷移する。
	ProcessCommonStateTransition();
}

void Kisi::ProcessChaseStateTransition()
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

void Kisi::ProcessAttackStateTransition()
{
	//攻撃アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する。
		ProcessCommonStateTransition();
	}
}

void Kisi::ProcessReceiveDamageStateTransition()
{
	//被ダメージアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//攻撃されたら距離関係無しに、取り敢えず追跡させる。
		m_enemyState = enEnemyState_Chase;
		Vector3 diff = m_player->GetPosition() - m_position;
		diff.Normalize();
		//移動速度を設定する。
		m_moveSpeed = diff * 250.0f;
	}
}

void Kisi::ProcessDownStateTransition()
{
	//ダウンアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		Game* game = FindGO<Game>("game");
		int radm = rand() % 100;
		if (radm >= 70) {
			Portion* portion = NewGO<Portion>(0, "portion");
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

		/*		if (radm > 20)
				{
					Portion* portion = NewGO<Portion>(0, "portion");
				}
		*/
		//自身を削除する。
		DeleteGO(this);
	}
}

void Kisi::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//キーの名前が「attack_start」の時。
	if (wcscmp(eventName, L"attack_start") == 0) {
		//攻撃中判定をtrueにする。
		m_isUnderAttack = true;
	}
	//キーの名前が「attack_end」の時。
	else if (wcscmp(eventName, L"attack_end") == 0) {
		//攻撃中判定をfalseにする。
		m_isUnderAttack = false;
	}
}

void Kisi::ManageState()
{
	switch (m_enemyState)
	{
		//待機ステートの時。
	case enEnemyState_Idle:
		//待機ステートのステート遷移処理。
		ProcessIdleStateTransition();
		break;
		//追跡ステートの時。
	case enEnemyState_Chase:
		//追跡ステートのステート遷移処理。
		ProcessChaseStateTransition();
		break;
		//攻撃ステートの時。
	case enEnemyState_Attack:
		//攻撃ステートのステート遷移処理。
		ProcessAttackStateTransition();
		break;
		//被ダメージステートの時。
	case enEnemyState_ReceiveDamage:
		//被ダメージステートのステート遷移処理。
		ProcessReceiveDamageStateTransition();
		break;
		//ダウンステートの時。
	case enEnemyState_Down:
		//ダウンステートのステート遷移処理。
		ProcessDownStateTransition();
		break;
	}
}

void Kisi::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_enemyState)
	{
		//待機ステートの時。
	case enEnemyState_Idle:
		//待機アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//追跡ステートの時。
	case enEnemyState_Chase:
		m_modelRender.SetAnimationSpeed(1.2f);
		//走りアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
		//攻撃ステートの時。
	case enEnemyState_Attack:
		m_modelRender.SetAnimationSpeed(1.0f);
		//攻撃アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
		//被ダメージステートの時。
	case enEnemyState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(1.0f);
		//被ダメージアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
		//ダウンステートの時。
	case enEnemyState_Down:
		//ダウンアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	default:
		break;
	}
}

const bool Kisi::IsCanAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//エネミーとプレイヤーの距離が近かったら。
	if (diff.LengthSq() <= 180.0f * 180.0f)
	{
		//攻撃できる！
		return true;
	}
	//攻撃できない。
	return false;
}


void Kisi::MODEL()
{
	m_model = 1;

}

void Kisi::Render(RenderContext& rc)
{
	if (model == true && modeltimer > 2.5f)
	{
		//モデルを描画する。
		m_modelRender.Draw(rc);
	}
	else {
		return;
	}

	if (m_player->GetPlayerHP() != 0)
	{
		if (m_isShowHPBar == true) {
			m_spriteRender.Draw(rc);
		}
		else {
			return;
		}
	}
}