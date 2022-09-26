#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer/stb_image.h"
#include"Renderer/Shader.h"
#include"Renderer/Camera.h"
#include"Renderer/Head.h"
#include"Hair/HairNode.h"

#include<iostream>

using namespace std;
using namespace glm;

// Global properties for the window size
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

//Function decarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void CalculateMouseRay();
//bool IntersectionInRange(float start, float end, vec3 ray);
//vec3 GetPointOnRay(vec3 ray, float distance);

// Camera
Camera camera = Camera();
mat4 projection, view;

// Timing
float currentTime = 0.0f;
float prevTime = 0.0f;
float deltaTime = 0.0f;	// time between current frame and last frame
float frameTime = 0.0f;
unsigned int nFrames = 0; //counting frame rate
unsigned int frameCount = 0; // For average
float totalFPS = 0.0f;

// For physics
int fixedFPS = 60;
float fixedFrameS = (float)1 / fixedFPS;

bool usingVsync = false;

// Cursor
double mouseX, mouseY;
int state;

// Raycast
vec3 mouseRay;
//const float RAY_RANGE = 600;


int main() 
{
    // 1. glfw: initializes glfw.
    glfwInit();

    // 2. glfw: configure options from glfw.
    // For full option: https://www.glfw.org/docs/latest/window.html#window_hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set the version of OpenGL we are using, in our case we can try OpenGL 3.3.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Later we can try 4.6
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // This line is for MacOSX support.
#endif

    // 3. glfw: create our window context.
    // For creating different windows: https://www.glfw.org/docs/latest/window_guide.html
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "HairCuttingGL", NULL, NULL);
    // If failed to create window, return -1.
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set the current window as our context.
    glfwMakeContextCurrent(window);
    // Tell GLFW to call this function when doing every screen resize by registering it.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Tell GLFW to call this function cursor is moving in viewport and mouse related functions.
    //glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    //glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    // 4. glad: load all of OpenGL function pointers
    //      glad manages function pointers to OpenGL
    //      This loads the address of OpenGL pointers from GLAD and 
    //      glfwGetProcAddress gives us the definition of the function 
    //      based on which OS we're compiling for in GLFW.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // Build and compile shader programs
    Shader defaultShader("./src/Renderer/Shaders/Default.vert", "./src/Renderer/Shaders/Default.frag");
    Shader hairShader("./src/Renderer/Shaders/Hair.vert", "./src/Renderer/Shaders/Hair.frag");

    // All the model and texture files for head
    const char* headModelSrc = "./src/Renderer/Models/rabbit.obj";
    const char* headTexSrc = "./src/Renderer/Textures/fur.jpg";
    
    // All the texture file for hair, mesh will generate within
    const char* hairTexSource = "./src/Renderer/Textures/hair01.png";

    Head headModel(headModelSrc, headTexSrc);
    //Hair hair1(vec3(0, 0, 0), vec3(0, 1, 0), vec3(1, 1, 0), vec3(1, 0, 0), 100, hairTexSource);
    //Hair* hair1 = new Hair(vec3(0, 0, 0), vec3(0.25f, 1.0f, 0), vec3(0.75, -1, 0), vec3(1, 0, 0), 100, hairTexSource);
    
    Hair* hair1 = new Hair(vec3(0, 0.0f, 0), vec3(0, -0.75f, 1.0f), vec3(0, -1.25f, -1.0f), vec3(0, -2.0f, 0), 10, hairTexSource);
    Hair* hair2 = new Hair(vec3(1.0f, 0.0f, 0), vec3(1.0f, -0.75f, 1.0f), vec3(1.0f, -1.25f, -1.0f), vec3(1.0f, -2.0f, 0), 3, hairTexSource);
    Hair* hair3 = new Hair(vec3(-1.0f, 0.0f, 0), vec3(-1.75f, -0.75f, 1.0f), vec3(-2.25f, -1.25f, -1.0f), vec3(-3.0f, -2.0f, 0), 10, hairTexSource);

    //hair1->DeleteLink(2);
    //hair1->DeleteLink(5);

    //hair2->DeleteLink(1);

    //hair3->DeleteLink(4);
    
    //hair1->hairNodes;
    //std::cout << "Node count: " << hair1->nodeCount << std::endl;
    //std::cout << "Link count: " << hair1->linkCount<< std::endl;
    //delete hair1;
    //std::cout << hair1->nodeCount << std::endl;

    // Uncomment to change to wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 5. Create our rendering Loop
    //      We have to let the application to keep looping until we closes the window.

    //Disable vsync and obtain uncapped frame rate, comment to turn on vsync and limit to monitor refresh rate.
    if(!usingVsync)
        glfwSwapInterval(0);

    while (!glfwWindowShouldClose(window))
    {
        currentTime = (float)glfwGetTime();
        deltaTime = currentTime - prevTime;

        //Frame counter
        nFrames++;
        frameCount++;

        // Inputs when turning vsync on
        if (usingVsync) {
            // Print fps
            std::string FPS = std::to_string((int)round((1.0f / deltaTime) * nFrames));
            std::string ms = std::to_string((deltaTime / nFrames) * 1000);
            totalFPS += (1.0f / deltaTime) * nFrames;
            std::string avgFPS = std::to_string(totalFPS /frameCount);
            std::string newTitle = "HairCuttingGL " + FPS + "FPS / " + ms + "ms - Average FPS: " + avgFPS + "FPS";
            glfwSetWindowTitle(window, newTitle.c_str());

            prevTime = currentTime;
            nFrames = 0;

            // Inputs when turning vsync off, should be fine with capped frames
            processInput(window);
            
        } 
        else
        {
            totalFPS += (1.0f / deltaTime) * nFrames;
            //Limit frames for inputs physics update etc...
            if (deltaTime >= fixedFrameS) // Limit to 60 fps, and check if delta time is at around 16.67ms per frame.
            {
                // Print fps
                std::string FPS = std::to_string((int)round((1.0f / deltaTime) * nFrames));
                std::string ms = std::to_string((deltaTime / nFrames) * 1000);
                std::string avgFPS = std::to_string(totalFPS / frameCount);
                std::string newTitle = "HairCuttingGL " + FPS + "FPS / " + ms + "ms - Average FPS: " + avgFPS + "FPS";
                glfwSetWindowTitle(window, newTitle.c_str());

                prevTime = currentTime;
                nFrames = 0;

                // Process physics
                hair1->UpdatePhysics(fixedFrameS);
                hair2->UpdatePhysics(fixedFrameS);
                hair3->UpdatePhysics(fixedFrameS);

                // Inputs when turning vsync on
                processInput(window);

                // For ray to world intersection
                state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
                if (state == GLFW_PRESS)
                {
                    CalculateMouseRay();
                    hair1->AABB_Test(camera.GetPosition(), mouseRay);
                    hair2->AABB_Test(camera.GetPosition(), mouseRay);
                    hair3->AABB_Test(camera.GetPosition(), mouseRay);
                }
            }
        }
            
        // Clear the screen with the color of our choice and clear depth buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Create perspective matrix and set it in shader
        projection = perspective(radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

        // Get view matrix from camera and set it in shader
        view = camera.GetViewMatrix();
        
        // Debug camera positions
        //camera.Debug();

        //Draw head
        defaultShader.use();
        mat4 model = mat4(1.0f);
        model = translate(model, vec3(-2.0f, -1, 0));
        model = scale(model, vec3(0.05, 0.05, 0.05));
        defaultShader.setMat4("projection", projection);
        defaultShader.setMat4("view", view);
        defaultShader.setMat4("model", model);
        //headModel.Draw(defaultShader,headModel.textureID);

        // Draw hairs
        hairShader.use();
        model = mat4(1.0f);
        hairShader.setMat4("projection", projection);
        hairShader.setMat4("view", view);
        
        // Place hair in scene
        model = mat4(1.0f);
        //model = translate(model, hair1->hairPosition[i]);
        //model = translate(model, hair1->hairPosition);
        hairShader.setMat4("model", model);
        hair1->DrawHair(hairShader, hair1->hairTextureID);

        // Hair 2
        model = mat4(1.0f);
        //model = translate(model, hair2->hairPosition);
        hairShader.setMat4("model", model);
        hair2->DrawHair(hairShader, hair2->hairTextureID);

        // Hair 3
        model = mat4(1.0f);
        //model = translate(model, hair3->hairPosition);
        hairShader.setMat4("model", model);
        hair3->DrawHair(hairShader, hair3->hairTextureID);

        // glfw: swap buffers and poll IO events. (eg: key pressed, mouse moved, etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // When we exit the rendering loop, we terminate glfw with their libraries.
    glfwTerminate();
    return 0;
}

// glfw: a callback function that is set to dispatch when the user resizes the window.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// This function will keep track of the inputs, this will return which key it is being
// pressed.
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::UPWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::DOWNWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::IN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(Camera_Movement::OUT, deltaTime);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    mouseX = xpos;
    mouseY = ypos;
    //printf("xPos: %f, yPos: %f\n", mouseX, mouseY);
}

