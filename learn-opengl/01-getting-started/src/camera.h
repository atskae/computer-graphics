#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>


// A camera in a right-handed coordinate system
class Camera {
    private:
        // Where the camera sits
        glm::vec3 position;
        // The front of the camera
        // Negative z-axis values look into the screen
        glm::vec3 front;
        // Up vector of the coordinate system
        glm::vec3 up;
        // The *opposite* direction of where the camera faces
        glm::vec3 direction;

        // How fast we look around the scene
        float speed;

        // Difference between previous and current frame
        float deltaTime;
        // Timestamp of previous frame
        float prevFrame;

        // Previous positions of the mouse
        // x-coordinate increases from left to right
        // y-coordinate increases from top to bottom
        float prevCursorPosX;
        float prevCursorPosY;

        // Euler angles, defined in degrees
        float yawDegrees;
        float pitchDegrees;

        // Take a fraction of mouse movements so we don't make
        //  changes to drastically between one frame
        float mouseSensitivity;

        // Describes how much of the scene we see with zoom in/out
        // Zoom in => fov decreases
        // Zoom out => fov increases
        float fieldOfView;
};

#endif // header guard
