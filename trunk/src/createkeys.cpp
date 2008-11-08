/*
 * @fileoverview Uses SIFT to convert a JPG picture
 * into a SIFT key file.
 *
 * TODO(Mark): This should all be done in a shell script.
 *
 * Usage: ./createkeyfile file convertedFile
 * where file is created by doing a ls -R
 * of the files you want to convert to key files and
 * convertedFile is the file you want to contain the full paths
 * of all the keys that were successfully created.
 *
 * @author Mark Knichel
 */

#include <iostream>
#include <fstream>
#include <math.h>
#include <cv.h>
#include <cvaux.h>
#include <highgui.h>  
using namespace std;

void createKeyFile(string filename, int siftProgram);
string removeFileExtension(string filename);

int main(int argc, char** argv) {
  int siftProgram = 0; 
  if (argc != 3 && argc != 4) {
    cout << "Usage is: ./createkeys inputFile outputFile siftProgram" << endl;
    return 0;
  }
  if (argc == 4) {
    siftProgram = atoi(argv[3]);
  }
  ifstream file1(argv[1]);
  ofstream file2(argv[2], ios_base::out | ios_base::trunc);
  string dir = "";
  char buf[500];
  while(file1.getline(buf, 500)) {
    string f(buf);
    string fn = dir + f;
    if(f.length() > 1 && f.at(f.length() - 1) == ':') {
      dir = f;
      dir.at(f.length() - 1) = '/';
      continue;
    }
    if(fn.find(".JPG") != string::npos) {
      cout << fn << endl;
      string keyFilename = removeFileExtension(fn) + ".key";
      file2 << keyFilename << endl;
      createKeyFile(fn, siftProgram);
    }
  }
  file2.close();
  return 0;
}


/**
 * Removes the .xxx extension from the end of the filename.
 * Assumes that there is always a .xxx extension at the end of the filename.
 *
 * @param filename The file you want to remove an extension from.
 * @return The new filename without the extension.
 */
string removeFileExtension(string filename) {
  return filename.substr(0, filename.find_last_of("."));
}


/**
 * Creates a key file from the given image using the correct SIFT program.
 * This has the side effect that it saves the key file directly to the folder
 * where the image is found.  It removes the temporary images created to
 * create the key file.
 *
 * @param filename The name of the file to create a key descriptor file for.
 * @param siftProgram Which SIFT program this function should use to create
 *     the key file.
 */
void createKeyFile(string filename, int siftProgram) {
  string baseFilename = removeFileExtension(filename);
  string originalPgmFilename = baseFilename + ".pgm";
  string newPgmFilename = baseFilename + "pgm.pgm";
  string keyFilename = baseFilename + ".key";
  string cmd("");

  IplImage* img;
  img = cvLoadImage(filename.c_str(), 0);
  int scale = (int)ceil((double)img->width / 600);
  int width = img->width / scale;
  int height = img->height / scale;
  IplImage* newImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
  cvResize(img, newImg, CV_INTER_AREA);
  cvSaveImage(originalPgmFilename.c_str(), newImg);

  cmd = "pnmdepth 255 \"" + originalPgmFilename + "\" >\"" + newPgmFilename +
      "\"";
  system(cmd.c_str());

  switch (siftProgram) {
  case 0:
    cmd = "./bin/lowesift <\"" + newPgmFilename + "\" >\"" + keyFilename + "\"";
    break;
  case 1:
    cmd = "./bin/siftpp \"" + newPgmFilename + "\" --output=\"" + keyFilename +
        "\"";
    break;
  case 2:
    cmd = "./bin/vlfeatsift --peak-thresh=1 \"" + newPgmFilename +
        "\" --output=\"" + keyFilename + "\"";
  case 3:
    cmd = "./bin/hesssift -x \"" + newPgmFilename +
        "\" -o \"" + keyFilename + "\"";
  }
  system(cmd.c_str());
  cmd = "rm \"" + originalPgmFilename + "\" \"" + newPgmFilename + "\"";
  system(cmd.c_str());
}
