//yf's version
#define STB_IMAGE_IMPLEMENTATION

//#include <stdlib.h>
#include "esUtil.h"
#include "stb_image.h"
float vertices[] = {
	//-位置-					-纹理坐标-		-法线-
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		0.0f, 0.0f, -1.0f,
	0.5f, 0.5f, -0.5f,		1.0f, 1.0f,		0.0f, 0.0f, -1.0f,
	0.5f, 0.5f, -0.5f,		1.0f, 1.0f,		0.0f, 0.0f, -1.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		0.0f, 0.0f, -1.0f,

	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,	
	0.5f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f,	
	0.5f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,

	-0.5f, 0.5f, 0.5f,		1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,

	0.5f, 0.5f, 0.5f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
};

struct LightProperties
{
	GLfloat direction[3];//GLfloat direction[3];
	GLfloat ambientColor[4];
	GLfloat diffuseColor[4];
	GLfloat specularColor[4];
};

struct MaterialProperties
{
	GLfloat ambientColor[4];
	GLfloat diffuseColor[4];
	GLfloat specularColor[4];
	GLfloat specularExponent;//GLfloat specularExponent;
};

struct LightProperties light = {
	{ 0.0f, -1.0f, -1.0f },			//direction{ 0.57735f, 0.57735f, 0.57735f }
	{ 0.4f, 0.4f, 0.4f, 1.0f },		//ambientColor;
	{ 1.0f, 1.0f, 1.0f, 1.0f },		//diffuseColor;
	{ 0.5f, 0.5f, 0.5f, 1.0f } };	//specularColor;
struct MaterialProperties material = {
	{ 0.8f, 0.8f, 0.8f, 1.0f },		 //ambientColor;
	{ 1.0f, 1.0f, 1.0f, 1.0f },		 //diffuseColor;
	{ 0.4f, 0.4f, 0.4f, 1.0f },		 //specularColor;
	1.0f };						 //specularExponent;

struct LightLocations
{
	GLint directionLocation;
	GLint ambientColorLocation;
	GLint diffuseColorLocation;
	GLint specularColorLocation;
};

struct MaterialLocations
{
	GLint ambientColorLocation;
	GLint diffuseColorLocation;
	GLint specularColorLocation;
	GLint specularExponentLocation;
};
struct LightLocations g_light;
struct MaterialLocations g_material;


typedef struct
{
	// Handle to a program object
	GLuint programObject;

	// Uniform locations
	GLint  mvpLoc;
	GLint  mvLoc;
	GLfloat	viewPos[3];
	GLuint viewPosLoc;

	GLfloat   angle;

	ESMatrix  mvpMatrix;
	ESMatrix  mvMatrix;

	GLuint myVBO;
	GLuint myVAO;

} UserData;

