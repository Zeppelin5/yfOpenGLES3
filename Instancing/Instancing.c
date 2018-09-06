#define STB_IMAGE_IMPLEMENTATION
#include <stdlib.h>
#include <math.h>
#include "esUtil.h"
#include "stb_image.h"
#ifdef _WIN32
#define srandom srand
#define random rand
#endif

float vertices[] = {
	//-位置-					-纹理坐标-		-法线-
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

#define NUM_INSTANCES   100
#define POSITION_LOC    0
#define TEXTRUE_LOC		1
#define COLOR_LOC       2
#define MVP_LOC         3


typedef struct
{
   // Handle to a program object
   GLuint programObject;

   // VBOs
   GLuint colorVBO;
   GLuint mvpVBO;

   GLuint myVBO;//含有顶点坐标和纹理
   //GLuint myVAO;

   // Rotation angle
   GLfloat angle[NUM_INSTANCES];

} UserData;

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

///
// Initialize the shader and program object
//
int Init ( ESContext *esContext )
{
   GLfloat *positions;
   GLuint *indices;

   UserData *userData = esContext->userData;
   const char vShaderStr[] =
	   "#version 300 es										\n"
	   "layout(location = 0) in vec4 a_position;			\n"
	   "layout(location = 1) in vec2 a_texCoord;			\n"
	   "layout(location = 2) in vec4 a_color;				\n"
	   "layout(location = 3) in mat4 a_mvpMatrix;			\n"
	   "smooth centroid out vec4 v_color;									\n"
	   "out vec2 v_texCoord;								\n"
	   "void main()											\n"
	   "{													\n"
	   "   v_color = a_color;								\n"
	   "   gl_Position = a_mvpMatrix * a_position;			\n"
	   "   v_texCoord=a_texCoord;							\n"
      "}													\n";

   const char fShaderStr[] =
	   "#version 300 es										\n"
	   "precision mediump float;							\n"
	   "in vec4 v_color;									\n"
	   "in vec2 v_texCoord;									\n"
	   "out vec4 outColor;									\n"
	   "uniform sampler2D s_texture;						\n"
	   "void main()											\n"
	   "{													\n"
	  "  outColor = v_color+texture(s_texture,v_texCoord);	\n"
      "}													\n";

   // Load the shaders and get a linked program object
   userData->programObject = esLoadProgram ( vShaderStr, fShaderStr );

   // Generate the vertex data
   //glGenVertexArrays(1, &userData->myVAO);
   //glBindVertexArray(userData->myVAO);

   glGenBuffers(1, &userData->myVBO); //myVBO包含顶点坐标和纹理
   glBindBuffer(GL_ARRAY_BUFFER, userData->myVBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   // position attribute
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   // texture coord attribute
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);

   LoadTexture();

   // Random color for each instance
   {
      GLubyte colors[NUM_INSTANCES][4];
      int instance;
      srandom ( 0 );
      for ( instance = 0; instance < NUM_INSTANCES; instance++ ){
         colors[instance][0] = random() % 255/3;
         colors[instance][1] = random() % 255/3;
         colors[instance][2] = random() % 255/3;
         colors[instance][3] = 0;
      }
      glGenBuffers ( 1, &userData->colorVBO );
      glBindBuffer ( GL_ARRAY_BUFFER, userData->colorVBO );
      glBufferData ( GL_ARRAY_BUFFER, NUM_INSTANCES * 4, colors, GL_STATIC_DRAW );
   }

   // Allocate storage to store MVP per instance
   {
      int instance;
      // Random angle for each instance, compute the MVP later
      for ( instance = 0; instance < NUM_INSTANCES; instance++ ){
         userData->angle[instance] = ( float ) ( random() % 32768 ) / 32767.0f * 360.0f;
      }
      glGenBuffers ( 1, &userData->mvpVBO );
      glBindBuffer ( GL_ARRAY_BUFFER, userData->mvpVBO );
      glBufferData ( GL_ARRAY_BUFFER, NUM_INSTANCES * sizeof ( ESMatrix ), NULL, GL_DYNAMIC_DRAW );
   }

   // Load the instance color buffer
   glBindBuffer(GL_ARRAY_BUFFER, userData->colorVBO);
   glVertexAttribPointer(COLOR_LOC, 4, GL_UNSIGNED_BYTE, GL_TRUE, 4 * sizeof(GLubyte), (const void *)NULL);
   glEnableVertexAttribArray(COLOR_LOC);
   glVertexAttribDivisor(COLOR_LOC, 1); // One color per instance

   // Load the instance MVP buffer
   glBindBuffer(GL_ARRAY_BUFFER, userData->mvpVBO);
   // Load each matrix row of the MVP.  Each row gets an increasing attribute location.
   glVertexAttribPointer(MVP_LOC + 0, 4, GL_FLOAT, GL_FALSE, sizeof(ESMatrix), (const void *)NULL);
   glVertexAttribPointer(MVP_LOC + 1, 4, GL_FLOAT, GL_FALSE, sizeof(ESMatrix), (const void *)(sizeof(GLfloat) * 4));
   glVertexAttribPointer(MVP_LOC + 2, 4, GL_FLOAT, GL_FALSE, sizeof(ESMatrix), (const void *)(sizeof(GLfloat) * 8));
   glVertexAttribPointer(MVP_LOC + 3, 4, GL_FLOAT, GL_FALSE, sizeof(ESMatrix), (const void *)(sizeof(GLfloat) * 12));
   glEnableVertexAttribArray(MVP_LOC + 0);
   glEnableVertexAttribArray(MVP_LOC + 1);
   glEnableVertexAttribArray(MVP_LOC + 2);
   glEnableVertexAttribArray(MVP_LOC + 3);
   // One MVP per instance
   glVertexAttribDivisor(MVP_LOC + 0, 1);
   glVertexAttribDivisor(MVP_LOC + 1, 1);
   glVertexAttribDivisor(MVP_LOC + 2, 1);
   glVertexAttribDivisor(MVP_LOC + 3, 1);

   glBindBuffer ( GL_ARRAY_BUFFER, 0 );

   glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );

   return GL_TRUE;
}


