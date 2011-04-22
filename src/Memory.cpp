#include "Memory.h"


void* Memory::create_page(uInt page_addr){
	// create the page
	void* page = aalloc(PAGE_SIZE, PAGE_SIZE);
	if (uIntptr(page) & 0xfff)
		warn("New page is not page-aligned!\n");

	// initialise to zero
	memset(page, 0, 4096);

	// Add it to table and return pointer
#ifndef PHYS_PAGE_TABLES
	return pages[page_addr] = page;
#else
	return phys_page_walk(page_addr, page);
#endif
};


Memory::Memory(){
};


Memory::~Memory(){
#ifndef PHYS_PAGE_TABLES
	for (pair<uInt,void*> elm : pages)
		FREE(elm.second);
#endif
};


#ifdef PHYS_PAGE_TABLES
void* const Memory::phys_page_walk(const uInt page_addr, void* const newpage){
	const uInt tableid = PAGEDIR_ADDR2TABLEID(page_addr);
	pagetable_t* ppt = pagedir.entry[tableid];

	if (ppt == NULL){
		void* const ptr = aalloc(sizeof(pagetable_t), 4096);
		ppt = pagedir.entry[tableid] = new (ptr) pagetable_t;
	}

	const uInt pageid = PAGETABLE_ADDR2PAGEID(page_addr);
	void* const page = ppt->entry[pageid];

	if (page != NULL)
		return page;

	if (newpage != NULL)
		return ppt->entry[pageid] = newpage;

	return NULL;
};


void* const Memory::phys_page_walk(const uInt page_addr){
	const uInt tableid = PAGEDIR_ADDR2TABLEID(page_addr);
	const pagetable_t* const ppt = pagedir.entry[tableid];

	if (ppt != NULL){
		const uInt pageid = PAGETABLE_ADDR2PAGEID(page_addr);
		void* const page = ppt->entry[pageid];
		return (page != NULL) ? page : NULL;
	}

	return NULL;
};
#endif
