#include <stdlib.h>
#include <math.h>
#include "esUtil.h"

#define POSITION_LOC    0
#define COLOR_LOC       1

const char vShadowMapShaderStr[] =
"#version 300 es                                  \n"
"uniform mat4 u_mvpLightMatrix;                   \n"
"layout(location = 0) in vec4 a_position;         \n"
"out vec4 v_color;                                \n"
"void main()                                      \n"
"{                                                \n"
"   gl_Position = u_mvpLightMatrix * a_position;  \n"
"}                                                \n";

const char fShadowMapShaderStr[] =
"#version 300 es                                  \n"
"precision lowp float;                            \n"
"void main()                                      \n"
"{                                                \n"
"}                                                \n";

const char vSceneShaderStr[] =
"#version 300 es                                   \n"
"uniform mat4 u_mvpMatrix;                         \n"
"uniform mat4 u_mvpLightMatrix;                    \n"
"layout(location = 0) in vec4 a_position;          \n"
"layout(location = 1) in vec4 a_color;             \n"
"out vec4 v_color;                                 \n"
"out vec4 v_shadowCoord;                           \n"
"void main()                                       \n"
"{                                                 \n"
"   v_color = a_color;                             \n"
"   gl_Position = u_mvpMatrix * a_position;        \n"
"   v_shadowCoord = u_mvpLightMatrix * a_position; \n"
"                                                  \n"
"   // transform from [-1,1] to [0,1];             \n"
"   v_shadowCoord = v_shadowCoord * 0.5 + 0.5;     \n"
"}                                                 \n";

const char fSceneShaderStr[] =
"#version 300 es                                                \n"
"precision lowp float;                                          \n"
"uniform lowp sampler2DShadow s_shadowMap;                      \n"
"in vec4 v_color;                                               \n"
"in vec4 v_shadowCoord;                                         \n"
"layout(location = 0) out vec4 outColor;                        \n"
"                                                               \n"
"float lookup ( float x, float y )                              \n"
"{                                                              \n"
"   float pixelSize = 0.002; // 1/500                           \n"
"   vec4 offset = vec4 ( x * pixelSize * v_shadowCoord.w,       \n"
"                        y * pixelSize * v_shadowCoord.w,       \n"
"                        -0.005 * v_shadowCoord.w, 0.0 );       \n"
"   return textureProj ( s_shadowMap, v_shadowCoord + offset ); \n"
"}                                                              \n"
"                                                               \n"
"void main()                                                    \n"
"{                                                              \n"
"   // 3x3 kernel with 4 taps per sample, effectively 6x6 PCF   \n"
"   float sum = 0.0;                                            \n"
"   float x, y;                                                 \n"
"   for ( x = -2.0; x <= 2.0; x += 2.0 )                        \n"
"      for ( y = -2.0; y <= 2.0; y += 2.0 )                     \n"
"         sum += lookup ( x, y );                               \n"
"                                                               \n"
"   // divide sum by 9.0                                        \n"
"   sum = sum * 0.11;                                           \n"
"   outColor = v_color * sum;                                   \n"
"}                                                              \n";

GLfloat cubeVertices[] = {
	// Positions          // Texture Coords
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};
GLfloat floorVertices[] = {
	// Positions          // Texture Coords (note we set these higher than 1 that together with GL_REPEAT as texture wrapping mode will cause the floor texture to repeat)
	5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
	-5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
	-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

	5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
	5.0f, -0.5f, -5.0f, 2.0f, 2.0f
};

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

typedef struct
{
	// Handle to a program object
	GLuint sceneProgramObject;
	GLuint shadowMapProgramObject;

	GLuint groundGridSize;
	GLuint groundNumIndices;

	GLfloat eyePosition[3];
	GLfloat lightPosition[3];

	GLuint shadowMapTextureWidth;
	GLuint shadowMapTextureHeight;

	GLuint shadowMapTextureId;
	GLuint shadowMapBufferId;

	GLuint groundMVPfromLight;
	GLuint groundMVPfromEye;
	GLuint cubeMVPfromLight;
	GLuint cubeMVPfromEye;

	GLuint cubeVAO, cubeVBO;
	GLuint groundVAO, groundVBO;

	GLuint texIDcube, texIDground;
} UserData;

int InitMVP ( ESContext *esContext )
{
}

int InitShadowMap ( ESContext *esContext )
{
	
}

