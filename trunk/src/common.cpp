#include "defs.h"


void debug(string dbg) {
#ifdef _DEBUG_
  cout << dbg << endl;
#endif
}

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

void removeSky(IplImage* src) {
  debug("Entering removeSky");
  IplImage* hsv = cvCreateImage(cvGetSize(src), 8, 3);
  cvCvtColor( src, hsv, CV_BGR2HSV );
  //cvSaveImage("temp.jpg", hsv);
  CvScalar s;
  CvPoint pt = cvPoint(0,0);
  CvScalar black = cvScalar(0,0,0);
  for(int y = 0; y < src->height/2; y++) {
    for (int x = 0; x < src->width; x++) {
      s = cvGet2D(hsv, y, x);
      //cout << "Hue: " << s.val[2] << " S: " << s.val[1] << " V: " << s.val[0] << endl;
      if((s.val[2] >= 210 && s.val[0] >= 100) ||
	 (s.val[2] >= 250 && s.val[1] <= 6)) {
	s.val[0] = 0;
	s.val[1] = 0;
	s.val[2] = 0;
	cvSet2D(src, y, x, s);
      } else {
	//cout << "Hue: " << s.val[2] << " S: " << s.val[1] << " V: " << s.val[0] << endl;
      }
    }
  }
  debug("Leaving removeSky");
}

#define NUM_BINS 18

int colorHistVector(string img_name, float vec[]) {
  debug("Entering colorHistVector");
  IplImage* src;
  CvHistogram *hist;
  if((src=cvLoadImage(img_name.c_str(), 1))!= 0) {
    int scale = (int)ceil((double)src->width / 600);
    int width = src->width / scale;
    int height = src->height / scale;
    IplImage* img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
    cvResize(src, img, CV_INTER_AREA);
    
    removeSky(img);
    //string sky_img_name = img_name.insert(img_name.size()-4, "_sky");
    //cvSaveImage(sky_img_name.c_str(), img);
    IplImage* hsv = cvCreateImage( cvGetSize(img), 8, 3 );
    IplImage* h_plane = cvCreateImage( cvGetSize(img), 8, 1 );
    IplImage* s_plane = cvCreateImage( cvGetSize(img), 8, 1 );
    IplImage* v_plane = cvCreateImage( cvGetSize(img), 8, 1 );
    IplImage* planes[] = { h_plane };
    int hist_size[] = {18, 3, 3};
    float h_ranges[] = { 1, 179 };
    float s_ranges[] = { 1, 254 };
    float v_ranges[] = { 1, 254 };
    float* ranges[] = { h_ranges, s_ranges, v_ranges };
    
    cvCvtColor( img, hsv, CV_BGR2HSV );
    cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0);
    hist = cvCreateHist( 1, hist_size, CV_HIST_ARRAY, ranges, 1 );
    cvCalcHist( planes, hist, 0, 0 );
    cvNormalizeHist(hist, 10);
    for (int i = 0; i < 18; i++) {
      vec[i] = cvQueryHistValue_1D(hist, i);
    }
    /*for(int i = 0; i < 20; i++) {
      for(int j = 0; j < 3; j++) {
	for(int k = 0; k < 3; k++) {
	  vec[9*i + 3*j + k] = cvQueryHistValue_3D(hist, i, j, k);
	}
      }
      }*/
    debug("Leaving colorHistVector");
    return 0;
  }
  return -1;
}

#define NUM_LINES 2*75

int calcLineVector(string img_name, float vec[]) {
  IplImage* src;
  if((src=cvLoadImage(img_name.c_str(), 0))!= 0) {
    int scale = (int)ceil((double)src->width / 600);
    int width = src->width / scale;
    int height = src->height / scale;
    IplImage* img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
    cvResize(src, img, CV_INTER_AREA);

    IplImage* dst = cvCreateImage( cvGetSize(img), 8, 1 );
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* lines = 0;
    map<double, double> edges;

    int i;
    cvCanny( img, dst, 50, 200, 3 );
    lines = cvHoughLines2(
        dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 70, 10, 10 );

    for( i = 0; i < lines->total; i++ ) {
      CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
      int x1 = line[0].x;
      int x2 = line[1].x;
      int y1 = line[0].y;
      int y2 = line[1].y;
      int diffy = y2 - y1;
      int diffx = x2 - x1;
      double length = sqrt(pow(diffy, 2) + pow(diffy, 2));
      double theta;
      if (diffx == 0) {
	theta = 1.5708;
      } else {
	theta = atan(diffy / diffx);
      }
      edges.insert(pair<double, double>(length, theta));
    }

    map<double, double>::reverse_iterator iter = edges.rbegin();
    for (i = 0; iter != edges.rend() && i < NUM_LINES; iter++, i = i + 2) {
      vec[i] = iter->first;
      vec[i+1] = iter->second;
    }
    return 0;
  }
  return -1;
}
