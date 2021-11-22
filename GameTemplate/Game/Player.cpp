#include "stdafx.h"
#include "Player.h"

#include "Game.h"


//CollisionObject���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "collision/CollisionObject.h"

//EffectEmitter���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "graphics/effect/EffectEmitter.h"

Player::Player()
{

}

Player::~Player()
{

}

bool Player::Start()
{
	//�A�j���[�V���������[�h����B
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

	// �ԍ����w�肵�āAefk�t�@�C����ǂݍ��ށB
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/efk/syoukann.efk");
	
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
		OnAnimationEvent(clipName, eventName);
		});

	m_spriteRender.Init("Assets/sprite/HP2.dds", 512.0f, 32.0f);
	//�\��������W��ݒ肷��B
	m_spriteRender.SetPosition({ -950.0f,450.0f ,0.0f });
	m_spriteRender.SetPivot({ 0.0f, 0.5f });

	m_game = FindGO<Game>("game");



	//�uSword�v�{�[����ID(�ԍ�)���擾����B
	m_sword_jointBoneId = m_modelRender.FindBoneID(L"sword_joint");

	return true;
}

void Player::Update()
{
	//�ړ������B
	Move();
	//��]�����B
	Rotation();
	//�U�������B
	Attack();
	//�����蔻��B
	Collision();
	//�A�j���[�V�����̍Đ��B
	PlayAnimation();
	//�X�e�[�g�̑J�ڏ����B
	ManageState();

	//���f���̍X�V�B
	m_modelRender.Update();

	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"HP:%d", int(m_hp));
	m_fontRender.SetText(wcsbuf);
	//�\��������W��ݒ肷��B
	m_fontRender.SetPosition({ -950.0f,520.0f,0.0f });
	//�����̑傫����ς���B
	//fontRender.SetScale(1.5f);
	//�\������F��ݒ肷��B
	m_fontRender.SetColor(g_vec4White);


	wchar_t wcsbuf2[256];
	swprintf_s(wcsbuf2, 256, L"Level:%d", int(level));
	m_fontRender2.SetText(wcsbuf2);
	//�\��������W��ݒ肷��B
	m_fontRender2.SetPosition({ -950.0f,420.0f,0.0f });
	//�����̑傫����ς���B
	//fontRender.SetScale(1.5f);
	//�\������F��ݒ肷��B
	m_fontRender2.SetColor(g_vec4White);

	wchar_t wcsbuf3[256];
	swprintf_s(wcsbuf3, 256, L"�J�M:%d", int(m_kagi));
	m_fontRender3.SetText(wcsbuf3);
	//�\��������W��ݒ肷��B
	m_fontRender3.SetPosition({ -950.0f,320.0f,0.0f });
	//�����̑傫����ς���B
	//fontRender.SetScale(1.5f);
	//�\������F��ݒ肷��B
	m_fontRender3.SetColor(g_vec4White);


	//HP�o�[
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
	
	//�ړ��ł��Ȃ���Ԃł���΁A�ړ������͂��Ȃ��B
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
	m_moveSpeed += cameraForward * lStick_y * 600.0f;	//�������ւ̈ړ����x�����Z�B
	m_moveSpeed += cameraRight * lStick_x * 600.0f;		//�E�����ւ̈ړ����x�����Z�B

	
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
			m_moveSpeed += cameraForward * lStick_y *  5000.0f;	//�������ւ̈ړ����x�����Z�B
			m_moveSpeed += cameraRight * lStick_x * 5000.0f;		//�E�����ւ̈ړ����x�����Z�B
		}
		else if (timer >= 0.1f && timer < 0.3f)
		{
			m_moveSpeed += cameraForward * lStick_y * 500.0f;	//�������ւ̈ړ����x�����Z�B
			m_moveSpeed += cameraRight * lStick_x * 500.0f;		//�E�����ւ̈ړ����x�����Z�B	
		}
		else if (timer >= 2.0f)
		{
			timer = 0.0f;
			dash = false;
		}
	}

