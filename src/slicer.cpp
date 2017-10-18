#include "slicer.h"
#include <sys/stat.h>
#include "util.h"
#include <memory.h>

Slicer::Slicer(string infile, string outfolder, int sliceLines, string extension, int sliceNoDigits, int gzipSetting, bool keepOriginalFilename) {
    mInFile = infile;
    mOutFolder = outfolder;
    mExtension = extension;
    mSliceLines = sliceLines;
    mSliceNoDigits = sliceNoDigits;
    mKeepOriginalFilename = keepOriginalFilename;
    mTotalLine = 0;
    mTotalSlice = 0;

    // gzip handlling
    if(ends_with(infile, ".gz"))
        mInZipped = true;
    else
        mInZipped = false;
    if(gzipSetting == GZIP_TRUE)
        mOutZipped = true;
    else if(gzipSetting == GZIP_FALSE)
        mOutZipped = false;
    else
        mOutZipped = mInZipped;
}

Slicer::~Slicer() {
    close();
}

void Slicer::run(){
    init();
    int sliceNumber = 1;
    int line = 0;
    // the maximum bytes in a line
    const int MAX_LINE = 10000;
    char* lineData = new char[MAX_LINE];
    memset(lineData, 0, MAX_LINE);
    while(true) {
        if(!getLine(lineData, MAX_LINE))
            break;
        if(line == 0) {
            prepareOutput(sliceNumber);
            mTotalSlice = sliceNumber;
        }
        mTotalLine++;
        line++;
        write(lineData);

        // this slice is full, start next one
        if(line == mSliceLines) {
            closeOutput();
            sliceNumber++;
            line = 0;
        }
    }


    closeOutput();

    delete lineData;
    lineData = NULL;
}

void Slicer::prepareOutput(int sliceNumber){
    string num = int2str(sliceNumber);
    // padding
    while(num.size() < mSliceNoDigits)
        num = "0" + num;

    string outFile = joinpath(mOutFolder, num);
    if(mKeepOriginalFilename) {
        string bname = basename(mInFile);
        outFile = outFile + "." + bname;
    } else if (mExtension !="") {
        outFile = outFile + "." + mExtension;
    }

    if(mOutZipped) {
        if(!ends_with(outFile, ".gz"))
            outFile = outFile + ".gz";
        mOutZip = gzopen(outFile.c_str(), "w");
    } else {
        if(ends_with(outFile, ".gz"))
            outFile = outFile.substr(0, outFile.size() - 3);
        mOutStream.open(outFile.c_str(), ifstream::out);
    }
}

void Slicer::closeOutput() {
    if (mOutZipped){
        if (mOutZip){
            gzflush(mOutZip, Z_FINISH);
            gzclose(mOutZip);
            mOutZip = NULL;
        }
    }
    else {
        if (mOutStream.is_open()){
            mOutStream.flush();
            mOutStream.close();
        }
    }
}

void Slicer::write(char* data) {
    int size = strlen(data);
    if(mOutZipped){
        gzwrite(mOutZip, data, size);
        gzputc(mOutZip, '\n');
    }
    else{
        //cout << mOutStream.good() << ":"<<size<<endl;
        mOutStream.write(data, size);
        mOutStream.put('\n');
    }
}

void Slicer::init(){
    if (mInZipped){
        mInZip = gzopen(mInFile.c_str(), "r");
    }
    else {
        mInStream.open(mInFile.c_str(), ifstream::in);
    }
}

bool Slicer::getLine(char* line, int maxLine){
    bool status = true;
    if(mInZipped)
        status = gzgets(mInZip, line, maxLine);
    else {
        mInStream.getline(line, maxLine);
        status = !mInStream.fail();
    }

    return status;
}

void Slicer::close(){
    if (mInZipped){
        if (mInZip){
            gzclose(mInZip);
            mInZip = NULL;
        }
    }
    else {
        if (mInStream.is_open()){
            mInStream.close();
        }
    }
}