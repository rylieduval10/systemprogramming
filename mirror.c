#include <stdio.h>
#include <stdlib.h>

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

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, origin);
    fwrite(header, sizeof(unsigned char), 54, new);

    // Get width and height from header
    int width = *(int *)&header[18];
    int height = *(int *)&header[22];

    int padding = (4 - (width * 3) % 4) % 4;

    struct Pixel *row = malloc(width * sizeof(struct Pixel));

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

    free(row);
    fclose(origin);
    fclose(new);

    return 0;
}
