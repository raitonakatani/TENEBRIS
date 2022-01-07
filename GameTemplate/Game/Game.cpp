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

//EffectEmitter���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "graphics/effect/EffectEmitter.h"


#include "MovingFloor.h"

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

	if (BGMchange == 0) {
		DeleteGO(GameBGM);
	}

	if (BGMchange == 3)
	{
		DeleteGO(BossBGM);
	}
}

bool Game::Start()
{
	/*
	�Q�[���p�b�h�F�L�[�{�[�h
	A�{�^���@�@ �FJ�{�^��
	B�{�^���@�@ �FK�{�^��
	X�{�^���@�@ �FL�{�^��
	Y�{�^���@�@ �FI�{�^��
	*/

	clearCounter = FindGO<ClearCounter>("ClearCounter");

	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/6GameBGM.wav");
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/7BossBGM.wav");

	//���ʉ����Đ�����B
	GameBGM = NewGO<SoundSource>(0);
	GameBGM->Init(6);
	GameBGM->Play(true);
	GameBGM->SetVolume(0.5f);


	BossBGM = NewGO<SoundSource>(1);
	BossBGM->Init(7);

		m_fastplayer = NewGO<fastPlayer>(0, "fastplayer");
		m_fastplayer->SetPosition({ 0.0f,500.0f,-16500.0f });
		m_fastplayer->SetScale({ 2.5f,2.5f,2.5f });

	//	m_fire = NewGO<Fire>(0, "fire");
	//	m_fire->SetPosition({ 250.0f,500.0f,-14500.0f });
		//m_fire->SetScale({ 2.5f,2.5f,2.5f });

		if (clearCounter->clearCounter == 0|| clearCounter->clearCounter == 1)
		{
			//���x�����\�z����B
			m_levelRender.Init("Assets/level/stage.tkl", [&](LevelObjectData& objData) {
				if (objData.EqualObjectName(L"player") == true) {
					//�v���C���[�̃C���X�^���X�𐶐�����B
					m_player = NewGO<Player>(0, "player");
					m_player->SetPosition({ objData.position });
					m_player->SetRotation(objData.rotation);
					m_player->SetScale(objData.scale);
					//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
					return true;
				}
				else if (objData.ForwardMatchName(L"enemy") == true) {
					//�G�l�~�[�i�R�m�j�̃C���X�^���X�𐶐�����B
					Kisi* kisi = NewGO<Kisi>(0, "enemy");
					kisi->SetPosition(objData.position);
					kisi->SetRotation(objData.rotation);
					kisi->SetScale(objData.scale);
					//�ԍ���ݒ肷��B
					kisi->SetkisiNumber(objData.number);
					//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
					return true;
				}
				else if (objData.ForwardMatchName(L"wizard") == true) {
					//�G�l�~�[�i���@�g���B�j�̃C���X�^���X�𐶐�����B
					Enemy2* wizard = NewGO<Enemy2>(0, "enemy2");
					wizard->SetPosition(objData.position);
					wizard->SetRotation(objData.rotation);
					wizard->SetScale(objData.scale);
					wizard->SetwizardNumber(objData.number);
					//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
					return true;
				}
				else if (objData.ForwardMatchName(L"boss") == true) {
					//�{�X�i���@���m�j�̃C���X�^���X�𐶐�����B
					Enemy3* boss = NewGO<Enemy3>(0, "enemy3");
					boss->SetPosition(objData.position);
					boss->SetRotation(objData.rotation);
					boss->SetScale(objData.scale);
					boss->SetbossNumber(objData.number);
					//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
					return true;
				}
				//���O��door��������B
				else if (objData.ForwardMatchName(L"door") == true)
				{
					//�h�A�̃I�u�W�F�N�g���쐬����B
					m_door = NewGO<Door>(0, "door");
					//���W��ݒ肷��B
					m_door->SetPosition(objData.position);
					//�傫����ݒ肷��B
					m_door->SetScale(objData.scale);
					//��]��ݒ肷��B
					m_door->SetRotation(objData.rotation);
					m_door->SetDoorNumber(objData.number);
					//�h�A�̔z��ɍ쐬�����I�u�W�F�N�g��������B
					m_doorVector.push_back(m_door);
					return true;
				}
			if (objData.EqualObjectName(L"BackGround") == true) {

					m_background = NewGO<BackGround>(0, "background");
					m_background->SetPosition(objData.position);
					m_background->SetRotation(objData.rotation);
					m_background->SetScale(objData.scale);
					//false�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ĕz�u�����B
					return true;
				}
				if (objData.EqualObjectName(L"Fire") == true) {

					m_fire = NewGO<Fire>(0, "fire");
					m_fire->SetPosition(objData.position);
					m_fire->SetRotation(objData.rotation);
					m_fire->SetScale(objData.scale);
					//false�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ĕz�u�����B
					return true;
				}

				return true;
				});
		}

	//m_skycube = NewGO<SkyCube>(0, "skycube");

	//m_TIMER = NewGO<TIMER>(0, "m_timer");

	m_Warp = NewGO<warp>(0, "warp");
	m_Warp->SetPosition({ 0.0f,800.0f,-10000.0f });

	g_camera3D->SetPosition({ 0.0f, 100.0f, -600.0f });
	m_camera = NewGO<CAMERA>(0, "m_camera");

	//MovingFloor* move = NewGO<MovingFloor>(0);
	//move->SetPosition({ 0.0f,850.0f,-12000.0f });


	m_spriteRender.Init("Assets/sprite/TENEBRIS.dds", 2050, 1100);
	m_spriteRender.SetPosition(Vector3(0.0f, 30.0f, 0.0f));
	m_spriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.5f));


	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();
	return true;
}


