#include "FirstPersonCamera.h"


FirstPersonCamera::FirstPersonCamera()
{
	this->SetLookAt(0, 0, 1,
		0, 0, 0,
		0, 1, 0);

	SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
}




FirstPersonCamera::~FirstPersonCamera()
{
}

vec3 FirstPersonCamera::GetLookDirection()
{
	return -mDirection;
}

void FirstPersonCamera::SetLookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ)
{
	vec3 eyePt(eyeX, eyeY, eyeZ);
	vec3 centerPt(centerX, centerY, centerZ);
	vec3 upVec(upX, upY, upZ);

	this->SetLookAt(eyePt, centerPt, upVec);
}

void FirstPersonCamera::SetLookAt(const vec3 &eye, const vec3 &center, vec3 &up)
{
	mPosition = eye;

	mDirection = normalize(mPosition - center);
	mRight = normalize(cross(up, mDirection));
	mUp = normalize(cross(mDirection, mRight));
}

mat4 FirstPersonCamera::GetViewMatrix()
{
	vec3 mCenter = mPosition + this->GetLookDirection();
	return lookAt(mPosition, mCenter, mUp);
}

mat4 FirstPersonCamera::GetProjectionMatrix()
{
	return mProjectionMatrix;
}

void FirstPersonCamera::SetPerspectiveProjection(float FOV, float aspectRatio, float near, float far)
{
	mProjectionMatrix = perspective(FOV, aspectRatio, near, far);
}

void FirstPersonCamera::Slide(float stepR, float stepU, float stepD)
{
	mPosition += stepR * movingSpeed * mRight;
	mPosition += stepU * movingSpeed * mUp;
	mPosition += stepD * movingSpeed *movingSpeed * this->GetLookDirection();
	//cout << mPosition.z << endl;
}

void FirstPersonCamera::Yaw(float angleDegrees)
{
	mDirection = glm::rotate(mDirection, angleDegrees * CameraSpeed, mUp);
	mRight = glm::rotate(mRight, angleDegrees * CameraSpeed, mUp);
}

void FirstPersonCamera::Pitch(float angleDegrees)
{
	mUp = glm::rotate(mUp, angleDegrees * CameraSpeed, mRight);
	mDirection = glm::rotate(mDirection, angleDegrees * CameraSpeed, mRight);
}

void FirstPersonCamera::Roll(float angleDegrees)
{
	mRight = glm::rotate(mRight, angleDegrees * CameraSpeed, mDirection);
	mUp = glm::rotate(mUp, angleDegrees * CameraSpeed, mDirection);
}

void FirstPersonCamera::Walk(float dist)
{
	
	mPosition += dist * movingSpeed * this->GetLookDirection();
	//not letting the camera fall down
	if (mPosition.y < -0.5f || mPosition.y > 0.5f)
		mPosition.y = 0.0f;

	//handling moving over the road outside the house
	if (outofHouse && (mPosition.x >= 4.0f || mPosition.x <= -4.0f))
		mPosition.x = 0;
	cout << "X: " << mPosition.x << " Y: " << mPosition.y << " Z: " << mPosition.z << endl;
}

void FirstPersonCamera::Strafe(float dist)
{
	mPosition += dist * movingSpeed *mRight;
	//not letting the camera fall down
	if (mPosition.y < -0.5f || mPosition.y > 0.5f)
		mPosition.y = 0.0f;
	//handling moving over the road outside the house
	if (outofHouse && (mPosition.x >= 4.0f || mPosition.x <= -4.0f))
		mPosition.x = 0;

	cout << "X: " << mPosition.x << " Y: " << mPosition.y << " Z: " << mPosition.z << endl;
}


//we no more use this function
void FirstPersonCamera::Fly(float dist)
{
	mPosition += dist * movingSpeed * mUp;
	if (mPosition.y < -0.5f || mPosition.y > 0.5f)
		mPosition.y = 0.0f;
	cout << "X: " << mPosition.x << " Y: " << mPosition.y << " Z: " << mPosition.z << endl;
}

glm::vec3 FirstPersonCamera::GetEyePosition()
{
	return mPosition;
}

void FirstPersonCamera::checkOutOfHouse()
{
	if (mPosition.z >= -22.0f)
	{
		outofHouse = false;
	}
	else
	{
		outofHouse = true;
	}
}

bool FirstPersonCamera::gethouseBool()
{
	return outofHouse;
}