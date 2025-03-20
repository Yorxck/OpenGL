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
double windowMaximalized = -glfwGetTime();
float speed = 20;

std::vector<Body> Bodies = {
    //Body(Vector3(0, 10, 0), 69.6340, 1.989 * std::pow(10, 30)), // Sun
    //Body(Vector3(54657000/100000, 0, 0), 2.4397, 3.285 * std::pow(10, 23), Vector3(0, 0, 47)), // Mercury
    //Body(Vector3(107700000/100000, 0, 0), 6.0518, 4.867 * std::pow(10, 24), Vector3(0, 0, 35)), // Venus
    Body(Vector3(0, 0, 0), 6.378, 5.972e24, Vector3(0, 0, 29.78)), // Earth
    //Body(Vector3(250120000/100000, 0, 0), 3.3895, 6.39 * std::pow(10, 23), Vector3(0, 0, 24)), // Mars
    //Body(Vector3(763850000/100000, 0, 0), 71.492, 1.898 * std::pow(10, 27), Vector3(0, 0, 13.06)), // Jupiter
    //Body(Vector3(1436400000/100000, 0, 0), 58.232, 5.683 * std::pow(10, 26), Vector3(0, 0, 9.68)), // Saturn
    //Body(Vector3(2922000000/100000, 0, 0), 25.362, 8.681 * std::pow(10, 25), Vector3(0, 0, 6.80)), // Uranus
    //Body(Vector3(4471200000/100000, 0, 0), 24.622, 1.024 * std::pow(10, 26), Vector3(0, 0, 5.43)), // Neptune
};

int main(void) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST); 
    glfwSetCursorPosCallback(glfwGetCurrentContext(), update_camera);

    /* Variables */
    double lastTime = glfwGetTime();
    float fpsLimit = 1.0 / 120.0;
    camera.fov = 70;
    camera.Position = Vector3(0, 100, 0);

    // rs = (2*G*Mass)/ pow(299792458, 2)
    // y = 2*sqrt(rs(r-rs))

    while (!glfwWindowShouldClose(window)) {
        if ((glfwGetTime() - lastTime) >= fpsLimit) {
            delta = glfwGetTime() - lastTime;
            lastTime = glfwGetTime(); /* Update timestamp for dt */
            doChores();
 
            for (int i = 0; i < Bodies.size(); i++) {
                Body& b1 = Bodies[i];

                for (int j = 0; j < Bodies.size(); j++) {
                    Body& b2 = Bodies[j];
                    if (&b1 == &b2) continue;

                    Vector3 difference = b2.Position - b1.Position;
                    float d = difference.magnitude();
                    if (d < 0.0001f) continue;
                    Vector3 direction = difference.normalized();
                    d *= 1000 * 100000;

                    float gforce = (G * b1.Mass * b2.Mass) / (d*d);

                    Vector3 Acceleration = direction * (gforce / b1.Mass);
                    b1.accelerate(Acceleration);
                }

                //b1.update(delta * speed);
                b1.draw(camera);
            }

            /*
            for (int x = -20; x < 20; x++) {
                for (int z = -20; z < 20; z++) {
                    Vector3 vertex1 = Vector3(50 * x, 0, 50 * z);
                    Vector3 vertex2 = Vector3(50 * x, 0, 50 * (z+1));

                    for (int i = 0; i < Bodies.size(); i++) {
                        Body& b = Bodies[i];
                        
                        float rs = (2*G*b.Mass) / pow(299792458, 2);
                        float y1 = 2 * std::sqrt(rs * ((vertex1 - b.Position).magnitude()*1000 - rs));
                        float y2 = 2 * std::sqrt(rs * ((vertex2 - b.Position).magnitude()*1000 - rs));

                        std::cout << rs << ", " << y1 << ", " << y2 << "\n";

                        vertex1.Y -= y1/100;
                        vertex2.Y -= y2/100;
                    }

                    
                    Vector3 p1 = camera.projectPoint(vertex1);
                    Vector3 p2 = camera.projectPoint(vertex2);

                    if (p1.Z <= 0 || p2.Z <= 0) continue; // depth check

                    glColor3f(1, 1, 1);

                    glBegin(GL_LINES);
                        glVertex3f(p1.X * width, p1.Y * height, 2 * (1 / p1.Z) - 1);
                        glVertex3f(p2.X * width, p2.Y * height, 2 * (1 / p2.Z) - 1);
                    glEnd();

                }
            }*/
            

            camera.update(delta);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    write(Vector2(1, height - 8*3), 
        "Position: " + camera.Position.toString() + 
        "\nRotation: " + camera.Rotation.toString() + 
        "\nfps: " + std::to_string((int) std::round(1/delta))
        , 1
    ); // write fps

    if ((glfwGetTime() - windowMaximalized) > 0.5 && glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_F11) == GLFW_PRESS) {
        if (glfwGetWindowAttrib(window, GLFW_MAXIMIZED) == GLFW_TRUE) {
            glfwRestoreWindow(window);
        } else {
            glfwMaximizeWindow(window);
        }
        windowMaximalized = glfwGetTime();
    }
}

void update_camera(GLFWwindow* window, double xpos, double ypos) {
    camera.mousecallback(window, Vector2(xpos, ypos), delta);
}