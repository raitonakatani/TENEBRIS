#include "k2EnginePreCompile.h"
#include "SceneLight.h"

namespace nsK2Engine {
    void PointLight::Update()
    {
        // 使用中のライトはカメラ空間での座標を計算する。
        if (!isUse) {
            return;
        }
        positionInView = position;
        g_camera3D->GetViewMatrix().Apply(positionInView);
    }
    void SceneLight::Init()
    {
        // 太陽光
        m_light.directionalLight[0].color.x = 1.5f;
        m_light.directionalLight[0].color.y = 1.5f;
        m_light.directionalLight[0].color.z = 1.5f;



        m_light.directionalLight[0].direction.x = 1.0f;
        m_light.directionalLight[0].direction.y = -1.0f;
        m_light.directionalLight[0].direction.z = -1.0f;
        m_light.directionalLight[0].direction.Normalize();
        m_light.directionalLight[0].castShadow = true;


//        PointLight pointlight;

//        m_light.pointLights[0].SetPosition({ 1.0f,1.0f,1.0f });

/*        m_light.pointLights[0].COLOR.x = 1.0f;
        m_light.pointLights[0].COLOR.y = 1.0f;
        m_light.pointLights[0].COLOR.z = 1.0f;


        m_light.pointLights[0].POSITION.x = 1.0f;
        m_light.pointLights[0].POSITION.y = 1.0f;
        m_light.pointLights[0].POSITION.z = 1.0f;
        m_light.pointLights[0].POSITION.Normalize();
*/        //
      /*  m_light.directionalLight[1].color.x = 1.0f;
        m_light.directionalLight[1].color.y = 1.0f;
        m_light.directionalLight[1].color.z = 1.0f;

        m_light.directionalLight[1].direction.x = -1.0f;
        m_light.directionalLight[1].direction.y = -1.0f;
        m_light.directionalLight[1].direction.z = 1.0f;
        m_light.directionalLight[1].direction.Normalize();
        m_light.directionalLight[1].castShadow = false;

        // 地面からの照り返し
        m_light.directionalLight[2].color.x = 0.6f;
        m_light.directionalLight[2].color.y = 0.6f;
        m_light.directionalLight[2].color.z = 0.6f;

        m_light.directionalLight[2].direction.x = -1.0f;
        m_light.directionalLight[2].direction.y = 1.0f;
        m_light.directionalLight[2].direction.z = 1.0f;
        m_light.directionalLight[2].direction.Normalize();*/

        m_light.ambinetLight.x = 0.1f;
        m_light.ambinetLight.y = 0.1f;
        m_light.ambinetLight.z = 0.1f;
        m_light.eyePos = g_camera3D->GetPosition();
        m_light.numPointLight = 0;

        // 全てのポイントライトを未使用にする
        for (auto& pt : m_light.pointLights) {
            pt.UnUse();
            pt.SetAffectPowParam(1.0f);
            m_unusePointLightQueue.push_back(&pt);
        }
    }
    void SceneLight::SetIBLTextureForAmbient(const wchar_t* textureFilePath, float luminance)
    {
        g_renderingEngine->ReInitIBL(textureFilePath, luminance);
    }
    void SceneLight::Update()
    {
        for (auto& pt : m_light.pointLights) {
            pt.Update();
        }
        m_light.numPointLight = MAX_POINT_LIGHT - static_cast<int>(m_unusePointLightQueue.size());
    }
}