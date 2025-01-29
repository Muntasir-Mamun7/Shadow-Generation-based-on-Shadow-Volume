#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Camera.h"
#include <vector>

#define ERROR_LOG(Message) std::cout << Message << std::endl; glfwTerminate(); return -1;
std::vector<glm::vec3> vertices_cube_vec;  // Add this line
Shader* shadowVolumeShader = nullptr;
Shader* shadowOverlayShader = nullptr;
// Global variables
Camera camera(glm::vec3(0.0f, 0.0f, 0.3f));
GLfloat currentTime;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
int screenWidth, screenHeight;
bool keys[1024];
glm::vec3 lightPos(5.0f, 10.0f, 0.0f); // Adjust these values as needed
// Shader pointers
Shader* cubeshader = nullptr;
Shader* lightshader = nullptr;


// VAOs and VBOs
static GLuint VAO_cube, VBO_cube, VAO_surface, VBO_surface;
GLuint shadowVolumeVAO = 0;
GLuint shadowVolumeVBO = 0;

// Function declarations
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void ScollCallback(GLFWwindow* window, double xPoxOffset, double yOffset);
void ProcessKeyboard();
void SetViewport(GLFWwindow* window);
void CreateVertexBuffer();
void InitializeShadowVolumeBuffers();
void CreateShadowVolume(const glm::vec3& lightPos, const std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& shadowVolume);
void RenderShadowVolume(const std::vector<glm::vec3>& shadowVolume);
void RenderScreen(GLFWwindow* window);
void RenderShadowOverlay();

// Vertex data
float vertices_cube[] = {
    // Cube vertices - positions and colors
    // Front face (red)
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    // Back face (green)
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    // Left face (blue)
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    // Right face (yellow)
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
     // Top face (magenta)
     -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
     -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
     // Bottom face (cyan)
     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
};

float vertices_surface[] = {
    // Surface vertices (gray)
    -5.0f, -1.0f,  5.0f,  0.5f, 0.5f, 0.5f,
     5.0f, -1.0f,  5.0f,  0.5f, 0.5f, 0.5f,
     5.0f, -1.0f, -5.0f,  0.5f, 0.5f, 0.5f,
     5.0f, -1.0f, -5.0f,  0.5f, 0.5f, 0.5f,
    -5.0f, -1.0f, -5.0f,  0.5f, 0.5f, 0.5f,
    -5.0f, -1.0f,  5.0f,  0.5f, 0.5f, 0.5f
};

void InitializeShadowVolumeBuffers() {
    glGenVertexArrays(1, &shadowVolumeVAO);
    glGenBuffers(1, &shadowVolumeVBO);

    glBindVertexArray(shadowVolumeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, shadowVolumeVBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);
}

