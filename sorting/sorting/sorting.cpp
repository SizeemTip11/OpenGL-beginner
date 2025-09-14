#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <opengl/shader.h>
#include <opengl/camera.h>
#include <opengl/sorter.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <vector>
#include <map>
#include <array>
#include <chrono>
#include <random>
#include <string>
#include <functional>
#include <stack>
using namespace std;

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;
const size_t numberLines = 1000;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void manageVertex(unsigned int& VAO, unsigned int& VBO, vector<float>& vertices);

void configLines(vector<float>& linePos, vector<float>& lineScale);

float length;
vector<float> linePos(numberLines);
vector<float> lineScale(numberLines);

unsigned int VAO, VBO;
GLFWwindow* window = nullptr;

vector<string>method = {
	"Bubble",
	"Selection",
	"Insertion",
	"Merge",
	"Quick"
};

int num;

int main()
{
	cout << "AVAILABLE METHODS:" << endl;
	for (int i = 1; i <= method.size(); i++)
	{
		cout << i << ". " << method[i - 1] << endl;
	}
	cout << "PREFERRED METHOD (INDEX): ";
	cin >> num;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Sorting", NULL, NULL);
	
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader shader("C:/Users/admin/OneDrive/Desktop/Projects/sorting/shaders/shader.vs", "C:/Users/admin/OneDrive/Desktop/Projects/sorting/shaders/shader.fs");

	
	configLines(linePos, lineScale);

	vector<float>vertices = {
		0.0f, 0.0f, 0.0f,					0.0f, 0.75f, 0.75f,
		0.0f, (float)SCR_HEIGHT, 0.0f,		0.75f, 0.0f, 0.75f,
		length, 0.0f, 0.0f,					0.75f, 0.75f, 0.0f,

		0.0f, (float)SCR_HEIGHT, 0.0f,		0.75f, 0.0f, 0.75f,
		length, (float)SCR_HEIGHT, 0.0f,		0.0f, 0.75f, 0.75f,
		length, 0.0f, 0.0f,					0.75f, 0.75f, 0.0f
	};

	manageVertex(VAO, VBO, vertices);

	Sorter sorter(window, shader, VAO, linePos, lineScale);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		sorter.Sort(method[num - 1]);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
void manageVertex(unsigned int& VAO, unsigned int& VBO, vector<float>& vertices)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}
void configLines(vector<float>& linePos, vector<float>& lineScale)
{
	float space = (float)SCR_WIDTH / linePos.size();
	length = space - 2.0f;
	for (size_t i = 0; i < linePos.size(); ++i)linePos[i] = i * space;

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distInt(0, lineScale.size() - 1);

	float vert = 1.0f / lineScale.size();
	for (size_t i = 0; i < lineScale.size(); ++i) lineScale[i] = 1.0f - distInt(gen) * vert;
}