int Init ( ESContext *esContext )
{
	UserData *userData = esContext->userData;

	// Load the shaders and get a linked program object
	userData->shadowMapProgramObject = esLoadProgram(vShadowMapShaderStr, fShadowMapShaderStr);
	userData->sceneProgramObject = esLoadProgram(vSceneShaderStr, fSceneShaderStr);

	//cube
	glGenVertexArrays(1, &userData->cubeVAO);//初始化cube的vao
	glGenBuffers(1, &userData->cubeVBO);//初始化装载cube属性的vbo
	glBindVertexArray(userData->cubeVAO);//以下操作对cubeVAO负责
	glBindBuffer(GL_ARRAY_BUFFER, userData->cubeVBO);//以下操作对cubeVBO负责
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);//复制数据到当前vbo 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)0);//设置顶点位置
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);
	userData->texIDcube = LoadTextureFile("./Models/container.jpg");
	glBindVertexArray(0);//完成cubeVAO的设置
	//ground
	glGenVertexArrays(1, &userData->groundVAO);//初始化地板vao
	glGenBuffers(1, &userData->groundVBO);
	glBindVertexArray(userData->groundVAO);
	glBindBuffer(GL_ARRAY_BUFFER, userData->groundVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)(sizeof(GLfloat) * 3));
	userData->texIDground = LoadTextureFile("./Models/brick_DIFF.bmp");
	glBindVertexArray(0);//完成groundVAO的设置

	userData->eyePosition[0] = -5.0f;
	userData->eyePosition[1] = 3.0f;
	userData->eyePosition[2] = 5.0f;
	userData->lightPosition[0] = 10.f;
	userData->lightPosition[1] = 5.0f;
	userData->lightPosition[2] = 2.0f;

	// Init shadowmap
	GLenum none = GL_NONE;
	userData->shadowMapTextureWidth = 1024;
	userData->shadowMapTextureHeight = 1024;
	glGenTextures(1, &userData->shadowMapTextureId);
	glBindTexture(GL_TEXTURE_2D, userData->shadowMapTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//比较方法：https://www.cnblogs.com/aokman/archive/2013/12/26/3492294.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, userData->shadowMapTextureWidth, userData->shadowMapTextureHeight,
		0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	//set fbo
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, 0);
	glGenFramebuffers(1, &userData->shadowMapBufferId);
	glDrawBuffer(1, GL_NONE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, userData->shadowMapTextureId, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, userData->shadowMapBufferId);
	if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
		return FALSE;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//fbo set ended & shadowmap initiated

	glClearColor(1.f, 1.f, 1.f, 0.f);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	return TRUE;
}

///
// Draw the model
//
void DrawScene ( ESContext *esContext, 
                 GLint mvpLoc, 
                 GLint mvpLightLoc )
{

}

void Update(ESContext *esContext){
	UserData *userData = esContext->userData;
	ESMatrix perspective;
	ESMatrix ortho;
	ESMatrix modelview;
	ESMatrix model;
	ESMatrix view;

	GLfloat aspect = (GLfloat)esContext->width / (GLfloat)esContext->height;
	esMatrixLoadIdentity(&perspective);//眼睛视角的透视矩阵
	esPerspective(&perspective, 45.f, aspect, 0.1f, 100.f);
	esMatrixLoadIdentity(&ortho);//光源视角的正交矩阵
	esOrtho(&ortho, -10.f, 10.f, -10.f, 10.f, -30.f, 30.f);

	//ground-地面矩阵更新
	//模型矩阵
	esMatrixLoadIdentity(&model);
	esTranslate(&model, -2.f, -2.f, 0.f);
	esScale(&model, 1.f, 1.f, 1.f);//暂不缩放
	esRotate(&model, 90.f, 1.f, 0.f, 0.f);

	//相机矩阵-眼睛视角
	esMatrixLookAt(&view, userData->eyePosition[0], userData->eyePosition[1], userData->eyePosition[2],
		0.f, 0.f, 0.f,
		0.f, 1.f, 0.f);
	//模型视图矩阵-眼睛视角
	esMatrixMultiply(&modelview, &model, &view);
	//ground的MVP矩阵-眼睛视角(透视)
	esMatrixMultiply(&userData->groundMVPfromEye, &modelview, &perspective);

	//相机矩阵-光源视角
	esMatrixLookAt(&view, userData->lightPosition[0], userData->lightPosition[1], userData->lightPosition[2],
		0.f, 0.f, 0.f,
		0.f, 1.f, 0.f);
	//模型视图矩阵-光源视角
	esMatrixMultiply(&modelview, &model, &view);
	//ground的MVP矩阵-光源视角（正交）
	esMatrixMultiply(&userData->groundMVPfromLight, &modelview, &ortho);

	//////////////////////////////////////////////////////////////////////////
	//cube-立方体矩阵更新
	//模型矩阵
	esMatrixLoadIdentity(&model);
	esTranslate(&model, 5.f, -0.4f, -3.0f);
	esScale(&model, 1.f, 2.5f, 1.f);//暂不缩放
	esRotate(&model, -15.f, 0.f, 1.f, 0.f);

	//相机矩阵-眼睛视角
	esMatrixLookAt(&view, userData->eyePosition[0], userData->eyePosition[1], userData->eyePosition[2],
		0.f, 0.f, 0.f,
		0.f, 1.f, 0.f);
	//模型视图矩阵-眼睛视角
	esMatrixMultiply(&modelview, &model, &view);
	//ground的MVP矩阵-眼睛视角(透视)
	esMatrixMultiply(&userData->cubeMVPfromEye, &modelview, &perspective);

	//相机矩阵-光源视角
	esMatrixLookAt(&view, userData->lightPosition[0], userData->lightPosition[1], userData->lightPosition[2],
		0.f, 0.f, 0.f,
		0.f, 1.f, 0.f);
	//模型视图矩阵-光源视角
	esMatrixMultiply(&modelview, &model, &view);
	//ground的MVP矩阵-光源视角（正交）
	esMatrixMultiply(&userData->cubeMVPfromLight, &modelview, &ortho);

	return TRUE;
}

