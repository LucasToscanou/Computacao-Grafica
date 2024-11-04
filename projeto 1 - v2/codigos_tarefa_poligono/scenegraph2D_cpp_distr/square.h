#ifndef SQUARE_H
#define SQUARE_H

#include "shape.h"
#include <memory>

class Square;
using SquarePtr = std::shared_ptr<Square>; 

class Square : public Shape {
private:
    GLuint m_vao;         // Vertex Array Object
    float m_x, m_y;       // Position of the square

public:
    // Factory method to create a SquarePtr
    static SquarePtr Make(float x, float y);

    // Constructor
    Square(float x, float y);

    // Destructor
    ~Square();

    // Override the Draw method from the Shape interface
    void Draw(StatePtr st) override;
};

using SquarePtr = std::shared_ptr<Square>;

#endif // SQUARE_H
