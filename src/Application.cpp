#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <windows.h>
#include <GL/glu.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ------------------ Utility Functions for Restaurant Front View ------------------

// Utility to draw a filled rectangle with outline
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

// Utility to draw a U-shape curve
void drawUShape(float cx, float cy, float radius, int segments) {
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= segments; i++) {
        float theta = M_PI * (float)i / (float)segments; // half-circle
        float x = cx + radius * cos(theta);
        float y = cy - radius * sin(theta);
        glVertex2f(x, y);
    }
    glEnd();
}

// Utility to draw a filled circle (supports alpha)
void drawCircleFilled(float cx, float cy, float r, int segments, float cr, float cg, float cb, float ca) {
    glColor4f(cr, cg, cb, ca);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; ++i) {
        float theta = 2.0f * (float)M_PI * (float)i / (float)segments;
        float x = cx + r * cosf(theta);
        float y = cy + r * sinf(theta);
        glVertex2f(x, y);
    }
    glEnd();
}

// Utility to draw a realistic 3D flower with petals and center
void drawRealisticFlower(float cx, float cy, float size, float r, float g, float b) {
    // Draw shadow first (darker, slightly offset)
    drawCircleFilled(cx + size*0.1f, cy - size*0.1f, size*1.1f, 8, 0.0f, 0.0f, 0.0f, 0.2f);
    
    // Draw outer petals (larger, lighter)
    float petalSize = size * 1.2f;
    glColor3f(r * 0.8f, g * 0.8f, b * 0.8f); // slightly darker outer petals
    for (int i = 0; i < 5; i++) {
        float angle = (float)i * 2.0f * M_PI / 5.0f;
        float petalX = cx + cosf(angle) * size * 0.3f;
        float petalY = cy + sinf(angle) * size * 0.3f;
        drawCircleFilled(petalX, petalY, petalSize * 0.4f, 6, r * 0.8f, g * 0.8f, b * 0.8f, 1.0f);
    }
    
    // Draw main flower body
    drawCircleFilled(cx, cy, size, 8, r, g, b, 1.0f);
    
    // Draw inner petals (smaller, brighter)
    glColor3f(r * 1.2f > 1.0f ? 1.0f : r * 1.2f, 
              g * 1.2f > 1.0f ? 1.0f : g * 1.2f, 
              b * 1.2f > 1.0f ? 1.0f : b * 1.2f);
    for (int i = 0; i < 3; i++) {
        float angle = (float)i * 2.0f * M_PI / 3.0f + 0.5f;
        float petalX = cx + cosf(angle) * size * 0.15f;
        float petalY = cy + sinf(angle) * size * 0.15f;
        drawCircleFilled(petalX, petalY, size * 0.25f, 4, 
                        r * 1.2f > 1.0f ? 1.0f : r * 1.2f,
                        g * 1.2f > 1.0f ? 1.0f : g * 1.2f,
                        b * 1.2f > 1.0f ? 1.0f : b * 1.2f, 1.0f);
    }
    
    // Draw flower center (darker)
    float centerR = (r > 0.5f) ? 0.2f : 0.8f;
    float centerG = (g > 0.5f) ? 0.2f : 0.8f;
    float centerB = (b > 0.5f) ? 0.2f : 0.8f;
    drawCircleFilled(cx, cy, size * 0.2f, 6, centerR, centerG, centerB, 1.0f);
    
    // Add tiny center highlight
    drawCircleFilled(cx - size*0.05f, cy + size*0.05f, size * 0.08f, 4, 1.0f, 1.0f, 1.0f, 0.8f);
}

// Utility to draw a circle outline
void drawCircleLine(float cx, float cy, float r, int segments, float cr, float cg, float cb) {
    glColor3f(cr, cg, cb);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i) {
        float theta = 2.0f * (float)M_PI * (float)i / (float)segments;
        float x = cx + r * cosf(theta);
        float y = cy + r * sinf(theta);
        glVertex2f(x, y);
    }
    glEnd();
}

