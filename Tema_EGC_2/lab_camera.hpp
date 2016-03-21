//-------------------------------------------------------------------------------------------------
// Descriere: header camera
//
// Autor: Lucian Petrescu
// Data: 14 oct 2013
//-------------------------------------------------------------------------------------------------

#define _USE_MATH_DEFINES
#include <math.h>

#pragma once
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"


// Rotates a Point/Vector around the world OY (0, 1, 0) with a specific angle(radians)
// This just implements the rotation matrix around the world UP vector that we learned in the previous lab
// For rotating a vector/point around an axis use glm::rotate()
inline glm::vec3 RotateOY(const glm::vec3 Point, float radians)
{
	glm::vec3 R;
	R.x = Point.x * cos(radians) - Point.z * sin(radians);
	R.y = Point.y;
	R.z = Point.x * sin(radians) + Point.z * cos(radians);
	return R;
}

//**************************************************************
//**************************************************************
//		Comments are there for a reason - READ THEM !!!!!
//		Comments are there for a reason - READ THEM !!!!!
//		Comments are there for a reason - READ THEM !!!!!
//		Comments are there for a reason - READ THEM !!!!!
//		Comments are there for a reason - READ THEM !!!!!
//						
//										  before asking stupid questions
//
//		P.S  Questions are welcomed!
//			 Just make sure the explanation isn't already given 
//			 in comments or the Lab docummentation
//
//**************************************************************
//**************************************************************

namespace lab
{
	class Camera
	{
	public:
		Camera()
		{
			position = glm::vec3(0, 0, 50);
			forward = glm::vec3(0, 0, -1);
			up		= glm::vec3(0, 1, 0);
			right	= glm::vec3(1, 0, 0);
			distanceToTarget = 40;
		}

		Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
		{
			set(position, center,up);
		}

		~Camera()
		{ }

		// Update camera
		void set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
		{
			this->position = position;
			forward = glm::normalize(center-position);
			right	= glm::cross(forward, up);
			this->up = glm::cross(right,forward);
		}

		void moveForwardKeepOY(float distance)
		{
			glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
			position += dir * distance;
		}

		void translateForward(float distance)
		{
			position += forward * distance;
		}

		void translateUpword(float distance)
		{
			position += up * distance;
		}

		void translateRight(float distance)
		{
			glm::vec3 dir = glm::normalize(glm::vec3(right.x, 0, right.z));
			position += dir	* distance;
		}

		// Rotate the camera in FPS mode over the local OX axis
		void rotateFPS_OX(float angle)
		{
			glm::mat3 matrix = glm::mat3(glm::rotate(glm::mat4(1), angle, right));
			up = glm::normalize(matrix * up);
			forward = glm::normalize(matrix * forward);
		}

		void rotateFPS_OY(float angle)
		{
			glm::mat3 matrix = glm::mat3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)));
			right = glm::normalize(matrix * right);
			forward = glm::normalize(matrix * forward);
			up = glm::normalize(matrix * up);
		}

		void rotateFPS_OZ(float angle)
		{
			glm::mat3 matrix = glm::mat3(glm::rotate(glm::mat4(1), angle, forward));
			up = glm::normalize(matrix * up);
			right = glm::normalize(matrix * right);
		}

		void rotateTPS_OX(float angle)
		{
			translateForward(distanceToTarget);
			rotateFPS_OX(angle);
			translateForward(-distanceToTarget);
		}

		void rotateTPS_OY(float angle)
		{
			translateForward(distanceToTarget);
			rotateFPS_OY(angle);
			translateForward(-distanceToTarget);
		}

		void rotateTPS_OZ(float angle)
		{
			translateForward(distanceToTarget);
			rotateFPS_OZ(angle);
			translateForward(-distanceToTarget);
		}

		glm::mat4 getViewMatrix()
		{
			// Return the View Matrix
			return glm::lookAt(position, position + forward, up);
		}

		glm::vec3 getTargetPosition()
		{
			return position + forward * distanceToTarget;
		}

		glm::vec3 getPosition()
		{
			return position;
		}

		glm::vec3 getForward()
		{
			return forward;
		}

		float distanceToTarget;
		glm::vec3 position;
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;
	};
}