///
// Update MVP matrix based on time
//
void Update ( ESContext *esContext, float deltaTime )
{
   UserData *userData = ( UserData * ) esContext->userData;
   ESMatrix *matrixBuf;
   ESMatrix perspective;
   float    aspect;
   int      instance = 0;
   int      numRows;
   int      numColumns;

   // Compute the window aspect ratio
   aspect = ( GLfloat ) esContext->width / ( GLfloat ) esContext->height;

   // Generate a perspective matrix with a 60 degree FOV
   esMatrixLoadIdentity ( &perspective );
   esPerspective ( &perspective, 60.0f, aspect, 1.0f, 20.0f );

   glBindBuffer ( GL_ARRAY_BUFFER, userData->mvpVBO );
   matrixBuf = ( ESMatrix * ) glMapBufferRange ( GL_ARRAY_BUFFER, 0, sizeof ( ESMatrix ) * NUM_INSTANCES, GL_MAP_WRITE_BIT );

   // Compute a per-instance MVP that translates and rotates each instance differnetly
   numRows = ( int ) sqrtf ( NUM_INSTANCES );
   numColumns = numRows;

   for ( instance = 0; instance < NUM_INSTANCES; instance++ )
   {
      ESMatrix modelview;
      float translateX = ( ( float ) ( instance % numRows ) / ( float ) numRows ) * 2.0f - 1.0f;
      float translateY = ( ( float ) ( instance / numColumns ) / ( float ) numColumns ) * 2.0f - 1.0f;

      // Generate a model view matrix to rotate/translate the cube
      esMatrixLoadIdentity ( &modelview );
	  
      // Per-instance translation
      esTranslate ( &modelview, translateX, translateY, -2.0f );

	  esScale(&modelview, 0.16f, 0.16f, 0.16f);//把立方体缩小防止交叉

      // Compute a rotation angle based on time to rotate the cube
      userData->angle[instance] += ( deltaTime * 90.0f );

      if ( userData->angle[instance] >= 360.0f )
      {
         userData->angle[instance] -= 360.0f;
      }

      // Rotate the cube
      esRotate ( &modelview, userData->angle[instance], 1.0, 0.0, 1.0 );

      // Compute the final MVP by multiplying the
      // modevleiw and perspective matrices together
      esMatrixMultiply ( &matrixBuf[instance], &modelview, &perspective );//matrixBuf在draw里调用
   }

   glUnmapBuffer ( GL_ARRAY_BUFFER );
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );

   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData->programObject );

   glDrawArraysInstanced(GL_TRIANGLES, 0, 36, NUM_INSTANCES);

   glEnable(GL_DEPTH_TEST);
}

///
// Cleanup
//
void Shutdown ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

   //glDeleteBuffers ( 1, &userData->myVAO );
   glDeleteBuffers ( 1, &userData->colorVBO );
   glDeleteBuffers ( 1, &userData->mvpVBO );
   glDeleteBuffers(1, &userData->myVBO);
   // Delete program object
   glDeleteProgram ( userData->programObject );
}


int esMain ( ESContext *esContext )
{
   esContext->userData = malloc ( sizeof ( UserData ) );

   esCreateWindow ( esContext, "实例化渲染", 640, 480, ES_WINDOW_RGB | ES_WINDOW_DEPTH );

   if ( !Init ( esContext ) )
   {
      return GL_FALSE;
   }

   esRegisterDrawFunc ( esContext, Draw );
   esRegisterShutdownFunc(esContext, Shutdown);
   esRegisterUpdateFunc(esContext, Update);

   return GL_TRUE;
}

