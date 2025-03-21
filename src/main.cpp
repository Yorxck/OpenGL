#include <glad/glad.h> 
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

const char* VertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    uniform vec3 CameraPosition;
    uniform vec3 CameraRotation;
    uniform float fov;
    uniform float aspectratio;

    out float Brightness;

    void main() {
        float pitchRad = radians(CameraRotation.x);
        float yawRad = radians(CameraRotation.y);

        vec3 forward = normalize(vec3(cos(yawRad) * cos(pitchRad), sin(pitchRad), sin(yawRad) * cos(pitchRad)));
        vec3 right = normalize(cross(vec3(0,1,0), forward));
        vec3 up = normalize(cross(forward, right));

        vec3 V = (aPos - CameraPosition);
        float d = dot(V, forward); // Depth

        float x = dot(V, right);
        float y = dot(V, up);

        float vfov = radians(fov);
        float hfov = 2.0f * atan(aspectratio * tan(vfov * 0.5f));

        float h = x / (d * tan(hfov / 2.0f));
        float v = y / (d * tan(vfov / 2.0f));

        Brightness = 1.0f;
        gl_Position = vec4(h, v, 2 * (1 / d) - 1, 1.0f);
    }
)glsl";

const char* FragmentShaderSource = R"glsl(
    #version 330 core
    in float Brightness;

    out vec4 FragColor;

    void main() {
        float fade = smoothstep(0.0, 10.0, Brightness*10);
        FragColor = vec4(vec3(1.0, 1.0, 1.0) * fade, 1);
    } 
)glsl";

/* Predefine functions */
GLFWwindow* CreateGLFWWindow(int w = 640, int h = 480, const char* Title = "New Window");
void update_camera(GLFWwindow* window, double xpos, double ypos);
void doChores();
GLint CreateShader();

/* Constants */
const float G = 6.6743e-11;
const float G_SCALED = G / std::pow(10,12); // 1 = 1.000.000m; G / (1.000.000*1.000.000)

/* Variables */
GLFWwindow* window = CreateGLFWWindow(1200, 800, "Simulation"); /* Create a windowed mode window and its OpenGL context */
Camera camera(Vector3::zero);
int width, height;
double delta;
double windowMaximalized = -glfwGetTime();
float speed = 100;

std::vector<Body> Bodies = {
    //Body(Vector3(0, 10, 0), 69.6340, 1.989 * std::pow(10, 30)), // Sun
    //Body(Vector3(54657000/100000, 0, 0), 2.4397, 3.285 * std::pow(10, 23), Vector3(0, 0, 47)), // Mercury
    //Body(Vector3(107700000/100000, 0, 0), 6.0518, 4.867 * std::pow(10, 24), Vector3(0, 0, 35)), // Venus
    Body(Vector3(0, 0, 0), 63.78, 5.972e24, Vector3(0, 0, 0)), // Earth
    Body(Vector3(384.400, 0, 0), 17.374, 7.35e22, Vector3(0, 0, 1.022)) // Moon
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

    GLint ShaderProgram = CreateShader();
    GLint CameraPositionLoc = glGetUniformLocation(ShaderProgram, "CameraPosition");
    GLint CameraRotationLoc = glGetUniformLocation(ShaderProgram, "CameraRotation");
    GLint fovLoc = glGetUniformLocation(ShaderProgram, "fov");
    GLint aspectratioLoc = glGetUniformLocation(ShaderProgram, "aspectratio");
    glUseProgram(ShaderProgram);

    /* Variables */
    double lastTime = glfwGetTime();
    float fpsLimit = 1.0 / 120.0;
    camera.fov = 70;
    camera.Position = Vector3(0, 100, 0);

    Bodies[0].Brightness = 1;

    while (!glfwWindowShouldClose(window)) {
        if ((glfwGetTime() - lastTime) >= fpsLimit) {
            delta = glfwGetTime() - lastTime;
            lastTime = glfwGetTime(); /* Update timestamp for dt */
            doChores();

            glUseProgram(ShaderProgram);
            GLenum err;
            while ((err = glGetError()) != GL_NO_ERROR) {
                std::cout << "OpenGL Error: " << err << std::endl;
            }
 
            for (int i = 0; i < Bodies.size(); i++) {
                Body& b1 = Bodies[i];

                glUniform3f(CameraPositionLoc, camera.Position.X, camera.Position.Y, camera.Position.Z);
                glUniform3f(CameraRotationLoc, camera.Rotation.X, camera.Rotation.Y, camera.Rotation.Z);
                glUniform1f(fovLoc, camera.fov);
                glUniform1f(aspectratioLoc, (float) width / (float) height);

                glBindVertexArray(b1.VAO);
                glDrawArrays(GL_TRIANGLES, 0, b1.verticesCount);
                glBindVertexArray(0);
                // for (int j = 0; j < Bodies.size(); j++) {
                //     Body& b2 = Bodies[j];
                //     if (&b1 == &b2) continue;

                //     Vector3 difference = b2.Position - b1.Position;
                //     float d = difference.magnitude();
                //     Vector3 direction = difference.normalized();

                //     float gforce = (G_SCALED * b1.Mass * b2.Mass) / (d*d);

                //     Vector3 Acceleration = direction * (gforce / b1.Mass);
                //     b1.accelerate(Acceleration);
                // }

                // for (int j = 0; j < Bodies.size(); j++) {
                //     Body& b2 = Bodies[j];

                //     // Check collision
                //     float d = (b1.Position - b2.Position).magnitude();
                //     float mind = (b1.Radius + b2.Radius);

                //     if (d <= mind)
                //         b1.Velocity *= -0.2f;
                // }

                // b1.update(delta * speed);
                //b1.draw(camera);
            }
            
            // float rs = (2*G_SCALED*b.Mass) / pow(299.792458, 2);
            // float y1 = 2 * std::sqrt(rs * ((vertex1 - b.Position).magnitude() - rs));
            // float y2 = 2 * std::sqrt(rs * ((vertex2 - b.Position).magnitude() - rs));

            camera.update(delta);
            glfwSwapBuffers(window); /* Swap front and back buffers */
            glfwPollEvents(); /* Poll for and process events */
        }
    }

    for (int i = 0; i < Bodies.size(); i++) {
        Body& b1 = Bodies[i];
        
        glDeleteVertexArrays(1, &b1.VAO);
        glDeleteBuffers(1, &b1.VBO);
    }

    glDeleteProgram(ShaderProgram);
    glfwTerminate();
    return 0;
}

void resize_framebuffer_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
    

GLFWwindow* CreateGLFWWindow(int w, int h, const char* Title) {

    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(w, h, Title, NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create window" << std::endl;
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, resize_framebuffer_callback);  

    return window;
}

void doChores() {
    glfwGetWindowSize(window, &width, &height);
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //glOrtho(0, width, height, 0, -1, 1);
    //glMatrixMode(GL_MODELVIEW);
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

GLint CreateShader() {
    GLint success;

    GLint VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &VertexShaderSource, NULL);
    glCompileShader(VertexShader);

    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(VertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
    }

    GLint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &FragmentShaderSource, NULL);
    glCompileShader(FragmentShader);

    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(FragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
    }

    GLint ShaderProgram = glCreateProgram();
    
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);

    
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(ShaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
    }

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);  

    return ShaderProgram;
}