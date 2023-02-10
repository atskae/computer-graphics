#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// A camera in a right-handed coordinate system
class Camera {
    private:
        // If true, camera is a first-person shooter camera, which
        //  restricts the camera movement to the ground
        // The user may still look up and around though
        const bool isFPS;

        // Where the camera sits
        glm::vec3 position;
        // The front of the camera
        // Negative z-axis values look into the screen
        glm::vec3 front;
        // Up vector of the world coordinate system
        const glm::vec3 worldUp;
        // The *opposite* direction of where the camera faces
        glm::vec3 direction;

        // Ratio of window width and height
        float aspectRatio;

        // Take a fraction of mouse movements so we don't make
        //  changes to drastically between one frame
        const float mouseSensitivity;

        // Where the near and far planes are along the z-axis
        const float nearPlane; // z-min
        const float farPlane; // z-max

        // How fast we look around the scene
        // This is adjusted depending on the render time between
        //  the current frame and the previous frame
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

        
        // Describes how much of the scene we see with zoom in/out
        // Zoom in => fov decreases
        // Zoom out => fov increases
        float fieldOfView;
    
    public:
        Camera(int window_width, int window_height, bool is_fps): 
            isFPS(is_fps),
            position(glm::vec3(0.0f, 0.0f, 3.0f)),
            front(glm::vec3(0.0f, 0.0f, -1.0f)),
            worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
            mouseSensitivity(0.5f),
            nearPlane(0.1f),
            farPlane(100.0f),
            speed(0.05f),
            deltaTime(0.0f),
            prevFrame(0.0f),
            yawDegrees(-90.0f),
            pitchDegrees(0.0f),
            fieldOfView(45.0f)
        {
            std::cout << "isFPS=" << this->isFPS << std::endl;
            this->direction = this->position + this->front;
            this->aspectRatio = (float)window_width / (float)window_height;
            
            // Have the mouse position start in the center of the window
            this->prevCursorPosX = (float)window_width / 2.0f;
            this->prevCursorPosY = (float)window_height / 2.0f;
        }

        /*
            Methods for moving the camera in the scene
        */
        
        // Move forward into the screen
        // Camera's front vector is pointing in the negative z-axis direction
        void moveForward() {
            this->position += this->speed * this->front;
            // If FPS, prevent movement in the y direction
            if (isFPS) this->position.y = 0;
        }
        // Move backwards away from the screen 
        void moveBackwards() {
            this->position -= this->speed * this->front;
            // If FPS, prevent movement in the y direction
            if (isFPS) this->position.y = 0;
        }
        void moveLeft() {
            // Get the direction of the x-axis
            // The direction (positive x-axis or negative) depends on where the
            //  camera's front is. If the camera front is into the screen, negative z-axis,
            //  then the right vector would point toward the negative x-axis
            // The cross product gives us a new vector that is perpendicular to both
            //  the up vector and the front vector
            // We normalize the result to avoid applying scaling when we move
            glm::vec3 rightVector = glm::normalize(glm::cross(this->worldUp, this->front));
            this->position += this->speed * rightVector;
            
            // If FPS, prevent movement in the y direction
            if (isFPS) this->position.y = 0;
        }
        void moveRight() {
            glm::vec3 rightVector = glm::normalize(glm::cross(this->worldUp, this->front));
            this->position -= this->speed * rightVector;
            
            // If FPS, prevent movement in the y direction
            if (isFPS) this->position.y = 0;
        }

        /*
            Methods for changing camera settings with user input
        */
        void updateFrontVector(double xpos, double ypos);
        void updateFieldOfView(double scroll_direction);
        // Update the speed based on the duration between the recent frames
        void updateSpeed() { this->speed = 2.5f * this->deltaTime; }
        void updateTimestamp(float currentTime);

        /*
            Methods for computing matrices with camera settings
        */
       // Perspective projection matrix
       glm::mat4 getPerspectiveMatrix();
       // Get the view matrix
       glm::mat4 getLookAtMatrix();
};

