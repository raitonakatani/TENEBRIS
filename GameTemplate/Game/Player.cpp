#include "stdafx.h"
#include "Player.h"
#include"fastPlayer.h"
#include "Telop.h"

#include "Game.h"
#include "Menu.h"
#include "WarpCounter.h"


//CollisionObject���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "collision/CollisionObject.h"

//EffectEmitter���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "graphics/effect/EffectEmitter.h"

Player::~Player()
{
	DeleteGO(m_telop);
}
bool Player::Start()
{
	//�A�j���[�V���������[�h����B
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


	//���m�̃��f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/kensi.tkm", m_animationClips, enAnimationClip_Num);
	m_modelRender.SetScale(m_scale);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	//�L�����N�^�[�R���g���[���[���������B
	m_charaCon.Init(
		30.0f,			//���a�B
		120.0f,			//�����B
		m_position		//���W�B
	);

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);});


	m_HPberRender.Init("Assets/sprite/HP.dds", 512.0f, 32.0f);
	//�\��������W��ݒ肷��B
	m_HPberRender.SetPosition({ -694.0f,500.0f ,0.0f });
	m_HPRender.SetPivot({ 0.0f, 0.5f });

	m_HPRender.Init("Assets/sprite/HP.dds", 512.0f, 32.0f);
	//�\��������W��ݒ肷��B
	m_HPRender.SetPosition({ -950.0f,500.0f ,0.0f });
	m_HPRender.SetPivot({ 0.0f, 0.5f });

	m_stmnberRender.Init("Assets/sprite/HP.dds", 512.0f, 32.0f);
	//�\��������W��ݒ肷��B
	m_stmnberRender.SetPosition({ -694.0f,430.0f ,0.0f });
	m_HPRender.SetPivot({ 0.0f, 0.5f });

	m_staminaRender.Init("Assets/sprite/HP.dds", 512.0f, 32.0f);
	//�\��������W��ݒ肷��B
	m_staminaRender.SetPosition({ -950.0f,430.0f ,0.0f });
	m_staminaRender.SetPivot({ 0.0f, 0.5f });

	m_portionRender.Init("Assets/sprite/Portion.dds", 200, 200);
	m_portionRender.SetPosition({ 800.0f, -300.0f, 0.0f });

	m_statusRender.Init("Assets/sprite/status.dds", 553, 324);
	m_statusRender.SetPosition({ -680.0f, 380.0f, 0.0f });

	m_game = FindGO<Game>("game");

	m_telop = NewGO<Telop>(0, "telop");


	//�G�t�F�N�g��ǂݍ��ށB
	EffectEngine::GetInstance()->ResistEffect(2, u"Assets/effect/efk/Heal.efk");
	EffectEngine::GetInstance()->ResistEffect(3, u"Assets/effect/efk/Hit2.efk");

	//�T�E���h��ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/9Step.wav");

	//�uSword�v�{�[����ID(�ԍ�)���擾����B
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
		//�ړ������B
		Move();
		//��]�����B
		Rotation();
		//�U�������B
		Attack();
		//�񕜏����B
		Heal();
		//�����蔻��B
		Collision();
		//�A�j���[�V�����̍Đ��B
		PlayAnimation();
		//�X�e�[�g�̑J�ڏ����B
		ManageState();

	}
		//���f���̍X�V�B
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
	//�\��������W��ݒ肷��B
	m_fontRender2.SetPosition({ -950.0f,320.0f,0.0f });
	//�����̑傫����ς���B
	//fontRender.SetScale(1.5f);
	//�\������F��ݒ肷��B
	m_fontRender2.SetColor(g_vec4Black);

	wchar_t wcsbuf3[256];
	swprintf_s(wcsbuf3, 256, L"x%d", int(m_portion));
	m_fontRender3.SetText(wcsbuf3);
	//�\��������W��ݒ肷��B
	m_fontRender3.SetPosition({ 860.0f,-380.0f,0.0f });
	//�����̑傫����ς���B
	m_fontRender.SetScale(0.5f);
	//�\������F��ݒ肷��B
	m_fontRender3.SetColor(g_vec4White);

	m_HPberRender.SetScale({ hpber,1.0f,0.0f });
	m_HPberRender.Update();
	m_stmnberRender.SetScale({ stmnber,1.0f,0.0f });
	m_stmnberRender.Update();

	//HP�o�[
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



	if (g_pad[0]->IsTrigger(enButtonStart)		//Start�{�^���������ꂽ�B
		&& m_menu == false)					//����m_menu==false�̎��B
	{
		m_menu = true;
		menu = NewGO<Menu>(0);
	}
	else if (g_pad[0]->IsTrigger(enButtonStart)		//Start�{�^���������ꂽ�B
		&& m_menu == true)					//����m_menu==true�̎��B 
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
	//�ړ��ł��Ȃ���Ԃł���΁A�ړ������͂��Ȃ��B
	if (IsEnableMove() == false)
	{
		//�U��
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

	//���̃t���[���̈ړ��ʂ����߂�B
	//���X�e�B�b�N�̓��͗ʂ��󂯎��B
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	//�J�����̑O�������ƉE�������擾�B
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//XZ���ʂł̑O�������A�E�����ɕϊ�����B
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//XZ�����̈ړ����x���N���A�B


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
			m_moveSpeed += m_forward * 200.0f;	//�������ւ̈ړ����x�����Z�B
		}
		if (timer > 0.2f && timer <= 1.2f)
		{
			m_moveSpeed += m_forward * 500.0f;	//�������ւ̈ړ����x�����Z�B
		}
		else if (timer >= 1.2f)
		{
			m_roll = false;
		}
	}
	else {
		if (COOLtime == true)
		{
			m_moveSpeed += cameraForward * lStick_y * 250.0f;	//�������ւ̈ړ����x�����Z�B
			m_moveSpeed += cameraRight * lStick_x * 250.0f;		//�E�����ւ̈ړ����x�����Z�B
			m_sutamina++;
		}
		if (COOLtime == false)
		{
			if (g_pad[0]->IsPress(enButtonLB1) && m_sutamina > 0)
			{
				m_moveSpeed += cameraForward * lStick_y * 600.0f;	//�������ւ̈ړ����x�����Z�B
				m_moveSpeed += cameraRight * lStick_x * 600.0f;		//�E�����ւ̈ړ����x�����Z�B
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
				m_moveSpeed += cameraForward * lStick_y * 300.0f;	//�������ւ̈ړ����x�����Z�B
				m_moveSpeed += cameraRight * lStick_x * 300.0f;		//�E�����ւ̈ړ����x�����Z�B
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

	//�L�����N�^�[�R���g���[���[���g�p���āA���W���X�V�B
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//�n�ʂɂ����B
		m_moveSpeed.y = 0.0f;
	}

	m_moveSpeed.y -= 500.0f * g_gameTime->GetFrameDeltaTime();

	Vector3 modelPosition = m_position;
	//������Ƃ������f���̍��W��������B
	modelPosition.y += 2.5f;
	m_modelRender.SetPosition(modelPosition);
}

void Player::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		//m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
		//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ��B
		return;
	}
	//atan2��tan�Ƃ̒l���p�x(���W�A���P��)�ɕϊ����Ă����֐��B
	//m_moveSpeed.x / m_moveSpeed.z�̌��ʂ�tan�ƂɂȂ�B
	//atan2���g�p���āA�p�x�����߂Ă���B
	//���ꂪ��]�p�x�ɂȂ�B
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
	//SetRotationDeg�ł͂Ȃ�SetRotation���g�p����B
	m_rotation.SetRotationY(-angle);

	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);

	//�v���C���[�̐��ʃx�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}


