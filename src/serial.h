#pragma once

#include <string>
#include <sstream>
#include "trie.h"

using namespace std;

template <class value_type>
string serialize(value_type value)
{
	return string((char*)(&value), sizeof(value_type));
}

template<>
string serialize(string value)
{
	return value;
}

template <class value_type>
void deserialize(value_type *value, string str)
{
	str.copy((char*)value, sizeof(value_type));
}

template <>
string serialize(trie *value)
{
	stringstream result;
	value->write(result);
	return result.str(); 
}

template <>
void deserialize(trie **value, string str)
{
	if (*value == nullptr)
		*value = new trie();

	stringstream input(str);
	(*value)->read(input);
}


