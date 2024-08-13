#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define M 475 // cols
#define N 460 // rows

// Static arrays to store the input image, blurred image, and edge-detected image
unsigned char frame1[N * M];   // Input image
unsigned char filt[N * M];     // Output filtered image
unsigned char gradient[N * M]; // Output image

// 2D Gaussian mask with integers
const signed char Mask[5][5] = {
    {2, 4, 5, 4, 2},
    {4, 9, 12, 9, 4},
    {5, 12, 15, 12, 5},
    {4, 9, 12, 9, 4},
    {2, 4, 5, 4, 2}
};

// Sobel masks for edge detection
const signed char GxMask[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
};

const signed char GyMask[3][3] = {
    {-1, -2, -1},
    {0, 0, 0},
    {1, 2, 1}
};

// Function to open a file
void openfile(char* filename, FILE** finput) {
    if (fopen_s(finput, filename, "r") != 0) {
        printf("Error opening file %s for reading\n", filename);
        exit(1);
    }
}

// Function to apply Gaussian blur to the input image
void Gaussian_Blur() {
    int row, col, rowOffset, colOffset;
    int newPixel;
    unsigned char pix;

    // Iterate over each pixel in the input image
    for (row = 0; row < N; row++) {
        for (col = 0; col < M; col++) {
            newPixel = 0;
            // Apply the Gaussian blur mask to the current pixel
            for (rowOffset = -2; rowOffset <= 2; rowOffset++) {
                for (colOffset = -2; colOffset <= 2; colOffset++) {
                    if ((row + rowOffset < 0) || (row + rowOffset >= N) || (col + colOffset < 0) || (col + colOffset >= M))
                        pix = 0;
                    else
                        pix = frame1[M * (row + rowOffset) + col + colOffset];

                    newPixel += pix * Mask[2 + rowOffset][2 + colOffset];
                }
            }
            // Store the blurred pixel value in the output array
            filt[M * row + col] = (unsigned char)(newPixel / 159);
        }
    }
}

// Function to apply Sobel edge detection to the blurred image
void Sobel() {
    int row, col, rowOffset, colOffset;
    int Gx, Gy;

    // Iterate over each pixel in the blurred image
    for (row = 1; row < N - 1; row++) {
        for (col = 1; col < M - 1; col++) {
            Gx = 0;
            Gy = 0;

            // Apply the Sobel masks to the current pixel
            for (rowOffset = -1; rowOffset <= 1; rowOffset++) {
                for (colOffset = -1; colOffset <= 1; colOffset++) {
                    Gx += filt[M * (row + rowOffset) + col + colOffset] * GxMask[rowOffset + 1][colOffset + 1];
                    Gy += filt[M * (row + rowOffset) + col + colOffset] * GyMask[rowOffset + 1][colOffset + 1];
                }
            }

            // Calculate the gradient strength and store it in the output array
            gradient[M * row + col] = (unsigned char)sqrt(Gx * Gx + Gy * Gy);
        }
    }
}

int main() {
    // Call the functions here
    Gaussian_Blur();
    Sobel();

    return 0;
}