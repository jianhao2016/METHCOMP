# METHCOMP

## Requirements
  * For compiling: C++ 11
  * At least 2GB of RAM and 50GB of storage
  * [optional] multi-core CPU for parallel decoding.

## Data Description
The bedMethyl files are "revised" bed files. A typical bedMethyl file has 9 + 2 columns, with the first 9 columns matching those of the bed file and 2 additional columns containing coverage and percentage information

|          | column 1 | column 2 | column 3 | column 4 | column 5 | column 6 | column 7 | column 8 | column 9 | column 10 | column 11 |
| -------- |:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:---------:|:---------:|
|definition|chrom     |chromStart|chromEnd  |name      |score     |strand    |thickStart|thickEnd  |itemRGB   |coverage   |percentage |
|datatype  |string    |int32_t   |int32_t   |string    |int32_t   |string    |int32_t   |int32_t   |string    |int32_t    |uint8_t    |

Sample files can be download from the ENCODE database, for example the [WGBS experiment of forebrain embryo](https://www.encodeproject.org/files/ENCFF369TZO/ "File summary for ENCFF369TZO (bed)"). Here, we provide a small test file in the folder **data**

## How to Use the Software
### Install
```
# download METHCOMP
git clone https://github.com/jianhao2016/METHCOMP

# compile
make
```
### Run
To run METHCOMP on the test file, use the following command:
<!-- First you need to compile the project and get the executable file `output`. After that in, Mac/Linux, open the terminal and type -->

`./output full ../data/test .`

This command will compress the test file into **"compressed_test-0"** and then decompress it to **"reconstructed_test-0"**. The compressed files and decompressed files are stored in the folder **DerivedData/**

To run the compression/decompression algorithm on an arbitrary methylation data file, use the following command:  

`./output <mode> <path to file> <output folder path> [optional <number of line per block> <line to be decoded>]`  

Or use the scripts for block-mode compression:

`source auto_compress.sh <path to file> <output path> [optional <lines in each block>]`

And block-mode decompression:

`source auto_decompress.sh <file name> <file directory> <output path> [optional <lines in each block>]`

`mode` can be chosen from the following menu: **full** (joint compression and decompression), **compress** and **decompress**. When `<output folder path>` is omitted, the default output folder is **DerivedData/**.


Two optional arguments, `<number of line per block>` and `<line to be decoded>`, are used for random access. To
compress the original file in blocks, enter the number of lines in each block you would like to. To decoded a specific
line, enter the line number to decode the block it belongs to.

The `auto_script` is for parallel decoding.

## Output Files
The compressed files and decompressed files are named **"compressed_\<file name\>"** and **"reconstructed_\<file name\>"**
