
#ifndef M_PI
#define M_PI 3.14159265358979323846f // Define M_PI if not defined
#endif // !M_PI

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <opengl/shader.h>
using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

vector<float> genStar(float OuterR, float InnerR)
{
    vector<float>vertices;
    int numP = 5;
	float angleS = 2* M_PI / numP;
	vertices.push_back(0.0f); // center
	vertices.push_back(0.0f); // center
	vertices.push_back(0.0f); // center
    for (int i = 0; i < numP * 2; i++)
    {
		float r = (i % 2 == 0) ? OuterR : InnerR;
		float angle = i * angleS - M_PI / 2; // start from the top
		float x = r * cos(angle);
		float y = r * sin(angle);
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(0.0f); // z-coordinate
    }
	return vertices;
}
int main()
{
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    Shader shaderProgram("C:/Users/admin/OneDrive/Desktop/Projects/StarMaking/shaders/shader.vs", "C:/Users/admin/OneDrive/Desktop/Projects/StarMaking/shaders/shader.fs");
    
	vector<float> starVertices = genStar(0.8f, 0.35f);

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, starVertices.size() * sizeof(float), starVertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO
	glBindVertexArray(0); // unbind VAO
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.use();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		float timeValue = glfwGetTime();
		shaderProgram.setFloat("time", timeValue);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, starVertices.size() / 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
    glfwTerminate();
	return 0;
}


