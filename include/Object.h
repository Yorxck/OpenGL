#include <Vector2.h>
#include <Camera.h>
#include <cmath>
#include <GLFW/glfw3.h>

#ifndef BODY_H
#define BODY_H

class Body {
    public:
        // Constructors
        Body(Vector3 _position, float _radius, float _mass) : position(_position), radius(_radius), mass(_mass) {}
    
        // Properties
        Vector3 position;
        Vector3 velocity;
        float radius;
        float mass;

        // Functions
        void accelerate(Vector3 Force) {
            velocity += Force;
        }
        void update() {
            position += velocity;
        }
        void draw(Camera camera) {
            int width, height;
            glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

            Vector3 s1 = camera.projectPoint(position);

            for (int i = 0; i < 100; i++) {
                float theta1 = (i * (2 * PI) / 100);
                float theta2 = ((i + 1) * (2 * PI) / 100);
                
                Vector3 p1(0, radius * cos(theta1), radius * sin(theta1));
                Vector3 p2(0, radius * cos(theta2), radius * sin(theta2));
        
                p1 += position;
                p2 += position;

                Vector3 s2 = camera.projectPoint(p1);
                Vector3 s3 = camera.projectPoint(p2);
        
                glBegin(GL_TRIANGLES);

                glVertex2f(s1.X * width, s1.Y * height);
                glVertex2f(s2.X * width, s2.Y * height);
                glVertex2f(s3.X * width, s3.Y * height);

                glEnd();
            }
        }
};

#endif