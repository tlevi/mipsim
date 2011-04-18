#include "Memory.h"


void Memory::set(uInt addr, uInt val){
	const uInt page_addr = addr & PAGE_MASK;
	const uInt page_bits = addr & PAGE_BITS;

	void* page = pages[page_addr];
	if (page == NULL) page = create_page(page_addr);
	static_cast<uInt*>(page)[page_bits] = val;
};


void* Memory::create_page(uInt page_addr){
	// create the page
	void* page = arealloc<void*>(PAGE_SIZE, PAGE_SIZE, NULL, 0);

	// initialise to zero
	memset(page, 0, 4096);

	// Add it to table and return pointer
	return pages[page_addr] = page;
};


Memory::~Memory(){
	for (pair<uInt,void*> elm : pages)
		FREE(elm.second);
};
