#include "polygon.h"
#include "os_specific_libs.h"

PolygonPtr Polygon::Make (float* xy, unsigned char* rgb, unsigned int* inc, unsigned int numIndices)
{
  return PolygonPtr(new Polygon(xy, rgb, inc, numIndices));
}

Polygon::Polygon (float* xy, unsigned char* rgb, unsigned int* inc, unsigned int numIndices)
{
  m_numIndices = numIndices;

  // Create VAO
  glGenVertexArrays(1,&m_vao);
  glBindVertexArray(m_vao);

  // Create buffers
  GLuint id[3];
  glGenBuffers(3,id);

  // Coordinate buffer
  glBindBuffer(GL_ARRAY_BUFFER, id[0]);
  glBufferData(GL_ARRAY_BUFFER, m_numIndices * sizeof(float) * 2, (void*)xy, GL_STATIC_DRAW); 
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); 
  glEnableVertexAttribArray(0);

  // Color buffer
  glBindBuffer(GL_ARRAY_BUFFER, id[1]);
  glBufferData(GL_ARRAY_BUFFER, m_numIndices * sizeof(unsigned char) * 3, (void*)rgb, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);  // Colors
  glEnableVertexAttribArray(1);

  // Index buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numIndices * sizeof(unsigned int), (void*)inc, GL_STATIC_DRAW);

  // Unbind the VAO
  glBindVertexArray(0);
}

Polygon::~Polygon () 
{
  glDeleteVertexArrays(1, &m_vao);
}

void Polygon::Draw ()
{
  glBindVertexArray(m_vao);
  glDrawElements(GL_LINE_LOOP, m_numIndices, GL_UNSIGNED_INT, 0);
}
