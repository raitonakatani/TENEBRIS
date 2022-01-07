#include "stdafx.h"
#include "Door.h"
#include "Game.h"
#include "Player.h"
#include "Kisi.h"
#include "Enemy2.h"
#include "Enemy3.h"
#include "Enemy4.h"


Door::Door()
{

}

Door::~Door()
{

}

bool Door::Start()
{
	//アニメーションを読み込む。
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/door/door.idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Open].Load("Assets/animData/door/door.open.tka");
	m_animationClips[enAnimationClip_Open].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Close].Load("Assets/animData/door/door.close.tka");
	m_animationClips[enAnimationClip_Close].SetLoopFlag(false);
	//モデルを読み込む。
	m_modelRender.Init("Assets/modelData/door/door.tkm", m_animationClips, enAnimationClip_Num);
	//座標を設定する。
	m_modelRender.SetPosition(m_position);
	//大きさを設定する。
	m_modelRender.SetScale(m_scale);
	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);

	m_telopRender.Init("Assets/sprite/huti.dds", 1200.0f, 400.0f);
	//表示する座標を設定する。
	m_telopRender.SetPosition({ 0.0f,-350.0f ,0.0f });

	m_kaiwa1Render.Init("Assets/sprite/door1.dds", 1024.0f, 512.0f);
	//表示する座標を設定する。
	m_kaiwa1Render.SetPosition({ -160.0f,-350.0f ,0.0f });

	m_player = FindGO<Player>("player");

	m_game = FindGO<Game>("game");

	g_soundEngine->ResistWaveFileBank(8, "Assets/sound/8Door.wav");

	//モデルの更新。
	m_modelRender.Update();
	//当たり判定を作成する。
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	
	return true;
}

void Door::Update()
{
	//アニメーションを再生する。
	PlayAnimation();
	//ステート管理。
	ManageState();

	if (m_player->GetBOSS() == 0)
	{
		auto boss = FindGO<Enemy3>("enemy3");
		Vector3 diff2 = boss->GetPosition() - m_player->GetPosition();
		if (diff2.Length() <= 1200.0f)
		{
			if (m_doorState == enDoorState_Open_Idle)
			{
				boss->PlusBOSSBEYA();
				SoundSource* doorse = NewGO<SoundSource>(0);
				doorse->Init(8);
				doorse->Play(false);
				doorse->SetVolume(2.0f);
				NotifyClose();
			}
		}
	}
	//プレイヤーから☆に向かうベクトルを計算。
	Vector3 DOORPOSI = m_position;
	DOORPOSI.y -= 500.0f;
	Vector3 diff = m_player->GetPosition() - DOORPOSI;
	//ベクトルの長さが120.0fより小さかったら。
	if (diff.Length() <= 300.0f)
	{
		if (m_doorState == enDoorState_Idle)
		{
			door = true;


			/*
			m_game->GetsibouEnemy() == 0 && m_DoorCount == 0 || m_game->GetsibouEnemy() == 0 && m_DoorCount == 1 ||
				m_game->GetsibouEnemy() == 4 && m_DoorCount == 2 ||
				m_game->GetsibouEnemy() == 5 && m_DoorCount == 3 ||
				m_game->GetsibouEnemy() == 10 && m_DoorCount == 4 ||
				m_game->GetsibouEnemy() == 11 && m_DoorCount == 5 ||
				m_game->GetsibouEnemy() == 17 && m_DoorCount == 6 || m_game->GetsibouEnemy() == 17 && m_DoorCount == 7
			*/
			if (m_game->GetsibouEnemy() == 0 ||
				m_game->GetsibouEnemy() == 4 ||
				m_game->GetsibouEnemy() == 5 ||
				m_game->GetsibouEnemy() == 10||
				m_game->GetsibouEnemy() == 11 ||
				m_game->GetsibouEnemy() == 17 
				)
			{
/*				m_fontRender.SetText(L"扉を開ける。\n \n Aボタン");
				//表示する座標を設定する。
				m_fontRender.SetPosition({ -150.0f,-2500.0f,0.0f });
				//文字の大きさを変える。
				//m_fontRender.SetScale(1.5f);
				//表示する色を設定する。
				m_fontRender.SetColor(g_vec4White);
*/
			
					if (g_pad[0]->IsTrigger(enButtonA))
					{
						m_DoorCount += 1;
						SoundSource* doorse = NewGO<SoundSource>(0);
						doorse->Init(8);
						doorse->Play(false);
						doorse->SetVolume(2.0f);

						NotifyOpen();
					}
			}
			else {
				m_fontRender.SetText(L"カギが閉まっている");
				//表示する座標を設定する。
				m_fontRender.SetPosition({ -150.0f,-250.0f,0.0f });
				//文字の大きさを変える。
				//m_fontRender.SetScale(1.5f);
				//表示する色を設定する。
				m_fontRender.SetColor(g_vec4White);
			}
		}
		else
		{
			door = false;
		}
	}
	else {
		door = false;
	}

	//モデルの更新。
	m_modelRender.Update();
	m_kaiwa1Render.Update();
	m_telopRender.Update();
}

