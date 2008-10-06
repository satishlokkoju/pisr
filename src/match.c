/************************************************************************
Demo software: Invariant keypoint matching.
Author: David Lowe

match.c:
This file contains a sample program to read images and keypoints, then
   draw lines connecting matched keypoints.

Adapted by Mark Knichel, 2008.
*************************************************************************/


#include "defs.h"

/* -------------------- Local function prototypes ------------------------ */

int FindMatches(Keypoint keys1, Keypoint keys2);
Keypoint CheckForMatch(Keypoint key, Keypoint klist);
int DistSquared(Keypoint k1, Keypoint k2);


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
  char buf[100];
  char keyBuf[100];
  Keypoint k1 = NULL, k2 = NULL;
  int maxMatches = 0;
  char maxFile[100];
  k1 = ReadKeyFile(key);

  while(fgets(buf, 100, fp)) {
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
  fprintf(stderr, "The best match is %s with %d matches.\n", maxFile, maxMatches);
  return 0;
}

void convertToKeyFile(char* imageFile, char* keyFile) {
  strncpy(keyFile, imageFile, 100);
  char* ext = strrchr(keyFile, '.') + 1;
  strcpy(ext, "key");
}

/* Given a pair of images and their keypoints, pick the first keypoint
   from one image and find its closest match in the second set of
   keypoints.  Then write the result to a file.
*/
int FindMatches(Keypoint keys1, Keypoint keys2)
{
    Keypoint k, match;
    Image result;
    int count = 0;

    /* Match the keys in list keys1 to their best matches in keys2.
    */
    for (k= keys1; k != NULL; k = k->next) {
      match = CheckForMatch(k, keys2);  

      if (match != NULL) {
	count++;
      }
    }

    return count;
}


/* This searches through the keypoints in klist for the two closest
   matches to key.  If the closest is less than 0.6 times distance to
   second closest, then return the closest match.  Otherwise, return
   NULL.
*/
Keypoint CheckForMatch(Keypoint key, Keypoint klist)
{
    int dsq, distsq1 = 100000000, distsq2 = 100000000;
    Keypoint k, minkey = NULL;

    /* Find the two closest matches, and put their squared distances in
       distsq1 and distsq2.
    */
    for (k = klist; k != NULL; k = k->next) {
      dsq = DistSquared(key, k);

      if (dsq < distsq1) {
	distsq2 = distsq1;
	distsq1 = dsq;
	minkey = k;
      } else if (dsq < distsq2) {
	distsq2 = dsq;
      }
    }

    /* Check whether closest distance is less than 0.6 of second. */
    if (10 * 10 * distsq1 < 10 * 7 * distsq2) {
      return minkey;
    }
    else return NULL;
}


/* Return squared distance between two keypoint descriptors.
*/
int DistSquared(Keypoint k1, Keypoint k2)
{
    int i, dif, distsq = 0;
    unsigned char *pk1, *pk2;

    pk1 = k1->descrip;
    pk2 = k2->descrip;

    for (i = 0; i < 128; i++) {
      dif = (int) *pk1++ - (int) *pk2++;
      distsq += dif * dif;
    }
    return distsq;
}