void Player::Collision()
{


	//��_���[�W�̎��́B
	//�����蔻�菈���͂��Ȃ��B
	if (m_playerState == enPlayerState_ReceiveDamage||
		m_playerState == enPlayerState_Death)
	{
		return;
	}

	//����ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/3damage.wav");
	//���ʉ����Đ�����B
	SoundSource* damagese = NewGO<SoundSource>(0);
	damagese->Init(3);


	//�G�l�~�[�iknight�j�̍U���p�̃R���W�������擾����B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_charaCon))
		{
			//HP��1���炷�B
			m_hp -= 10;

			//�����Đ�����B
			damagese->Play(false);
			damagese->SetVolume(1.5f);

			      

			//HP��0�Ȃ�B
			if (m_hp <= 0)
			{
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_playerState = enPlayerState_Death;
			}
			//����HP��0����Ȃ�B
			else
			{
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_playerState = enPlayerState_ReceiveDamage;
			}
		
			return;
		}
	}

	//�G�l�~�[�̃t�@�C���[�{�[���p�̃R���W�������擾����B
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("enemy_Magic");
	//for���Ŕz����񂷁B
	for (auto collision : collisions2)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_charaCon))
		{
			//HP��10���炷�B
			m_hp -= 10;

			//�����Đ�����B
			damagese->Play(false);
			damagese->SetVolume(1.5f);

			//����HP��0����Ȃ�B
			if (m_hp > 0)
			{
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_playerState = enPlayerState_ReceiveDamage;
			}
			//HP��0�Ȃ�B
			else if (m_hp <= 0)
			{
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_playerState = enPlayerState_Death;
			}
			return;
		}
	}

	//�G�l�~�[�i�{�X�j�̍U���p�̃R���W�������擾����B
	const auto& collisions3 = g_collisionObjectManager->FindCollisionObjects("enemy3_attack");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions3)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_charaCon))
		{
			//HP��1���炷�B
			m_hp -= 10;

			//�����Đ�����B
			damagese->Play(false);
			damagese->SetVolume(1.5f);

			//����HP��0����Ȃ�B
			if (m_hp > 0)
			{
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_playerState = enPlayerState_ReceiveDamage;
			}
			//HP��0�Ȃ�B
			else if (m_hp <= 0)
			{
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_playerState = enPlayerState_Death;
			}

			return;
		}
	}

	//�G�l�~�[�i�J�M�����j�̍U���p�̃R���W�������擾����B
	const auto& collisions4 = g_collisionObjectManager->FindCollisionObjects("enemy4_attack");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions4)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_charaCon))
		{
			//HP��1���炷�B
			m_hp -= 10;

			//�����Đ�����B
			damagese->Play(false);
			damagese->SetVolume(1.5f);

			//����HP��0����Ȃ�B
			if (m_hp > 0)
			{
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_playerState = enPlayerState_ReceiveDamage;
			}
			//HP��0�Ȃ�B
			else if (m_hp <= 0)
			{
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_playerState = enPlayerState_Death;
			}

			return;
		}
	}
}

