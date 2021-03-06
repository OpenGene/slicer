#include <stdio.h>
#include <time.h>
#include "cmdline.h"
#include "slicer.h"
#include "util.h"
#include <sstream>

#define VERSION "0.3.0"

int main(int argc, char* argv[]){
    cmdline::parser cmd;
    cmd.add<string>("input", 'i', "input file name", true, "");
    cmd.add<string>("outdir", 'o', "the output folder, default is currently working directory", false, ".");
    cmd.add<int>("line", 'l', "how many lines per slice", true, 0);
    cmd.add<int>("digits", 'd', "the digits for the slice number padding (1~10), default is 4, so the filename will be padded as 0001.xxx, 0 to disable padding", false, 4);
    cmd.add("gzip", 'z', "force gzip output, default the gzip setting is following the input");
    cmd.add("nogzip", 'n', "don't use gzip output, default the gzip setting is following the input");
    cmd.add<int>("compression", 'c', "the gzip compression level (0 ~ 9), 0 for best speed, 9 for best compression ratio, default is 2", false, 2);
    cmd.add("simple_name", 's', "use the simple file name like 0001, and discard the original file name");
    cmd.add<string>("ext", 'e', "set the file extension to be added to the output if using simple_name. This option only works when --simple_name enabled", false, "");
    cmd.parse_check(argc, argv);

    string infile = cmd.get<string>("input");
    string outfolder = cmd.get<string>("outdir");
    string extension = cmd.get<string>("ext");

    int sliceLines = cmd.get<int>("line");
    int sliceDigits = cmd.get<int>("digits");
    if(sliceDigits > 10)
        sliceDigits = 10;

    int compression = cmd.get<int>("compression");
    compression = max(min(9, compression), 0);

    bool gzip = cmd.exist("gzip");
    bool nogzip = cmd.exist("nogzip");

    if(gzip && nogzip) {
        cout << "ERROR: you cannot set the output to be both gzip and nogzip, choose either to run again." << endl;
        exit(-1);
    }

    int gzipSetting = GZIP_AS_INPUT;
    if(gzip)
        gzipSetting = GZIP_TRUE;
    else if(nogzip)
        gzipSetting = GZIP_FALSE;

    bool keepOriginalFilename = !cmd.exist("simple_name");

    check_file_valid(infile);

    if(file_exists(outfolder) && !is_directory(outfolder)) {
        cout << "ERROR: " << outfolder << " exists but is not a folder." << endl ;
        exit(-1);
    }

    // create the output folder if it doesn't exist
    if(!file_exists(outfolder)) {
        mkdir(outfolder.c_str(), 0777);
        if(!file_exists(outfolder)) {
            cout << "ERROR: " << outfolder << " doesn't exist and cannot be created." << endl ;
            exit(-1);
        }
    }

    time_t t1 = time(NULL);
    Slicer slicer(infile, outfolder, sliceLines, extension, sliceDigits, gzipSetting, keepOriginalFilename, compression);
    slicer.run();
    time_t t2 = time(NULL);
    cout << slicer.getTotalLines() << " lines cut to " << slicer.getTotalSlices() << " slices in " << t2 -t1 << " seconds, by slicer v" << VERSION << endl;


    return 0;
}