#ifndef CELESTIALOBJECT_H
#define CELESTIALOBJECT_H

#include <vector>

using namespace std;

// Enum to represent the ty pe of the object (asteroid, life-up, or ammo)
enum ObjectType {
    ASTEROID = 0,
    LIFE_UP = 1,
    AMMO = 2
};

class CelestialObject {
public:

    // Constructor to initialize CelestialObject with essential properties
    CelestialObject(const vector<vector<bool>> &shape, ObjectType type, int start_row, int time_of_appearance);

    // Copy constructor for CelestialObject
    CelestialObject(const CelestialObject *other);

    // Shape of the object represented as a 2D boolean vector
    vector<vector<bool>> shape;

    // Pointer to the object's clockwise neighbor (its right rotation)
    CelestialObject *right_rotation = nullptr;

    // Pointer to the object's clockwise neighbor (its left rotation)
    CelestialObject *left_rotation = nullptr;

    // Pointer to the next celestial object in the list
    CelestialObject *next_celestial_object = nullptr;

    // Type of the object (asteroid, life-up, or ammo)
    ObjectType object_type;

    // Row in the space grid where the object's top will appear
    int starting_row;

    int entire_size;

    int current_size;

    // The step in the game after which the object will appear on the grid
    int time_of_appearance;

    // Function to delete rotations of a given celestial object. It should free the dynamically allocated
    // space for each rotation.
    static void delete_rotations(CelestialObject *target);

    // Getter for entire_size
    int get_entire_size() const {
        return entire_size;
    }

    // Setter for entire_size
    void set_entire_size(int size) {
        entire_size = size;
    }
    // Getter for current_size
    int get_current_size() const {
        return current_size;
    }

    // Setter for current_size
    void set_current_size(int size) {
        entire_size = size;
    }
};

#endif // CELESTIALOBJECT_H
