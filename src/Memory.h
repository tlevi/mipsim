#ifndef MEMORY_H_
#define MEMORY_H_

#include "shared.h"
#include "pagetables.hpp"
#include "MemoryStorage.hpp"


class Memory{
public:

	template <class T> T const getu(const uInt addr);
	template <class T> T* const getpu(const uInt addr);


	template <class T> void const checkAlign(const uInt addr) const{
		// this should really throw an exception
		// in the vm for now we explode
		if (addr % sizeof(T) != 0)
			fatalError("Misaligned access\n");
	};


	template <class T> T const get(const uInt addr){
		checkAlign<T>(addr);
		return getu<T>(addr);
	};


	template <class T> T* const getp(const uInt addr){
		checkAlign<T>(addr);
		return getpu<T>(addr);
	};


	template <class T> void const set(const uInt addr, const T val){
		checkAlign<T>(addr);
		*getpu<T>(addr) = val;
	};


	template <class T> void const setu(const uInt addr, const T val){
		*getpu<T>(addr) = val;
	};


	Memory();
	~Memory();


private:

	MemoryStorage& storage;

	void* create_page(uInt addr);
};


template <class T> T const Memory::getu(const uInt addr){
	const uInt page_addr = addr & PAGE_MASK;
	const uInt page_bits = addr & PAGE_BITS;

	void* const page = storage.getpage(page_addr);
	T* const pval = static_cast<T*>(page) + page_bits;

	return (page != NULL) ? *pval : 0;
};


template <class T> T* const Memory::getpu(const uInt addr){
	const uInt page_addr = addr & PAGE_MASK;
	const uInt page_bits = addr & PAGE_BITS;

	void* page = storage.getpage(page_addr);

	if (page != NULL)
		return static_cast<T*>(page) + page_bits;

	page = create_page(page_addr);
	storage.setpage(page_addr, page);
	return static_cast<T*>(page) + page_bits;
};


#endif /* MEMORY_H_ */