void Player::Attack()
{
	//�U�����łȂ��Ȃ�A���������Ȃ��B
	if (m_playerState != enPlayerState_Attack)
	{
		return;
	}

	//�U�����蒆�ł���΁B
	if (m_isUnderAttack == true)
	{
		//MakeAttackEffect();
		//�U���p�̃R���W�������쐬����B
		MakeAttackCollision();
	}

	//�U�����蒆�ł���΁B
	if (m_isEffect == true)
	{
		MakeAttackEffect();
	}
	
}

void Player::Heal()
{
	//�񕜒��łȂ��Ȃ�A���������Ȃ��B
	if (m_playerState != enPlayerState_Heal)
	{
		return;
	}

	//�񕜔��蒆�ł���΁B
	if (m_isUnderHeal == true)
	{
		//�񕜂���B
		kaihuku = true;
		//m_hp += 50;
		m_portion--;
		m_isUnderHeal = false;
	}
}

void Player::MakeAttackCollision()
{
	//�R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);

	Vector3 collisionPosition = m_position;
	//���W���v���C���[�̏����O�ɐݒ肷��B
	collisionPosition += m_forward * 50.0f;
	collisionPosition.y += 100.0f;
	//�{�b�N�X��̃R���W�������쐬����B
	collisionObject->CreateBox(collisionPosition,				   //���W�B
		Quaternion::Identity,                                      //��]�B
		Vector3(250.0f, 35.0f, 35.0f)                              //�傫���B
	);
	collisionObject->SetName("player_attack");

	//�uSword�v�{�[���̃��[���h�s����擾����B
	Matrix matrix = m_modelRender.GetBone(m_sword_jointBoneId)->GetWorldMatrix();
	//�uSword�v�{�[���̃��[���h�s����R���W�����ɓK�p����B
	collisionObject->SetWorldMatrix(matrix);
}

