#ifndef SHARED_H_
#define SHARED_H_

using namespace std;

#include <iostream>
#include <string>
#include <malloc.h>

#define FREE(ptr) { if ((ptr) != NULL){ free(((void*)ptr)); (ptr) = NULL; } }
#define PAGE_SIZE 4096
#define UNUSED __attribute__ ((unused))
#define ALIGNED(sz) __attribute__ ((__aligned__(sz)))

typedef void* PPAGE ALIGNED(4096);


UNUSED static inline const void fatalError(string msg){
	cerr << msg;
	exit(EXIT_FAILURE);
};


template <class T>
UNUSED static inline T arealloc(size_t sz, size_t align, T src, size_t cpsz){
	T const ptr = static_cast<T>(memalign(align, sz));
	if (ptr == NULL) fatalError("Failed to (re-)allocate memory!\n");
	if (src != NULL) memcpy(ptr, src, ((cpsz <= sz) ? cpsz : sz));
	return ptr;
};


#endif /* SHARED_H_ */
