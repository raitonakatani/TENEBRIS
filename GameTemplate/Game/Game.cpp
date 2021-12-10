#include "stdafx.h"
#include "Game.h"

#include "Player.h"
#include"fastPlayer.h"

#include "Kisi.h"
#include "Enemy2.h"
#include "Enemy3.h"
#include "Enemy4.h"

#include "Door.h"
#include "Menu.h"
#include "warp.h"

#include "BackGround.h"
#include "BackGround2.h"

#include "GameCamera.h"
#include "CAMERA.h"

#include "TIMER.h"
#include "Fade.h"
#include "Title.h"
#include "GameClear.h"
#include"Portion.h"
#include "Fire.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

#include "ClearCounter.h"

//EffectEmitterを使用するために、ファイルをインクルードする。
#include "graphics/effect/EffectEmitter.h"

Game::Game()
{

}

Game::~Game()
{
	for (auto door : m_doorVector)
	{
		DeleteGO(door);
	}
	const auto& enemys = FindGOs<Kisi>("enemy");
	for (auto enemy : enemys)
	{
		DeleteGO(enemy);
	}
	const auto& enemys2 = FindGOs<Enemy2>("enemy2");
	for (auto enemy2 : enemys2)
	{
		DeleteGO(enemy2);
	}
	const auto& enemys3 = FindGOs<Enemy3>("enemy3");
	for (auto enemy3 : enemys3)
	{
		DeleteGO(enemy3);
	}

	const auto& portions = FindGOs<Portion>("portion");
	for (auto portion : portions)
	{
		DeleteGO(portion);
	}

	const auto& fires = FindGOs<Fire>("fire");
	for (auto m_fire : fires)
	{
		DeleteGO(m_fire);
	}

	DeleteGO(m_camera);
	DeleteGO(m_player);
	DeleteGO(m_fastplayer);
	DeleteGO(m_skycube);
	DeleteGO(m_background);
	DeleteGO(m_TIMER);
	DeleteGO(GameBGM);


}

