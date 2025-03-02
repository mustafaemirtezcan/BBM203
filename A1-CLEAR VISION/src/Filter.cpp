#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage &image, int kernelSize) {
    GrayscaleImage original = image;
    int kernel_dimension = kernelSize / 2;
    for (int i = 0; i < original.get_height(); i++) {
        for (int j = 0; j < original.get_width(); j++) {
            int kernel_sum = 0;
            for (int k = -kernel_dimension; k <= kernel_dimension; k++) {
                for (int l = -kernel_dimension; l <= kernel_dimension; l++) {
                    if (i + k >= 0 && i + k < original.get_height() && j + l >= 0 && j + l < original.get_width()) {
                        kernel_sum += original.get_pixel(i + k, j + l);
                    }
                }
            }
            image.set_pixel(i, j, kernel_sum / (kernelSize * kernelSize));
        }
    }
}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage &image, int kernelSize, double sigma) {
    int kernel_dimension = kernelSize / 2;
    double gaussian_kernel[kernelSize][kernelSize];
    double sum_weights = 0.0;
    for (int k = -kernel_dimension; k <= kernel_dimension; k++) {
        for (int l = -kernel_dimension; l <= kernel_dimension; l++) {
            double weight =   exp(-(k * k + l * l) / (2.0 * sigma * sigma))/(2.0 * M_PI * sigma * sigma);
            gaussian_kernel[k + kernel_dimension][l + kernel_dimension] = weight;
            sum_weights += weight;
        }
    }
    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            gaussian_kernel[i][j] = gaussian_kernel[i][j] / sum_weights;
        }
    }
    GrayscaleImage original = image;
    for (int i = 0; i < original.get_height(); i++) {
        for (int j = 0; j < original.get_width(); j++) {
            double weighted_sum = 0;
            for (int k = -kernel_dimension; k <= kernel_dimension; k++) {
                for (int l = -kernel_dimension; l <= kernel_dimension; l++) {
                    if (i + k >= 0 && i + k < original.get_height() && j + l >= 0 && j + l < original.get_width()) {
                        weighted_sum += original.get_pixel(i + k, j + l) * gaussian_kernel[k + kernel_dimension][l + kernel_dimension];
                    }
                }
            }
            int new_value = weighted_sum;
            image.set_pixel(i, j, new_value);
        }
    }
}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage &image, int kernelSize, double amount) {
    GrayscaleImage blurred_image = image;
    apply_gaussian_smoothing(blurred_image, kernelSize, 1.0);
    for (int i = 0; i < image.get_height(); ++i) {
        for (int j = 0; j < image.get_width(); ++j) {
            int original_pixel = image.get_pixel(i, j);
            int blurred_image_pixel = blurred_image.get_pixel(i, j);
            int new_pixel_value = original_pixel + amount * (original_pixel - blurred_image_pixel);
            new_pixel_value = std::min(255, std::max(0, new_pixel_value));
            image.set_pixel(i, j, new_pixel_value);
        }
    }
}
