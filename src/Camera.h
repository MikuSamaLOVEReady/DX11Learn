#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <d3d11_1.h>
#include <DirectXMath.h>
#include "Transform.h"

extern int orbitID;
class Camera
{
public:
    Camera() = default;
    virtual ~Camera() = 0;

    DirectX::XMVECTOR GetPositionXM() const;
    DirectX::XMFLOAT3 GetPosition() const;

    //
    // 获取摄像机旋转
    //

    // 获取绕X轴旋转的欧拉角弧度
    float GetRotationX() const;
    // 获取绕Y轴旋转的欧拉角弧度
    float GetRotationY() const;

    //
    // 获取摄像机的坐标轴向量
    //

    DirectX::XMVECTOR GetRightAxisXM() const;
    DirectX::XMFLOAT3 GetRightAxis() const;
    DirectX::XMVECTOR GetUpAxisXM() const;
    DirectX::XMFLOAT3 GetUpAxis() const;
    DirectX::XMVECTOR GetLookAxisXM() const;
    DirectX::XMFLOAT3 GetLookAxis() const;

    //
    // 获取矩阵
    //

    DirectX::XMMATRIX GetLocalToWorldMatrixXM() const;
    DirectX::XMMATRIX GetViewMatrixXM() const;
    DirectX::XMMATRIX GetProjMatrixXM(bool reversedZ = false) const;
    DirectX::XMMATRIX GetViewProjMatrixXM(bool reversedZ = false) const;


    virtual void LookTo(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& to, const DirectX::XMFLOAT3& up){};
    virtual void Pitch(float rad){};
    virtual void RotateY(float rad){};
    virtual void SetPosition(const DirectX::XMFLOAT3& pos){};

protected:

    // 摄像机的变换
    Transform m_Transform = {};
    
    // 视锥体属性
    float m_NearZ = 0.0f;
    float m_FarZ = 0.0f;
    float m_Aspect = 0.0f;
    float m_FovY = 0.0f;

    // 当前视口
    D3D11_VIEWPORT m_ViewPort = {};

};

class FirstPersonCamera : public Camera
{
public:
    FirstPersonCamera() = default;
    ~FirstPersonCamera() override;

    // 设置摄像机位置
    void SetPosition(float x, float y, float z);
    void SetPosition(const DirectX::XMFLOAT3& pos);
    // 设置摄像机的朝向
    void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target,const DirectX::XMFLOAT3& up);
    void LookTo(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& to, const DirectX::XMFLOAT3& up);
    // 上下观察
    // 正rad值向上观察
    // 负rad值向下观察
    void Pitch(float rad);
    // 左右观察
    // 正rad值向右观察
    // 负rad值向左观察
    void RotateY(float rad);
};

class ThirdPersonCamera : public Camera
{
public:
    ThirdPersonCamera() = default;
    ~ThirdPersonCamera() override;

    // 获取当前跟踪物体的位置
    DirectX::XMFLOAT3 GetTargetPosition() const;
    // 获取与物体的距离
    float GetDistance() const;
    // 绕物体垂直旋转(注意绕x轴旋转欧拉角弧度限制在[0, pi/3])
    void RotateX(float rad);
    void RotateY(float rad);
    void Approach(float dist);
    // 设置初始绕X轴的弧度(注意绕x轴旋转欧拉角弧度限制在[0, pi/3])
    void SetRotationX(float rad);
    // 设置初始绕Y轴的弧度
    void SetRotationY(float rad);
    // 设置并绑定待跟踪物体的位置
    void SetTarget(const DirectX::XMFLOAT3& target);
    // 设置初始距离
    void SetDistance(float dist);
    // 设置最小最大允许距离
    void SetDistanceMinMax(float minDist, float maxDist);
    // 每帧更新，跟随飞船
    void SetPosition(const DirectX::XMFLOAT3& pos);


private:
    DirectX::XMFLOAT3 m_Target = {};
    float m_Distance = 0.0f;
    // 最小允许距离，最大允许距离
    float m_MinDist = 0.0f, m_MaxDist = 0.0f;
};


#endif
