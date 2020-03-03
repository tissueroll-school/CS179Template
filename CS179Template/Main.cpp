#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

#include "GLUtils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


bool isFirstMouse = true;
// Struct containing vertex info
struct Vertex
{
	// Position
	float x, y, z;

	// Normal
	float nx, ny, nz;

	// UV coordinates
	float u, v;
};

struct SimpleVertex
{
	// position
	float x, y, z;
};

int main()
{
	// Initialize GLFW
	if (glfwInit() == GLFW_FALSE)
	{
		std::cerr << "Cannot initialize GLFW!" << std::endl;
		return -1;
	}

	// Tell GLFW to use opengl without the deprecated functions (core profile, forward compatible)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Tell GLFW to use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	int windowWidth = 640;
	int windowHeight = 480;

	// Create a GLFW window
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Basic Lighting", nullptr, nullptr);

	// Check window validity
	if (!window)
	{
		std::cerr << "Cannot create window.";
		return -1;
	}

	// Make the current window as the current context for OpenGL
	glfwMakeContextCurrent(window);

	// Load OpenGL extensions via GLAD
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// Vertices of the cube.
	// Convention for each face: lower-left, lower-right, upper-right, upper-left
	Vertex cubeVertices[] =
	{
		// Front
		{ -1.0f, -1.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f },
		{ 1.0f, -1.0f, 1.0f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f },

		// Back
		{ 1.0f, -1.0f, -1.0f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f },
		{ -1.0f, -1.0f, -1.0f,	0.0f, 0.0f, -1.0f,	1.0f, 0.0f },
		{ -1.0f, 1.0f, -1.0f,	0.0f, 0.0f, -1.0f,	1.0f, 1.0f },
		{ 1.0f, 1.0f, -1.0f,	0.0f, 0.0f, -1.0f,	0.0f, 1.0f },

		// Left
		{ -1.0f, -1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,	0.0f, 0.0f },
		{ -1.0f, -1.0f, 1.0f,	-1.0f, 0.0f, 0.0f,	1.0f, 0.0f },
		{ -1.0f, 1.0f, 1.0f,	-1.0f, 0.0f, 0.0f,	1.0f, 1.0f },
		{ -1.0f, 1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,	0.0f, 1.0f },

		// Right
		{ 1.0f, -1.0f, 1.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f },
		{ 1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f },
		{ 1.0f, 1.0f, -1.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f },

		// Top
		{ -1.0f, 1.0f, 1.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f },
		{ 1.0f, 1.0f, -1.0f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f },
		{ -1.0f, 1.0f, -1.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f },

		// Bottom
		{ -1.0f, -1.0f, -1.0f,	0.0f, -1.0f, 0.0f,	0.0f, 0.0f },
		{ 1.0f, -1.0f, -1.0f,	0.0f, -1.0f, 0.0f,	1.0f, 0.0f },
		{ 1.0f, -1.0f, 1.0f,	0.0f, -1.0f, 0.0f,	1.0f, 1.0f },
		{ -1.0f, -1.0f, 1.0f,	0.0f, -1.0f, 0.0f,	0.0f, 1.0f }
	};

	SimpleVertex simpleCubeVertices[] =
	{
		// Front
		{ -1.0f, -1.0f, 1.0f},
		{ 1.0f, -1.0f, 1.0f},
		{ 1.0f, 1.0f, 1.0f},
		{ -1.0f, 1.0f, 1.0f},

		// Back
		{ 1.0f, -1.0f, -1.0f},
		{ -1.0f, -1.0f, -1.0f},
		{ -1.0f, 1.0f, -1.0f},
		{ 1.0f, 1.0f, -1.0f},

		// Left
		{ -1.0f, -1.0f, -1.0f},
		{ -1.0f, -1.0f, 1.0f},
		{ -1.0f, 1.0f, 1.0f},
		{ -1.0f, 1.0f, -1.0f},

		// Right
		{ 1.0f, -1.0f, 1.0f},
		{ 1.0f, -1.0f, -1.0f},
		{ 1.0f, 1.0f, -1.0f},
		{ 1.0f, 1.0f, 1.0f},

		// Top
		{ -1.0f, 1.0f, 1.0f},
		{ 1.0f, 1.0f, 1.0f},
		{ 1.0f, 1.0f, -1.0f},
		{ -1.0f, 1.0f, -1.0f},

		// Bottom
		{ -1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f, 1.0f},
		{ -1.0f, -1.0f, 1.0f}
	};

	// Vertex indices for the cube
	unsigned int cubeIndices[] =
	{
		// Front
		0, 1, 2, 2, 3, 0,

		// Back
		4, 5, 6, 6, 7, 4,

		// Left
		8, 9, 10, 10, 11, 8,

		// Right
		12, 13, 14, 14, 15, 12,

		// Top
		16, 17, 18, 18, 19, 16,

		// Bottom
		20, 21, 22, 22, 23, 20
	};

	// Enable depth testing to handle occlusion
	glEnable(GL_DEPTH_TEST);

	// Construct VBO for the cube
	GLuint cubeVbo;
	glGenBuffers(1, &cubeVbo);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	// Construct EBO (Element Buffer Object) for the cube
	GLuint cubeEbo;
	glGenBuffers(1, &cubeEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

	// Construct VAO for the cube
	GLuint cubeVao;
	glGenVertexArrays(1, &cubeVao);
	glBindVertexArray(cubeVao);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEbo);

	// Vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nx));

	// UV coordinates attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));

	// Create texture handle for the cube's diffuse map
	GLuint cubeDiffuseTex;
	glGenTextures(1, &cubeDiffuseTex);

	// Bind our diffuse texture
	glBindTexture(GL_TEXTURE_2D, cubeDiffuseTex);

	// Set up the parameters for our diffuse texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Read the data for our diffuse map
	int diffuseWidth, diffuseHeight, diffuseNumChannels;
	unsigned char* diffuseData = stbi_load("container-diffuse.png", &diffuseWidth, &diffuseHeight, &diffuseNumChannels, 0);

	// Upload the diffuse map data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, diffuseWidth, diffuseHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, diffuseData);

	// Since we already uploaded the diffuse map data to opengl, we don't need it anymore.
	stbi_image_free(diffuseData);
	diffuseData = nullptr;

	// Create texture handle for the cube's specular map
	GLuint cubeSpecularTex;
	glGenTextures(1, &cubeSpecularTex);

	// Bind our specular texture
	glBindTexture(GL_TEXTURE_2D, cubeSpecularTex);

	// Set up the parameters for our specular texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Read the data for our specular map
	int specularWidth, specularHeight, specularNumChannels;
	unsigned char* specularData = stbi_load("container-specular.png", &specularWidth, &specularHeight, &specularNumChannels, 0);

	// Upload the specular map data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, specularWidth, specularHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, specularData);

	// Since we already uploaded the specular map data to opengl, we don't need it anymore.
	stbi_image_free(specularData);
	specularData = nullptr;

	// skybox
	// Construct VBO for the cube
	GLuint skyboxVbo;
	glGenBuffers(1, &skyboxVbo);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(simpleCubeVertices), simpleCubeVertices, GL_STATIC_DRAW);

	// Construct EBO (Element Buffer Object) for the cube
	GLuint skyboxEbo;
	glGenBuffers(1, &skyboxEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

	// Construct VAO for the cube
	GLuint skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);

	glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEbo);

	// Vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), 0);

	/*
	// Vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nx));

	// UV coordinates attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
	*/

	GLuint cubeMapTex;
	glGenTextures(1, &cubeMapTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTex);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	int width, height, nChannels;
	unsigned char* data = stbi_load("right.jpg", &width, &height, &nChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	data = stbi_load("left.jpg", &width, &height, &nChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	data = stbi_load("top.jpg", &width, &height, &nChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	data = stbi_load("bottom.jpg", &width, &height, &nChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	data = stbi_load("back.jpg", &width, &height, &nChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	data = stbi_load("front.jpg", &width, &height, &nChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	// Construct VAO for the light source
	GLuint lightVao;
	glGenVertexArrays(1, &lightVao);
	glBindVertexArray(lightVao);

	// Since the light source is also a cube, we can reuse the VBO and EBO
	glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEbo);

	// Vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Create shader program for the light source
	GLuint lightProgram = CreateShaderProgram("Basic.vsh", "Basic.fsh");

	// Create shader program for the cube
	GLuint cubeProgram = CreateShaderProgram("BasicLighting.vsh", "BasicLighting.fsh");

	GLuint skyboxProgram = CreateShaderProgram("skybox.vsh", "skybox.fsh");

	// Construct the projection matrix
	glm::mat4 projMatrix = glm::perspective(glm::radians(45.0f), windowWidth * 1.0f / windowHeight, 0.1f, 100.0f);

	// Camera parameters
	glm::vec3 eyePosition = glm::vec3(0.0f, 0.0f, 0.0f);
	float cameraPitch = 0.0f;
	float cameraYaw = -90.0f;
	float movementSpeed = 10.0f; // 10 distance units per second
	float lookSpeed = 45.0f; // 45 degrees per second

	// Light-related parameters
	glm::vec3 spotLightPosition(0.0f, 0.0f, 0.0f);

	// Cube positions
	std::vector<glm::vec3> cubePositions;
	cubePositions.push_back(glm::vec3(2.0f, 5.0f, -15.0f));
	cubePositions.push_back(glm::vec3(-1.5f, -2.2f, -2.5f));
	cubePositions.push_back(glm::vec3(-3.8f, -2.0f, -12.3f));
	cubePositions.push_back(glm::vec3(2.4f, -0.4f, -3.5f));
	cubePositions.push_back(glm::vec3(-1.7f, 3.0f, -7.5f));
	cubePositions.push_back(glm::vec3(1.3f, -2.0f, -2.5f));
	cubePositions.push_back(glm::vec3(1.5f, 2.0f, -2.5f));
	cubePositions.push_back(glm::vec3(1.5f, 0.2f, -1.5f));
	cubePositions.push_back(glm::vec3(-1.3f, 1.0f, -1.5f));

	double prevTime = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		// Calculate amount of time passed since the last frame
		float deltaTime = glfwGetTime() - prevTime;
		prevTime = glfwGetTime();

		// Set background color to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind the vao of the cube
		glBindVertexArray(cubeVao);

		// Use the shader for the cube
		glUseProgram(cubeProgram);

		// Handle camera look input (up/down)
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			cameraPitch += lookSpeed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			cameraPitch -= lookSpeed * deltaTime;
		}
		cameraPitch = glm::clamp(cameraPitch, -89.0f, 89.0f);

		// Handle camera look input (left/right)
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			cameraYaw -= lookSpeed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			cameraYaw += lookSpeed * deltaTime;
		}

		// Calculate the camera's look direction based on the
		// camera's pitch and yaw using spherical coordinates
		glm::vec3 lookDir(0.0f);
		lookDir.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
		lookDir.y = sin(glm::radians(cameraPitch));
		lookDir.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));

		// Calculate the right facing vector relative to the camera
		// by taking the cross product between the camera's look direction and the global up vector
		glm::vec3 rightVec = glm::cross(lookDir, glm::vec3(0.0f, 1.0f, 0.0f));

		// Handle camera movement
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			eyePosition -= rightVec * movementSpeed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			eyePosition += rightVec * movementSpeed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			eyePosition += lookDir * movementSpeed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			eyePosition -= lookDir * movementSpeed * deltaTime;
		}

		/*
		// Pass the eye position vector to the current shader that we're using
		glUniform3f(glGetUniformLocation(cubeProgram, "eyePos"), eyePosition.x, eyePosition.y, eyePosition.z);

		// Pass directional light parameters to the shader
		glUniform3fv(glGetUniformLocation(cubeProgram, "dirLight.direction"), 1, glm::value_ptr(glm::vec3(0.0f, -1.0f, 0.0f)));
		glUniform3fv(glGetUniformLocation(cubeProgram, "dirLight.ambient"), 1, glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.05f)));
		glUniform3fv(glGetUniformLocation(cubeProgram, "dirLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		glUniform3fv(glGetUniformLocation(cubeProgram, "dirLight.specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

		// Pass point light parameters to the shader
		glUniform3fv(glGetUniformLocation(cubeProgram, "pointLight.position"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
		glUniform3fv(glGetUniformLocation(cubeProgram, "pointLight.ambient"), 1, glm::value_ptr(glm::vec3(0.01f, 0.01f, 0.01f)));
		glUniform3fv(glGetUniformLocation(cubeProgram, "pointLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		glUniform3fv(glGetUniformLocation(cubeProgram, "pointLight.specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		glUniform1f(glGetUniformLocation(cubeProgram, "pointLight.kConstant"), 1.0f);
		glUniform1f(glGetUniformLocation(cubeProgram, "pointLight.kLinear"), 0.09f);
		glUniform1f(glGetUniformLocation(cubeProgram, "pointLight.kQuadratic"), 0.032f);

		// Pass spot light parameters to the shader
		// We pass the camera position and direction as the spot light position and direction respectively
		// to emulate a flash light
		glUniform3fv(glGetUniformLocation(cubeProgram, "spotLight.position"), 1, glm::value_ptr(eyePosition));
		glUniform3fv(glGetUniformLocation(cubeProgram, "spotLight.direction"), 1, glm::value_ptr(lookDir));
		glUniform3fv(glGetUniformLocation(cubeProgram, "spotLight.ambient"), 1, glm::value_ptr(glm::vec3(0.1f, 0.1f, 0.1f)));
		glUniform3fv(glGetUniformLocation(cubeProgram, "spotLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		glUniform3fv(glGetUniformLocation(cubeProgram, "spotLight.specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		glUniform1f(glGetUniformLocation(cubeProgram, "spotLight.kConstant"), 1.0f);
		glUniform1f(glGetUniformLocation(cubeProgram, "spotLight.kLinear"), 0.09f);
		glUniform1f(glGetUniformLocation(cubeProgram, "spotLight.kQuadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(cubeProgram, "spotLight.cutOffAngle"), glm::radians(12.5f));

		// Pass the projection matrix to the shader
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "projMatrix"), 1, GL_FALSE, glm::value_ptr(projMatrix));
		*/
		
		// Construct the view matrix, and pass the view matrix to the shader
		glm::mat4 viewMatrix = glm::lookAt(eyePosition, eyePosition + lookDir, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
		/*
		// Render the cubes
		for (int i = 0; i < cubePositions.size(); ++i)
		{
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, cubePositions[i]);

			float angle = 20.0f * i;
			modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));

			// Set the active texture unit to 0, and
			// bind the diffuse map texture to it
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, cubeDiffuseTex);

			// Set the active texture unit to 1, and
			// bind the specular map texture to it
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, cubeSpecularTex);

			// Tell the shader that the diffuse map texture is texture unit 0
			glUniform1i(glGetUniformLocation(cubeProgram, "diffuseTex"), 0);

			// Tell the shader that the specular map texture is texture unit 1
			glUniform1i(glGetUniformLocation(cubeProgram, "specularTex"), 1);

			glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}

		// --- Render a cube where the point light is for visualization purposes

		// Switch to the shader for the light source
		glUseProgram(lightProgram);

		// Bind the light source VAO
		glBindVertexArray(lightVao);

		// Initialize the light model matrix
		glm::mat4 lightModelMatrix = glm::mat4(1.0f);
		lightModelMatrix = glm::translate(lightModelMatrix, spotLightPosition);
		lightModelMatrix = glm::scale(lightModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));

		// Pass the projection matrix to the shader
		glUniformMatrix4fv(glGetUniformLocation(lightProgram, "projMatrix"), 1, GL_FALSE, glm::value_ptr(projMatrix));

		// Pass the view matrix to the shader
		glUniformMatrix4fv(glGetUniformLocation(lightProgram, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

		// Pass the model matrix to the shader
		glUniformMatrix4fv(glGetUniformLocation(lightProgram, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(lightModelMatrix));

		// Pass the color of the light source to the shader
		glUniform3fv(glGetUniformLocation(lightProgram, "color"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

		// Draw the cube for the light source
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		*/
		// skybox
		glUseProgram(skyboxProgram);
		glBindVertexArray(skyboxVAO);

		// Pass the projection matrix to the shader
		glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "projMatrix"), 1, GL_FALSE, glm::value_ptr(projMatrix));

		// Pass the view matrix to the shader
		glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glDepthMask(GL_FALSE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTex);
		// draw skybox
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glDepthMask(GL_TRUE);

		// Swap the front and back buffers
		glfwSwapBuffers(window);

		// Poll pending events
		glfwPollEvents();
	}

	// Terminate GLFW
	glfwTerminate();

	return 0;
}
