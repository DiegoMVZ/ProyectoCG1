/*
Semestre 2023-1
Animación:
1.- Simple o básica:Por banderas y condicionales
2.- Compleja: Por medio de funciones y algoritmos,Textura Animada.
4.- Técnicas de Animación: Por Keyframes
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>r
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movHel;
float movHel1;
float movCo1;
float movCo2;
float movCo3;
float movCo4;
float movOffset;
float rotllanta;
float rotllanta1;
float rotllantaOffset;
bool avanza;
bool avanza1;
bool avanza2;
bool avanzac2;
bool avanzac1;
bool avanzac3;
bool avanzac4;
float toffsetu = 0.0f;
float toffsetv = 0.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
//Texture AgaveTexture;
Texture FlechaTexture;
Texture FlechaTexture1;

Model Kitt_M;
Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;
Model Dado_M;
Model Pista_M;
Model Me_M;
Model PuestoTortas1;
Model PuestoTacos1;
Model Fdhd;
Model Fdpi;
Model Dongato;
Model Poste_M;
Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};


	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	//AgaveTexture = Texture("Textures/Agave.tga");
	//AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/LetreroTacostga.tga");
	FlechaTexture.LoadTextureA();
	FlechaTexture1 = Texture("Textures/LetreroTortastga.tga");
	FlechaTexture1.LoadTextureA();
	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/k_rueda.3ds");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Camino_M = Model();
	Camino_M.LoadModel("Models/railroad track.obj");
	Pista_M = Model();
	Pista_M.LoadModel("Models/pista.obj");
	Me_M = Model();
	Me_M.LoadModel("Models/mitsev3obj.obj");
	Poste_M = Model();
	Poste_M.LoadModel("Models/posteobj.obj");
	PuestoTortas1 = Model();
	PuestoTortas1.LoadModel("Models/PuestoTortas1.obj");
	PuestoTacos1 = Model();
	PuestoTacos1.LoadModel("Models/PuestoTacos1.obj");
	Fdhd = Model();
	Fdhd.LoadModel("Models/Foodthd.obj");
	Fdpi = Model();
	Fdpi.LoadModel("Models/Foodtpi.obj");
	Dongato = Model();
	Dongato.LoadModel("Models/Dongato.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//luz de helicóptero

	//luz de faro



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	movCoche = 0.0f;
	movHel = 0.0f;
	movHel1 = 0.0f;
	movOffset = 0.1f;//.01
	movOffset = 0.1f;//.01
	rotllanta = 6.1f;
	rotllantaOffset = 0.9f;


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime; ///Animación del coche, lo q estas vviendo esté estandarizado. 
		//deltaTime se mantenga constante la animación, si no se usara deltTime se vería la animación a difernete velocidad
		//dependiendo del equipo gráfico con el q se cuente
		///estamos diciedcon mientars sea menor a 30, decrementa el valor por el offset multiplicado por el deltaTime
		//vale .01m decrementandpo .01
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		if (avanza)
		{
			if (movCoche > -300.0f)
			{
				movCoche -= movOffset * deltaTime;
				//printf("avanza%f \n ",movCoche);
			}
			else {
				avanza = false;
			}
		}
		if (!avanza)
		{
			if (movCoche < 300.0f)
			{
				movCoche += movOffset * deltaTime;
			}
			else
			{
				avanza = true;
			}
		}
		if (avanza1)
		{
			if (movHel > -100.0f)
			{
				movHel -= movOffset * deltaTime;
				//printf("avanza%f \n ",movCoche);
			}
			else {
				avanza1 = false;
			}
		}
		if (!avanza1)
		{
			if (movHel < 100.0f)
			{
				movHel += movOffset * deltaTime;
			}
			else
			{
				avanza1 = true;
			}
		}
		//
		if (avanza2)
		{
			if (movHel1 > -100.0f)
			{
				movHel1 -= movOffset * deltaTime;
				//printf("avanza%f \n ",movCoche);
			}
			else {
				avanza2 = false;
			}
		}
		if (!avanza2)
		{
			if (movHel1 < 100.0f)
			{
				movHel1 += movOffset * deltaTime;
			}
			else
			{
				avanza2 = true;
			}
		}
		//coche
		if (avanzac1)
		{
			if (movCo1 > -5.0f)
			{
				movCo1 -= movOffset * deltaTime;
				//printf("avanza%f \n ",movCoche);
			}
			else {
				avanzac1 = false;
			}
		}
		if (!avanzac1)
		{
			if (movCo1 < 10.0f)
			{
				movCo1 += movOffset * deltaTime;
			}
			else
			{
				avanzac1 = true;
			}
		}
		if (avanzac2)
		{
			if (movCo2 > -5.0f)
			{
				movCo2 -= movOffset * deltaTime;
				//printf("avanza%f \n ",movCoche);
			}
			else {
				avanzac2 = false;
			}
		}
		if (!avanzac2)
		{
			if (movCo2 < 50.0f)
			{
				movCo2 += movOffset * deltaTime;
			}
			else
			{
				avanzac2 = true;
			}
		}
		if (avanzac3)
		{
			if (movCo3 > 10.0f)
			{
				movCo3 -= movOffset * deltaTime;
				//printf("avanza%f \n ",movCoche);
			}
			else {
				avanzac3 = false;
			}
		}
		if (!avanzac3)
		{
			if (movCo3 < 30.0f)
			{
				movCo3 += movOffset * deltaTime;
			}
			else
			{
				avanzac3 = true;
			}
		}
		if (avanzac4)
		{
			if (movCo4 > -40.0f)
			{
				movCo4 -= movOffset * deltaTime;
				//printf("avanza%f \n ",movCoche);
			}
			else {
				avanzac4 = false;
			}
		}
		if (!avanzac4)
		{
			if (movCo4 < 40.0f)
			{
				movCo4 += movOffset * deltaTime;
			}
			else
			{
				avanzac4 = true;
			}
		}


		rotllanta += rotllantaOffset * deltaTime;
		rotllanta1 += rotllantaOffset * deltaTime;
		//rotllanta += rotllantaOffset * deltaTime;
		//rotllanta += rotllantaOffset * deltaTime;
		//if (rotllanta > 359.0f)
			//rotllanta = 0.0f;
		//printf("rotllanta:%f\n",rotllanta);
		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche, 0.5f, -1.5f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Kitt_M.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.0, -0.1f, 0.2f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.07f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Llanta_M.RenderModel();

		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 3.0f, -1.0));
		model = glm::translate(model, glm::vec3(-movHel1, 3.0f + (3.5 * sin(glm::radians(rotllanta))), -movHel - 0.1 * rotllanta));
		//model = glm::translate(model, glm::vec3(-movHel1 + (3.5 * sin(glm::radians(rotllanta))), 0.0f, -movHel - 0.1 * rotllanta));

		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 3.0f + (3.5 * sin(glm::radians(rotllanta))), 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//color = glm::vec3(0.0f, 1.0f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Blackhawk_M.RenderModel();

		//color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.53f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 1.0f));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Camino_M.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 3.0f, -1.0));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Fdhd.RenderModel();

			//1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 3.0f, -1.0));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuestoTacos1.RenderModel();

		//2

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(200.0f, 3.0f, -1.0));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuestoTortas1.RenderModel();


		//3

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, 3.0f, -1.0));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Fdpi.RenderModel();

		//4

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, 3.0f, -1.0));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dongato.RenderModel();

		//textura con movimiento
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetu += 0.001;
		toffsetv += 0.0;
		//para que no se desborde la variable
		if (toffsetu > 1.0)
			toffsetu = 0.0;//eje hacia arriba
		//if (toffsetv > 1.0)
		//	toffsetv = 0;
		//printf("\ntfosset %f \n", toffsetu);
		//pasar a la variable uniform el valor actualizado
		toffset = glm::vec2(toffsetu, toffsetv);//2 valores, el incremento hace q las flechas se vean animadas
		//las felchas se mueven es un plano y e mueven las coordenadas de texturizado
		//ecuación del peralte
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.2f, -6.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//toffset=glm::vec2(0.0f,0.0f);
		//AgaveTexture.UseTexture();
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[3]->RenderMesh();
		glDisable(GL_BLEND);
		//meshList[3]->RenderMesh();
		///
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 0.2f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		toffset = glm::vec2(0.0f, 0.0f);
		//AgaveTexture.UseTexture();
		FlechaTexture1.UseTexture();
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[4]->RenderMesh();
		//
		glDisable(GL_BLEND);


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}