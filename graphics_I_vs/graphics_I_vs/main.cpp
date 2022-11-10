/*  by Alun Evans 2016 LaSalle (alunthomasevasevans@gmail.com)

MAIN:
+ This file creates the window and uses a simple render loop to paint a coloured quad
+ GLUT is a simple toolkit for create OpenGL applications. 
  To download glut go to http://alunevans.info/resources/freeglut.zip
  To install GLUT, unzip the copy the contents of the 'include', 'bin' and 'lib' directories to any valid 
  for your project e.g. default places for VS2015: 
    C:\Archivos de programa (x86)\Microsoft Visual Studio 14.0\VC\include
	C:\Archivos de programa (x86)\Microsoft Visual Studio 14.0\VC\bin
	C:\Archivos de programa (x86)\Microsoft Visual Studio 14.0\VC\lib
+ GLEW is a library that loads some useful OpenGL extensions.
  To download GLEW go to http://alunevans.info/resources/glew.zip  
  To install GLEW, unzip and copy the contents the same as GLUT
+ GLM is a maths library that provides us useful C++ classes for managing vectors and matrices
  To download GLM go here http://alunevans.info/resources/glm.zip 
  To install, unzip and copy the contents of the include directory to the include path. GLM
  is a 'header only' library which means it does not have any pre-compiled libraries
*/

//include some standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include "imageloader.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
//include GLUT, GLEW and GLM - see below
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>

#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include<glm/gtc/type_ptr.hpp>

//include our custom shader Class
#include "Shader.h"

//namespaces in C++ are declarative regions that permit scope. For example, the GLM library
//declares the glm:: namespace. So, normally, to use anything inside the library we must
//prefix it with glm:: e.g. to create a 3-component vector we type glm::vec3(0.0,1.0,0.0);

//the 'using namespace' declaration tells the compiler that, for this file, we dispense with 
//the need to prefix glm:: to all of GLMs components e.g. we only have to type vec3 instead of glm::vec3 
using namespace glm;

// ------------------------------------------------------------------------------------------
// Global Variables
// ------------------------------------------------------------------------------------------

//Texture object
GLuint texture_id;
GLuint texture_id1;
GLuint texture_id2;
GLuint texture_id3;
GLuint texture_id4;
GLuint texture_id5;
GLuint texture_id6;
GLuint texture_id7;
GLuint texture_id8;
GLuint texture_id9;


// Default window size, in pixels
int g_ViewportWidth = 512;
int g_ViewportHeight = 512;
int rot = 0;
int trans = 0;
float g_catar;
float g_angle_earth = 1.04f;
float g_angle_venus = 5.02f;
float g_angle_mercury = 8.06f;
float g_angle_mars = 3.03f;
float g_angle_jupiter = 9.02f;
float g_angle_saturn = 5.05f;
float g_angle_uranus = 7.08f;
float g_angle_neptune = 2.07f;
float g_angle = 0.0f;
float x_move = 0.0f;
float y_move = 0.0f;
float x_view = 0.0f;
float y_view = 0.0f;
float z_view = 0.0f;
glm::vec3 g_campos(-7.0f, 7.0f, -15.0f);
glm::vec3 g_camtar(-2 + x_view, 2 - y_view, -3);
// background colour - a GLM 3-component vector
const vec3 g_ColorFons(0.2f, 0.2f, 0.2f);

