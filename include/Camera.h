#include <Vector3.h>
#include <Vector2.h>
#include <cmath>
#include <GLFW/glfw3.h>

#ifndef CAMERA_H
#define CAMERA_H

class Camera {
    public:
        // Constructors
        Camera(Vector3 _Position) : Position(_Position) {}
    
        // Properties
        Vector3 Position; // Camera Positon 
        Vector3 Rotation = Vector3(); // Pitch (X), Yaw (Y), Roll (Z)
        float Speed = 50.0f;
        float Sensitivity = 15.0f;
        float fov = 80.0f;

        // Functions
        Vector3 direction() {
            float pitchRad = Rotation.X * (PI / 180.0f);
            float yawRad = Rotation.Y * (PI / 180.0f);

            return Vector3(
                cos(yawRad) * cos(pitchRad),
                sin(pitchRad),
                sin(yawRad) * cos(pitchRad)
            ).normalized();
        }

        void update(double delta) {
            Vector3 forward = this->direction();
            Vector3 right = Vector3::cross(Vector3::up, forward).normalized();
            Vector3 up = Vector3::cross(forward, right).normalized();

            float rspeed = (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) ? Speed * 2 : Speed;

            if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS) {
                Position += forward * (rspeed * delta); // Move forward
            }
            if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS) {
                Position -= forward * (rspeed * delta); // Move back
            }
            if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS) {
                Position -= right * (rspeed * delta); // Move right
            }
            if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS) {
                Position += right * (rspeed * delta); // Move left
            }
            if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_E) == GLFW_PRESS) {
                Position += up * (rspeed * delta); // Move up
            }
            if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Q) == GLFW_PRESS) {
                Position -= up * (rspeed * delta); // Move down
            }
        }

        void mousecallback(GLFWwindow* window, Vector2 Mouse, double delta) {
            if (firstCall) {
                lastMouse = Mouse;
                firstCall = false;
            }

            Vector2 Offset = lastMouse - Mouse;
            lastMouse = Mouse;
            Offset *= (Sensitivity * delta);

            Rotation.Y += Offset.X; // Yaw
            Rotation.X += Offset.Y; // Pitch
            Rotation.X = (Rotation.X) < (-89.0f) ? (-89.0f) : ((Rotation.X) > (89.0f) ? (89.0f) : (Rotation.X));
        }

        Vector3 projectPoint(Vector3 point) {
            int width, height;
            glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
            Vector3 forward = this->direction();
            Vector3 right = Vector3::cross(Vector3::up, forward).normalized();
            Vector3 up = Vector3::cross(forward, right).normalized();

            Vector3 V = point - Position;
            float d = Vector3::dot(V, forward); // Depth

            float x = Vector3::dot(V, right);
            float y = Vector3::dot(V, up);
            float aspectratio = (float) width / (float) height; // force floating point

            float vfov = fov * (PI /180);
            float hfov = 2.0f * atan(aspectratio * tan(vfov * 0.5f));

            float u = x / (d * tan(hfov / 2.0f));
            float v = y / (d * tan(vfov / 2.0f));

            float uScreen = (u + 1.0f) / 2.0f;
            float vScreen = 1.0f - (v + 1.0f) / 2.0f;

            return Vector3(uScreen, vScreen, d);
        }

    private:
        Vector2 lastMouse = Vector2();
        bool firstCall = true;

};

#endif