#include "Crypto.h"
#include "GrayscaleImage.h"


// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage &secret_image, int message_length) {
    std::vector<int> LSB_array;
    GrayscaleImage reconstructed = secret_image.reconstruct();
    int image_dimension = reconstructed.get_width() * reconstructed.get_height();
    int total_bits = message_length * 7;
    if (total_bits > image_dimension) {
        throw std::runtime_error("Image hasn't enough pixels");
    }
    int starting_pixel = image_dimension - total_bits;
    int row_number = starting_pixel / reconstructed.get_width();
    int column_number = starting_pixel % reconstructed.get_width();
    for (int i = row_number; i < reconstructed.get_height(); ++i) {
        for (int j = column_number; j < reconstructed.get_width(); ++j) {
            int pixel_value = reconstructed.get_pixel(i, j);
            LSB_array.push_back(pixel_value % 2);
        }
        column_number = 0;
    }
    return LSB_array;
}


// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int> &LSB_array) {
    std::string message;
    if (LSB_array.size() % 7 != 0) {
        throw std::runtime_error("LSB array size is not a multiple of 7");
    }
    int bit_number = 0;
    int decimal_value=0;
    int base=64;
    for (int bit: LSB_array) {
        bit_number++;
        decimal_value+=bit*base;
        base=base/2;
        if (bit_number == 7) {
            char asciiChar = static_cast<char>(decimal_value);
            message += asciiChar;
            bit_number = 0;
            decimal_value=0;
            base=64;
        }
    }
    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string &message) {
    std::vector<int> LSB_array;
    for (char character: message) {
        std::bitset<7> binary(character);
        for (int i = 6; i >= 0; --i) {
            LSB_array.push_back(binary[i]);
        }
    }
    return LSB_array;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage &image, const std::vector<int> &LSB_array) {
    int image_dimension = image.get_height() * image.get_width();
    if (image_dimension < LSB_array.size()) {
        throw std::runtime_error("Image hasn't enough pixels");
    }
    int starting_pixel = image_dimension - LSB_array.size();
    int row_number = starting_pixel / image.get_width();
    int column_number = starting_pixel % image.get_width();
    int lsb_index = 0;
    for (int i = row_number; i < image.get_height(); ++i) {
        for (int j = column_number; j < image.get_width(); ++j) {
            if (image.get_pixel(i, j) % 2 != LSB_array[lsb_index]) {
                if (image.get_pixel(i, j) % 2 == 1) {
                    int new_value = std::min(255, std::max(0, image.get_pixel(i, j) - 1));
                    image.set_pixel(i, j, new_value);
                } else {
                    int new_value = std::min(255, std::max(0, image.get_pixel(i, j) + 1));
                    image.set_pixel(i, j, new_value);
                }
            }
            lsb_index++;
        }
        column_number = 0;
    }
    SecretImage secret_image(image);
    return secret_image;
}
