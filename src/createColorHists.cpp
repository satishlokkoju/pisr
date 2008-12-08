#include <iostream>
#include <cv.h>
#include <cvaux.h>
#include <ml.h>
#include <highgui.h>
#include <math.h>
#include <map>
#include <stdio.h>

#include "defs.h"
using namespace std;

#define NUM_BINS 18

int main(int argc, char** argv) {
  FILE* fp = fopen(argv[1], "r");
  FILE* out = fopen("histdata.dat", "awb");
  char buf[500];

  while(fgets(buf, 500, fp)) {
    // Erase the newline.
    buf[strlen(buf) - 1] = '\0';
    string db_img(buf);
    float vec[NUM_BINS] = { -100 };
    colorHistVector(db_img, vec);
    fwrite(vec, 1, sizeof(vec), out);
  }
}
