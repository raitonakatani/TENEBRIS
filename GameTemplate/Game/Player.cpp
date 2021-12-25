#include "stdafx.h"
#include "Player.h"
#include"fastPlayer.h"
#include "Telop.h"

#include "Game.h"
#include "Menu.h"
#include "WarpCounter.h"


//CollisionObjectを使用するために、ファイルをインクルードする。
#include "collision/CollisionObject.h"

//EffectEmitterを使用するために、ファイルをインクルードする。
#include "graphics/effect/EffectEmitter.h"

Player::~Player()
{
	DeleteGO(m_telop);
}
bool Player::Start()
{
	//アニメーションをロードする。
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData2/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData2/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData2/Walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData2/damage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData2/attack2.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Attack2].Load("Assets/animData2/attack2.tka");
	m_animationClips[enAnimationClip_Attack2].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Heal].Load("Assets/animData2/drink.tka");
	m_animationClips[enAnimationClip_Heal].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Death].Load("Assets/animData2/Death.tka");
	m_animationClips[enAnimationClip_Death].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Roll].Load("Assets/animData2/Rowling.tka");
	m_animationClips[enAnimationClip_Roll].SetLoopFlag(false);


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
		OnAnimationEvent(clipName, eventName);});


	m_HPberRender.Init("Assets/sprite/HP.dds", 512.0f, 32.0f);
	//表示する座標を設定する。
	m_HPberRender.SetPosition({ -694.0f,500.0f ,0.0f });
	m_HPRender.SetPivot({ 0.0f, 0.5f });

	m_HPRender.Init("Assets/sprite/HP.dds", 512.0f, 32.0f);
	//表示する座標を設定する。
	m_HPRender.SetPosition({ -950.0f,500.0f ,0.0f });
	m_HPRender.SetPivot({ 0.0f, 0.5f });

	m_stmnberRender.Init("Assets/sprite/HP.dds", 512.0f, 32.0f);
	//表示する座標を設定する。
	m_stmnberRender.SetPosition({ -694.0f,430.0f ,0.0f });
	m_HPRender.SetPivot({ 0.0f, 0.5f });

	m_staminaRender.Init("Assets/sprite/HP.dds", 512.0f, 32.0f);
	//表示する座標を設定する。
	m_staminaRender.SetPosition({ -950.0f,430.0f ,0.0f });
	m_staminaRender.SetPivot({ 0.0f, 0.5f });

	m_portionRender.Init("Assets/sprite/Portion.dds", 200, 200);
	m_portionRender.SetPosition({ 800.0f, -300.0f, 0.0f });

	m_statusRender.Init("Assets/sprite/status.dds", 553, 324);
	m_statusRender.SetPosition({ -680.0f, 380.0f, 0.0f });

	m_game = FindGO<Game>("game");

	m_telop = NewGO<Telop>(0, "telop");


	//エフェクトを読み込む。
	EffectEngine::GetInstance()->ResistEffect(2, u"Assets/effect/efk/Heal.efk");
	EffectEngine::GetInstance()->ResistEffect(3, u"Assets/effect/efk/Hit2.efk");

	//サウンドを読み込む。
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/9Step.wav");

	//「Sword」ボーンのID(番号)を取得する。
	m_sword_jointBoneId = m_modelRender.FindBoneID(L"sword_joint");
	m_sword_jointBoneId2 = m_modelRender.FindBoneID(L"sword");

	m_pointLight = g_sceneLight->NewPointLight();

	m_pointLight->SetColor(Vector3::One * 0.9f);
	m_pointLight->SetAffectPowParam(0.2f);
	m_pointLight->SetRange(125.0f);
	m_pointLight->SetPosition(m_position + Vector3(0.0f,70.0f,0.0f));

	return true;
}

