#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <Vector2.h>
#include <Vector3.h>
#include <Object.h>
#include <Font.h>
#include <Camera.h>

/* Predefine functions */
GLFWwindow* CreateGLFWWindow(int w = 640, int h = 480, const char* Title = "New Window");
void update_camera(GLFWwindow* window, double xpos, double ypos);
void doChores();

/* Constants */
const double G = 6.6743e-11;

/* Variables */
GLFWwindow* window = CreateGLFWWindow(1200, 800, "Simulation"); /* Create a windowed mode window and its OpenGL context */
Camera camera(Vector3::zero);
int width, height;
double delta;

std::vector<Body> Bodies = {
    Body(Vector3(20, 0, 0), 50, 0)
};

int main(void) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(glfwGetCurrentContext(), update_camera);

    /* Variables */
    double lastTime = glfwGetTime();
    float fpsLimit = 1.0 / 120.0;
    camera.fov = 70;

    while (!glfwWindowShouldClose(window)) {
        if ((glfwGetTime() - lastTime) >= fpsLimit) {
            delta = glfwGetTime() - lastTime;
            doChores();
 
            for (int i = 0; i < Bodies.size(); i++) {
                Body body = Bodies[i];
                body.draw(camera);
            }
            
            camera.update(delta);
            lastTime = glfwGetTime(); /* Update timestamp for dt */
            glfwSwapBuffers(window); /* Swap front and back buffers */
            glfwPollEvents(); /* Poll for and process events */
        }
    }

    glfwTerminate();
    return 0;
}

GLFWwindow* CreateGLFWWindow(int w, int h, const char* Title) {

    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    GLFWwindow* window = glfwCreateWindow(w, h, Title, NULL, NULL);

    if (window) {
        glfwMakeContextCurrent(window);
    }

    return window;
}

void doChores() {
    glfwGetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT);
    write(Vector2(1, height - 8*3), 
        "Position: " + std::to_string((int) camera.Position.X) + ", " + std::to_string((int) camera.Position.Y) + "," + std::to_string((int) camera.Position.Z) + 
        "\nRotation: " + std::to_string((int) camera.Rotation.X) + ", " + std::to_string((int) camera.Rotation.Y) + "," + std::to_string((int) camera.Rotation.Z) + 
        "\nfps: " + std::to_string((int) std::round(1/delta))
        , 1
    ); // write fps
}

void update_camera(GLFWwindow* window, double xpos, double ypos) {
    camera.mousecallback(window, Vector2(xpos, ypos), delta);
}