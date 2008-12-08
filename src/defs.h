#ifndef __DEFS__
#define __DEFS__

/************************************************************************
Demo software: Invariant keypoint matching.
Author: David Lowe

defs.h:
This file contains the headers for a sample program to read images and
  keypoints, then perform simple keypoint matching.
*************************************************************************/

/* From the standard C libaray: */
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <cv.h>
#include <cvaux.h>
#include <ml.h>
#include <highgui.h>
#include <math.h>
#include <map>
#include <algorithm>
using namespace std;

/*------------------------------ Macros  ---------------------------------*/

#define ABS(x)    (((x) > 0) ? (x) : (-(x)))
#define MAX(x,y)  (((x) > (y)) ? (x) : (y))
#define MIN(x,y)  (((x) < (y)) ? (x) : (y))


/*---------------------------- Structures --------------------------------*/

/* Data structure for a float image.
*/
typedef struct ImageSt {
  int rows, cols;          /* Dimensions of image. */
  float **pixels;          /* 2D array of image pixels. */
  struct ImageSt *next;    /* Pointer to next image in sequence. */
} *Image;


/* Data structure for a keypoint.  Lists of keypoints are linked
   by the "next" field.
*/
typedef struct KeypointSt {
  float row, col;             /* Subpixel location of keypoint. */
  float scale, ori;           /* Scale and orientation (range [-PI,PI]) */
  unsigned char *descrip;     /* Vector of descriptor values */
  struct KeypointSt *next;    /* Pointer to next keypoint in list. */
} *Keypoint;



/*-------------------------- Function prototypes -------------------------*/
/* These are prototypes for the external functions that are shared
   between files.
*/

/* From util.c */
void FatalError(char *fmt, ...);
Image CreateImage(int rows, int cols);
Image ReadPGMFile(char *filename);
Image ReadPGM(FILE *fp);
void WritePGM(FILE *fp, Image image);
void DrawLine(Image image, int r1, int c1, int r2, int c2);
Keypoint ReadKeyFile(char *filename);
Keypoint ReadKeys(FILE *fp);

/* From common.cpp */
// There is a weird behavior where it wouldn't accept these functions being
// defined in common.h.  I guess this spot is okay.
int FindMatches(Keypoint keys1, Keypoint keys2);
Keypoint CheckForMatch(Keypoint key, Keypoint klist);
int DistSquared(Keypoint k1, Keypoint k2);
int calcLineVector(string img_name, float vec[]);
int colorHistVector(string img_name, float vec[]);
void removeSky(IplImage* src);
void debug(string dbg);

#endif
