#include <iostream>
#include <fstream>
#include <string>
#include <string.h>

using namespace std;

struct trie
{
	trie(string value = "") {
		this->value = value;
		for (int i = 0; i < 26; i++) {
			next[i] = nullptr;
		}
		isEnd = true;
	}

	~trie() {
		for (int i = 0; i < 26; i++) {
			if (next[i]) {
				delete next[i];
				next[i] = nullptr;
			}
		}
		isEnd = true;
	}

	string value;
	trie *next[26];
	bool isEnd;

	bool insert(string word)
	{
		int i = 0;
		while (i < word.size() and i < value.size() and word[i] == value[i]) i++;
		
		if (i == word.size() and i == value.size() and isEnd) {
			return true;
		} else if (i == word.size()) {
			trie *toAdd = new trie(value.substr(i));
			for (int i = 0; i < 26; i++) {
				toAdd->next[i] = next[i];
				next[i] = nullptr;
			}
			toAdd->isEnd = isEnd;
			next[value[i] - 'a'] = toAdd;
			isEnd = true;
			value.erase(i);
			return false;
		} else if (i == value.size()) {
			if (next[word[i] - 'a'] != nullptr) {
				return next[word[i] - 'a']->insert(word.substr(i));
			} else {
				next[word[i] - 'a'] = new trie(word.substr(i));
				return false;
			}
		} else {
			trie *toAdd = new trie(value.substr(i));
			for (int i = 0; i < 26; i++) {
				toAdd->next[i] = next[i];
				next[i] = nullptr;
			}
			toAdd->isEnd = isEnd;
			next[value[i] - 'a'] = toAdd;
			next[word[i] - 'a'] = new trie(word.substr(i));
			isEnd = false;
			value.erase(i);
			return false;
		}
	}

	bool has(string word)
	{
		if (word.size() < value.size()) {
			return false;
		}

		if (strncmp(word.c_str(), value.c_str(), value.size()) == 0) {
			if (word.size() == value.size() and isEnd) {
				return true;
			} else if (next[word[value.size()] - 'a'] != nullptr) {
				return next[word[value.size()] - 'a']->has(word.substr(value.size()));
			} else {
				return false;
			}
		} else {
			return false;
		}
	}

	void print(string tab = "")
	{
		cout << tab << value << endl;
		for (int i = 0; i < 26; i++) {
			if (next[i]) {
				next[i]->print(tab + "\t");
			}
		}
	}
};

int main(int argc, char **argv)
{
	trie t;
	t.isEnd = false;
	ofstream tofile;
	
	if (argc > 1) {
		tofile.open(argv[1], ios::out);
		if (tofile.is_open()) {
			string word;
			while (getline(cin, word)) {
				cout << word;
				if (not t.insert(word)) {
					tofile << word << endl;
					cout << "+" << endl;
				}
			}
		}
		tofile.close();
	}
}


