#pragma once

#include <string>
#include <string.h>
#include <unordered_map>
#include <sstream>

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
template <typename value_type>
struct trie
{
	trie(string key = "")
	{
		this->key = key;
		this->value = nullptr;
	}

	trie(string key, const value_type &value)
	{
		this->key = key;
		this->value = new value_type(value);
	}

	~trie()
	{
		clear();
	}

	using child_iterator = typename unordered_map<char, trie<value_type>*>::iterator;
	using child_const_iterator = typename unordered_map<char, trie<value_type>*>::const_iterator;

	template <typename... arg_types>
	using condition_function = bool(const value_type&, arg_types...);

	string key;
	value_type *value;
	unsigned int found:1;

	unordered_map<char, trie<value_type>*> child;

	void clear()
	{
		key = "";
		delete value;
		value = nullptr;

		for (child_iterator i = child.begin(); i != child.end(); i++) {
			delete i->second;
		}
		child.clear();
	}

	// Insert a word into the trie. Return false if the word was already in the
	// trie.
	bool insert(string key, const value_type &value = value_type())
	{
		// First, we have to compare the prefix stored in this node against the
		// first part of the input word. Our goal is to determine the index at
		// which they differ.
		int i = 0;
		while (i < this->key.size() and i < key.size() and this->key[i] == key[i]) i++;
		
		if (i == key.size() and i == this->key.size()) {
			// If they're the same and this node represents the end of a word, then the
			// word is already in the dictionary and we return that we found the word.
			bool result = (this->value == nullptr);
			this->value = new value_type(value);
			return result;
		} else if (i == key.size()) {
			// Otherwise, if the key at this node is longer than the input word.
			// This happens when the inserted word is a substring of an already
			// inserted word. We need to split this node into two so that we can
			// designate where the newly inserted word ends.
			
			// Create a child node for the second half of the key of this node and move all 
			// other children of this node to that new child node.
			trie<value_type> *toAdd = new trie<value_type>(this->key.substr(i));
			toAdd->value = this->value;
			toAdd->child = this->child;

			// This node now designates the end of the newly inserted word, the child
			// map now only stores the newly inserted child node.
			this->value = new value_type(value);
			this->child.clear();
			this->child.insert(pair<char, trie<value_type>*>(this->key[i], toAdd));

			// This node must now only contain the last part of the newly inserted word.
			this->key.erase(i);

			// The inserted word was not already in the dictionary so we return true.
			return true;
		} else if (i == this->key.size()) {
			// Otherwise, if the value in this node is shorter than the input word,
			// then we need to recurse. This happens when a substring of the inserted
			// word is already in the dictionary. Check to see if we have the
			// appropriate child node for the continuatio on of the inserted word. If
			// there is one, then we recurse. If not, then we simply insert the child
			// with the remaining half of the word.
			child_iterator n = this->child.find(key[i]);
			if (n != this->child.end()) {
				return n->second->insert(key.substr(i), value);
			} else {
				this->child.insert(pair<char, trie<value_type>*>(key[i], new trie<value_type>(key.substr(i), value)));
				return true;
			}
		} else {
			// Finally, the input string is different from the base string some point
			// in the middle. This means we'll have to create a branch point here.

			// So, create a new node to store the remaining half of this node's
			// prefix.
			trie<value_type> *toAdd = new trie<value_type>(this->key.substr(i));
			toAdd->value = this->value;
			toAdd->child = this->child;
			
			// Then, move all of the children to that new node and add the decision
			// point as the two new children.
			this->value = nullptr;
			this->child.clear();
			this->child.insert(pair<char, trie<value_type>*>(this->key[i], toAdd));
			this->child.insert(pair<char, trie<value_type>*>(key[i], new trie<value_type>(key.substr(i), value)));
			
			this->key.erase(i);
			return true;
		}
	}

