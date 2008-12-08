#include <iostream>
#include <cv.h>
#include <cvaux.h>
#include <ml.h>
#include <highgui.h>
#include <math.h>
#include <map>

#include "defs.h"
using namespace std;

#define NUM_LINES 2*75

int main(int argc, char** argv) {
  FILE* fp = fopen(argv[1], "r");
  FILE* out = fopen("edgedata.dat", "awb");
  char buf[500];

  int numImg = 0;
  while(fgets(buf, 500, fp)) {
    // Erase the newline.
    buf[strlen(buf) - 1] = '\0';
    string db_img(buf);
    float vec[NUM_LINES] = { -100 };
    calcLineVector(db_img, vec);
    fwrite(vec, 1, sizeof(vec), out);
  }
}