// shader program identifier - this is literally just an integer, which is assigned automatically
// later on
GLuint g_ShaderProgram = 0;
GLuint g_ShaderProgram_stars = 0;
GLuint g_Vao;
std::vector<GLuint> g_vaos;
std::vector<GLuint> g_triangulos;
// ------------------------------------------------------------------------------------------
// This function manually creates a square geometry (defined in the array vertices[])
// It also creates a colour array (one colour for each vertex - colors[])
// Then it creates an index buffer which makes two triangles.
// Then it creates Vertex Array Object (VAO) to store everything, 
// and three Vertex Buffer Objects (one for positions, one for colours, and one for indices 
// Finally it sets the number of triangles, which of course is the length of indices[] / 3 
// (because there are three vertices in every triangle)
// ------------------------------------------------------------------------------------------
void animate() {
	g_angle += 0.005f;
	g_angle_mercury += 0.01f;
	g_angle_venus += 0.012f;
	g_angle_earth += 0.006f;
	g_angle_mars += 0.008f;
	g_angle_jupiter += 0.005f;
	g_angle_saturn += 0.007f;
	g_angle_uranus += 0.002f;
	g_angle_neptune += 0.001f;
	glutPostRedisplay();//RECORDATORI: QUAN CANVIEM VARIABLE CAMERA FER SERVIR AQUESTA FUNCIO PER TORNAR A PINTAR
}
void creaGeometria()
{
	std::string basepath = "";
	std::string inputfile = basepath + "sphere.obj";
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	Image * image = loadBMP("earthmap1k.bmp");
	Image * image1 = loadBMP("milkyway.bmp");
	Image * image2 = loadBMP("sunmap.bmp");
	Image * image3 = loadBMP("mercurymap.bmp");
	Image * image4 = loadBMP("venusmap.bmp");
	Image * image5 = loadBMP("marsmap.bmp");
	Image * image6 = loadBMP("jupitermap.bmp");
	Image * image7 = loadBMP("saturnmap.bmp");
	Image * image8 = loadBMP("uranusmap.bmp");
	Image * image9 = loadBMP("neptunemap.bmp");


	std::cout << "# of shapes : " << shapes.size() << std::endl;
	bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str(), basepath.c_str());
	//the positions of the corner of the square, each a 3-component vector, x, y, z

	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	for (int i = 0; i < shapes.size(); i++) {

	//reusable int to store created buffers
	GLuint buffer;

	GLuint new_vao;

	// Create the VAO where we store all geometry (stored in g_Vao)
	glGenVertexArrays(1, &new_vao); // glGenVertexArrays requires a reference 
	glBindVertexArray(new_vao); //bind it now so we can start writing to it

	// Crea VBO per vèrtexs
	glGenBuffers(1, &buffer); //glGenBuffers requires a reference
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //bind our new buffer
	glBufferData(GL_ARRAY_BUFFER, shapes[i].mesh.positions.size() * sizeof(float) * 3, &(shapes[i].mesh.positions[0]), GL_STATIC_DRAW); //fill it with out vertices

	// Activate shader attribute for this buffer
	GLuint vertexLoc = glGetAttribLocation(g_ShaderProgram, "a_vertex"); //find attribute vertexPos in shader
	glEnableVertexAttribArray(vertexLoc); //enable it
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0); //specify location and data format


	//Create VBO for UVS
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER,
			shapes[0].mesh.texcoords.size()*2*sizeof(float),
			&(shapes[0].mesh.texcoords[0]),
			GL_STATIC_DRAW);

	GLuint uvLoc = glGetAttribLocation(g_ShaderProgram, "uvs");
	glEnableVertexAttribArray(uvLoc);
	glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//Create VBO for Normals
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER,
		shapes[0].mesh.normals.size() * 3 * sizeof(float),
		&(shapes[0].mesh.normals[0]),
		GL_STATIC_DRAW);

	GLuint norLoc = glGetAttribLocation(g_ShaderProgram, "normal");
	glEnableVertexAttribArray(norLoc);
	glVertexAttribPointer(norLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Create VBO por indices
	glGenBuffers(1, &buffer); //create more new buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer); //bind them as element array
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[i].mesh.indices.size() * sizeof(unsigned int), &(shapes[i].mesh.indices[0]), GL_STATIC_DRAW); //fill buffer with index data

	//make sure global var has correct number of triangles
	//why? because we will need it in onDisplay
	GLuint new_tris = shapes[i].mesh.indices.size() / 3;

	g_vaos.push_back(new_vao);
	g_triangulos.push_back(new_tris);





	//Create the texture object
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexParameteri(GL_TEXTURE_2D,
					GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,
					0,
					GL_RGB,
					image->width,
					image->height,
					0,
					GL_RGB,
					GL_UNSIGNED_BYTE,
					image->pixels);

	//Create the texture for the milkyWay
	glGenTextures(1, &texture_id2);
	glBindTexture(GL_TEXTURE_2D, texture_id2);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		image1->width,
		image1->height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		image1->pixels);

	//Create the texture for the Sun
	glGenTextures(1, &texture_id1);
	glBindTexture(GL_TEXTURE_2D, texture_id1);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		image2->width,
		image2->height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		image2->pixels);

	//Create the texture for Mercury
	glGenTextures(1, &texture_id3);
	glBindTexture(GL_TEXTURE_2D, texture_id3);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		image3->width,
		image3->height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		image3->pixels);

	//Create the texture for Venus
	glGenTextures(1, &texture_id4);
	glBindTexture(GL_TEXTURE_2D, texture_id4);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		image4->width,
		image4->height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		image4->pixels);

	//Create the texture for Mars
	glGenTextures(1, &texture_id5);
	glBindTexture(GL_TEXTURE_2D, texture_id5);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		image5->width,
		image5->height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		image5->pixels);

	//Create the texture for Jupiter
	glGenTextures(1, &texture_id6);
	glBindTexture(GL_TEXTURE_2D, texture_id6);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		image6->width,
		image6->height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		image6->pixels);

	//Create the texture for Saturn
	glGenTextures(1, &texture_id7);
	glBindTexture(GL_TEXTURE_2D, texture_id7);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		image7->width,
		image7->height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		image7->pixels);

	//Create the texture for Uranus
	glGenTextures(1, &texture_id8);
	glBindTexture(GL_TEXTURE_2D, texture_id8);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		image8->width,
		image8->height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		image8->pixels);

	//Create the texture for Uranus
	glGenTextures(1, &texture_id9);
	glBindTexture(GL_TEXTURE_2D, texture_id9);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		image9->width,
		image9->height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		image9->pixels);


}
	glBindVertexArray(0); //unbind VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffers
}

