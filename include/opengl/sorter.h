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
#include <thread>
#include <random>
#include <string>
#include <functional>
#include <stack>
using namespace std;

enum method {
	Bubble,
	Merge,
	Selection,
	Insertion
};

glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
glm::mat4 view = glm::mat4(1.0f);

class Sorter
{
	GLFWwindow* window;
	Shader shader;

	unsigned int VAO;

	vector<float>linePos;
	vector<float>lineScale;

public:
	Sorter(GLFWwindow* window, Shader& shader, unsigned int& VAO, vector<float>& linePos, vector<float>& lineScale)
	{
		this->window = window;
		this->shader = shader;

		this->VAO = VAO;
		this->linePos = linePos;
		this->lineScale = lineScale;
	}

	void Sort(string m)
	{
		if (m == "Bubble") this->BubbleSort();
		else if (m == "Merge") this->MergeSort();
		else if (m == "Selection") this->SelectionSort();
		else if (m == "Insertion") this->InsertionSort();
		else if (m == "Quick") this->QuickSort();
		else cout << "ERROR::SORTING METHOD NOT FOUND";
		//glfwSetWindowShouldClose(window, true);
	}

private:
	void renderLine()
	{
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
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}

	void render()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		this->renderLine();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void BubbleSort()
	{
		for (int i = 0; i + 1 < lineScale.size(); i++)
		{
			this->render();
			for (int j = 0; i + j + 1 < lineScale.size(); j++)
			{
				if (lineScale[j] > lineScale[j + 1])swap(lineScale[j], lineScale[j + 1]);
			}
		}
	}

	void merge(Shader shader, size_t left, size_t mid, size_t right)
	{
		size_t n1 = mid - left + 1;
		size_t n2 = right - mid;

		vector<float> L(n1), R(n2);

		for (size_t i = 0; i < n1; i++)
		{
			L[i] = lineScale[left + i];
		}
		for (size_t i = 0; i < n2; i++)
		{
			R[i] = lineScale[mid + i + 1];
		}

		size_t i = 0, j = 0, k = left;
		while (i < n1 && j < n2)
		{
			this->render();
			this_thread::sleep_for(chrono::microseconds(1));
			if (L[i] <= R[j])
				lineScale[k++] = L[i++];
			else
				lineScale[k++] = R[j++];
		}

		while (i < n1)
			lineScale[k++] = L[i++];
		while (j < n2)
			lineScale[k++] = R[j++];

	}

	void MergeSort()
	{
		size_t n = lineScale.size();

		for (size_t i = 1; i <= n - 1; i *= 2)
		{
			for (size_t left = 0; left < n - 1; left += 2 * i)
			{
				size_t mid = min(left + i - 1, n - 1);
				size_t right = min(left + 2 * i - 1, n - 1);

				this->merge(shader, left, mid, right);
			}
		}
	}

	void SelectionSort()
	{
		size_t n = lineScale.size();

		for (size_t i = 0; i < n; ++i)
		{
			this->render();

			size_t mi = i;

			for (size_t j = i + 1; j < n; ++j)
			{

				if (lineScale[j] < lineScale[mi])
				{
					mi = j;
				}
			}
			swap(lineScale[i], lineScale[mi]);
		}
	}

	void InsertionSort()
	{
		size_t n = lineScale.size();

		for (size_t i = 0; i + 1 < n; ++i)
		{
			this->render();
			for (size_t j = i + 1; j < n; ++j)
			{
				if (lineScale[j] < lineScale[i])swap(lineScale[j], lineScale[i]);
			}
		}
	}

	int partition(int low, int high)
	{
		float pivot = lineScale[high];
		int i = low - 1;

		for (int j = low; j <= high - 1; ++j)
		{
			if (lineScale[j] < pivot)
			{
				++i;
				swap(lineScale[i], lineScale[j]);
			}
		}

		swap(lineScale[i + 1], lineScale[high]);
		return i + 1;
	}

	void QuickSort()
	{
		int n = (int)lineScale.size();
		stack<pair<int, int>> st;

		st.push({ 0, n - 1 });

		while (!st.empty())
		{
			this->render();
			this_thread::sleep_for(chrono::milliseconds(1));
			if (glfwWindowShouldClose(window)) return;

			int low = st.top().first;
			int high = st.top().second;
			st.pop();

			if (low >= high) continue; 

			int pivot = partition(low, high);

			if (pivot - 1 >= low)
				st.push({ low, pivot - 1 });
			if (pivot + 1 <= high)
				st.push({ pivot + 1, high });
		}
	}

};
	
