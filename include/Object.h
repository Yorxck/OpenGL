#include <Vector2.h>
#include <Camera.h>
#include <cmath>
#include <GLFW/glfw3.h>

#ifndef BODY_H
#define BODY_H

class Body {
    public:
        // Constructors
        Body(Vector3 _position, float _radius, float _mass) : Position(_position), Radius(_radius), Mass(_mass) {}
    
        // Properties
        Vector3 Position;
        Vector3 Velocity;
        float Radius;
        float Mass;

        // Functions
        void accelerate(Vector3 Force) {
            Velocity += Force;
        }
        
        void update() {
            Position += Velocity;
        }

        void draw(Camera camera) {
            int width, height;
            glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
            float distance = (camera.Position - Position).magnitude();
            int resolution = 8; // calculate later based on distance

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

                    //Vector3 p1 = camera.projectPoint(v1);
                    //Vector3 p2 = camera.projectPoint(v2);
                    //Vector3 p3 = camera.projectPoint(v3);
                    //Vector3 p4 = camera.projectPoint(v4);

                    std::vector<std::vector<Vector3>> triangles {
                        {v1, v2, v3},
                        {v2, v3, v4}
                    };

                    for (std::vector<Vector3> vertices : triangles) {
                        int behindcamera = 0;

                        for (Vector3 vertex : vertices) {
                            
                        }
                    }


                    /*if (p1.Z <= 0 || p2.Z <= 0 || p3.Z <= 0 || p4.Z <= 0) continue; // depth check

                    glBegin(GL_TRIANGLES);
                        glVertex2f(p1.X * width, p1.Y * height);
                        glVertex2f(p2.X * width, p2.Y * height);
                        glVertex2f(p3.X * width, p3.Y * height);

                        glVertex2f(p2.X * width, p2.Y * height);
                        glVertex2f(p3.X * width, p3.Y * height);
                        glVertex2f(p4.X * width, p4.Y * height);
                    glEnd();*/
                }
            }
        }
};

#endif