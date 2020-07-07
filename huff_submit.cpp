/*
	First name	ADITYA
	Last name	DESHPANDE
	Student ID
	Username	ad5718
*/
//USING C++11

//IMPORTANT: before submitting your source code, please make sure to comment your main function (needed for automated testing)

#include <iostream>
#include <string>
#include <map>
#include <vector> //or #include <list>
#include <stack>

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
void frequency(std::map<char, int>& f_table, std::string message);
void leaf_check(const hufftreeptr hufftree, int& count, int& valid, std::vector<hufftreeptr>& leaves);
void subtree_check(const hufftreeptr hufftree, int& count, int& valid, std::vector<hufftreeptr>& leaves, std::vector<hufftreeptr>& non_leaf);
bool member(std::vector<hufftreeptr>& leaves, hufftreeptr node);
void generator(std::map<char, int>& f_table, std::vector<hufftreeptr>& forest);
void order_place(hufftreeptr to_push, std::vector<hufftreeptr>& forest);
std::string find_path(hufftreeptr node, const char c, bool& found, const std::string& path);


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

	message = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
	std::cout << "Message: " << message << std::endl;
	// 1) encoding (compression)
	encodedmessage = huffencode(message, freqtable, hufftree, hufftable);
	// freqtable should be as in Table 1.
	// hufftree might be as in Figure 2 (remember, the Huffman coding tree IS NOT UNIQUE. If we swap two siblings in a Huffman tree, it is still a Huffman tree of the same message.)
	// hufftable should correspond to hufftree, and might be as in Table 2 <' ',"100">, <'g',"00">, <'o',"01">, <'p',"1110">, <'h',"1101">, <'e',"101">, <'r',"1111">, <'s',"1100">.
	// encodedmessage might then be the 37-bits string "0001100000110000011110110110111111100" (i.e. "00.01.100.00.01.100.00.01.1110.1101.101.1111.1100")

	if (valid_hufftree(hufftree)) {
		std::cout << "valid Huffman tree." << std::endl;
	}
	else {
		std::cout << "not valid Huffman tree." << std::endl;
	}

	// 2) decoding (uncompression)
	decodedmessage = huffdecode(encodedmessage, hufftree);
	// should be "go go gophers"

	std::cout << "Decoded: " << decodedmessage << std::endl;
	if (decodedmessage == message) {
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

bool valid_hufftree(const hufftreeptr hufftree)
{
	int count, valid = 0;
	std::vector<hufftreeptr> leaves;
	leaf_check(hufftree, count, valid, leaves);
	if (leaves.size() != valid)
	{
		return false;
	}
	count = valid = 0;
	std::vector<hufftreeptr> non_leaf;
	subtree_check(hufftree, count, valid, leaves, non_leaf);
	if (count != valid)
	{
		return false;
	}

	for (int i = 0; i < non_leaf.size(); i++)
	{
		hufftreeptr current = non_leaf[i];
		hufftreeptr left = current->left;
		hufftreeptr right = current->right;
		if (current->frequency != (left->frequency + right->frequency))
		{
			return false;
		}
	}
}

std::string huffencode(const std::string& message, std::map<char, int>& freqtable, hufftreeptr& hufftree, std::map<char, std::string>& hufftable)
{
	frequency(freqtable, message);
	std::vector<hufftreeptr> forest;
	generator(freqtable, forest);

	while (forest.size() > 1)
	{
		hufftreeptr low = forest[0];
		hufftreeptr sec_low = forest[1];
		hufftreeptr parent = new hufftreenode;

		parent->character = NULL;
		parent->frequency = low->frequency + sec_low->frequency;
		parent->left = low;
		parent->right = sec_low;
		forest.erase(forest.begin());
		forest.erase(forest.begin());
		order_place(parent, forest);
	}

	hufftree = forest[0];
	hufftreeptr node = hufftree;
	for (char c = ' '; c <= '~'; c++)
	{
		if (freqtable.find(c) != freqtable.end())
		{
			std::string path;
			bool found = false;
			path = find_path(node, c, found, path);
			hufftable.insert(std::pair<char, std::string>(c, path));
		}
	}

	std::string compressed;
	for (int i = 0; i < message.size(); i++)
	{
		char key = message[i];
		std::string path = hufftable[key];
		compressed.append(path);
	}

	return compressed;
}

std::string huffdecode(const std::string& encodedmessage, const hufftreeptr& hufftree)
{
	std::string enc = encodedmessage;
	std::string decoded;
	hufftreeptr node = hufftree;

	if (!enc.empty())
	{
		while (enc.size() > 0)
		{
			char current = enc.front();
			enc.erase(enc.begin());
			if (current == '0')
			{
				node = node->left;
			}
			if (current == '1')
			{
				node = node->right;
			}

			if (node->left == NULL && node->right == NULL)
			{
				decoded.push_back(node->character);
				node = hufftree;
			}
		}
	}
	else
	{
		for (int i = 0; i < node->frequency; i++)
		{
			decoded.push_back(node->character);
		}
	}
	return decoded;
}

void frequency(std::map<char, int>& f_table, std::string message)
{
	for (char c = ' '; c <= '~'; c++)
	{
		int count = 0;
		for (int i = 0; i < message.size(); i++)
		{
			if (c == message[i])
			{
				count++;
			}
		}
		if (count > 0)
		{
			f_table.insert(std::pair<char, int>(c, count));
		}
	}
}

void leaf_check(const hufftreeptr hufftree, int& count, int& valid, std::vector<hufftreeptr>& leaves)
{
	if (hufftree != NULL)
	{
		leaf_check(hufftree->left, count, valid, leaves);
		if (hufftree->left == NULL && hufftree->right == NULL)
		{
			count++;
			leaves.push_back(hufftree);
			if (hufftree->character != NULL)
			{
				valid++;
			}
		}
		leaf_check(hufftree->right, count, valid, leaves);
	}
}

void subtree_check(const hufftreeptr hufftree, int& count, int& valid, std::vector<hufftreeptr>& leaves, std::vector<hufftreeptr>& non_leaf)
{
	if (hufftree != NULL)
	{
		subtree_check(hufftree->left, count, valid, leaves, non_leaf);
		if (!member(leaves, hufftree))
		{
			count++;
			non_leaf.push_back(hufftree);
			if (hufftree->left != NULL && hufftree->right != NULL)
			{
				valid++;
			}
		}
	}
}

bool member(std::vector<hufftreeptr>& leaves, hufftreeptr node)
{
	for (int i = 0; i < leaves.size(); i++)
	{
		if (leaves[i] == node)
		{
			return true;
		}
	}
	return false;
}

void generator(std::map<char, int>& f_table, std::vector<hufftreeptr>& forest)
{
	for (char c = ' '; c <= '~'; c++)
	{

		if (f_table.find(c) != f_table.end())
		{

			hufftreeptr to_push = new hufftreenode;
			to_push->character = c;
			to_push->left = NULL;
			to_push->right = NULL;
			to_push->frequency = f_table[c];

			order_place(to_push, forest);
		}
	}
}

void order_place(hufftreeptr to_push, std::vector<hufftreeptr>& forest)
{
	if (forest.empty())
	{
		forest.push_back(to_push);
	}
	else
	{
		bool broken = false;
		int location;
		int i = 0;
		while (i < forest.size() && !broken)
		{
			hufftreeptr current = forest[i];
			if (to_push->frequency < current->frequency)
			{
				location = i;
				broken = true;
			}
			i++;
		}


		if (broken)
		{
			forest.emplace(forest.begin() + location, to_push);
		}
		else
		{
			forest.push_back(to_push);
		}
	}
}

std::string find_path(hufftreeptr node, const char c, bool& found, const std::string& path)
{
	if (found)
	{
		return path;
	}

	std::string result = path;

	if (node->left != NULL)
	{
		result = find_path(node->left, c, found, path + "0");
	}
	if (node->right != NULL && !found)
	{
		result = find_path(node->right, c, found, path + "1");
	}
	if (node->character == c)
	{
		found = true;
	}
	return result;
}

