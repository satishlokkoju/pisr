#include "defs.h"


/**
 * Returns the number of matches of keys between the two sets of keypoints.
 *
 * @return The number of matches between the two sets of keypoints.
 */
int FindMatches(Keypoint keys1, Keypoint keys2)
{
    Keypoint k, match;
    int count = 0;

    for (k= keys1; k != NULL; k = k->next) {
      match = CheckForMatch(k, keys2);  

      if (match != NULL) {
	count++;
      }
    }

    return count;
}


/**
 * This searches through the keypoints in klist for the two closest
 * matches to key.  If the closest is less than 0.8 times distance to
 * second closest, then return the closest match.  Otherwise, return
 * NULL.
 *
 * @return The closest key or NULL if no match was found.
 */
Keypoint CheckForMatch(Keypoint key, Keypoint klist)
{
    int dsq, distsq1 = 100000000, distsq2 = 100000000;
    Keypoint k, minkey = NULL;

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

    if (100 * distsq1 < 80 * distsq2) {
      return minkey;
    }
    else return NULL;
}


/**
 * @return squared distance between two keypoint descriptors.
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
