#pragma once

#include <string>
#include <string.h>
#include <unordered_map>

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
struct trie
{
	trie(string value = "", bool isEnd = false);
	~trie();

	string value;
	unordered_map<char, trie*> next;
	bool isEnd;

	// Insert a word into the trie. Return false if the word was already in the
	// trie.
	bool insert(string word);

	// Check if a word is in the trie
	bool has(string word);
	
	// Prints a representation of the trie for debugging purposes.
	void print(string tab = "");
};

