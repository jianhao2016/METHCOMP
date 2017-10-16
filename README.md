# METHCOMP

## Requirements
  * For compiling: C++ 11
  * At least 2GB of RAM and 50GB of storage

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

This command will compress the test file into **"compressed_test"** and then decompress it to **"reconstructed_test"**. The compressed files and decompressed files are stored in the folder **DerivedData/**

To run the compression/decompression algorithm on an arbitrary methylation data file, use the following command:  

`./output <mode> <path to file> <output folder path>`  

`mode` can be chosen from the following menu: **full** (joint compression and decompression), **compression** and **decompression**. When `<output folder path>` is omitted, the default output folder is **DerivedData/**

## Output Files
The compressed files and decompressed files are named **"compressed_\<file name\>"** and **"reconstructed_\<file name\>"**