// Utility to draw rectangular flower box matching window width
void drawFlowerBox(float x1, float y1, float x2, float y2, float height) {
    // Box base (rectangular to match window width)
    glColor3f(0.6f, 0.4f, 0.2f); // brown box
    glBegin(GL_QUADS);
    glVertex2f(x1, y1 - height*0.4f);
    glVertex2f(x2, y1 - height*0.4f);
    glVertex2f(x2, y1 + height*0.2f);
    glVertex2f(x1, y1 + height*0.2f);
    glEnd();
    
    // Box rim
    glColor3f(0.5f, 0.3f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(x1 - 0.01f, y1 + height*0.15f);
    glVertex2f(x2 + 0.01f, y1 + height*0.15f);
    glVertex2f(x2, y1 + height*0.2f);
    glVertex2f(x1, y1 + height*0.2f);
    glEnd();
    
    // Soil
    glColor3f(0.4f, 0.2f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(x1 + 0.01f, y1 + height*0.1f);
    glVertex2f(x2 - 0.01f, y1 + height*0.1f);
    glVertex2f(x2 - 0.01f, y1 + height*0.2f);
    glVertex2f(x1 + 0.01f, y1 + height*0.2f);
    glEnd();
    
    // Extremely dense flower stems and greenery (no empty spaces)
    glColor3f(0.2f, 0.6f, 0.2f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    float boxWidth = x2 - x1;
    float cx = (x1 + x2) * 0.5f; // center x
    int numStems = (int)(boxWidth * 80.0f); // overflowing dense stems
    for (int i = 0; i < numStems; i++) {
        float stemX = x1 + 0.005f + (boxWidth - 0.01f) * (float)i / (float)(numStems - 1);
        float stemHeight = height * (0.15f + 0.3f * sinf(stemX * 30.0f + (float)i * 0.3f)); // shorter stems
        glVertex2f(stemX, y1 + height*0.2f);
        glVertex2f(stemX, y1 + height*0.2f + stemHeight);
    }
    glEnd();
    
    // Add dense greenery/foliage stems
    glColor3f(0.15f, 0.5f, 0.15f); // darker green for variety
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    int greeneryStems = (int)(boxWidth * 60.0f);
    for (int i = 0; i < greeneryStems; i++) {
        float stemX = x1 + 0.008f + (boxWidth - 0.016f) * (float)i / (float)(greeneryStems - 1);
        float stemHeight = height * (0.1f + 0.2f * sinf(stemX * 35.0f + (float)i * 0.8f)); // shorter greenery stems
        glVertex2f(stemX, y1 + height*0.2f);
        glVertex2f(stemX, y1 + height*0.2f + stemHeight);
    }
    glEnd();
    glLineWidth(1.0f);
    
    // Extremely dense summer flowers (no empty spaces)
    for (int i = 0; i < numStems; i++) {
        float stemX = x1 + 0.005f + (boxWidth - 0.01f) * (float)i / (float)(numStems - 1);
        float stemHeight = height * (0.15f + 0.3f * sinf(stemX * 30.0f + (float)i * 0.3f)); // shorter stems for main flowers
        float flowerSize = height * (0.05f + 0.04f * sinf(stemX * 20.0f + (float)i * 0.4f));
        
        // Expanded summer flower colors (15 different colors)
        switch(i % 15) {
            case 0: drawRealisticFlower(stemX, y1 + height*0.2f + stemHeight, flowerSize, 1.0f, 0.2f, 0.2f); break; // red
            case 1: drawRealisticFlower(stemX, y1 + height*0.2f + stemHeight, flowerSize, 1.0f, 0.8f, 0.0f); break; // yellow
            case 2: drawRealisticFlower(stemX, y1 + height*0.2f + stemHeight, flowerSize, 0.8f, 0.2f, 0.8f); break; // purple
            case 3: drawRealisticFlower(stemX, y1 + height*0.2f + stemHeight, flowerSize, 1.0f, 0.4f, 0.7f); break; // pink
            case 4: drawRealisticFlower(stemX, y1 + height*0.2f + stemHeight, flowerSize, 0.2f, 0.6f, 1.0f); break; // blue
            case 5: drawRealisticFlower(stemX, y1 + height*0.2f + stemHeight, flowerSize, 1.0f, 1.0f, 0.2f); break; // bright yellow
            case 6: drawRealisticFlower(stemX, y1 + height*0.2f + stemHeight, flowerSize, 0.9f, 0.5f, 0.1f); break; // orange
            case 7: drawRealisticFlower(stemX, y1 + height*0.2f + stemHeight, flowerSize, 0.8f, 0.8f, 1.0f); break; // light blue
            case 8: drawRealisticFlower(stemX, y1 + height*0.2f + stemHeight, flowerSize, 1.0f, 0.6f, 0.8f); break; // light pink
            case 9: drawRealisticFlower(stemX, y1 + height*0.2f + stemHeight, flowerSize, 0.7f, 1.0f, 0.7f); break; // light green
            case 10: drawRealisticFlower(stemX, y1 + height*0.2f + stemHeight, flowerSize, 1.0f, 0.9f, 0.6f); break; // cream
            case 11: drawRealisticFlower(stemX, y1 + height*0.2f + stemHeight, flowerSize, 0.9f, 0.7f, 1.0f); break; // lavender
            case 12: drawRealisticFlower(stemX, y1 + height*0.2f + stemHeight, flowerSize, 1.0f, 0.5f, 0.5f); break; // coral
            case 13: drawRealisticFlower(stemX, y1 + height*0.2f + stemHeight, flowerSize, 0.5f, 1.0f, 0.8f); break; // mint green
            case 14: drawRealisticFlower(stemX, y1 + height*0.2f + stemHeight, flowerSize, 1.0f, 0.8f, 0.9f); break; // pale rose
        }
    }
    
    // Multiple layers of smaller flowers to fill all gaps
    int extraFlowers = (int)(boxWidth * 90.0f); // more bloom flowers
    for (int i = 0; i < extraFlowers; i++) {
        float flowerX = x1 + 0.01f + (boxWidth - 0.02f) * (float)i / (float)(extraFlowers - 1);
        float flowerY = y1 + height*0.22f + height * (0.15f + 0.35f * sinf(flowerX * 32.0f + (float)i * 0.6f));
        float smallFlowerSize = height * (0.025f + 0.025f * sinf(flowerX * 25.0f));
        
        // Small filler flowers in bright summer colors
        switch(i % 8) {
            case 0: drawCircleFilled(flowerX, flowerY, smallFlowerSize, 6, 1.0f, 0.3f, 0.3f, 1.0f); break; // bright red
            case 1: drawCircleFilled(flowerX, flowerY, smallFlowerSize, 6, 1.0f, 1.0f, 0.3f, 1.0f); break; // bright yellow
            case 2: drawCircleFilled(flowerX, flowerY, smallFlowerSize, 6, 0.3f, 0.3f, 1.0f, 1.0f); break; // bright blue
            case 3: drawCircleFilled(flowerX, flowerY, smallFlowerSize, 6, 1.0f, 0.3f, 1.0f, 1.0f); break; // bright magenta
            case 4: drawCircleFilled(flowerX, flowerY, smallFlowerSize, 6, 0.3f, 1.0f, 0.3f, 1.0f); break; // bright green
            case 5: drawCircleFilled(flowerX, flowerY, smallFlowerSize, 6, 1.0f, 0.6f, 0.3f, 1.0f); break; // bright orange
            case 6: drawCircleFilled(flowerX, flowerY, smallFlowerSize, 6, 0.8f, 0.3f, 0.8f, 1.0f); break; // bright purple
            case 7: drawCircleFilled(flowerX, flowerY, smallFlowerSize, 6, 0.3f, 0.8f, 0.8f, 1.0f); break; // bright cyan
        }
    }
    
    // Third layer of tiny flowers to fill remaining gaps
    int tinyFlowers = (int)(boxWidth * 120.0f); // even more tiny blooms
    for (int i = 0; i < tinyFlowers; i++) {
        float flowerX = x1 + 0.007f + (boxWidth - 0.014f) * (float)i / (float)(tinyFlowers - 1);
        float flowerY = y1 + height*0.18f + height * (0.1f + 0.25f * sinf(flowerX * 40.0f + (float)i * 0.9f));
        float tinyFlowerSize = height * (0.015f + 0.015f * sinf(flowerX * 28.0f));
        
        // Tiny flowers in pastel colors
        switch(i % 10) {
            case 0: drawCircleFilled(flowerX, flowerY, tinyFlowerSize, 4, 1.0f, 0.7f, 0.7f, 1.0f); break; // pale red
            case 1: drawCircleFilled(flowerX, flowerY, tinyFlowerSize, 4, 1.0f, 1.0f, 0.7f, 1.0f); break; // pale yellow
            case 2: drawCircleFilled(flowerX, flowerY, tinyFlowerSize, 4, 0.7f, 0.7f, 1.0f, 1.0f); break; // pale blue
            case 3: drawCircleFilled(flowerX, flowerY, tinyFlowerSize, 4, 1.0f, 0.7f, 1.0f, 1.0f); break; // pale magenta
            case 4: drawCircleFilled(flowerX, flowerY, tinyFlowerSize, 4, 0.7f, 1.0f, 0.7f, 1.0f); break; // pale green
            case 5: drawCircleFilled(flowerX, flowerY, tinyFlowerSize, 4, 1.0f, 0.9f, 0.7f, 1.0f); break; // pale orange
            case 6: drawCircleFilled(flowerX, flowerY, tinyFlowerSize, 4, 0.9f, 0.7f, 1.0f, 1.0f); break; // pale purple
            case 7: drawCircleFilled(flowerX, flowerY, tinyFlowerSize, 4, 0.7f, 1.0f, 1.0f, 1.0f); break; // pale cyan
            case 8: drawCircleFilled(flowerX, flowerY, tinyFlowerSize, 4, 1.0f, 0.8f, 0.8f, 1.0f); break; // pale pink
            case 9: drawCircleFilled(flowerX, flowerY, tinyFlowerSize, 4, 0.8f, 1.0f, 0.8f, 1.0f); break; // pale mint
        }
    }
    
    // Add greenery leaves to fill any remaining spaces
    int leaves = (int)(boxWidth * 50.0f);
    for (int i = 0; i < leaves; i++) {
        float leafX = x1 + 0.006f + (boxWidth - 0.012f) * (float)i / (float)(leaves - 1);
        float leafY = y1 + height*0.15f + height * (0.05f + 0.2f * sinf(leafX * 45.0f + (float)i * 1.2f));
        float leafSize = height * (0.01f + 0.02f * sinf(leafX * 35.0f));
        
        // Green leaves in various shades
        switch(i % 5) {
            case 0: drawCircleFilled(leafX, leafY, leafSize, 3, 0.2f, 0.6f, 0.2f, 1.0f); break; // dark green
            case 1: drawCircleFilled(leafX, leafY, leafSize, 3, 0.3f, 0.7f, 0.3f, 1.0f); break; // medium green
            case 2: drawCircleFilled(leafX, leafY, leafSize, 3, 0.4f, 0.8f, 0.4f, 1.0f); break; // light green
            case 3: drawCircleFilled(leafX, leafY, leafSize, 3, 0.1f, 0.5f, 0.1f, 1.0f); break; // very dark green
            case 4: drawCircleFilled(leafX, leafY, leafSize, 3, 0.5f, 0.9f, 0.5f, 1.0f); break; // very light green
        }
    }
}

// Forward declaration for the OPEN sign's text rendering helper
void drawOpenText(float cx, float cy, float letterWidth, float letterHeight, float spacing);

// Utility to draw text-like rectangles for OPEN sign
void drawOpenSign(float cx, float cy, float width, float height) {
    // Sign background (brighter fluorescent effect)
    glColor4f(0.1f, 1.0f, 0.3f, 0.85f); // Brighter green
    glBegin(GL_QUADS);
    glVertex2f(cx - width*0.5f, cy - height*0.5f);
    glVertex2f(cx + width*0.5f, cy - height*0.5f);
    glVertex2f(cx + width*0.5f, cy + height*0.5f);
    glVertex2f(cx - width*0.5f, cy + height*0.5f);
    glEnd();
    
    // Sign border
    glColor3f(0.0f, 0.5f, 0.1f); // Darker green
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(cx - width*0.5f, cy - height*0.5f);
    glVertex2f(cx + width*0.5f, cy - height*0.5f);
    glVertex2f(cx + width*0.5f, cy + height*0.5f);
    glVertex2f(cx - width*0.5f, cy + height*0.5f);
    glEnd();
    
    // "OPEN" text with neon glow effect
    float letterWidth = width * 0.12f;
    float letterHeight = height * 0.6f;
    float spacing = width * 0.18f;

    // Draw glow first (thicker, semi-transparent)
    glColor4f(0.6f, 1.0f, 0.7f, 0.6f); // Light green glow
    glLineWidth(5.0f);
    drawOpenText(cx, cy, letterWidth, letterHeight, spacing);

    // Draw main text (thinner, solid color)
    glColor3f(1.0f, 1.0f, 1.0f); // Bright white text
    glLineWidth(2.0f);
    drawOpenText(cx, cy, letterWidth, letterHeight, spacing);

    glLineWidth(1.0f); // Reset line width
}

// Helper function to draw the OPEN letters
void drawOpenText(float cx, float cy, float letterWidth, float letterHeight, float spacing) {
    // O
    glBegin(GL_LINE_LOOP);
    glVertex2f(cx - spacing*1.5f - letterWidth*0.5f, cy - letterHeight*0.5f);
    glVertex2f(cx - spacing*1.5f + letterWidth*0.5f, cy - letterHeight*0.5f);
    glVertex2f(cx - spacing*1.5f + letterWidth*0.5f, cy + letterHeight*0.5f);
    glVertex2f(cx - spacing*1.5f - letterWidth*0.5f, cy + letterHeight*0.5f);
    glEnd();
    
    // P
    glBegin(GL_LINES);
    glVertex2f(cx - spacing*0.5f - letterWidth*0.5f, cy - letterHeight*0.5f);
    glVertex2f(cx - spacing*0.5f - letterWidth*0.5f, cy + letterHeight*0.5f);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(cx - spacing*0.5f - letterWidth*0.5f, cy);
    glVertex2f(cx - spacing*0.5f + letterWidth*0.5f, cy);
    glVertex2f(cx - spacing*0.5f + letterWidth*0.5f, cy + letterHeight*0.5f);
    glVertex2f(cx - spacing*0.5f - letterWidth*0.5f, cy + letterHeight*0.5f);
    glEnd();
    
    // E
    glBegin(GL_LINES);
    glVertex2f(cx + spacing*0.5f - letterWidth*0.5f, cy - letterHeight*0.5f);
    glVertex2f(cx + spacing*0.5f - letterWidth*0.5f, cy + letterHeight*0.5f);
    glVertex2f(cx + spacing*0.5f - letterWidth*0.5f, cy + letterHeight*0.5f);
    glVertex2f(cx + spacing*0.5f + letterWidth*0.5f, cy + letterHeight*0.5f);
    glVertex2f(cx + spacing*0.5f - letterWidth*0.5f, cy);
    glVertex2f(cx + spacing*0.5f + letterWidth*0.3f, cy);
    glVertex2f(cx + spacing*0.5f - letterWidth*0.5f, cy - letterHeight*0.5f);
    glVertex2f(cx + spacing*0.5f + letterWidth*0.5f, cy - letterHeight*0.5f);
    glEnd();
    
    // N
    glBegin(GL_LINES);
    glVertex2f(cx + spacing*1.5f - letterWidth*0.5f, cy - letterHeight*0.5f);
    glVertex2f(cx + spacing*1.5f - letterWidth*0.5f, cy + letterHeight*0.5f);
    glVertex2f(cx + spacing*1.5f - letterWidth*0.5f, cy + letterHeight*0.5f);
    glVertex2f(cx + spacing*1.5f + letterWidth*0.5f, cy - letterHeight*0.5f);
    glVertex2f(cx + spacing*1.5f + letterWidth*0.5f, cy - letterHeight*0.5f);
    glVertex2f(cx + spacing*1.5f + letterWidth*0.5f, cy + letterHeight*0.5f);
    glEnd();
}

// Utility to draw a chimney-like extractor on the roof
void drawExtractor(float cx, float cy, float width, float height, float slope) {
    // Calculate the y-offsets for the angled base
    float y_offset_left = -width / 2 * slope;
    float y_offset_right = width / 2 * slope;

    // Flashing at the base for integration (angled with the roof)
    float flashingWidth = width * 0.7f;
    float flashingHeight = 0.02f;
    glColor3f(0.35f, 0.12f, 0.12f); // Roof eave/shadow color for better blending
    glBegin(GL_QUADS);
    glVertex2f(cx - flashingWidth, cy - flashingWidth * slope);
    glVertex2f(cx + flashingWidth, cy + flashingWidth * slope);
    glVertex2f(cx + flashingWidth, cy + flashingWidth * slope - flashingHeight);
    glVertex2f(cx - flashingWidth, cy - flashingWidth * slope - flashingHeight);
    glEnd();

    // Main body with metallic gradient and angled base
    glBegin(GL_QUADS);
    glColor3f(0.65f, 0.65f, 0.7f); // Lighter top
    glVertex2f(cx - width / 2, cy + y_offset_left + height);
    glVertex2f(cx + width / 2, cy + y_offset_right + height);
    glColor3f(0.45f, 0.45f, 0.5f); // Darker bottom
    glVertex2f(cx + width / 2, cy + y_offset_right);
    glVertex2f(cx - width / 2, cy + y_offset_left);
    glEnd();

    // Cap on top (adjusted to be horizontal)
    float cap_y = cy + height + (y_offset_left + y_offset_right) / 2.0f;
    drawRectangle(cx - width * 0.6f, cap_y, cx + width * 0.6f, cap_y + 0.02f, 0.3f, 0.3f, 0.3f);
}

// ------------------ Camera Controls ------------------
float scrollX = 0.0f;
float scrollY = 0.0f;
float zoomLevel = 50.0f; // Initial zoom

// ------------------ CLASSES ------------------

class Toilet {
public:
    void Draw(float x, float y, float orientation = 0.0f) {
        // orientation: 0 = tank on right, 1 = tank on left, 2 = tank on top, 3 = tank on bottom

        glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f); // White

        float bowlRadius = 0.2f;
        float innerRadius = 0.12f;
        float tankSize = 0.15f; // Square tank

        if (orientation == 0) { // Tank on right (default)
            // Outer circle (bowl)
            DrawCircle(x, y, bowlRadius, 16);

            // Inner circle
            DrawCircle(x, y, innerRadius, 12);

            // Tank (square on right)
            float tankX = x + bowlRadius + tankSize / 2;
            glVertex2f(tankX - tankSize / 2, y - tankSize / 2); glVertex2f(tankX + tankSize / 2, y - tankSize / 2);
            glVertex2f(tankX + tankSize / 2, y - tankSize / 2); glVertex2f(tankX + tankSize / 2, y + tankSize / 2);
            glVertex2f(tankX + tankSize / 2, y + tankSize / 2); glVertex2f(tankX - tankSize / 2, y + tankSize / 2);
            glVertex2f(tankX - tankSize / 2, y + tankSize / 2); glVertex2f(tankX - tankSize / 2, y - tankSize / 2);

        }
        else if (orientation == 1) { // Tank on left
            // Outer circle (bowl)
            DrawCircle(x, y, bowlRadius, 16);

            // Inner circle
            DrawCircle(x, y, innerRadius, 12);

            // Tank (square on left)
            float tankX = x - bowlRadius - tankSize / 2;
            glVertex2f(tankX - tankSize / 2, y - tankSize / 2); glVertex2f(tankX + tankSize / 2, y - tankSize / 2);
            glVertex2f(tankX + tankSize / 2, y - tankSize / 2); glVertex2f(tankX + tankSize / 2, y + tankSize / 2);
            glVertex2f(tankX + tankSize / 2, y + tankSize / 2); glVertex2f(tankX - tankSize / 2, y + tankSize / 2);
            glVertex2f(tankX - tankSize / 2, y + tankSize / 2); glVertex2f(tankX - tankSize / 2, y - tankSize / 2);

        }
        else if (orientation == 2) { // Tank on top
            // Outer circle (bowl)
            DrawCircle(x, y, bowlRadius, 16);

            // Inner circle
            DrawCircle(x, y, innerRadius, 12);

            // Tank (square on top)
            float tankY = y + bowlRadius + tankSize / 2;
            glVertex2f(x - tankSize / 2, tankY - tankSize / 2); glVertex2f(x + tankSize / 2, tankY - tankSize / 2);
            glVertex2f(x + tankSize / 2, tankY - tankSize / 2); glVertex2f(x + tankSize / 2, tankY + tankSize / 2);
            glVertex2f(x + tankSize / 2, tankY + tankSize / 2); glVertex2f(x - tankSize / 2, tankY + tankSize / 2);
            glVertex2f(x - tankSize / 2, tankY + tankSize / 2); glVertex2f(x - tankSize / 2, tankY - tankSize / 2);

        }
        else if (orientation == 3) { // Tank on bottom
            // Outer circle (bowl)
            DrawCircle(x, y, bowlRadius, 16);

            // Inner circle
            DrawCircle(x, y, innerRadius, 12);

            // Tank (square on bottom)
            float tankY = y - bowlRadius - tankSize / 2;
            glVertex2f(x - tankSize / 2, tankY - tankSize / 2); glVertex2f(x + tankSize / 2, tankY - tankSize / 2);
            glVertex2f(x + tankSize / 2, tankY - tankSize / 2); glVertex2f(x + tankSize / 2, tankY + tankSize / 2);
            glVertex2f(x + tankSize / 2, tankY + tankSize / 2); glVertex2f(x - tankSize / 2, tankY + tankSize / 2);
            glVertex2f(x - tankSize / 2, tankY + tankSize / 2); glVertex2f(x - tankSize / 2, tankY - tankSize / 2);
        }

        glEnd();
    }

private:
    void DrawCircle(float cx, float cy, float radius, int segments) {
        for (int i = 0; i < segments; i++) {
            float theta1 = 2.0f * M_PI * i / segments;
            float theta2 = 2.0f * M_PI * (i + 1) / segments;
            glVertex2f(cx + radius * cos(theta1), cy + radius * sin(theta1));
            glVertex2f(cx + radius * cos(theta2), cy + radius * sin(theta2));
        }
    }
};

class FireExtinguisher {
public:
    void Draw(float x, float y) {
        glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f); // Red color for fire safety

        // Main body (circle from top-down view)
        float radius = 0.15f;
        int segments = 12;

        // Draw circle for extinguisher body
        for (int i = 0; i < segments; i++) {
            float theta1 = 2.0f * M_PI * i / segments;
            float theta2 = 2.0f * M_PI * (i + 1) / segments;
            glVertex2f(x + radius * cos(theta1), y + radius * sin(theta1));
            glVertex2f(x + radius * cos(theta2), y + radius * sin(theta2));
        }

        // Add "FE" text indicator (using lines)
        // F shape
        glVertex2f(x - radius * 0.3f, y + radius * 0.5f); glVertex2f(x - radius * 0.3f, y - radius * 0.5f);
        glVertex2f(x - radius * 0.3f, y + radius * 0.5f); glVertex2f(x - radius * 0.1f, y + radius * 0.5f);
        glVertex2f(x - radius * 0.3f, y); glVertex2f(x - radius * 0.1f, y);

        // E shape  
        glVertex2f(x + radius * 0.1f, y + radius * 0.5f); glVertex2f(x + radius * 0.1f, y - radius * 0.5f);
        glVertex2f(x + radius * 0.1f, y + radius * 0.5f); glVertex2f(x + radius * 0.3f, y + radius * 0.5f);
        glVertex2f(x + radius * 0.1f, y); glVertex2f(x + radius * 0.3f, y);
        glVertex2f(x + radius * 0.1f, y - radius * 0.5f); glVertex2f(x + radius * 0.3f, y - radius * 0.5f);

        glEnd();
    }
};

void DrawDoorArc(float cx, float cy, float radius, float startAngle, float endAngle, int segments = 20) {
    for (int i = 0; i < segments; i++) {
        float theta1 = startAngle + (endAngle - startAngle) * (i / (float)segments);
        float theta2 = startAngle + (endAngle - startAngle) * ((i + 1) / (float)segments);
        glVertex2f(cx + radius * cos(theta1), cy + radius * sin(theta1));
        glVertex2f(cx + radius * cos(theta2), cy + radius * sin(theta2));
    }
}

class Bin {
public:
    void Draw(float x, float y, float size = 0.25f) {
        glBegin(GL_LINES);
        glColor3f(0.6f, 0.6f, 0.6f); // Gray color for bins

        float outerSize = size;
        float innerSize = size * 0.6f; // Smaller inner square

        // Outer square
        glVertex2f(x - outerSize / 2, y - outerSize / 2); glVertex2f(x + outerSize / 2, y - outerSize / 2);
        glVertex2f(x + outerSize / 2, y - outerSize / 2); glVertex2f(x + outerSize / 2, y + outerSize / 2);
        glVertex2f(x + outerSize / 2, y + outerSize / 2); glVertex2f(x - outerSize / 2, y + outerSize / 2);
        glVertex2f(x - outerSize / 2, y + outerSize / 2); glVertex2f(x - outerSize / 2, y - outerSize / 2);

        // Inner square
        glVertex2f(x - innerSize / 2, y - innerSize / 2); glVertex2f(x + innerSize / 2, y - innerSize / 2);
        glVertex2f(x + innerSize / 2, y - innerSize / 2); glVertex2f(x + innerSize / 2, y + innerSize / 2);
        glVertex2f(x + innerSize / 2, y + innerSize / 2); glVertex2f(x - innerSize / 2, y + innerSize / 2);
        glVertex2f(x - innerSize / 2, y + innerSize / 2); glVertex2f(x - innerSize / 2, y - innerSize / 2);

        glEnd();
    }
};

class FloorPlan {
public:
    void Draw() {
        glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f); // White floor plan

        // Outer square
		glVertex2f(-6, -10); glVertex2f(6, -10);// bottom
        glVertex2f(-6, -10); glVertex2f(-6, 10);//left
        glVertex2f(-6, 10); glVertex2f(6, 10);//top
        glVertex2f(6, -10); glVertex2f(6,10);//right     

        //Counter space Kitchen
		glVertex2f(5.4f,9.4f); glVertex2f(1.5f, 9.4f);
        glVertex2f(5.4f, 10.0f); glVertex2f(5.4f, 2.0f); 
        glVertex2f(5.4f, 2.0f); glVertex2f(6.0f, 2.0f);
        //sinks
		glVertex2f(2.5f, 10.0f); glVertex2f(2.5f, 9.4f);
        glVertex2f(1.5f, 9.8f); glVertex2f(2.0f, 9.8f);
        glVertex2f(1.5f, 9.7f); glVertex2f(2.0f, 9.7f);
        glVertex2f(1.5f, 9.6f); glVertex2f(2.0f, 9.6f);
        glVertex2f(1.5f, 9.5f); glVertex2f(2.0f, 9.5f);

        glVertex2f(3.5f, 10.0f); glVertex2f(3.5f, 9.4f);
        glVertex2f(2.5f, 9.8f); glVertex2f(3.0f, 9.8f);
        glVertex2f(2.5f, 9.7f); glVertex2f(3.0f, 9.7f);
        glVertex2f(2.5f, 9.6f); glVertex2f(3.0f, 9.6f);
        glVertex2f(2.5f, 9.5f); glVertex2f(3.0f, 9.5f);

        glVertex2f(4.5f, 10.0f); glVertex2f(4.5f, 9.4f);
        glVertex2f(3.5f, 9.8f); glVertex2f(4.0f, 9.8f);
        glVertex2f(3.5f, 9.7f); glVertex2f(4.0f, 9.7f);
        glVertex2f(3.5f, 9.6f); glVertex2f(4.0f, 9.6f);
        glVertex2f(3.5f, 9.5f); glVertex2f(4.0f, 9.5f);

        //stoves and grills
        glVertex2f(5.5f, 2.1); glVertex2f(5.9f, 2.1f);
        glVertex2f(5.5f, 2.8f); glVertex2f(5.5f, 2.1f);
        glVertex2f(5.9f, 2.1f); glVertex2f(5.9f, 2.8f);
        glVertex2f(5.9f, 2.8f); glVertex2f(5.5f, 2.8f);

        glVertex2f(5.5f, 3.1); glVertex2f(5.9f, 3.1f);
        glVertex2f(5.5f, 3.8f); glVertex2f(5.5f, 3.1f);
        glVertex2f(5.9f, 3.1f); glVertex2f(5.9f, 3.8f);
        glVertex2f(5.9f, 3.8f); glVertex2f(5.5f, 3.8f);

        glVertex2f(5.5f, 4.1); glVertex2f(5.9f, 4.1f);
        glVertex2f(5.5f, 4.8f); glVertex2f(5.5f, 4.1f);
        glVertex2f(5.9f, 4.1f); glVertex2f(5.9f, 4.8f);
        glVertex2f(5.9f, 4.8f); glVertex2f(5.5f, 4.8f);

        glVertex2f(5.5f, 5.1); glVertex2f(5.7f, 5.1f);
        glVertex2f(5.5f, 5.8f); glVertex2f(5.5f, 5.1f);
        glVertex2f(5.7f, 5.1f); glVertex2f(5.7f, 5.8f);
        glVertex2f(5.7f, 5.8f); glVertex2f(5.5f, 5.8f);

        glVertex2f(5.5f, 6.1); glVertex2f(5.7f, 6.1f);
        glVertex2f(5.5f, 6.8f); glVertex2f(5.5f, 6.1f);
        glVertex2f(5.7f, 6.1f); glVertex2f(5.7f, 6.8f);
        glVertex2f(5.7f, 6.8f); glVertex2f(5.5f, 6.8f);

        glVertex2f(5.5f, 7.1); glVertex2f(5.7f, 7.1f);
        glVertex2f(5.5f, 7.8f); glVertex2f(5.5f, 7.1f);
        glVertex2f(5.7f, 7.1f); glVertex2f(5.7f, 7.8f);
        glVertex2f(5.7f, 7.8f); glVertex2f(5.5f, 7.8f);

        //Kitchen counter
		glVertex2f(3.5f, 4.5f); glVertex2f(3.5f, 5.0f);
		glVertex2f(3.5f, 5.0f); glVertex2f(-0.5f, 5.0f);
		glVertex2f(-0.5f, 5.0f); glVertex2f(-0.5f, 4.5f);
		glVertex2f(-0.5f, 4.5f); glVertex2f(3.5f, 4.5f);
        
        glVertex2f(3.5f, 3.0f); glVertex2f(3.5f, 3.5f);
        glVertex2f(3.5f, 3.5f); glVertex2f(-0.5f, 3.5f);
        glVertex2f(-0.5f, 3.5f); glVertex2f(-0.5f, 3.0f);
        glVertex2f(-0.5f, 3.0f); glVertex2f(3.5f, 3.0f);

        //Bottom windows
        glVertex2f(-6, -9.85); glVertex2f(-1.05, -9.85);
        glVertex2f(-4.7625, -9.85); glVertex2f(-4.7625, -10);
        glVertex2f(-3.525, -9.85); glVertex2f(-3.525, -10);
        glVertex2f(-2.2875, -9.85); glVertex2f(-2.2875, -10);
        glVertex2f(-1.05, -9.85); glVertex2f(-1.05, -10);

        glVertex2f(6, -9.85); glVertex2f(1.05, -9.85);
        glVertex2f(4.7625, -9.85); glVertex2f(4.7625, -10);
        glVertex2f(3.525, -9.85); glVertex2f(3.525, -10);
        glVertex2f(2.2875, -9.85); glVertex2f(2.2875, -10);
        glVertex2f(1.05, -9.85); glVertex2f(1.05, -10);

        //bottom doors
        glVertex2f(-1.05f, -10.0f);  glVertex2f(-1.05f, -8.95f);
        DrawDoorArc(-1.05f, -10.0f, 1.05f, 0.0f, M_PI / 2.0f);
        glVertex2f(1.05f, -10.0f);  glVertex2f(1.05f, -8.95f);
        DrawDoorArc(1.05f, -10.0f, -1.05f, 0.0f, M_PI / -2.0f);

        //West windows 1.225
        glVertex2f(-5.85, -10); glVertex2f(-5.85, -0.2);
        glVertex2f(-5.85, -0.2); glVertex2f(-6, -0.2);
        glVertex2f(-5.85, -1.425); glVertex2f(-6, -1.425);
        glVertex2f(-5.85, -2.65); glVertex2f(-6, -2.65);
        glVertex2f(-5.85, -3.875); glVertex2f(-6, -3.875);
        glVertex2f(-5.85, -5.1); glVertex2f(-6, -5.1);
		glVertex2f(-5.85, -6.325); glVertex2f(-6, -6.325);
		glVertex2f(-5.85, -7.55); glVertex2f(-6, -7.55);
		glVertex2f(-5.85, -8.775); glVertex2f(-6, -8.775);
		glVertex2f(-5.85, -9.85); glVertex2f(-6, -9.85);
		glVertex2f(-5.85f, -0.2f); glVertex2f(-5.85f, 4.414f);
		glVertex2f(-5.85f, 4.414f); glVertex2f(-6, 4.414f);
        glVertex2f(-5.85f, 4.414f); glVertex2f(-6, 3.637f);
		glVertex2f(-5.85f, 3.637f); glVertex2f(-6, 3.637f);
		glVertex2f(-5.85f, 2.86f); glVertex2f(-6, 2.86f);
        glVertex2f(-5.85f, 2.86f); glVertex2f(-6, 2.083f);
        glVertex2f(-5.85f, 2.083f); glVertex2f(-6, 2.083f);
		glVertex2f(-5.85f, 1.306f); glVertex2f(-6, 1.306f);
		glVertex2f(-5.85f, 1.306f); glVertex2f(-6, 0.529f);
		glVertex2f(-5.85f, 0.529f); glVertex2f(-6, 0.529f);
        

        //Wall separating dining area and kithcen
		glVertex2f(-6, -0.2); glVertex2f(6, -0.2);

        //Door to Office
        glVertex2f(-5.8f, -0.2f);  glVertex2f(-5.8f, 0.8f);
        DrawDoorArc(-5.8f, -0.2f, 1.0f, 0.0f, M_PI / 2.0f);
        glVertex2f(-3.8f, 3.5f); glVertex2f(-3.0f, 3.5f); 
        DrawDoorArc(-3.0f, 3.5f, -0.8f, 0.0f, M_PI / 2.0f);


        //Office Walls
		glVertex2f(-6, 4.414); glVertex2f(-3, 4.414);
        glVertex2f(-3, -0.2); glVertex2f(-3, 4.414);

        // ===== Desk (shifted left) =====
// Rectangle desk along the wall (centered near top wall)
        glVertex2f(-5.6f, 3.7f); glVertex2f(-4.2f, 3.7f);  // top edge
        glVertex2f(-5.6f, 3.3f); glVertex2f(-4.2f, 3.3f);  // bottom edge
        glVertex2f(-5.6f, 3.7f); glVertex2f(-5.6f, 3.3f);  // left side
        glVertex2f(-4.2f, 3.7f); glVertex2f(-4.2f, 3.3f);  // right side

        // ===== Chair Blocks (0.4 x 0.4) =====
        // Chair below the desk
        glVertex2f(-5.0f, 2.5f); glVertex2f(-4.6f, 2.5f);
        glVertex2f(-5.0f, 2.1f); glVertex2f(-4.6f, 2.1f);
        glVertex2f(-5.0f, 2.5f); glVertex2f(-5.0f, 2.1f);
        glVertex2f(-4.6f, 2.5f); glVertex2f(-4.6f, 2.1f);

        // Chair above the desk
        glVertex2f(-5.0f, 4.1f); glVertex2f(-4.6f, 4.1f);
        glVertex2f(-5.0f, 3.7f); glVertex2f(-4.6f, 3.7f);
        glVertex2f(-5.0f, 4.1f); glVertex2f(-5.0f, 3.7f);
        glVertex2f(-4.6f, 4.1f); glVertex2f(-4.6f, 3.7f);


        //Door between office and storage
		glVertex2f(-6.0f, 5.0f);  glVertex2f(-7.0f, 5.0f);
        DrawDoorArc(-6.0f, 5.0f, -1.0f, 0.0f, M_PI / -2.0f);

		//Dry Storage Walls
		glVertex2f(-6.0, 7.0); glVertex2f(-3.0, 7.0);
        glVertex2f(-3.0, 7.0); glVertex2f(-3.0, 10.0);
        //Door to Storage
        glVertex2f(-5.8f, 7.0f);  glVertex2f(-5.8f, 8.0f);
        DrawDoorArc(-5.8f, 7.0f, 1.0f, 0.0f, M_PI / 2.0f);

		//Freezer
        glVertex2f(-0.5, 10.0); glVertex2f(-0.5, 7.0);
        glVertex2f(-0.5, 7.0); glVertex2f(1.5, 7.0);
        glVertex2f(1.5, 7.0); glVertex2f(1.5, 10.0);
        //Freezer Door 
        glVertex2f(-0.3f, 7.0f);  glVertex2f(-0.3f, 8.0f);
        DrawDoorArc(-0.3f, 7.0f, 1.0f, 0.0f, M_PI / 2.0f);

        //East windows
        glVertex2f(5.85, -7.55); glVertex2f(6, -7.55);
        glVertex2f(5.85, -8.775); glVertex2f(6, -8.775);
        glVertex2f(5.85, -9.85); glVertex2f(6, -9.85);
        glVertex2f(5.85, -6.325); glVertex2f(6, -6.325);
        glVertex2f(5.85, -10); glVertex2f(5.85, -6.325);

        //Toilet Walls
        glVertex2f(6, -6.325); glVertex2f(2.2875, -6.325);
        glVertex2f(2.2875, -6.325); glVertex2f(2.2875, -4.1625);
        glVertex2f(2.2875, -3.1625); glVertex2f(6, -3.1625);
        glVertex2f(2.2875, -3.1625); glVertex2f(2.2875, -1.2);
		glVertex2f(4.14075, -1.2); glVertex2f(6, -1.2);
        glVertex2f(4.14075, -2.2); glVertex2f(6, -2.2);
        glVertex2f(4.14075, -4.1625); glVertex2f(6, -4.1625);
        glVertex2f(4.14075, -5.1625); glVertex2f(6, -5.1625);

        // Sink 1 (upper)
        glVertex2f(2.35, -1.8); glVertex2f(2.75, -1.8);
        glVertex2f(2.75, -1.8); glVertex2f(2.75, -1.4);
        glVertex2f(2.35, -1.4); glVertex2f(2.75, -1.4);
        glVertex2f(2.35, -1.8); glVertex2f(2.35, -1.4);

        // Sink 2 (just below it)
        glVertex2f(2.35, -2.7); glVertex2f(2.75, -2.7);
        glVertex2f(2.75, -2.7); glVertex2f(2.75, -2.3);
        glVertex2f(2.35, -2.3); glVertex2f(2.75, -2.3);
        glVertex2f(2.35, -2.7); glVertex2f(2.35, -2.3);

		//Sink 3 (upper)
        glVertex2f(2.35, -4.8); glVertex2f(2.75, -4.8);
        glVertex2f(2.75, -4.8); glVertex2f(2.75, -4.4);
        glVertex2f(2.35, -4.4); glVertex2f(2.75, -4.4);
        glVertex2f(2.35, -4.8); glVertex2f(2.35, -4.4);

        // Sink 4 (just below it)
        glVertex2f(2.35, -5.7); glVertex2f(2.75, -5.7);
        glVertex2f(2.75, -5.7); glVertex2f(2.75, -5.3);
        glVertex2f(2.35, -5.3); glVertex2f(2.75, -5.3);
        glVertex2f(2.35, -5.7); glVertex2f(2.35, -5.3);

		//Toilet Doors 1
        glVertex2f(2.2875, -0.2); glVertex2f(2.2875, -1.2);
        DrawDoorArc(2.2875, -0.2, 1.0f, 0.0f, M_PI / -2.0f);

        glVertex2f(4.14075, -0.2); glVertex2f(4.14075, -1.2);
        DrawDoorArc(4.14075, -0.2, 1.0f, 0.0f, M_PI / -2.0f);

        glVertex2f(4.14075, -1.2); glVertex2f(4.14075, -2.2);
        DrawDoorArc(4.14075, -1.2, 1.0f, 0.0f, M_PI / -2.0f);

        glVertex2f(4.14075, -2.2); glVertex2f(4.14075, -3.2);
        DrawDoorArc(4.14075, -2.2, 1.0f, 0.0f, M_PI / -2.0f);
        
        //Toilet Doors 2
        glVertex2f(2.2875, -3.2); glVertex2f(2.2875, -4.2);
        DrawDoorArc(2.2875, -3.2, 1.0f, 0.0f, M_PI / -2.0f);

        glVertex2f(4.14075, -3.2); glVertex2f(4.14075, -4.2);
        DrawDoorArc(4.14075, -3.2, 1.0f, 0.0f, M_PI / -2.0f);

        glVertex2f(4.14075, -4.2); glVertex2f(4.14075, -5.2);
        DrawDoorArc(4.14075, -4.2, 1.0f, 0.0f, M_PI / -2.0f);

        glVertex2f(4.14075, -5.2); glVertex2f(4.14075, -6.2);
        DrawDoorArc(4.14075, -5.2, 1.0f, 0.0f, M_PI / -2.0f);

       //Doors to kitchen
       // Left door
        glVertex2f(-2.05f, -0.2f);  glVertex2f(-2.05f, -1.2f);
        DrawDoorArc(-2.05f, -0.2f, 1.025f, 0.0f, -M_PI / 2.0f);

        // Right door
        glVertex2f(0.0f, -0.2f);  glVertex2f(0.0f, -1.2f);
        DrawDoorArc(0.0f, -0.2f, 1.025f, M_PI, 3.0f * M_PI / 2.0f);
        //Privacy wall 
        glVertex2f(1.05f, 2.0f);  glVertex2f(-3.0f, 2.0f);

        //Loading Doors
        glVertex2f(-2.7f, 10.0f);  glVertex2f(-2.7f, 11.0);
        DrawDoorArc(-2.7f, 10.0f, 1.0f, 0.0f, M_PI / 2.0f);
        glVertex2f(-0.70f, 10.0f);  glVertex2f(-0.70f, 11.0f);
        DrawDoorArc(-0.70f, 10.0f, -1.0f, 0.0f, M_PI / -2.0f);

		// Dining tables
		glVertex2f(-4.0f, -4.0f); glVertex2f(-3.0f, -4.0f);
            glVertex2f(-3.7, -4.0); glVertex2f(-3.7f, -4.4);
            glVertex2f(-3.7f, -4.4); glVertex2f(-3.3f, -4.4);
            glVertex2f(-3.3f, -4.4); glVertex2f(-3.3f, -4.0);
		glVertex2f(-4.0f, -3.0f); glVertex2f(-3.0f, -3.0f);
            glVertex2f(-3.7f,-3.0f); glVertex2f(-3.7f, -2.6f);
            glVertex2f(-3.7f, -2.6f); glVertex2f(-3.3f, -2.6f);
            glVertex2f(-3.3f, -2.6f); glVertex2f(-3.3f, -3.0f);
		glVertex2f(-4.0f, -4.0f); glVertex2f(-4.0f, -3.0f);
            glVertex2f(-4.0f, -3.7f); glVertex2f(-4.4f, -3.7f);
            glVertex2f(-4.4f, -3.7f); glVertex2f(-4.4f, -3.3f);
            glVertex2f(-4.4f, -3.3f); glVertex2f(-4.0f, -3.3f);
		glVertex2f(-3.0f, -4.0f); glVertex2f(-3.0f, -3.0f);
            glVertex2f(-3.0f, -3.7f); glVertex2f(-2.6f, -3.7f);
            glVertex2f(-2.6f, -3.7f); glVertex2f(-2.6f, -3.3f);
            glVertex2f(-2.6f, -3.3f); glVertex2f(-3.0f, -3.3f);

            glVertex2f(-4.0f, -7.0f); glVertex2f(-3.0f, -7.0f);
            glVertex2f(-3.7, -7.0); glVertex2f(-3.7f, -7.4);
            glVertex2f(-3.7f, -7.4); glVertex2f(-3.3f, -7.4);
            glVertex2f(-3.3f, -7.4); glVertex2f(-3.3f, -7.0);
            glVertex2f(-4.0f, -6.0f); glVertex2f(-3.0f, -6.0f);
            glVertex2f(-3.7f, -6.0f); glVertex2f(-3.7f, -5.6f);
            glVertex2f(-3.7f, -5.6f); glVertex2f(-3.3f, -5.6f);
            glVertex2f(-3.3f, -5.6f); glVertex2f(-3.3f, -6.0f);
            glVertex2f(-4.0f, -7.0f); glVertex2f(-4.0f, -6.0f);
            glVertex2f(-4.0f, -6.7f); glVertex2f(-4.4f, -6.7f);
            glVertex2f(-4.4f, -6.7f); glVertex2f(-4.4f, -6.3f);
            glVertex2f(-4.4f, -6.3f); glVertex2f(-4.0f, -6.3f);
            glVertex2f(-3.0f, -7.0f); glVertex2f(-3.0f, -6.0f);
            glVertex2f(-3.0f, -6.7f); glVertex2f(-2.6f, -6.7f);
            glVertex2f(-2.6f, -6.7f); glVertex2f(-2.6f, -6.3f);
            glVertex2f(-2.6f, -6.3f); glVertex2f(-3.0f, -6.3f);

            glVertex2f(-1.0f, -4.0f); glVertex2f(-0.0f, -4.0f);
            glVertex2f(-0.7, -4.0); glVertex2f(-0.7f, -4.4);
            glVertex2f(-0.7f, -4.4); glVertex2f(-0.3f, -4.4);
            glVertex2f(-0.3f, -4.4); glVertex2f(-0.3f, -4.0);
            glVertex2f(-1.0f, -3.0f); glVertex2f(-0.0f, -3.0f);
            glVertex2f(-0.7f, -3.0f); glVertex2f(-0.7f, -2.6f);
            glVertex2f(-0.7f, -2.6f); glVertex2f(-0.3f, -2.6f);
            glVertex2f(-0.3f, -2.6f); glVertex2f(-0.3f, -3.0f);
            glVertex2f(-1.0f, -4.0f); glVertex2f(-1.0f, -3.0f);
            glVertex2f(-1.0f, -3.7f); glVertex2f(-1.4f, -3.7f);
            glVertex2f(-1.4f, -3.7f); glVertex2f(-1.4f, -3.3f);
            glVertex2f(-1.4f, -3.3f); glVertex2f(-1.0f, -3.3f);
            glVertex2f(-0.0f, -4.0f); glVertex2f(-0.0f, -3.0f);
            glVertex2f(-0.0f, -3.7f); glVertex2f(0.4f, -3.7f);
            glVertex2f(0.4f, -3.7f); glVertex2f(0.4f, -3.3f);
            glVertex2f(0.4f, -3.3f); glVertex2f(0.0f, -3.3f);

            glVertex2f(-1.0f, -7.0f); glVertex2f(-0.0f, -7.0f);
            glVertex2f(-0.7, -7.0); glVertex2f(-0.7f, -7.4);
            glVertex2f(-0.7f, -7.4); glVertex2f(-0.3f, -7.4);
            glVertex2f(-0.3f, -7.4); glVertex2f(-0.3f, -7.0);
            glVertex2f(-1.0f, -6.0f); glVertex2f(-0.0f, -6.0f);
            glVertex2f(-0.7f, -6.0f); glVertex2f(-0.7f, -5.6f);
            glVertex2f(-0.7f, -5.6f); glVertex2f(-0.3f, -5.6f);
            glVertex2f(-0.3f, -5.6f); glVertex2f(-0.3f, -6.0f);
            glVertex2f(-1.0f, -7.0f); glVertex2f(-1.0f, -6.0f);
            glVertex2f(-1.0f, -6.7f); glVertex2f(-1.4f, -6.7f);
            glVertex2f(-1.4f, -6.7f); glVertex2f(-1.4f, -6.3f);
            glVertex2f(-1.4f, -6.3f); glVertex2f(-1.0f, -6.3f);
            glVertex2f(-0.0f, -7.0f); glVertex2f(-0.0f, -6.0f);
            glVertex2f(-0.0f, -6.7f); glVertex2f(0.4f, -6.7f);
            glVertex2f(0.4f, -6.7f); glVertex2f(0.4f, -6.3f);
            glVertex2f(0.4f, -6.3f); glVertex2f(0.0f, -6.3f);

            glVertex2f(3.6f, -8.6f); glVertex2f(4.6f, -8.6f);
            glVertex2f(3.9f, -8.6f); glVertex2f(3.9f, -9.0f);
            glVertex2f(3.9f, -9.0f); glVertex2f(4.3f, -9.0f);
            glVertex2f(4.3f, -9.0f); glVertex2f(4.3f, -8.6f);

            glVertex2f(3.6f, -7.6f); glVertex2f(4.6f, -7.6f);
            glVertex2f(3.9f, -7.6f); glVertex2f(3.9f, -7.2f);
            glVertex2f(3.9f, -7.2f); glVertex2f(4.3f, -7.2f);
            glVertex2f(4.3f, -7.2f); glVertex2f(4.3f, -7.6f);

            glVertex2f(3.6f, -8.6f); glVertex2f(3.6f, -7.6f);
            glVertex2f(3.6f, -8.3f); glVertex2f(3.2f, -8.3f);
            glVertex2f(3.2f, -8.3f); glVertex2f(3.2f, -7.9f);
            glVertex2f(3.2f, -7.9f); glVertex2f(3.6f, -7.9f);

            glVertex2f(4.6f, -8.6f); glVertex2f(4.6f, -7.6f);
            glVertex2f(4.6f, -8.3f); glVertex2f(5.0f, -8.3f);
            glVertex2f(5.0f, -8.3f); glVertex2f(5.0f, -7.9f);
            glVertex2f(5.0f, -7.9f); glVertex2f(4.6f, -7.9f);

            //That thing that i cant remember the name of right now
			glVertex2f(0.5f, -8.0f); glVertex2f(0.5f, -8.5f);
            glVertex2f(0.5f, -8.5f); glVertex2f(1.0f, -8.5f);
            glVertex2f(1.0f, -8.5f); glVertex2f(1.0f, -8.0f);
			glVertex2f(1.0f, -8.0f); glVertex2f(0.5f, -8.0f);

            //Storage Details
            glVertex2f(-5.5f, 9.8f); glVertex2f(-3.5f, 9.8f); 
            glVertex2f(-5.5f, 9.8f); glVertex2f(-5.5f, 9.2f); 
            glVertex2f(-3.5f, 9.8f); glVertex2f(-3.5f, 9.2f); 
            glVertex2f(-5.5f, 9.2f); glVertex2f(-3.5f, 9.2f);
            glVertex2f(-5.0f, 9.8f); glVertex2f(-5.0f, 9.2f);
            glVertex2f(-4.5f, 9.8f); glVertex2f(-4.5f, 9.2f);
            glVertex2f(-4.0f, 9.8f); glVertex2f(-4.0f, 9.2f);

            //Freezer
            glVertex2f(-0.3f, 9.8f); glVertex2f(1.3f, 9.8f);   // Back of shelving
            glVertex2f(-0.3f, 9.8f); glVertex2f(-0.3f, 9.4f);  // Left side
            glVertex2f(1.3f, 9.8f); glVertex2f(1.3f, 9.4f);    // Right side
            glVertex2f(-0.3f, 9.4f); glVertex2f(1.3f, 9.4f);   // Front of shelving

            // Shelf divider
            glVertex2f(0.5f, 9.8f); glVertex2f(0.5f, 9.4f);
            
            //Fridge
            glVertex2f(3.0f, 0.5f); glVertex2f(5.0f, 0.5f);  // Front of fridge
            glVertex2f(3.0f, 0.5f); glVertex2f(3.0f, 0.0f);  // Left side
            glVertex2f(5.0f, 0.5f); glVertex2f(5.0f, 0.0f);  // Right side
            glVertex2f(3.0f, 0.0f); glVertex2f(5.0f, 0.0f);  // Back against wall

            // Door division (center line)
            glVertex2f(4.0f, -0.05f); glVertex2f(4.0f, -0.05f);

            glVertex2f(3.0f, -0.1f); glVertex2f(5.0f, -0.1f);


            FireExtinguisher extinguisher;

            // Kitchen area
            extinguisher.Draw(1.7f, 8.0f);
            extinguisher.Draw(5.7f, 0.0f);
            extinguisher.Draw(-5.8f, 4.8f);
            //Office
            extinguisher.Draw(-3.7f, 0.0f);
            //Dinning
            extinguisher.Draw(-2.0f, -9.6f);

            Toilet toilet;

            // Upper bathroom
            toilet.Draw(5.65f, -1.6f, 0);
            toilet.Draw(5.65f, -0.6f, 0);
            toilet.Draw(5.65f, -2.6f, 0);
            

            // Lower bathroom  
            toilet.Draw(5.65f, -4.6f, 0);
            toilet.Draw(5.65f, -3.6f, 0);
            toilet.Draw(5.65f, -5.6f, 0);
           
            Bin bin;

            // Kitchen
            bin.Draw(1.7f, 7.5f);
            bin.Draw(-0.7f, 4.8f);
            bin.Draw(-0.7f, 3.2f);
            bin.Draw(3.7f, 4.8f);
            bin.Draw(3.7f, 3.2f);

            // Office
			bin.Draw(-3.7f, 3.8f);

            //Bathrooms 
			bin.Draw(3.5f, -3.0f);
            bin.Draw(3.5f, -6.0f);

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
        // Position the restaurant front view in the front elevation area
        float offsetY = -45.0f; // center vertically below floor plan
        float offsetX = 0.0f;   // center horizontally
        float scale = 12.0f;    // scale to fit in the elevation area
        
        glPushMatrix();
        glTranslatef(offsetX, offsetY, 0.0f);
        glScalef(scale, scale, 1.0f);
        
        // Enable alpha blending for transparency effects
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        
        // Black background for the restaurant view
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(-1.5f, -0.8f);
        glVertex2f(1.5f, -0.8f);
        glVertex2f(1.5f, 1.2f);
        glVertex2f(-1.5f, 1.2f);
        glEnd();
        
        // Main building rectangle (beige color)
        drawRectangle(-1.2f, -0.5f, 1.2f, 0.3f, 0.96f, 0.87f, 0.70f); // beige

        // ---- Modern Glass Door (matching reference image) ----
        // Main door frame (orange/brown)
        drawRectangle(-0.25f, -0.5f, 0.25f, 0.15f, 0.8f, 0.5f, 0.2f);
        
        // Large glass panels (blue tinted like the image)
        drawRectangle(-0.23f, -0.48f, -0.02f, 0.13f, 0.6f, 0.75f, 0.9f); // left glass
        drawRectangle(0.02f, -0.48f, 0.23f, 0.13f, 0.6f, 0.75f, 0.9f); // right glass
        
        // Vertical door frames
        drawRectangle(-0.25f, -0.5f, -0.23f, 0.15f, 0.8f, 0.5f, 0.2f); // left frame
        drawRectangle(0.23f, -0.5f, 0.25f, 0.15f, 0.8f, 0.5f, 0.2f); // right frame
        drawRectangle(-0.02f, -0.5f, 0.02f, 0.15f, 0.8f, 0.5f, 0.2f); // center frame
        
        // Horizontal frames
        drawRectangle(-0.25f, -0.5f, 0.25f, -0.48f, 0.8f, 0.5f, 0.2f); // bottom
        drawRectangle(-0.25f, 0.13f, 0.25f, 0.15f, 0.8f, 0.5f, 0.2f); // top
        
        // Door handles (modern style)
        drawRectangle(-0.04f, -0.15f, -0.03f, -0.05f, 0.3f, 0.3f, 0.3f); // left handle
        drawRectangle(0.03f, -0.15f, 0.04f, -0.05f, 0.3f, 0.3f, 0.3f); // right handle

        // Horizontal window (aligned with tops of vertical windows) with frame
        drawRectangle(-0.23f, 0.16f, 0.23f, 0.26f, 0.8f, 0.5f, 0.2f); // orange frame
        drawRectangle(-0.21f, 0.17f, 0.21f, 0.25f, 0.6f, 0.75f, 0.9f); // blue glass
        
        // OPEN sign on the horizontal window
        drawOpenSign(0.0f, 0.21f, 0.25f, 0.06f);

        // ---- Vertical windows ----
        float windowWidth = 0.20f; // thinner window panes
        float gap = 0.02f;

        // Compute centered layout for side windows (within main building rectangle)
        float buildingLeft = -1.2f;
        float buildingRight = 1.2f;
        float doorLeft = -0.25f; // expanded for spacing
        float doorRight = 0.25f;  // expanded for spacing
        float leftContainerStart = buildingLeft;
        float leftContainerEnd = doorLeft;
        float rightContainerStart = doorRight;
        float rightContainerEnd = buildingRight;
        float leftContainerWidth = leftContainerEnd - leftContainerStart;   // -0.18 - (-1.2) = 1.02
        float rightContainerWidth = rightContainerEnd - rightContainerStart; // 1.2 - 0.18 = 1.02
        int windowsPerSide = 4;
        float totalWindowsWidth = windowsPerSide * windowWidth + (windowsPerSide - 1) * gap;
        // Center windows properly with some spacing from walls
        float wallSpacing = 0.05f; // smaller spacing for better alignment
        float leftAvailableWidth = leftContainerWidth - (2 * wallSpacing);
        float rightAvailableWidth = rightContainerWidth - (2 * wallSpacing);
        float leftStartX = leftContainerStart + wallSpacing + (leftAvailableWidth - totalWindowsWidth) * 0.5f;
        float rightStartX = rightContainerStart + wallSpacing + (rightAvailableWidth - totalWindowsWidth) * 0.5f;

        // Left side windows (centered within left side of building) with frames
        for (int i = 0; i < windowsPerSide; i++) {
            float x = leftStartX + i * (windowWidth + gap);
            // Window frame
            drawRectangle(x - 0.01f, -0.49f, x + windowWidth + 0.01f, 0.26f, 0.8f, 0.5f, 0.2f);
            // Glass
            drawRectangle(x, -0.48f, x + windowWidth, 0.25f, 0.6f, 0.75f, 0.9f);
        }

        // Right side windows (mirrored and centered within right side of building) with frames
        for (int i = 0; i < windowsPerSide; i++) {
            float x = rightStartX + i * (windowWidth + gap);
            // Window frame
            drawRectangle(x - 0.01f, -0.49f, x + windowWidth + 0.01f, 0.26f, 0.8f, 0.5f, 0.2f);
            // Glass
            drawRectangle(x, -0.48f, x + windowWidth, 0.25f, 0.6f, 0.75f, 0.9f);
        }
        
        // Flower boxes aligned with windows (same width as windows)
        float boxHeight = 0.12f;
        float boxY = -0.5f; // aligned with building base
        
        // Left side flower boxes (aligned with left windows)
        for (int i = 0; i < windowsPerSide; i++) {
            float x = leftStartX + i * (windowWidth + gap);
            drawFlowerBox(x, boxY, x + windowWidth, boxY, boxHeight);
        }
        
        // Right side flower boxes (aligned with right windows)
        for (int i = 0; i < windowsPerSide; i++) {
            float x = rightStartX + i * (windowWidth + gap);
            drawFlowerBox(x, boxY, x + windowWidth, boxY, boxHeight);
        }

        // ---- Roof triangle ----
        // Slightly darker gradient for better cohesion with eave shadow
        glBegin(GL_TRIANGLES);
        glColor3f(0.82f, 0.48f, 0.48f); // base left
        glVertex2f(-1.3f, 0.3f);
        glColor3f(0.82f, 0.48f, 0.48f); // base right
        glVertex2f(1.3f, 0.3f);
        glColor3f(0.90f, 0.62f, 0.62f); // towards the peak
        glVertex2f(0.0f, 1.0f);
        glEnd();

        // Eave cap along the bottom edge of the roof (slight overhang)
        glBegin(GL_QUADS);
        glColor3f(0.35f, 0.12f, 0.12f);
        glVertex2f(-1.32f, 0.29f);
        glVertex2f(1.32f, 0.29f);
        glVertex2f(1.30f, 0.33f);
        glVertex2f(-1.30f, 0.33f);
        glEnd();

        // Soft roof shadow on the building facade
        glBegin(GL_QUADS);
        glColor4f(0.0f, 0.0f, 0.0f, 0.20f);
        glVertex2f(-1.2f, 0.30f);
        glVertex2f(1.2f, 0.30f);
        glVertex2f(1.2f, 0.24f);
        glVertex2f(-1.2f, 0.24f);
        glEnd();

        // Roof tiling texture with overlapping dimension
        glColor3f(0.6f, 0.25f, 0.25f); // tile definition lines
        glLineWidth(1.0f);
        
        // Smaller tile dimensions
        float tileRowSpacing = 0.04f; // smaller rows
        float roofBaseY = 0.3f;
        float roofPeakY = 1.0f;
        float roofHalfWidth = 1.3f;
        
        // Draw overlapping tile effect with multiple passes
        int row = 0;
        for (float y = roofBaseY + tileRowSpacing; y < roofPeakY - 0.03f; y += tileRowSpacing, row++) {
            float yProgress = (y - roofBaseY) / (roofPeakY - roofBaseY);
            float widthAtY = roofHalfWidth * (1.0f - yProgress);
            
            // Draw tile shadows first (overlapping effect - lighter)
            glColor3f(0.65f, 0.35f, 0.35f); // lighter shadow
            glBegin(GL_LINES);
            glVertex2f(-widthAtY + 0.02f, y - 0.005f); // shadow line slightly below
            glVertex2f(widthAtY - 0.02f, y - 0.005f);
            glEnd();
            
            // Draw main tile line (lighter)
            glColor3f(0.75f, 0.45f, 0.45f);
            glBegin(GL_LINES);
            glVertex2f(-widthAtY + 0.02f, y);
            glVertex2f(widthAtY - 0.02f, y);
            glEnd();
        }
        
        // Vertical tile separators (smaller, staggered pattern)
        float tileWidth = 0.08f; // smaller individual tiles
        row = 0;
        for (float y = roofBaseY + tileRowSpacing * 0.5f; y < roofPeakY - 0.08f; y += tileRowSpacing, row++) {
            float yProgress = (y - roofBaseY) / (roofPeakY - roofBaseY);
            float widthAtY = roofHalfWidth * (1.0f - yProgress);
            
            // Stagger every other row for realistic overlap
            float offset = (row % 2 == 0) ? 0.0f : tileWidth * 0.5f;
            
            // Draw vertical separators with shadow effect
            for (float x = -widthAtY + offset; x < widthAtY - 0.02f; x += tileWidth) {
                if (x > -widthAtY + 0.02f && x < widthAtY - 0.02f) {
                    // Shadow line (lighter)
                    glColor3f(0.65f, 0.35f, 0.35f);
                    glBegin(GL_LINES);
                    glVertex2f(x + 0.002f, y - tileRowSpacing * 0.3f); // offset shadow
                    glVertex2f(x + 0.002f, y + tileRowSpacing * 0.3f);
                    glEnd();
                    
                    // Main separator line (lighter)
                    glColor3f(0.75f, 0.45f, 0.45f);
                    glBegin(GL_LINES);
                    glVertex2f(x, y - tileRowSpacing * 0.3f);
                    glVertex2f(x, y + tileRowSpacing * 0.3f);
                    glEnd();
                }
            }
        }
        
        // Ridge cap with dimensional effect
        glLineWidth(2.5f);
        // Ridge shadow
        glColor3f(0.3f, 0.10f, 0.10f);
        glBegin(GL_LINES);
        glVertex2f(-0.07f, 0.955f);
        glVertex2f( 0.07f, 0.955f);
        glEnd();
        
        // Main ridge cap
        glColor3f(0.4f, 0.15f, 0.15f);
        glBegin(GL_LINES);
        glVertex2f(-0.06f, 0.96f);
        glVertex2f( 0.06f, 0.96f);
        glEnd();
        glLineWidth(1.0f);

        // ---- Extractor on the roof ----
        // Position it on the right slope of the roof, closer to the top
        float extractorX = 0.4f;
        float roofSlope = (1.0f - 0.3f) / (0.0f - 1.3f);
        float extractorY = roofSlope * (extractorX - 1.3f) + 0.3f;
        drawExtractor(extractorX, extractorY, 0.08f, 0.12f, roofSlope);
        
        glPopMatrix();
    }
};

