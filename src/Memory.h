#ifndef MEMORY_H_
#define MEMORY_H_

#include "shared.h"

#ifndef PHYS_PAGE_TABLES
#include <map>
#else
#include "pagetables.h"
#endif

class Memory{
public:

	template <class T>
	T get(uInt addr);

	template <class T>
	void set(uInt addr, T val);

	template <class T>
	T* getp(uInt addr);

	Memory();
	~Memory();

private:
#ifndef PHYS_PAGE_TABLES
	map<uInt,void*> pages;
#else
	pagedir_t pagedir ALIGNED(64);
	void* const phys_page_walk(const uInt page_addr, void* const page);
	void* const phys_page_walk(const uInt page_addr);
#endif

	void* create_page(uInt addr);
};


template <class T>
T Memory::get(uInt addr){
#if DEBUGLEVEL > 1
	if (addr % sizeof(T) != 0)
		fatalError("Misaligned access\n");
#endif

	const uInt page_addr = addr & PAGE_MASK;
	const uInt page_bits = addr & PAGE_BITS;

#ifndef PHYS_PAGE_TABLES
	void* const page = pages[page_addr];
#else
	void* const page = phys_page_walk(page_addr);
#endif

	T* pval = static_cast<T*>(page) + page_bits;
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

#ifndef PHYS_PAGE_TABLES
	void* page = pages[page_addr];
	if (page == NULL) page = create_page(page_addr);
	return static_cast<T*>(page) + page_bits;

#else
	void* page = phys_page_walk(page_addr);
	if (page == NULL){
		page = create_page(page_addr);
		phys_page_walk(page_addr, page);
	}
	return static_cast<T*>(page) + page_bits;
#endif
};


template <class T>
void Memory::set(uInt addr, T val){
	*getp<T>(addr) = val;
};


#endif /* MEMORY_H_ */