// ------------------------------------------------------------------------------------------
// Load all resources (shaders and geometry in this case)
// ------------------------------------------------------------------------------------------
void loadResources()
{
	// Carrega els shaders i crea el programa de shaders
	Shader simpleShader("shader.vert", "shader.frag");
	g_ShaderProgram = simpleShader.program;

	Shader texShader("shader-tex.vert", "shader-tex.frag");
	g_ShaderProgram_stars = texShader.program;

	// Crea la geomtria i desa-la al VAO
	creaGeometria();
}

// --------------------------------------------------------------
// GLUT callback for keyboard
// if we want to capture key information, we do so here
// --------------------------------------------------------------
void onKeyUp(unsigned char key, int x, int y)
{
	//'key' is a value which represents a key on the keyboard.
	//if we want to find the value we could write
	//cout << key << endl;

	if (key == 27 )		// 27 is the code for ESC key
	{
		// exit the programme
		exit(EXIT_SUCCESS);
	}

}

void onKeyDown(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		if (rot == 0){
			rot = 1;
		}
		else{
			rot = 0;
		}
		break;
	case 't':
		if (trans == 0){
			trans = 1;
		}
		else{
			trans = 0;
		}

	}
}
void mouse(int x, int y){
	x_view = (float)x / 600;
	y_view = (float)y / 600;
}

// --------------------------------------------------------------
// GLUT callback detects window resize
// --------------------------------------------------------------
void onReshape(int w, int h)
{
	g_ViewportWidth = w;
	g_ViewportHeight = h;

	// Modify the gl viewport so that it covers all the window
	glViewport(0, 0, g_ViewportWidth, g_ViewportHeight);
}

void drawSun()
{


	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id1);

	// Select the shader we use to paint
	glUseProgram(g_ShaderProgram_stars);

	//find sampler uniform in shader
	GLuint u_texture = glGetUniformLocation(g_ShaderProgram_stars, "u_texture");

	//bind the sampler to texture unit 0
	glUniform1i(u_texture, 0);


	GLuint u_projection = glGetUniformLocation(g_ShaderProgram_stars, "u_projection");
	GLuint u_view = glGetUniformLocation(g_ShaderProgram_stars, "u_view");

	g_camtar = glm::vec3(-2 + x_view, 2 - y_view, -3);

	glm::mat4 view_matrix = glm::lookAt(
		glm::vec3(g_campos),
		glm::vec3(g_camtar),
		glm::vec3(0, 1, 0)
		);

	glm::mat4 projection_matrix = glm::perspective(
		80.0f,
		3.0f / 3.0f,
		0.1f,
		50.0f
		);

	glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));
	glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view_matrix));

	GLuint u_model = glGetUniformLocation(g_ShaderProgram_stars, "u_model");

	GLuint u_light_dir = glGetUniformLocation(g_ShaderProgram_stars, "u_light_dir");
	glUniform3f(u_light_dir, -110 * cosf(g_angle + g_angle), 0, -110 * sinf(g_angle + g_angle));

	GLuint u_cam_pos = glGetUniformLocation(g_ShaderProgram_stars, "u_cam_pos");
	glUniform3f(u_cam_pos, g_campos[0], g_campos[1], g_campos[2]);


	GLuint colorLoc = glGetUniformLocation(g_ShaderProgram_stars, "u_color");
	//for (int i = 0; i < g_vaos.size(); i++) {
	mat4 translate_matrix = translate(mat4(1.0f), vec3(0, 0,0));


	mat4 model_matrix = translate_matrix;

	glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model_matrix));
	// Bind the VAO
	glBindVertexArray(g_vaos[0]);

	// Draw to back buffer
	glDrawElements(GL_TRIANGLES, 3 * g_triangulos[0], GL_UNSIGNED_INT, 0);
	//}

}