void Door::NotifyOpen()
{
	//オープンステートに遷移する。
	m_doorState = enDoorState_Open;
}

void Door::NotifyClose()
{
	//クローズステートに遷移する。
	m_doorState = enDoorState_Close;
}

void Door::PlayAnimation()
{
	switch (m_doorState)
	{
		//待機ステートの時。
	case enDoorState_Idle:
		//待機アニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
		break;
		//オープンステートの時。
	case enDoorState_Open:
		//オープンアニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_Open);
		m_modelRender.SetAnimationSpeed(0.8f);
		break;
		//クローズステートの時。
	case enDoorState_Close:
		//クローズアニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_Close);
		m_modelRender.SetAnimationSpeed(0.8f);
		break;
	default:
		break;
	}
}

void Door::ReleasePhysicsObject()
{
	//当たり判定を開放する。
	m_physicsStaticObject.Release();
}

void Door::CreatePhysicsObject()
{
	//当たり判定を作成する。
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
}

void Door::ProcessOpenStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == true) {
		//当たり判定を開放する。
		ReleasePhysicsObject();


		auto bosss = FindGOs<Enemy3>("enemy3");
		for (auto boss : bosss) {
			if(m_game->GetBGM()==0 && m_doorNumber == boss->GetbossNumber())
			m_game->SetBGM();
		}
	}

	//オープンアニメーションの再生が終了したら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//当たり判定を開放する。
		//ReleasePhysicsObject();
		//オープン終わりステートに遷移する。
		m_doorState = enDoorState_Open_Idle;

		auto enemys = FindGOs<Kisi>("enemy");
		for (auto enemy : enemys)
		{
			for (int i = 0;i < 3;i++) {
				//騎士を召喚。
				if (m_doorNumber == enemy->GetkisiNumber())
				{

					enemy->MODEL();
					break;
				}
			}
		}

		auto wizards = FindGOs<Enemy2>("enemy2");
		for (auto wizard : wizards) {
			for (int i = 0;i < 3;i++) {
				//魔法使いを召喚。
				if (m_doorNumber == wizard->GetwizardNumber())
				{
					wizard->MODEL();
					break;
				}
			}
		}

		auto bosss = FindGOs<Enemy3>("enemy3");
		for (auto boss : bosss) {

			//ボスを召喚。
			if (m_doorNumber == boss->GetbossNumber())
			{
				boss->MODEL();
				break;
			}
		}

		/*	//中ボスを召喚。
			if (m_doorNumber == tyuuboss->GettyuubossNumber())
			{
				tyuuboss->MODEL();
			}
		*/
	}
}

void Door::ProcessCloseStateTransition()
{
	//クローズアニメーションが終了したら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//当たり判定を作成する。
		CreatePhysicsObject();
		//クローズ終わりステートに遷移する。
		m_doorState = enDoorState_Close_Idle;
	}
}

void Door::ManageState()
{
	switch (m_doorState)
	{
		//待機ステートの時。
	case Door::enDoorState_Idle:

		break;
		//オープンステートの時。
	case Door::enDoorState_Open:
		//オープンステートのステート遷移処理。
		ProcessOpenStateTransition();
		break;
		//オープン終わりステートの時。
	case Door::enDoorState_Open_Idle:

		break;
		//クローズステートの時。
	case Door::enDoorState_Close:
		//クローズステートのステート遷移処理。
		ProcessCloseStateTransition();
		break;
		//クローズ終わりステートの時。
	case Door::enDoorState_Close_Idle:

		break;
	default:
		break;
	}
}

void Door::Render(RenderContext& rc)
{
	//モデルを描画する。
	m_modelRender.Draw(rc);

	//ドアに近づいたとき。
	if (door == true)
	{

		m_telopRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.7f));
		m_telopRender.Draw(rc);
		//文字を描写する。
		m_kaiwa1Render.Draw(rc);
		m_fontRender.Draw(rc);

	}
	else {
		return;
	}
}

