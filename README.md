# Slicer
Slice a text file to smaller files by lines. This tool also supports gzip compression for input/output.

# Usage
```shell
# simplest
slicer -i <input_file_name> -l <how_many_lines_per_slice>

# specify a folder to store the slicedfiles
slicer -i <input_file_name> -l <how_many_lines_per_slice> -o <output_dir>

# force gzip
slicer -i <input_file_name> -l <how_many_lines_per_slice> -o <output_dir> --gzip
```

# Download
Get latest
```shell
# download by http
https://github.com/OpenGene/slicer/archive/master.zip

# or clone by git
git clone https://github.com/OpenGene/slicer.git
```
Get the stable releases  
https://github.com/OpenGene/slicer/releases/latest

# Build
slicer only depends on `libz`, which is always available on Linux or Mac systems. If your system has no `libz`, install it first.
```shell
cd slicer
make
```

# Install
After build is done, run
```
sudo make install
```

# Full options
```
usage: slicer --input=string --line=int [options] ... 
options:
  -i, --input          input file name (string)
  -o, --outdir         the output folder, default is currently working directory (string [=.])
  -l, --line           how many lines per slice (int)
  -d, --digits         the digits for the slice number padding, default is 4, so the filename will be padded as 0001.xxx, -1 to disable padding (int [=4])
  -e, --ext            the file extension to be added to the output if using simple_name) (string [=])
  -z, --gzip           force gzip output, default the gzip setting is following the input
  -n, --nogzip         don't use gzip output, default the gzip setting is following the input
  -s, --simple_name    use the simple file name like 0001, and discard the original file name
  -?, --help           print this message
```
