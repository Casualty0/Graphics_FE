#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <windows.h>
#include <GL/glu.h>

void drawBrownWindow(float x_left, float y_bottom, float x_right, float y_top) {
    float window_width = x_right - x_left;
    float window_height = y_top - y_bottom;

    // --- 1. Draw the Dark Interior/Glass Area ---
    // The image's interior is a uniform gray, so we change the color here.
    //drawRectangle(x_left, y_bottom, x_right, y_top, 0.70f, 0.70f, 0.70f); // Gray background /*DONT NEED THIS*/

    // --- 2. Draw the Brown Grid/Shutter Lines ---

    // Set the brown/orange color for the lines in the frame
    float line_r = 0.94f;
    float line_g = 0.49f;
    float line_b = 0.17f; // A bright orange/brown frame
    glColor3f(line_r, line_g, line_b);
    glLineWidth(3.1f); // Increase line thickness to better represent the image's structure 

    glBegin(GL_LINES);

    // A. Draw the Horizontal Slats 
    // The frame has about 20 horizontal divisions for the slats.
    int horizontal_slats = 20;

    // We only need to draw 'horizontal_slats - 1' lines
    for (int i = 1; i < horizontal_slats; ++i) {
        float y = y_bottom + (window_height / horizontal_slats) * i;
        glVertex2f(x_left, y);
        glVertex2f(x_right, y);
    }

    // B. Draw the Diagonal Cross-Bracing Lines (the 'X')

    // 1. Bottom-left to top-right
    glVertex2f(x_left, y_bottom);
    glVertex2f(x_right, y_top);

    // 2. Top-left to bottom-right
    glVertex2f(x_left, y_top);
    glVertex2f(x_right, y_bottom);

    // C. Draw a simple frame for the window edge (Optional, but helps define the border)
    // Left border
    glVertex2f(x_left, y_bottom);
    glVertex2f(x_left, y_top);
    // Right border
    glVertex2f(x_right, y_bottom);
    glVertex2f(x_right, y_top);
    // Bottom border
    glVertex2f(x_left, y_bottom);
    glVertex2f(x_right, y_bottom);
    // Top border
    glVertex2f(x_left, y_top);
    glVertex2f(x_right, y_top);

    glEnd();
    glLineWidth(1.0f); // Reset line thickness
}

