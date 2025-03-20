#include <Vector2.h>
#include <Vector3.h>
#include <Camera.h>
#include <cmath>
#include <GLFW/glfw3.h>

#ifndef BODY_H
#define BODY_H

class Body {
    public:
        // Constructors
        Body(Vector3 _position, float _radius, float _mass) : Position(_position), Radius(_radius), Mass(_mass) {}
        Body(Vector3 _position, float _radius, float _mass, Vector3 _velocity) : Position(_position), Radius(_radius), Mass(_mass), Velocity(_velocity) {}
    
        // Properties
        Vector3 Position;
        Vector3 Velocity;
        float Radius;
        float Mass;

        // Functions
        void accelerate(Vector3 Force) {
            Velocity += Force;
        }
        
        void update(double delta) {
            Position += Velocity * delta;
        }

        void draw(Camera& camera) {
            //float distance = (camera.Position - Position).magnitude();
            int resolution = 50; // calculate later based on distance

            for (int i = 0; i < resolution / 2; i++) {
                float stacktheta1 = (i * PI) / (resolution / 2);
                float stacktheta2 = ((i + 1) * PI) / (resolution / 2);

                for (int j = 0; j < resolution; j++) {
                    float sectortheta1 = (j * (2 * PI)) / resolution;
                    float sectortheta2 = ((j + 1) * (2 * PI)) / resolution;

                    Vector3 v1 = Position + Vector3(sin(stacktheta1) * cos(sectortheta1), cos(stacktheta1), sin(stacktheta1) * sin(sectortheta1)) * Radius;
                    Vector3 v2 = Position + Vector3(sin(stacktheta1) * cos(sectortheta2), cos(stacktheta1), sin(stacktheta1) * sin(sectortheta2)) * Radius;
                    Vector3 v3 = Position + Vector3(sin(stacktheta2) * cos(sectortheta1), cos(stacktheta2), sin(stacktheta2) * sin(sectortheta1)) * Radius;
                    Vector3 v4 = Position + Vector3(sin(stacktheta2) * cos(sectortheta2), cos(stacktheta2), sin(stacktheta2) * sin(sectortheta2)) * Radius;

                    Vector3 p1 = camera.projectPoint(v1);
                    Vector3 p2 = camera.projectPoint(v2);
                    Vector3 p3 = camera.projectPoint(v3);
                    Vector3 p4 = camera.projectPoint(v4);

                    if (p1.Z <= 0 || p2.Z <= 0 || p3.Z <= 0 || p4.Z <= 0) continue; // depth check

                    Vector3 normal1 = Vector3::cross(v2 - v1, v3 - v1).normalized();
                    Vector3 normal2 = Vector3::cross(v2 - v3, v4 - v3).normalized();

                    Vector3 lightDir1 = (Vector3(0, 100, 0) - (v1 + v2 + v3) / 3).normalized();
                    Vector3 lightDir2 = (Vector3(0, 100, 0) - (v2 + v3 + v4) / 3).normalized();

                    float brightness1 = std::max(0.0f, Vector3::dot(normal1, lightDir1));
                    float brightness2 = std::max(0.0f, Vector3::dot(normal2, lightDir2));
                            
                    triangle(p1, p2, p3, brightness1);
                    triangle(p2, p3, p4, brightness2);
                }
            }
        }
    private:
        void triangle(Vector3 p1, Vector3 p2, Vector3 p3, float b) {
            int width, height;
            glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
            
            glColor3f(1 * b, 1 * b, 1 * b);

            glBegin(GL_TRIANGLES);
                glVertex3f(p1.X * width, p1.Y * height, 2 * (1 / p1.Z) - 1);
                glVertex3f(p2.X * width, p2.Y * height, 2 * (1 / p2.Z) - 1);
                glVertex3f(p3.X * width, p3.Y * height, 2 * (1 / p3.Z) - 1);
            glEnd();
        }
};

#endif