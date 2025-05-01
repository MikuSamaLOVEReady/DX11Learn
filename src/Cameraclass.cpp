#include "Cameraclass.h"

#include "Applicationclass.h"

CameraClass::CameraClass():m_positionX(0.0f),m_positionY(0.0f),m_positionZ(0.0f),
                           m_rotationX(0.0f),m_rotationY(0.0f),m_rotationZ(0.0f),
m_cameraUp(XMVectorSet(0.0f,1.0f,0.0f,0.0f)),
m_cameraFront(XMVectorSet(0.0f,0.0f,1.0f,0.0f)),
m_position(XMVectorSet(m_positionX, m_positionY, m_positionZ,0.0f))
{
}

CameraClass::CameraClass(const CameraClass& other)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x , float y,  float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	m_position = XMVectorSet(m_positionX,
		m_positionY,
		m_positionZ,
		1.0f);
	return;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_positionZ = z;
	return;
}

XMFLOAT3 CameraClass::GetPosition()
{
	return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}

XMFLOAT3 CameraClass::GetRotation()
{
	return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

/// build View Matrix
void CameraClass::Render()
{

	  
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	XMVECTOR upVector = m_cameraUp;
	XMVECTOR positionVector = m_position;
	XMVECTOR lookAtVector = m_cameraFront;

	/// rotate() 
	pitch = m_rotationX * 0.0174532925f;		/// degree -> rad
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	/// transform to
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	lookAtVector = XMVectorAdd(positionVector, lookAtVector);	/// pos + front = lookat

	UpdateViewMatrix(positionVector,lookAtVector,upVector);
	return;
}

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}

void CameraClass::ProcessKeyboardInput(float deltaTime)
{
	XMVECTOR cur_pos = m_position;
	XMVECTOR cur_front = m_cameraFront;
	XMVECTOR cur_up = m_cameraUp;

	XMVECTOR forward = XMVector3Normalize(cur_pos + cur_front);
	XMVECTOR right = XMVector3Normalize(XMVector3Cross(cur_up, forward));

	XMVECTOR moveDelta = XMVectorZero();
	const float speed = 15.0f;

	if (GetAsyncKeyState('W') & 0x8000)
		moveDelta -= forward;
	if (GetAsyncKeyState('S') & 0x8000)
		moveDelta += forward;
	if (GetAsyncKeyState('A') & 0x8000)
		moveDelta += right;
	if (GetAsyncKeyState('D') & 0x8000)
		moveDelta -= right;

	if (!XMVector3Equal(moveDelta, XMVectorZero()))
	{
		moveDelta = XMVector3Normalize(moveDelta)
			* speed
			* deltaTime;
		m_position = XMVectorAdd(m_position, moveDelta);
		//g_CamTarget = XMVectorAdd(g_CamTarget, moveDelta);
	}

}