// Updates the front vector of the camera given new mouse positions
void Camera::updateFrontVector(double xpos, double ypos) {
    std::cout << "Mouse xpos=" << xpos << ", y=pos=" << ypos << std::endl;

    // xpos increases from left to right
    // ypos increases from top to bottom
    float deltaX = xpos - this->prevCursorPosX;
    float deltaY = this->prevCursorPosY - ypos;
    this->prevCursorPosX = xpos;
    this->prevCursorPosY = ypos;

    // Apply mouse sensitivity
    deltaX *= this->mouseSensitivity;
    deltaY *= this->mouseSensitivity;

    // Left/right mouse movement adjusts the yaw angle
    this->yawDegrees += deltaX;
    
    // Up/down mouse movement adjusts the pitch angle
    this->pitchDegrees += deltaY;
    
    // Contrain the angle between -89 to 89 degrees
    // Otherwise we'd flip when the direction vector is parallel to the up vector (positive y-axis)
    if (this->pitchDegrees > 89.0f) pitchDegrees = 89.0f;
    if (this->pitchDegrees < -89.0f) pitchDegrees = -89.0f;

    // Convert angles to radians
    float yawRadians = glm::radians(yawDegrees);
    float pitchRadians = glm::radians(pitchDegrees);

    // Compute the new cameraFront vector
    glm::vec3 cameraFront;
    cameraFront.x = cos(yawRadians) * cos(pitchRadians);
    cameraFront.y = sin(pitchRadians);
    cameraFront.z = sin(yawRadians) * cos(pitchRadians);
    this->front = glm::normalize(cameraFront);

}

// Allow the camera to zoom in  and out given mouse scroll offsets
// The scroll direction is negative if the user scrolled down
//  and positive if the user scrolled up
void Camera::updateFieldOfView(double scroll_direction) {
    // Scrolling up will increase the newFieldOfView, scrolling down decreases newFieldOfView
    double newFieldOfView = this->fieldOfView - scroll_direction;
    
    // Ensure newFieldOfView stays positive and at most 45 degrees
    if (newFieldOfView < 0) newFieldOfView = 1.0f;
    else if (newFieldOfView > 45.0f) newFieldOfView = 45.0f;

    // Update the setting
    this->fieldOfView = newFieldOfView;

    std::cout << "Mouse scroll direction" << scroll_direction << ", new fov=" << this->fieldOfView << std::endl;
}

// Update the previous frame's timestamp
void Camera::updateTimestamp(float currentTime) {
    this->deltaTime = currentTime - this->prevFrame;
    this->prevFrame = currentTime;
}

glm::mat4 Camera::getPerspectiveMatrix() {
    return glm::perspective(
        glm::radians(this->fieldOfView), // convert from degrees to radians
        this->aspectRatio,
        this->nearPlane,
        this->farPlane 
    );
}

glm::mat4 Camera::getLookAtMatrix() {
    // Compute the direction vector: the "z-axis" in this matrix
    // X - Y = a vector pointing from Y to Z
    // This ends up pointing in the opposite direction of where the camera is facing
    // The coordinates of where the camera is looking at
    glm::vec3 cameraTarget = this->position + this->front;
    glm::vec3 directionVector = glm::normalize(this->position - cameraTarget);

    std::cout << "Camera position: " << glm::to_string(this->position) << std::endl;
    std::cout << "Camera front: " << glm::to_string(this->front) << std::endl;
    std::cout << "Camera target: " << glm::to_string(cameraTarget) << std::endl;
    std::cout << "position - front: " << glm::to_string(this->position - this->front) << std::endl;
    std::cout << "position - target: " << glm::to_string(this->position - cameraTarget) << std::endl;
    std::cout << "Incorrect z-axis: " << glm::to_string(
        glm::normalize(this->position - this->front)
    ) << std::endl;
    std::cout << "Correct z-axis: " << glm::to_string(directionVector) << std::endl;
    
    // Get the vector that represents the position x-axis of the camera
    // The cross product gives us the vector that is orthogonal to the up and direction vector
    // We normalize the result so we don't have a scaling effect when moving in the x direction
    glm::vec3 rightVector = glm::normalize(
        glm::cross(this->worldUp, directionVector)
    );

    // Already normalized
    glm::vec3 upVector = glm::cross(directionVector, rightVector);

    std::cout << "Camera x-axis: " << glm::to_string(rightVector) << std::endl;
    std::cout << "Camera y-axis: " << glm::to_string(upVector) << std::endl;
    std::cout << "Camera z-axis: " << glm::to_string(directionVector) << std::endl;

    glm::mat4 translationMatrix = glm::mat4(1.0f);
    // Invert the camera's position since we want things in the scene to move
    //  in the *opposite* direction of the camera
    glm::vec4 position(-1.0f * this->position, 1.0f);
    translationMatrix[3] = position;
    
    std::cout << "Translation: " << glm::to_string(position) << std::endl;
    std::cout << "-----" << std::endl;

    // mat4 constructor takes in vectors by columns
    // glm::mat4 applies each input vector as a column
    // Apply transpose so that each row is the vectors we used as input
    glm::mat4 rotationMatrix = glm::transpose(
        glm::mat4(
            glm::vec4(rightVector, 0.0f),
            glm::vec4(upVector, 0.0f),
            glm::vec4(directionVector, 0.0f),
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
        )
    );

    // Transformations read from right to left
    // First we translate, then we apply camera -> world
    glm::mat4 lookAtMatrix = rotationMatrix * translationMatrix;
    return lookAtMatrix;
}

#endif // header guard