//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
//{
//    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
//        CalculateMouseRay();
//}

void CalculateMouseRay() {
    //TODO: Normalized Device Coordinates range [-1:1, -1:1, -1:1]
    float x = (float)(2.0f * mouseX) / SCREEN_WIDTH - 1.0f;
    float y = 1.0f - (float)(2.0f * mouseY) / SCREEN_HEIGHT;

    vec2 rayNDS = vec2(x, y);
    //printf("rayNDS:[%f, %f]\n", x, y);

    // 4D Homogeneous Clip Coordinates
    vec4 rayClip = vec4(rayNDS.x, rayNDS.y, -1.0f, 1.0f);
    //printf("rayClip:[%f, %f, %f, %f]\n", rayClip.x, rayClip.y, rayClip.z, rayClip.w);

    // 4D Eye (Camera) Coordinates
    vec4 rayEye = inverse(projection) * rayClip;
    rayEye = vec4(rayEye.x, rayEye.y, -1.0, 0.0);
    //printf("rayEye:[%f, %f, %f, %f]\n", rayEye.x, rayEye.y, rayEye.z, rayEye.w);

    // 4D World Coordinates
    vec4 rayWorld = inverse(view) * rayEye;

    // Ray to world coordinates
    mouseRay = vec3(rayWorld.x, rayWorld.y, rayWorld.z);
    mouseRay = normalize(mouseRay);
    printf("mouseRay:[%f, %f, %f]\n", mouseRay.x, mouseRay.y, mouseRay.z);
}

//bool IntersectionInRange(float start, float end, vec3 ray)
//{
//    vec3 startPoint = GetPointOnRay(ray, start);
//    vec3 endPoint = GetPointOnRay(ray, end);
//    return false;
//}
//
//vec3 GetPointOnRay(vec3 ray, float distance)
//{
//    vec3 camPos = camera.GetPosition();
//    vec3 scaledRay = vec3(ray.x * distance, ray.y * distance, ray.z * distance);
//    vec3 finalPos = camPos + scaledRay;
//    return finalPos;
//}

