#include "stdafx.h"
#include "Enemy2.h"

#include "Game.h"
#include "Magic.h"
#include "Player.h"
#include "Portion.h"

#include <time.h>
#include <stdlib.h>

//CollisionObject���g�p���������߁A�t�@�C�����C���N���[�h����B
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
	//�A�j���[�V������ǂݍ��ށB
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
	//���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/Enemy/wizard.tkm", m_animationClips, enAnimationClip_Num);

	//���W��ݒ肷��B
	m_modelRender.SetPosition(m_position);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);
	//�傫����ݒ肷��B
	m_modelRender.SetScale(m_scale);

	//�L�����N�^�[�R���g���[���[���������B
	m_charaCon.Init(
		40.0f,			//���a�B
		150.0f,			//�����B
		m_position		//���W�B
	);


	//�G�t�F�N�g��ǂݍ��ށB
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/enemy.efk");


	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	m_player = FindGO<Player>("player");

	m_spriteRender.Init("Assets/sprite/HP2.dds", 64.0f, 32.0f);
	//�\��������W��ݒ肷��B
	m_spriteRender.SetPosition({ 0.0f,1450.0f ,0.0f });


	//�������������B
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	return true;
}

void Enemy2::Update()
{
	//�ǐՏ����B
	Chase();
	//��]�����B
	Rotation();
	//�����蔻��B
	Collision();
	//�A�j���[�V�����̍Đ��B
	PlayAnimation();
	//�X�e�[�g�̑J�ڏ����B
	ManageState();


	modeltimer += g_gameTime->GetFrameDeltaTime();

	if (m_model == 1)
	{
		if (model == false) {
			//�G�t�F�N�g�̃I�u�W�F�N�g���쐬����B
			m_effectEmitter = NewGO <EffectEmitter>(0);
			m_effectEmitter->Init(1);
			m_effectEmitter->SetPosition(m_position);
			//�G�t�F�N�g�̑傫����ݒ肷��B
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

	//HP�o�[
	a = m_hp / 2;
	m_spriteRender.SetScale({ a, 0.5f, 0.0f });

	Vector3 diff = m_player->GetPosition() - m_position;

	//�x�N�g���̒�����1000.0f��菬����������B
	if (diff.Length() <= 1000.0f)
	{
		//���[���h���W�ɕϊ��B
		//���W���G�l�~�[�̏�����ɐݒ肷��B
		Vector4 worldPos = Vector4(m_position.x, m_position.y + 250.0f, m_position.z, 1.0f);

		Matrix matrix;
		matrix.Multiply(g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix());

		matrix.Apply(worldPos);

		//�J�����̃r���[�s����|����B
		//�J�������W�ɕϊ��B
		worldPos.x = (worldPos.x / worldPos.w);
		worldPos.y = (worldPos.y / worldPos.w);

		//�J�����̃v���W�F�N�V�����s����|����B
		//�X�N���[�����W�ɕϊ��B
		worldPos.x *= FRAME_BUFFER_W / 2;
		worldPos.y *= FRAME_BUFFER_H / 2;

		//�|�W�V�����̐ݒ�B
		m_spriteRender.SetPosition(Vector3(worldPos.x, worldPos.y, 0.0f));
		//HP�o�[��\������B
		m_isShowHPBar = true;
	}
	else
	{
		//HP�o�[��\�����Ȃ��B
		m_isShowHPBar = false;
	}
	m_spriteRender.Update();

	//�ΉB
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

	//���f���̍X�V�B
	m_modelRender.Update();
}

void Enemy2::Rotation()
{
	if (sibou == false)
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

		Vector3 diff = m_player->GetPosition() - m_position;

		float angle = atan2(-diff.x, diff.z);
		//atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
		//SetRotationDeg�ł͂Ȃ�SetRotation���g�p����B
		m_rotation.SetRotationY(-angle);


		//��]��ݒ肷��B
		m_modelRender.SetRotation(m_rotation);

		//�v���C���[�̑O�x�N�g�����v�Z����B
		m_forward = Vector3::AxisZ;
		m_rotation.Apply(m_forward);
	}
}

void Enemy2::Chase()
{
	//�ǐՃX�e�[�g�łȂ��Ȃ�A�ǐՏ����͂��Ȃ��B
	if (m_enemy2State != enEnemy2State_Chase)
	{
		return;
	}
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//�n�ʂɂ����B
		m_moveSpeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	//������Ƃ������f���̍��W��������B
	modelPosition.y += 2.5f;
	m_modelRender.SetPosition(modelPosition);
}

void Enemy2::Collision()
{
	//��_���[�W�A���邢�̓_�E���X�e�[�g�̎��́B
	//�����蔻�菈���͂��Ȃ��B
	if (m_enemy2State == enEnemy2State_ReceiveDamage ||
		m_enemy2State == enEnemy2State_Down)
	{
		return;
	}

	//�v���C���[�̍U���p�̃R���W�������擾����B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_charaCon))
		{
			//HP��1���炷�B
			m_hp -= karyoku;

			//����ǂݍ��ށB
			g_soundEngine->ResistWaveFileBank(3, "Assets/sound/3damage.wav");
			//���ʉ����Đ�����B
			SoundSource* damagese = NewGO<SoundSource>(0);
			damagese->Init(3);
			damagese->Play(false);
			damagese->SetVolume(1.5f);

			//����HP��0����Ȃ�B
			if (m_hp > 0)
			{
				sibou = false;
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_enemy2State = enEnemy2State_ReceiveDamage;
			}
			//HP��0�Ȃ�B
			else if (m_hp <= 0)
			{
				sibou = true;
				//�_�E���X�e�[�g�ɑJ�ڂ���B
				m_enemy2State = enEnemy2State_Down;
			}

			return;
		}
	}

}



