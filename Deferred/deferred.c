//yf's version
#define STB_IMAGE_IMPLEMENTATION
#include <stdlib.h>
#include "esUtil.h"
#include "stb_image.h"

typedef struct
{
	// Handle to a program object
	GLuint programObject;
	GLuint programObjectQuad;

	GLuint cubeVAO, cubeVBO;
	GLuint floorVAO, floorVBO;
	GLuint quadVAO, quadVBO;
	GLuint framebuffer;

	ESMatrix mvMatrix;
	ESMatrix mvpMatrix;

	GLuint mvLoc;
	GLuint mvpLoc;

	GLuint cubeTexture;
	GLuint floorTexture;

	// Rotation angle
	GLfloat angle;

	GLuint texIDfloor;
	GLuint texIDcube;

	GLuint texColorbuffer;
	GLuint texColorArray[4];

	GLuint textureWidth;
	GLuint textureHeight;
} UserData;

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
//渲染到这个四边形
GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// Positions   // TexCoords
	-1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f,

	-1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};

GLuint screenWidth = 600;
GLuint screenHeight = 400;

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

char vShaderStr[] =
"#version 300 es										 \n"
"layout(location = 0) in vec4 a_position;				 \n"
"layout(location = 1) in vec2 a_texCoord;				 \n"
"uniform mat4 u_mvpMatrix;								 \n"
"out vec2 v_texCoord;									 \n"
"void main()											 \n"
"{														 \n"
"    gl_Position = u_mvpMatrix * a_position;			 \n"
"	 v_texCoord=a_texCoord;								 \n"
"}														 \n";

char fShaderStr[] =
"#version 300 es										 \n"
"precision mediump float;								 \n"
"in vec2 v_texCoord;									 \n"
"uniform sampler2D screenTexture;						 \n"
"out vec4 outColor;										 \n"
"void main()											 \n"
"{														 \n"
"	outColor = texture(screenTexture,v_texCoord);		 \n"
"}														 \n";

//填充quad面片的VS
char vScreenShaderStr[] =
"#version 300 es													\n"
"layout(location = 0) in vec2 a_position;							\n"
"layout(location = 1) in vec2 texCoord;								\n"
"out vec2 v_texCoord;												\n"
"void main()														\n"
"{																	\n"
"   gl_Position = vec4(a_position.x,a_position.y,0.0,1.0);          \n"
"   v_texCoord=texCoord;											\n"
"}																    \n";

////正常：outColor=texture(Texture,v_texCoord);
char fScreenShaderStr[] =												//正常Shader
"#version 300 es													\n"
"precision mediump float;											\n"
"out vec4 outColor;													\n"
"in vec2 v_texCoord;												\n"
"uniform sampler2D Texture;											\n"
"void main()														\n"
"{																	\n"
//"	outColor=texture(Texture,v_texCoord);							\n"//正常色
"	outColor=vec4(vec3(1.0-texture(Texture,v_texCoord)),1.0);		\n"//反色
"}																	\n";

char fScreenShaderStrKernel[] =//核效果着色器
"#version 300 es													\n"
"precision mediump float;											\n"
"const float offset=1.0/300.0;										\n"
"layout(location = 0) out vec4 outColor0;							\n"
"layout(location = 1) out vec4 outColor1;							\n"
"layout(location = 2) out vec4 outColor2;							\n"
"layout(location = 3) out vec4 outColor3;							\n"
"in vec2 v_texCoord;												\n"
"uniform sampler2D Texture;											\n"
"void main()														\n"
"{																	\n"
"	vec2 offsets[9] = vec2[](																\n"
"	vec2(-offset, offset), 																	\n"
"	vec2(0.0f, offset), 																	\n"
"	vec2(offset, offset), 																	\n"
"	vec2(-offset, 0.0f),   																	\n"
"	vec2(0.0f, 0.0f),   																	\n"
"	vec2(offset, 0.0f),   																	\n"
"	vec2(-offset, -offset), 																\n"
"	vec2(0.0f, -offset),																	\n"
"	vec2(offset, -offset) 																	\n"
"	);																						\n"

"	float kernel[9] = float[](																\n"//模糊核0
"		1.0/16.0, 2.0/16.0, 1.0/16.0,														\n"
"		2.0/16.0, 4.0/16.0, 2.0/16.0,														\n"
"		1.0/16.0, 2.0/16.0, 1.0/16.0														\n"
"	);																						\n"
"	vec3 sampleTex[9];																		\n"
"	for (int i = 0; i < 9; i++)																\n"
"	{																						\n"
"		sampleTex[i] = vec3(texture(Texture, v_texCoord.st + offsets[i]));					\n"
"	}																						\n"
"	vec3 col = vec3(0.0);																	\n"
"	for (int i = 0; i < 9; i++)																\n"
"		col += sampleTex[i] * kernel[i];													\n"
"	outColor0 = vec4(col, 1.0);																\n"

