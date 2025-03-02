#include "SecretImage.h"


// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage &image) {
    int one_dimension = image.get_width();
    int lower_triangular_size = (one_dimension * one_dimension - one_dimension) / 2;
    int upper_triangular_size = lower_triangular_size + one_dimension;
    lower_triangular = new int[lower_triangular_size];
    upper_triangular = new int[upper_triangular_size];
    width = one_dimension;
    height = one_dimension;

    for (int i = 0; i < one_dimension; ++i) {
        for (int j = i; j < one_dimension; ++j) {
            upper_triangular[one_dimension * i + j - (i * (i + 1) / 2)] = image.get_pixel(i, j);
        }
    }
    for (int i = 1; i < one_dimension; ++i) {
        for (int j = 0; j < i; ++j) {
            lower_triangular[one_dimension * i + j - (i * one_dimension) + i * (i - 1) / 2] = image.get_pixel(i, j);
        }
    }
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int *upper, int *lower) {
    width = w;
    height = h;
    upper_triangular = new int[width * (width + 1) / 2];
    lower_triangular = new int[width * (width - 1) / 2];
    std::copy(upper, upper + (width * (width + 1) / 2), upper_triangular);
    std::copy(lower, lower + (width * (width - 1) / 2), lower_triangular);
    delete[] upper;
    delete[] lower;
}


// Destructor: free the arrays
SecretImage::~SecretImage() {
    delete[] upper_triangular;
    delete[] lower_triangular;
}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);
    int one_dimension = height;
    for (int i = 0; i < one_dimension; ++i) {
        for (int j = i; j < one_dimension; ++j) {
            image.set_pixel(i, j, upper_triangular[one_dimension * i + j - (i * (i + 1) / 2)]);
        }
    }
    for (int i = 1; i < one_dimension; ++i) {
        for (int j = 0; j < i; ++j) {
            image.set_pixel(i, j, lower_triangular[one_dimension * i + j - (i * one_dimension) + i * (i - 1) / 2]);
        }
    }
    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage &image) {
    int one_dimension = image.get_width();
    for (int i = 0; i < one_dimension; ++i) {
        for (int j = i; j < one_dimension; ++j) {
            upper_triangular[one_dimension * i + j - (i * (i + 1) / 2)] = image.get_pixel(i, j);
        }
    }
    for (int i = 1; i < one_dimension; ++i) {
        for (int j = 0; j < i; ++j) {
            lower_triangular[one_dimension * i + j - (i * one_dimension) + i * (i - 1) / 2] = image.get_pixel(i, j);
        }
    }
}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string &filename) {
    std::ofstream file(filename);
    file << std::to_string(width) << " " << std::to_string(height) << std::endl;
    for (int i = 0; i < width * (width + 1) / 2 - 1; ++i) {
        file << upper_triangular[i] << " ";
    }
    file << upper_triangular[width * (width + 1) / 2 - 1] << std::endl;
    for (int i = 0; i < width * (width - 1) / 2 - 1; ++i) {
        file << lower_triangular[i] << " ";
    }
    file << lower_triangular[width * (width - 1) / 2 - 1] << std::endl;
    file.close();
}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string &filename) {
    std::ifstream file(filename);
    int width;
    int height;
    file >> width >> height;
    int lower_triangular_size = width * (width - 1) / 2;
    int upper_triangular_size = width * (width + 1) / 2;
    int *lower_triangular = new int[lower_triangular_size];
    int *upper_triangular = new int[upper_triangular_size];
    for (int i = 0; i < upper_triangular_size; ++i) {
        file >> upper_triangular[i];
    }
    for (int i = 0; i < lower_triangular_size; ++i) {
        file >> lower_triangular[i];
    }
    file.close();
    SecretImage secret(width, height, upper_triangular, lower_triangular);
    return secret;
}

// Returns a pointer to the upper triangular part of the secret image.
int *SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int *SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}
