#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

enum camMove {
	W,
	S,
	D,
	A,
	UP,
	DOWN,
	R
};
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENS = 0.1f;
const float FOV = 45.0f;
class Camera {
public:
	glm::vec3 Pos;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	float Speed;
	float Sens;
	float Fov;
	
	Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) :Front(glm::vec3(0.0f, 0.0f, -1.0f)), Speed(SPEED), Sens(SENS), Fov(FOV)
	{
		Pos = pos;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		update();
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), Speed(SPEED), Sens(SENS), Fov(FOV)
	{
		Pos = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		update();
	}
	glm::mat4 getView()
	{
		return glm::lookAt(Pos, Pos + Front, Up);
	}
		
	void keyboard(camMove direction, float deltaTime)
	{
		float camSpeed = Speed * deltaTime;
		if (direction == W)
			Pos += Front * camSpeed;
		if (direction == S)
			Pos -= Front * camSpeed;
		if (direction == D)
			Pos += Right * camSpeed;
		if (direction == A)
			Pos -= Right * camSpeed;
		if (direction == UP)
			Pos += WorldUp * camSpeed;
		if (direction == DOWN)
			Pos -= WorldUp * camSpeed;
		if (direction == R)
		{
			glm::vec3 dir;
			Yaw = -90.0f;
			Pitch = 0.0f;
			dir.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			dir.y = sin(glm::radians(Pitch));
			dir.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			Front = glm::normalize(dir);
			Pos = glm::vec3(0.0f, 0.0f, 3.0f);
		}
	}
	
	void mouseMove(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= Sens;
		yoffset *= Sens;

		Yaw += xoffset;
		Pitch += yoffset;

		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}
		update();
	}
	void mouseCroll(float yoffset)
	{
		Fov -= yoffset;
		if (Fov < 1.0f)
			Fov = 1.0f;
		if (Fov > 45.0f)
			Fov = 45.0f;
	}
private:
	void update()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

};
#endif