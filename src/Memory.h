#ifndef MEMORY_H_
#define MEMORY_H_

#include "shared.h"
#include <map>


class Memory{
public:

	template <class T>
	T get(uInt addr);

	template <class T>
	void set(uInt addr, T val);

	template <class T>
	T* getp(uInt addr);

	~Memory();
private:
	map<uInt,void*> pages;

	void* create_page(uInt addr);
};


template <class T>
T Memory::get(uInt addr){
	const uInt page_addr = addr & PAGE_MASK;
	const uInt page_bits = addr & PAGE_BITS;

	void* const page = pages[page_addr];
	T* pval = static_cast<T*>(page) + page_bits;

	return (page != NULL) ? *pval : 0;
};


template <class T>
T* Memory::getp(uInt addr){
	const uInt page_addr = addr & PAGE_MASK;
	const uInt page_bits = addr & PAGE_BITS;

	void* page = pages[page_addr];
	if (page == NULL) page = create_page(page_addr);

	return static_cast<T*>(page) + page_bits;
};


template <class T>
void Memory::set(uInt addr, T val){
	*getp<T>(addr) = val;
};


#endif /* MEMORY_H_ */