void drawEarth()
{
	

	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	// Select the shader we use to paint
	glUseProgram(g_ShaderProgram);

	//find sampler uniform in shader
	GLuint u_texture = glGetUniformLocation(g_ShaderProgram, "u_texture");
	
	//bind the sampler to texture unit 0
	glUniform1i(u_texture, 0);


	GLuint u_projection = glGetUniformLocation(g_ShaderProgram, "u_projection");
	GLuint u_view = glGetUniformLocation(g_ShaderProgram, "u_view");

	g_camtar = glm::vec3(-2 + x_view, 2 - y_view, -3);

	glm::mat4 view_matrix = glm::lookAt(
		glm::vec3(g_campos),
		glm::vec3(g_camtar),
		glm::vec3(0, 1, 0)
		);

	glm::mat4 projection_matrix = glm::perspective(
		135.0f,
		3.0f / 3.0f,
		0.1f,
		50.0f
		);

	glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));
	glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view_matrix));

	GLuint u_model = glGetUniformLocation(g_ShaderProgram, "u_model");

		GLuint u_light_dir = glGetUniformLocation(g_ShaderProgram, "u_light_dir");
		glUniform3f(u_light_dir, -110 * cosf(g_angle + g_angle_earth), 0, -110 * sinf(g_angle + g_angle_earth));

	GLuint u_cam_pos = glGetUniformLocation(g_ShaderProgram, "u_cam_pos");
	glUniform3f(u_cam_pos, g_campos[0], g_campos[1], g_campos[2]);


	GLuint colorLoc = glGetUniformLocation(g_ShaderProgram, "u_color");
	//for (int i = 0; i < g_vaos.size(); i++) {
	mat4 translate_matrix = translate(mat4(1.0f), vec3(9 * cosf(g_angle_earth), 0, 9 * sinf(g_angle_earth)));

		glm::vec3 myRotationAxis(0, 1, 0);
		mat4 rotate_matrix = glm::rotate(mat4(), g_angle*57.0f, myRotationAxis);

		mat4 model_matrix = translate_matrix*rotate_matrix;

		glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model_matrix));
		// Bind the VAO
		glBindVertexArray(g_vaos[0]);

		// Draw to back buffer
		glDrawElements(GL_TRIANGLES, 3 * g_triangulos[0], GL_UNSIGNED_INT, 0);
	//}

}

void drawMercury()
{


	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id3);

	// Select the shader we use to paint
	glUseProgram(g_ShaderProgram);

	//find sampler uniform in shader
	GLuint u_texture = glGetUniformLocation(g_ShaderProgram, "u_texture");

	//bind the sampler to texture unit 0
	glUniform1i(u_texture, 0);


	GLuint u_projection = glGetUniformLocation(g_ShaderProgram, "u_projection");
	GLuint u_view = glGetUniformLocation(g_ShaderProgram, "u_view");

	g_camtar = glm::vec3(-2 + x_view, 2 - y_view, -3);

	glm::mat4 view_matrix = glm::lookAt(
		glm::vec3(g_campos),
		glm::vec3(g_camtar),
		glm::vec3(0, 1, 0)
		);

	glm::mat4 projection_matrix = glm::perspective(
		160.0f,
		3.0f / 3.0f,
		0.1f,
		50.0f
		);

	glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));
	glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view_matrix));

	GLuint u_model = glGetUniformLocation(g_ShaderProgram, "u_model");

	GLuint u_light_dir = glGetUniformLocation(g_ShaderProgram, "u_light_dir");
	glUniform3f(u_light_dir, -110 * cosf(g_angle + g_angle_mercury), 0, -110 * sinf(g_angle + g_angle_mercury));

	GLuint u_cam_pos = glGetUniformLocation(g_ShaderProgram, "u_cam_pos");
	glUniform3f(u_cam_pos, g_campos[0], g_campos[1], g_campos[2]);


	GLuint colorLoc = glGetUniformLocation(g_ShaderProgram, "u_color");
	//for (int i = 0; i < g_vaos.size(); i++) {
	mat4 translate_matrix = translate(mat4(1.0f), vec3(10 * cosf(g_angle_mercury), -0.5, 10 * sinf(g_angle_mercury)));

	glm::vec3 myRotationAxis(0, 1, 0);
	mat4 rotate_matrix = glm::rotate(mat4(), g_angle*57.0f, myRotationAxis);

	mat4 model_matrix = translate_matrix*rotate_matrix;

	glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model_matrix));
	// Bind the VAO
	glBindVertexArray(g_vaos[0]);

	// Draw to back buffer
	glDrawElements(GL_TRIANGLES, 3 * g_triangulos[0], GL_UNSIGNED_INT, 0);
	//}

}

