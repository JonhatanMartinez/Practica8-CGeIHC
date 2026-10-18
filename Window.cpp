#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;
	subeyBaja = 2.0f;
	moverAvion = 2.0f; 
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	articulacion3 = 0.0f;
	articulacion4 = 0.0f;
	articulacion5 = 0.0f;
	articulacion6 = 0.0f;
	articulacion7 = 0.0f;
	articulacion10 = 0.0f;
	articulacion11 = 0.0f;
	articulacion12 = 0.0f;
	colorAbajoAvion = 0;
	luzHolocron = true;
	luzPinzaRover = true;
	luzNave = true;
	luzMotorIzq = false;
	luzMotorDer = false;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica 7:Iluminacion 1", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_Y)
	{
		theWindow-> muevex += 1.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow-> muevex -= 1.0;
	}
	if (key == GLFW_KEY_2)
	{
		theWindow->moverAvion -= 1.0f;
	}
	if (key == GLFW_KEY_1)
	{
		theWindow->moverAvion += 1.0f;
	}
	if (key == GLFW_KEY_4)
	{
		theWindow->subeyBaja += 1.0f;
	}
	if (key == GLFW_KEY_3)
	{
		theWindow->subeyBaja -= 1.0f;
	}
	if (key == GLFW_KEY_E)
	{
		theWindow->rotax += 10.0;
	}
	if (key == GLFW_KEY_R)
	{
		theWindow->rotay += 10.0;
	}
	if (key == GLFW_KEY_T)
	{
		theWindow->rotaz += 10.0;
	}
	if (key == GLFW_KEY_F)
	{
		theWindow->articulacion1 += 10.0;
	}

	if (key == GLFW_KEY_G)
	{
		theWindow->articulacion2 += 10.0;
	}
	if (key == GLFW_KEY_H)
	{
		theWindow->articulacion3 += 10.0;
	}
	if (key == GLFW_KEY_J)
	{
		theWindow->articulacion4 += 10.0;
	}
	if (key == GLFW_KEY_K)
	{
		theWindow->articulacion5 += 10.0;
	}
	if (key == GLFW_KEY_L)
	{
		theWindow->articulacion6 += 10.0;
	}

	if (key == GLFW_KEY_L)
	{
		theWindow->articulacion6 += 10.0;
	}

	if (key == GLFW_KEY_Z) {
		theWindow->articulacion10 += 2.0f;
	}
	if (key == GLFW_KEY_X) {
		theWindow->articulacion10 -= 2.0f;
	}
	if (key == GLFW_KEY_C) {
		theWindow->articulacion11 += 2.0f;
	}
	if (key == GLFW_KEY_V) {
		theWindow->articulacion11 -= 2.0f;
	}
	if (key == GLFW_KEY_B) {
		theWindow->articulacion12 += 2.0f;
	}
	if (key == GLFW_KEY_N) {
		theWindow->articulacion12 -= 2.0f;
	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		theWindow->colorAbajoAvion++;
		if (theWindow->colorAbajoAvion > 5) {
			theWindow->colorAbajoAvion = 0;
		}
	}
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		theWindow->luzHolocron = !theWindow->luzHolocron;
	}
	if (key == GLFW_KEY_I && action == GLFW_PRESS)
	{
		theWindow->luzPinzaRover = !theWindow->luzPinzaRover; 
	}
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		theWindow->luzNave = !theWindow->luzNave;
	}

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		theWindow->luzMotorIzq = !theWindow->luzMotorIzq;
	}


	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		theWindow->luzMotorDer = !theWindow->luzMotorDer;
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		//printf("se presiono la tecla: %s\n",key_name);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