class LeftElevation {
public:
    void Draw() {
        glPushMatrix();

        float offsetX = -45.0f;
        float offsetY = 0.0f;

        float wallHeightGL = 15.0f;
        float wallWidthGL = 20.0f;

        float originalHeight = 0.6f;
        float originalWidth = 1.7f;

        float scaleY = wallHeightGL / originalHeight;
        float scaleX = wallWidthGL / originalWidth;

        drawWalls(scaleX, scaleY, offsetX, offsetY);
        drawRoof(scaleX, scaleY, offsetX, offsetY);
        drawDoor(scaleX, scaleY, offsetX, offsetY, wallWidthGL);
        drawWindows(scaleX, scaleY, offsetX, offsetY, wallWidthGL);
        drawGlassPanels(scaleX, scaleY, offsetX, offsetY, wallWidthGL);

        glPopMatrix();
    }

private:
    void drawWalls(float sx, float sy, float ox, float oy) {
        glColor3f(0.78f, 0.72f, 0.65f);
        glBegin(GL_POLYGON);
        glVertex2f(-0.85f * sx + ox, -0.5f * sy + oy);
        glVertex2f(0.85f * sx + ox, -0.5f * sy + oy);
        glVertex2f(0.85f * sx + ox, 0.1f * sy + oy);
        glVertex2f(-0.85f * sx + ox, 0.1f * sy + oy);
        glEnd();
    }

