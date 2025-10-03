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
    drawCircleFilled(cx + size * 0.1f, cy - size * 0.1f, size * 1.1f, 8, 0.0f, 0.0f, 0.0f, 0.2f);

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
    drawCircleFilled(cx - size * 0.05f, cy + size * 0.05f, size * 0.08f, 4, 1.0f, 1.0f, 1.0f, 0.8f);
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
    glVertex2f(x1, y1 - height * 0.4f);
    glVertex2f(x2, y1 - height * 0.4f);
    glVertex2f(x2, y1 + height * 0.2f);
    glVertex2f(x1, y1 + height * 0.2f);
    glEnd();

    // Box rim
    glColor3f(0.5f, 0.3f, 0.15f);
    glBegin(GL_QUADS);
    glVertex2f(x1 - 0.01f, y1 + height * 0.15f);
    glVertex2f(x2 + 0.01f, y1 + height * 0.15f);
    glVertex2f(x2, y1 + height * 0.2f);
    glVertex2f(x1, y1 + height * 0.2f);
    glEnd();

    // Soil
    glColor3f(0.4f, 0.2f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(x1 + 0.01f, y1 + height * 0.1f);
    glVertex2f(x2 - 0.01f, y1 + height * 0.1f);
    glVertex2f(x2 - 0.01f, y1 + height * 0.2f);
    glVertex2f(x1 + 0.01f, y1 + height * 0.2f);
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
        glVertex2f(stemX, y1 + height * 0.2f);
        glVertex2f(stemX, y1 + height * 0.2f + stemHeight);
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
        glVertex2f(stemX, y1 + height * 0.2f);
        glVertex2f(stemX, y1 + height * 0.2f + stemHeight);
    }
    glEnd();
    glLineWidth(1.0f);

    // Extremely dense summer flowers (no empty spaces)
    for (int i = 0; i < numStems; i++) {
        float stemX = x1 + 0.005f + (boxWidth - 0.01f) * (float)i / (float)(numStems - 1);
        float stemHeight = height * (0.15f + 0.3f * sinf(stemX * 30.0f + (float)i * 0.3f)); // shorter stems for main flowers
        float flowerSize = height * (0.05f + 0.04f * sinf(stemX * 20.0f + (float)i * 0.4f));

        // Expanded summer flower colors (15 different colors)
        switch (i % 15) {
        case 0: drawRealisticFlower(stemX, y1 + height * 0.2f + stemHeight, flowerSize, 1.0f, 0.2f, 0.2f); break; // red
        case 1: drawRealisticFlower(stemX, y1 + height * 0.2f + stemHeight, flowerSize, 1.0f, 0.8f, 0.0f); break; // yellow
        case 2: drawRealisticFlower(stemX, y1 + height * 0.2f + stemHeight, flowerSize, 0.8f, 0.2f, 0.8f); break; // purple
        case 3: drawRealisticFlower(stemX, y1 + height * 0.2f + stemHeight, flowerSize, 1.0f, 0.4f, 0.7f); break; // pink
        case 4: drawRealisticFlower(stemX, y1 + height * 0.2f + stemHeight, flowerSize, 0.2f, 0.6f, 1.0f); break; // blue
        case 5: drawRealisticFlower(stemX, y1 + height * 0.2f + stemHeight, flowerSize, 1.0f, 1.0f, 0.2f); break; // bright yellow
        case 6: drawRealisticFlower(stemX, y1 + height * 0.2f + stemHeight, flowerSize, 0.9f, 0.5f, 0.1f); break; // orange
        case 7: drawRealisticFlower(stemX, y1 + height * 0.2f + stemHeight, flowerSize, 0.8f, 0.8f, 1.0f); break; // light blue
        case 8: drawRealisticFlower(stemX, y1 + height * 0.2f + stemHeight, flowerSize, 1.0f, 0.6f, 0.8f); break; // light pink
        case 9: drawRealisticFlower(stemX, y1 + height * 0.2f + stemHeight, flowerSize, 0.7f, 1.0f, 0.7f); break; // light green
        case 10: drawRealisticFlower(stemX, y1 + height * 0.2f + stemHeight, flowerSize, 1.0f, 0.9f, 0.6f); break; // cream
        case 11: drawRealisticFlower(stemX, y1 + height * 0.2f + stemHeight, flowerSize, 0.9f, 0.7f, 1.0f); break; // lavender
        case 12: drawRealisticFlower(stemX, y1 + height * 0.2f + stemHeight, flowerSize, 1.0f, 0.5f, 0.5f); break; // coral
        case 13: drawRealisticFlower(stemX, y1 + height * 0.2f + stemHeight, flowerSize, 0.5f, 1.0f, 0.8f); break; // mint green
        case 14: drawRealisticFlower(stemX, y1 + height * 0.2f + stemHeight, flowerSize, 1.0f, 0.8f, 0.9f); break; // pale rose
        }
    }

    // Multiple layers of smaller flowers to fill all gaps
    int extraFlowers = (int)(boxWidth * 90.0f); // more bloom flowers
    for (int i = 0; i < extraFlowers; i++) {
        float flowerX = x1 + 0.01f + (boxWidth - 0.02f) * (float)i / (float)(extraFlowers - 1);
        float flowerY = y1 + height * 0.22f + height * (0.15f + 0.35f * sinf(flowerX * 32.0f + (float)i * 0.6f));
        float smallFlowerSize = height * (0.025f + 0.025f * sinf(flowerX * 25.0f));

        // Small filler flowers in bright summer colors
        switch (i % 8) {
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
        float flowerY = y1 + height * 0.18f + height * (0.1f + 0.25f * sinf(flowerX * 40.0f + (float)i * 0.9f));
        float tinyFlowerSize = height * (0.015f + 0.015f * sinf(flowerX * 28.0f));

        // Tiny flowers in pastel colors
        switch (i % 10) {
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
        float leafY = y1 + height * 0.15f + height * (0.05f + 0.2f * sinf(leafX * 45.0f + (float)i * 1.2f));
        float leafSize = height * (0.01f + 0.02f * sinf(leafX * 35.0f));

        // Green leaves in various shades
        switch (i % 5) {
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
    glVertex2f(cx - width * 0.5f, cy - height * 0.5f);
    glVertex2f(cx + width * 0.5f, cy - height * 0.5f);
    glVertex2f(cx + width * 0.5f, cy + height * 0.5f);
    glVertex2f(cx - width * 0.5f, cy + height * 0.5f);
    glEnd();

    // Sign border
    glColor3f(0.0f, 0.5f, 0.1f); // Darker green
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(cx - width * 0.5f, cy - height * 0.5f);
    glVertex2f(cx + width * 0.5f, cy - height * 0.5f);
    glVertex2f(cx + width * 0.5f, cy + height * 0.5f);
    glVertex2f(cx - width * 0.5f, cy + height * 0.5f);
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
    glVertex2f(cx - spacing * 1.5f - letterWidth * 0.5f, cy - letterHeight * 0.5f);
    glVertex2f(cx - spacing * 1.5f + letterWidth * 0.5f, cy - letterHeight * 0.5f);
    glVertex2f(cx - spacing * 1.5f + letterWidth * 0.5f, cy + letterHeight * 0.5f);
    glVertex2f(cx - spacing * 1.5f - letterWidth * 0.5f, cy + letterHeight * 0.5f);
    glEnd();

    // P
    glBegin(GL_LINES);
    glVertex2f(cx - spacing * 0.5f - letterWidth * 0.5f, cy - letterHeight * 0.5f);
    glVertex2f(cx - spacing * 0.5f - letterWidth * 0.5f, cy + letterHeight * 0.5f);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(cx - spacing * 0.5f - letterWidth * 0.5f, cy);
    glVertex2f(cx - spacing * 0.5f + letterWidth * 0.5f, cy);
    glVertex2f(cx - spacing * 0.5f + letterWidth * 0.5f, cy + letterHeight * 0.5f);
    glVertex2f(cx - spacing * 0.5f - letterWidth * 0.5f, cy + letterHeight * 0.5f);
    glEnd();

    // E
    glBegin(GL_LINES);
    glVertex2f(cx + spacing * 0.5f - letterWidth * 0.5f, cy - letterHeight * 0.5f);
    glVertex2f(cx + spacing * 0.5f - letterWidth * 0.5f, cy + letterHeight * 0.5f);
    glVertex2f(cx + spacing * 0.5f - letterWidth * 0.5f, cy + letterHeight * 0.5f);
    glVertex2f(cx + spacing * 0.5f + letterWidth * 0.5f, cy + letterHeight * 0.5f);
    glVertex2f(cx + spacing * 0.5f - letterWidth * 0.5f, cy);
    glVertex2f(cx + spacing * 0.5f + letterWidth * 0.3f, cy);
    glVertex2f(cx + spacing * 0.5f - letterWidth * 0.5f, cy - letterHeight * 0.5f);
    glVertex2f(cx + spacing * 0.5f + letterWidth * 0.5f, cy - letterHeight * 0.5f);
    glEnd();

    // N
    glBegin(GL_LINES);
    glVertex2f(cx + spacing * 1.5f - letterWidth * 0.5f, cy - letterHeight * 0.5f);
    glVertex2f(cx + spacing * 1.5f - letterWidth * 0.5f, cy + letterHeight * 0.5f);
    glVertex2f(cx + spacing * 1.5f - letterWidth * 0.5f, cy + letterHeight * 0.5f);
    glVertex2f(cx + spacing * 1.5f + letterWidth * 0.5f, cy - letterHeight * 0.5f);
    glVertex2f(cx + spacing * 1.5f + letterWidth * 0.5f, cy - letterHeight * 0.5f);
    glVertex2f(cx + spacing * 1.5f + letterWidth * 0.5f, cy + letterHeight * 0.5f);
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
        glVertex2f(0.07f, 0.955f);
        glEnd();

        // Main ridge cap
        glColor3f(0.4f, 0.15f, 0.15f);
        glBegin(GL_LINES);
        glVertex2f(-0.06f, 0.96f);
        glVertex2f(0.06f, 0.96f);
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
        // Position the building relative to our drawing area
        float offsetX = 35.0f;
        float offsetY = 0.0f;

        // Main building dimensions
        float width = 27.5f;    // overall width
        float height = 7.0f;    // wall height
        float roofH = 5.0f;     // roof height

        // Calculate the four corners of our building
        float leftX = offsetX - width / 2.0f;
        float rightX = offsetX + width / 2.0f;
        float baseY = offsetY;
        float topY = offsetY + height;

        // Define our color palette
        const float wallR = 0.82f, wallG = 0.78f, wallB = 0.72f;     // warm beige for walls
        const float roofR = 0.95f, roofG = 0.60f, roofB = 0.60f;     // terracotta roof tiles
        const float glassR = 0.45f, glassG = 0.65f, glassB = 0.90f;  // sky blue glass
        const float outlineR = 1.00f, outlineG = 0.36f, outlineB = 0.05f;  // orange accent lines
        const float boxWallR = 0.75f, boxWallG = 0.72f, boxWallB = 0.70f;  // slightly darker beige
        const float ventR = 0.65f, ventG = 0.65f, ventB = 0.66f;  // metallic grey for vents

        // Draw the main wall rectangle
        glColor3f(wallR, wallG, wallB);
        glBegin(GL_QUADS);
        glVertex2f(leftX, baseY);
        glVertex2f(rightX, baseY);
        glVertex2f(rightX, topY);
        glVertex2f(leftX, topY);
        glEnd();

        // Add a thin decorative trim under the eaves
        float trimH = 0.5f;
        glColor3f(0.78f, 0.60f, 0.58f);  // subtle contrast color
        glBegin(GL_QUADS);
        glVertex2f(leftX, topY - trimH * 0.5f);
        glVertex2f(rightX, topY - trimH * 0.5f);
        glVertex2f(rightX, topY + trimH * 0.5f);
        glVertex2f(leftX, topY + trimH * 0.5f);
        glEnd();

        // Draw the main roof shape as a trapezoid
        glColor3f(0.95f, 0.55f, 0.55f);  // roof color
        float ridgeHalf = width * 0.20f;  // flat section at roof peak
        float ridgeY = topY + roofH;      // height of roof ridge

        glBegin(GL_QUADS);
        glVertex2f(leftX - 0.3f, topY);        // bottom left eave
        glVertex2f(leftX + ridgeHalf, ridgeY); // ridge start
        glVertex2f(rightX - ridgeHalf, ridgeY); // ridge end
        glVertex2f(rightX + 0.3f, topY);       // bottom right eave
        glEnd();

        // Add texture to make the roof look tiled
        glColor3f(0.6f, 0.25f, 0.25f); // darker color for tile lines
        glLineWidth(1.0f);

        // Tile spacing and roof boundaries
        float tileRowSpacing = 0.3f;
        float roofBaseY = topY;
        float roofPeakY = ridgeY;

        // Roof edge coordinates
        float leftEaveX = leftX - 0.3f;
        float rightEaveX = rightX + 0.3f;
        float leftRidgeX = leftX + ridgeHalf;
        float rightRidgeX = rightX - ridgeHalf;

        // Draw horizontal tile lines across the roof
        int row = 0;
        for (float y = roofBaseY + tileRowSpacing; y < roofPeakY; y += tileRowSpacing, row++) {
            float yProgress = (y - roofBaseY) / (roofPeakY - roofBaseY);

            // Calculate where this tile row sits on the sloping roof
            float leftXAtY = leftEaveX + (leftRidgeX - leftEaveX) * yProgress;
            float rightXAtY = rightEaveX + (rightRidgeX - rightEaveX) * yProgress;

            // Shadow under each tile row
            glColor3f(0.65f, 0.35f, 0.35f);
            glBegin(GL_LINES);
            glVertex2f(leftXAtY, y - 0.03f);
            glVertex2f(rightXAtY, y - 0.03f);
            glEnd();

            // Main tile separation line
            glColor3f(0.75f, 0.45f, 0.45f);
            glBegin(GL_LINES);
            glVertex2f(leftXAtY, y);
            glVertex2f(rightXAtY, y);
            glEnd();
        }

        // Add vertical separators between tiles with staggered pattern
        float tileWidth = 0.35f;
        row = 0;
        for (float y = roofBaseY + tileRowSpacing * 0.5f; y < roofPeakY - 0.1f; y += tileRowSpacing, row++) {
            float yProgress = (y - roofBaseY) / (roofPeakY - roofBaseY);

            // Current roof width at this height
            float leftXAtY = leftEaveX + (leftRidgeX - leftEaveX) * yProgress;
            float rightXAtY = rightEaveX + (rightRidgeX - rightEaveX) * yProgress;
            float widthAtY = rightXAtY - leftXAtY;

            // Offset every other row for realistic brick pattern
            float offset = (row % 2 == 0) ? 0.0f : tileWidth * 0.5f;

            // Draw the vertical tile separators
            for (float x = leftXAtY + offset; x < rightXAtY; x += tileWidth) {
                if (x > leftXAtY && x < rightXAtY) {
                    float separatorHeight = tileRowSpacing * 0.6f;

                    // Shadow line
                    glColor3f(0.65f, 0.35f, 0.35f);
                    glBegin(GL_LINES);
                    glVertex2f(x + 0.015f, y - separatorHeight * 0.5f);
                    glVertex2f(x + 0.015f, y + separatorHeight * 0.5f);
                    glEnd();

                    // Main separator
                    glColor3f(0.75f, 0.45f, 0.45f);
                    glBegin(GL_LINES);
                    glVertex2f(x, y - separatorHeight * 0.5f);
                    glVertex2f(x, y + separatorHeight * 0.5f);
                    glEnd();
                }
            }
        }

        // Draw two tall glass doors on the left side
        glColor3f(glassR, glassG, glassB);
        float doorW = width * 0.095f / 2;     // door panel width
        float doorH = height * 0.88f;         // door height
        float doorBase = baseY;
        float startX = leftX + width * 0.07f; // starting position

        // Draw the two door panels
        for (int i = 0; i < 2; i++) {
            float x1 = startX + i * doorW;
            float x2 = x1 + doorW;

            glBegin(GL_QUADS);
            glVertex2f(x1, doorBase);
            glVertex2f(x2, doorBase);
            glVertex2f(x2, doorBase + doorH);
            glVertex2f(x1, doorBase + doorH);
            glEnd();
        }

        // Add five small square windows above the doors
        glColor3f(glassR, glassG, glassB);
        float winSize = height * 0.18f;      // window size
        float winGap = width * 0.017f;       // spacing between windows
        float winStart = startX + 2.0f * doorW + width * 0.04f; // position
        float winY = baseY + height * 0.53f; // height from ground

        for (int i = 0; i < 5; ++i) {
            float x1 = winStart + i * (winSize + winGap);
            float x2 = x1 + winSize;
            float y1 = winY;
            float y2 = y1 + winSize;

            glBegin(GL_QUADS);
            glVertex2f(x1, y1);
            glVertex2f(x2, y1);
            glVertex2f(x2, y2);
            glVertex2f(x1, y2);
            glEnd();
        }

        // Draw the rainwater collection system
        glColor3f(0.25f, 0.25f, 0.28f); // dark grey for pipes

        float pipeW = 0.35f;     // pipe width
        float pipeH = 6.0f;     // pipe height
        float gutterH = 0.8f;   // gutter height
        float gutterBoxW = 4.0f; // collection box width
        float gutterBoxH = 1.5f; // collection box height

        // Position the downpipe on the right side
        float px = rightX - pipeW;   // pipe x position
        float py1 = roofH * 1.4f;    // pipe top (below gutter)
        float py2 = baseY + gutterH + 0.2f; // pipe bottom

        // Draw the collection box at ground level
        float boxW = 2.0f;
        float boxH = 1.0f;
        float boxY1 = baseY;
        float boxY2 = boxY1 + boxH;
        float boxX1 = width * 1.7f;
        float boxX2 = boxX1 + boxW;

        // Box fill
        glColor3f(0.45f, 0.45f, 0.48f);
        glBegin(GL_QUADS);
        glVertex2f(boxX1, boxY1);
        glVertex2f(boxX2, boxY1);
        glVertex2f(boxX2, boxY2);
        glVertex2f(boxX1, boxY2);
        glEnd();

        // Box outline
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(boxX1, boxY1);
        glVertex2f(boxX2, boxY1);
        glVertex2f(boxX2, boxY2);
        glVertex2f(boxX1, boxY2);
        glEnd();

        // Draw the downpipe
        glColor3f(0.6f, 0.6f, 0.65f);
        glBegin(GL_QUADS);
        glVertex2f(px, py1);
        glVertex2f(px + pipeW, py1);
        glVertex2f(px + pipeW, py2);
        glVertex2f(px, py2);
        glEnd();

        // Pipe outline
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(px, py1);
        glVertex2f(px + pipeW, py1);
        glVertex2f(px + pipeW, py2);
        glVertex2f(px, py2);
        glEnd();

        // Add three extractor vents along the roof ridge
        float ventW = 0.55f;       // vent width
        float ventH = 1.0f;       // vent height
        float capOverhang = 0.4f; // cap overhang
        float spacing = 0.9f;     // spacing between vents

        for (int i = 0; i < 3; i++) {
            // Position each vent along the ridge
            float baseX1 = (rightX - ridgeHalf - ventW * 1.5f) - i * (ventW + spacing);
            float baseX2 = baseX1 + ventW;
            float baseY1 = ridgeY;          // sits on roof ridge
            float baseY2 = baseY1 + ventH;

            // Vent body
            glColor3f(0.6f, 0.6f, 0.65f);
            glBegin(GL_QUADS);
            glVertex2f(baseX1, baseY1);
            glVertex2f(baseX2, baseY1);
            glVertex2f(baseX2, baseY2);
            glVertex2f(baseX1, baseY2);
            glEnd();

            // Vent cap with overhang
            glColor3f(0.55f, 0.55f, 0.60f);
            glBegin(GL_QUADS);
            glVertex2f(baseX1 - capOverhang, baseY2);
            glVertex2f(baseX2 + capOverhang, baseY2);
            glVertex2f(baseX2 + capOverhang, baseY2 + 0.35f);
            glVertex2f(baseX1 - capOverhang, baseY2 + 0.35f);
            glEnd();

            // Outline the vent and cap
            glColor3f(0.0f, 0.0f, 0.0f);
            glLineWidth(1.0f);
            glBegin(GL_LINES);

            // Vent body outline
            glVertex2f(baseX1, baseY1); glVertex2f(baseX2, baseY1);
            glVertex2f(baseX2, baseY1); glVertex2f(baseX2, baseY2);
            glVertex2f(baseX2, baseY2); glVertex2f(baseX1, baseY2);
            glVertex2f(baseX1, baseY2); glVertex2f(baseX1, baseY1);

            // Cap outline
            glVertex2f(baseX1 - capOverhang, baseY2);          glVertex2f(baseX2 + capOverhang, baseY2);
            glVertex2f(baseX2 + capOverhang, baseY2);          glVertex2f(baseX2 + capOverhang, baseY2 + 0.35f);
            glVertex2f(baseX2 + capOverhang, baseY2 + 0.35f);  glVertex2f(baseX1 - capOverhang, baseY2 + 0.35f);
            glVertex2f(baseX1 - capOverhang, baseY2 + 0.35f);  glVertex2f(baseX1 - capOverhang, baseY2);
            glEnd();
        }

        // Draw all the outline details
        glColor3f(outlineR, outlineG, outlineB);
        glLineWidth(1.0f);
        glBegin(GL_LINES);

        // Outline the two doors
        float x1 = startX;
        float x2 = x1 + doorW;

        // Left door outline
        glVertex2f(x1, doorBase);
        glVertex2f(x2, doorBase);
        glVertex2f(x2, doorBase);
        glVertex2f(x2, doorBase + doorH);
        glVertex2f(x2, doorBase + doorH);
        glVertex2f(x1, doorBase + doorH);
        glVertex2f(x1, doorBase + doorH);
        glVertex2f(x1, doorBase);

        // Right door outline
        x1 = startX + doorW;
        x2 = x1 + doorW;

        glVertex2f(x1, doorBase);
        glVertex2f(x2, doorBase);
        glVertex2f(x2, doorBase);
        glVertex2f(x2, doorBase + doorH);
        glVertex2f(x2, doorBase + doorH);
        glVertex2f(x1, doorBase + doorH);
        glVertex2f(x1, doorBase + doorH);
        glVertex2f(x1, doorBase);

        // Add door handles
        float handleY1 = baseY + height * 0.45f;
        float handleY2 = handleY1 + 0.8f;
        float handleX1_left = startX + doorW * 0.7f;
        float handleX2_left = handleX1_left + 0.1f;
        float handleX1_right = startX + doorW + doorW * 0.2f;
        float handleX2_right = handleX1_right + 0.1f;

        drawRectangle(handleX1_left, handleY1, handleX2_left, handleY2, 0.3f, 0.3f, 0.3f);
        drawRectangle(handleX1_right, handleY1, handleX2_right, handleY2, 0.3f, 0.3f, 0.3f);

        // Left handle fill
        glColor3f(0.3f, 0.3f, 0.3f);
        glBegin(GL_QUADS);
        glVertex2f(handleX1_left, handleY1);
        glVertex2f(handleX2_left, handleY1);
        glVertex2f(handleX2_left, handleY2);
        glVertex2f(handleX1_left, handleY2);
        glEnd();

        // Switch to thinner lines for fine details
        glLineWidth(0.25f);
        glColor3f(outlineR, outlineG, outlineB);
        glBegin(GL_LINES);

        // Building foundation and walls
        glVertex2f(leftX, baseY);
        glVertex2f(rightX, baseY);
        glVertex2f(rightX, baseY);
        glVertex2f(rightX, topY);
        glVertex2f(leftX, baseY);
        glVertex2f(leftX, topY);

        // Roof edges
        glVertex2f(leftX - 0.3f, topY);
        glVertex2f(leftX + ridgeHalf, ridgeY);
        glVertex2f(rightX + 0.3f, topY);
        glVertex2f(rightX - ridgeHalf, ridgeY);
        glVertex2f(leftX + ridgeHalf, ridgeY);
        glVertex2f(rightX - ridgeHalf, ridgeY);

        // Eaves trim
        glVertex2f(leftX - 0.3f, topY);
        glVertex2f(rightX + 0.3f, topY);

        // Window frames and muntins (crossbars)
        for (int i = 0; i < 5; ++i) {
            float x1 = winStart + i * (winSize + winGap);
            float x2 = x1 + winSize;
            float y1 = winY;
            float y2 = y1 + winSize;

            // Window frame
            glVertex2f(x1, y1);
            glVertex2f(x2, y1);
            glVertex2f(x2, y1);
            glVertex2f(x2, y2);
            glVertex2f(x2, y2);
            glVertex2f(x1, y2);
            glVertex2f(x1, y2);
            glVertex2f(x1, y1);


            // Diagonal cross (X pattern)
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
            glVertex2f(x2, y1);
            glVertex2f(x1, y2);

            // Center V pattern
            float cx = (x1 + x2) * 0.5f;
            glVertex2f(x1 + 0.12f, y2 - 0.12f);
            glVertex2f(cx, y1 + 0.04f);
            glVertex2f(x2 - 0.12f, y2 - 0.12f);
            glVertex2f(cx, y1 + 0.04f);
        }
        glEnd();

        glLineWidth(1.0f);
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
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  scrollX -= 0.1f;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) scrollX += 0.1f;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)    scrollY += 0.1f;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  scrollY -= 0.1f;

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) zoomLevel -= 0.1f;
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) zoomLevel += 0.1f;

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