#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <windows.h>
#include <GL/glu.h>

// ------------------ Camera Controls ------------------
float scrollX = 0.0f;
float scrollY = 0.0f;
float zoomLevel = 50.0f; // Initial zoom

// ------------------ CLASSES ------------------
class FloorPlan {
public:
    void Draw() {
        glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f); // White floor plan

        float size = 20.0f; // floor plan box (square)

        // Outer square
        glVertex2f(-size, -size); glVertex2f(size, -size);
        glVertex2f(size, -size); glVertex2f(size, size);
        glVertex2f(size, size); glVertex2f(-size, size);
        glVertex2f(-size, size); glVertex2f(-size, -size);

        // Reference cross
        glColor3f(1.0f, 0.0f, 0.0f); // red cross
        glVertex2f(-size, 0.0f); glVertex2f(size, 0.0f);
        glVertex2f(0.0f, -size); glVertex2f(0.0f, size);

        glEnd();
    }
};

class RearElevation {
public:
    void Draw() {
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f); // Green

        float size = 15.0f;    // half-size of square
        float offsetY = 45.0f; // center vertically above floor plan
        float offsetX = 0.0f;  // center horizontally

        // Outer square
        glVertex2f(offsetX - size, offsetY - size); glVertex2f(offsetX + size, offsetY - size); // bottom
        glVertex2f(offsetX + size, offsetY - size); glVertex2f(offsetX + size, offsetY + size); // right
        glVertex2f(offsetX + size, offsetY + size); glVertex2f(offsetX - size, offsetY + size); // top
        glVertex2f(offsetX - size, offsetY + size); glVertex2f(offsetX - size, offsetY - size); // left

        glEnd();
    }
};

class FrontElevation {
public:
    void Draw() {
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 1.0f); // Blue

        float size = 15.0f;    // half-size of square
        float offsetY = -45.0f; // center vertically below floor plan
        float offsetX = 0.0f;   // center horizontally

        // Outer square
        glVertex2f(offsetX - size, offsetY - size); glVertex2f(offsetX + size, offsetY - size); // bottom
        glVertex2f(offsetX + size, offsetY - size); glVertex2f(offsetX + size, offsetY + size); // right
        glVertex2f(offsetX + size, offsetY + size); glVertex2f(offsetX - size, offsetY + size); // top
        glVertex2f(offsetX - size, offsetY + size); glVertex2f(offsetX - size, offsetY - size); // left

        glEnd();
    }
};

class LeftElevation {
public:
    void Draw() {
        glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 1.0f); // Purple

        float size = 15.0f;      // half-size of square
        float offsetX = -45.0f;  // center horizontally
        float offsetY = 0.0f;    // center vertically

        // Outer square
        glVertex2f(offsetX - size, offsetY - size); glVertex2f(offsetX + size, offsetY - size); // bottom
        glVertex2f(offsetX + size, offsetY - size); glVertex2f(offsetX + size, offsetY + size); // right
        glVertex2f(offsetX + size, offsetY + size); glVertex2f(offsetX - size, offsetY + size); // top
        glVertex2f(offsetX - size, offsetY + size); glVertex2f(offsetX - size, offsetY - size); // left

        glEnd();
    }
};

class RightElevation {
public:
    void Draw() {
        glBegin(GL_LINES);
        glColor3f(0.5f, 0.5f, 0.0f); // Olive / yellow-green

        float size = 15.0f;      // half-size of square
        float offsetX = 45.0f;   // center horizontally
        float offsetY = 0.0f;    // center vertically

        // Outer square
        glVertex2f(offsetX - size, offsetY - size); glVertex2f(offsetX + size, offsetY - size); // bottom
        glVertex2f(offsetX + size, offsetY - size); glVertex2f(offsetX + size, offsetY + size); // right
        glVertex2f(offsetX + size, offsetY + size); glVertex2f(offsetX - size, offsetY + size); // top
        glVertex2f(offsetX - size, offsetY + size); glVertex2f(offsetX - size, offsetY - size); // left

        glEnd();
    }
};

// ------------------ MAIN ------------------
int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    // Fullscreen
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    window = glfwCreateWindow(mode->width, mode->height, "Architecture Views", glfwGetPrimaryMonitor(), NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Create objects
    FloorPlan floor;
    FrontElevation front;
    RearElevation rear;
    LeftElevation left;
    RightElevation right;

    while (!glfwWindowShouldClose(window))
    {
        // --- Controls ---
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  scrollX -= 0.01f;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) scrollX += 0.01f;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)    scrollY += 0.01f;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  scrollY -= 0.01f;

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) zoomLevel -= 0.01f;
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) zoomLevel += 0.01f;

        if (zoomLevel < 5.0f) zoomLevel = 5.0f;
        if (zoomLevel > 200.0f) zoomLevel = 200.0f;

        glClear(GL_COLOR_BUFFER_BIT);

        // Projection update
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-zoomLevel + scrollX, zoomLevel + scrollX,
            -zoomLevel + scrollY, zoomLevel + scrollY,
            -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // --- Draw All ---
        floor.Draw();
        front.Draw();
        rear.Draw();
        left.Draw();
        right.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}