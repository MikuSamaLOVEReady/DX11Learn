#include "Camera.h"
using namespace DirectX;
int orbitID = 0;

Camera::~Camera()
{
}

XMVECTOR Camera::GetPositionXM() const
{
    return m_Transform.GetPositionXM();
}

XMFLOAT3 Camera::GetPosition() const
{
    return m_Transform.GetPosition();
}

float Camera::GetRotationX() const
{
    return m_Transform.GetRotation().x;
}

float Camera::GetRotationY() const
{
    return m_Transform.GetRotation().y;
}


XMVECTOR Camera::GetRightAxisXM() const
{
    return m_Transform.GetRightAxisXM();
}

XMFLOAT3 Camera::GetRightAxis() const
{
    return m_Transform.GetRightAxis();
}

XMVECTOR Camera::GetUpAxisXM() const
{
    return m_Transform.GetUpAxisXM();
}

XMFLOAT3 Camera::GetUpAxis() const
{
    return m_Transform.GetUpAxis();
}

XMVECTOR Camera::GetLookAxisXM() const
{
    return m_Transform.GetForwardAxisXM();
}

XMFLOAT3 Camera::GetLookAxis() const
{
    return m_Transform.GetForwardAxis();
}

DirectX::XMMATRIX Camera::GetLocalToWorldMatrixXM() const
{
    return m_Transform.GetLocalToWorldMatrixXM();
}

XMMATRIX Camera::GetViewMatrixXM() const
{
    return m_Transform.GetWorldToLocalMatrixXM();
}

XMMATRIX Camera::GetProjMatrixXM(bool reversedZ) const
{
    if (reversedZ)
        return XMMatrixPerspectiveFovLH(m_FovY, m_Aspect, m_FarZ, m_NearZ);
    else
        return XMMatrixPerspectiveFovLH(m_FovY, m_Aspect, m_NearZ, m_FarZ);
}

XMMATRIX Camera::GetViewProjMatrixXM(bool reversedZ) const
{
    return GetViewMatrixXM() * GetProjMatrixXM(reversedZ);
}



// ******************
// 第一人称/自由视角摄像机
//

FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::SetPosition(float x, float y, float z)
{
    SetPosition(XMFLOAT3(x, y, z));
}

void FirstPersonCamera::SetPosition(const XMFLOAT3& pos)
{
    m_Transform.SetPosition(pos);
}

void FirstPersonCamera::LookAt(const XMFLOAT3 & pos, const XMFLOAT3 & target,const XMFLOAT3 & up)
{
    m_Transform.SetPosition(pos);
    m_Transform.LookAt(target, up);
}

void FirstPersonCamera::LookTo(const XMFLOAT3 & pos, const XMFLOAT3 & to, const XMFLOAT3 & up)
{
    m_Transform.SetPosition(pos);
    m_Transform.LookTo(to, up);
}



void FirstPersonCamera::Pitch(float rad)
{
    XMFLOAT3 rotation = m_Transform.GetRotation();
    // 将绕x轴旋转弧度限制在[-7pi/18, 7pi/18]之间
    rotation.x += rad;
    if (rotation.x > XM_PI * 7 / 18)
        rotation.x = XM_PI * 7 / 18;
    else if (rotation.x < -XM_PI * 7 / 18)
        rotation.x = -XM_PI * 7 / 18;

    m_Transform.SetRotation(rotation);
}

void FirstPersonCamera::RotateY(float rad)
{
    XMFLOAT3 rotation = m_Transform.GetRotation();
    rotation.y = XMScalarModAngle(rotation.y + rad);
    m_Transform.SetRotation(rotation);
}



// ******************
// 第三人称摄像机
//

ThirdPersonCamera::~ThirdPersonCamera()
{
}

XMFLOAT3 ThirdPersonCamera::GetTargetPosition() const
{
    return m_Target;
}

float ThirdPersonCamera::GetDistance() const
{
    return m_Distance;
}

void ThirdPersonCamera::RotateX(float rad)
{
    XMFLOAT3 rotation = m_Transform.GetRotation();
    // 将绕x轴旋转弧度限制在[0, pi/3]之间
    rotation.x += rad;
    if (rotation.x < 0.0f)
        rotation.x = 0.0f;
    else if (rotation.x > XM_PI / 3)
        rotation.x = XM_PI / 3;

    m_Transform.SetRotation(rotation);
    m_Transform.SetPosition(m_Target);
    m_Transform.Translate(m_Transform.GetForwardAxis(), -m_Distance);
}

void ThirdPersonCamera::RotateY(float rad)
{
    XMFLOAT3 rotation = m_Transform.GetRotation();
    rotation.y = XMScalarModAngle(rotation.y + rad);

    m_Transform.SetRotation(rotation);
    m_Transform.SetPosition(m_Target);
    m_Transform.Translate(m_Transform.GetForwardAxis(), -m_Distance);
}

void ThirdPersonCamera::Approach(float dist)
{
    m_Distance += dist;
    // 限制距离在[m_MinDist, m_MaxDist]之间
    if (m_Distance < m_MinDist)
        m_Distance = m_MinDist;
    else if (m_Distance > m_MaxDist)
        m_Distance = m_MaxDist;

    m_Transform.SetPosition(m_Target);
    m_Transform.Translate(m_Transform.GetForwardAxis(), -m_Distance);
}

void ThirdPersonCamera::SetRotationX(float rad)
{
    XMFLOAT3 rotation = m_Transform.GetRotation();
    // 将绕x轴旋转弧度限制在[0, pi/3]之间
    rotation.x = rad;
    if (rotation.x < 0.0f)
        rotation.x = 0.0f;
    else if (rotation.x > XM_PI / 3)
        rotation.x = XM_PI / 3;

    m_Transform.SetRotation(rotation);
    m_Transform.SetPosition(m_Target);
    m_Transform.Translate(m_Transform.GetForwardAxis(), -m_Distance);
}

void ThirdPersonCamera::SetRotationY(float rad)
{
    XMFLOAT3 rotation = m_Transform.GetRotation();
    rotation.y = XMScalarModAngle(rad);
    m_Transform.SetRotation(rotation);
    m_Transform.SetPosition(m_Target);
    m_Transform.Translate(m_Transform.GetForwardAxis(), -m_Distance);
}

void ThirdPersonCamera::SetTarget(const XMFLOAT3 & target)
{
    m_Target = target;
}

void ThirdPersonCamera::SetDistance(float dist)
{
    m_Distance = dist;
}

void ThirdPersonCamera::SetDistanceMinMax(float minDist, float maxDist)
{
    m_MinDist = minDist;
    m_MaxDist = maxDist;
}

void ThirdPersonCamera::SetPosition(const DirectX::XMFLOAT3& pos)
{
    m_Transform.SetPosition(pos);
    m_Transform.Translate(m_Transform.GetForwardAxis(), -m_Distance);
}

