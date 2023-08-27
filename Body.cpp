// Source file containing implementation of class member functions declared in "Body.h"

#include "Body.h" // Ensuring that the class header file is included
#include <cmath>

// Defining function to update the acceleration of a body

void Body::updateAcceleration(const std::vector<Body>& bodies, const unsigned int N, const double G, double xStep, double yStep) {
    /* This function updates the acceleration of a body at each timestep given the values of the
     * member variables from the 'Body' class
     *
     * Inputs:
     * std::vector<Body>& bodies --- Reference to the vector of bodies to allow direct manipulation of the body objects
     *
     * int N ----------------------- Number of bodies in the orbit simulation
     *
     * double G -------------------- Gravitational constant
     *
     * xStep ----------------------- Temporary x-position variable as intermediate value at each timestep
     *
     * yStep ----------------------- Temporary y-position variable as intermediate value at each timestep
     *
     * Outputs: There is no required output from this function as it just modifies member variables
     * so 'void' is the return type
     * 
     * 
     * NOTE: Some variables are declared as const <DATATYPE> as they shouldn't be modified by this function
     */


    // Initialising acceleration values for each timestep
    ax = 0;
    ay = 0;

    double r = 0.0; // Initialising radial distance

    for (int i = 0; i < N; i++) {    // Looping through all bodies
        
        // The if statement ensures that relative displacement is only calculated for distinct bodies
        if (this != &bodies.at(i)) {
            double dx = bodies.at(i).getX() - xStep;
            double dy = bodies.at(i).getY() - yStep;

            // Calculating radial distance between bodies
            r = sqrt(dx * dx + dy * dy);

            // Checking if distance between bodies is zero before calculating acceleration for a given timestep
            if (r == 0) {
                ax = 0;
                ay = 0;
            }

            else {
                // Adding up accelerations due to every other body's force acting on body i
                ax += G * bodies.at(i).getMass() * dx / (r * r * r);
                ay += G * bodies.at(i).getMass() * dy / (r * r * r);
            }
        }
    }
}

// Implementing the Runge-Kutta 4th order numerical method to integrate the system of ODEs

void Body::updatePosition(const std::vector<Body>& bodies, const unsigned int N, const double G, const double dt) {
    /*
     * Inputs: 
     * std::vector<Body>& bodies --- Reference to the vector of bodies to allow direct manipulation of the body objects
     *
     * int N ----------------------- Number of bodies in the orbit simulation
     *
     * double G -------------------- Gravitational constant
     * 
     * Outputs: There is no required output from this function as it just modifies member variables
     * so 'void' is the return type
     * 
     * NOTE: Some variables are declared as const <DATATYPE> as they shouldn't be modified by this function
     */

    // Defining the four constants needed for the numerical method
    updateAcceleration(bodies, N, G, x, y);
    std::vector<double> k1 = { vx * dt, vy * dt, ax * dt, ay * dt };

    updateAcceleration(bodies, N, G, x + 0.5 * k1[0], y + 0.5 * k1[1]);
    std::vector<double> k2 = { (vx + 0.5 * k1[2]) * dt, (vy + 0.5 * k1[3]) * dt, ax * dt, ay * dt };

    updateAcceleration(bodies, N, G, x + 0.5 * k2[0], y + 0.5 * k2[1]);
    std::vector<double> k3 = { (vx + 0.5 * k2[2]) * dt, (vy + 0.5 * k2[3]) * dt, ax * dt, ay * dt };

    updateAcceleration(bodies, N, G, x + k3[0], y + k3[1]);
    std::vector<double> k4 = { (vx + k3[2]) * dt, (vy + k3[3]) * dt, ax * dt, ay * dt };


    // Performing the integration of the coupled differential equations
    x += (k1[0] + 2.0 * k2[0] + 2.0 * k3[0] + k4[0]) / 6.0;
    y += (k1[1] + 2.0 * k2[1] + 2.0 * k3[1] + k4[1]) / 6.0;
    vx += (k1[2] + 2.0 * k2[2] + 2.0 * k3[2] + k4[2]) / 6.0;
    vy += (k1[3] + 2.0 * k2[3] + 2.0 * k3[3] + k4[3]) / 6.0;
}