bool Game::Start()
{
	/*
	ゲームパッド：キーボード
	Aボタン　　 ：Jボタン
	Bボタン　　 ：Kボタン
	Xボタン　　 ：Lボタン
	Yボタン　　 ：Iボタン
	*/

	clearCounter = FindGO<ClearCounter>("ClearCounter");

	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/6GameBGM.wav");
	//効果音を再生する。
	GameBGM = NewGO<SoundSource>(0);
	GameBGM->Init(6);
	GameBGM->Play(true);
	GameBGM->SetVolume(0.5f);

		m_fastplayer = NewGO<fastPlayer>(0, "fastplayer");
		m_fastplayer->SetPosition({ 0.0f,500.0f,-16500.0f });
		m_fastplayer->SetScale({ 2.5f,2.5f,2.5f });

	//	m_fire = NewGO<Fire>(0, "fire");
	//	m_fire->SetPosition({ 250.0f,500.0f,-14500.0f });
		//m_fire->SetScale({ 2.5f,2.5f,2.5f });

		if (clearCounter->clearCounter == 0|| clearCounter->clearCounter == 1)
		{
			//レベルを構築する。
			m_levelRender.Init("Assets/level/stage.tkl", [&](LevelObjectData& objData) {
				if (objData.EqualObjectName(L"player") == true) {
					//プレイヤーのインスタンスを生成する。
					m_player = NewGO<Player>(0, "player");
					m_player->SetPosition({ objData.position });
					m_player->SetRotation(objData.rotation);
					m_player->SetScale(objData.scale);
					//trueにすると、レベルの方でモデルが読み込まれない。
					return true;
				}
				else if (objData.ForwardMatchName(L"enemy") == true) {
					//エネミー（騎士）のインスタンスを生成する。
					Kisi* kisi = NewGO<Kisi>(0, "enemy");
					kisi->SetPosition(objData.position);
					kisi->SetRotation(objData.rotation);
					kisi->SetScale(objData.scale);
					//番号を設定する。
					kisi->SetkisiNumber(objData.number);
					//trueにすると、レベルの方でモデルが読み込まれない。
					return true;
				}
				else if (objData.ForwardMatchName(L"wizard") == true) {
					//エネミー（魔法使い。）のインスタンスを生成する。
					Enemy2* wizard = NewGO<Enemy2>(0, "enemy2");
					wizard->SetPosition(objData.position);
					wizard->SetRotation(objData.rotation);
					wizard->SetScale(objData.scale);
					wizard->SetwizardNumber(objData.number);
					//trueにすると、レベルの方でモデルが読み込まれない。
					return true;
				}
				else if (objData.ForwardMatchName(L"boss") == true) {
					//ボス（魔法剣士）のインスタンスを生成する。
					Enemy3* boss = NewGO<Enemy3>(0, "enemy3");
					boss->SetPosition(objData.position);
					boss->SetRotation(objData.rotation);
					boss->SetScale(objData.scale);
					boss->SetbossNumber(objData.number);
					//trueにすると、レベルの方でモデルが読み込まれない。
					return true;
				}
				//名前がdoorだったら。
				else if (objData.ForwardMatchName(L"door") == true)
				{
					//ドアのオブジェクトを作成する。
					m_door = NewGO<Door>(0, "door");
					//座標を設定する。
					m_door->SetPosition(objData.position);
					//大きさを設定する。
					m_door->SetScale(objData.scale);
					//回転を設定する。
					m_door->SetRotation(objData.rotation);
					m_door->SetDoorNumber(objData.number);
					//ドアの配列に作成したオブジェクトを加える。
					m_doorVector.push_back(m_door);
					return true;
				}
				if (objData.EqualObjectName(L"BackGround") == true) {

					m_background = NewGO<BackGround>(0, "background");
					m_background->SetPosition(objData.position);
					m_background->SetRotation(objData.rotation);
					m_background->SetScale(objData.scale);
					//falseにすると、レベルの方でモデルが読み込まれて配置される。
					return true;
				}
				if (objData.EqualObjectName(L"Fire") == true) {

					m_fire = NewGO<Fire>(0, "fire");
					m_fire->SetPosition(objData.position);
					m_fire->SetRotation(objData.rotation);
					m_fire->SetScale(objData.scale);
					//falseにすると、レベルの方でモデルが読み込まれて配置される。
					return true;
				}

				return true;
				});
		}

/*		if (clearCounter->clearCounter == 1)
		{
			//レベルを構築する。
			m_levelRender.Init("Assets/level/stage2.tkl", [&](LevelObjectData& objData) {
				if (objData.EqualObjectName(L"player") == true) {
					//プレイヤーのインスタンスを生成する。
					m_player = NewGO<Player>(0, "player");
					m_player->SetPosition({ objData.position });
					m_player->SetRotation(objData.rotation);
					m_player->SetScale(objData.scale);
					//trueにすると、レベルの方でモデルが読み込まれない。
					return true;
				}
				else if (objData.ForwardMatchName(L"enemy") == true) {
					//エネミー（騎士）のインスタンスを生成する。
					Kisi* kisi = NewGO<Kisi>(0, "enemy");
					kisi->SetPosition(objData.position);
					kisi->SetRotation(objData.rotation);
					kisi->SetScale(objData.scale);
					//番号を設定する。
					kisi->SetkisiNumber(objData.number);
					//trueにすると、レベルの方でモデルが読み込まれない。
					return true;
				}
				else if (objData.ForwardMatchName(L"wizard") == true) {
					//エネミー（魔法使い。）のインスタンスを生成する。
					Enemy2* wizard = NewGO<Enemy2>(0, "enemy2");
					wizard->SetPosition(objData.position);
					wizard->SetRotation(objData.rotation);
					wizard->SetScale(objData.scale);
					wizard->SetwizardNumber(objData.number);
					//trueにすると、レベルの方でモデルが読み込まれない。
					return true;
				}
				else if (objData.ForwardMatchName(L"boss") == true) {
					//ボス（魔法剣士）のインスタンスを生成する。
					Enemy3* boss = NewGO<Enemy3>(0, "enemy3");
					boss->SetPosition(objData.position);
					boss->SetRotation(objData.rotation);
					boss->SetScale(objData.scale);
					boss->SetbossNumber(objData.number);
					//trueにすると、レベルの方でモデルが読み込まれない。
					return true;
				}
				//名前がdoorだったら。
				else if (objData.ForwardMatchName(L"door") == true)
				{
					//ドアのオブジェクトを作成する。
					m_door = NewGO<Door>(0, "door");
					//座標を設定する。
					m_door->SetPosition(objData.position);
					//大きさを設定する。
					m_door->SetScale(objData.scale);
					//回転を設定する。
					m_door->SetRotation(objData.rotation);
					m_door->SetDoorNumber(objData.number);
					//ドアの配列に作成したオブジェクトを加える。
					m_doorVector.push_back(m_door);
					return true;
				}
				if (objData.EqualObjectName(L"BackGround") == true) {

					m_background2 = NewGO<BackGround2>(0, "background2");
					m_background2->SetPosition(objData.position);
					m_background2->SetRotation(objData.rotation);
					m_background2->SetScale(objData.scale);
					//falseにすると、レベルの方でモデルが読み込まれて配置される。
					return true;
				}
				return true;
				});
		}

*/

	//m_skycube = NewGO<SkyCube>(0, "skycube");

	m_TIMER = NewGO<TIMER>(0, "m_timer");

	m_Warp = NewGO<warp>(0, "warp");
	m_Warp->SetPosition({ 0.0f,850.0f,-10000.0f });

	g_camera3D->SetPosition({ 0.0f, 100.0f, -600.0f });
	m_camera = NewGO<CAMERA>(0, "m_camera");



	m_spriteRender.Init("Assets/sprite/TENEBRIS.dds", 2050, 1100);
	m_spriteRender.SetPosition(Vector3(0.0f, 30.0f, 0.0f));
	m_spriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.5f));


	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();



	return true;
}