void CreateVertexBuffer() {
    // Cube setup
    glGenVertexArrays(1, &VAO_cube);
    glGenBuffers(1, &VBO_cube);
    glBindVertexArray(VAO_cube);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_cube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cube), vertices_cube, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Surface setup
    glGenVertexArrays(1, &VAO_surface);
    glGenBuffers(1, &VBO_surface);
    glBindVertexArray(VAO_surface);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_surface);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_surface), vertices_surface, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CreateShadowVolume(const glm::vec3& lightPos, const std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& shadowVolume) {
    shadowVolume.clear();

    // Adjust extrusion length based on scene size and light position
    float sceneSize = 30.0f;
    float extrudeLength = glm::length(lightPos) + sceneSize;

    // Process each triangle (face)
    for (size_t i = 0; i < vertices.size(); i += 3) {
        glm::vec3 v0 = vertices[i];
        glm::vec3 v1 = vertices[i + 1];
        glm::vec3 v2 = vertices[i + 2];

        // Calculate face normal
        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

        // Calculate center point of the triangle
        glm::vec3 faceCenter = (v0 + v1 + v2) / 3.0f;

        // Vector from face center to light
        glm::vec3 toLight = glm::normalize(lightPos - faceCenter);

        // If face is potentially visible from light source
        if (glm::dot(normal, toLight) < 0.0f) {
            // Calculate light directions for each vertex
            glm::vec3 v0ToLight = glm::normalize(v0 - lightPos);
            glm::vec3 v1ToLight = glm::normalize(v1 - lightPos);
            glm::vec3 v2ToLight = glm::normalize(v2 - lightPos);

            // Calculate extruded positions
            glm::vec3 v0Extruded = v0 + v0ToLight * extrudeLength;
            glm::vec3 v1Extruded = v1 + v1ToLight * extrudeLength;
            glm::vec3 v2Extruded = v2 + v2ToLight * extrudeLength;

            // Front cap (original triangle)
            shadowVolume.push_back(v0);
            shadowVolume.push_back(v1);
            shadowVolume.push_back(v2);

            // Back cap (extruded triangle, winding order reversed)
            shadowVolume.push_back(v0Extruded);
            shadowVolume.push_back(v2Extruded);
            shadowVolume.push_back(v1Extruded);

            // Side quads
            // Side 1
            shadowVolume.push_back(v0);
            shadowVolume.push_back(v0Extruded);
            shadowVolume.push_back(v1);
            shadowVolume.push_back(v1);
            shadowVolume.push_back(v0Extruded);
            shadowVolume.push_back(v1Extruded);

            // Side 2
            shadowVolume.push_back(v1);
            shadowVolume.push_back(v1Extruded);
            shadowVolume.push_back(v2);
            shadowVolume.push_back(v2);
            shadowVolume.push_back(v1Extruded);
            shadowVolume.push_back(v2Extruded);

            // Side 3
            shadowVolume.push_back(v2);
            shadowVolume.push_back(v2Extruded);
            shadowVolume.push_back(v0);
            shadowVolume.push_back(v0);
            shadowVolume.push_back(v2Extruded);
            shadowVolume.push_back(v0Extruded);
        }
    }
}
void RenderFullScreenQuad() {
    static GLuint quadVAO = 0;
    static GLuint quadVBO;

    if (quadVAO == 0) {
        float quadVertices[] = {
            // positions
            -1.0f,  1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,

            -1.0f,  1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,
        };

        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    }

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
void RenderShadowVolume(const std::vector<glm::vec3>& shadowVolume) {
    if (shadowVolume.empty()) return;

    glBindVertexArray(shadowVolumeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, shadowVolumeVBO);
    glBufferData(GL_ARRAY_BUFFER, shadowVolume.size() * sizeof(glm::vec3), shadowVolume.data(), GL_DYNAMIC_DRAW);

    // Set up stencil test
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);

    // Disable color and depth writing
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // First pass - increment stencil for front faces
    glCullFace(GL_BACK);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR_WRAP);
    glDrawArrays(GL_TRIANGLES, 0, shadowVolume.size());

    // Second pass - decrement stencil for back faces
    glCullFace(GL_FRONT);
    glStencilOp(GL_KEEP, GL_KEEP, GL_DECR_WRAP);
    glDrawArrays(GL_TRIANGLES, 0, shadowVolume.size());

    // Restore state
    glDisable(GL_CULL_FACE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);

    glBindVertexArray(0);
}

void SetViewport(GLFWwindow* window) {
    // Get the current window size
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    // Set the viewport to match the window size
    glViewport(0, 0, screenWidth, screenHeight);
}
void RenderScreen(GLFWwindow* window, Shader& cubeshader) {
    // Clear all buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // 1. First render pass - render scene normally
    cubeshader.UseProgram();
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(camera.GetZoom(),
        static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
        0.1f, 100.0f);

    cubeshader.SetMatrix4fv("view", view);
    cubeshader.SetMatrix4fv("projection", projection);
    cubeshader.SetMatrix4fv("transform", glm::mat4(1.0f));
    cubeshader.SetVec3("LightPos", lightPos);
    cubeshader.SetVec3("viewPos", camera.GetPosition());

    // Draw scene objects normally
    glBindVertexArray(VAO_cube);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(VAO_surface);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // 2. Shadow volume pass
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    // Modified stencil operations
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glStencilMask(0xFF);

    // First pass - front faces
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR_WRAP);

    shadowVolumeShader->UseProgram();
    shadowVolumeShader->SetMatrix4fv("view", view);
    shadowVolumeShader->SetMatrix4fv("projection", projection);
    shadowVolumeShader->SetMatrix4fv("model", glm::mat4(1.0f));

    // Clear and populate vertices_cube_vec
    vertices_cube_vec.clear();
    for (size_t i = 0; i < sizeof(vertices_cube) / (6 * sizeof(float)); ++i) {
        vertices_cube_vec.emplace_back(
            vertices_cube[i * 6],
            vertices_cube[i * 6 + 1],
            vertices_cube[i * 6 + 2]
        );
    }

    std::vector<glm::vec3> shadowVolume;
    CreateShadowVolume(lightPos, vertices_cube_vec, shadowVolume);

    // Render shadow volume
    glBindVertexArray(shadowVolumeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, shadowVolumeVBO);
    glBufferData(GL_ARRAY_BUFFER, shadowVolume.size() * sizeof(glm::vec3),
        shadowVolume.data(), GL_DYNAMIC_DRAW);

    // First pass - increment stencil for front faces
    glCullFace(GL_BACK);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR_WRAP);
    glDrawArrays(GL_TRIANGLES, 0, shadowVolume.size());

    // Second pass - decrement stencil for back faces
    glCullFace(GL_FRONT);
    glStencilOp(GL_KEEP, GL_KEEP, GL_DECR_WRAP);
    glDrawArrays(GL_TRIANGLES, 0, shadowVolume.size());

    // 3. Final pass - render shadowed areas
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilMask(0x00);
    glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // Enable blending for shadow overlay
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shadowOverlayShader->UseProgram();
    float shadowIntensity = 0.5f; // Adjust this value as needed
    shadowOverlayShader->SetFloat("shadowIntensity", shadowIntensity);

    RenderFullScreenQuad();

    // Restore state
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glStencilMask(0xFF);
}