const bool Enemy2::SearchPlayer() const
{
	Vector3 diff = m_player->GetPosition() - m_position;

	//�v���C���[�ɂ�����x�߂�������.�B
	if (diff.LengthSq() <= 1000.0f * 1000.0f)
	{
		//�G�l�~�[����v���C���[�Ɍ������x�N�g���𐳋K������B
		diff.Normalize();
		//�G�l�~�[�̐��ʂ̃x�N�g���ƁA�G�l�~�[����v���C���[�Ɍ������x�N�g���́B
		//����(cos��)�����߂�B
		float cos = m_forward.Dot(diff);
		//����(cos��)����p�x(��)�����߂�B
		float angle = acosf(cos);
		//�p�x(��)��120����菬������΁B
		if (angle <= (Math::PI / 180.0f) * 135.0f)
		{
			//�v���C���[���������I
			return true;
		}
	}
	//�v���C���[���������Ȃ������B
	return false;
}



void Enemy2::MakeFireBall()
{
	//�t�@�C���[�{�[�����쐬����B
	Magic* fireBall = NewGO<Magic>(0);
	Vector3 fireBallPosition = m_position;
	//���W��������ɂ���B
	fireBallPosition.y += 70.0f;
	//���W��ݒ肷��B
	fireBall->SetPosition(fireBallPosition);
	fireBall->SetRotation(m_rotation);
	//�p�҂̓v���C���[�ɂ���B
	fireBall->SetEnMagician(Magic::enMagician_Enemy2);

}

void Enemy2::ProcessCommonStateTransition()
{
	//�e�^�C�}�[���������B
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	//�G�l�~�[����v���C���[�Ɍ������x�N�g�����v�Z����B
	Vector3 diff = m_player->GetPosition() - m_position;
//	Vector3 diff = m_position - m_player->GetPosition();
	

//	m_rotation.SetRotationY(atan2(diff.x , diff.z));

	//��]��ݒ肷��B
//	m_modelRender.SetRotation(m_rotation);

	//�v���C���[����������B
	if (SearchPlayer() == true)
	{
		//�x�N�g���𐳋K������B
		diff.Normalize();
		//�ړ����x��ݒ肷��B
		m_moveSpeed -= diff * 100.0f;


		//�U���ł��鋗���Ȃ�B
		if (IsCanAttack() == true)
		{
			//�����ɂ���āA�U�����邩�ҋ@�����邩�����肷��B	
			int ram = rand() % 100;
			if (ram > 50)
			{
				//�U���X�e�[�g�ɑJ�ڂ���B
				m_enemy2State = enEnemy2State_MagicAttack;
				m_isUnderAttack = false;
				return;
			}
			else
			{
				//�ǐՃX�e�[�g�ɑJ�ڂ���B
				m_enemy2State = enEnemy2State_Chase;
				return;
			}

		}
		//�U���ł��Ȃ������Ȃ�B
		else
		{
			//�����ɂ���āA�ǐՂ����邩���@�U�������邩���肷��B	
			int ram = rand() % 100;
			if (ram > 30)
			{
				//�ǐՃX�e�[�g�ɑJ�ڂ���B
				m_enemy2State = enEnemy2State_Chase;
				return;
			}
			else {
				//���݂����@�U���X�e�[�g�Ȃ�B
				if (m_enemy2State == enEnemy2State_MagicAttack)
				{
					//�A���Ŗ��@���������Ȃ��悤�ɁB
					//�ǐՃX�e�[�g�ɑJ�ڂ���B
					m_enemy2State = enEnemy2State_Chase;
					return;
				}
				//���݂����@�U���X�e�[�g�łȂ��Ȃ�B
				else
				{
					//���@�U���X�e�[�g�ɑJ�ڂ���B
					m_enemy2State = enEnemy2State_MagicAttack;
					return;
				}
			}
		}
	}
	//�v���C���[���������Ȃ���΁B
	else
	{
		//�ҋ@�X�e�[�g�ɑJ�ڂ���B
		m_enemy2State = enEnemy2State_Idle;
		return;

	}
}