void drawVenus()
{


	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id4);

	// Select the shader we use to paint
	glUseProgram(g_ShaderProgram);

	//find sampler uniform in shader
	GLuint u_texture = glGetUniformLocation(g_ShaderProgram, "u_texture");

	//bind the sampler to texture unit 0
	glUniform1i(u_texture, 0);


	GLuint u_projection = glGetUniformLocation(g_ShaderProgram, "u_projection");
	GLuint u_view = glGetUniformLocation(g_ShaderProgram, "u_view");

	g_camtar = glm::vec3(-2 + x_view, 2 - y_view, -3);

	glm::mat4 view_matrix = glm::lookAt(
		glm::vec3(g_campos),
		glm::vec3(g_camtar),
		glm::vec3(0, 1, 0)
		);

	glm::mat4 projection_matrix = glm::perspective(
		150.0f,
		3.0f / 3.0f,
		0.1f,
		50.0f
		);

	glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));
	glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view_matrix));

	GLuint u_model = glGetUniformLocation(g_ShaderProgram, "u_model");

	GLuint u_light_dir = glGetUniformLocation(g_ShaderProgram, "u_light_dir");
	glUniform3f(u_light_dir, -110 * cosf(g_angle + g_angle_venus), 0, -110 * sinf(g_angle + g_angle_venus));

	GLuint u_cam_pos = glGetUniformLocation(g_ShaderProgram, "u_cam_pos");
	glUniform3f(u_cam_pos, g_campos[0], g_campos[1], g_campos[2]);


	GLuint colorLoc = glGetUniformLocation(g_ShaderProgram, "u_color");
	//for (int i = 0; i < g_vaos.size(); i++) {
	mat4 translate_matrix = translate(mat4(1.0f), vec3(10 * cosf(g_angle_venus), 0, 10 * sinf(g_angle_venus)));

	glm::vec3 myRotationAxis(0, 1, 0);
	mat4 rotate_matrix = glm::rotate(mat4(), g_angle*57.0f, myRotationAxis);

	mat4 model_matrix = translate_matrix*rotate_matrix;

	glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model_matrix));
	// Bind the VAO
	glBindVertexArray(g_vaos[0]);

	// Draw to back buffer
	glDrawElements(GL_TRIANGLES, 3 * g_triangulos[0], GL_UNSIGNED_INT, 0);
	//}

}

void drawMilkyWay()
{
	

	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id2);

	// Select the shader we use to paint
	glUseProgram(g_ShaderProgram_stars);

	//find sampler uniform in shader
	GLuint u_texture2 = glGetUniformLocation(g_ShaderProgram_stars, "u_texture");

	//bind the sampler to texture unit 0
	glUniform1i(u_texture2, 0);

	GLuint u_projection = glGetUniformLocation(g_ShaderProgram_stars, "u_projection");
	GLuint u_view = glGetUniformLocation(g_ShaderProgram_stars, "u_view");

	g_camtar = glm::vec3(-2 + x_view, 2 - y_view, -3);

	glm::mat4 view_matrix = glm::lookAt(
		glm::vec3(g_campos),
		glm::vec3(g_camtar),
		glm::vec3(0, 1, 0)
		);

	glm::mat4 projection_matrix = glm::perspective(
		100.0f,
		3.0f / 3.0f,
		0.1f,
		50.0f
		);

	glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));
	glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view_matrix));

	GLuint u_model = glGetUniformLocation(g_ShaderProgram_stars, "u_model");

	GLuint u_light_dir = glGetUniformLocation(g_ShaderProgram_stars, "u_light_dir");
	glUniform3f(u_light_dir, 100, 100, 100);

	GLuint u_cam_pos = glGetUniformLocation(g_ShaderProgram_stars, "u_cam_pos");
	glUniform3f(u_cam_pos, g_campos[0], g_campos[1], g_campos[2]);

	
	GLuint colorLoc = glGetUniformLocation(g_ShaderProgram_stars, "u_color");
	for (int i = 0; i < g_vaos.size(); i++) {
		mat4 translate_matrix = translate(mat4(1.0f), vec3(g_campos[0], g_campos[1], g_campos[2] ));

		glm::vec3 myRotationAxis(0, 1, 0);
		mat4 rotate_matrix = glm::rotate(mat4(), g_angle*57.0f, myRotationAxis);

		mat4 scalate_matrix = glm::scale(vec3(50.0f, 50.0f, 50.0f));


		mat4 model_matrix = translate_matrix * scalate_matrix;

		glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glUniform3f(colorLoc, ((i *	4) - 0.5), ((i / 2) + 0.3), ((i / 3) + 0.3));
		// Bind the VAO
		glBindVertexArray(g_vaos[i]);

		// Draw to back buffer
		glDrawElements(GL_TRIANGLES, 3 * g_triangulos[i], GL_UNSIGNED_INT, 0);
	}

}

