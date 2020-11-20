#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <unordered_map>

#include "trie.h"

using namespace std;

int main(int argc, char **argv)
{
	trie t;
	ofstream tofile;
	
	if (argc > 1) {
		tofile.open(argv[1], ios::out);
		if (tofile.is_open()) {
			string word;
			// Loop through all of the lines from stdin and add them to the trie if
			// they weren't already in the trie, then we also save the to the file.
			while (getline(cin, word)) {
				if (t.insert(word)) {
					tofile << word << endl;
				}
			}
		} else {
			cerr << "error: file could not be opened" << endl;
		}
		tofile.close();
	} else {
		cerr << "error: expected output file name" << endl;
	}
}