void Player::Update()
{
	//MakeAttackEffect();
	//m_isEffect = true;

	warpCounter = FindGO<WarpCounter>("WarpCounter");

	if (warpCounter->warpCounter != 0)
	{
		//移動処理。
		Move();
		//回転処理。
		Rotation();
		//攻撃処理。
		Attack();
		//回復処理。
		Heal();
		//当たり判定。
		Collision();
		//アニメーションの再生。
		PlayAnimation();
		//ステートの遷移処理。
		ManageState();

	}
		//モデルの更新。
		m_modelRender.Update();


		if (m_menu == true)
		{
			m_HPberRender.SetPosition({ 331.0f,380.0f ,0.0f });
			m_HPRender.SetPosition({ 75.0f,380.0f ,0.0f });
			m_stmnberRender.SetPosition({ 331.0f,260.0f ,0.0f });
			m_staminaRender.SetPosition({ 75.0f,260.0f ,0.0f });
		}
		else {
			m_HPberRender.SetPosition({ -694.0f,500.0f ,0.0f });
			m_HPRender.SetPosition({ -950.0f,500.0f ,0.0f });
			m_stmnberRender.SetPosition({ -694.0f,430.0f ,0.0f });
			m_staminaRender.SetPosition({ -950.0f,430.0f ,0.0f });
		}


	wchar_t wcsbuf2[256];
	swprintf_s(wcsbuf2, 256, L"Level:%d", int(level));
	m_fontRender2.SetText(wcsbuf2);
	//表示する座標を設定する。
	m_fontRender2.SetPosition({ -950.0f,320.0f,0.0f });
	//文字の大きさを変える。
	//fontRender.SetScale(1.5f);
	//表示する色を設定する。
	m_fontRender2.SetColor(g_vec4Black);

	wchar_t wcsbuf3[256];
	swprintf_s(wcsbuf3, 256, L"x%d", int(m_portion));
	m_fontRender3.SetText(wcsbuf3);
	//表示する座標を設定する。
	m_fontRender3.SetPosition({ 860.0f,-380.0f,0.0f });
	//文字の大きさを変える。
	m_fontRender.SetScale(0.5f);
	//表示する色を設定する。
	m_fontRender3.SetColor(g_vec4White);

	m_HPberRender.SetScale({ hpber,1.0f,0.0f });
	m_HPberRender.Update();
	m_stmnberRender.SetScale({ stmnber,1.0f,0.0f });
	m_stmnberRender.Update();

	//HPバー
/*	if (m_hp <= 0.0f)
	{
		m_hp = 0.0f;
	}
*/
	if (m_hp >= 200.0f)
	{
		m_hp = 200.0f;
	}

	life = m_hp/ 100.0f;
	m_HPRender.SetScale({ life, 1.0f, 0.0f });
	m_HPRender.Update();

	if (m_sutamina <= 0.0f)
	{
		m_sutamina = 0.0f;
	}
	if (m_sutamina == 0.0f)
	{
		COOLtime = true;
	}
	if (m_sutamina >= 150.0f)
	{
		m_sutamina = 150.0f;
	}

	if (COOLtime == true)
	{
		cooltime += g_gameTime->GetFrameDeltaTime();
		if (m_sutamina >= 150.0f)
		{
			COOLtime = false;
		}
	}
	else {
		cooltime = 0.0f;
	}

	if (kaihuku == true)
	{
		m_hp++;
		m_sutamina++;
		if (m_hp == genHP + 50 || m_hp >= 200.0f)
		{
			kaihuku = false;
		}
	}
	else {
		genHP = m_hp;
	}

	m_hurusutamina = m_sutamina / 150.0f;
	m_staminaRender.SetScale({ m_hurusutamina,1.0f,0.0f });
	m_staminaRender.Update();

	m_portionRender.Update();
	m_statusRender.Update();

	m_pointLight->SetPosition(m_position + Vector3(0.0f, 100.0f, 0.0f));



	if (g_pad[0]->IsTrigger(enButtonStart)		//Startボタンが押された。
		&& m_menu == false)					//かつm_menu==falseの時。
	{
		m_menu = true;
		menu = NewGO<Menu>(0);
	}
	else if (g_pad[0]->IsTrigger(enButtonStart)		//Startボタンが押された。
		&& m_menu == true)					//かつm_menu==trueの時。 
	{
		m_menu = false;
		DeleteGO(menu);
	}




	if (warpCounter->warpCounter == 0) {
		return;
	}
}

