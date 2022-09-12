#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer/stb_image.h"
#include"Renderer/Shader.h"
#include"Renderer/Camera.h"
#include"Renderer/Head.h"
#include"Hair/HairNode.h"

#include<iostream>

using namespace std;

// Global properties for the window size
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//Function decarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// stores how much we're seeing of either texture
float mixValue = 0.2f;

// Camera
Camera camera = Camera();

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float frameTime = 0.0f;
int nFrames = 0; //counting frame rate

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

    // Set up vertex data (and buffer(s)) and configure vertex attributes
    //float vertices[] = {
    //    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    //     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    //    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    //
    //    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    //    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    //    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //
    //    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //
    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //
    //    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    //     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //
    //    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    //    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    //};
    //// world space positions of our cubes
    //glm::vec3 cubePositions[] = {
    //    glm::vec3(0.0f,  0.0f,  0.0f),
    //    glm::vec3(2.0f,  5.0f, -15.0f),
    //    glm::vec3(-1.5f, -2.2f, -2.5f),
    //    glm::vec3(-3.8f, -2.0f, -12.3f),
    //    glm::vec3(2.4f, -0.4f, -3.5f),
    //    glm::vec3(-1.7f,  3.0f, -7.5f),
    //    glm::vec3(1.3f, -2.0f, -2.5f),
    //    glm::vec3(1.5f,  2.0f, -2.5f),
    //    glm::vec3(1.5f,  0.2f, -1.5f),
    //    glm::vec3(-1.3f,  1.0f, -1.5f)
    //};

    float hairVertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        0.25f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.25f, -0.5f,  0.0f,  1.0f,  1.0f,
        0.25f,  0.5f,  0.0f,  1.0f,  0.0f
    };

    // Hair locations
    // --------------------------------
    vector<glm::vec3> hairPosition
    {
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3(1.5f, 0.0f, 0.51f),
        glm::vec3(0.5f, 0.0f, 0.7f),
        glm::vec3(-0.3f, 0.0f, -2.3f),
        glm::vec3(0.5f, 0.0f, -0.6f)
    };

    // Hair VAO
    unsigned int hairVAO, hairVBO;
    glGenVertexArrays(1, &hairVAO);
    glGenBuffers(1, &hairVBO);
    glBindVertexArray(hairVAO);
    glBindBuffer(GL_ARRAY_BUFFER, hairVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(hairVertices), hairVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    //unsigned int VBO, VAO, EBO;
    //glGenVertexArrays(1, &VAO); // Create vertex array object to store vertex buffer(s)
    //glGenBuffers(1, &VBO);
    ////glGenBuffers(1, &EBO);
    //
    //// Bind Vertex Array Object first, then bind and set vertex buffers(s), then configure vertex attribute(s).
    //glBindVertexArray(VAO);
    //
    //// Copy vertices array in a vertex buffer for OpenGL to use.
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //
    //// Copy index array in a element buffer for OpenGL to use.
    ////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    ////glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //
    //// Set the vertex attributes pointers.
    //// Position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    //// Color attribute
    ////glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    ////glEnableVertexAttribArray(1);
    //// Texture attribute
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    //
    //// load and create a texture 
    //unsigned int texture1, texture2;
   
    // texture 1
    // load image, create texture and generate mipmaps
    unsigned int hairTextureID;
    int width, height, nrChannels;
    glGenTextures(1, &hairTextureID);
    glBindTexture(GL_TEXTURE_2D, hairTextureID);
    glGenerateMipmap(GL_TEXTURE_2D);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    

    const char* texSource = "./src/Renderer/Textures/hair01.png";
    unsigned char* data = stbi_load(texSource, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //
    //// texture 2
    //glGenTextures(1, &texture2);
    //glBindTexture(GL_TEXTURE_2D, texture2);
    //// set the texture wrapping parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //// set texture filtering parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //// load image, create texture and generate mipmaps
    //const char* texSource2 = "./src/Renderer/Textures/awesomeface.png";
    //data = stbi_load(texSource2, &width, &height, &nrChannels, 0);
    //if (data)
    //{
    //    // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //    glGenerateMipmap(GL_TEXTURE_2D);
    //}
    //else
    //{
    //    std::cout << "Failed to load texture" << std::endl;
    //}
    //stbi_image_free(data);

    const char* headModelSrc = "./src/Renderer/Models/rabbit.obj";
    const char* headTexSrc = "./src/Renderer/Textures/fur.jpg";
    Head headModel(headModelSrc, headTexSrc);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    //defaultShader.use(); // don't forget to activate/use the shader before setting uniforms!
    //defaultShader.setInt("texture1", 0);
    //defaultShader.setInt("texture2", 1);


    // The call to glVertexAttribPointer registered VBO so can safely unbind this buffer
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //Can unbind VAO so other other VAO call will not modify this VAO
    //glBindVertexArray(0);

    // Uncomment to change to wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    HairNode hairnode1(glm::vec3(0, 2, 4));

    hairnode1.ApplyForce(glm::vec3(0, 2, 4));
    std::cout << "next acc:" << hairnode1.acceleration.x << hairnode1.acceleration.y << hairnode1.acceleration.z << std::endl;

    // 5. Create our rendering Loop
    //      We have to let the application to keep looping until we closes the window.
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //Frame counter
        nFrames++;
        frameTime += deltaTime;
        //printf("%f delta time\n", deltaTime);
        if (frameTime >= 1.0) { // if last prinf() was more than 1 sec ago
         // printf and reset timer
            //printf("%f ms/frame\n",  double(nFrames));
            nFrames = 0;
            frameTime = 0;
        }
        //------

        // Inputs
        processInput(window);

        // Clear the screen with the color of our choice and clear
        // TODO: clear out depth buffer etc...
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind textures on corresponding texture units
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, texture1);
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, texture2);

        // set the texture mix value in the shader
        //defaultShader.setFloat("mixValue", mixValue);

        defaultShader.use();

        //create perspective matrix and set it in shader
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        defaultShader.setMat4("projection", projection);

        //get view matrix from camera and set it in shader
        glm::mat4 view = camera.GetViewMatrix();
        defaultShader.setMat4("view", view);
        
        // Debug camera positions
        //camera.Debug();

        // After calculated the color and set it in the shader, now we render our triangle
        //glBindVertexArray(VAO);

        // Calculate the model matrix 
        //for (unsigned int i = 0; i < 10; i++)
        //{
        //    glm::mat4 model = glm::mat4(1.0f); // Initialize matrix to identity matrix first
        //    model = glm::translate(model, cubePositions[i]);
        //    float angle = 20.0f * i;
        //    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //    defaultShader.setMat4("model", model);
        //
        //    glDrawArrays(GL_TRIANGLES, 0, 36);
        //}
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0, -1, 0));
        model = glm::scale(model, glm::vec3(0.05, 0.05, 0.05));
        defaultShader.setMat4("model", model);
        headModel.Draw(defaultShader,headModel.textureID);

        // Draw objects
        hairShader.use();
        projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        model = glm::mat4(1.0f);
        hairShader.setMat4("projection", projection);
        hairShader.setMat4("view", view);

        // Place hair in scene
        glBindVertexArray(hairVAO);
        glBindTexture(GL_TEXTURE_2D, hairTextureID);
        for (unsigned int i = 0; i < hairPosition.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, hairPosition[i]);
            //model = glm::translate(model, hairPosition[i] + sin(glm::vec3(glfwGetTime(), 0, 0)));
            hairShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        // glfw: swap buffers and poll IO events. (eg: key pressed, mouse moved, etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Optional: de-allocate all resources when they are not used
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    //glDeleteProgram(shaderProgram);


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

    //if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    //{
    //    mixValue += 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
    //    if (mixValue >= 1.0f)
    //        mixValue = 1.0f;
    //}
    //if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    //{
    //    mixValue -= 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
    //    if (mixValue <= 0.0f)
    //        mixValue = 0.0f;
    //}

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