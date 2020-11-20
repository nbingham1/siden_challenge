#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <unordered_map>

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
	unordered_map<char, trie*> next;
	bool isEnd;

	bool insert(string word)
	{
		int i = 0;
		while (i < word.size() and i < value.size() and word[i] == value[i]) i++;
		
		if (i == word.size() and i == value.size() and isEnd) {
			return true;
		} else if (i == word.size()) {
			trie *toAdd = new trie(value.substr(i));
			toAdd->next = next;
			next.clear();
			toAdd->isEnd = isEnd;
			next.insert(pair<char, trie*>(value[i], toAdd));
			isEnd = true;
			value.erase(i);
			return false;
		} else if (i == value.size()) {
			unordered_map<char, trie*>::iterator n = next.find(word[i]);
			if (n != next.end()) {
				return n->second->insert(word.substr(i));
			} else {
				next.insert(pair<char, trie*>(word[i], new trie(word.substr(i))));
				return false;
			}
		} else {
			trie *toAdd = new trie(value.substr(i));
			toAdd->next = next;
			next.clear();
			toAdd->isEnd = isEnd;
			next.insert(pair<char, trie*>(value[i], toAdd));
			next.insert(pair<char, trie*>(word[i], new trie(word.substr(i))));
			isEnd = false;
			value.erase(i);
			return false;
		}
	}

	bool has(string word)
	{
		return value.size() <= word.size()
		   and strncmp(word.c_str(), value.c_str(), value.size()) == 0
       and ((word.size() == value.size()
		     and isEnd
		     ) or (next.count(word[value.size()])
		     and next[word[value.size()]]->has(word.substr(value.size()))
		     ));
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