void Player::Move()
{
	//移動できない状態であれば、移動処理はしない。
	if (IsEnableMove() == false)
	{
		//攻撃
		return;
	}

	if (IsEnableMove() == true) {
		if (m_menu == true)
		{
			m_playerState = enPlayerState_Idle;
			m_moveSpeed.x = 0.0f;
			m_moveSpeed.z = 0.0f;
			m_sutamina++;
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


	timer += g_gameTime->GetFrameDeltaTime();
	if (g_pad[0]->IsTrigger(enButtonB)
		&& m_roll == false
		&& m_sutamina >= 30.0f)
	{
		timer = 0.0f;
		m_roll = true;
		m_sutamina -= 30.0f;
	}

	if (m_roll == true)
	{
		if (timer <= 0.2f)
		{
			m_moveSpeed += m_forward * 200.0f;	//奥方向への移動速度を加算。
		}
		if (timer > 0.2f && timer <= 1.2f)
		{
			m_moveSpeed += m_forward * 500.0f;	//奥方向への移動速度を加算。
		}
		else if (timer >= 1.2f)
		{
			m_roll = false;
		}
	}
	else {
		if (COOLtime == true)
		{
			m_moveSpeed += cameraForward * lStick_y * 250.0f;	//奥方向への移動速度を加算。
			m_moveSpeed += cameraRight * lStick_x * 250.0f;		//右方向への移動速度を加算。
			m_sutamina++;
		}
		if (COOLtime == false)
		{
			if (g_pad[0]->IsPress(enButtonLB1) && m_sutamina > 0)
			{
				m_moveSpeed += cameraForward * lStick_y * 600.0f;	//奥方向への移動速度を加算。
				m_moveSpeed += cameraRight * lStick_x * 600.0f;		//右方向への移動速度を加算。
				if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
				{
					m_sutamina--;
				}
				else {
					m_sutamina++;
				}
			}
			else
			{
				m_sutamina++;
				m_moveSpeed += cameraForward * lStick_y * 300.0f;	//奥方向への移動速度を加算。
				m_moveSpeed += cameraRight * lStick_x * 300.0f;		//右方向への移動速度を加算。
			}
		}
	}

/*	if (m_playerState == enPlayerState_Roll)
	{
		m_charaCon.SetPosition({
			m_position.x,
			m_position.y - 60.0f,
			m_position.z });
	}
	else {
		m_charaCon.SetPosition(m_position);
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
	if (m_playerState == enPlayerState_ReceiveDamage||
		m_playerState == enPlayerState_Death)
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

			      

			//HPが0なら。
			if (m_hp <= 0)
			{
				//被ダメージステートに遷移する。
				m_playerState = enPlayerState_Death;
			}
			//もしHPが0より上なら。
			else
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
				m_playerState = enPlayerState_Death;
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
				m_playerState = enPlayerState_Death;
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
				m_playerState = enPlayerState_Death;
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
		//MakeAttackEffect();
		//攻撃用のコリジョンを作成する。
		MakeAttackCollision();
	}

	//攻撃判定中であれば。
	if (m_isEffect == true)
	{
		MakeAttackEffect();
	}
	
}

void Player::Heal()
{
	//回復中でないなら、処理をしない。
	if (m_playerState != enPlayerState_Heal)
	{
		return;
	}

	//回復判定中であれば。
	if (m_isUnderHeal == true)
	{
		//回復する。
		kaihuku = true;
		//m_hp += 50;
		m_portion--;
		m_isUnderHeal = false;
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
	collisionObject->CreateBox(collisionPosition,				   //座標。
		Quaternion::Identity,                                      //回転。
		Vector3(250.0f, 35.0f, 35.0f)                              //大きさ。
	);
	collisionObject->SetName("player_attack");

	//「Sword」ボーンのワールド行列を取得する。
	Matrix matrix = m_modelRender.GetBone(m_sword_jointBoneId)->GetWorldMatrix();
	//「Sword」ボーンのワールド行列をコリジョンに適用する。
	collisionObject->SetWorldMatrix(matrix);
}

void Player::MakeAttackEffect()
{

	//エフェクトのオブジェクトを作成する。
	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->Init(3);
	//エフェクトの大きさを設定する。
	m_effectEmitter->SetScale({ 5.0f,5.0f,20.0f });
	Vector3 effectposition = m_position;
	effectposition += m_forward * 200.0f;
	//effectposition.y += 100.0f;
	m_effectEmitter->SetPosition(effectposition);
	Quaternion rotation;
	//プレイヤーの回転。
	rotation = m_rotation;
	//回転させる。
	rotation.AddRotationDegY(360.0f);
	//rotation.AddRotationDegZ(180.0f);
	//回転を設定する。
	m_effectEmitter->SetRotation(rotation);
	m_effectEmitter->Play();

	//「Sword」ボーンのワールド行列を取得する。
	Matrix matrix = m_modelRender.GetBone(m_sword_jointBoneId)->GetWorldMatrix();
	//「Sword」ボーンのワールド行列をコリジョンに適用する。
	m_effectEmitter->SetWorldMatrix(matrix);

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
		m_isEffect = false;
		return;
	}
	
	if (g_pad[0]->IsTrigger(enButtonX) && m_portion >= 1)
	{


		//m_playerState = enPlayerState_Attack;
		m_playerState = enPlayerState_Heal;

		//エフェクトのオブジェクトを作成する。
		m_effectEmitter = NewGO <EffectEmitter>(0);
		m_effectEmitter->Init(2);
		m_effectEmitter->SetPosition(m_position);
		//エフェクトの大きさを設定する。
		m_effectEmitter->SetScale(m_scale * 5.0f);
		m_effectEmitter->Play();

		//フラグをtrueにする。
		m_isUnderHeal = false;
		return;
	}


/*
	
	if (g_pad[0]->IsTrigger(enButtonX))
	{
		MakePushLeverCollision();
	}
*/

	if (m_roll == true)
	{
		m_playerState = enPlayerState_Roll;
	}
	else {
		//xかzの移動速度があったら(スティックの入力があったら)。
		if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
		{
			if (COOLtime == true) {
				m_playerState = enPlayerState_Walk;
			}
			if (COOLtime == false)
			{
				//移動速度が一定以上だったら。
				if (g_pad[0]->IsPress(enButtonLB1))
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
		}
		//xとzの移動速度が無かったら(スティックの入力が無かったら)。
		else
		{
			//ステートを待機にする。
			m_playerState = enPlayerState_Idle;
			return;
		}
	}
}

//待機。
void Player::ProcessIdleStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}

//走り。
void Player::ProcessRunStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}

//歩く。
void Player::ProcessWalkStateTransition()
{
	//ステートを遷移する。
	ProcessCommonStateTransition();
}

//回復。
void Player::ProcessHealStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false) {
		//ステートを遷移する。
		ProcessCommonStateTransition();
	}
}

