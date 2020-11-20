#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <unordered_map>

#include "trie.h"

using namespace std;

/* C++ does not have a built in trie structure, so we have to implement one
 * ourselves. A trie structure puts the first character of a string at the root
 * of a tree and subsequent characters at child nodes. For example:
 *      h
 *     / \
 *    i   e
 *         \
 *          l
 *           \
 *            l
 *             \
 *              o
 *
 * This tree structure has an optimization that compresses tree nodes with only
 * one child like so:
 *      h
 *     / \
 *    i   ello
 */
trie::trie(string value, bool isEnd) {
	this->value = value;
	this->isEnd = isEnd;
}

trie::~trie() {
	isEnd = false;
	next.clear();
}

// Insert a word into the trie. Return false if the word was already in the
// trie.
bool trie::insert(string word)
{
	// First, we have to compare the prefix stored in this node against the
	// first part of the input word. Our goal is to determine the index at
	// which they differ.
	int i = 0;
	while (i < word.size() and i < value.size() and word[i] == value[i]) i++;
	
	if (i == word.size() and i == value.size() and isEnd) {
		// If they're the same and this node represents the end of a word, then the
		// word is already in the dictionary and we return that we found the word.
		return false;
	} else if (i == word.size()) {
		// Otherwise, if the value at this node is longer than the input word.
		// This happens when the inserted word is a substring of an already
		// inserted word. We need to split this node into two so that we can
		// designate where the newly inserted word ends.
		
		// Create a child node for the second half of the value of this node and move all 
		// other children of this node to that new child node.
		trie *toAdd = new trie(value.substr(i), true);
		toAdd->isEnd = isEnd;
		toAdd->next = next;

		// This node now designates the end of the newly inserted word, the next
		// map now only stores the newly inserted child node.
		isEnd = true;
		next.clear();
		next.insert(pair<char, trie*>(value[i], toAdd));

		// This node must now only contain the last part of the newly inserted word.
		value.erase(i);

		// The inserted word was not already in the dictionary so we return true.
		return true;
	} else if (i == value.size()) {
		// Otherwise, if the value in this node is shorter than the input word,
		// then we need to recurse. This happens when a substring of the inserted
		// word is already in the dictionary. Check to see if we have the
		// appropriate child node for the continuatio on of the inserted word. If
		// there is one, then we recurse. If not, then we simply insert the child
		// with the remaining half of the word.
		unordered_map<char, trie*>::iterator n = next.find(word[i]);
		if (n != next.end()) {
			return n->second->insert(word.substr(i));
		} else {
			next.insert(pair<char, trie*>(word[i], new trie(word.substr(i), true)));
			return true;
		}
	} else {
		// Finally, the input string is different from the base string some point
		// in the middle. This means we'll have to create a branch point here.

		// So, create a new node to store the remaining half of this node's
		// prefix.
		trie *toAdd = new trie(value.substr(i), true);
		toAdd->isEnd = isEnd;
		toAdd->next = next;
		
		// Then, move all of the children to that new node and add the decision
		// point as the two new children.
		isEnd = false;
		next.clear();
		next.insert(pair<char, trie*>(value[i], toAdd));
		next.insert(pair<char, trie*>(word[i], new trie(word.substr(i), true)));
		
		value.erase(i);
		return true;
	}
}

// Check if a word is in the trie
bool trie::has(string word)
{
	// To check if a word is in the trie, we need to first compare it against the prefix
	return value.size() <= word.size()
		 and strncmp(word.c_str(), value.c_str(), value.size()) == 0
		 // The test word starts with the prefix in this node, so we need to
		 // check other conditions. If this prefix is the same length as the word
		 // and this marks the end of a word in the dictionary, then we are done.
		 and ((word.size() == value.size()
			 and isEnd
			 // Otherwise, if the next letter in the given word is in the children,
			 // we need to recurse.
			 ) or (next.count(word[value.size()])
			 and next[word[value.size()]]->has(word.substr(value.size()))
			 ));
}

// Prints a representation of the trie for debugging purposes.
void trie::print(string tab)
{
	cout << tab << value << endl;
	for (int i = 0; i < 26; i++) {
		if (next[i]) {
			next[i]->print(tab + "\t");
		}
	}
}

