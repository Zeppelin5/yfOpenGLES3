//yf's version
#define STB_IMAGE_IMPLEMENTATION
#include <stdlib.h>
#include "esUtil.h"
#include "stb_image.h"

float vertices[] = {
	//-位置-					-纹理坐标-		-法线-
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
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

// This is a white light.
struct LightProperties light = {
	{ 1.0f, 1.0f, 1.0f },
	{ 0.3f, 0.3f, 0.3f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f } };
// Blue color material with white specular color.
struct MaterialProperties material = {
	{ 0.0f, 0.0f, 1.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	20.0f };

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
static struct LightLocations g_light;
static struct MaterialLocations g_material;


typedef struct
{
	// Handle to a program object
	GLuint programObject;

	// Uniform locations
	GLint  mvpLoc;
	GLint  mvLoc;

	GLint testLoc;

	// Rotation angle
	GLfloat   angle;

	// MVP matrix
	ESMatrix  mvpMatrix;
	ESMatrix  mvMatrix;

	GLuint mVboIds[3];
	GLuint mVaoId;

	GLuint myVBO;
	GLuint myVAO;
	GLuint myEBO;

	GLuint textureID;
	GLuint samplerLoc;

} UserData;

const static GLuint VERTEX_POS_INDX = 0;//VERTEX_POS_INDX对应shader里location的位置
const static GLuint VERTEX_COLOR_INDX = 1;
const static GLuint VERTEX_TEXTURE_INDX = 2;
const static GLint VERTEX_POS_SIZE = 3;
const static GLint VERTEX_COLOR_SIZE = 4;
const static GLint VERTEX_TEXTURE_SIZE = 2;
const static GLsizei INDEX_NUMBER = 36;//索引的元素数量

// Create a simple 2x2 texture image with four different colors
void LoadTexture()
{
	// Use tightly packed data
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Load the texture
	int width, height, nrChannels;
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
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

GLfloat testvec[4] = { 1, 1, 1, 1 };

// Initialize the shader and program object
//
int Init(ESContext *esContext)
{
	UserData *userData = (UserData* )esContext->userData;

	const char vShaderStr[] =
		"#version 300 es                             \n"
		"uniform mat4 u_mvpMatrix;                   \n"
		"uniform mat4 u_mvMatrix;					 \n"
		"uniform vec4 testUniform;					 \n"
		"uniform int testInt;						\n"
		"layout(location = 0) in vec4 a_position;    \n"
		"layout(location = 1) in vec2 a_texCoord;    \n"
		"layout(location = 2) in vec3 a_normal;		 \n"
		"out vec2 v_texCoord;						 \n"
		"out vec3 v_eye;							 \n"
		"out vec3 v_normal;							 \n"
		"void main()                                 \n"
		"{                                           \n"
		"   vec4 vertex=u_mvMatrix*a_position;		 \n"
		"	v_eye=-vec3(vertex);					 \n"
		"	v_texCoord=a_texCoord;					 \n"
		"	v_normal=a_normal;						 \n"
		"   gl_Position = u_mvpMatrix * a_position;  \n"
		"}                                           \n";

	const char fShaderStr[] =
		"#version 300 es											\n"
		"precision mediump float;									\n"

		"struct LightProperties										 \n"
		"{															 \n"
		"	vec3 direction;											 \n"
		"	vec4 ambientColor;										 \n"
		"	vec4 diffuseColor;										 \n"
		"	vec4 specularColor;										 \n"
		"};															 \n"
		"															 \n"
		"struct MaterialProperties									 \n"
		"{															 \n"
		"	vec4 ambientColor;										 \n"
		"	vec4 diffuseColor;										 \n"
		"	vec4 specularColor;										 \n"
		"	float specularExponent;									 \n"
		"};												\n"
		"															 \n"
		"uniform LightProperties u_light;						 \n"
		"uniform MaterialProperties u_material;					 \n"

		"in vec3 v_eye;															\n"
		"in vec2 v_texCoord;													\n"
		"in vec3 v_normal;														\n"
		"out vec4 outColor;														\n"
		"uniform sampler2D s_texture;											\n"
		"void main()															\n"
		"{																		\n"
		"	vec4 color = u_light.ambientColor * u_material.ambientColor;		\n"
		"	vec3 normal =normalize(v_normal);									\n"
		"	float nDotL = max(dot(u_light.direction, normal), 0.0);				\n"
		"	if (nDotL > 0.0)																		\n"
		"	{																						\n"
		"		vec3 eye = normalize(v_eye);														\n"
		"																							\n"
		"		// Incident vector is opposite light direction vector.								\n"
		"		vec3 reflection = reflect(-u_light.direction, normal);								\n"
		"																							\n"
		"		float eDotR = max(dot(eye, reflection), 0.0);										\n"
		"																							\n"
		"		color += u_light.diffuseColor * u_material.diffuseColor * nDotL;					\n"
		"																							\n"
		"		float specularIntensity = 0.0;														\n"
		"																							\n"
		"		if (eDotR > 0.0)																	\n"
		"		{																					\n"
		"			specularIntensity = pow(eDotR, u_material.specularExponent);					\n"
		"		}																					\n"
		"																							\n"
		"		color += u_light.specularColor * u_material.specularColor * specularIntensity;		\n"
		"	}																						\n"
		"   outColor =texture(s_texture,v_texCoord)+color;											\n"
		"}																							\n";

	// Load the shaders and get a linked program object
	userData->programObject = esLoadProgram(vShaderStr, fShaderStr);

	g_light.directionLocation = glGetUniformLocation(userData->programObject, "u_light.direction");
	g_light.ambientColorLocation = glGetUniformLocation(userData->programObject, "u_light.ambientColor");
	g_light.diffuseColorLocation = glGetUniformLocation(userData->programObject, "u_light.diffuseColor");
	g_light.specularColorLocation = glGetUniformLocation(userData->programObject, "u_light.specularColor");
	g_material.ambientColorLocation = glGetUniformLocation(userData->programObject, "u_material.ambientColor");
	g_material.diffuseColorLocation = glGetUniformLocation(userData->programObject, "u_material.diffuseColor");
	g_material.specularColorLocation = glGetUniformLocation(userData->programObject, "u_material.specularColor");
	g_material.specularExponentLocation = glGetUniformLocation(userData->programObject, "u_material.specularExponent");
	//The following error occurred in glUniform4fv: GL_INVALID_OPERATION

	userData->testLoc = glGetUniformLocation(userData->programObject, "testInt");

	//64页，尝试用block来传输试试

	// Get the uniform locations
	userData->mvpLoc = glGetUniformLocation(userData->programObject, "u_mvpMatrix");
	userData->mvLoc = glGetUniformLocation(userData->programObject, "u_mvMatrix");
	//// mvpMatrix在Update里用modelview和perspective算出来,
	//// 然后再Draw里用glUniformMatrix4fv(userData->mvpLoc, 1, GL_FALSE, (GLfloat *)&userData->mvpMatrix)传给mvpLoc

	// Generate the vertex data
	//userData->numIndices = esGenCube(1.0, &userData->vertices,
	//	NULL, NULL, &userData->indices);

	glGenVertexArrays(1, &userData->myVAO);
	glGenBuffers(1, &userData->myVBO);

	glBindVertexArray(userData->myVAO);

	glBindBuffer(GL_ARRAY_BUFFER, userData->myVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Starting rotation angle for the cube
	userData->angle = 0.0f;

	userData->samplerLoc = glGetUniformLocation(userData->programObject, "s_texture");
	LoadTexture();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	return GL_TRUE;
}


///
// Update MVP matrix based on time
//
void Update(ESContext *esContext, float deltaTime)
{
	UserData *userData = (UserData*)esContext->userData;
	ESMatrix perspective;
	ESMatrix modelview;
	float    aspect;

	// Compute a rotation angle based on time to rotate the cube
	userData->angle += (deltaTime * 70.0f);

	if (userData->angle >= 360.0f)
	{
		userData->angle -= 360.0f;
	}

	// Compute the window aspect ratio
	aspect = (GLfloat)esContext->width / (GLfloat)esContext->height;

	// Generate a perspective matrix with a 60 degree FOV
	esMatrixLoadIdentity(&perspective);
	esPerspective(&perspective, 60.0f, aspect, 1.0f, 20.0f);

	// Generate a model view matrix to rotate/translate the cube
	esMatrixLoadIdentity(&userData->mvMatrix);

	// Translate away from the viewer
	esTranslate(&userData->mvMatrix, 0.0, 0.0, -2.0);

	// Rotate the cube这里是转动物体
	esRotate(&userData->mvMatrix, userData->angle, 1.0, 0.0, 1.0);

	// Compute the final MVP by multiplying the
	// modevleiw and perspective matrices together
	esMatrixMultiply(&userData->mvpMatrix, &userData->mvMatrix, &perspective);
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw(ESContext *esContext)
{
	UserData *userData = (UserData*)esContext->userData;

	// Set the viewport
	glViewport(0, 0, esContext->width, esContext->height);

	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the program object
	glUseProgram(userData->programObject);

	// Load the MVP matrix
	// mvpMatrix是在Update里用modelview和perspective算出来的
	glUniformMatrix4fv(userData->mvpLoc, 1, GL_FALSE, (GLfloat *)&userData->mvpMatrix);
	glUniformMatrix4fv(userData->mvLoc, 1, GL_FALSE, (GLfloat*)&userData->mvMatrix);

	//传输光照参数
	glUniform4fv(userData->testLoc, 1, testvec);
	glUniform4fv(g_light.ambientColorLocation, 1, light.direction);
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
	UserData *userData = (UserData*)esContext->userData;

	glDeleteVertexArrays(1, &userData->myVAO);
	glDeleteBuffers(1, &userData->myVBO);
	glDeleteBuffers(1, &userData->myEBO);
	glDeleteProgram(userData->programObject);
}


int esMain(ESContext *esContext)
{
	esContext->userData = malloc(sizeof(UserData));

	esCreateWindow(esContext, "Simple_VertexShader", 320, 240, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if (!Init(esContext))
	{
		return GL_FALSE;
	}

	esRegisterShutdownFunc(esContext, Shutdown);
	esRegisterUpdateFunc(esContext, Update);
	esRegisterDrawFunc(esContext, Draw);

	return GL_TRUE;
}

