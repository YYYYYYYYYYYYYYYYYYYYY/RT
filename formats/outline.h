#include <iostream>
#include <cstring>
#include <random>
/**
 * Class that should outline shape on the picture
 */
class Outline {
public:
    /**
     * method that outline needed shape on the picture pic1.
     * @param N - Height of the picture.
     * @param M - Width of the picture.
     * @param pic1 - Picture when we outline shape (array of N*M*3 int).
     * @param mask - Mask with shape's number on it.
     * @param shapeNum - Number of shape on mask.
     * @return - new picture with outlined picture (array of N*M*3 int).
     */
    char *outline(int N, int M, char *pic1, int *mask, int shapeNum) {
        Y = N;
        X = M;
        pic2 = mask;
        newPic = pic1;
        pic2 = mask;
        curr_sum = shapeNum * 8;
        shape_n = shapeNum;

        walking();
        return newPic;
    }

private:
    int X, Y;
    int *pic2;
    char *newPic;
    int curr_sum;
    int shape_n;
    int R,G,B;
    void walking() {
        for (int y = 0; y < Y; ++y) {
            for (int x = 0; x < X; ++x) {
                if (pic2[X * y + x] == shape_n && isBoundary(x, y))
                    colourAround(x, y);
            }
        }
    }

    bool isGood(int x, int y) {
        return 0 <= x && x < X && 0 <= y && y < Y;
    }

/*
 * TODO:
 * change colour to Vector3( or just RGB )
 */
    void colourAround(int x, int y) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (isGood(x + i, y + j)) {
                    R = 255*rand()%255;
                    G = 153*rand()%255;
                    B = 51*rand()%255;
                    newPic[(X * (y + i) + x + j) * 3] = R;
                    newPic[(X * (y + i) + x + j) * 3 + 1] = G;
                    newPic[(X * (y + i) + x + j) * 3 + 2] = B;
                }

            }
        }
    }

    bool isBoundary(int x, int y) {
        int sum = 0;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i != 0 || j != 0) {
                    if (isGood(x + i, y + j)) {
                        sum += (pic2[X * (y + j) + x + i]);
                    } else return false;
                }
            }
        }
        return sum != curr_sum;
    }
};