#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <time.h>
#include <random>
#include <glm/glm.hpp>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <learnopengl/filesystem.h>
#include "shader.h"
#include "camera.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace glm;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
int main() {
    srand(time(0));
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    Shader lightingShader("../2.1.point.vs", "../2.1.point.fs", "../2.1.point.gs");
    //Shader lightingShader("2.1.basic_lighting.vs", "2.1.basic_lighting.fs");
    //Shader lightCubeShader("../2.1.light_cube.vs", "../2.1.light_cube.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         -0.5f, -0.5f, 0.5f
    };
    vector<vec3> puntos, normales;
    float dx,dy,dz;
    /*float y, z = 0.0;
    for (float x=-10; x < 10; x+=0.01 ) {
        y = -3*x*x*x*x*x - 7.4*x*x*x*x + 3.1415*x*x - x - 2;
        puntos.emplace_back(vec3(x,y,z));
    }*/
    //float z;
    /*for (float x=-10; x < 10; x+=0.1 ) {
        for (float y=-10; y < 10; y+=0.1 ) {
            z = 5 - x*x - y*y;
            puntos.emplace_back(vec3(x, y, z));
            dx = 2*x + y*y;
            dy = x*x + 2*y;
            dz = x*x + y*y + 1;
            normales.emplace_back(normalize(vec3(dx,dy,dz)));
        }
    }*/


    //GENERATE TUNEL
    vec3 center(0,0,0);
    int resolution_per_depth = 3;
    int p_per_quadrant = 20;
    int r = 5;
    float range = 90.0/p_per_quadrant;
    int depth = 1000;
    int min = -2;
    int max = 2;
    int rng = max - min + 1;

    ofstream file;
    file.open("tunel.csv", std::ofstream::out | std::ofstream::trunc);
    for( int d =0; d<depth; ++d) {
        for (float re = d; re < d+1; re += 1.0/resolution_per_depth) {
            center.y = cos(re/5)*1.5;
            center.x = re*re*re/10000;
            for (float i = 0; i <= 90; i += range) {
                //primer cuadrante

                float x = r * cos(i * 3.14159 / 180);
                float y = r * sin(i * 3.14159 / 180);
                float z = re;
                auto v1 = center + vec3(x + (rand() % rng + min) / 10.0, y + (rand() % rng + min) / 10.0,
                               z + (rand() % rng + min) / 10.0);
                auto v2 = center +vec3(x + (rand() % rng + min) / 10.0, -y + (rand() % rng + min) / 10.0,
                               z + (rand() % rng + min) / 10.0);
                auto v3 = center +  vec3(-x + (rand() % rng + min) / 10.0, -y + (rand() % rng + min) / 10.0,
                               z + (rand() % rng + min) / 10.0);
                auto v4 = center + vec3(-x + (rand() % rng + min) / 10.0, y + (rand() % rng + min) / 10.0,
                               z + (rand() % rng + min) / 10.0);
              //  puntos.emplace_back(v1);
              //  normales.emplace_back(normalize(v1));
                file << ceil(v1.x*100.0)/100.0  << "," << ceil(v1.y*100.0)/100.0  << "," << ceil(v1.z*100.0)/100.0  << endl;
                //segundo cuadrante
               // puntos.emplace_back(v2);
               // normales.emplace_back(normalize(v2));
                file << ceil(v2.x*100.0)/100.0  << "," << ceil(v2.y*100.0)/100.0  << "," << ceil(v2.z*100.0)/100.0  << endl;
                //tercer cuadrante
               // puntos.emplace_back(v3);
                //normales.emplace_back(normalize(v3));
                file << ceil(v3.x*100.0)/100.0  << "," << ceil(v3.y*100.0)/100.0  << "," << ceil(v3.z*100.0)/100.0  << endl;
                //tercer cuadrante
                //puntos.emplace_back(v4);
                //normales.emplace_back(normalize(v4));
                file << ceil(v4.x*100.0)/100.0  << "," << ceil(v4.y*100.0)/100.0  << "," << ceil(v4.z*100.0)/100.0  << endl;


            }
        }
    }

    file.close();

    ifstream in("tunel.csv");
    vector<vector<float>> fields;

    if (in) {
        string line;

        while (getline(in, line)) {
            stringstream sep(line);
            string field;

            fields.push_back(vector<float>());

            while (getline(sep, field, ',')) {
                fields.back().push_back(stof(field));
            }
        }
    }else{
        cout << "no lee" << endl;
    }

    for (auto row : fields) {
        puntos.emplace_back(vec3(row[0],row[1],row[2]));
        normales.emplace_back(normalize(vec3(row[0],row[1],row[2])));
    }




    // first, configure the cube's VAO (and VBO)
    unsigned int VBO[2], cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(2, VBO);

    glBindVertexArray(cubeVAO);
    // position attribute
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, puntos.size() * sizeof(vec3), puntos.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, normales.size() * sizeof(vec3), normales.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
    glPointSize(2);

    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // render the cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_POINTS, 0, puntos.size());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(2, VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
