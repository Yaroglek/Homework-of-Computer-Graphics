#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "shader.h"
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

int main();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Homework 6 - Lights and Shading", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();
	
	Shader phong("phong.vs", "phong.fs");
	Shader gouraud("gouraud.vs", "gouraud.fs");
	Shader light("light.vs", "light.fs");
	
	float vertices[] = {
		-2.0f, -2.0f, -2.0f, 1.0f, 0.0f, 0.0f,
		2.0f, -2.0f, -2.0f, 1.0f, 0.0f, 0.0f,
		2.0f, 2.0f, -2.0f, 1.0f, 0.0f, 0.0f,
		2.0f, 2.0f, -2.0f, 1.0f, 0.0f, 0.0f,
		-2.0f, 2.0f, -2.0f, 1.0f, 0.0f, 0.0f,
		-2.0f, -2.0f, -2.0f, 1.0f, 0.0f, 0.0f,
	
		-2.0f, -2.0f, 2.0f, 0.0f, 1.0f, 0.0f,
		2.0f, -2.0f, 2.0f, 0.0f, 1.0f, 0.0f,
		2.0f, 2.0f, 2.0f, 0.0f, 1.0f, 0.0f,
		2.0f, 2.0f, 2.0f, 0.0f, 1.0f, 0.0f,
		-2.0f, 2.0f, 2.0f, 0.0f, 1.0f, 0.0f,
		-2.0f, -2.0f, 2.0f, 0.0f, 1.0f, 0.0f,
	
		-2.0f, 2.0f, 2.0f, 0.0f, 0.0f, 1.0f,
		-2.0f, 2.0f, -2.0f, 0.0f, 0.0f, 1.0f,
		-2.0f, -2.0f, -2.0f, 0.0f, 0.0f, 1.0f,
		-2.0f, -2.0f, -2.0f, 0.0f, 0.0f, 1.0f,
		-2.0f, -2.0f, 2.0f, 0.0f, 0.0f, 1.0f,
		-2.0f, 2.0f, 2.0f, 0.0f, 0.0f, 1.0f,
	
		2.0f, 2.0f, 2.0f, 0.0f, 1.0f, 1.0f,
		2.0f, 2.0f, -2.0f, 0.0f, 1.0f, 1.0f,
		2.0f, -2.0f, -2.0f, 0.0f, 1.0f, 1.0f,
		2.0f, -2.0f, -2.0f, 0.0f, 1.0f, 1.0f,
		2.0f, -2.0f, 2.0f, 0.0f, 1.0f, 1.0f,
		2.0f, 2.0f, 2.0f, 0.0f, 1.0f, 1.0f,
	
		-2.0f, -2.0f, -2.0f, 1.0f, 0.0f, 1.0f,
		2.0f, -2.0f, -2.0f, 1.0f, 0.0f, 1.0f,
		2.0f, -2.0f, 2.0f, 1.0f, 0.0f, 1.0f,
		2.0f, -2.0f, 2.0f, 1.0f, 0.0f, 1.0f,
		-2.0f, -2.0f, 2.0f, 1.0f, 0.0f, 1.0f,
		-2.0f, -2.0f, -2.0f, 1.0f, 0.0f, 1.0f,
	
		-2.0f, 2.0f, -2.0f, 1.0f, 1.0f, 0.0f,
		2.0f, 2.0f, -2.0f, 1.0f, 1.0f, 0.0f,
		2.0f, 2.0f, 2.0f, 1.0f, 1.0f, 0.0f,
		2.0f, 2.0f, 2.0f, 1.0f, 1.0f, 0.0f,
		-2.0f, 2.0f, 2.0f, 1.0f, 1.0f, 0.0f,
		-2.0f, 2.0f, -2.0f, 1.0f, 1.0f, 0.0f,
	};
	
	unsigned int cubeVAO, VBO, lightVAO;

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	int flag = 0;
	glm::vec3 lightPosition(32.0f, 32.0f, 32.0f);
	glm::vec3 viewPosition = glm::vec3(16.0f, 16.0f, 16.0f);
	float ambientStrength = 0.1;
	float specularStrength = 0.5;
	float shininess = 32.0;
	float diffuseMultiple = 1.0;

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glfwPollEvents();

		ImGui_ImplGlfwGL3_NewFrame();
		ImGui::RadioButton("Phong", &flag, 0);
		ImGui::RadioButton("Gouraud", &flag, 1);
		ImGui::RadioButton("bonus", &flag, 2);
		ImGui::SliderFloat("ambient", &ambientStrength, 0.0f, 1.0f);
		ImGui::SliderFloat("diffuse", &diffuseMultiple, 0.0f, 1.0f);
		ImGui::SliderFloat("specular", &specularStrength, 0.0f, 1.0f);
		ImGui::SliderFloat("shininess", &shininess, 0.0f, 64.0f);
		ImGui::SliderFloat("light x", &lightPosition.x, -32.0f, 32.0f);
		ImGui::SliderFloat("light y", &lightPosition.y, -32.0f, 32.0f);
		ImGui::SliderFloat("light z", &lightPosition.z, -32.0f, 32.0f);
		ImGui::SliderFloat("view x", &viewPosition.x, -32.0f, 32.0f);
		ImGui::SliderFloat("view y", &viewPosition.y, -32.0f, 32.0f);
		ImGui::SliderFloat("view z", &viewPosition.z, -32.0f, 32.0f);

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		model = glm::rotate(model, 64.0f, glm::vec3(0, 1, 0));
		view = glm::lookAt(viewPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		projection = glm::perspective(32.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 100.0f);
		float radius = 32.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		Shader shader = phong;
		switch (flag) {
		case 0:
			shader = phong;
			break;
		case 1:
			shader = gouraud;
			break;
		case 2:
			shader = phong;
			lightPosition = glm::vec3(camX, lightPosition.y, camZ);
			break;
		default:
			break;
		}
		shader.use();
		shader.setVec3("objectColor", 1.0f, 0.0f, 0.0f);
		shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		shader.setVec3("lightPos", lightPosition.x, lightPosition.y, lightPosition.z);
		shader.setVec3("viewPos", viewPosition.x, viewPosition.y, viewPosition.z);
		shader.setFloat("ambientStrength", ambientStrength);
		shader.setFloat("diffuseMultiple", diffuseMultiple);
		shader.setFloat("specularStrength", specularStrength);
		shader.setFloat("shininess", shininess);
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
	
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		light.use();
		model = glm::mat4();
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::translate(model, lightPosition);
		light.setMat4("model", model);
		light.setMat4("view", view);
		light.setMat4("projection", projection);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}