"	kernel = float[](																		\n"//锐化核1
"		-1, -1, -1,																			\n"
"		-1, 9, -1,																			\n"
"		-1, -1, -1																			\n"
"		);																					\n"
"	for (int i = 0; i < 9; i++)																\n"
"	{																						\n"
"		sampleTex[i] = vec3(texture(Texture, v_texCoord.st + offsets[i]));					\n"
"	}																						\n"
"	col = vec3(0.0);																		\n"
"	for (int i = 0; i < 9; i++)																\n"
"		col += sampleTex[i] * kernel[i];													\n"
"	outColor1 = vec4(col, 1.0);																\n"
"	outColor2 = texture(Texture,v_texCoord);												\n"//正常色2
"	outColor3 = vec4(vec3(1.0-texture(Texture,v_texCoord)),1.0);							\n"//反色3
"}																							\n";

int Init(ESContext *esContext)
{
	UserData *userData = esContext->userData;
	userData->textureWidth = 600;
	userData->textureHeight = 400;
	// Load the shaders and get a linked program object
	userData->programObject = esLoadProgram(vShaderStr, fShaderStr);//载入文件、附件、编译、判错、删除Shader
	userData->programObjectQuad = esLoadProgram(vScreenShaderStr, fScreenShaderStrKernel);

	userData->angle = 0.0f;
	userData->mvLoc = glGetUniformLocation(userData->programObject, "u_mvMatrix");
	userData->mvpLoc = glGetUniformLocation(userData->programObject, "u_mvpMatrix");
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
	userData->texIDcube = LoadTextureFile("container.jpg");
	glBindVertexArray(0);//完成cubeVAO的设置
	//floor
	glGenVertexArrays(1, &userData->floorVAO);//初始化地板vao
	glGenBuffers(1, &userData->floorVBO);
	glBindVertexArray(userData->floorVAO);
	glBindBuffer(GL_ARRAY_BUFFER, userData->floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)(sizeof(GLfloat) * 3));
	userData->texIDfloor = LoadTextureFile("brick_DIFF.bmp");
	glBindVertexArray(0);//完成floorVAO的设置
	//quad
	glGenVertexArrays(1, &userData->quadVAO);
	glGenBuffers(1, &userData->quadVBO);
	glBindVertexArray(userData->quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, userData->quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (GLvoid*)(sizeof(GLfloat) * 2));
	glBindVertexArray(0);

	//GLuint defaultFramebuffer = 0;
	const GLenum attachments[4] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3
	};
	//glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFramebuffer);

	//设置FBO
	glGenFramebuffers(1, &userData->framebuffer);//初始化帧缓冲framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, userData->framebuffer);//以下代码对framebuffer负责,包括纹理附件设置和rbo附件设置
	glGenTextures(4, &userData->texColorArray);
	for (int i = 0; i < 4; i++){
		//userData->texColorArray[i] = generateAttachmentTexture(GL_FALSE, GL_FALSE);//纹理附件
		glBindTexture(GL_TEXTURE_2D, userData->texColorArray[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			userData->textureWidth, userData->textureHeight,
			0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);//设置宽高但不赋予纹理文件的数据
		// Set the filtering mode
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, userData->texColorArray[i], 0);//完成FBO的纹理附件挂载
	}
	glDrawBuffers(4, attachments);
	if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
		return FALSE;

	//设置渲染缓冲对象附件
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);//初始化rbo附件
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);//以下操作对rbo负责
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);//完成对rbo的设置
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);//完成FBO的rbo附件挂载

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);//完成纹理附件和rbo附件的设置，解绑fbo

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);//初始刷新一下
	glEnable(GL_DEPTH_TEST);
	return TRUE;
}

void Update(ESContext *esContext, float deltaTime)
{
	UserData *userData = esContext->userData;

	userData->angle += (deltaTime * 50.0f);//转动
	if (userData->angle >= 360.0f)
		userData->angle -= 360.0f;

	esMatrixLoadIdentity(&userData->mvMatrix);
	esTranslate(&userData->mvMatrix, 0.0f, 0.0f, -3.0f);
	esRotate(&userData->mvMatrix, userData->angle, 0.0f, 1.0f, 0.0f);

	ESMatrix perspective;
	esMatrixLoadIdentity(&perspective);//单位化一个矩阵作为透视投影矩阵
	float aspect = (GLfloat)esContext->width / ((GLfloat)esContext->height);
	esPerspective(&perspective, 60.f, aspect, 0.2f, 20.f);

	esMatrixMultiply(&userData->mvpMatrix, &userData->mvMatrix, &perspective);
}

