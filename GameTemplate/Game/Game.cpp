#include "stdafx.h"
#include "Game.h"

#include "Player.h"
#include "Enemy.h"
#include "Enemy2.h"
#include "Enemy3.h"
#include "Enemy4.h"
#include "Door.h"
#include "KAGI.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "TIMER.h"
#include "Fade.h"
#include "GameClear.h"
#include "Title.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"



//EffectEmitter���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "graphics/effect/EffectEmitter.h"

Game::Game()
{

}

Game::~Game()
{
	DeleteGO(m_player);
	DeleteGO(m_skycube);
	DeleteGO(m_gameCamera);
	DeleteGO(m_background);
	DeleteGO(m_TIMER);
	DeleteGO(GameBGM);

	for (auto door : m_doorVector)
	{
		DeleteGO(door);
	}
	const auto& enemys = FindGOs<Enemy>("enemy");
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
/*	const auto& enemys4 = FindGOs<Enemy4>("enemy4");
	for (auto enemy4 : enemys4)
	{
		DeleteGO(enemy4);
	}
*/
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

	g_camera3D->SetPosition({ 0.0f, 100.0f, -600.0f });

	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/6GameBGM.wav");
	//���ʉ����Đ�����B
	GameBGM = NewGO<SoundSource>(0);
	GameBGM->Init(6);
	GameBGM->Play(true);
	GameBGM->SetVolume(0.5f);

	
	//���x�����\�z����B
	m_levelRender.Init("Assets/level/stage.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"player") == true) {
			//�v���C���[�̃C���X�^���X�𐶐�����B
			m_player = NewGO<Player>(0, "player");
			m_player->SetPosition(objData.position);
			m_player->SetRotation(objData.rotation);
			m_player->SetScale(objData.scale);
			//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
			return true;
		}
		else if (objData.EqualObjectName(L"enemy") == true) {
			//�G�l�~�[�i�R�m�j�̃C���X�^���X�𐶐�����B
			Enemy* kisi = NewGO<Enemy>(0, "enemy");
			kisi->SetPosition(objData.position);
			kisi->SetRotation(objData.rotation);
			kisi->SetScale(objData.scale);
			//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
			return true;
		}
		else if (objData.EqualObjectName(L"wizard") == true) {
			//�G�l�~�[�i���@�g���B�j�̃C���X�^���X�𐶐�����B
			Enemy2* wizard = NewGO<Enemy2>(0,"enemy2");
			wizard->SetPosition(objData.position);
			wizard->SetRotation(objData.rotation);
			wizard->SetScale(objData.scale);
			//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
			return true;
		}
		else if (objData.EqualObjectName(L"boss") == true) {
			//�{�X�i���@���m�j�̃C���X�^���X�𐶐�����B
			Enemy3* boss = NewGO<Enemy3>(0, "enemy3");
			boss->SetPosition(objData.position);
			boss->SetRotation(objData.rotation);
			boss->SetScale(objData.scale);
			//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
			return true;
		}
		//���O��door��������B
		else if (objData.EqualObjectName(L"door") == true)
		{
			//�h�A�̃I�u�W�F�N�g���쐬����B
			m_door = NewGO<Door>(0, "door");
			//���W��ݒ肷��B
			m_door->SetPosition(objData.position);
			//�傫����ݒ肷��B
			m_door->SetScale(objData.scale);
			//��]��ݒ肷��B
			m_door->SetRotation(objData.rotation);
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
		return true;
	});

/*	Enemy4* enemy4 = NewGO<Enemy4>(0,"enemy4");
	enemy4->SetPosition({ 0.0f,800.0f,4000.0f });
*/
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	m_skycube = NewGO<SkyCube>(0, "skycube");

	m_TIMER = NewGO<TIMER>(0, "m_timer");


	

	/*
		float umblleraHP = kasa->InitialHP - kasa->HP;//�P���g�p��������
		gaugescale.x = 1.0f - (1.0f / kasa->InitialHP) * umblleraHP;
		gauge.SetScale(gaugescale);
	*/

	m_spriteRender.Init("Assets/sprite/TENEBRIS.dds", 2050, 1100);
	m_spriteRender.SetPosition(Vector3(0.0f, 30.0f, 0.0f));
	m_spriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.5f));

	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	return true;
}


//�X�V�����B
void Game::Update()
{/*
	a = m_TIMER->m_timer / 10.0f;
	if (a >= 1.0f)
	{
		a = 1.0f;
	}

	spriteRender.SetScale({ a, 1.0f, 0.0f });
*/

	if (m_isWaitFadeout && m_player->GetBOSS() == 1) {
		if (!m_fade->IsFade()) {
			NewGO<GameClear>(0, "gameclear");
			DeleteGO(this);
		}
	}
	else {
		if (m_player->GetBOSS() == 1)
		{
			m_isWaitFadeout = true;
			m_fade->StartFadeOut();
		}
	}



	//if (g_pad[0]->IsPress(enButtonStart))
	if (g_pad[0]->IsTrigger(enButtonX)		//X�{�^���������ꂽ�B
		&& m_menu == false)					//����m_menu==false�̎��B
	{
		m_menu = true;
	}
	else if(g_pad[0]->IsTrigger(enButtonX)		//X�{�^���������ꂽ�B
		&& m_menu == true)					//����m_menu==true�̎��B 
	{
		m_menu = false;
	}
	if (m_numDefeatedEnemy == m_numEnemy)
	{
		m_player->level += 1;
		m_numEnemy += 3;
		if (m_TIMER->m_timer >= t)
		{
			/*m_levelRender.Init("Assets/level/stage.tkl", [&](LevelObjectData& objData) {
				if (objData.EqualObjectName(L"enemy") == true) {
					//Unity�����B
					//�v���C���[�̃C���X�^���X�𐶐�����B
					Enemy* kisi = NewGO<Enemy>(0, "enemy");
					kisi->SetPosition(objData.position);
					kisi->SetRotation(objData.rotation);
					kisi->SetScale(objData.scale);
					//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
					return true;
				}
			});
			*/
			//m_player->level += 1;
			//m_numEnemy += 3;
			//t += 30.0f;
		}
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
		//�����̑傫����ς���B
		m_fontRender.SetScale(5.0f);
		//�\��������W��ݒ肷��B
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

}

void Game::Render(RenderContext& rc)
{

	m_fontRender.SetColor(Vector4(1.0f, 0.0f, 0.0f, fabsf(sinf(alpha))));
	m_fontRender.Draw(rc);

	if (m_menu == true) {
		m_spriteRender.Draw(rc);
	}
	else {
		return;
	}
}