#include <iostream>

// Let's see if this deprecated code even compiles.... . .
#include <GLUT/glut.h>

void lineSegment() {
    // Draw a line segment
    // Weeeeeird way to do this
    glBegin(GL_LINES);
        glVertex2i(180, 15);
        glVertex2i(10, 145);
    glEnd();
    glFlush(); // Process OpenGL calls immediately (instead of buffering) ?
}

int run_opengl(int* argc, char** argv) {
    // Initialize GLUT
    // Why do you need a reference to an integer... why not just copy it?
    glutInit(argc, argv);
    
    // Set other window options - combine options with OR operator
    // Use 1 refresh buffer and use RGB color mode
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    // Set distance away from the top-left corner of the display
    // Ex. 50 pixels from the left-edge, 100 pixels down from the top-edge
    glutInitWindowPosition(50, 100);

    // Set window size
    glutInitWindowSize(400, 300);
    
    // Create a window with a title bar
    glutCreateWindow("My first OpenGL window ever");   

    // Set background color to white
    // (R, G, B, alpha)
    glClearColor(1.0, 1.0, 1.0, 1.0); // set the color

    // Set projection mode
    glMatrixMode(GL_PROJECTION); // orthogonal projection
    // Viewing parameters

    // (x-min, x-max, y-min, y-max)
    // lower-left corner = (x=0.0, y=0.0)
    // upper-right corner = (x=200, y=150)
    gluOrtho2D(0.0, 200.0, 0.0, 150);
    
    // Use the colors in the color/refresh buffer
    glClear(GL_COLOR_BUFFER_BIT); // actually applies settings

    // 3f indicates specify in RGB
    // Define color for object?
    glColor3f(0.0, 0.4, 0.2);

    // Specify what to display in the window
    // Define an image and assign it to the window
    // We create a 3D scene and create a 2D projectopm

    // Set display callback to draw a line segment
    // Sends image to display
    glutDisplayFunc(lineSegment);
        
    // Run the window forever until we exit
    glutMainLoop();
    
    return 0;
}

int main(int argc, char** argv) {
    std::cout << "OpenGL! Wahoo!" << std::endl;
    return run_opengl(&argc, argv);
}