void Player::MakeAttackEffect()
{

	//�G�t�F�N�g�̃I�u�W�F�N�g���쐬����B
	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->Init(3);
	//�G�t�F�N�g�̑傫����ݒ肷��B
	m_effectEmitter->SetScale({ 5.0f,5.0f,20.0f });
	Vector3 effectposition = m_position;
	effectposition += m_forward * 200.0f;
	//effectposition.y += 100.0f;
	m_effectEmitter->SetPosition(effectposition);
	Quaternion rotation;
	//�v���C���[�̉�]�B
	rotation = m_rotation;
	//��]������B
	rotation.AddRotationDegY(360.0f);
	//rotation.AddRotationDegZ(180.0f);
	//��]��ݒ肷��B
	m_effectEmitter->SetRotation(rotation);
	m_effectEmitter->Play();

	//�uSword�v�{�[���̃��[���h�s����擾����B
	Matrix matrix = m_modelRender.GetBone(m_sword_jointBoneId)->GetWorldMatrix();
	//�uSword�v�{�[���̃��[���h�s����R���W�����ɓK�p����B
	m_effectEmitter->SetWorldMatrix(matrix);

}

void Player::ProcessCommonStateTransition()
{
	
	//Y�{�^���������ꂽ��B
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		//�U���X�e�[�g�Ɉڍs����B
		m_playerState = enPlayerState_Attack;
		//�t���O��false�ɂ���B
		m_isUnderAttack = false;
		m_isEffect = false;
		return;
	}
	
	if (g_pad[0]->IsTrigger(enButtonX) && m_portion >= 1)
	{


		//m_playerState = enPlayerState_Attack;
		m_playerState = enPlayerState_Heal;

		//�G�t�F�N�g�̃I�u�W�F�N�g���쐬����B
		m_effectEmitter = NewGO <EffectEmitter>(0);
		m_effectEmitter->Init(2);
		m_effectEmitter->SetPosition(m_position);
		//�G�t�F�N�g�̑傫����ݒ肷��B
		m_effectEmitter->SetScale(m_scale * 5.0f);
		m_effectEmitter->Play();

		//�t���O��true�ɂ���B
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
		//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
		if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
		{
			if (COOLtime == true) {
				m_playerState = enPlayerState_Walk;
			}
			if (COOLtime == false)
			{
				//�ړ����x�����ȏゾ������B
				if (g_pad[0]->IsPress(enButtonLB1))
				{
					//m_sutamina--;
					//�X�e�[�g�𑖂�ɂ���B
					m_playerState = enPlayerState_Run;
					return;
				}

				else
				{
					//�����ɂ���B
					m_playerState = enPlayerState_Walk;
					return;
				}
			}
		}
		//x��z�̈ړ����x������������(�X�e�B�b�N�̓��͂�����������)�B
		else
		{
			//�X�e�[�g��ҋ@�ɂ���B
			m_playerState = enPlayerState_Idle;
			return;
		}
	}
}

//�ҋ@�B
void Player::ProcessIdleStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}

//����B
void Player::ProcessRunStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}

//�����B
void Player::ProcessWalkStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}

//�񕜁B
void Player::ProcessHealStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false) {
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
	}
}

//�U���B
void Player::ProcessAttackStateTransition()
{
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
	}
}

//�_���[�W�B
void Player::ProcessDamageStateTransition()
{
	//m_timer += g_gameTime->GetFrameDeltaTime();

	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false
		/*&& m_timer >= 0.3f*/)
	{
		//�U�����ꂽ��A�ҋ@�A�j���[�V�����ɂ�����B
		m_playerState = enPlayerState_Idle;
		//m_timer = 0.0f;
	}
}

