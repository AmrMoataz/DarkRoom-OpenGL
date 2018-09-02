#ifndef Renderer_h__
#define Renderer_h__

#include <gl\glew.h>
#include <gl\glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "shader.hpp"
#include "Model.h"
#include "FirstPersonCamera.h"
#include "texture.h"
#include "ShaderProgram.h"
#include "Model3D.h"
#include <windows.h> 

using namespace glm;
using namespace std;

class Renderer
{
	GLuint ProgramID;
	GLuint VertexBufferID;
	GLuint VertexArrayID;
	GLuint ViewProjectionMatrixID;
	GLuint ModelMatrixID;
	GLuint ColorID;

	FirstPersonCamera* MyCamera;
	FirstPersonCamera* SecondaryCam;

	Model* MySquare;
	Model* MyObject;

	ShaderProgram StaticShader;

	Model3D StaticModel; // Stores 3D Model Data
	Model3D Crate;

	mat4 StaticModelMatrix; // will be used to transform the model

	mat4 ModelMatrix, S, R, T;

	vec3 TheColor;

	//Light
	GLuint AmbLightID;
	GLuint DiffLightID;
	GLuint LightPosID;
	GLuint SpecLightID;
	GLuint MaterialID;
	GLuint EyePosID;
	GLuint FlashLightID;

	vec3 AmbLight;
	vec3 DiffLight;
	vec3 LightPos;
	vec3 SpecLight;
	vec3 EyePos;
	vec3 StoreMainCamPos;

	vec3 Postions[20];

	float Material;

	double PreTime;

	bool OnMainCam = true;
	bool OpenDoor = false; //check if door is opened or not
	bool OpenedOnce = false; //check if door was opened for the first time
	bool Walk = true; //used to handle collision
	

	int FlashLightBool;

	//Skybox
	Texture* Front;
	Texture* Back;
	Texture* Right;
	Texture* Left;
	Texture* Up;
	Texture* Ground;
	Texture* Wood;
	Texture* HouseText;
	Texture* Door;
	Texture* Floor;
	Texture* outdoorGround;

public:
	Renderer();
	~Renderer();

	void Initialize(int WindowWidth, int WindowHeight);
	void Draw();
	void HandleKeyboardInput(int Key, int Action);
	void HandleMouseMove(double DeltaX, double DeltaY);
	void HandleMouseClick(int MouseAction, int MouseButton, double MouseXPos, double MouseYPos);
	void HandleWindowResize(int WindowWidth, int WindowHeight);
	void Update();
	bool IsColliding(vec3 Player, vec3 Collider);
	void ChangeCam();
	void Cleanup();
};

#endif