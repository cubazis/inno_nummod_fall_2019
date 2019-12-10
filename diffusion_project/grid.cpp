#include <iostream>
#include <fstream>

using namespace std;

#define M 100

void pasteLike100(int grid[M][M], int x, int y ){
    int h1 = 26;
    int w1 = 10;
    int w2 = 6;
    int h2 = 6;

    for (int i = x; i < x + w1; i++) {
        for (int j = y; j < y + h2; j++) {
            grid[i][j] = 0;
        }
    }

    for (int i = x; i < x + w2; i++) {
        for (int j = y; j < y + h1; j++) {
            grid[i][j] = 0;
        }
    }
}

void pasteLike110(int grid[M][M], int x, int y ){
    int h1 = 26;
    int w1 = 10;
    int w2 = 6;
    int h2 = 6;

    for (int i = x; i < x + w1; i++) {
        for (int j = y-6; j < y; j++) {
            grid[i][j] = 0;
        }
    }

    for (int i = x + 3 + 1; i < x + w1; i++) {
        for (int j = y-h1; j < y; j++) {
            grid[i][j] = 0;
        }
    }
}

void pasteLike108(int grid[M][M], int x, int y ){
    for (int i = x; i < x+6; i++) {
        for (int j = y; j < y+6; j++) {
            grid[i][j] = 0;
        }
    }
}

void getGrid(int grid[100][100]) {
//    double e = 1e-5;
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            grid[i][j] = 1;
        }
    }

    pasteLike100(grid, 23, 4);
    pasteLike100(grid, 64, 4);
    pasteLike100(grid, 23, 55);
    pasteLike100(grid, 64, 55);
    pasteLike110(grid, 42 , 43);
    pasteLike110(grid, 64+19, 43);
    pasteLike110(grid, 42, 55+39);
    pasteLike110(grid, 64+19, 55+39);
    pasteLike108(grid, 23, 37);
    pasteLike108(grid, 46, 4);
    pasteLike108(grid, 64+19+4, 4);
    pasteLike108(grid, 64, 37);
    pasteLike108(grid, 23, 55+33);
    pasteLike108(grid, 46, 55);//
    pasteLike108(grid, 64, 55+33);
    pasteLike108(grid, 64+19+4, 55);

    //cin >> e;
//    ofstream gridfile;
//    gridfile.open("../grid.txt");
//    for (int j = 99; j >= 0; j--) {
//        for (int i = 0; i < 100; i++) {
//            gridfile << grid[i][j] << " ";
//        }
//        gridfile << "\n";
//    }
//    gridfile.close();
    return;
}