    void drawRoof(float sx, float sy, float ox, float oy) {
        glColor3f(0.95f, 0.45f, 0.40f);

        float roofOverhangGL = 0.19f; // scaled to 20 GL units wall
        float glToModelX = originalWidth / 20.0f;

        float overhangModel = roofOverhangGL * glToModelX;

        float left = -0.85f - overhangModel;
        float right = 0.85f + overhangModel;

        glBegin(GL_POLYGON);
        glVertex2f(left * sx + ox, 0.1f * sy + oy);
        glVertex2f(right * sx + ox, 0.1f * sy + oy);
        glVertex2f(0.5f * sx + ox, 0.5f * sy + oy);
        glVertex2f(-0.5f * sx + ox, 0.5f * sy + oy);
        glEnd();
    }

    void drawDoor(float sx, float sy, float ox, float oy, float wallWidthGL) {
        glColor3f(0.75f, 0.45f, 0.25f);

        float leftWallModel = -0.85f;
        float glToModel = originalWidth / wallWidthGL;

        float doorLeftModel = leftWallModel + 3.16f * glToModel;
        float doorRightModel = doorLeftModel + 1.0f * glToModel;

        float doorBottom = -0.5f * sy + oy;
        float doorTop = -0.1f * sy + oy;

        glBegin(GL_POLYGON);
        glVertex2f(doorLeftModel * sx + ox, doorBottom);
        glVertex2f(doorRightModel * sx + ox, doorBottom);
        glVertex2f(doorRightModel * sx + ox, doorTop);
        glVertex2f(doorLeftModel * sx + ox, doorTop);
        glEnd();
    }

