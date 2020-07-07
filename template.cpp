/*
	First name	XXX
	Last name		XXX
	Student ID	XXXXXX (e.g. 01234567)
	Username		XXX (e.g. aaa123)	
*/

//IMPORTANT: before submitting your source code, please make sure to comment your main function (needed for automated testing)


#include <iostream>
#include <string>
#include <map>
#include <vector> //or #include <list>

// do not use using namespace std

// do not alter the struct declaration
struct hufftreenode {
	char character;
	int frequency;
	hufftreenode* left;
	hufftreenode* right;
};

// do not alter the typedef 
typedef hufftreenode* hufftreeptr;


// you can define and use any additional *structures* if you might need to

// YOUR CODE HERE FOR ADDIIONAL STRUCTS DECLARATION, IF ANY



/////////////////////////////////////////
// Functions declarations (prototypes) //
/////////////////////////////////////////

// do not alter these functions declarations

//utility function
bool valid_hufftree(const hufftreeptr hufftree);
 // returns true if the input hufftree is a valid Huffman tree
 // i.e. all the terminal nodes (leaves) have characters,
 // all the non-leaf nodes have two sub-trees each,
 // and the occurence frequency of a non-leaf node equals
 // the sum of the occurence frequencies of its two sub-trees.



std::string huffencode(const std::string& message, std::map<char, int>& freqtable, hufftreeptr& hufftree, std::map<char, std::string>& hufftable);
// encodes an input message (a string of characters) into an encoded message (string of bits) using the Huffman coding
// builds the Huffman tree and its associated encoding table (i.e. the association between the characters in the tree leaves and their encodings (string of bits))

std::string huffdecode(const std::string& encodedmessage, const hufftreeptr& hufftree);
// decodes an encoded message (a string of bits) into the original message (a string of characters) using the Huffman tree built during the encoding



// you can define and use any additional *functions* if you might need to

// YOUR CODE HERE FOR ADDIIONAL FUNCTIONS DECLARATION (NOT THE IMPLEMENTATION), IF ANY




///////////////////
// main function //
///////////////////

//IMPORTANT (Reminder): before submitting your source code, please make sure to comment your main function (needed for automated testing)
int main() {

	// YOUR CODE HERE FOR TESTING YOUR PROGRAM

	// an example of basic encoding/decoding
	std::string message, encodedmessage, decodedmessage;
	hufftreeptr hufftree;
	std::map<char, int> freqtable;
	std::map<char, std::string> hufftable;
	
	message = "go go gophers";
	
	// 1) encoding (compression)
	encodedmessage = huffencode(message, freqtable, hufftree, hufftable);
	// freqtable should be as in Table 1.
	// hufftree might be as in Figure 2 (remember, the Huffman coding tree IS NOT UNIQUE. If we swap two siblings in a Huffman tree, it is still a Huffman tree of the same message.)
	// hufftable should correspond to hufftree, and might be as in Table 2 <' ',"100">, <'g',"00">, <'o',"01">, <'p',"1110">, <'h',"1101">, <'e',"101">, <'r',"1111">, <'s',"1100">.
	// encodedmessage might then be the 37-bits string "0001100000110000011110110110111111100" (i.e. "00.01.100.00.01.100.00.01.1110.1101.101.1111.1100")

	if(valid_hufftree(hufftree))  {
		std::cout << "valid Huffman tree." << std::endl;
	}
	else {
		std::cout << "not valid Huffman tree." << std::endl;
	}

	// 2) decoding (uncompression)
	decodedmessage = huffdecode(encodedmessage, hufftree);
	// should be "go go gophers"
	
	if(decodedmessage == message) {
		std::cout << "decoding OK." << std::endl;
	}
	else {
		std::cout << "decoding not OK." << std::endl;
	}

	return 0;
}





/////////////////////////////////////////////
// Functions definitions (implementations) //
/////////////////////////////////////////////

// YOUR CODE HERE FOR THE FUNCTIONS IMPLEMENTATIONS
