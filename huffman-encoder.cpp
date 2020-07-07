#include <iostream>
#include <string>
#include <map>
#include <vector> 
#include <stack>
#include <fstream>


struct hufftreenode {
	char character;
	int frequency;
	hufftreenode* left;
	hufftreenode* right;
};

typedef hufftreenode* hufftreeptr;

/////////////////////////////////////////
// Functions declarations (prototypes) //
/////////////////////////////////////////


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

void frequency(std::map<char, int>& f_table, std::string message);
// builds a frequency table of characters in the input string

void leaf_check(const hufftreeptr hufftree, int& count, int& valid, std::vector<hufftreeptr>& leaves);
// checks if all the terminal nodes (leaves) have characters associated with them

void subtree_check(const hufftreeptr hufftree, int& count, int& valid, std::vector<hufftreeptr>& leaves, std::vector<hufftreeptr>& non_leaf);
// checks if all the non-leaf nodes have two sub-trees each

bool member(std::vector<hufftreeptr>& leaves, hufftreeptr node);
// checks if a given hufftree node is present in a vector of nodes

void generator(std::map<char, int>& f_table, std::vector<hufftreeptr>& forest);
// generates a forest of nodes from the frequency table

void order_place(hufftreeptr to_push, std::vector<hufftreeptr>& forest);
// ensures that nodes are added to the forest in the correct order to make building the Huffman Tree simpler 

std::string find_path(hufftreeptr node, const char c, bool& found, const std::string& path);
// returns the Huffman Code for a given character by finding the path to it in the Huffman Tree

void hufftable_to_CSV(std::map<char, std::string>& hufftable);

///////////////////
// main function //
///////////////////

int main(int argc, char* argv[]) {

    std::string message, encodedmessage, decodedmessage;
	hufftreeptr hufftree;
	std::map<char, int> freqtable;
	std::map<char, std::string> hufftable;
    bool files;
    std::string option;
    std::cerr<< "1" <<std::endl;
    
    if(argc == 2){
        option= argv[1];
    }

    if(argc == 4){
        option = argv[2];
    }

   
    
    if(option == "-o"){
        files = true;
        std::string input_filename = argv[1];
        try{
            std::ifstream infile(input_filename);
            if(!infile.is_open()){
                throw("File: " + input_filename + " not found");
            }

            message.assign((std::istreambuf_iterator<char>(infile)), (std::istreambuf_iterator<char>()));
        }
        catch(std::string e){
            std::cerr << e;
        }
        
        
    }
    else if(option == "--example"){
        message = "go go gophers";
    }

    else if(option == "--user-input"){
        std::cout << "Please enter a message: " << std::endl;
        std::cin >> message;
    }
    else{
        message = "default";
    }
   
    //message = "go go gophers";
    std::cerr<< "2" <<std::endl;
	std::cout << "Message to Code: " << std::endl << message << std::endl;
	// 1) encoding (compression)
    std::cerr<< "3" <<std::endl;
	encodedmessage = huffencode(message, freqtable, hufftree, hufftable);
    std::cerr<< "4" <<std::endl;
	if (valid_hufftree(hufftree)) {
		std::cout << "Valid Huffman tree generated." << std::endl;
	}
	else {
		std::cout << "Valid Huffman tree could not be generated" << std::endl;
	}

    hufftable_to_CSV(hufftable);
    std::cout << "Huffman Table generated. Can be found at huffman-table.csv" << std::endl;
    std::string output_filename;
    if(files){
        output_filename = argv[3];
    }
    else{
        output_filename = "encoded-message.txt"
    }
    std::ofstream outfile(output_filename);
    outfile << encodedmessage;
    std::cout <<  "Huffman Code generated. Can be found at " << output_filename << std::endl;


	// 2) decoding (uncompression)
	decodedmessage = huffdecode(encodedmessage, hufftree);

	std::cout << "Decoded message to verify: " << std::endl << decodedmessage << std::endl;
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
	for (char c = NULL; c <= '~'; c++)
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

void hufftable_to_CSV(std::map<char, std::string>& hufftable)
{
    std::ofstream csv;
    csv.open("huffman-table.csv");
    csv << "Character,Huffman Code (Tree Path)\n";

    std::map<char, std::string>::iterator it;
    for(it = hufftable.begin(); it != hufftable.end(); it++)
    {
        csv << it->first << "," << it->second << "\n";
    }

    csv.close();
}

