#include <iostream>
#include <cstring>

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
    int *outline(int N, int M, int *pic1, int *mask, int shapeNum) {
        Y = N;
        X = M;
        pic2 = mask;

        int start_point_x = -1;
        int start_point_y = -1;
        for (int i = 0; i < N; i += 1) {
            for (int j = 0; j < M; j++)
                if (mask[j + i * M] == shapeNum) {
                    start_point_x = j;
                    start_point_y = i;
                    break;
                }
            if (start_point_x != -1)
                break;
        }
        newPic = (int *) malloc(sizeof(int) * N * M * 3);
        pic2 = (int *) malloc(sizeof(int) * N * M);
        memcpy(newPic, pic1, sizeof(int) * N * M * 3);
        memcpy(pic2, mask, sizeof(int) * N * M);
        shape_n = shapeNum;
        walking(start_point_x, start_point_y);
        return newPic;
    }

private:
    int X, Y;
    int *pic2;
    int *newPic;
    int shape_n;

    void walking(int x, int y) {
        if (x > X || x < 0 || y < 0 || y > Y)
            return;
        if (abs(pic2[X * y + x]) != shape_n) {
            makePumkin(x, y);
            return;
        } else if ((pic2[X * y + x]) == -shape_n)
            return;
        pic2[X * y + x] = -shape_n;
        makeMandarine(x, y);
        if (!isBoundary(x, y)) return;
        walking(x + 1, y);
        walking(x + 1, y + 1);
        walking(x + 1, y - 1);
        walking(x - 1, y);
        walking(x - 1, y + 1);
        walking(x - 1, y - 1);
        walking(x, y - 1);
        walking(x, y + 1);
    }

// 255 136 0
    void makeMandarine(int x, int y) {
        newPic[(X * y + x) * 3] = 255;
        newPic[(X * y + x) * 3 + 1] = 136;
        newPic[(X * y + x) * 3 + 2] = 0;
    }

// 255 153 51
    void makePumkin(int x, int y) {
        newPic[(X * y + x) * 3] = 255;
        newPic[(X * y + x) * 3 + 1] = 153;
        newPic[(X * y + x) * 3 + 2] = 51;
    }

    bool isBoundary(int x, int y) {
        if (x + 1 >= X || (x + 1 < X && abs(pic2[X * y + x + 1]) != shape_n)) return true;
        if (x - 1 <= -1 || (x - 1 > -1 && abs(pic2[X * y + x - 1]) != shape_n)) return true;
        if (y - 1 <= -1 || (y - 1 > -1 && abs(pic2[X * (y - 1) + x]) != shape_n)) return true;
        if (y + 1 >= Y || (y + 1 < Y && abs(pic2[X * (y + 1) + x]) != shape_n)) return true;
        if (x + 1 < X && y - 1 > -1 && abs(pic2[X * (y - 1) + x + 1]) != shape_n) return true;
        if (x - 1 > -1 && y - 1 > -1 && abs(pic2[X * (y - 1) + x - 1]) != shape_n) return true;
        if (y + 1 < Y && x + 1 < X && abs(pic2[X * (y + 1) + x + 1]) != shape_n) return true;
        if (y + 1 < Y && x - 1 > -1 && abs(pic2[X * (y + 1) + x - 1]) != shape_n) return true;
        return false;
    }
};