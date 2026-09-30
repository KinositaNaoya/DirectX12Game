#pragma once

class ImpactActor : public vnObject
{
public:

	void ForceUpdate();

	void KnockBackObject(vnObject* Target, float Distance = 20.0f, float Force = 0.5f);

private:

	vnObject* m_KnockBackTarget = nullptr;

	XMVECTOR m_KnockBackDirection = XMVectorZero();

	float m_KnockBackForce = 0.0f;
	float m_KnockBackDistance = 0.0f;
	float m_KnockBackMovedDistance = 0.0f;

};