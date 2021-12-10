/*

#pragma once

namespace nsK2Engine {

    /// <summary>
   /// �|�C���g���C�g�\����
   /// </summary>
   /// <remark>
   /// ���̍\���̂�POD�^�Ƃ��Ĉ����Ă��܂��B
   /// �{�\���̂ɉ��z�֐��Ȃǂ͐�΂ɒǉ����Ȃ��悤�ɂ��Ă��������B
   /// memcpy�Amemset�Ȃǂ̊֐��𗘗p���Ă���\��������܂��B
   /// ���z�֐��Ȃǂ�ǉ�����ƁA���z�֐��e�[�u�����󂳂�܂��B
   /// 
   /// �܂��A���̍\���̂̃I�u�W�F�N�g�̃f�[�^�̓V�F�[�_�[���ɒ萔�o�b�t�@�Ƃ��ē]������Ă��܂��B
   /// �����o�ϐ���ǉ������ꍇ�́AlightCulling.fx�ADeferredLighting.fx���ύX����K�v������܂��B
   /// </remark>
    struct PointLight
    {
    private:
        Vector3 position;       // ���W
        int isUse = false;      // �g�p���t���O�B
        Vector3 positionInView; // �J������Ԃł̍��W
        float pad1;
        Vector3 color;          // ���C�g�̃J���[
        float pad2;
        Vector3 attn;           // �����p�����[�^�Bx�ɉe���͈́Ay�ɂ͉e�����ɗݏ悷��p�����[�^�B
        float pad3;
    public:

        Vector3 POSITION;
        Vector4 COLOR;


        /// <summary>
        /// ���W��ݒ�B
        /// </summary>
        /// <param name="position"></param>
        void SetPosition(const Vector3& position)
        {
            this->position = position;
        }
        void SetPosition(float x, float y, float z)
        {
            SetPosition({ x, y, z });
        }
        /// <summary>
        /// �J���[��ݒ�B
        /// </summary>
        /// <param name="color"></param>
        void SetColor(const Vector3& color)
        {
            this->color = color;
        }
        void SetColor(float r, float g, float b)
        {
            SetColor({ r, g, b });
        }
        /// <summary>
        /// �͈͂�ݒ�B
        /// </summary>
        /// <param name="range"></param>
        void SetRange(float range)
        {
            attn.x = range;
        }
        /// <summary>
        /// �e�����̗ݏ搔��ݒ�B
        /// </summary>
        /// <param name="powParam"></param>
        void SetAffectPowParam(float powParam)
        {
            attn.y = powParam;
        }
        /// <summary>
        /// ���W���擾�B
        /// </summary>
        /// <returns></returns>
        const Vector3& GetPosition() const
        {
            return position;
        }
        /// <summary>
        /// �J���[���擾�B
        /// </summary>
        /// <returns></returns>
        const Vector3& GetColor() const
        {
            return color;
        }
        /// <summary>
        /// �e���͈͂��擾�B
        /// </summary>
        /// <returns></returns>
        float GetRange() const
        {
            return attn.x;
        }
        /// <summary>
        /// �|�C���g���C�g���g�p���ɂ���B
        /// </summary>
        /// /// <remark>
        /// ���̊֐���k2Engine�����ŗ��p����Ă��܂��B
        /// �Q�[��������͎g�p���Ȃ��悤�ɒ��ӂ��Ă��������B
        /// </remark>
        void Use()
        {
            isUse = true;
        }
        /// <summary>
        /// �|�C���g���C�g�𖢎g�p�ɂ���B
        /// </summary>
        /// <remark>
        /// ���̊֐���k2Engine�����ŗ��p����Ă��܂��B
        /// �Q�[��������͎g�p���Ȃ��悤�ɒ��ӂ��Ă��������B
        /// </remark>
        void UnUse()
        {
            isUse = false;
        }
        /// <summary>
        /// �X�V�B
        /// </summary>
        /// <remark>
        /// ���̊֐���k2Engine�����ŗ��p����Ă��܂��B
        /// �Q�[��������͎g�p���Ȃ��悤�ɒ��ӂ��Ă��������B
        /// </remark>
        void Update();

    };

}

*/