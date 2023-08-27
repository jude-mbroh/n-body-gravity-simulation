// Header guard
#ifndef BODY_H
#define BODY_H

// This class serves as a blueprint for an object representing a (celestial) body in this N-Body simulation

#include <vector>

class Body {

    // Class member variables have been made private to ensure data encapsulation
    private:
        double mass;    // Mass of a body
        double x, y;    // Position of a body
        double vx, vy;  // Velocity of a body
        double ax, ay;  // Acceleration of a body

    /*
     * Class member functions have been made public to provide a controlled and consistent way for other
     * parts of the code to interact with the class's data.
     */
    public:

    // Overloading the assignment operator to prevent shallow copies of the objects

        Body& operator=(const Body& other) {
            // Checking for self-assignment
            if (this == &other) {
                return *this;
            }
            // copy (or move) resources
            x = other.x;
            y = other.y;
            vx = other.vx;
            vy = other.vy;
            ax = other.ax;
            ay = other.ay;
            mass = other.mass;
            return *this;
        }

        // Default constructor - creates objects with default values when no values are passed to the constructor
        Body() : x(0), y(0), vx(0), vy(0), ax(0), ay(0), mass(0) {}

        // Initialiser list constructor to create objects with specific values
        Body(double x, double y, double vx, double vy, double mass) : x(x), y(y), vx(vx), vy(vy), mass(mass) {}

        // The member variables ax and ay don't need to be explicitly initialised, they can take default values

        // NOTE: This class doesn't allocate resources on the heap so a destructor was not created in this class

        // Initialising class member functions
        void updateAcceleration(const std::vector<Body>& bodies, const unsigned int N, const double G, double xStep, double yStep);
        void updatePosition(const std::vector<Body>& bodies, const unsigned int N, const double dt, const double G);

        // Defining getter functions to read (but not modify) body data
        double getMass() const { return mass; };
        double getX() const { return x; };
        double getY() const { return y; };
        double getVX() const { return vx; };
        double getVY() const { return vy; };

};
#endif // BODY_H
