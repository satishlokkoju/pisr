/************************************************************************
Demo software: Invariant keypoint matching.
Author: David Lowe

match.c:
This file contains a sample program to read images and keypoints, then
   draw lines connecting matched keypoints.

Adapted by Mark Knichel, 2008.
This program does basic exhaustive matching which is inefficient in almost
all cases.
*************************************************************************/

#include "defs.h"

/*----------------------------- Routines ----------------------------------*/

void convertToKeyFile(char* imageFile, char* keyFile);

int main (int argc, char **argv)
{
  if (argc != 3) {
    fprintf(stderr, "Usage is: ./match keyFile inputFile\n");
    return 0;
  }
  char* key = argv[2];
  FILE* fp = fopen(argv[1], "r");
  char buf[500];
  char keyBuf[500];
  Keypoint k1 = NULL, k2 = NULL;
  int maxMatches = 0;
  char maxFile[500];
  k1 = ReadKeyFile(key);

  while(fgets(buf, 500, fp)) {
    // Erase the newline.
    buf[strlen(buf) - 1] = '\0';
    convertToKeyFile(buf, keyBuf);
    fprintf(stderr, "%s", buf);
    k2 = ReadKeyFile(keyBuf);
    if(k1 == NULL || k2 == NULL) {
      continue;
    }
    int numMatches = FindMatches(k1, k2);
    fprintf(stderr, "Number of matches: %d\n", numMatches);
    if(numMatches > maxMatches) {
      maxMatches = numMatches;
      strcpy(maxFile, buf);
    }
  }
  fprintf(
      stderr, "The best match is %s with %d matches.\n", maxFile, maxMatches);
  return 0;
}

void convertToKeyFile(char* imageFile, char* keyFile) {
  strncpy(keyFile, imageFile, 500);
  char* ext = strrchr(keyFile, '.') + 1;
  strcpy(ext, "key");
}