void drawMars()
{


	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id5);

	// Select the shader we use to paint
	glUseProgram(g_ShaderProgram);

	//find sampler uniform in shader
	GLuint u_texture = glGetUniformLocation(g_ShaderProgram, "u_texture");

	//bind the sampler to texture unit 0
	glUniform1i(u_texture, 0);


	GLuint u_projection = glGetUniformLocation(g_ShaderProgram, "u_projection");
	GLuint u_view = glGetUniformLocation(g_ShaderProgram, "u_view");

	g_camtar = glm::vec3(-2 + x_view, 2 - y_view, -3);

	glm::mat4 view_matrix = glm::lookAt(
		glm::vec3(g_campos),
		glm::vec3(g_camtar),
		glm::vec3(0, 1, 0)
		);

	glm::mat4 projection_matrix = glm::perspective(
		125.0f,
		3.0f / 3.0f,
		0.1f,
		50.0f
		);

	glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));
	glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view_matrix));

	GLuint u_model = glGetUniformLocation(g_ShaderProgram, "u_model");

	GLuint u_light_dir = glGetUniformLocation(g_ShaderProgram, "u_light_dir");
	glUniform3f(u_light_dir, -110 * cosf(g_angle + g_angle_mars), 0, -110 * sinf(g_angle + g_angle_mars));

	GLuint u_cam_pos = glGetUniformLocation(g_ShaderProgram, "u_cam_pos");
	glUniform3f(u_cam_pos, g_campos[0], g_campos[1], g_campos[2]);


	GLuint colorLoc = glGetUniformLocation(g_ShaderProgram, "u_color");
	//for (int i = 0; i < g_vaos.size(); i++) {
	mat4 translate_matrix = translate(mat4(1.0f), vec3(10 * cosf(g_angle_mars), 0, 10 * sinf(g_angle_mars)));

	glm::vec3 myRotationAxis(0, 1, 0);
	mat4 rotate_matrix = glm::rotate(mat4(), g_angle*57.0f, myRotationAxis);

	mat4 model_matrix = translate_matrix*rotate_matrix;

	glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model_matrix));
	// Bind the VAO
	glBindVertexArray(g_vaos[0]);

	// Draw to back buffer
	glDrawElements(GL_TRIANGLES, 3 * g_triangulos[0], GL_UNSIGNED_INT, 0);
	//}

}

void drawJupiter()
{


	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id6);

	// Select the shader we use to paint
	glUseProgram(g_ShaderProgram);

	//find sampler uniform in shader
	GLuint u_texture = glGetUniformLocation(g_ShaderProgram, "u_texture");

	//bind the sampler to texture unit 0
	glUniform1i(u_texture, 0);


	GLuint u_projection = glGetUniformLocation(g_ShaderProgram, "u_projection");
	GLuint u_view = glGetUniformLocation(g_ShaderProgram, "u_view");

	g_camtar = glm::vec3(-2 + x_view, 2 - y_view, -3);

	glm::mat4 view_matrix = glm::lookAt(
		glm::vec3(g_campos),
		glm::vec3(g_camtar),
		glm::vec3(0, 1, 0)
		);

	glm::mat4 projection_matrix = glm::perspective(
		105.0f,
		3.0f / 3.0f,
		0.1f,
		50.0f
		);

	glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));
	glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view_matrix));

	GLuint u_model = glGetUniformLocation(g_ShaderProgram, "u_model");

	GLuint u_light_dir = glGetUniformLocation(g_ShaderProgram, "u_light_dir");
	glUniform3f(u_light_dir, -110 * cosf(g_angle + g_angle_jupiter), 0, -110 * sinf(g_angle + g_angle_jupiter));

	GLuint u_cam_pos = glGetUniformLocation(g_ShaderProgram, "u_cam_pos");
	glUniform3f(u_cam_pos, g_campos[0], g_campos[1], g_campos[2]);


	GLuint colorLoc = glGetUniformLocation(g_ShaderProgram, "u_color");
	//for (int i = 0; i < g_vaos.size(); i++) {
	mat4 translate_matrix = translate(mat4(1.0f), vec3(11 * cosf(g_angle_jupiter), 0, 11 * sinf(g_angle_jupiter)));

	glm::vec3 myRotationAxis(0, 1, 0);
	mat4 rotate_matrix = glm::rotate(mat4(), g_angle*57.0f, myRotationAxis);

	mat4 model_matrix = translate_matrix*rotate_matrix;

	glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model_matrix));
	// Bind the VAO
	glBindVertexArray(g_vaos[0]);

	// Draw to back buffer
	glDrawElements(GL_TRIANGLES, 3 * g_triangulos[0], GL_UNSIGNED_INT, 0);
	//}

}

