#ifndef SLICER_H
#define SLICER_H

#include <stdio.h>
#include <stdlib.h>
#include "zlib/zlib.h"
#include <iostream>
#include <fstream>

#define GZIP_TRUE 1
#define GZIP_FALSE 0
#define GZIP_AS_INPUT -1

using namespace std;

class Slicer{
public:
    Slicer(string infile, string outfolder, int sliceLines, string extension = "", int sliceNoDigits = 4, int gzipSetting = GZIP_AS_INPUT, bool keepOriginalFilename = true);
    ~Slicer();
    void run();
    int getTotalLines() { return mTotalLine; }
    int getTotalSlices() { return mTotalSlice; }

private:
    void init();
    void close();
    bool getLine(char* line, int maxLine);
    void write(char* data);
    void prepareOutput(int sliceNumber);
    void closeOutput();

private:
    string mInFile;
    string mOutFolder;
    string mExtension;
    int mSliceLines;
    int mSliceNoDigits;
    ifstream mInStream;
    gzFile mInZip;
    ofstream mOutStream;
    gzFile mOutZip;
    bool mInZipped;
    bool mOutZipped;
    bool mKeepOriginalFilename;
    int mTotalLine;
    int mTotalSlice;
};

#endif