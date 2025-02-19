#include <stdio.h>
#include <stdlib.h>

#pragma pack(1) 

struct Pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

int main() {
    FILE *origin, *new;

    // Open BMP files
    origin = fopen("darthvador(1).bmp", "rb");
    new = fopen("darthvador(1)_mirrored.bmp", "wb");

    // Read and write BMP header (54 bytes)
    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, origin);
    fwrite(header, sizeof(unsigned char), 54, new);

    // Get width and height from header
    int width = *(int *)&header[18];
    int height = *(int *)&header[22];

    // Calculate padding
    int padding = (4 - (width * 3) % 4) % 4;

    // Allocate memory for a row of pixels
    struct Pixel *row = malloc(width * sizeof(struct Pixel));

    // Read, mirror, and write each row
    for (int y = 0; y < height; y++) {
        fread(row, sizeof(struct Pixel), width, origin);
        fseek(origin, padding, SEEK_CUR); // Skip padding

        for (int x = width - 1; x >= 0; x--) {
            fwrite(&row[x], sizeof(struct Pixel), 1, new);
        }

        // Write padding
        for (int p = 0; p < padding; p++) {
            fputc(0, new);
        }
    }

    // Clean up
    free(row);
    fclose(origin);
    fclose(new);

    return 0;
}