    void drawWindows(float sx, float sy, float ox, float oy, float wallWidthGL) {
        float glToModel = originalWidth / wallWidthGL;

        float winWidthModel = 0.9f * glToModel;
        float gapModel = 0.52f * glToModel;

        float winHeight = 0.25f * sy;
        float startX = -0.4f * sx;
        float startY = -0.35f * sy;
        float frameThick = 0.01f * sx;

        for (int i = 0; i < 3; i++) {
            float x = startX + i * ((winWidthModel + gapModel) * sx);

            glColor3f(0.55f, 0.27f, 0.07f);
            // left frame
            glBegin(GL_POLYGON);
            glVertex2f(x + ox, startY + oy);
            glVertex2f(x + frameThick + ox, startY + oy);
            glVertex2f(x + frameThick + ox, startY + winHeight + oy);
            glVertex2f(x + ox, startY + winHeight + oy);
            glEnd();

            // right frame
            glBegin(GL_POLYGON);
            glVertex2f(x + winWidthModel * sx - frameThick + ox, startY + oy);
            glVertex2f(x + winWidthModel * sx + ox, startY + oy);
            glVertex2f(x + winWidthModel * sx + ox, startY + winHeight + oy);
            glVertex2f(x + winWidthModel * sx - frameThick + ox, startY + winHeight + oy);
            glEnd();

            // top frame
            glBegin(GL_POLYGON);
            glVertex2f(x + ox, startY + winHeight - frameThick + oy);
            glVertex2f(x + winWidthModel * sx + ox, startY + winHeight - frameThick + oy);
            glVertex2f(x + winWidthModel * sx + ox, startY + winHeight + oy);
            glVertex2f(x + ox, startY + winHeight + oy);
            glEnd();

            // bottom frame
            glBegin(GL_POLYGON);
            glVertex2f(x + ox, startY + oy);
            glVertex2f(x + winWidthModel * sx + ox, startY + oy);
            glVertex2f(x + winWidthModel * sx + ox, startY + frameThick + oy);
            glVertex2f(x + ox, startY + frameThick + oy);
            glEnd();

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
            glBegin(GL_POLYGON);
            glVertex2f(x + frameThick + ox, startY + frameThick + oy);
            glVertex2f(x + winWidthModel * sx - frameThick + ox, startY + frameThick + oy);
            glVertex2f(x + winWidthModel * sx - frameThick + ox, startY + winHeight - frameThick + oy);
            glVertex2f(x + frameThick + ox, startY + winHeight - frameThick + oy);
            glEnd();
            glDisable(GL_BLEND);
        }
    }

