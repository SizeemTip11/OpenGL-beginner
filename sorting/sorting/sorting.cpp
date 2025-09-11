#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <opengl/shader.h>
#include <opengl/camera.h>
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
const size_t numberLines = 100;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path);
void manageVertex(unsigned int& VAO, unsigned int& VBO, vector<float>& vertices);

void configLines(vector<float>& linePos, vector<float>& lineScale);
float length;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Sorting", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader shader("C:/Users/admin/OneDrive/Desktop/Projects/sorting/shaders/shader.vs", "C:/Users/admin/OneDrive/Desktop/Projects/sorting/shaders/shader.fs");

	vector<float> linePos(numberLines);
	vector<float> lineScale(numberLines);
	configLines(linePos, lineScale);

	vector<float>vertices = {
		0.0f, 0.0f, 0.0f,					0.0f, 0.75f, 0.75f,
		0.0f, (float)SCR_HEIGHT, 0.0f,		0.75f, 0.0f, 0.75f,
		length, 0.0f, 0.0f,					0.75f, 0.75f, 0.0f,

		0.0f, (float)SCR_HEIGHT, 0.0f,		0.75f, 0.0f, 0.75f,
		length, (float)SCR_HEIGHT, 0.0f,		0.0f, 0.75f, 0.75f,
		length, 0.0f, 0.0f,					0.75f, 0.75f, 0.0f
	};

	unsigned int VAO, VBO;
	manageVertex(VAO, VBO, vertices);

	while (!glfwWindowShouldClose(window))
	{
		for (size_t i = 0; i + 1 < lineScale.size(); ++i)
		{
			for (size_t j = 0; j + i + 1 < lineScale.size(); ++j)
			{
				processInput(window);

				glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT);
				glm::mat4 view = glm::mat4(1.0f);
				glm::mat4 model = glm::mat4(1.0f);

				shader.use();

				shader.setMat4("projection", projection);
				shader.setMat4("view", view);

				glBindVertexArray(VAO);
				for (size_t k = 0; k < lineScale.size(); ++k)
				{
					model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(linePos[k], 0.0f, 0.0f));
					model = glm::scale(model, glm::vec3(1.0f, lineScale[k], 1.0f));
					shader.setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}

				glfwSwapBuffers(window);
				glfwPollEvents();

				if (lineScale[j] > lineScale[j + 1]) swap(lineScale[j], lineScale[j + 1]);
			}
		}
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
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
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
