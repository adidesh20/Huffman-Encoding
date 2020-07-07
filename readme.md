<b>Huffman Encoding with C++ Binary Trees</b>
=============================================

This program was created for the coursework section of the first year module <b>Algorithms and Data Structures</b>, which is a part of Electronic and Information Engineering at Imperial College London. To ensure standardisation and ease of testing, it was compulsory to use this *[template](template.cpp)* which was provided by the lecturer. The initial submission which complies with the template's rules can be found at *[huff_submit.cpp](huff_submit.cpp)*. I have added additional functionality outside of the range of the initial specification to create a final program: *[huffman-encoder.cpp](huffman-encoder.cpp)*.

Overview
----------

<b>A full overview of the program and an explanation of how it works can be found at *[Code-Explanation-and-Analysis.pdf](Code-Explanation-and-Analysis.pdf)*.</b>

How to run the program
=======================

Prequisites
------------

- Make
- GCC/G++ (or another compiler of your choice)

Install
--------
<b>Clone the repository</b> by either downloading the zip file using the Git GUI or using the command line:
    
    git clone https://github.com/adidesh20/Huffman-Encoding.git

Then, from the top of the directory tree, compile the program:

    g++ -std=c++11 huffman-encoder.cpp -o huff

Compressing a text file
----------------------------------------------
Assuming the message to be compressed is stored in the text file `input.txt`, running command:

    ./huff input.txt -o output.txt

Will result in the creation of file `output.txt` which will contain the encoded message, and a file `huffman-table.csv` which contains the Huffman Table.

Compressing User Input
----------------------
Run the program with the user input flag:

    ./huff --user-input

You will then be prompted to enter a string to compress. `encoded-message.txt` will store the encoded message, while `huffman-table.csv` will store the Huffman Table.

To run the example given in the orignal spec, run the program with the example flag:

    ./huff --example

This compresses the string *"go go gophers"*.