//�X�V�����B
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
	if (g_pad[0]->IsTrigger(enButtonStart)		//Start�{�^���������ꂽ�B
		&& m_menu == false)					//����m_menu==false�̎��B
	{
		//menu = NewGO<Menu>(0);
		m_menu = true;
	}
	else if (g_pad[0]->IsTrigger(enButtonStart)		//Start�{�^���������ꂽ�B
		&& m_menu == true)					//����m_menu==true�̎��B 
	{
		//DeleteGO(menu);
		m_menu = false;
	}


	//Vector3 diff2 = boss->GetPosition() - m_player->GetPosition();

		
//	if (BGMvolume >= 0.001f)
//	{
//	}
	 if(BGMchange == 0){
		GameBGM->SetVolume(0.5f);
	}

	if (BGMchange == 1)
	{
		GameBGM->SetVolume(BGMvolume);
		BGMvolume -= 0.01f;
		if (BGMvolume <= 0.1f) {
			//BGMvolume = 0.0001f;
			DeleteGO(GameBGM);
			BGMchange = 2;
		}
	}

	if (BGMchange == 3)
	{
		BossBGM->Play(true);
		BossBGM->SetVolume(0.5f);
		//BGMchange = 4;
	}


	if (m_numDefeatedEnemy == m_numEnemy)
	{
		m_player->level += 1;
		m_numEnemy += 5;
	}

	if (m_player->level >= 3)
	{
		m_player->level = 3;
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
		siboutimer += g_gameTime->GetFrameDeltaTime();
			m_fontRender.SetText(L"Death");
			//�\��������W��ݒ肷��B
			m_fontRender.SetPosition(Vector3{ -300.0f,0.0f,0.0f });
			//�����̑傫����ς���B
			m_fontRender.SetScale(5.0f);
			if (siboutimer <= 1.5f) {
				alpha = alpha + 0.02f;
				if (alpha > 1.0f) {
					alpha = 1.0f;
				}
			}

			else if (siboutimer > 1.5f) {
				alpha = alpha - 0.02f;
				if (alpha < 0.0f) {
					alpha = 0.0f;
				}
			}
			//m_modelRender.Update();
		


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
	if (siboutimer <= 3.5f) {
		m_fontRender.SetColor(Vector4(1.0f, 0.0f, 0.0f, fabsf(sinf(alpha))));
		m_fontRender.Draw(rc);
	}
}