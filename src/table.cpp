#include "table.h"

table::table()
{
	fptr = nullptr;
	size = 0;
	stride = 0;
}

table::table(int stride, const char *name)
{
	this->stride = stride;
	fptr = fopen(name, "rw");
	fseek(fptr, 0, SEEK_END);
	size = ftell(fptr)/stride;
}

table::~table()
{
	if (fptr != nullptr) {
		fclose(fptr);
		size = 0;
		stride = 0;
	}
	fptr = nullptr;
}

