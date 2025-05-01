#pragma once

#include <DirectXMath.h>
using namespace DirectX;

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();			/// create view Matrix
	void GetViewMatrix(XMMATRIX&);
	void UpdateViewMatrix(XMVECTOR positionVector, XMVECTOR lookAtVector, XMVECTOR upVector);
	void ProcessKeyboardInput(float deltaTime);


private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;	/// unit == degree
	XMMATRIX m_viewMatrix;
	XMVECTOR  m_cameraFront;		/// positionVector + Front = look at
	XMVECTOR  m_cameraUp;
	XMVECTOR  m_position;
};



inline void CameraClass::UpdateViewMatrix(XMVECTOR positionVector , XMVECTOR lookAtVector , XMVECTOR upVector)
{
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
	return;
}

