#include "Renderer.h"

Renderer::Renderer()
{

}

Renderer::~Renderer()
{
	Cleanup();
}

void Renderer::Initialize(int WindowWidth, int WindowHeight)
{


	PreTime = glfwGetTime();

	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//////////////////////////////////////////////////////////////////////////
	// Camera
	MyCamera = new FirstPersonCamera();
	SecondaryCam = new FirstPersonCamera();
	// Projection matrix : 
	MyCamera->SetPerspectiveProjection(45.0f, (float)WindowWidth / (float)WindowHeight, 0.1f, 10000.0f);
	SecondaryCam->SetPerspectiveProjection(45.0f, (float)WindowWidth / (float)WindowHeight, 0.1f, 10000.0f);
	
	// View matrix : 
	MyCamera->SetLookAt(
		vec3(+0.0f, +0.0f, -80.0f),// Camera Position
		vec3(+0.0f, +0.0f, +0.0f),// Look at Point
		vec3(+0.0f, +1.0f, +0.0f) // Up Vector
		);

	SecondaryCam->SetLookAt(
		vec3(+0.0f, +40.0f, -60.0f),// Camera Position
		vec3(+0.0f, +0.0f, +0.0f),// Look at Point
		vec3(+0.0f, +1.0f, +0.0f) // Up Vector
	);

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Create and compile our GLSL program from the shaders
	ProgramID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	// Use our shader
	glUseProgram(ProgramID);
	// Get the ID of VP from shaders
	ViewProjectionMatrixID = glGetUniformLocation(ProgramID, "ViewProjectionMatrix");
	// Get the ID of ModelMatrix from shaders
	ModelMatrixID = glGetUniformLocation(ProgramID, "ModelMatrix");

	ColorID = glGetUniformLocation(ProgramID, "TheColor");

	AmbLightID = glGetUniformLocation(ProgramID, "AmbLight");

	DiffLightID = glGetUniformLocation(ProgramID, "DiffLight");

	LightPosID = glGetUniformLocation(ProgramID, "LightPos");

	MaterialID = glGetUniformLocation(ProgramID, "Material");

	EyePosID = glGetUniformLocation(ProgramID, "EyePos");

	FlashLightID = glGetUniformLocation(ProgramID, "FlashLightBool");
	
	
	

	//////////////////////////////////////////////////////////////////////////

	AmbLight = vec3(0.0f, 0.0f, 0.02f);

	DiffLight = vec3(0.0f, 0.2f, 0.4f);

	LightPos = MyCamera->GetEyePosition();

	EyePos = MyCamera->GetLookDirection();

	FlashLightBool = 1;

	Front = new Texture("DarkStormyFront2048.png", 0);
	Back = new Texture("DarkStormyBack2048.png", 0);
	Left = new Texture("DarkStormyLeft2048.png", 0);
	Right = new Texture("DarkStormyRight2048.png", 0);
	Up = new Texture("DarkStormyUp2048.png", 0);
	Ground = new Texture("DarkStormyDown2048.png", 0);
	Wood = new Texture("TexturesCom_PlywoodNew0077_S.jpg", 0);
	HouseText = new Texture("TexturesCom_PlywoodOld0025_18_S.jpg", 0);
	Door = new Texture("TexturesCom_DoorsWoodSingleOld0240_1_S.jpg", 0);
	Floor = new Texture("TexturesCom_WoodPlanksFloors0010_2_S.jpg", 0);
	outdoorGround = new Texture("TexturesCom_Roads0075_1_seamless_S.jpg", 0);

	StaticShader.LoadProgram();

	StaticModel.LoadFromFile("Car/CHALLENGER71.3ds", true);
	StaticModel.Initialize();

	//////////////////////////////////////////////////////////////////////////
	// Initialization using the Ordinary Way
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLfloat VertexData[] = {
		-1.0f, +1.0f, 0.0f, 1, 0, 0,
		+1.0f, +1.0f, 0.0f, 1, 0, 0,
		-1.0f, -1.0f, 0.0f, 1, 0, 0,
		+1.0f, +1.0f, 0.0f, 1, 0, 0,
		-1.0f, -1.0f, 0.0f, 1, 0, 0,
		+1.0f, -1.0f, 0.0f, 1, 0, 0,
	};

	glGenBuffers(1, &VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	S = scale(0.5f, 0.5f, 0.5f);
	T = translate(-1.0f, 0.0f, -1.0f);
	R = rotate(0.0f, vec3(0, 0, 1));
	ModelMatrix = T * R * S;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Initialization using Model Class
	// Square consists of 4 vertices
	MySquare = new Model();
	// adding 4 positions
	MySquare->VertexData.push_back(vec3(-1, +1, 0));
	MySquare->VertexData.push_back(vec3(+1, +1, 0));
	MySquare->VertexData.push_back(vec3(+1, -1, 0));
	MySquare->VertexData.push_back(vec3(-1, -1, 0));
	// adding 4 colors
	MySquare->ColorData.push_back(vec3(+1, +1, +1));
	MySquare->ColorData.push_back(vec3(+1, +1, +1));
	MySquare->ColorData.push_back(vec3(+1, +1, +1));
	MySquare->ColorData.push_back(vec3(+1, +1, +1));
	// adding 4 normals
	MySquare->NormalsData.push_back(vec3(+0, +0, +1));
	MySquare->NormalsData.push_back(vec3(+0, +0, +1));
	MySquare->NormalsData.push_back(vec3(+0, +0, +1));
	MySquare->NormalsData.push_back(vec3(+0, +0, +1));
	// adding 6 indices
	MySquare->IndicesData.push_back(0);
	MySquare->IndicesData.push_back(1);
	MySquare->IndicesData.push_back(2);
	MySquare->IndicesData.push_back(0);
	MySquare->IndicesData.push_back(2);
	MySquare->IndicesData.push_back(3);
	//adding 4 UVs
	MySquare->UVData.push_back(vec2(0, 0));
	MySquare->UVData.push_back(vec2(1, 0));
	MySquare->UVData.push_back(vec2(1, 1));
	MySquare->UVData.push_back(vec2(0, 1));
	// send data to GPU
	MySquare->Initialize();


	// Transformations
	S = scale(2.0f, 2.0f, 2.0f);
	T = translate(0.0f, 0.0f, -2.0f);
	R = rotate(45.0f, vec3(0, 0, 1));
	MySquare->ModelMatrix = T * R * S;
	//////////////////////////////////////////////////////////////////////////
}
void Renderer::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(ProgramID);

	mat4 ViewProjectionMatrix;

	if (OnMainCam)
	{
		ViewProjectionMatrix = MyCamera->GetProjectionMatrix() * MyCamera->GetViewMatrix();
		glUniformMatrix4fv(ViewProjectionMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);
	}
	else
	{
		ViewProjectionMatrix = SecondaryCam->GetProjectionMatrix() * SecondaryCam->GetViewMatrix();
		glUniformMatrix4fv(ViewProjectionMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);
	}
	
	
	//////////////////////////////////////////////////////////////////////////
	// Drawing using the Ordinary Way
	glBindVertexArray(VertexArrayID);

#pragma region SkyBox

	//FRONT
	S = scale(1000.0f, 1000.0f, 1000.0f);
	T = translate(0.0f, 0.0f, -1000.0f);
	R = rotate(0.0f, vec3(0, 0, 1));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 10000000;
	glUniform1fv(MaterialID, 1, &Material);
	
	Front->Bind();
	MySquare->Draw();

	//UP
	S = scale(1000.0f, 1000.0f, 1000.0f);
	T = translate(0.0f, 1000.0f, 0.0f);
	R = rotate(90.0f, vec3(1, 0, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 10000000;
	glUniform1fv(MaterialID, 1, &Material);
	Up->Bind();
	MySquare->Draw();

	//BACK
	S = scale(1000.0f, 1000.0f, 1000.0f);
	T = translate(0.0f, 0.0f, 1000.0f);
	R = rotate(180.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 10000000;
	glUniform1fv(MaterialID, 1, &Material);
	Back->Bind();
	MySquare->Draw();


	//LEFT
	S = scale(1000.0f, 1000.0f, 1000.0f);
	T = translate(-1000.0f, 0.0f, 0.0f);
	R = rotate(90.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 10000000;
	glUniform1fv(MaterialID, 1, &Material);
	Right->Bind();
	MySquare->Draw();


	//GROUND
	S = scale(1000.0f, 1000.0f, 1000.0f);
	T = translate(0.0f, -1000.0f, 0.0f);
	R = rotate(270.0f, vec3(1, 0, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 10000000;
	glUniform1fv(MaterialID, 1, &Material);
	Ground->Bind();
	MySquare->Draw();


	//RIGHT
	S = scale(1000.0f, 1000.0f, 1000.0f);
	T = translate(1000.0f, 0.0f, 0.0f);
	R = rotate(270.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 10000000;
	glUniform1fv(MaterialID, 1, &Material);
	Left->Bind();
	MySquare->Draw();

#pragma endregion

#pragma region House

	//FRONT
	//Right Wall Of door
	S = scale(6.0f, 4.5f, 6.0f);
	T = translate(9.0f, -1.5f, -25.0f);
	R = rotate(0.0f, vec3(0, 0, 1));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if(FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	HouseText->Bind();
	MySquare->Draw();

	S = scale(6.0f, 4.5f, 6.0f);
	T = translate(9.0f, -1.5f, -25.1f);
	R = rotate(180.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	HouseText->Bind();
	MySquare->Draw();


	//left wall of door
	S = scale(6.0f, 4.5f, 6.0f);
	T = translate(-9.0f, -1.5f, -25.0f);
	R = rotate(0.0f, vec3(0, 0, 1));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	HouseText->Bind();
	MySquare->Draw();

	S = scale(6.0f, 4.5f, 6.0f);
	T = translate(-9.0f, -1.5f, -25.1f);
	R = rotate(180.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	HouseText->Bind();
	MySquare->Draw();

	//up wall of door
	S = scale(15.0f, 1.5f, 6.0f);
	T = translate(0.0f, 4.5f, -25.0f);
	R = rotate(0.0f, vec3(0, 0, 1));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	HouseText->Bind();
	MySquare->Draw();

	S = scale(15.0f, 1.5f, 6.0f);
	T = translate(0.0f, 4.5f, -25.1f);
	R = rotate(180.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	HouseText->Bind();
	MySquare->Draw();


	//Door
	//closed state
	/*T = translate(0.0f, -1.5f, -11.5f);
	R = rotate(0.0f, vec3(0, 1, 0));*/

	//OpenState
	/*T = translate(-3.0f, -1.5f, -28.5f);
	R = rotate(90.0f, vec3(0, 1, 0));*/

	//state
	S = scale(3.5f, 4.5f, 6.0f);
	if (!OpenDoor)
	{
		T = translate(0.0f, -1.5f, -25.0f);
		R = rotate(0.0f, vec3(0, 1, 0));
	}
	else
	{
		T = translate(-3.0f, -1.5f, -22.0f);
		R = rotate(90.0f, vec3(0, 1, 0));
	}
	
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Door->Bind();
	MySquare->Draw();

	

	// state
	S = scale(3.5f, 4.5f, 6.0f);
	if (!OpenDoor)
	{
		T = translate(0.0f, -1.5f, -25.1f);
		R = rotate(0.0f, vec3(0, 1, 0));
	}
	else
	{
		T = translate(-3.0f, -1.5f, -22.0f);
		R = rotate(90.0f, vec3(0, 1, 0));
	}
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Door->Bind();
	MySquare->Draw();


	//UP
	S = scale(15.0f, 25.0f, 6.0f);
	T = translate(0.0f, 6.0f, 0.0f);
	R = rotate(90.0f, vec3(1, 0, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	HouseText->Bind();
	if (OnMainCam)
		MySquare->Draw();

	S = scale(15.0f, 25.0f, 6.0f);
	T = translate(0.0f, 6.1f, 0.0f);
	R = rotate(-90.0f, vec3(1, 0, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	HouseText->Bind();
	if (OnMainCam)
		MySquare->Draw();


	//BACK
	S = scale(15.0f, 6.0f, 6.0f);
	T = translate(0.0f, 0.0f, 25.0f);
	R = rotate(180.0f, vec3(1, 0, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	HouseText->Bind();
	MySquare->Draw();

	S = scale(15.0f, 6.0f, 6.0f);
	T = translate(0.0f, 0.0f, 25.1f);
	R = rotate(360.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	HouseText->Bind();
	MySquare->Draw();

	//LEFT
	S = scale(25.0f, 6.0f, 6.0f);
	T = translate(-15.0f, 0.0f, 0.0f);
	R = rotate(90.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	HouseText->Bind();
	MySquare->Draw();

	S = scale(25.0f, 6.0f, 6.0f);
	T = translate(-15.1f, 0.0f, 0.0f);
	R = rotate(-90.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	HouseText->Bind();
	MySquare->Draw();

	//GROUND
	S = scale(15.0f, 25.0f, 6.0f);
	T = translate(0.0f, -6.0f, 0.0f);
	R = rotate(270.f, vec3(1, 0, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Floor->Bind();
	MySquare->Draw();


	//RIGHT
	S = scale(25.0f, 6.0f, 9.5f);
	T = translate(15.0f, 0.0f, 0.0f);
	R = rotate(270.f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	HouseText->Bind();
	MySquare->Draw();

	S = scale(25.0f, 6.0f, 9.5f);
	T = translate(15.1f, 0.0f, 0.0f);
	R = rotate(-270.f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	HouseText->Bind();
	MySquare->Draw();

#pragma endregion

#pragma region Objects

	// Long Table
	//FRONT
	S = scale(2.0f, 0.5f, 2.0f);
	T = translate(0.0f, -3.0f, -5.0f);
	R = rotate(180.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();


	//UP
	S = scale(2.0f, 5.0f, 2.0f);
	T = translate(0.0f, -2.5f, 0.0f);
	R = rotate(-90.0f, vec3(1, 0, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();


	//BACK
	S = scale(2.0f, 0.5f, 2.0f);
	T = translate(0.0f, -3.0f, 5.0f);
	R = rotate(0.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();


	//LEFT
	S = scale(5.0f, 0.5f, 2.0f);
	T = translate(vec3(-2.0f, -3.0f, 0.0f));
	R = rotate(-90.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();


	//GROUND
	S = scale(2.0f, 5.0f, 2.0f);
	T = translate(0.0f, -3.5f, 0.0f);
	R = rotate(90.0f, vec3(1, 0, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();

	//RIGHT
	S = scale(5.0f, 0.5f, 2.0f);
	T = translate(2.0f, -3.0f, 0.0f);
	R = rotate(90.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();

	//////////////////////////////////////////////////////////////////////////

	
#pragma region Legs
	//Obeject Legs
	//1st Leg
	//FRONT
	S = scale(0.3f, 2.0f, 0.3f);
	T = translate(0.0f + 1.5f, -5.0f, -0.3f - 4.5f);
	R = rotate(180.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();


	////UP
	//S = scale(0.3f, 0.3f, 0.3f);
	//T = translate(0.0f, 5.5f, 0.0f);
	//R = rotate(90.0f, vec3(1, 0, 0));
	//MySquare->ModelMatrix = T * R * S;
	//glUniform1i(ColorID, 10);
	//glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	//glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	//glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	//glUniform3fv(LightPosID, 1, &LightPos[0]);
	//glUniform3fv(EyePosID, 1, &EyePos[0]);
	//Material = 2.5f;
	//glUniform1fv(MaterialID, 1, &Material);
	//Wood->Bind();
	//MySquare->Draw();


	//BACK
	S = scale(0.3f, 2.0f, 0.3f);
	T = translate(0.0f + 1.5f, -5.0f, 0.3f - 4.5f);
	R = rotate(0.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();


	//LEFT
	S = scale(0.3f, 2.0f, 0.3f);
	T = translate(-0.3f + 1.5f, -5.0f, 0.0f - 4.5f);
	R = rotate(-90.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();


	//GROUND
	/*S = scale(0.3f, 0.3f, 0.3f);
	T = translate(0.0f, -5.9f, 0.0f);
	R = rotate(270.0f, vec3(1, 0, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();*/

	//RIGHT
	S = scale(0.3f, 2.0f, 0.3f);
	T = translate(0.3f + 1.5f, -5.0f, 0.0f - 4.5f);
	R = rotate(90.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();

	//2nd Leg

	//FRONT
	S = scale(0.3f, 2.0f, 0.3f);
	T = translate(0.0f - 1.5f, -5.0f, -0.3f - 4.5f);
	R = rotate(180.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();


	//UP
	/*S = scale(0.3f, 0.3f, 0.3f);
	T = translate(0.0f, 5.5f, 0.0f);
	R = rotate(90.0f, vec3(1, 0, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();*/


	//BACK
	S = scale(0.3f, 2.0f, 0.3f);
	T = translate(0.0f - 1.5f, -5.0f, 0.3f - 4.5f);
	R = rotate(0.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();


	//LEFT
	S = scale(0.3f, 2.0f, 0.3f);
	T = translate(-0.3f - 1.5f, -5.0f, 0.0f - 4.5f);
	R = rotate(-90.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();


	//GROUND
	/*S = scale(0.3f, 0.3f, 0.3f);
	T = translate(0.0f, -5.9f, 0.0f);
	R = rotate(270.0f, vec3(1, 0, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();
*/
	//RIGHT
	S = scale(0.3f, 2.0f, 0.3f);
	T = translate(0.3f - 1.5f, -5.0f, 0.0f - 4.5f);
	R = rotate(90.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();

	//3rd Leg
	//FRONT
	S = scale(0.3f, 2.0f, 0.3f);
	T = translate(0.0f - 1.5f, -5.0f, -0.3f + 4.5f);
	R = rotate(180.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();


	//UP
	/*S = scale(0.3f, 0.3f, 0.3f);
	T = translate(0.0f, 5.5f, 0.0f);
	R = rotate(90.0f, vec3(1, 0, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();
*/

	//BACK
	S = scale(0.3f, 2.0f, 0.3f);
	T = translate(0.0f - 1.5f, -5.0f, 0.3f + 4.5f);
	R = rotate(0.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();


	//LEFT
	S = scale(0.3f, 2.0f, 0.3f);
	T = translate(-0.3f - 1.5f, -5.0f, 0.0f + 4.5f);
	R = rotate(-90.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();


	//GROUND
	//S = scale(0.3f, 0.3f, 0.3f);
	//T = translate(0.0f, -5.9f, 0.0f);
	//R = rotate(90.0f, vec3(1, 0, 0));
	//MySquare->ModelMatrix = T * R * S;
	//glUniform1i(ColorID, 10);
	//glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	//glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	//glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	//glUniform3fv(LightPosID, 1, &LightPos[0]);
	//glUniform3fv(EyePosID, 1, &EyePos[0]);
	//Material = 2.5f;
	//glUniform1fv(MaterialID, 1, &Material);
	//Wood->Bind();
	//MySquare->Draw();

	//RIGHT
	S = scale(0.3f, 2.0f, 0.3f);
	T = translate(0.3f - 1.5f, -5.0f, 0.0f + 4.5f);
	R = rotate(90.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();

	//4th Leg

	//FRONT
	S = scale(0.3f, 2.0f, 0.3f);
	T = translate(0.0f + 1.5f, -5.0f, -0.3f + 4.5f);
	R = rotate(180.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();


	//UP
	/*S = scale(0.3f, 0.3f, 0.3f);
	T = translate(0.0f, 5.5f, 0.0f);
	R = rotate(90.0f, vec3(1, 0, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();
*/

	//BACK
	S = scale(0.3f, 2.0f, 0.3f);
	T = translate(0.0f + 1.5f, -5.0f, 0.3f + 4.5f);
	R = rotate(0.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();


	//LEFT
	S = scale(0.3f, 2.0f, 0.3f);
	T = translate(-0.3f + 1.5f, -5.0f, 0.0f + 4.5f);
	R = rotate(-90.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();


	//GROUND
	/*S = scale(0.3f, 0.3f, 0.3f);
	T = translate(0.0f, -5.9f, 0.0f);
	R = rotate(270.0f, vec3(1, 0, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();
*/
	//RIGHT
	S = scale(0.3f, 2.0f, 0.3f);
	T = translate(0.3f + 1.5f, -5.0f, 0.0f + 4.5f);
	R = rotate(90.0f, vec3(0, 1, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	Wood->Bind();
	MySquare->Draw();

#pragma endregion


#pragma endregion

#pragma region outdoor Floor
	//outdoor floor
	S = scale(3.5f, 50.0f, 6.0f);
	T = translate(0.0f, -6.0f, -75.0f);
	R = rotate(270.f, vec3(1, 0, 0));
	MySquare->ModelMatrix = T * R * S;
	glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 2.5f;
	glUniform1fv(MaterialID, 1, &Material);
	outdoorGround->Bind();
	MySquare->Draw();
#pragma endregion

#pragma region Loaded Models
	ViewProjectionMatrix = MyCamera->GetProjectionMatrix() * MyCamera->GetViewMatrix();
	StaticShader.UseProgram();
	StaticShader.BindViewProjectionMatrix(&ViewProjectionMatrix[0][0]);
	mat4 R = rotate(160.0f, 0.0f, 1.0f, 0.0f);
	mat4 R2 = rotate(200.0f, 1.0f, 0.0f, 0.0f);
	mat4 T = translate(-15.0f, -3.0f, -45.0f);
	mat4 S = scale(3.0f, 3.0f, 3.0f);
	StaticModelMatrix = T * R * R2 * S;
	/*glUniform1i(ColorID, 10);
	if (FlashLightBool == 1)
		glUniform1i(FlashLightID, 1);
	else
		glUniform1i(FlashLightID, 0);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &MySquare->ModelMatrix[0][0]);
	glUniform3fv(AmbLightID, 1, &AmbLight[0]);
	glUniform3fv(DiffLightID, 1, &DiffLight[0]);
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(EyePosID, 1, &EyePos[0]);
	Material = 10000000;
	glUniform1fv(MaterialID, 1, &Material);*/
	if (OnMainCam)
	{
		StaticShader.BindModelMatrix(&StaticModelMatrix[0][0]);
		StaticModel.Render(&StaticShader, StaticModelMatrix); // Draw the Model
	}
	
#pragma endregion

	
	//////////////////////////////////////////////////////////////////////////
}

void Renderer::Update()
{
	double CurrentTime = glfwGetTime();
	double DeltaTime = CurrentTime - PreTime;
	PreTime = CurrentTime;

#pragma region simulating Collision

	Walk = true;

	//Close the door when the Player gets inside the room
	if (MyCamera->GetEyePosition().z >= -9.0f)
	{
		OpenDoor = false;
	}

	//handling not to get out of the road when out of the house
	MyCamera->checkOutOfHouse();

	//collision with door
	if (!OpenedOnce && MyCamera->GetEyePosition().z >= -32.0f)
		Walk = false;
	
	
	//collision with front wall while inside house
	if (!MyCamera->gethouseBool() && !OpenDoor && MyCamera->GetEyePosition().z <= -20.0f)
		Walk = false;

	//Collision with Back wall of house
	if (MyCamera->GetEyePosition().z >= 20.0f)
		Walk = false;

	//collision with side walls of house
	if (MyCamera->GetEyePosition().x >= 11.0f || MyCamera->GetEyePosition().x <= -11.0f)
		Walk = false;
#pragma endregion

}

void Renderer::HandleKeyboardInput(int Key, int Action)
{
	if (Action == GLFW_PRESS || Action == GLFW_REPEAT)
	{
		switch (Key)
		{
			//Moving forward
		case GLFW_KEY_UP:
		case GLFW_KEY_W:
			if (OnMainCam)
				if(Walk)
				MyCamera->Walk(0.5);
			break;

			//Moving backword
		case GLFW_KEY_DOWN:
		case GLFW_KEY_S:
			//if ()
			if (OnMainCam)
				MyCamera->Walk(-0.5);
			break;

			// Moving right
		case GLFW_KEY_RIGHT:
		case GLFW_KEY_D:
			if (OnMainCam)	
					MyCamera->Strafe(0.1);
			break;

			// Moving left
		case GLFW_KEY_LEFT:
		case GLFW_KEY_A:
			if (OnMainCam)
					MyCamera->Strafe(-0.1);
			break;

			// Moving up
	/*	case GLFW_KEY_SPACE:
		case GLFW_KEY_R:
			if (OnMainCam)
				MyCamera->Fly(0.1);
			break;*/

			// Moving down
		/*case GLFW_KEY_LEFT_CONTROL:
		case GLFW_KEY_F:
			if (OnMainCam)
				MyCamera->Fly(-0.1);
			break;*/

		case GLFW_KEY_I:
			if (OnMainCam)
				MyCamera->Pitch(1);
			break;

		case GLFW_KEY_K:
			if (OnMainCam)
				MyCamera->Pitch(-1);
			break;

		case GLFW_KEY_L:
			if (OnMainCam)
				MyCamera->Yaw(-1);
			break;

		case GLFW_KEY_J:
			if (OnMainCam)
				MyCamera->Yaw(1);
			break;

		case GLFW_KEY_O:
			if (OnMainCam)
				MyCamera->Roll(-1);
			break;

		case GLFW_KEY_U:
			if (OnMainCam)
				MyCamera->Roll(1);
			break;
		case GLFW_KEY_M:
			ChangeCam();
			FlashLightBool = 0;
			break;
		case GLFW_KEY_X:
			if (FlashLightBool == 1)
				FlashLightBool = 0;
			else
				FlashLightBool = 1;
			break;
		case GLFW_KEY_E:
			if (MyCamera->GetEyePosition().z >= -32.0f)
			{
				OpenDoor = true;
				OpenedOnce = true;
			}
				
			break;
		default:
			break;

			
		}

		LightPos = MyCamera->GetEyePosition();
		EyePos = MyCamera->GetLookDirection();
	}
}

void Renderer::HandleMouseMove(double DeltaX, double DeltaY)
{
	MyCamera->Yaw(DeltaX);
	MyCamera->Pitch(DeltaY);
	LightPos = MyCamera->GetEyePosition();
	EyePos = MyCamera->GetLookDirection();
}

void Renderer::HandleMouseClick(int MouseAction, int MouseButton, double MouseXPos, double MouseYPos)
{

}

void Renderer::HandleWindowResize(int WindowWidth, int WindowHeight)
{
	MyCamera->SetPerspectiveProjection(45.0f, (float)WindowWidth / (float)WindowHeight, 0.1f, 1000.0f);
}

//bool Renderer::IsColliding(vec3 Player, vec3 Collider)
//{
//	if (Player.x == Collider.x && Player.z == Collider.z)
//		return true;
//	else
//		return false;
//}

void Renderer::ChangeCam()
{
	OnMainCam = !OnMainCam;

	if (!OnMainCam) //Means was on main camera before hitting M
	{
		StoreMainCamPos = MyCamera->GetEyePosition();
		LightPos = SecondaryCam->GetEyePosition();
		EyePos = SecondaryCam->GetLookDirection();
	}
	else //Means Was on Secondary Camera Before Hitting M
	{
		MyCamera->SetLookAt(
			StoreMainCamPos,// Camera Position
			vec3(+0.0f, +0.0f, +0.0f),// Look at Point
			vec3(+0.0f, +1.0f, +0.0f) // Up Vector
		);
		LightPos = MyCamera->GetEyePosition();
		EyePos = MyCamera->GetLookDirection();
	}


}

void Renderer::Cleanup()
{
	glDeleteProgram(ProgramID);
	delete MyCamera;
}