    void drawGlassPanels(float sx, float sy, float ox, float oy, float wallWidthGL) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        int numPanels = 8;
        float paneGLWidth = 1.185f;
        float gapGL = 0.1f;
        float glToModel = originalWidth / wallWidthGL;

        float wallRight = 0.85f;
        float totalWidth = numPanels * paneGLWidth + (numPanels - 1) * gapGL;
        float startGLX = wallWidthGL - totalWidth;
        float startModelX = -0.85f + startGLX * glToModel;
        float paneModelWidth = paneGLWidth * glToModel;
        float gapModel = gapGL * glToModel;

        float bottom = -0.45f * sy + oy;
        float top = 0.1f * sy + oy;
        float frameThickness = 0.01f * sx;

        for (int i = 0; i < numPanels; i++) {
            float x0 = startModelX + i * (paneModelWidth + gapModel);
            float x1 = x0 + paneModelWidth;

            float sx0 = x0 * sx + ox;
            float sx1 = x1 * sx + ox;
            float sb = bottom;
            float st = top;
            float sf = frameThickness;

            glColor3f(0.55f, 0.27f, 0.07f);
            glBegin(GL_POLYGON); glVertex2f(sx0, sb); glVertex2f(sx0 + sf, sb); glVertex2f(sx0 + sf, st); glVertex2f(sx0, st); glEnd();
            glBegin(GL_POLYGON); glVertex2f(sx1 - sf, sb); glVertex2f(sx1, sb); glVertex2f(sx1, st); glVertex2f(sx1 - sf, st); glEnd();
            glBegin(GL_POLYGON); glVertex2f(sx0, st - sf); glVertex2f(sx1, st - sf); glVertex2f(sx1, st); glVertex2f(sx0, st); glEnd();
            glBegin(GL_POLYGON); glVertex2f(sx0, sb); glVertex2f(sx1, sb); glVertex2f(sx1, sb + sf); glVertex2f(sx0, sb + sf); glEnd();

            glColor4f(0.6f, 0.75f, 0.9f, 0.6f);
            glBegin(GL_POLYGON);
            glVertex2f(sx0 + sf, sb + sf);
            glVertex2f(sx1 - sf, sb + sf);
            glVertex2f(sx1 - sf, st - sf);
            glVertex2f(sx0 + sf, st - sf);
            glEnd();
        }

        glDisable(GL_BLEND);
    }

    const float originalWidth = 1.7f;
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
    
    // Enable smooth shading and alpha blending for better visuals
    glShadeModel(GL_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // Create objects
    FloorPlan floor;
    FrontElevation front;
    RearElevation rear;
    LeftElevation left;
    RightElevation right;

    while (!glfwWindowShouldClose(window))
    {
        // --- Controls ---
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  scrollX -= 0.001f;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) scrollX += 0.001f;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)    scrollY += 0.001f;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  scrollY -= 0.001f;

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