	// Check if a word is in the trie
	value_type *get(string key)
	{
		// To check if a word is in the trie, we need to first compare it against the prefix
		if (this->key.size() <= key.size() and strncmp(key.c_str(), this->key.c_str(), this->key.size()) == 0) {
			// The test word starts with the prefix in this node, so we need to check
			// other conditions.
			if (this->key.size() == key.size() and this->value != nullptr) {
				// If this prefix is the same length as the word and this marks the end
				// of a word in the dictionary, then we are done.
				return this->value;
			} else {
				// Otherwise, if the child letter in the given word is in the children,
				// we need to recurse.
				child_iterator n = this->child.find(key[this->key.size()]);
				if (n != this->child.end()) {
					return n->second->get(key.substr(this->key.size()));
				} else {
					return nullptr;
				}
			}
		} else {
			return nullptr;
		}
	}

	template <typename... arg_types>
	bool set_condition(condition_function<arg_types...> *cmp = nullptr, arg_types... args)
	{
		found = 0;

		for (child_iterator i = this->child.begin(); i != this->child.end(); i++) {
			if (i->second != nullptr and i->second->set_condition(cmp, args...)) {
				found = 1;
			}
		}

		if (cmp == nullptr or (this->value != nullptr and cmp(*this->value, args...))) {
			found = 1;
		}

		return found;
	}
};

template <typename value_type>
ostream &operator<<(ostream &str, const trie<value_type> &t)
{
	static const char stx = '('; // start of text
	static const char etx = ')'; // end of text
	static const char gs  = ','; // group separator
	static const char rs = ':'; // start of text

	str << t.key;
	if (t.value != nullptr) {
		str << rs << *t.value;
	}

	if (not t.child.empty() and t.found == 1) {
		bool first = true;
		for (typename trie<value_type>::child_const_iterator i = t.child.begin(); i != t.child.end(); i++) {
			if (i->second and i->second->found == 1) {
				if (!first) {
					str << gs;
				} else {
					str << stx;
					first = false;
				}
				str << *(i->second);
			}
		}
		if (!first) {
			str << etx;
		}
	}

	return str;
}

template <typename value_type>
istream &operator>>(istream &str, trie<value_type> &t)
{
	static const char stx = '('; // start of text
	static const char etx = ')'; // end of text
	static const char gs  = ','; // group separator
	static const char rs = ':'; // record separator

	string key;
	char c = str.peek();
	while (((key.size() < t.key.size() and c == t.key[key.size()])
	   or (t.child.empty() and t.value == nullptr))
    and c != stx and c != rs and c != gs and c != etx and c != '\0') {
		key += str.get();
		c = str.peek();
	}

	if (t.child.empty() and t.value == nullptr) {
		t.key = key;
	} else if (key.size() < t.key.size()) {
		trie<value_type> *toAdd = new trie<value_type>(t.key.substr(key.size()), t.value);
		toAdd->child = t.child;

		// This node now designates the end of the newly inserted key, the t.child
		// map now only stores the newly inserted t.child node.
		
		t.child.clear();
		t.child.insert(pair<char, trie<value_type>*>(t.key[key.size()], toAdd));

		// This node must now only contain the last part of the newly inserted key.
		t.key.erase(key.size());
	}

	if (c == rs) {
		str.get();
		if (t.value == nullptr) {
			t.value = new value_type();
		}
		str >> *t.value;
		c = str.peek();
	}

	if (c == stx) {
		do {
			c = str.get();
			c = str.peek();
			typename trie<value_type>::child_iterator n = t.child.find(c);
			if (n == t.child.end()) {
				n = t.child.insert(pair<char, trie<value_type>*>(c, new trie<value_type>("", nullptr))).first;
			}
			str >> n->second;
			c = str.peek();
		}	while (c == gs);
		c = str.get();
	} else if (c != etx and c != gs and c != '\0') {
		typename trie<value_type>::child_iterator n = t.child.find(c);
		if (n == t.child.end()) {
			n = t.child.insert(pair<char, trie<value_type>*>(c, new trie<value_type>("", nullptr))).first;
		}
		str >> *(n->second);
	}

	return str;
}

