#include <gl/glew.h>
#include "shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#pragma once
class Light
{
	GLuint AmbLightID;
	GLuint DiffLightID;
	GLuint LightPosID;
	GLuint SpecLightID;

	vec3 AmbLight;
	vec3 DiffLight;
	vec3 LightPos;

public:
	Light();
	~Light();


};

