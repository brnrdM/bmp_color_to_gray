Converts compatible BMP image files from color to grayscale using the NTCS Luminosity grayscale algorithm.

Usage: build/bmp_clr_to_gs [OPTIONS]
 -i         Input File Path
 -o         Output File Path
 -h         Help
 
How to compile:
 1) Need build/ directory
 2) run "make"

How to compile with debug flags:
 1) Open makefile in your favorite text editor, add " -g" to CFLAGS
 2) run "make"

How to test output:
 1) Look at the output BMP file
 2) Convert it to hex with the command:
    cat output.bmp | xxd > output.hex

*Currently only supports uncompressed 24 bits per pixel bmp files.
