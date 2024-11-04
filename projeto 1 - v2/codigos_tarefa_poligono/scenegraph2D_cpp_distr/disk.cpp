#include "disk.h"

// Factory method to create a DiskPtr
DiskPtr Disk::Make (int nslice, float radius) {
    return DiskPtr(new Disk(nslice, radius));
}

// Constructor for Disk
Disk::Disk(int nslice, float radius) {
    m_nslice = nslice;
    m_radius = radius;

    // Generate vertices
    float* xy = generateDiskVertices();
    float* tex = generateDiskTexCoord();

    
    // Create VAO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // Create buffers
    GLuint id[2];
    glGenBuffers(2, id);

    // Position buffer
    glBindBuffer(GL_ARRAY_BUFFER, id[0]);
    glBufferData(GL_ARRAY_BUFFER, (m_nslice + 2) * 2 * sizeof(float), (void*)xy, GL_STATIC_DRAW); 
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(0);

    // Texture coordinate buffer
    glBindBuffer(GL_ARRAY_BUFFER, id[1]);
    glBufferData(GL_ARRAY_BUFFER, (m_nslice + 2) * 2 * sizeof(float), (void*)tex, GL_STATIC_DRAW); 
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(1);

    // Free memory
    delete[] xy;
    delete[] tex;


    std::cout << "Disk created with " << m_nslice << " slices."<< std::endl;

    // Unbind the VAO
    glBindVertexArray(0);
}

// Destructor for Disk
Disk::~Disk () {
    glDeleteVertexArrays(1, &m_vao);
}

// Draw method
void Disk::Draw(StatePtr st) {
    // Drawing function: bind the VAO and draw the disk
    glBindVertexArray(m_vao);

    glDrawArrays(GL_TRIANGLE_FAN, 0, m_nslice + 2);

    // Unbind the VAO
    glBindVertexArray(0);  
}

// Generate vertices for the disk
float* Disk::generateDiskVertices() {
    // Allocate memory for the vertices: 1 center vertex + nslice vertices + 1 repeat of the first perimeter vertex
    float* vertices = new float[(m_nslice + 2) * 2]; // 2 components (x, y) per vertex

    // The center of the disk (at the origin)
    vertices[0] = 0.0f; // x
    vertices[1] = 0.0f; // y

    // The angle increment between each slice
    float angleIncrement = 2.0f * glm::pi<float>() / m_nslice;

    // Loop to create vertices for the perimeter of the disk
    for (int i = 0; i < m_nslice; ++i) {
        // Calculate the angle for the current slice
        float angle = i * angleIncrement;

        // Use polar coordinates to calculate the position of each vertex
        float x = m_radius * cos(angle);
        float y = m_radius * sin(angle);

        vertices[(i + 1) * 2]     = x; // x position
        vertices[(i + 1) * 2 + 1] = y; // y position
    }

    // Add the first perimeter vertex again at the end to close the fan
    vertices[(m_nslice + 1) * 2]     = vertices[2];  // x of first perimeter vertex
    vertices[(m_nslice + 1) * 2 + 1] = vertices[3];  // y of first perimeter vertex

    return vertices; // Return the dynamically allocated array
}


// Generate texture coordinates for the disk
float* Disk::generateDiskTexCoord() {
    // Allocate memory for the texture coordinates
    float* texCoords = new float[(m_nslice + 2) * 2]; // u, v

    // The center of the disk
    texCoords[0] = 0.5f; // u
    texCoords[1] = 0.5f; // v

    // The angle increment between each slice
    float angleIncrement = 2.0f * glm::pi<float>() / m_nslice;

    // Loop to create texture coordinates for the perimeter of the disk
    for (int i = 0; i < m_nslice; ++i) {
        // Calculate the angle for the current slice
        float angle = i * angleIncrement;

        // Texture coordinates based on polar coordinates, mapping to [0, 1] range
        float u = 0.5f + 0.5f * cos(angle);
        float v = 0.5f + 0.5f * sin(angle);

        texCoords[(i + 1) * 2]     = u; // u
        texCoords[(i + 1) * 2 + 1] = v; // v
    }

    // Add the first perimeter vertex's texture coordinates again to close the fan
    texCoords[(m_nslice + 1) * 2]     = texCoords[2];  // u
    texCoords[(m_nslice + 1) * 2 + 1] = texCoords[3];  // v

    return texCoords;
}