//更新処理。
void Game::Update()
{

	m_spriteRender.Update();


	if (m_isWaitFadeout && m_player->GetBOSS() == 1) {
		if (!m_fade->IsFade()) {
			NewGO<GameClear>(0, "gameclear");
			DeleteGO(this);
		}
	}
	else {
		if (m_player->GetBOSS() == 1)
		{
			clearCounter->clearCounter += 1;
			m_isWaitFadeout = true;
			m_fade->StartFadeOut();
		}
	}



	//if (g_pad[0]->IsPress(enButtonStart))
	if (g_pad[0]->IsTrigger(enButtonStart)		//Startボタンが押された。
		&& m_menu == false)					//かつm_menu==falseの時。
	{
		menu = NewGO<Menu>(0);
		m_menu = true;
	}
	else if (g_pad[0]->IsTrigger(enButtonStart)		//Startボタンが押された。
		&& m_menu == true)					//かつm_menu==trueの時。 
	{
		DeleteGO(menu);
		m_menu = false;
	}


	if (m_numDefeatedEnemy == m_numEnemy)
	{
		m_player->level += 1;
		m_numEnemy += 2;
	}

	if (m_player->level >= 5)
	{
		m_player->level = 5;
	}



	if (m_player->GetPlayerHP() <= 0)
	{
		sibou = true;
	}
	else {
		sibou = false;
	}
	if (sibou == true)
	{
		m_fontRender.SetText(L"GAME OVER");
		//文字の大きさを変える。
		m_fontRender.SetScale(5.0f);
		//表示する座標を設定する。
		m_fontRender.SetPosition(Vector3{ -300.0f,0.0f,0.0f });
		alpha = alpha + 0.02f;
		if (alpha > 1.0f) {
			alpha = 1.0f;
		}
		siboutimer += g_gameTime->GetFrameDeltaTime();

		m_modelRender.Update();



		if (m_isWaitFadeout && siboutimer >= 3.0f) {
			if (!m_fade->IsFade()) {
				NewGO<Title>(0, "title");
				DeleteGO(this);
			}
		}
		else {
			if (siboutimer >= 3.0f)
			{
				m_isWaitFadeout = true;
				m_fade->StartFadeOut();
			}
		}
	}
	else {
		return;
	}


/*	if (m_fastplayer->GetWarp() == 1)//&& m_player != nullptr
	{
		//DeleteGO(m_camera);
		DeleteGO(m_fastplayer);
		m_player->minusWarp();

	//	g_camera3D->SetPosition({ 0.0f, 100.0f, -600.0f });
	//	m_camera->SetPosition({ 0.0f, 200.0f, -200.0f });

		//WARP = true;
	}
	else {
		return;
	}
*/
}

void Game::Render(RenderContext& rc)
{

	m_fontRender.SetColor(Vector4(1.0f, 0.0f, 0.0f, fabsf(sinf(alpha))));
	m_fontRender.Draw(rc);
}