//攻撃。
void Player::ProcessAttackStateTransition()
{
	//攻撃アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//ステートを遷移する。
		ProcessCommonStateTransition();
	}
}

//ダメージ。
void Player::ProcessDamageStateTransition()
{
	//m_timer += g_gameTime->GetFrameDeltaTime();

	//被ダメージアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false
		/*&& m_timer >= 0.3f*/)
	{
		//攻撃されたら、待機アニメーションにさせる。
		m_playerState = enPlayerState_Idle;
		//m_timer = 0.0f;
	}
}

void Player::ProcessDeathStateTransition()
{
	//ステートを遷移する。
	//ProcessCommonStateTransition();
}

void Player::ProcessRowlingStateTransition()
{
	ProcessCommonStateTransition();
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
		//回復ステートの時。
	case enPlayerState_Heal:
		//攻撃ステートのステート遷移処理。
		ProcessHealStateTransition();
		break;
	case enPlayerState_Death:
		ProcessDeathStateTransition();
		break;
	case enPlayerState_Roll:
		ProcessRowlingStateTransition();
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
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.2f);
		break;
		//走りステートの時。
	case enPlayerState_Run:
		m_modelRender.SetAnimationSpeed(1.2f);
		//走りアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.2f);
		break;
	case enPlayerState_Walk:
		m_modelRender.SetAnimationSpeed(1.5f);
		//歩きアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
		//被ダメージステートの時。
	case enPlayerState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(0.8f);
		//被ダメージアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.2f);
		break;
		//攻撃ステートの時。
	case enPlayerState_Attack:
		m_modelRender.SetAnimationSpeed(1.2f);
		//攻撃アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.2f);
		break;
	case enPlayerState_Heal:
		m_modelRender.SetAnimationSpeed(2.5f);
		//回復アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Heal, 0.5f);
		break;
	case enPlayerState_Death:
		//m_modelRender.SetAnimationSpeed(0.8f);
		//死亡アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Death, 0.5f);
		break;
	case enPlayerState_Roll:
		m_modelRender.SetAnimationSpeed(1.0f);
		//回避アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Roll, 0.2f);
		break;
	default:
		break;
	}
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

	//キーの名前が「attack_start」の時。
	if (wcscmp(eventName, L"attack_start") == 0) {
		//エフェクトを発生させる。
		//MakeAttackEffect();
		//攻撃中にする。
		m_isUnderAttack = true;
	}
	//キーの名前が「attack_end」の時。
	else if (wcscmp(eventName, L"attack_end") == 0) {
		//攻撃を終わる。
		m_isUnderAttack = false;
	//	MakeAttackEffect();
	}

	//キーの名前が「Heal_start」の時。
	if (wcscmp(eventName, L"Heal_start") == 0) {
		//回復中にする。
		m_isUnderHeal = true;
	}

	if (wcscmp(eventName, L"Walk_step") == 0) {
		//足音。
		 //効果音を再生する。
		SoundSource* stepse = NewGO<SoundSource>(0);
		stepse->Init(9);
		stepse->Play(false);
		stepse->SetVolume(1.5f);
	}

	//キーの名前が「Effect_start」の時。
	if (wcscmp(eventName, L"Effect_start") == 0) {
		//回復中にする。
		m_isEffect = true;
	}
	//キーの名前が「Effect_start」の時。
	if (wcscmp(eventName, L"Effect_end") == 0) {
		//回復中にする。
		m_isEffect = false;
	}
}

void Player::Render(RenderContext& rc)
{

	m_HPberRender.SetMulColor(Vector4(1.0f, 0.0f, 0.0f, 0.9f));
	m_HPberRender.Draw(rc);
	m_stmnberRender.SetMulColor(Vector4(1.0f, 0.0f, 0.0f, 0.9f));
	m_stmnberRender.Draw(rc);

	//モデルを描画する。
	m_modelRender.Draw(rc);
	//画像を描写する。
	m_HPRender.Draw(rc);
	//m_staminaRender.SetMulColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	m_staminaRender.Draw(rc);

	if (m_menu == false)
	{
	//	m_statusRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.9f));
	//	m_statusRender.Draw(rc);
		m_portionRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.9f));
		m_portionRender.Draw(rc);
		//文字を描写する。
		m_fontRender2.Draw(rc);
		m_fontRender3.Draw(rc);
	}
}
