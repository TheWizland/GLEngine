#pragma once
#include <GLM/vec4.hpp>
#include <GLM/vec3.hpp>
#include <GLM/mat4x4.hpp>
#include <GL\glew.h>
#include <GLM/gtc/matrix_transform.hpp>
#include "LightStruct.h"

namespace Lighting
{
	extern glm::vec4 globalAmbient;

	class Light
	{
	private:
	protected:
		glm::vec3 position = glm::vec3(0, 0, 0); //Positional + Spot
		//Position is also used for Directional light sources to handle shadow mapping.
		glm::vec3 direction = glm::vec3(0, -1, 0); //Spot + Directional
		glm::mat4 perspective;
		LightType type;
		float cutofFAngle = 30; //Spot
		float cutoffExponent = 1; //Spot
		//TODO: Maybe shadow map should be stored inside Light? Would provide better support for multiple lights.
		//Positional lights should use a cubemap.
	public: 
		ADS ads;
		Attenuation attenuation;

		LightType getType() { return type; }
		
		glm::vec3 getPosition() const { return position; }
		//For directional lights, position should be set to somewhere outside the scene (for shadow mapping).
		void setPosition(float x, float y, float z);
		void setPosition(glm::vec3 xyz);
		void translate(float x, float y, float z);
		void translate(glm::vec3 xyz);
		

		glm::vec3 getDirection() const { return direction; }
		glm::mat4 getView() const;

		virtual void setPerspective() = 0;
		virtual void setPerspective(float aspectRatio, float size) = 0;
		glm::mat4 getPerspective() const { return perspective; }
	};

	class PositionalLight : public Light
	{
	public:
		PositionalLight()
		{
			type = LightType::Positional;
			setPerspective();
		}

		void setPerspective() override
		{
			setPerspective(4.f / 3.f, 90.f);
		}

		void setPerspective(float aspectRatio, float fov = 90.f) override
		{
			perspective = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.f);
		}
	};

	class DirectionalLight : public Light
	{
	public:
		DirectionalLight()
		{
			type = LightType::Directional;
			attenuation.linear = 0;
			attenuation.quadratic = 0;
			setPerspective(4.f / 3.f);
		}

		void setPerspective() override
		{
			setPerspective(4.f / 3.f, 10.f);
		}

		void setPerspective(float aspectRatio, float width = 10.f) override
		{
			float left = -width;
			float right = width;
			float bottom = -width / aspectRatio;
			float top = width / aspectRatio;
			perspective = glm::ortho(left, right, bottom, top, 0.1f, 100.f);
		}

		void setDirection(glm::vec3 direction)
		{
			this->direction = direction;
		}
	};

	class SpotLight : public Light
	{
	public:
		SpotLight()
		{
			type = LightType::Spot;
			setPerspective();
		}

		void setPerspective() override
		{
			setPerspective(4.f / 3.f, 90.f);
		}

		void setPerspective(float aspectRatio, float fov = 90.f) override
		{
			perspective = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.f);
		}

		void setDirection(glm::vec3 direction)
		{
			this->direction = direction;
		}
	};



	/*
	--POSITIONAL LIGHT REQUIREMENTS
	Projection Perspective

	--SPOT LIGHT REQUIREMENTS
	Projection Perspective
	Direction
	Cutoff Angle/Exponent

	--DIRECTIONAL LIGHT REQUIREMENTS
	Orthogonal Perspective: Should be big enough for the entire area for shadows to be rendered.
	Direction

	--SHARED REQUIREMENTS
	ADS
	Attenuation
	Position (Directional only needs it for the sake of rendering shadow maps)\
	Shadow Texture (Positional needs CubeMap, but CubeMap is also a GLuint)
	*/
}