#include <iostream>
#include <stdexcept>
#include <cmath>
#include <climits>


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Shader.hpp"
#include "Program.hpp"
#include "TextureLoader.hpp"

// INIT GL
const unsigned GL_BASE_VERSION = 4;
const unsigned GL_SUB_VERSION = 1;
const unsigned WINDOW_WIDTH = 1280;
const unsigned WINDOW_HEIGHT = 720;
const char * WINDOW_NAME = "Assignment 6";
const unsigned SAMPLES = 8;
GLFWwindow * window = nullptr;

// Configuring Window Specifications + Camera rotations.
const float CLOSE_PLANE = 0.1f;
const float BACK_PLANE = 500.0f;
const float FOV = 45.0f;
double deltaTime = 0.0;
bool rotateLeft = false;
bool rotateRight = false;

// Used in Tessalations + Textures.
const unsigned NUM_TEXTURES = 5;
const unsigned TESSLVL = 1;
const float AMPLITUDE = 0.11f;

unsigned heightMap[NUM_TEXTURES];
unsigned waterTex;
unsigned boatTex;
unsigned eyesTex;
unsigned headTex;
float interpFactor = 0.0f;

// Init program, projection matrix and VAO
Program program;
glm::mat4 projection;
unsigned VAO = 0;


void initGL();
void configureWindow(int width, int height);
void keyboardInput(GLFWwindow * window);
void renderMesh();

// Main Program, loads in textures, configures MVP
// Handles creating program inputs for shaders
// Creates wave specifications as described in assignment, stores it in a wave struct.
// Implements rotation of camera given key input
// Clear resources.
int main()
{
    initGL();
    glGenVertexArrays(1, &VAO);
    glGenTextures(NUM_TEXTURES, heightMap);
    glGenTextures(1, &waterTex);

    TextureLoader::loadTexture(heightMap[0], "build/textures/displacement-map1.bmp");

    std::cout << heightMap;

    TextureLoader::loadTexture(waterTex, "build/textures/waterBMP.bmp");
    /*
    TextureLoader::loadTexture(boatTex, "build/textures/boatBMP.bmp");
    TextureLoader::loadTexture(eyesTex, "build/textures/eyes.bmp");
    TextureLoader::loadTexture(headTex, "build/textures/head.bmp");
    */


    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(100.0f));
    glm::vec3 viewPos = glm::vec3(1.0f);
    glm::mat4 view = glm::lookAt(
      viewPos,
      glm::vec3(0.0f, 0.0f,  0.0f),
      glm::vec3(0.0f, 1.0f,  0.0f)
    );

    program.use();
    program.setVec3("light.direction", glm::vec3(0.0, -1.0, 0.0));
    program.setVec3("light.ambient", glm::vec3(0.13, 0.13, 0.13));
    program.setVec3("light.diffuse", glm::vec3(0.70, 0.70, 0.70));
    program.setVec3("light.specular", glm::vec3(2.0, 2.0, 2.0));
    program.setFloat("interpolateFactor", interpFactor);
    program.setFloat("depth", AMPLITUDE);
    program.setInt("tessLevel", TESSLVL);

    // Add wave parameters and loop to set uniform values
    struct Wave {
        float w;
        glm::vec2 D;
        float A;
        float phi;
        float Q;
    };

    Wave waves[4] = {
        {4.0f, glm::vec2(0.3f, 0.6f), 0.08f, 1.1f, 0.75f},
        {2.0f, glm::vec2(0.2f, 0.866f), 0.05f, 1.1f, 0.75f},
        {0.6f, glm::vec2(0.3f, 0.7f), 0.2f, 0.4f, 0.1f},
        {0.9f, glm::vec2(0.8f, 0.1f), 0.15f, 0.4f, 0.1f}
    };
    program.use();
    program.setFloat("time", static_cast<float>(glfwGetTime()));
   for (int i = 0; i < 4; ++i) {
        std::string number = std::to_string(i);
        program.setFloat(("wave" + number + ".w").c_str(), waves[i].w);
        program.setVec2(("wave" + number + ".D").c_str(), waves[i].D);
        program.setFloat(("wave" + number + ".A").c_str(), waves[i].A);
        program.setFloat(("wave" + number + ".phi").c_str(), waves[i].phi);
        program.setFloat(("wave" + number + ".Q").c_str(), waves[i].Q);
    }


    double currentFrame = 0;
    double prevFrame = 0;
    float radius = 75.0f;

    float camera_x = sin(glfwGetTime()*0.5) * radius,
          camera_z = cos(glfwGetTime()*0.5) * radius;
    viewPos = glm::vec3(camera_x, 50.0f, camera_z);

    while (!glfwWindowShouldClose(window))
    {
      keyboardInput(window);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glClearColor(0, 0, 0, 1);

      currentFrame = glfwGetTime();
      deltaTime = currentFrame - prevFrame;
      prevFrame = currentFrame;

      if (rotateLeft)
      {
        radius = 60.0f;
        float camera_x = sin(glfwGetTime()*0.5) * radius,
              camera_z = cos(glfwGetTime()*0.5) * radius;
        viewPos = glm::vec3(camera_x, 30.0f, camera_z);
      }
      else if (rotateRight)
      {
           radius = -60.0f;
        float camera_x = sin(glfwGetTime()*0.5) * radius,
              camera_z = cos(glfwGetTime()*0.5) * radius;
        viewPos = glm::vec3(camera_x, 30.0f, camera_z);
      }

      view = glm::lookAt(
        viewPos,
        glm::vec3(0.0f, 0.0f,  0.0f),
        glm::vec3(0.0f, 1.0f,  0.0f)
      );

      program.use();
      program.setMat4("model", model);
      program.setMat4("mvp", projection * view * model);
      program.setVec3("viewPos", viewPos);
      renderMesh();

      glfwPollEvents();
      glfwSwapBuffers(window);
    }
  glDeleteVertexArrays(1, &VAO);
  glDeleteTextures(NUM_TEXTURES, heightMap);
  glDeleteTextures(1, &waterTex);

  glfwTerminate();
  return 0;
}

