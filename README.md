# Slicer
Slice a text file to smaller files by lines, with gzip compression for input/output supported. This tool can be used to slice big `FASTQ` files to smaller ones for parallel processing.

# Usage
```shell
# simplest
slicer -i <input_file_name> -l <how_many_lines_per_slice>

# specify a folder to store the sliced files
slicer -i <input_file_name> -l <how_many_lines_per_slice> -o <output_dir>

# force gzip
slicer -i <input_file_name> -l <how_many_lines_per_slice> -o <output_dir> --gzip
```

# Example
Assuming that you have a text file called `filename.for.test.data` with 400000 lines, you want to cut it to 4 slices (100000 lines for each). You'd like to gzip all the slices, keep the file extension `.data`, and store them in a folder `sliced`. You can use following command:
```shell
slicer -i filename.for.test.data -l 100000 -o sliced -e data -z -s
```
Then you will get four files in the folder `sliced`:
```
├── filename.for.test.data
└── sliced
    ├── 0001.data.gz
    ├── 0002.data.gz
    ├── 0003.data.gz
    └── 0004.data.gz
```

# Get slicer
## Download
Get latest
```shell
# download by http
https://github.com/OpenGene/slicer/archive/master.zip

# or clone by git
git clone https://github.com/OpenGene/slicer.git
```
Get the stable releases  
https://github.com/OpenGene/slicer/releases/latest

## Build
slicer only depends on `libz`, which is always available on Linux or Mac systems. If your system has no `libz`, install it first.
```shell
cd slicer
make
```

## Install
After build is done, run
```
sudo make install
```

# Full options
```
usage: ./slicer --input=string --line=int [options] ... 
options:
  -i, --input          input file name (string)
  -o, --outdir         the output folder, default is currently working directory (string [=.])
  -l, --line           how many lines per slice (int)
  -d, --digits         the digits for the slice number padding (1~10), default is 4, so the filename will be padded as 0001.xxx, 0 to disable padding (int [=4])
  -z, --gzip           force gzip output, default the gzip setting is following the input
  -n, --nogzip         don't use gzip output, default the gzip setting is following the input
  -c, --compression    the gzip compression level (0 ~ 9), 0 for best speed, 9 for best compression ratio, default is 2 (int [=2])
  -s, --simple_name    use the simple file name like 0001, and discard the original file name
  -e, --ext            set the file extension to be added to the output if using simple_name. This option only works when --simple_name enabled (string [=])
  -?, --help           print this message
```

# Work with FASTQ
* Make sure you set the line number (-l xxxx, or --line=xxxx) correctly as a multiple of 4, since each record always has 4 lines.
* If you want to keep the `.fq` or `.fastq` file extension, you can set the extension by `--ext=fq` or `--ext=fastq`
* If your data are paired-end sequencing files, you can run this tool for the pair of files separately.
* If your data are paired-end sequencing files, and you enable the `simple_name` to use short file name. For read1, you can set the extension as R1.fq by `--ext=R1.fq`, and for read2 you can set R2.fq by `--ext=R2.fq`, then you will get the sliced files like `0001.R1.fq`, `0002.R2.fq`.