void drawSaturn()
{


	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id7);

	// Select the shader we use to paint
	glUseProgram(g_ShaderProgram);

	//find sampler uniform in shader
	GLuint u_texture = glGetUniformLocation(g_ShaderProgram, "u_texture");

	//bind the sampler to texture unit 0
	glUniform1i(u_texture, 0);


	GLuint u_projection = glGetUniformLocation(g_ShaderProgram, "u_projection");
	GLuint u_view = glGetUniformLocation(g_ShaderProgram, "u_view");

	g_camtar = glm::vec3(-2 + x_view, 2 - y_view, -3);

	glm::mat4 view_matrix = glm::lookAt(
		glm::vec3(g_campos),
		glm::vec3(g_camtar),
		glm::vec3(0, 1, 0)
		);

	glm::mat4 projection_matrix = glm::perspective(
		110.0f,
		3.0f / 3.0f,
		0.1f,
		50.0f
		);

	glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));
	glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view_matrix));

	GLuint u_model = glGetUniformLocation(g_ShaderProgram, "u_model");

	GLuint u_light_dir = glGetUniformLocation(g_ShaderProgram, "u_light_dir");
	glUniform3f(u_light_dir, -110 * cosf(g_angle + g_angle_saturn), 0, -110 * sinf(g_angle + g_angle_saturn));

	GLuint u_cam_pos = glGetUniformLocation(g_ShaderProgram, "u_cam_pos");
	glUniform3f(u_cam_pos, g_campos[0], g_campos[1], g_campos[2]);


	GLuint colorLoc = glGetUniformLocation(g_ShaderProgram, "u_color");
	//for (int i = 0; i < g_vaos.size(); i++) {
	mat4 translate_matrix = translate(mat4(1.0f), vec3(15 * cosf(g_angle_saturn), 0, 15 * sinf(g_angle_saturn)));

	glm::vec3 myRotationAxis(0, 1, 0);
	mat4 rotate_matrix = glm::rotate(mat4(), g_angle*57.0f, myRotationAxis);

	mat4 model_matrix = translate_matrix*rotate_matrix;

	glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model_matrix));
	// Bind the VAO
	glBindVertexArray(g_vaos[0]);

	// Draw to back buffer
	glDrawElements(GL_TRIANGLES, 3 * g_triangulos[0], GL_UNSIGNED_INT, 0);
	//}

}

void drawUranus()
{


	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id8);

	// Select the shader we use to paint
	glUseProgram(g_ShaderProgram);

	//find sampler uniform in shader
	GLuint u_texture = glGetUniformLocation(g_ShaderProgram, "u_texture");

	//bind the sampler to texture unit 0
	glUniform1i(u_texture, 0);


	GLuint u_projection = glGetUniformLocation(g_ShaderProgram, "u_projection");
	GLuint u_view = glGetUniformLocation(g_ShaderProgram, "u_view");

	g_camtar = glm::vec3(-2 + x_view, 2 - y_view, -3);

	glm::mat4 view_matrix = glm::lookAt(
		glm::vec3(g_campos),
		glm::vec3(g_camtar),
		glm::vec3(0, 1, 0)
		);

	glm::mat4 projection_matrix = glm::perspective(
		120.0f,
		3.0f / 3.0f,
		0.1f,
		50.0f
		);

	glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));
	glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view_matrix));

	GLuint u_model = glGetUniformLocation(g_ShaderProgram, "u_model");

	GLuint u_light_dir = glGetUniformLocation(g_ShaderProgram, "u_light_dir");
	glUniform3f(u_light_dir, -110 * cosf(g_angle + g_angle_uranus), 0, -110 * sinf(g_angle + g_angle_uranus));

	GLuint u_cam_pos = glGetUniformLocation(g_ShaderProgram, "u_cam_pos");
	glUniform3f(u_cam_pos, g_campos[0], g_campos[1], g_campos[2]);


	GLuint colorLoc = glGetUniformLocation(g_ShaderProgram, "u_color");
	//for (int i = 0; i < g_vaos.size(); i++) {
	mat4 translate_matrix = translate(mat4(1.0f), vec3(21 * cosf(g_angle_uranus), 0, 21 * sinf(g_angle_uranus)));

	glm::vec3 myRotationAxis(0, 1, 0);
	mat4 rotate_matrix = glm::rotate(mat4(), g_angle*57.0f, myRotationAxis);

	mat4 model_matrix = translate_matrix*rotate_matrix;

	glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model_matrix));
	// Bind the VAO
	glBindVertexArray(g_vaos[0]);

	// Draw to back buffer
	glDrawElements(GL_TRIANGLES, 3 * g_triangulos[0], GL_UNSIGNED_INT, 0);
	//}

}

