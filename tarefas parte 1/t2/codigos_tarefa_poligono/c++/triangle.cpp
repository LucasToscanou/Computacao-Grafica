#include "triangle.h"

#include "os_specific_libs.h"

TrianglePtr Triangle::Make ()
{
  return TrianglePtr(new Triangle());
}

Triangle::Triangle ()
{
  float coord[] = {
   -0.5f,-0.5f, 
    0.5f,-0.5f,
    0.0f, 0.5f,
  };
  unsigned char color[] = {
    255,0,0,
    0,255,0,
    0,0,255
  };
  unsigned int index[] = {0,1,2};
  // create VAO
  glGenVertexArrays(1,&m_vao);
  glBindVertexArray(m_vao);
  // create coord buffer
  GLuint id[3];
  glGenBuffers(3,id);
  glBindBuffer(GL_ARRAY_BUFFER,id[0]);
  glBufferData(GL_ARRAY_BUFFER,2*3*sizeof(float),(void*)coord,GL_STATIC_DRAW);
  glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,0);  // coord
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER,id[1]);
  glBufferData(GL_ARRAY_BUFFER,3*3*sizeof(unsigned char),(void*)color,GL_STATIC_DRAW);
  glVertexAttribPointer(1,3,GL_UNSIGNED_BYTE,GL_TRUE,0,0);  // color
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,3*sizeof(GLuint),(void*)index ,GL_STATIC_DRAW);
}

Triangle::~Triangle () 
{
  glDeleteVertexArrays(1,&m_vao);
}

void Triangle::Draw ()
{
  glBindVertexArray(m_vao);
  glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
  //glDrawArrays(GL_TRIANGLES,0,3);
}