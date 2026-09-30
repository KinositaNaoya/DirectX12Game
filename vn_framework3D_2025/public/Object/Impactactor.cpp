#include "../../framework.h"
#include "../../framework/vn_environment.h"

/// <summary>
/// ノックバックの実処理関数
/// </summary>
void ImpactActor::ForceUpdate()
{
    if (m_KnockBackTarget == nullptr)
        return;

    float MoveDistance = m_KnockBackForce;

    // 残り距離
    float RemainingDistance = m_KnockBackDistance - m_KnockBackMovedDistance;

    // 残り距離より大きく移動しない
    if (MoveDistance > RemainingDistance)MoveDistance = RemainingDistance;

    // 移動量を作る
    XMVECTOR Vel = XMVectorScale(m_KnockBackDirection, MoveDistance);

    // Targetを移動
    m_KnockBackTarget->addPosition(&Vel);

    // 移動した距離を記録
    m_KnockBackMovedDistance += MoveDistance;

    if(m_KnockBackForce > 0.2f) m_KnockBackForce *= 0.95f;

    // 指定距離まで飛んだら終了
    if (m_KnockBackMovedDistance >= m_KnockBackDistance)
    {
        m_KnockBackTarget = nullptr;
        m_KnockBackForce = 0.0f;
        m_KnockBackDistance = 0.0f;
        m_KnockBackMovedDistance = 0.0f;
    }
}

/// <summary>
/// 自分の位置からTargetに対してノックバックを行う
/// </summary>
/// <param name="Target">ノックバック対象オブジェクト</param>
/// <param name="Distance">ノックバック距離</param>
/// <param name="Force">ぱわー</param>
void ImpactActor::KnockBackObject(vnObject* Target, float Distance,float Force)
{

    if (Target == nullptr)return;

    XMVECTOR TargetPos = *Target->getPosition();

    // ImpactActor → Target
    XMVECTOR Direction =
        TargetPos - *this->getPosition();

    // 同じ位置なら終了
    if (XMVectorGetX(XMVector3LengthSq(Direction)) < 0.0001f)
        return;

    Direction = XMVector3Normalize(Direction);

    // ノックバック情報を保存
    m_KnockBackTarget = Target;
    m_KnockBackDirection = Direction;

    m_KnockBackForce = Force;
    m_KnockBackDistance = Distance;
    m_KnockBackMovedDistance = 0.0f;
}
