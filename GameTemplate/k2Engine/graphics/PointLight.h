/*

#pragma once

namespace nsK2Engine {

    /// <summary>
   /// ポイントライト構造体
   /// </summary>
   /// <remark>
   /// この構造体はPOD型として扱っています。
   /// 本構造体に仮想関数などは絶対に追加しないようにしてください。
   /// memcpy、memsetなどの関数を利用している可能性があります。
   /// 仮想関数などを追加すると、仮想関数テーブルが壊されます。
   /// 
   /// また、この構造体のオブジェクトのデータはシェーダー側に定数バッファとして転送されています。
   /// メンバ変数を追加した場合は、lightCulling.fx、DeferredLighting.fxも変更する必要があります。
   /// </remark>
    struct PointLight
    {
    private:
        Vector3 position;       // 座標
        int isUse = false;      // 使用中フラグ。
        Vector3 positionInView; // カメラ空間での座標
        float pad1;
        Vector3 color;          // ライトのカラー
        float pad2;
        Vector3 attn;           // 減衰パラメータ。xに影響範囲、yには影響率に累乗するパラメータ。
        float pad3;
    public:

        Vector3 POSITION;
        Vector4 COLOR;


        /// <summary>
        /// 座標を設定。
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
        /// カラーを設定。
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
        /// 範囲を設定。
        /// </summary>
        /// <param name="range"></param>
        void SetRange(float range)
        {
            attn.x = range;
        }
        /// <summary>
        /// 影響率の累乗数を設定。
        /// </summary>
        /// <param name="powParam"></param>
        void SetAffectPowParam(float powParam)
        {
            attn.y = powParam;
        }
        /// <summary>
        /// 座標を取得。
        /// </summary>
        /// <returns></returns>
        const Vector3& GetPosition() const
        {
            return position;
        }
        /// <summary>
        /// カラーを取得。
        /// </summary>
        /// <returns></returns>
        const Vector3& GetColor() const
        {
            return color;
        }
        /// <summary>
        /// 影響範囲を取得。
        /// </summary>
        /// <returns></returns>
        float GetRange() const
        {
            return attn.x;
        }
        /// <summary>
        /// ポイントライトを使用中にする。
        /// </summary>
        /// /// <remark>
        /// この関数はk2Engine内部で利用されています。
        /// ゲーム側からは使用しないように注意してください。
        /// </remark>
        void Use()
        {
            isUse = true;
        }
        /// <summary>
        /// ポイントライトを未使用にする。
        /// </summary>
        /// <remark>
        /// この関数はk2Engine内部で利用されています。
        /// ゲーム側からは使用しないように注意してください。
        /// </remark>
        void UnUse()
        {
            isUse = false;
        }
        /// <summary>
        /// 更新。
        /// </summary>
        /// <remark>
        /// この関数はk2Engine内部で利用されています。
        /// ゲーム側からは使用しないように注意してください。
        /// </remark>
        void Update();

    };

}

*/