void drawRectangle(float x1, float y1, float x2, float y2, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();

    // Outline in dark grey
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

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
        // The original parameters define the boundaries for the drawing:
 // X: [-15.0, 15.0], Y: [30.0, 60.0]
 float size = 15.0f;     // half-size of rectangle
 float offsetY = 45.0f;  // center vertically
 float offsetX = 0.0f;   // center horizontally
 float scale = 12.0f;
 
 glPushMatrix();
 glTranslatef(offsetX, offsetY, 0.0f);
 glScalef(scale, scale, 1.0f);

 glEnable(GL_BLEND);
 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 glEnable(GL_LINE_SMOOTH);
 glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
 
 /*-- Main building(Cream color)--*/
 drawRectangle(-1.2f, -0.5f, 1.2f, 0.3f, 0.96f, 0.87f, 0.70f);

 /*---water pipe---*/
 drawRectangle(-1.23f, -0.45f, -1.20f, 0.3f, 1.0f, 1.0f, 1.0f);

 /*---Extractor vent---*/
 drawRectangle(-0.8f, 0.4f, -0.65f, 0.7f, 0.75f, 0.75f, 0.75f);  //change size & color

 // ----Main Window frame (orange/brown)----

 // Placement is on the right side, within the building boundaries:
 // X: from 0.1f to 1.0f 
 // Y: from -0.3f to 0.2f
 float window_x1 = 0.2f;
 float window_y1 = -0.48f;
 float window_x2 = 0.6f;
 float window_y2 = 0.17f;

 drawBrownWindow(window_x1, window_y1, window_x2, window_y2); 

 // Triangle for Roof
 
 glBegin(GL_TRIANGLES);
 glColor3f(0.82f, 0.48f, 0.48f); 
 glVertex2f(-1.3f, 0.3f);
 glColor3f(0.82f, 0.48f, 0.48f); 
 glVertex2f(1.3f, 0.3f);
 glColor3f(0.90f, 0.62f, 0.62f); 
 glVertex2f(0.0f, 1.0f);
 glEnd();

 
 glBegin(GL_QUADS);
 glColor3f(0.35f, 0.12f, 0.12f);
 glVertex2f(-1.32f, 0.29f);
 glVertex2f(1.32f, 0.29f);
 glVertex2f(1.30f, 0.33f);
 glVertex2f(-1.30f, 0.33f);
 glEnd();

 
 glBegin(GL_QUADS);
 glColor4f(0.0f, 0.0f, 0.0f, 0.20f);
 glVertex2f(-1.2f, 0.30f);
 glVertex2f(1.2f, 0.30f);
 glVertex2f(1.2f, 0.24f);
 glVertex2f(-1.2f, 0.24f);
 glEnd();

 // roof tiles
 glColor3f(0.6f, 0.25f, 0.25f);
 glLineWidth(1.0f);

 
 float tileRowSpacing = 0.04f; 
 float roofBaseY = 0.3f;
 float roofPeakY = 1.0f;
 float roofHalfWidth = 1.3f;

 // tile texture effect
 int row = 0;
 for (float y = roofBaseY + tileRowSpacing; y < roofPeakY - 0.03f; y += tileRowSpacing, row++) {
     float yProgress = (y - roofBaseY) / (roofPeakY - roofBaseY);
     float widthAtY = roofHalfWidth * (1.0f - yProgress);

     
     glColor3f(0.65f, 0.35f, 0.35f); 
     glBegin(GL_LINES);
     glVertex2f(-widthAtY + 0.02f, y - 0.005f);
     glVertex2f(widthAtY - 0.02f, y - 0.005f);
     glEnd();

     
     glColor3f(0.75f, 0.45f, 0.45f);
     glBegin(GL_LINES);
     glVertex2f(-widthAtY + 0.02f, y);
     glVertex2f(widthAtY - 0.02f, y);
     glEnd();
 }

 
 float tileWidth = 0.08f; 
 row = 0;
 for (float y = roofBaseY + tileRowSpacing * 0.5f; y < roofPeakY - 0.08f; y += tileRowSpacing, row++) {
     float yProgress = (y - roofBaseY) / (roofPeakY - roofBaseY);
     float widthAtY = roofHalfWidth * (1.0f - yProgress);

     
     float offset = (row % 2 == 0) ? 0.0f : tileWidth * 0.5f;

     
     for (float x = -widthAtY + offset; x < widthAtY - 0.02f; x += tileWidth) {
         if (x > -widthAtY + 0.02f && x < widthAtY - 0.02f) {
             
             glColor3f(0.65f, 0.35f, 0.35f);
             glBegin(GL_LINES);
             glVertex2f(x + 0.002f, y - tileRowSpacing * 0.3f); 
             glVertex2f(x + 0.002f, y + tileRowSpacing * 0.3f);
             glEnd();

             
             glColor3f(0.75f, 0.45f, 0.45f);
             glBegin(GL_LINES);
             glVertex2f(x, y - tileRowSpacing * 0.3f);
             glVertex2f(x, y + tileRowSpacing * 0.3f);
             glEnd();
         }
     }
 }

 // Ridge
 glLineWidth(2.5f);
 
 glColor3f(0.3f, 0.10f, 0.10f);
 glBegin(GL_LINES);
 glVertex2f(-0.07f, 0.955f);
 glVertex2f(0.07f, 0.955f);
 glEnd();

 
 glColor3f(0.4f, 0.15f, 0.15f);
 glBegin(GL_LINES);
 glVertex2f(-0.06f, 0.96f);
 glVertex2f(0.06f, 0.96f);
 glEnd();
 glLineWidth(1.0f);

 glPopMatrix();
 
 
 // Optional: Uncomment the code below to see the original green bounding box
/*
 glBegin(GL_LINES);
 glColor3f(0.0f, 1.0f, 0.0f); // Green

 // bottom
 glVertex2f(offsetX - size, offsetY - size); glVertex2f(offsetX + size, offsetY - size);
 // right
 glVertex2f(offsetX + size, offsetY - size); glVertex2f(offsetX + size, offsetY + size);
 // top
 glVertex2f(offsetX + size, offsetY + size); glVertex2f(offsetX - size, offsetY + size);
 // left
 glVertex2f(offsetX - size, offsetY + size); glVertex2f(offsetX - size, offsetY - size);

 glEnd();
 */
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
