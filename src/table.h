#pragma once

#include <string>
#include <fstream>

using namespace std;

template <typename value_type>
struct table
{
	table()
	{
		size = 0;
		stride = 0;
	}

	table(const char *name, unsigned long long stride)
	{
		this->stride = stride;
		file.open(name);
		file.seekp(0, ios_base::end);
		size = file.tellp()/stride;
	}

	~table()
	{
		if (file.is_open()) {
			file.close();
			size = 0;
			stride = 0;
		}
	}

	fstream file;
	unsigned long long stride;
	int size;

	void write(int index, const value_type &value)
	{
		if (index > size) {
			file.seekp(0, ios_base::end);
			for (int i = 0; i < index*stride - size; i++) {
				file.put('\0');
			}
		}

		file.seekp(index*stride, ios_base::beg);
		file << value;
	}

	bool read(int index, value_type &value)
	{
		if (index < size) {
			file.seekp(index*stride, ios_base::beg);
			file >> value;
			return true;
		} else {
			return false;
		}
	}
};