void DrawQuad(ESContext *esContext)
{
	UserData *userData = esContext->userData;
	glBindFramebuffer(GL_FRAMEBUFFER, userData->framebuffer);
	glViewport(0, 0, esContext->width, esContext->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(userData->programObject);//启用programObject
	glUniformMatrix4fv(userData->mvpLoc, 1, GL_FALSE, (GLfloat *)&userData->mvpMatrix);//mvpMatrix在Update里更新

	glBindVertexArray(userData->cubeVAO);
	glBindTexture(GL_TEXTURE_2D, userData->texIDcube);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glBindVertexArray(userData->floorVAO);
	glBindTexture(GL_TEXTURE_2D, userData->texIDfloor);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Bind to default framebuffer again and draw the quad plane with attched screen texture.
	glBindFramebuffer(GL_FRAMEBUFFER, userData->framebuffer);//闪烁是因为重复渲染了三维图像和QUAD
	glUseProgram(userData->programObjectQuad);//启用programObjectQuad
	glBindVertexArray(userData->quadVAO);
	glBindTexture(GL_TEXTURE_2D, userData->texColorArray[0]);//texColorArray在Init里绑定到了fbo的纹理Attachment
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//ID号为0表示缺省帧缓存，即默认的window提供的帧缓存。因此，在glBindFramebuffer()中将ID号设置为0可以解绑定当前FBO。
}

void BlitTextures(ESContext *esContext)//区位块传送
{
	UserData *userData = esContext->userData;

	// set the fbo for reading
	glBindFramebuffer(GL_READ_FRAMEBUFFER, userData->framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	// Copy the output red buffer to lower left quadrant
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, userData->textureWidth, userData->textureHeight,
		0, 0, esContext->width / 2, esContext->height / 2,
		GL_COLOR_BUFFER_BIT, GL_LINEAR);
	//(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, 
	// GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)

	// Copy the output green buffer to lower right quadrant
	glReadBuffer(GL_COLOR_ATTACHMENT1);
	glBlitFramebuffer(0, 0, userData->textureWidth, userData->textureHeight,
		esContext->width / 2, 0, esContext->width, esContext->height / 2,
		GL_COLOR_BUFFER_BIT, GL_LINEAR);

	// Copy the output blue buffer to upper left quadrant
	glReadBuffer(GL_COLOR_ATTACHMENT2);
	glBlitFramebuffer(0, 0, userData->textureWidth, userData->textureHeight,
		0, esContext->height / 2, esContext->width / 2, esContext->height,
		GL_COLOR_BUFFER_BIT, GL_LINEAR);

	// Copy the output gray buffer to upper right quadrant
	glReadBuffer(GL_COLOR_ATTACHMENT3);
	glBlitFramebuffer(0, 0, userData->textureWidth, userData->textureHeight,
		esContext->width / 2, esContext->height / 2, esContext->width, esContext->height,
		GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void Draw(ESContext *esContext)
{
	DrawQuad(esContext);
	BlitTextures(esContext);
}

void ShutDown(ESContext *esContext)
{
	UserData *userData = esContext->userData;

	glDeleteVertexArrays(1, &userData->cubeVAO);
	glDeleteBuffers(1, &userData->cubeVBO);
	glDeleteVertexArrays(1, &userData->floorVAO);
	glDeleteBuffers(1, &userData->floorVBO);
	glDeleteVertexArrays(1, &userData->quadVAO);
	glDeleteBuffers(1, &userData->quadVBO);

	glDeleteBuffers(4, userData->texColorArray);
	glDeleteFramebuffers(1, &userData->framebuffer);

	glDeleteProgram(userData->programObject);
	glDeleteProgram(userData->programObjectQuad);
}

int esMain(ESContext *esContext)
{
	esContext->userData = malloc(sizeof(UserData));

	esCreateWindow(esContext, "Deferred Rendering", screenWidth, screenHeight, ES_WINDOW_RGB | ES_WINDOW_ALPHA | ES_WINDOW_DEPTH);

	if (!Init(esContext))
	{
		return GL_FALSE;
	}
	esRegisterShutdownFunc(esContext, ShutDown);
	esRegisterUpdateFunc(esContext, Update);
	esRegisterDrawFunc(esContext, Draw);

	return GL_TRUE;
}