void Enemy2::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//�ҋ@���Ԃ�������x�o�߂�����B
	if (m_idleTimer >= 0.6f)
	{
		//���̃X�e�[�g�֑J�ڂ���B
		ProcessCommonStateTransition();
	}

}

void Enemy2::ProcessWalkStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���B
	ProcessCommonStateTransition();
}

void Enemy2::ProcessChaseStateTransition()
{
	//�U���ł��鋗���Ȃ�B
	if (IsCanAttack() == true)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
		return;
	}
	m_chaseTimer += g_gameTime->GetFrameDeltaTime();
	//�ǐՎ��Ԃ�������x�o�߂�����B
	if (m_chaseTimer >= 0.5f)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}



void Enemy2::ProcessMagicAttackStateTransition()
{

	//���@�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Enemy2::ProcessReceiveDamageStateTransition()
{
	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U�����ꂽ�狗���֌W�����ɁA��芸�����ǐՂ�����B
		m_enemy2State = enEnemy2State_Chase;
		Vector3 diff = m_player->GetPosition() - m_position;
		diff.Normalize();
		//�ړ����x��ݒ肷��B
		m_moveSpeed -= diff * 100.0f;
	}
}

void Enemy2::ProcessDownStateTransition()
{
	//�_�E���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		Game* game = FindGO<Game>("game");
		int radm = rand() % 100;
		if (radm >= 70) {
			Portion* portion = NewGO<Portion>(0,"portion");
			portion->SetPosition(m_position);
		}
		//����ǂݍ��ށB
		g_soundEngine->ResistWaveFileBank(1, "Assets/sound/1sibouzi.wav");
		//���ʉ����Đ�����B
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(1);
		se->Play(false);
		se->SetVolume(1.5f);
		//�|���ꂽ�G�l�~�[�̐���+1����B
		game->AddDefeatedEnemyNumber();
		game->SetnumEnemydesu();
		
/*		int ram = rand() % 100;

		if (ram > 20)
		{
			Portion* portion = NewGO<Portion>(0, "portion");
		}
*/

		//���g���폜����B
		DeleteGO(this);
	}
}



void Enemy2::ManageState()
{
	switch (m_enemy2State)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enEnemy2State_Idle:
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessIdleStateTransition();
		break;
		//�ǐՃX�e�[�g�̎��B
	case enEnemy2State_Chase:
		//�ǐՃX�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessChaseStateTransition();
		break;
		//���@�U���X�e�[�g�̎��B
	case enEnemy2State_MagicAttack:
		//���@�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessMagicAttackStateTransition();
		break;
		//��_���[�W�X�e�[�g�̎��B
	case enEnemy2State_ReceiveDamage:
		//��_���[�W�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessReceiveDamageStateTransition();
		break;
		//�_�E���X�e�[�g�̎��B
	case enEnemy2State_Down:
		//�_�E���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessDownStateTransition();
		break;
	}
}

void Enemy2::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_enemy2State)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enEnemy2State_Idle:
		//�ҋ@�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//�ǐՃX�e�[�g�̎��B
	case enEnemy2State_Chase:
		m_modelRender.SetAnimationSpeed(1.2f);
		//����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
		//���@�U���X�e�[�g�̎��B
	case enEnemy2State_MagicAttack:
		m_modelRender.SetAnimationSpeed(1.5f);
		//���@�U���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_MagicAttack, 0.1f);
		break;
		//��_���[�W�X�e�[�g�̎��B
	case enEnemy2State_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(1.5f);
		//��_���[�W�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
		//�_�E���X�e�[�g�̎��B
	case enEnemy2State_Down:
		//�_�E���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	default:
		break;
	}
}

void Enemy2::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//�L�[�̖��O���umagic_attack�v�̎��B
	if (wcscmp(eventName, L"magic_attack") == 0) {
		//�t�@�C���[�{�[���̃I�u�W�F�N�g���쐬����B
		MakeFireBall();
	}
}

const bool Enemy2::IsCanAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//�G�l�~�[�ƃv���C���[�̋������߂�������B
	if (diff.LengthSq() <= 600.0f * 600.0f)
	{
		//�U���ł���I
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
		//���f����`�悷��B
		m_modelRender.Draw(rc);
	}
	else {
		return;
	}

	//�摜��`�悷��B
	if (m_isShowHPBar == true) {
		m_spriteRender.Draw(rc);
	}
	else {
		return;
	}
}