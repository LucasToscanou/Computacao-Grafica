#include "square.h"
#include <iostream>
#include "os_specific_libs.h"

SquarePtr Square::Make(float x, float y) {
    return SquarePtr(new Square(x, y));
}

Square::Square(float x, float y) : m_x(x), m_y(y) {
    float coord[] = {
        -x / 2.0f, -y / 2.0f,  // Bottom left
         x / 2.0f, -y / 2.0f,  // Bottom right
        -x / 2.0f,  y / 2.0f,  // Top left
         x / 2.0f,  y / 2.0f   // Top right
    };
    float texcoord[] = {
        0.0f, 0.0f,  // Bottom left
        1.0f, 0.0f,  // Bottom right
        0.0f, 1.0f,  // Top left
        1.0f, 1.0f   // Top right
    };

    // Create VAO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // Create VBOs
    GLuint id[2];
    glGenBuffers(2, id);

    // Create coordinate buffer (for vertices)
    glBindBuffer(GL_ARRAY_BUFFER, id[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coord), coord, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);  // coord
    glEnableVertexAttribArray(0);

    // Create texture coordinate buffer
    glBindBuffer(GL_ARRAY_BUFFER, id[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoord), texcoord, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);  // texcoord
    glEnableVertexAttribArray(1);
}

// Destructor
Square::~Square() {
    glDeleteVertexArrays(1, &m_vao);
}

void Square::Draw(StatePtr st) {
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
