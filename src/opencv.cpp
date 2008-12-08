#include <iostream>
#include <cv.h>
#include <cvaux.h>
#include <ml.h>
#include <highgui.h>
#include <math.h>
#include <map>
#include <algorithm>

#include "defs.h"
using namespace std;

CvKNearest knnTrain(string edge_data);
CvKNearest colorTrain(string hist_data);

#define NUM_BINS 18
#define NUM_LINES 2*75
#define NUM_DB_IMGS 288

string extractBuilding(string img_name) {
  int idx2 = img_name.find_last_of('/');
  int idx1 = img_name.find_last_of('/', idx2-1);
  return img_name.substr(idx1+1, idx2-idx1-1);
}

void runMatcher(int argc, char** argv) {
  string file_name(argv[1]);
  string histData(argv[2]);
  string edgeData(argv[3]);
  string files[NUM_DB_IMGS];
  FILE *file_names = fopen(argv[1], "r");
  char buf[500];
  int idx = 0;
  while(fgets(buf, 500, file_names)) {
   // Erase the newline.
    buf[strlen(buf) - 1] = '\0';
    string img_name(buf);
    files[idx++] = img_name;
  }
  CvKNearest lineKnn = knnTrain(edgeData);
  CvKNearest colorKnn = colorTrain(histData);
  cout << "Waiting for input..." << endl;

  FILE *fp = fopen(argv[4], "r");
  while(fgets(buf, 500, fp)) {
    // Erase the newline.
    buf[strlen(buf) - 1] = '\0';
    string img_name(buf);
    CvMat* nearestLines = cvCreateMat( 1, 10, CV_32FC1);
    CvMat* distanceLines = cvCreateMat(1, 10, CV_32FC1);
    CvMat* nearestColors = cvCreateMat( 1, 10, CV_32FC1);
    CvMat* distanceColors = cvCreateMat(1, 10, CV_32FC1);
    float vec[NUM_LINES] = { -100 };
    calcLineVector(img_name, vec);
    float colorVec[NUM_BINS];
    colorHistVector(img_name, colorVec);
    CvMat lineSample = cvMat(1, NUM_LINES, CV_32FC1, vec);
    CvMat colorSample = cvMat(1, NUM_BINS, CV_32FC1, colorVec);
    float response = lineKnn.find_nearest(&lineSample,10,0,0,nearestLines,distanceLines);
    colorKnn.find_nearest(&colorSample,10,0,0,nearestColors,distanceColors);
    
    map<string, float> scores;

    //cout << "LINES:" << endl;
    for(int i = 0; i < 10; i++) {
      string img_name = files[(int)nearestLines->data.fl[i]];
      //cout << files[(int)nearestLines->data.fl[i]] << endl;
      //cout << distanceLines->data.fl[i] << endl;
      scores[extractBuilding(img_name)] += ((float)(10-i))/10;
      //scores.insert(pair<string, float>(extractBuilding(img_name), (10-i)/10));
    }
    //cout << "------" << endl;
    //cout << "COLOR:" << endl;
    for(int i = 0; i < 10; i++) {
      string img_name = files[(int)nearestColors->data.fl[i]];
      //cout << files[(int)nearestColors->data.fl[i]] << endl;
      //cout << distanceColors->data.fl[i] << endl;
      scores[extractBuilding(img_name)] += ((float)(10-i))/10;
    }
    cout << "Results for " << img_name << ":" << endl;
    //sort(scores.begin(), scores.end(), scores.value_comp());
    multimap<float, string> ranked_scores;
    map<string, float>::iterator iter = scores.begin();
    for ( ; iter != scores.end(); iter++) {
      ranked_scores.insert(pair<float, string>(iter->second, iter->first));
      //cout << iter->first << ": " << iter->second << endl;
    }
    multimap<float, string>::reverse_iterator ranked_iter =
        ranked_scores.rbegin();
    for ( ; ranked_iter != ranked_scores.rend(); ranked_iter++) {
      cout << ranked_iter->second << ": " << ranked_iter->first << endl;
    }
    cout << endl;
  }
}

int main(int argc, char** argv) {
  runMatcher(argc, argv);
  return 0;
}

CvKNearest knnTrain(string edgeData) {
  const int K = 10;
  CvMat* trainData = cvCreateMat(NUM_DB_IMGS, NUM_LINES, CV_32FC1);
  CvMat* trainClasses = cvCreateMat(NUM_DB_IMGS, 1, CV_32FC1);

  FILE* fp = fopen(edgeData.c_str(), "r");
  float vec[NUM_LINES] = { -100 };

  int numImg = 0;
  while(fread(vec, 1, sizeof(vec), fp)) {
    for(int i = 0; i < NUM_LINES; i++) {
      cvmSet(trainData, numImg, i, vec[i]);
    }
    cvmSet(trainClasses, numImg, 0, (float)numImg);
    numImg++;
  }
  CvKNearest knn(trainData, trainClasses, 0, false, K);
  return knn;
}

CvKNearest colorTrain(string histData) {
  const int K = 10;
  CvMat* trainData = cvCreateMat(NUM_DB_IMGS, NUM_BINS, CV_32FC1);
  CvMat* trainClasses = cvCreateMat(NUM_DB_IMGS, 1, CV_32FC1);

  FILE* fp = fopen(histData.c_str(), "r");
  float vec[NUM_BINS] = { -100 };
  
  int numImg = 0;
  while(fread(vec, 1, sizeof(vec), fp)) {
    for(int i = 0; i < NUM_BINS; i++) {
      cvmSet(trainData, numImg, i, vec[i]);
    }
    cvmSet(trainClasses, numImg, 0, (float)numImg);
    numImg++;
  }
  CvKNearest knn(trainData, trainClasses, 0, false, K);
  return knn;
}
