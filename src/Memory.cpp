#include "Memory.h"


uInt Memory::get(uInt addr){
	const uInt page_addr = addr & PAGE_MASK;
	const uInt page_bits = addr & PAGE_BITS;
	uInt* const page = pages[page_addr];

	return (page != NULL) ? page[page_bits] : 0;
};


void Memory::set(uInt addr, uInt val){
	const uInt page_addr = addr & PAGE_MASK;
	const uInt page_bits = addr & PAGE_BITS;

	uInt* page = pages[page_addr];
	if (page == NULL) page = create_page(page_addr);
	page[page_bits] = val;
};


uInt* Memory::create_page(uInt page_addr){
	const int max = PAGE_SIZE/sizeof(uInt);

	// create the page and blank it
	uInt* page = new uInt[max];
	for (int i=0; i < max; ++i) page[i] = 0;

	pages[page_addr] = page;
	return page;
};


Memory::~Memory(){
	for (pair<uInt,uInt*> elm : pages)
		delete elm.second;
};
