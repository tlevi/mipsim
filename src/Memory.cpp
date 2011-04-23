#include "Memory.h"
#include "stlmapmem.hpp"
#include "pagetables.hpp"
#include "flatmemory.hpp"


void* Memory::create_page(uInt page_addr){
	// create the page
	void* page = aalloc(PAGE_SIZE, PAGE_SIZE);
	if (uIntptr(page) & 0xfff)
		warn("New page is not page-aligned!\n");

	// initialise to zero
	memset(page, 0, 4096);

	// return pointer, it is SEP to store this
	return page;
};


Memory::Memory() : storage(*new stlmapmem()) {
};


Memory::~Memory(){
	delete &storage;
};
