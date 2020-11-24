#pragma once

#include <string>
#include "serial.h"

using namespace std;

struct table
{
	table();
	table(int stride, const char *name);
	~table();

	FILE *fptr;
	int stride;
	int size;

	template <class value_type>
	void write(int index, const value_type &value)
	{
		if (index > size) {
			fseek(fptr, 0, SEEK_END);
			char pad = '\0';
			fwrite(&pad, 1, index*stride - size, fptr);
		}

		string str = serialize(value);
		fseek(fptr, index*stride, SEEK_SET);
		fwrite(str.c_str(), str.size(), 1, fptr);
	}

	template <class value_type>
	bool read(value_type *value, int index)
	{
		if (index < size) {
			string str(stride, '\0');
			fseek(fptr, index*stride, SEEK_SET);
			fread(&str[0], sizeof(char), stride, fptr);
			deserialize<value_type>(value, str);
			return true;
		} else {
			return false;
		}
	}
};

