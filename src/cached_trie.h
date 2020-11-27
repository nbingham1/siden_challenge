#pragma once

#include "table.h"
#include "trie.h"

using namespace std;


template <typename value_type>
int byte_length(const value_type &value)
{
	stringstream str;
	str << value;
	return str.tellp();
}

template <typename value_type>
int byte_length(const trie<value_type> &t)
{
	int result = t.key.size();
	if (t.value != nullptr) {
		result += 1;
		result += byte_length(*t.value);
	}

	if (t.child.size() > 0) {
		result += 1;
		bool first = true;
		for (typename trie<value_type>::child_iterator i = t.child.begin(); i != t.child.end(); i++) {
			if (i->second) {
				if (!first) {
					result += 1;
				}
				first = false;
				result += byte_length(*(i->second));
			}
		}
		result += 1;
	}
	return result;
}

template <typename value_type>
struct cached
{
	cached(unsigned long hash)
	{
		this->hash = hash;
		this->dirty = 1;
	}

	cached(unsigned long hash, const value_type &value)
	{
		this->value = value;
		this->hash = hash;
		this->dirty = 1;
	}
	~cached()
	{
	}

	value_type value;
	unsigned int hash;
	unsigned int dirty:1;
};

template <typename value_type>
ostream &operator<<(ostream &str, cached<value_type> value)
{
	//str << value.value;
	return str;
}

template <typename value_type>
istream &operator>>(istream &str, cached<value_type> value)
{
	//str >> value.value;
	return str;
}

template <typename value_type>
bool has_hash(const cached<value_type> &value, unsigned int hash, unsigned int mask)
{
	return (value.hash & mask) == (hash & mask);
}

template <typename value_type>
struct cached_trie : table<trie<cached<value_type> > >
{
	cached_trie()
	{
		this->memory = 4294967296;
		this->shift = 4;
	}

	cached_trie(const char *name, unsigned long long stride = 4096, unsigned long long memory = 4294967296ull) : table<trie<cached<value_type> > >(name, stride)
	{
		this->memory = memory;
		this->shift = 4;
	}
	~cached_trie()
	{
	}

	trie<cached<value_type> > cache;
	unsigned int shift;
	unsigned long long memory;

	using table<trie<cached<value_type> > >::write;
	using table<trie<cached<value_type> > >::read;

	void save(unsigned int hash)
	{
		cache.set_condition(&has_hash<value_type>, hash, (1u << shift)-1u);
		write(hash & ((1<<shift)-1), cache);
	}

	bool insert(string key, const value_type &value = value_type())
	{
		static std::hash<string> hasher;
		cached<value_type> c_value(hasher(key), value);
		cout << (c_value.hash & ((1u<<shift)-1u)) << ":\t" << key << endl;
		return cache.insert(key, c_value);		
	}
};