// Standard initalization of GL window, and initailzing the shaders.
void initGL()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_BASE_VERSION);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_SUB_VERSION);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, SAMPLES);

  if (!(window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr)))
    throw std::runtime_error("Error Creating Window.\n");
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
    throw std::runtime_error("OpenGL hasn't been initilized.\n");
  configureWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);

  // Initialize Program
  program.create();
  program.attachShader(Shader::createVertexShader("build/shaders/vert-shader.vs"));
  program.attachShader(Shader::createTessalationControlShader("build/shaders/tessellation-control.glsl"));
  program.attachShader(Shader::createTessalationEvaluationShader("build/shaders/tess-evaluation.glsl"));
  program.attachShader(Shader::createFragmentShader("build/shaders/frag-shader.fs"));
  program.link();
}


// Window Configurations
void configureWindow(int width, int height)
{
  projection = glm::perspective(glm::radians(FOV), (float)width/(float)height, CLOSE_PLANE, BACK_PLANE);
  glViewport(0, 0, width, height);
}

// User input to rotate the plane left or right
void keyboardInput(GLFWwindow * window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
  else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        rotateRight = true;
        rotateLeft = false;
  }
  else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        rotateLeft = true;
        rotateRight = false;
  }
}


// Activates shader program, sets uniform value for heightmap, water, and waveoffset
// Binds VAO which contains water mesh data
// Sets number of verticies per patch for tessellation
// Draws the water mesh in 64 x 64 grid - 4 verticies at a time
void renderMesh()
{
  program.use();
  program.setInt("heightMap1", 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, heightMap[0]);

  program.setInt("water", 4);
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, waterTex);

  static float offset = 0.0f;
  if (offset >= INT_MAX-2)
    offset = 0;
  offset += 0.2 * deltaTime;
  program.setFloat("wavesOffset", offset);

  glBindVertexArray(VAO);
  glPatchParameteri(GL_PATCH_VERTICES, 4);
  glDrawArraysInstanced(GL_PATCHES, 0, 4, 64*64);
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

