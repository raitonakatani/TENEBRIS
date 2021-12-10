#include "stdafx.h"
#include "fastPlayer.h"

#include "Game.h"
#include "Menu.h"
#include"WarpCounter.h"


//CollisionObject���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "collision/CollisionObject.h"

//EffectEmitter���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "graphics/effect/EffectEmitter.h"

fastPlayer::fastPlayer()
{

}

fastPlayer::~fastPlayer()
{

}

bool fastPlayer::Start()
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

	// �ԍ����w�肵�āAefk�t�@�C����ǂݍ��ށB
	//EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/efk/syoukann.efk");

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

	m_game = FindGO<Game>("game");

	//�G�t�F�N�g��ǂݍ��ށB
	EffectEngine::GetInstance()->ResistEffect(2, u"Assets/effect/efk/Heal.efk");

	//�uSword�v�{�[����ID(�ԍ�)���擾����B
	m_sword_jointBoneId = m_modelRender.FindBoneID(L"sword_joint");

	return true;
}

void fastPlayer::Update()
{
	warpCounter = FindGO<WarpCounter>("WarpCounter");

	if (warpCounter->warpCounter == 0) {

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
	}

	if (warpCounter->warpCounter != 0) {
		DeleteGO(this);
	}
	//���f���̍X�V�B
	m_modelRender.Update();




}

void fastPlayer::Move()
{

	//�ړ��ł��Ȃ���Ԃł���΁A�ړ������͂��Ȃ��B
	if (IsEnableMove() == false)
	{
		//�U��
		return;
	}

	if (IsEnableMove2() == false)
	{
		//�_���[�W
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

/*	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_moveSpeed.y += 1000.0f;
	}
*/
	if (COOLtime == false)
	{
		if (g_pad[0]->IsPress(enButtonLB1) && m_sutamina > 0)
		{
			m_moveSpeed += cameraForward * lStick_y * 600.0f;	//�������ւ̈ړ����x�����Z�B
			m_moveSpeed += cameraRight * lStick_x * 600.0f;		//�E�����ւ̈ړ����x�����Z�B
		}
		else
		{
			m_moveSpeed += cameraForward * lStick_y * 400.0f;	//�������ւ̈ړ����x�����Z�B
			m_moveSpeed += cameraRight * lStick_x * 400.0f;		//�E�����ւ̈ړ����x�����Z�B
		}
	}

	
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

void fastPlayer::Rotation()
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


void fastPlayer::Collision()
{

	//����ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/3damage.wav");
	//���ʉ����Đ�����B
	SoundSource* damagese = NewGO<SoundSource>(0);
	damagese->Init(3);

}

void fastPlayer::Attack()
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


void fastPlayer::MakeAttackCollision()
{
	//�R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);

	Vector3 collisionPosition = m_position;
	//���W���v���C���[�̏����O�ɐݒ肷��B
	collisionPosition += m_forward * 50.0f;
	collisionPosition.y += 100.0f;
	//�{�b�N�X��̃R���W�������쐬����B
	collisionObject->CreateBox(collisionPosition,				   //���W�B
		Quaternion::Identity,                                       //��]�B
		Vector3(220.0f, 30.0f, 30.0f)                               //�傫���B
	);
	collisionObject->SetName("fastPlayer_attack");

	//�uSword�v�{�[���̃��[���h�s����擾����B
	Matrix matrix = m_modelRender.GetBone(m_sword_jointBoneId)->GetWorldMatrix();
	//�uSword�v�{�[���̃��[���h�s����R���W�����ɓK�p����B
	collisionObject->SetWorldMatrix(matrix);
}

void fastPlayer::ProcessCommonStateTransition()
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


	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//�ړ����x�����ȏゾ������B
		if (g_pad[0]->IsPress(enButtonLB2))
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
	//x��z�̈ړ����x������������(�X�e�B�b�N�̓��͂�����������)�B
	else
	{
		//�X�e�[�g��ҋ@�ɂ���B
		m_playerState = enPlayerState_Idle;
		return;
	}
}

//�ҋ@�B
void fastPlayer::ProcessIdleStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}

//����B
void fastPlayer::ProcessRunStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}

//�����B
void fastPlayer::ProcessWalkStateTransition()
{
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
}

//�U���B
void fastPlayer::ProcessAttackStateTransition()
{
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
	}
}

//�_���[�W�B
void fastPlayer::ProcessDamageStateTransition()
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

void fastPlayer::ManageState()
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
	}
}

void fastPlayer::PlayAnimation()
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
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.5f);
		break;
		//�U���X�e�[�g�̎��B
	case enPlayerState_Attack:
		m_modelRender.SetAnimationSpeed(1.0f);
		//�U���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.5f);
		break;
	default:
		break;
	}
}

void fastPlayer::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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

void fastPlayer::Render(RenderContext& rc)
{
	//���f����`�悷��B
	m_modelRender.Draw(rc);
}