void Draw ( ESContext *esContext )
{
	UserData *userData = esContext->userData;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, 0);

	//第一步，从光源位置渲阴影映射图
	glBindFramebuffer(GL_FRAMEBUFFER, userData->shadowMapBufferId);
	glViewport(0, 0, userData->shadowMapTextureWidth, userData->shadowMapTextureHeight);
	glClear(GL_DEPTH_BUFFER_BIT);
	//关闭颜色渲染，仅写入深度缓存
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	//偏移消除伪像
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(5.f, 100.f);

	glUseProgram(userData->shadowMapProgramObject);
	//ground
	glBindVertexArray(userData->groundVAO);
	glBindTexture(GL_TEXTURE_2D, userData->texIDground);
	glUniformMatrix4fv(glGetUniformLocation(userData->shadowMapProgramObject, "u_mvpMatrix"), 1, GL_FALSE, (GLfloat*)&userData->groundMVPfromEye.m[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(userData->shadowMapProgramObject, "u_mvpLightMatrix"), 1, GL_FALSE, (GLfloat*)&userData->groundMVPfromLight.m[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	//cube
	glBindVertexArray(userData->cubeVAO);
	glBindTexture(GL_TEXTURE_2D, userData->texIDcube);
	glUniformMatrix4fv(glGetUniformLocation(userData->shadowMapProgramObject, "u_mvpMatrix"), 1, GL_FALSE, (GLfloat*)&userData->cubeMVPfromEye.m[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(userData->shadowMapProgramObject, "u_mvpLightMatrix"), 1, GL_FALSE, (GLfloat*)&userData->cubeMVPfromLight.m[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDisable(GL_POLYGON_OFFSET_FILL);
	
	//第二步，在眼睛视角渲染场景，并且使用上第一步渲染出的阴影贴图

}

///
// Cleanup
//
void Shutdown ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   // Delete shadow map
   glBindFramebuffer ( GL_FRAMEBUFFER, userData->shadowMapBufferId );
   glFramebufferTexture2D ( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0 );
   glBindFramebuffer ( GL_FRAMEBUFFER, 0 );
   glDeleteFramebuffers ( 1, &userData->shadowMapBufferId );
   glDeleteTextures ( 1, &userData->shadowMapTextureId );

   // Delete program object
   glDeleteProgram ( userData->sceneProgramObject );
   glDeleteProgram ( userData->shadowMapProgramObject );
}

int esMain ( ESContext *esContext )
{
   esContext->userData = malloc ( sizeof( UserData ) );

   esCreateWindow ( esContext, "Shadow Rendering", 500, 500, ES_WINDOW_RGB | ES_WINDOW_DEPTH );
   
   if ( !Init ( esContext ) )
   {
      return GL_FALSE;
   }

   esRegisterShutdownFunc ( esContext, Shutdown );
   esRegisterUpdateFunc(esContext, Update);
   esRegisterDrawFunc ( esContext, Draw );
   
   return GL_TRUE;
}

