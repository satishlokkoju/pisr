/**
 * @fileoverview An image matcher based off Fraudhorfer's visual vocabulary
 * tree implementation and SIFT.
 *
 * @author Mark Knichel
 */

#include <iostream>
#include <stdio.h>
#include <vector>

#include "defs.h"
#include "vocsearch/ff_voctree.h"
#include "vocsearch/ff_database.h"
#include "vocsearch/ff_sort.h"

using namespace std;


/**
 * Turns a keypoint file into an unsigned char array.
 */
int convertKeysToSift(Keypoint keys, unsigned char sift[]);


/**
 * Testing out whether we can use a combination of the visual vocabulary tree
 * and an exhaustive matcher.  This did not seem to help.
 */
void comboMatcher(char* file1, char* file2);

// TODO(Mark): Add usage.
int main(int argc, char **argv) {
  int num_images = 400;
  int max_num_descriptors = 6000;
  unsigned char *sift = new unsigned char[max_num_descriptors*128];
  ff_voctree voctree;
  voctree.init("src/vocsearch/voctrees/voctree.bin");
  ff_database db;
  db.init(num_images, voctree.nrvisualwords(), max_num_descriptors);
  unsigned int *vwread = new unsigned int[max_num_descriptors];
  Keypoint k;
  string filenames[num_images];

  FILE* fp = fopen(argv[1], "r");
  char buf[250];
  int index = 0;
  cout << "Inserting files into db" << endl;
  while(fgets(buf, 250, fp)) {
    // Erase the newline.
    buf[strlen(buf) - 1] = '\0';
    fprintf(stderr, "%d - %s\n", index, buf);
    k = ReadKeyFile(buf);
    if(k == NULL) {
      continue;
    }
    int size = convertKeysToSift(k, sift);
    fprintf(stderr, "Size: %d\n", size);
    for (int i = 0; i < size; i++) {
      voctree.quantize(&vwread[i], &sift[i*128]);
    }
    db.insertdoc(vwread, size, index);
    filenames[index] = buf;
    index++;
  }

  // compute idf score of the images in the database
  db.computeidf();
  // update normalization in database according to changed idf-weights
  db.normalize();

  int qdocnames[100];
  float qscores[100];

  while(1) {
    fgets(buf, 250, stdin);
    // Erase the newline.
    buf[strlen(buf) - 1] = '\0';
    cout << buf << endl;
    k = ReadKeyFile(buf);
    if (k == NULL) {
      cout << "Couldn't read key file." << endl;
    }
    int size = convertKeysToSift(k, sift);
    fprintf(stderr, "Size: %d\n", size);
    for (int i = 0; i < size; i++) {
      voctree.quantize(&vwread[i], &sift[i*128]);
    }
    db.querytopn(vwread, size, 10, qdocnames, qscores);   

    for (int j = 0; j < 10; j++) {
      cout << (j+1) << ": " << filenames[qdocnames[j]] << " Score: " <<
          qscores[j] << endl;
      //string f = filenames[qdocnames[j]];
      //char f2[f.size()+1];
      //strcpy(f2, f.c_str());
      //comboMatcher(buf, f2);
    }
  }

  return 0;
}

int convertKeysToSift(Keypoint keys, unsigned char sift[]) {
  Keypoint k;
  int i;
  int count = 0;
  for (k = keys; k != NULL; k = k->next) {
    unsigned char* d = k->descrip;
    for (i = 0; i < 128; i++) {
      sift[i * count] = *d;
    }
    count++;
  }
  return count;
}

void comboMatcher(char* file1, char* file2) {
  Keypoint k1, k2;
  k1 = ReadKeyFile(file1);
  k2 = ReadKeyFile(file2);
  if(k1 == NULL || k2 == NULL) {
    return;
  }
  int numMatches = FindMatches(k1, k2);
  fprintf(stderr, "Number of matches: %d\n", numMatches);
}
