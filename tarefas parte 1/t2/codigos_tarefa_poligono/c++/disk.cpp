#include "disk.h"

// Factory method to create a DiskPtr
DiskPtr Disk::Make (int nslice) {
    return DiskPtr(new Disk(nslice));
}

// Constructor for Disk
Disk::Disk(int nslice) {
    m_nslice = nslice;

    // Generate vertices
    float* xy = generateDiskVertices(m_nslice);

    // Generate colors vertices
    unsigned char rgb[(m_nslice + 1) * 3];
    for (int i = 0; i <= m_nslice; ++i) {
        rgb[i * 3] = 255;   // Red
        rgb[i * 3 + 1] = 255; // Green
        rgb[i * 3 + 2] = 255; // Blue
    }

    // Generate indices
    unsigned int inc[m_nslice + 2];
    inc[0] = 0; // The center vertex
    for (int i = 1; i <= m_nslice; ++i) {
        inc[i] = i;
    }
    inc[m_nslice + 1] = 1; // Repeat first perimeter vertex to close the triangle fan

    // Create VAO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // Create buffers
    GLuint id[3];
    glGenBuffers(3, id);

    // Coordinate buffer
    glBindBuffer(GL_ARRAY_BUFFER, id[0]);
    glBufferData(GL_ARRAY_BUFFER, (m_nslice + 2) * 2 * sizeof(float), (void*)xy, GL_STATIC_DRAW); 
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(0);

    // Free memory
    delete[] xy;

    // Color buffer
    glBindBuffer(GL_ARRAY_BUFFER, id[1]);
    glBufferData(GL_ARRAY_BUFFER, (m_nslice + 1) * sizeof(unsigned char) * 3, (void*)rgb, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);  // Colors
    glEnableVertexAttribArray(1);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (m_nslice + 2) * sizeof(unsigned int), (void*)inc, GL_STATIC_DRAW);

    std::cout << "Disk created with " << m_nslice << " slices."<< std::endl;

    // Unbind the VAO
    glBindVertexArray(0);
}

// Destructor for Disk
Disk::~Disk () {
    glDeleteVertexArrays(1, &m_vao);
}

// Draw method
void Disk::Draw() {
    // Drawing function: bind the VAO and draw the disk
    glBindVertexArray(m_vao);

    glDrawArrays(GL_TRIANGLE_FAN, 0, m_nslice + 2);

    // Unbind the VAO
    glBindVertexArray(0);  
}

// Generate vertices for the disk
float* Disk::generateDiskVertices(int nslice) {
    // Allocate memory for the vertices: 1 center vertex + nslice vertices + 1 repeat of the first perimeter vertex
    float* vertices = new float[(nslice + 2) * 2]; // Each vertex has 2 components (x, y)

    // The center of the disk
    vertices[0] = 0.0f; // x
    vertices[1] = 0.0f; // y

    // The angle increment between each slice
    float angleIncrement = 2.0f * glm::pi<float>() / nslice;

    // Loop to create vertices for the perimeter of the disk
    for (int i = 0; i < nslice; ++i) {
        // Calculate the angle for the current slice
        float angle = i * angleIncrement;

        // Use polar coordinates to calculate the position of each vertex
        float x = 1 * cos(angle);
        float y = 1 * sin(angle);

        // Add the vertex to the array
        vertices[(i + 1) * 2] = x;     // x component
        vertices[(i + 1) * 2 + 1] = y; // y component
    }

    // Add the first perimeter vertex again at the end to close the fan
    vertices[(nslice + 1) * 2] = vertices[2];       // x of first perimeter vertex
    vertices[(nslice + 1) * 2 + 1] = vertices[3];   // y of first perimeter vertex

    // // Print the vertices
    // std::cout << "Disk Vertices (x, y):" << std::endl;
    // for (int i = 0; i <= nslice + 1; ++i) {
    //     std::cout << "Vertex " << i << ": (" 
    //               << vertices[i * 2] << ", " 
    //               << vertices[i * 2 + 1] << ")" 
    //               << std::endl;
    // }

    return vertices; // Return the dynamically allocated array
}