void RenderShadowOverlay() {
    static const float quadVertices[] = {
        -1.0f,  1.0f,  // Top left
        -1.0f, -1.0f,  // Bottom left
         1.0f, -1.0f,  // Bottom right
         1.0f,  1.0f   // Top right
    };

    static GLuint quadVAO = 0;
    static GLuint quadVBO;

    if (quadVAO == 0) {
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    }

    // Use a simple shader that outputs semi-transparent black
    shadowOverlayShader->UseProgram();
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_QUADS, 0, 4);
    glBindVertexArray(0);
}
void ProcessInput(GLFWwindow* window) {
    // Existing camera controls...

    // Light position controls
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        lightPos.z -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        lightPos.z += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        lightPos.x -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        lightPos.x += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        lightPos.y += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        lightPos.y -= 0.1f;
}

int main() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_STENCIL_BITS, 8); // Add this line for stencil buffer

    GLFWwindow* window = glfwCreateWindow(1200, 900, "F22040119", NULL, NULL);
    if (window == NULL) {
        ERROR_LOG("Failed to create GLFW window")
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScollCallback);

    if (glewInit() != GLEW_OK) {
        ERROR_LOG("Failed to initialize GLEW")
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glDepthFunc(GL_LESS);


    CreateVertexBuffer();

    // Load and compile shaders
    Shader cubeshader("core.vs", "core.fs");
    Shader shader("light.vs", "light.fs");
    shadowVolumeShader = new Shader("shadow_volume.vs", "shadow_volume.fs");
    shadowOverlayShader = new Shader("shadow_overlay.vs", "shadow_overlay.fs");
    InitializeShadowVolumeBuffers();

    GLfloat lastTime = 0.0f;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        SetViewport(window);
        glfwPollEvents();
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        ProcessKeyboard();
        // Process input first
        ProcessInput(window);  // Add this line here!
        glClearColor(0.6f, 0.8f, 0.9f, 1.0f);
        RenderScreen(window, cubeshader);  // Pass cubeshader as parameter

        glfwSwapBuffers(window);
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO_surface);
    glDeleteBuffers(1, &VBO_surface);
    glDeleteVertexArrays(1, &VAO_cube);
    glDeleteBuffers(1, &VBO_cube);
    glDeleteVertexArrays(1, &shadowVolumeVAO);
    glDeleteBuffers(1, &shadowVolumeVBO);
    delete shadowVolumeShader;
    delete shadowOverlayShader;

    glfwTerminate();
    return 0;
}

void ProcessKeyboard()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}
GLfloat lastX = 0.0f, lastY = 0.0f;
bool firstMouse = true;
void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
    static bool firstMouse = true;
    static float lastX = 400, lastY = 300; // Assuming the initial mouse position is at the center of the window
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos; // Reversed since y-coordinates go from bottom to top

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

void ScollCallback(GLFWwindow* window, double xPoxOffset, double
    yOffset) {
    camera.ProcessMouseScroll(yOffset);

}