void Player::ProcessDeathStateTransition()
{
	//�X�e�[�g��J�ڂ���B
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
		//�ҋ@�X�e�[�g�̎��B
	case enPlayerState_Idle:
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessIdleStateTransition();
		break;
		//����X�e�[�g�̎��B
	case enPlayerState_Run:
		//����X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessRunStateTransition();
		break;
		//�����X�e�[�g�̎��B
	case enPlayerState_Walk:
		//����X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessWalkStateTransition();
		break;
	case enPlayerState_ReceiveDamage:
		//��_���[�W�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessDamageStateTransition();
		break;
		//�U���X�e�[�g�̎��B
	case enPlayerState_Attack:
		//�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessAttackStateTransition();
		break;
		//�񕜃X�e�[�g�̎��B
	case enPlayerState_Heal:
		//�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
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
		//�ҋ@�X�e�[�g�̎��B
	case enPlayerState_Idle:
		//�ҋ@�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.2f);
		break;
		//����X�e�[�g�̎��B
	case enPlayerState_Run:
		m_modelRender.SetAnimationSpeed(1.2f);
		//����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.2f);
		break;
	case enPlayerState_Walk:
		m_modelRender.SetAnimationSpeed(1.5f);
		//�����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
		//��_���[�W�X�e�[�g�̎��B
	case enPlayerState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(0.8f);
		//��_���[�W�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.2f);
		break;
		//�U���X�e�[�g�̎��B
	case enPlayerState_Attack:
		m_modelRender.SetAnimationSpeed(1.2f);
		//�U���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.2f);
		break;
	case enPlayerState_Heal:
		m_modelRender.SetAnimationSpeed(2.5f);
		//�񕜃A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Heal, 0.5f);
		break;
	case enPlayerState_Death:
		//m_modelRender.SetAnimationSpeed(0.8f);
		//���S�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Death, 0.5f);
		break;
	case enPlayerState_Roll:
		m_modelRender.SetAnimationSpeed(1.0f);
		//����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Roll, 0.2f);
		break;
	default:
		break;
	}
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

	//�L�[�̖��O���uattack_start�v�̎��B
	if (wcscmp(eventName, L"attack_start") == 0) {
		//�G�t�F�N�g�𔭐�������B
		//MakeAttackEffect();
		//�U�����ɂ���B
		m_isUnderAttack = true;
	}
	//�L�[�̖��O���uattack_end�v�̎��B
	else if (wcscmp(eventName, L"attack_end") == 0) {
		//�U�����I���B
		m_isUnderAttack = false;
	//	MakeAttackEffect();
	}

	//�L�[�̖��O���uHeal_start�v�̎��B
	if (wcscmp(eventName, L"Heal_start") == 0) {
		//�񕜒��ɂ���B
		m_isUnderHeal = true;
	}

	if (wcscmp(eventName, L"Walk_step") == 0) {
		//�����B
		 //���ʉ����Đ�����B
		SoundSource* stepse = NewGO<SoundSource>(0);
		stepse->Init(9);
		stepse->Play(false);
		stepse->SetVolume(1.5f);
	}

	//�L�[�̖��O���uEffect_start�v�̎��B
	if (wcscmp(eventName, L"Effect_start") == 0) {
		//�񕜒��ɂ���B
		m_isEffect = true;
	}
	//�L�[�̖��O���uEffect_start�v�̎��B
	if (wcscmp(eventName, L"Effect_end") == 0) {
		//�񕜒��ɂ���B
		m_isEffect = false;
	}
}

void Player::Render(RenderContext& rc)
{

	m_HPberRender.SetMulColor(Vector4(1.0f, 0.0f, 0.0f, 0.9f));
	m_HPberRender.Draw(rc);
	m_stmnberRender.SetMulColor(Vector4(1.0f, 0.0f, 0.0f, 0.9f));
	m_stmnberRender.Draw(rc);

	//���f����`�悷��B
	m_modelRender.Draw(rc);
	//�摜��`�ʂ���B
	m_HPRender.Draw(rc);
	//m_staminaRender.SetMulColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	m_staminaRender.Draw(rc);

	if (m_menu == false)
	{
	//	m_statusRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.9f));
	//	m_statusRender.Draw(rc);
		m_portionRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.9f));
		m_portionRender.Draw(rc);
		//������`�ʂ���B
		m_fontRender2.Draw(rc);
		m_fontRender3.Draw(rc);
	}
}
