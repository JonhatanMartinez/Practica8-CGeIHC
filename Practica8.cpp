/*
Práctica 7: Iluminación 1
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
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
#include "Mesh_tn.h"
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

Window mainWindow;
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
// Añadir Texturas del dado y del holocron
Texture dadoTexture;
Texture holocronTexture;

Model AvionTextura_M;

Model patafrontizq_M;

Model patafrontder_M;

Model patamidizq_M;

Model patamidder_M;

Model patabackizq_M;

Model patabackder_M;

Model Brazo_M;

Model Cuerpo_M;

Texture NaveTexture;
Model Nave_M;

Texture fuegoAzulTexture;

//Añadir modelo del avión.
//Model Avion_M;


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


//función de calculo de normales por promedio de vértices 
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

	MeshModel* obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel* obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel* obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);

	MeshModel* obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshListModel.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals

	GLfloat cubo_vertices[] = {
		// front Galactic Republic
		// x       y       z        S        T        NX     NY     NZ
		-0.5f,  -0.5f,   0.5f,    0.355f,  0.600f,   0.0f,  0.0f,  -1.0f, // 0
		 0.5f,  -0.5f,   0.5f,    0.555f,  0.600f,   0.0f,  0.0f,  -1.0f, // 1
		 0.5f,   0.5f,   0.5f,    0.555f,  0.775f,   0.0f,  0.0f,  -1.0f, // 2
		-0.5f,   0.5f,   0.5f,    0.355f,  0.775f,   0.0f,  0.0f,  -1.0f, // 3

		// rigth Rebel Alliance
		// x       y       z        S        T        NX     NY     NZ
		 0.5f,  -0.5f,   0.5f,    0.050f,  0.800f,  -1.0f,  0.0f,   0.0f, // 4
		 0.5f,  -0.5f,  -0.5f,    0.250f,  0.800f,  -1.0f,  0.0f,   0.0f, // 5
		 0.5f,   0.5f,  -0.5f,    0.250f,  0.965f,  -1.0f,  0.0f,   0.0f, // 6
		 0.5f,   0.5f,   0.5f,    0.050f,  0.965f,  -1.0f,  0.0f,   0.0f, // 7

		 // BACK Galactic Empire
		 // x       y       z        S        T        NX     NY     NZ
		 -0.5f,  -0.5f,  -0.5f,    0.585f,  0.800f,   0.0f,  0.0f,   1.0f, // 8
		  0.5f,  -0.5f,  -0.5f,    0.375f,  0.800f,   0.0f,  0.0f,   1.0f, // 9
		  0.5f,   0.5f,  -0.5f,    0.375f,  0.975f,   0.0f,  0.0f,   1.0f, // 10
		 -0.5f,   0.5f,  -0.5f,    0.585f,  0.975f,   0.0f,  0.0f,   1.0f, // 11

		 // LEFT Old Galactic Republic
		 // x       y       z        S        T        NX     NY     NZ
		 -0.5f,  -0.5f,  -0.5f,    0.200f,  0.600f,   1.0f,  0.0f,   0.0f, // 12
		 -0.5f,  -0.5f,   0.5f,    0.385f,  0.600f,   1.0f,  0.0f,   0.0f, // 13
		 -0.5f,   0.5f,   0.5f,    0.385f,  0.775f,   1.0f,  0.0f,   0.0f, // 14
		 -0.5f,   0.5f,  -0.5f,    0.200f,  0.775f,   1.0f,  0.0f,   0.0f, // 15

		 // BOTTOM Separatists
		 // x       y       z        S        T        NX     NY     NZ
		 -0.5f,  -0.5f,   0.5f,    0.170f,  0.400f,   0.0f,  1.0f,   0.0f, // 16
		  0.5f,  -0.5f,   0.5f,    0.340f,  0.400f,   0.0f,  1.0f,   0.0f, // 17
		  0.5f,  -0.5f,  -0.5f,    0.340f,  0.590f,   0.0f,  1.0f,   0.0f, // 18
		 -0.5f,  -0.5f,  -0.5f,    0.170f,  0.590f,   0.0f,  1.0f,   0.0f, // 19

		 // TOP First Order
		 // x       y       z        S        T        NX     NY     NZ
		 -0.5f,   0.5f,   0.5f,    0.690f,  0.400f,   0.0f, -1.0f,   0.0f, // 20
		  0.5f,   0.5f,   0.5f,    0.820f,  0.400f,   0.0f, -1.0f,   0.0f, // 21
		  0.5f,   0.5f,  -0.5f,    0.820f,  0.575f,   0.0f, -1.0f,   0.0f, // 22
		 -0.5f,   0.5f,  -0.5f,    0.690f,  0.575f,   0.0f, -1.0f,   0.0f  // 23
	};

	MeshModel* dado = new MeshModel();
	dado->CreateMeshModel(cubo_vertices, cubo_indices, 192, 36);
	meshListModel.push_back(dado);

}


void CrearHolocron()
{
	unsigned int holocron_indices[] = {
		// Cara Frontal
		0, 1, 2,
		0, 2, 3,

		// Cara Derecha
		4, 5, 6,
		4, 6, 7,

		// Cara Trasera 
		8, 9, 10,
		8, 10, 11,

		// Cara Izquierda
		12, 13, 14,
		12, 14, 15,

		// Cara Superior
		16, 17, 18,
		16, 18, 19,

		// --- 6. Triángulos Superiores
		20, 21, 22,
		23, 24, 25,
		26, 27, 28,
		29, 30, 31,

		// Triángulos Laterales Inferiores
		32, 33, 34,
		35, 36, 37,
		38, 39, 40,
		41, 42, 43,
		44, 45, 46,
		47, 48, 49,
		50, 51, 52,
		53, 54, 55,

		//Base Inferior
		58, 56, 57,
		59, 60, 58,
		58, 57, 61,
		61, 59, 58,
		57, 62, 61,
		61, 63, 59
	};

	GLfloat holocron_vertices[] = {
		// x           y          z          S       T       NX     NY     NZ

//  Cara Frontal
		// x           y          z          S       T       NX     NY     NZ
		-2.646786f, -0.139047f,  2.493757f,  0.240f, 0.770f,  0.0f,  0.0f, -1.0f, // 0
		-0.140696f, -2.601934f,  2.558561f,  0.380f, 0.770f,  0.0f,  0.0f, -1.0f, // 1
		 2.593003f, -0.085222f,  2.505755f,  0.380f, 0.985f,  0.0f,  0.0f, -1.0f, // 2
		 0.051152f,  2.399781f,  2.545043f,  0.240f, 0.985f,  0.0f,  0.0f, -1.0f, // 3  

		 //Cara Derecha: Galactic Empire 
		  2.593003f, -0.085222f,  2.505755f,  0.415f, 0.780f, -1.0f,  0.0f,  0.0f, // 4
		  2.632201f, -2.553892f,  0.007226f,  0.415f, 0.995f, -1.0f,  0.0f,  0.0f, // 5
		  2.534944f, -0.061146f, -2.580280f,  0.555f, 0.995f, -1.0f,  0.0f,  0.0f, // 6
		  2.521206f,  2.448241f,  0.063388f,  0.555f, 0.780f, -1.0f,  0.0f,  0.0f, // 7

		  //Cara Trasera
		   2.534944f, -0.061146f, -2.580280f,  0.200f, 0.600f,  0.0f,  0.0f,  1.0f, // 8
		   0.081432f, -2.513204f, -2.426005f,  0.200f, 0.800f,  0.0f,  0.0f,  1.0f, // 9
		  -2.453980f, -0.063388f, -2.477451f,  0.395f, 0.800f,  0.0f,  0.0f,  1.0f, // 10
		   0.049649f,  2.440128f, -2.417217f,  0.395f, 0.600f,  0.0f,  0.0f,  1.0f, // 11

		   //Cara Izquierda
				   // x           y          z          S       T       NX     NY     NZ
			-2.453980f, -0.063388f, -2.477451f,  0.370f, 0.600f,  1.0f,  0.0f,  0.0f, // 12
			-2.517274f, -2.579095f,  0.002115f,  0.540f, 0.600f,  1.0f,  0.0f,  0.0f, // 13
			-2.646786f, -0.139047f,  2.493757f,  0.540f, 0.800f,  1.0f,  0.0f,  0.0f, // 14
			-2.451372f,  2.421759f,  0.077538f,  0.370f, 0.800f,  1.0f,  0.0f,  0.0f, // 15

			// Cara Superior
					// x           y          z          S       T       NX     NY     NZ
			-2.451372f,  2.421759f,  0.077538f,  0.695f, 0.435f,  0.0f, -1.0f,  0.0f, // 16
			0.051152f,  2.399781f,  2.545043f,  0.695f, 0.625f,  0.0f, -1.0f,  0.0f, // 17
			2.521206f,  2.448241f,  0.063388f,  0.845f, 0.625f,  0.0f, -1.0f,  0.0f, // 18
			0.049649f,  2.440128f, -2.417217f,  0.845f, 0.435f,  0.0f, -1.0f,  0.0f, // 19

			// Triángulos Superiores
			// x           y          z          S       T       NX     NY     NZ
			-2.451372f,  2.421759f,  0.077538f,  0.510f, 0.760f,  0.5f, -0.5f, -0.5f, // 20
			-2.646786f, -0.139047f,  2.493757f,  0.725f, 0.760f,  0.5f, -0.5f, -0.5f, // 21
			0.051152f,  2.399781f,  2.545043f,  0.625f, 1.015f,  0.5f, -0.5f, -0.5f, // 22 

			// x           y          z          S       T       NX     NY     NZ
			0.051152f,  2.399781f,  2.545043f,  0.660f, 0.770f, -0.5f, -0.5f, -0.5f, // 23
			2.593003f, -0.085222f,  2.505755f,  0.880f, 0.770f, -0.5f, -0.5f, -0.5f, // 24
			2.521206f,  2.448241f,  0.063388f,  0.770f, 1.040f, -0.5f, -0.5f, -0.5f, // 25

			// x           y          z          S       T       NX     NY     NZ
			2.521206f,  2.448241f,  0.063388f,  0.805f, 0.770f, -0.5f, -0.5f,  0.5f, // 26
			2.534944f, -0.061146f, -2.580280f,  1.025f, 0.770f, -0.5f, -0.5f,  0.5f, // 27
			0.049649f,  2.440128f, -2.417217f,  0.915f, 1.040f, -0.5f, -0.5f,  0.5f, // 28

			// x           y       s   z          S       T       NX     NY     NZ
			0.049649f,  2.440128f, -2.417217f,  0.520f, 0.555f,  0.5f, -0.5f,  0.5f, // 29
			-2.453980f, -0.063388f, -2.477451f,  0.740f, 0.555f,  0.5f, -0.5f,  0.5f, // 30
			-2.451372f,  2.421759f,  0.077538f,  0.615f, 0.880f,  0.5f, -0.5f,  0.5f, // 31


			// Triángulos Laterales Inferiores

			// Carainferior 1
			// Mitad Izquierda
			-2.646786f, -0.139047f,  2.493757f,  0.135f, 0.220f,  0.5f,  0.5f, -0.5f, // 32
			-2.517274f, -2.579095f,  0.002115f,  0.015f, 0.000f,  0.5f,  0.5f, -0.5f, // 33
			-1.328995f, -2.578386f,  1.297838f,  0.135f, 0.000f,  0.5f,  0.5f, -0.5f, // 34
			// Mitad Derecha
			-2.646786f, -0.139047f,  2.493757f,  0.135f, 0.220f,  0.5f,  0.5f, -0.5f, // 35
			-1.328995f, -2.578386f,  1.297838f,  0.135f, 0.000f,  0.5f,  0.5f, -0.5f, // 36 
			-0.140696f, -2.601934f,  2.558561f,  0.255f, 0.000f,  0.5f,  0.5f, -0.5f, // 37

			// Cara Inferior 1
			// Mitad Izquierd
			2.593003f, -0.085222f,  2.505755f,  0.260f, 0.245f, -0.5f,  0.5f, -0.5f, // 38 
			-0.140696f, -2.601934f,  2.558561f,  0.115f, -0.025f, -0.5f,  0.5f, -0.5f, // 39
			1.385028f, -2.549476f,  1.308677f,  0.260f, -0.025f, -0.5f,  0.5f, -0.5f, // 40 
			// Mitad Derecha
			2.593003f, -0.085222f,  2.505755f,  0.260f, 0.245f, -0.5f,  0.5f, -0.5f, // 41)
			1.385028f, -2.549476f,  1.308677f,  0.260f, -0.025f, -0.5f,  0.5f, -0.5f, // 42
			2.632201f, -2.553892f,  0.007226f,  0.405f, -0.025f, -0.5f,  0.5f, -0.5f, // 43

			// Cara Inferior 3
			// Mitad Izquierda
			2.534944f, -0.061146f, -2.580280f,  0.530f, 0.245f, -0.5f,  0.5f,  0.5f, // 44
			2.632201f, -2.553892f,  0.007226f,  0.385f, -0.025f, -0.5f,  0.5f,  0.5f, // 45
			1.344182f, -2.542571f, -1.294387f,  0.530f, -0.025f, -0.5f,  0.5f,  0.5f, // 46
			// Mitad Derecha
			2.534944f, -0.061146f, -2.580280f,  0.530f, 0.245f, -0.5f,  0.5f,  0.5f, // 47
			1.344182f, -2.542571f, -1.294387f,  0.530f, -0.025f, -0.5f,  0.5f,  0.5f, // 48 
			0.081432f, -2.513204f, -2.426005f,  0.675f, -0.025f, -0.5f,  0.5f,  0.5f, // 49 

			// Cara Inferior 4
			// Mitad Izquierda
			-2.453980f, -0.063388f, -2.477451f,  0.650f, 0.250f,  0.5f,  0.5f,  0.5f, // 50
			0.081432f, -2.513204f, -2.426005f,  0.505f, -0.020f, 0.5f,  0.5f,  0.5f, // 51
			-1.185055f, -2.530499f, -1.198261f,  0.650f, -0.020f, 0.5f,  0.5f,  0.5f, // 52
			// Mitad Derecha
			-2.453980f, -0.063388f, -2.477451f,  0.650f, 0.250f,  0.5f,  0.5f,  0.5f, // 53
			-1.185055f, -2.530499f, -1.198261f,  0.650f, -0.020f, 0.5f,  0.5f,  0.5f, // 54 
			-2.517274f, -2.579095f,  0.002115f,  0.795f, -0.020f, 0.5f,  0.5f,  0.5f, // 55 

			// Base
						// x           y          z          S       T       NX    NY    NZ
			-2.517274f, -2.579095f, 0.002115f, 0.058f, 0.820f, 0.0f, 1.0f, 0.0f, // 56
			-1.185055f, -2.530499f, -1.198261f, 0.128f, 0.820f, 0.0f, 1.0f, 0.0f, // 57
			-1.328995f, -2.578386f, 1.297838f, 0.058f, 0.690f, 0.0f, 1.0f, 0.0f, // 58
			1.385028f, -2.549476f, 1.308677f, 0.128f, 0.560f, 0.0f, 1.0f, 0.0f, // 59 
			-0.140696f, -2.601934f, 2.558561f, 0.058f, 0.560f, 0.0f, 1.0f, 0.0f, // 60
			1.344182f, -2.542571f, -1.294387f, 0.198f, 0.690f, 0.0f, 1.0f, 0.0f, // 61
			0.081432f, -2.513204f, -2.426005f, 0.198f, 0.820f, 0.0f, 1.0f, 0.0f, // 62 
			2.632201f, -2.553892f, 0.007226f, 0.198f, 0.560f, 0.0f, 1.0f, 0.0f  // 63 
	};

	MeshModel* holocron = new MeshModel();
	// 64 vértices * 8 floats = 512 floats en el buffer; 28 triángulos * 3 índices = 84
	holocron->CreateMeshModel(holocron_vertices, holocron_indices, 512, 84);
	meshListModel.push_back(holocron);
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
	CrearHolocron();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	dadoTexture = Texture("Textures/star wars logos.png");
	dadoTexture.LoadTextureA();

	holocronTexture = Texture("Textures/star wars logos.png");
	holocronTexture.LoadTextureA();

	AvionTextura_M = Model();
	AvionTextura_M.LoadModel("Models/AvionTextura.obj");

	patafrontizq_M = Model();

	patafrontizq_M.LoadModel("Models/patafrontizq.obj");

	patafrontder_M = Model();

	patafrontder_M.LoadModel("Models/patafrontder.obj");

	patamidizq_M = Model();

	patamidizq_M.LoadModel("Models/patamidizq.obj");

	patamidder_M = Model();

	patamidder_M.LoadModel("Models/patamidder.obj");

	patabackizq_M = Model();

	patabackizq_M.LoadModel("Models/patabackizq.obj");

	patabackder_M = Model();

	patabackder_M.LoadModel("Models/patabackder.obj");

	Brazo_M = Model();

	Brazo_M.LoadModel("Models/Brazo.obj");

	Cuerpo_M = Model();

	Cuerpo_M.LoadModel("Models/Cuerpo.obj");

	NaveTexture = Texture("Textures/naveTexture.png");
	NaveTexture.LoadTextureA();

	Nave_M = Model();
	Nave_M.LoadModel("Models/nave.obj");

	fuegoAzulTexture = Texture("Textures/fuego_azul.png");
	fuegoAzulTexture.LoadTextureA();

	//Añadir modelo del avión
	//Avion_M = Model();
	//Avion_M.LoadModel("Models/");

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
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	//Luz Holocron
	pointLights[1] = PointLight(0.0f, 0.255f, 0.255f,
		0.5f, 1.0f,
		-5.0f, 4.5f, 5.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	//Luz Pinza Rover
	pointLights[2] = PointLight(0.0f, 1.0f, 0.0f,
		0.0f, 8.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.7f, 1.5f);
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
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//se crean mas luces puntuales y spotlight 

// Luz frontal del Rover
	spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.04f, 0.005f,
		15.0f);
	spotLightCount++;

	// Luz Abajo Avion
	spotLights[3] = SpotLight(1.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.2f, 0.05f,
		15.0f);
	spotLightCount++;

	//Frontal Avion
	spotLights[4] = SpotLight(0.0f, 1.0f, 1.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.04f, 0.005f,
		15.0f);
	spotLightCount++;

	// Luces Motores Nave
	// Luz Puntual Motor Izquierdo
	pointLights[3] = PointLight(0.0f, 0.5f, 1.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	// Luz Spotlight Motor Derecho
	spotLights[5] = SpotLight(0.0f, 0.5f, 1.0f,
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.04f, 0.005f,
		20.0f);
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	Sphere sp = Sphere(1.0f, 20, 20);
	sp.init();
	sp.load();
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

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

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//Encender Holocron
		if (mainWindow.getLuzHolocron())
		{
			pointLights[1].SetColor(glm::vec3(0.0f, 0.8f, 1.0f));
		}
		else
		{
			pointLights[1].SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
		}

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[2]->RenderMeshModel();

		// Avion
		glm::mat4 modelAvion = glm::mat4(1.0f);
		modelAvion = glm::translate(modelAvion, glm::vec3(mainWindow.getmoverAvion(), mainWindow.getsubeyBaja(), 6.0f));
		modelAvion = glm::scale(modelAvion, glm::vec3(1.0f, 1.0f, 1.0f));
		modelAvion = glm::rotate(modelAvion, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		// Luz Abajo Avion
		glm::vec3 avionPosDown = glm::vec3(modelAvion * glm::vec4(0.0f, -0.8f, 0.0f, 1.0f));
		glm::vec3 avionDownDir = glm::normalize(glm::vec3(modelAvion * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f)));
		spotLights[3].SetFlash(avionPosDown, avionDownDir);

		int colorIndex = mainWindow.getColorFaroAvion();
		glm::vec3 faroColor;
		switch (colorIndex) {
		case 0: faroColor = glm::vec3(1.0f, 0.0f, 0.0f); break;
		case 1: faroColor = glm::vec3(1.0f, 1.0f, 0.0f); break;
		case 2: faroColor = glm::vec3(0.0f, 1.0f, 0.0f); break;
		case 3: faroColor = glm::vec3(0.0f, 1.0f, 1.0f); break;
		case 4: faroColor = glm::vec3(0.0f, 0.0f, 1.0f); break;
		case 5: faroColor = glm::vec3(1.0f, 0.0f, 1.0f); break;
		}
		spotLights[3].SetColor(faroColor);

		// Luz frontal 
		glm::vec3 avionPosFront = glm::vec3(modelAvion * glm::vec4(0.0f, -0.1f, 3.2f, 1.0f));
		glm::vec3 avionFrontDir = glm::normalize(glm::vec3(modelAvion * glm::vec4(0.0f, -0.08f, 1.0f, 0.0f)));
		spotLights[4].SetFlash(avionPosFront, avionFrontDir);

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelAvion));
		AvionTextura_M.RenderModel();

		//Rover


		//Cuerpo Rover
		glm::mat4 bodyMatrix = glm::mat4(1.0f);

		color = glm::vec3(0.0f, 0.0f, 1.0f);
		model = bodyMatrix;
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), -1.25f, -1.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

		// Luz faro
		glm::vec3 roverPos = glm::vec3(model * glm::vec4(4.2f, 1.5f, 0.0f, 1.0f));
		glm::vec3 roverFrontDir = glm::normalize(glm::vec3(model * glm::vec4(1.0f, -0.1f, 0.0f, 0.0f)));
		spotLights[2].SetFlash(roverPos, roverFrontDir);

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cuerpo_M.RenderModel();

		bodyMatrix = model;

		glm::vec3 pivotFrontDer = glm::vec3(2.2f, 4.4f, -3.0f);
		glm::vec3 pivotFrontIzq = glm::vec3(2.2f, 4.4f, 3.0f);
		glm::vec3 pivotMidDer = glm::vec3(-3.2f, 3.3f, -3.0f);
		glm::vec3 pivotMidIzq = glm::vec3(-3.2f, 3.3f, 3.0f);
		glm::vec3 pivotBackDer = glm::vec3(-2.8f, 3.3f, -3.0f);
		glm::vec3 pivotBackIzq = glm::vec3(-2.8f, 3.3f, 3.0f);

		float giroFrontDer = 45.0f * abs(sin(glm::radians(mainWindow.getarticulacion1())));
		float giroFrontIzq = 45.0f * abs(sin(glm::radians(mainWindow.getarticulacion2())));
		float giroMidDer = 45.0f * abs(sin(glm::radians(mainWindow.getarticulacion3())));
		float giroMidIzq = 45.0f * abs(sin(glm::radians(mainWindow.getarticulacion4())));
		float giroBackDer = -45.0f * abs(sin(glm::radians(mainWindow.getarticulacion5())));
		float giroBackIzq = -45.0f * abs(sin(glm::radians(mainWindow.getarticulacion6())));

		// Llanta Frontal Derecha
		model = bodyMatrix;
		model = glm::translate(model, pivotFrontDer);
		model = glm::rotate(model, glm::radians(giroFrontDer), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 jointFrontDer = model;

		model = glm::scale(jointFrontDer, glm::vec3(0.3f, 0.3f, 0.3f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = jointFrontDer;
		model = glm::translate(model, -pivotFrontDer);
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		patafrontder_M.RenderModel();

		// Llanta Frontal Izquierda
		model = bodyMatrix;
		model = glm::translate(model, pivotFrontIzq);
		model = glm::rotate(model, glm::radians(giroFrontIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 jointFrontIzq = model;

		model = glm::scale(jointFrontIzq, glm::vec3(0.3f, 0.3f, 0.3f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = jointFrontIzq;
		model = glm::translate(model, -pivotFrontIzq);
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		patafrontizq_M.RenderModel();

		// Llanta Media Derecha
		model = bodyMatrix;
		model = glm::translate(model, pivotMidDer);
		model = glm::rotate(model, glm::radians(giroMidDer), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 jointMidDer = model;

		model = glm::scale(jointMidDer, glm::vec3(0.3f, 0.3f, 0.3f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = jointMidDer;
		model = glm::translate(model, -pivotMidDer);
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		patamidder_M.RenderModel();

		// Llanta Media Izquierda
		model = bodyMatrix;
		model = glm::translate(model, pivotMidIzq);
		model = glm::rotate(model, glm::radians(giroMidIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 jointMidIzq = model;

		model = glm::scale(jointMidIzq, glm::vec3(0.3f, 0.3f, 0.3f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = jointMidIzq;
		model = glm::translate(model, -pivotMidIzq);
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		patamidizq_M.RenderModel();

		// Llanta Trasera Derecha
		model = bodyMatrix;
		model = glm::translate(model, pivotBackDer);
		model = glm::rotate(model, glm::radians(giroBackDer), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 jointBackDer = model;

		model = glm::scale(jointBackDer, glm::vec3(0.3f, 0.3f, 0.3f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = jointBackDer;
		model = glm::translate(model, -pivotBackDer);
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		patabackder_M.RenderModel();

		// Llanta Trasera Izquierda
		model = bodyMatrix;
		model = glm::translate(model, pivotBackIzq);
		model = glm::rotate(model, glm::radians(giroBackIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 jointBackIzq = model;

		model = glm::scale(jointBackIzq, glm::vec3(0.3f, 0.3f, 0.3f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();

		model = jointBackIzq;
		model = glm::translate(model, -pivotBackIzq);
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		patabackizq_M.RenderModel();

		// Brazo
		model = bodyMatrix;
		glm::vec3 offsetBrazo = glm::vec3(0.0f, 0.0f, 0.0f);
		model = glm::translate(model, offsetBrazo);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Brazo_M.RenderModel();

		//Pinza
		glm::vec3 posPuntaPinza = glm::vec3(model * glm::vec4(2.1f, 11.97f, -0.94f, 1.0f));
		pointLights[2].SetPos(posPuntaPinza);

		if (mainWindow.getLuzPinzaRover())
		{
			pointLights[2].SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
		}
		else
		{
			pointLights[2].SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
		}

		glm::mat4 modelFoco = glm::mat4(1.0f);
		modelFoco = glm::translate(modelFoco, posPuntaPinza);
		modelFoco = glm::scale(modelFoco, glm::vec3(0.06f, 0.06f, 0.06f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelFoco));
		sp.render();

		// Dado
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-4.0f, 2.5f, 2.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		dadoTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[4]->RenderMeshModel();

		// Holocron
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.0f, 4.5f, 5.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		holocronTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[5]->RenderMeshModel();


		// Nave
		glm::mat4 modelNave = glm::mat4(1.0f);
		modelNave = glm::translate(modelNave, glm::vec3(mainWindow.getmoverAvion() + 8.0f, mainWindow.getsubeyBaja(), 6.0f));
		modelNave = glm::scale(modelNave, glm::vec3(1.0f, 1.0f, 1.0f));

		// Posiciones Motores
		glm::vec3 posMotorIzquierdo = glm::vec3(modelNave * glm::vec4(-0.16f, 0.27f, -1.5f, 1.0f));
		glm::vec3 posMotorDerecho = glm::vec3(modelNave * glm::vec4(0.18f, 0.22f, -1.5f, 1.0f));
		glm::vec3 dirMotores = glm::normalize(glm::vec3(modelNave * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));

		// Posiciones Luces
		pointLights[3].SetPos(posMotorIzquierdo);
		spotLights[5].SetFlash(posMotorDerecho, dirMotores);

		glm::vec3 colorAzul = glm::vec3(0.0f, 0.5f, 1.0f);
		pointLights[3].SetColor(colorAzul);
		spotLights[5].SetColor(colorAzul);

		// Motor Izquierdo (Control
		if (mainWindow.getLuzMotorIzq()) {
			// Encendido
			pointLights[3].SetIntensity(1.0f, 2.0f);
		}
		else {
			// Apagado
			pointLights[3].SetIntensity(0.0f, 0.0f);
		}

		// Motor Derecho Control
		if (mainWindow.getLuzMotorDer()) {
			spotLights[5].SetIntensity(1.0f, 2.0f);
		}
		else {
			spotLights[5].SetIntensity(0.0f, 0.0f);
		}

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelNave));
		NaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Nave_M.RenderModel();

		if (mainWindow.getLuzMotorIzq() || mainWindow.getLuzMotorDer())
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			fuegoAzulTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);

			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));

			// Plasma Izquierdo
			if (mainWindow.getLuzMotorIzq())
			{
				glm::mat4 modelFuegoIzq = glm::mat4(1.0f);
				modelFuegoIzq = glm::translate(modelFuegoIzq, posMotorIzquierdo);
				modelFuegoIzq = glm::scale(modelFuegoIzq, glm::vec3(0.06f, 0.06f, 1.8f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelFuegoIzq));
				sp.render();
			}

			// Plasma Derecho
			if (mainWindow.getLuzMotorDer())
			{
				glm::mat4 modelFuegoDer = glm::mat4(1.0f);
				modelFuegoDer = glm::translate(modelFuegoDer, posMotorDerecho);
				modelFuegoDer = glm::scale(modelFuegoDer, glm::vec3(0.06f, 0.06f, 1.8f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelFuegoDer));
				sp.render();
			}

			glDisable(GL_BLEND);
		}


		//Agave ¿qué sucede si lo renderizan antes del helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[3]->RenderMeshModel();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
