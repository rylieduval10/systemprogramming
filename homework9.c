#include <stdio.h>
#include <stdlib.h>

struct Pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

void rotate(char *path, char *newPath, int mode) {
    FILE *origin = fopen(path, "rb");
    FILE *new = fopen(newPath, "wb");
    
    unsigned char header[54];
    
    fread(header, sizeof(unsigned char), 54, origin);
    fwrite(header, sizeof(unsigned char), 54, new);
    
    int width = *(int *)&header[18];
    int height = *(int *)&header[22];
    int padding = (4 - (width * 3) % 4) % 4;
    
    struct Pixel *data = malloc(width * height * sizeof(struct Pixel));
    
    for (int y = 0; y < height; y++) {
        fread(&data[y * width], sizeof(struct Pixel), width, origin);
        fseek(origin, padding, SEEK_CUR);
    }
    
    if (mode == 1) { // 90-degree rotation
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                fwrite(&data[(height - 1 - x) * width + y], sizeof(struct Pixel), 1, new);
            }
        }

    } else if (mode == 2) { // 180-degree rotation
        for (int y = height - 1; y >= 0; y--) {
            for (int x = width - 1; x >= 0; x--) {
                fwrite(&data[y * width + x], sizeof(struct Pixel), 1, new);
            }
        }

    } else if (mode == 3) { // 270-degree rotation
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                fwrite(&data[x * width + (width - y - 1)], sizeof(struct Pixel), 1, new);
            }
        }

    } else if (mode == 4) { // mirror left to right
        for (int y = 0; y < height; y++) {
            for (int x = width - 1; x >= 0; x--) {
                fwrite(&data[y * width + x], sizeof(struct Pixel), 1, new);
            }
        }

    } else if (mode == 5) { // upside down flip
        for (int y = height - 1; y >= 0; y--) {
            fwrite(&data[y * width], sizeof(struct Pixel), width, new);
        }

    } else if (mode == 6) { // shrink by 50%
        int newWidth = width / 2;
        int newHeight = height / 2;
        for (int y = 0; y < newHeight; y++) {
            for (int x = 0; x < newWidth; x++) {
                fwrite(&data[(y * 2) * width + (x * 2)], sizeof(struct Pixel), 1, new);
            }
        }
    }
    
    free(data);
    fclose(origin);
    fclose(new);
}

int main() {
    rotate("darthvador(1).bmp", "output.bmp", 1);
    return 0;
}