// Create a simple 2x2 texture image with four different colors
void LoadTexture()
{
	// Use tightly packed data
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Load the texture
	int width, height, nrChannels;
	unsigned char *data = stbi_load("./Models/brick_DIFF.bmp", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		//std::cout << "Failed to load texture" << std::endl;
		printf("Failed to load texture\n");
	}
	stbi_image_free(data);

	// Set the filtering mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

GLuint LoadTextureFile(const char* filename)
{
	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		printf("Failed to load texture\n");
	stbi_image_free(data);
	return texID;
}

// Initialize the shader and program object
//
int Init(ESContext *esContext)
{
	UserData *userData = (UserData *)esContext->userData;

	const char vShaderStr[] =
		"#version 300 es												\n"
		"uniform mat4 u_mvpMatrix;										\n"
		"uniform mat4 u_mvMatrix;										\n"
		"layout(location = 0) in vec4 a_position;						\n"
		"layout(location = 1) in vec2 a_texCoord;						\n"
		"layout(location = 2) in vec3 a_normal;							\n"
		"out vec2 v_texCoord;											\n"
		"out vec3 v_fragPos;											\n"
		"out vec3 v_normal;												\n"
		"void main()													\n"
		"{																\n"
		"	v_fragPos=vec3(u_mvMatrix * a_position);					\n"
		"	v_texCoord=a_texCoord;										\n"
		"	v_normal=mat3(transpose(inverse(u_mvMatrix)))*a_normal;											\n"
		"   gl_Position = u_mvpMatrix * a_position;						\n"
		"}																\n";

	const char fShaderStr[] =
		"#version 300 es												\n"
		"precision mediump float;										\n"
		"struct LightProperties											\n"
		"{																\n"
		"	vec3 direction;												\n"
		"	vec4 ambientColor;											\n"
		"	vec4 diffuseColor;											\n"
		"	vec4 specularColor;											\n"
		"};																\n"
		"struct MaterialProperties										\n"
		"{																\n"
		"	vec4 ambientColor;											\n"
		"	vec4 diffuseColor;											\n"
		"	vec4 specularColor;											\n"
		"	float specularExponent;										\n"
		"};																\n"
		"uniform LightProperties u_light;								\n"
		"uniform MaterialProperties u_material;							\n"
		"in vec3 v_fragPos;																			\n"
		"in vec2 v_texCoord;																		\n"
		"in vec3 v_normal;																			\n"
		"in vec3 c_viewPos;																			\n"
		"out vec4 outColor;																			\n"
		"uniform sampler2D s_texture;																\n"
		"void main()																				\n"
		"{																							\n"
		"	vec4 color = u_light.ambientColor * u_material.ambientColor;							\n"//ambient
		"	vec3 N = normalize(v_normal);															\n"
		"	vec3 L = -normalize(u_light.direction);													\n"
		"	float NDotL = max(dot(N,L), 0.0);														\n"
		"	color += u_light.diffuseColor * u_material.diffuseColor * NDotL;						\n"//diffuse,Lambert
		"	vec3 R = reflect(L,N);																	\n"
		"	vec3 V=normalize(c_viewPos-v_fragPos);													\n"
		"	float VDotR = max(dot(V, R), 0.0);														\n"
		"	float specularIntensity = pow(VDotR, u_material.specularExponent);						\n"
		"	color += u_light.specularColor * u_material.specularColor * specularIntensity;			\n"//specular,Phong
		"   outColor =texture(s_texture,v_texCoord)*color;											\n"
		"}																							\n";

	// Load the shaders and get a linked program object
	userData->programObject = esLoadProgram(vShaderStr, fShaderStr);

	GLuint diffuseMap = LoadTextureFile("./Models/brick_DIFF.bmp");
	GLuint normapMap = LoadTextureFile("./Models/brick_NRM.bmp");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);

	g_light.directionLocation = glGetUniformLocation(userData->programObject, "u_light.direction");
	g_light.ambientColorLocation = glGetUniformLocation(userData->programObject, "u_light.ambientColor");
	g_light.diffuseColorLocation = glGetUniformLocation(userData->programObject, "u_light.diffuseColor");
	g_light.specularColorLocation = glGetUniformLocation(userData->programObject, "u_light.specularColor");
	g_material.ambientColorLocation = glGetUniformLocation(userData->programObject, "u_material.ambientColor");
	g_material.diffuseColorLocation = glGetUniformLocation(userData->programObject, "u_material.diffuseColor");
	g_material.specularColorLocation = glGetUniformLocation(userData->programObject, "u_material.specularColor");
	g_material.specularExponentLocation = glGetUniformLocation(userData->programObject, "u_material.specularExponent");

	userData->viewPos[0] = 20.f;
	userData->viewPos[1] = 20.f;
	userData->viewPos[2] = 20.f;
	userData->viewPosLoc = glGetUniformLocation(userData->programObject, "c_viewPos");
	userData->mvpLoc = glGetUniformLocation(userData->programObject, "u_mvpMatrix");
	userData->mvLoc = glGetUniformLocation(userData->programObject, "u_mvMatrix");
	userData->angle = 0.0f;



	glGenVertexArrays(1, &userData->myVAO);
	glBindVertexArray(userData->myVAO);
	glGenBuffers(1, &userData->myVBO);
	glBindBuffer(GL_ARRAY_BUFFER, userData->myVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal coord attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//LoadTexture();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0xFFFF);
	return GL_TRUE;
}


///
// Update MVP matrix based on time
//
void Update(ESContext *esContext, float deltaTime)
{
	UserData *userData = (UserData *)esContext->userData;
	ESMatrix perspective;
	float    aspect;

	// Compute a rotation angle based on time to rotate the cube
	userData->angle += (deltaTime * 90.0f);

	if (userData->angle >= 360.0f)
		userData->angle -= 360.0f;

	// Generate a model view matrix to rotate/translate the cube
	esMatrixLoadIdentity(&userData->mvMatrix);

	// Translate away from the viewer
	esTranslate(&userData->mvMatrix, 0.0, 0.0, -2.0);
	// Rotate the cube这里是转动物体
	esRotate(&userData->mvMatrix, userData->angle, 1.0, 0.0, 1.0);

	// Compute the window aspect ratio
	aspect = (GLfloat)esContext->width / (GLfloat)esContext->height;
	// Generate a perspective matrix with a 60 degree FOV
	esMatrixLoadIdentity(&perspective);
	esPerspective(&perspective, 60.0f, aspect, 1.0f, 20.0f);

	// Compute the final MVP by multiplying the
	// modevleiw and perspective matrices together
	esMatrixMultiply(&userData->mvpMatrix, &userData->mvMatrix, &perspective);
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw(ESContext *esContext)
{
	UserData *userData = (UserData *)esContext->userData;

	// Set the viewport
	glViewport(0, 0, esContext->width, esContext->height);

	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Use the program object
	glUseProgram(userData->programObject);

	// Load the MVP matrix
	// mvpMatrix是在Update里用modelview和perspective算出来的
	glUniformMatrix4fv(userData->mvpLoc, 1, GL_FALSE, (GLfloat *)&userData->mvpMatrix);
	glUniformMatrix4fv(userData->mvLoc, 1, GL_FALSE, (GLfloat*)&userData->mvMatrix);

	glUniform3fv(userData->viewPosLoc, 1, userData->viewPos);

	//传输光照参数
	glUniform3fv(g_light.directionLocation, 1, light.direction);
	glUniform4fv(g_light.ambientColorLocation, 1, light.ambientColor);
	glUniform4fv(g_light.diffuseColorLocation, 1, light.diffuseColor);
	glUniform4fv(g_light.specularColorLocation, 1, light.specularColor);
	glUniform4fv(g_material.ambientColorLocation, 1, material.ambientColor);
	glUniform4fv(g_material.diffuseColorLocation, 1, material.diffuseColor);
	glUniform4fv(g_material.specularColorLocation, 1, material.specularColor);
	glUniform1f(g_material.specularExponentLocation, material.specularExponent);
	//***********************

	glBindVertexArray(userData->myVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

	glCullFace(GL_FRONT);
}

///
// Cleanup
//
void Shutdown(ESContext *esContext)
{
	UserData *userData = (UserData *)esContext->userData;

	glDeleteVertexArrays(1, &userData->myVAO);
	glDeleteBuffers(1, &userData->myVBO);
	glDeleteProgram(userData->programObject);
}


int esMain(ESContext *esContext)
{
	esContext->userData = malloc(sizeof(UserData));

	esCreateWindow(esContext, "Simple_VertexShader", 400, 400, ES_WINDOW_RGB | ES_WINDOW_DEPTH);
	
	if (!Init(esContext))
	{
		return GL_FALSE;
	}

	esRegisterShutdownFunc(esContext, Shutdown);
	esRegisterUpdateFunc(esContext, Update);
	esRegisterDrawFunc(esContext, Draw);

	return GL_TRUE;
}