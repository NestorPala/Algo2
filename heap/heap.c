#include "heap.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h> //strcpy(), strcmp()


struct heap {
	void** arr;
	size_t cantidad;
	size_t capacidad;
	cmp_func_t cmp;
};







