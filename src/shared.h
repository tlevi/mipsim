#ifndef SHARED_H_
#define SHARED_H_

using namespace std;

#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <string>
#include <malloc.h>
#include <cstring>

#if DEBUG && !DEBUGLEVEL
#define DEBUGLEVEL 1
#else
#define DEBUGLEVEL 0
#endif

#define FREE(ptr) { if ((ptr) != NULL){ free((void*)(ptr)); (ptr) = NULL; } }
#define DELETE(ptr) { if ((ptr) != NULL){ delete (ptr); (ptr) = NULL; } }
#define UNUSED __attribute__ ((unused))
#define NORETURN __attribute__ ((noreturn))
#define ALIGNED(sz) __attribute__ ((__aligned__(sz)))
#define PAGE_SIZE 4096
#define PAGE_MASK (~(PAGE_SIZE-1))
#define PAGE_BITS (PAGE_SIZE-1)
#define PAGE_SHIFT 12
#define PAGE_ALIGNED ALIGNED(PAGE_SIZE)

typedef void* PPAGE PAGE_ALIGNED;
typedef int32_t sInt;
typedef uint32_t uInt;
typedef int8_t sChar;
typedef uint8_t uChar;
typedef int64_t sLong;
typedef uint64_t uLong;
typedef int16_t sShort;
typedef uint16_t uShort;
typedef uintptr_t uIntptr;


UNUSED NORETURN static inline const void fatalError(string msg){
	cerr << "Fatal error: " << msg;
	exit(EXIT_FAILURE);
};


UNUSED static inline const void warn(string msg){
	cerr << "Warning: " << msg;
	return;
};


template <class T>
UNUSED static inline T arealloc(uIntptr sz, size_t align, T src, size_t cpsz){
	T const ptr = static_cast<T>(memalign(align, sz));
	if (ptr == NULL) fatalError("Failed to (re-)allocate memory!\n");
	if (src == NULL) memset(ptr, 0, sz);
	else memcpy(ptr, src, ((cpsz <= sz) ? cpsz : sz));
	return ptr;
};


UNUSED static inline void* aalloc(size_t sz, size_t align){
	void* ptr = memalign(align, sz);
	if (ptr == NULL) fatalError("Failed to allocate memory!\n");
	memset(ptr, 0, sz);
	return ptr;
};


#endif /* SHARED_H_ */
