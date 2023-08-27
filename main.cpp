#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <exception>
#include <algorithm>
#include <iterator>
#include "Body.h" // Ensuring that the header file containing the "Body" class in included

int main() {

    // Initialising variables

    int N = 0;
    double G = 0.0, T = 0.0, dt = 0.0, t = 0.0;

    double tempX = 0.0, tempY = 0.0; // Temporary position variables for each body
    double tempVX = 0.0, tempVY = 0.0; // Temporary velocity variables for each body
    double tempMass = 0.0;

    // Declaring vector of Body objects
    std::vector<Body> bodies = {};

    // INPUT SECTION

    try {

        std::ifstream inputFile("parameters.txt"); // Opening the input file

        // Checking if the file is readable
        if (inputFile.good()) {

            //Checking if the file is empty
            if (inputFile.tellg() == -1) {
                throw std::runtime_error("The input file is empty");
            }

            std::string line; // Declaring a string to store the first line of the input file
            std::getline(inputFile, line);
            std::stringstream lineStream(line);

            // Moving the pointer back to the start of the file
            inputFile.seekg(0);

            if (!(lineStream >> G >> T >> dt)) {
                throw std::runtime_error("Incorrect data entry for line 1 in 'parameters.txt'");
            }

            // Testing if G, T, and dt have positive values
            if (G <= 0 || T <= 0 || dt <= 0) {
                throw std::runtime_error("G, T, and dt must have positive values.");
            }


            // While loop to count number of lines in "parameters.txt" to determine number of bodies
            while (true) {

                // Tests if end of file has been reached before incrementing N
                if (inputFile.eof()) {
                    break;
                }
                getline(inputFile, line);
                N++;
            }
            N = N - 1; // Decrementing N because line 1 in "parameters.txt" doesn't contain body data


            // Clearing end-of-file flag so that the file can be read again to get body data
            inputFile.clear();
            inputFile.seekg(0);

            getline(inputFile, line); // Reading in first line of data

            // Loop to add instances of the Body class to the bodies vector using input file data
            for (int i = 0; i < N; i++) {
                if (!(inputFile >> tempX >> tempY >> tempVX >> tempVY >> tempMass)) {
                    throw std::runtime_error("Incorrect data entry for body " + std::to_string(i + 1) + " in 'parameters.txt'. Please ensure datatypes and number of parameters is correct.");
                }

                // Testing if masses are negative
                if (tempMass <= 0) {
                    throw std::runtime_error("Body " + std::to_string(i + 1) + " has negative mass. Please ensure that all masses are positive.");
                }
                bodies.push_back(Body(tempX, tempY, tempVX, tempVY, tempMass));
            }


            // Closing the file after all necessary data has been read into the variables
            inputFile.close();

            // Checking if any pair of bodies have the same starting position
            for (int i = 0; i < bodies.size(); i++) {
                for (int j = i + 1; j < bodies.size(); j++) {
                    if (bodies.at(i).getX() == bodies.at(j).getX() && bodies.at(i).getY() == bodies.at(j).getY()) {
                        throw std::runtime_error("Please ensure that all bodies have distinct starting positions.");
                    }
                }
            }

        }

        else {
            throw std::ios_base::failure("Error opening file: parameters.txt");
        }
    }

    // Catch blocks for exception handling for input section

    catch (const std::ios_base::failure& e) {
        std::cerr << "An error occurred in the input section: " << e.what() << std::endl;
        std::exit(1);
    }

    catch (const std::runtime_error& e) {
        std::cerr << "An error occurred in the input section: " << e.what() << std::endl;
        std::exit(1);
    }

    // START OF OUTPUT SECTION

    try {
        // Creating/opening output file
        std::ofstream outputFile("output.txt", std::ios::trunc); // For output only, rewrites file if already exists

        if (!(outputFile.good())) {
            throw std::runtime_error("Failed to produce output file. Please ensure that you have file writing permissions.");
        }

        // For loop to update the dynamics of every Body object throughout the simulation time period

        for (double t = 0; t <= T + dt; t += dt) {

            // Updating acceleration, velocity, and position of all bodies
            for (int i = 0; i < N; i++) {
                bodies.at(i).updatePosition(bodies, N, G, dt);
            }

            // Outputting data to file using an output stream iterator

            int j = 0;
            std::transform(bodies.begin(), bodies.end(), std::ostream_iterator<std::string>(outputFile, "\n"),
                [&](Body& body) { // Capturing by reference to modify the actual data (and not a copy of it)
                    return std::to_string(++j) + " " +
                        std::to_string(t) + " " +
                        std::to_string(body.getX()) + " " +
                        std::to_string(body.getY()) + " " +
                        std::to_string(body.getVX()) + " " +
                        std::to_string(body.getVY()); });
        }

        // Closing output file
        outputFile.close();

    }

    // Catch block for exception handling for output section

    catch (const std::runtime_error& e) {
        std::cout << "An error occurred in the output section: " << e.what() << std::endl;
        std::exit(2);
    }

    return 0;
}