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

   // Handle to a framebuffer object
   GLuint fbo;

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

GLuint screenWidth = 400;
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

GLuint generateAttachmentTexture(GLboolean depth,GLboolean stencil){
	GLenum attachment_type;
	if (!depth && !stencil) attachment_type = GL_RGB;
	//else if (depth && !stencil) attachment_type = GL_DEPTH_COMPONENT;
	//else if (!depth && stencil) attachment_type = GL_STENCIL_COMPONENTS;
	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	//texID挂载为attachment_type的附件
	glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, screenWidth, screenHeight, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texID;
}

char vShaderStr[] =
"#version 300 es										 \n"
"layout(location = 0) in vec4 a_position;			 \n"
"layout(location = 1) in vec2 a_texCoord;			 \n"
"uniform mat4 u_mvpMatrix;							 \n"
"out vec2 v_texCoord;								 \n"
"void main()											 \n"
"{													 \n"
"    gl_Position = u_mvpMatrix * a_position;			 \n"
"	 v_texCoord=a_texCoord;						     \n"
"}													 \n";

char fShaderStr[] =
"#version 300 es                                      \n"
"precision mediump float;							 \n"
"in vec2 v_texCoord;									 \n"
"uniform sampler2D screenTexture;					 \n"
"out vec4 outColor;									 \n"
"void main()											 \n"
"{													 \n"
"	outColor = texture(screenTexture,v_texCoord);	 \n"
"}                                                    \n";

char vScreenShaderStr[] =
"#version 300 es													\n"
"layout(location = 0) in vec2 a_position;							\n"
"layout(location = 1) in vec2 texCoord;								\n"
"out vec2 v_texCoord;												\n"
"void main()														\n"
"{																	\n"
"   gl_Position = vec4(a_position.x,a_position.y,0.0,1.0);            \n"
"   v_texCoord=texCoord;											\n"
"}																    \n";
char fScreenShaderStr[] =
"#version 300 es													\n"
"precision mediump float;											\n"
"out vec4 outColor;													\n"
"in vec2 v_texCoord;												\n"
"uniform sampler2D Texture;											\n"
"void main()														\n"
"{																	\n"
//"	vec3 texColor=texture(Texture,v_texCoord).rgb;						\n"
//"	outColor=vec4(texColor,1.0);									\n"
"	outColor=texture(Texture,v_texCoord);							\n"
"}																	\n";

int Init ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

   // Load the shaders and get a linked program object
   userData->programObject = esLoadProgram ( vShaderStr, fShaderStr );
   userData->programObjectQuad = esLoadProgram(vScreenShaderStr, fScreenShaderStr);

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
   userData->texIDfloor=LoadTextureFile("brick_DIFF.bmp");
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

   //设置FBO
   glGenFramebuffers(1, &userData->framebuffer);//初始化帧缓冲framebuffer
   glBindFramebuffer(GL_FRAMEBUFFER, userData->framebuffer);//以下代码对framebuffer负责,包括纹理附件设置和rbo附件设置
   userData->texColorbuffer = generateAttachmentTexture(GL_FALSE, GL_FALSE);//纹理附件
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, userData->texColorbuffer, 0);//完成FBO的纹理附件挂载
   
   //设置渲染缓冲对象附件
   GLuint rbo;
   glGenRenderbuffers(1, &rbo);//初始化rbo附件
   glBindRenderbuffer(GL_RENDERBUFFER, rbo);//以下操作对rbo负责
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
   glBindRenderbuffer(GL_RENDERBUFFER, 0);//完成对rbo的设置
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);//完成FBO的rbo附件挂载

   if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	   printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
   glBindFramebuffer(GL_FRAMEBUFFER, 0);//完成fbo的设置，解绑
   
   glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
   glEnable(GL_DEPTH_TEST);
   return TRUE;
}

void Update( ESContext *esContext ,float deltaTime)
{
	UserData *userData = esContext->userData;

	userData->angle += (deltaTime * 50.0f);
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

void Draw ( ESContext *esContext )
{

	UserData *userData = esContext->userData;
	glViewport(0, 0, esContext->width, esContext->height);
	// Bind to framebuffer and draw to color texture 
	// as we normally would.
	glBindFramebuffer(GL_FRAMEBUFFER, userData->framebuffer);
	// Clear all attached buffers  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(userData->programObject);//启用programObject
	
	glUniformMatrix4fv(userData->mvpLoc, 1, GL_FALSE, (GLfloat *)&userData->mvpMatrix);

	glBindVertexArray(userData->cubeVAO);
	glBindTexture(GL_TEXTURE_2D, userData->texIDcube);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glBindVertexArray(userData->floorVAO);
	glBindTexture(GL_TEXTURE_2D, userData->texIDfloor);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	// Bind to default framebuffer again and draw the 
	// quad plane with attched screen texture.
	glUseProgram(userData->programObjectQuad);//启用programObjectQuad
	glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(userData->quadVAO);
	glBindTexture(GL_TEXTURE_2D, userData->texColorbuffer);
	//glDrawArrays(GL_LINES, 0, 6);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glCullFace(GL_FRONT);
}

void ShutDown ( ESContext *esContext )
{
	UserData *userData = esContext->userData;

	glDeleteVertexArrays(1, &userData->cubeVAO);
	glDeleteBuffers(1, &userData->cubeVBO);
	glDeleteVertexArrays(1, &userData->floorVAO);
	glDeleteBuffers(1, &userData->floorVBO);
	glDeleteVertexArrays(1, &userData->quadVAO);
	glDeleteBuffers(1, &userData->quadVBO);

	glDeleteProgram(userData->programObject);
	glDeleteProgram(userData->programObjectQuad);
}

int esMain ( ESContext *esContext )
{
   esContext->userData = malloc ( sizeof ( UserData ) );

   esCreateWindow(esContext, "FBO Demo", screenWidth, screenHeight, ES_WINDOW_RGB | ES_WINDOW_ALPHA | ES_WINDOW_DEPTH);

   if ( !Init ( esContext ) )
   {
      return GL_FALSE;
   }
   esRegisterShutdownFunc(esContext, ShutDown);
   esRegisterUpdateFunc(esContext, Update);
   esRegisterDrawFunc ( esContext, Draw );

   return GL_TRUE;
}