void drawNeptune()
{


	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id9);

	// Select the shader we use to paint
	glUseProgram(g_ShaderProgram);

	//find sampler uniform in shader
	GLuint u_texture = glGetUniformLocation(g_ShaderProgram, "u_texture");

	//bind the sampler to texture unit 0
	glUniform1i(u_texture, 0);


	GLuint u_projection = glGetUniformLocation(g_ShaderProgram, "u_projection");
	GLuint u_view = glGetUniformLocation(g_ShaderProgram, "u_view");

	g_camtar = glm::vec3(-2 + x_view, 2 - y_view, -3);

	glm::mat4 view_matrix = glm::lookAt(
		glm::vec3(g_campos),
		glm::vec3(g_camtar),
		glm::vec3(0, 1, 0)
		);

	glm::mat4 projection_matrix = glm::perspective(
		118.0f,
		3.0f / 3.0f,
		0.1f,
		50.0f
		);

	glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(projection_matrix));
	glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view_matrix));

	GLuint u_model = glGetUniformLocation(g_ShaderProgram, "u_model");

	GLuint u_light_dir = glGetUniformLocation(g_ShaderProgram, "u_light_dir");
	glUniform3f(u_light_dir, -110 * cosf(g_angle + g_angle_neptune), 0, -110 * sinf(g_angle + g_angle_neptune));

	GLuint u_cam_pos = glGetUniformLocation(g_ShaderProgram, "u_cam_pos");
	glUniform3f(u_cam_pos, g_campos[0], g_campos[1], g_campos[2]);


	GLuint colorLoc = glGetUniformLocation(g_ShaderProgram, "u_color");
	//for (int i = 0; i < g_vaos.size(); i++) {
	mat4 translate_matrix = translate(mat4(1.0f), vec3(24 * cosf(g_angle_neptune), 0, 24 * sinf(g_angle_neptune)));

	glm::vec3 myRotationAxis(0, 1, 0);
	mat4 rotate_matrix = glm::rotate(mat4(), g_angle*57.0f, myRotationAxis);

	mat4 model_matrix = translate_matrix*rotate_matrix;

	glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model_matrix));
	// Bind the VAO
	glBindVertexArray(g_vaos[0]);

	// Draw to back buffer
	glDrawElements(GL_TRIANGLES, 3 * g_triangulos[0], GL_UNSIGNED_INT, 0);
	//}

}




void onDisplay()
{
	
	// Clear the color buffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	drawMilkyWay();

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	
	drawSun();
	drawMercury();
	drawVenus();
	drawEarth();
	drawMars();
	drawJupiter();
	drawSaturn();
	drawUranus();
	drawNeptune();

	//clear everything
	glBindVertexArray(0);
	glUseProgram(0);

	// Swap the buffers so back buffer is on screen 
	glutSwapBuffers();
}

int main(int argc, char* argv[]) {

	//initalise GLUT - GLUT is a windowing toolkit designed to help us make 
	//OpenGL apps on MS Windows
	glutInit(&argc, argv);
	//screen format and precision
	glutInitDisplayMode( GLUT_RGBA  | GLUT_DOUBLE );
	//window size
	glutInitWindowSize(g_ViewportWidth, g_ViewportHeight);
	//create the window
	glutCreateWindow("Hello graphics!");
	//Init GLEW - glew allows us to use loads of useful opengl functions, especially on Windows
	//it's not 100% necessary though
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK){
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		return EXIT_FAILURE;
	}
	
	// Clear the background colour to our global variable
	glClearColor(g_ColorFons.x, g_ColorFons.y, g_ColorFons.z, 1.0f );

	// Load resources
	loadResources();

	//this line is very important, as it tell GLUT what function to execute
	//every frame (which should be our draw function)
	glutDisplayFunc( onDisplay );
	glutIdleFunc(animate);

	//tell GLUT about the keyboard and window resizing
	glutKeyboardUpFunc( onKeyUp );
	glutKeyboardFunc(onKeyDown);
	glutPassiveMotionFunc(mouse);
	glutReshapeFunc( onReshape );
		
	//start everything up
	glutMainLoop();

	return EXIT_SUCCESS;
}