/*
 * @fileoverview Uses SIFT to convert a JPG picture
 * into a SIFT key file.
 *
 * TODO(Mark): This should all be done in a shell script.
 *
 * Usage: ./createkeyfile file convertedFile
 * where file is created by doing a ls (or ls -R equivalent)
 * of the files you want to convert to key files and
 * convertedFile is a file containing the full paths
 * of all images that were successfully converted to key files.
 *
 * @author Mark Knichel
 */

#include <iostream>
#include <fstream>
#include <cv.h>
#include <cvaux.h>
#include <highgui.h>  
using namespace std;

void createKeyFile(string filename);

int main(int argc, char** argv) {
  ifstream file1(argv[1]);
  ofstream file2(argv[2], ios_base::out | ios_base::trunc);
  string dir = "";
  char buf[100];
  while(file1.getline(buf, 100)) {
    string f(buf);
    string fn = dir + f;
    if(f.length() > 1 && f.at(f.length() - 1) == ':') {
      cout << "dir" << endl;
      dir = f;
      dir.at(f.length() - 1) = '/';
      continue;
    }
    if(fn.find(".JPG") != -1) {
      cout << fn << endl;
      file2 << fn << endl;
      createKeyFile(fn);
    }
  }
  file2.close();
  return 0;
}

void createKeyFile(string filename) {
  IplImage* img;
  char buf[100];
  strcpy(buf, filename.c_str());
  img = cvLoadImage(filename.c_str(), 0);
  char *ext = strrchr(buf, '.') + 1;
  strcpy(ext, "pgm");
  cout << img->width << endl;
  IplImage* newImg = cvCreateImage(cvSize(img->width / 4, img->height/4), 8, 1);
  cvResize(img, newImg, CV_INTER_AREA);
  cvSaveImage(buf, newImg);
  char outFile[200];
  strcpy(outFile, filename.c_str());
  ext = strrchr(outFile, '.');
  strcpy(ext, "pgm.pgm");
  char cmd[500];
  sprintf(cmd, "pnmdepth 255 \"%s\" >\"%s\"", buf, outFile);
  cout << cmd << endl;
  system(cmd);
  char keyFile[200];
  strcpy(keyFile, filename.c_str());
  ext = strrchr(keyFile, '.') + 1;
  strcpy(ext, "key");
  sprintf(cmd, "./sift <\"%s\" >\"%s\"", outFile, keyFile);
  system(cmd);
  sprintf(cmd, "rm \"%s\" \"%s\"", buf, outFile);
  system(cmd);
}
