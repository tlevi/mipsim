#ifndef MEMORY_H_
#define MEMORY_H_

#include "shared.h"
#include "pagetables.hpp"
#include "MemoryStorage.hpp"


class Memory{
public:

	template <class T> T get(uInt addr);

	template <class T> void set(uInt addr, T val);

	template <class T> T* getp(uInt addr);

	Memory();
	~Memory();


private:

	MemoryStorage& storage;

	void* create_page(uInt addr);
};


template <class T>
T Memory::get(uInt addr){
#if DEBUGLEVEL > 1
	// this should really throw an exception in the vm
	// for now we explode when some debugging is on
	if (addr % sizeof(T) != 0)
		fatalError("Misaligned access\n");
#endif

	const uInt page_addr = addr & PAGE_MASK;
	const uInt page_bits = addr & PAGE_BITS;

	void* const page = storage.getpage(page_addr);
	T* const pval = static_cast<T*>(page) + page_bits;

	return (page != NULL) ? *pval : 0;
};


template <class T>
T* Memory::getp(uInt addr){
#if DEBUGLEVEL > 1
	if (addr % sizeof(T) != 0)
		fatalError("Misaligned access\n");
#endif

	const uInt page_addr = addr & PAGE_MASK;
	const uInt page_bits = addr & PAGE_BITS;

	void* page = storage.getpage(page_addr);

	if (page != NULL)
		return static_cast<T*>(page) + page_bits;

	page = create_page(page_addr);
	storage.setpage(page_addr, page);
	return static_cast<T*>(page) + page_bits;
};


template <class T>
void Memory::set(uInt addr, T val){
	*getp<T>(addr) = val;
};


#endif /* MEMORY_H_ */