/*	if (g_pad[0]->IsTrigger(enButtonA) //A�{�^���������ꂽ��
		&& m_charaCon.IsOnGround()  //���A�n�ʂɋ�����
		) {
		m_moveSpeed.y = 350.0f;	//������ɑ��x��ݒ肵�āA
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
	if (m_playerState == enPlayerState_ReceiveDamage)
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
				m_playerState = enPlayerState_ReceiveDamage;
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
				m_playerState = enPlayerState_ReceiveDamage;
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
				m_playerState = enPlayerState_ReceiveDamage;
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
		//�U���p�̃R���W�������쐬����B
		MakeAttackCollision();
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
	collisionObject->CreateBox(collisionPosition,               //���W�B
		Quaternion::Identity,                                       //��]�B
		Vector3(200.0f, 20.0f, 20.0f)                               //�傫���B
	);
	collisionObject->SetName("player_attack");

	//�uSword�v�{�[���̃��[���h�s����擾����B
	Matrix matrix = m_modelRender.GetBone(m_sword_jointBoneId)->GetWorldMatrix();
	//�uSword�v�{�[���̃��[���h�s����R���W�����ɓK�p����B
	collisionObject->SetWorldMatrix(matrix);
}

void Player::MakePushLeverCollision()
{
	//�R���W�����I�u�W�F�N�g���쐬����B
	CollisionObject* collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	//���W���v���C���[�̏����O�ɐݒ肷��B
	collisionPosition += m_forward * 50.0f;
	//����̃R���W�������쐬����B
	collisionObject->CreateSphere(collisionPosition,       //���W�B
		Quaternion::Identity,                                  //��]�B
		70.0f);                                                //���̑傫��(���a)�B
		//���O��t����B
	collisionObject->SetName("player_lever");
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
		return;
	}
	
/*
	
	if (g_pad[0]->IsTrigger(enButtonX))
	{
		MakePushLeverCollision();
	}
*/
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//�ړ����x�����ȏゾ������B
		if (g_pad[0]->IsPress(enButtonLB2))
		{
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
	//x��z�̈ړ����x������������(�X�e�B�b�N�̓��͂�����������)�B
	else
	{
		//�X�e�[�g��ҋ@�ɂ���B
		m_playerState = enPlayerState_Idle;
		return;
	}
}

void Player::ProcessIdleStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}

void Player::ProcessRunStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}

void Player::ProcessWalkStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}

void Player::ProcessAttackStateTransition()
{
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
	}
}


void Player::ProcessReceiveDamageStateTransition()
{
	m_timer += g_gameTime->GetFrameDeltaTime();

	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false
		&& m_timer >= 0.3f)
	{
		//�U�����ꂽ��A�ҋ@�A�j���[�V�����ɂ�����B
		m_playerState = enPlayerState_Idle;
		m_timer = 0.0f;
	}
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
	case enPlayerState_Walk:
		//����X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessWalkStateTransition();
		break;
	case enPlayerState_ReceiveDamage:
		//��_���[�W�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessReceiveDamageStateTransition();
		break;
		//�U���X�e�[�g�̎��B
	case enPlayerState_Attack:
		//�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessAttackStateTransition();
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
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//����X�e�[�g�̎��B
	case enPlayerState_Run:
		m_modelRender.SetAnimationSpeed(1.2f);
		//����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.5f);
		break;
	case enPlayerState_Walk:
		m_modelRender.SetAnimationSpeed(1.0f);
		//����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.5f);
		break;
		//��_���[�W�X�e�[�g�̎��B
	case enPlayerState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(0.8f);
		//��_���[�W�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
		//�U���X�e�[�g�̎��B
	case enPlayerState_Attack:
		m_modelRender.SetAnimationSpeed(1.0f);
		//�U���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
	default:
		break;
	}
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

	//�L�[�̖��O���uattack_start�v�̎��B
	if (wcscmp(eventName, L"attack_start") == 0) {
		//�U�����ɂ���B
		m_isUnderAttack = true;
	}
	//�L�[�̖��O���uattack_end�v�̎��B
	else if (wcscmp(eventName, L"attack_end") == 0) {
		//�U�����I���B
		m_isUnderAttack = false;
	}

}

void Player::Render(RenderContext& rc)
{
	//���f����`�悷��B
	m_modelRender.Draw(rc);
	//�摜��`�ʂ���B
	m_spriteRender.Draw(rc);
	//������`�ʂ���B
	m_fontRender.Draw(rc);
	m_fontRender2.Draw(rc);
	m_fontRender3.Draw(rc);
}
