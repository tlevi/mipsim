#include "pagetables.hpp"


void* const pagetables::getpage(const uInt page_addr) const{
	const uInt tableid = PAGEDIR_ADDR2TABLEID(page_addr);
	const pagetable_t* const ppt = pagedir.entry[tableid];

	if (ppt != NULL){
		const uInt pageid = PAGETABLE_ADDR2PAGEID(page_addr);
		void* const page = ppt->entry[pageid];
		return (page != NULL) ? page : NULL;
	}

	return NULL;
};


void* const pagetables::getsetpage(const uInt page_addr, void* const newpage){
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


void const pagetables::setpage(const uInt page_addr, void* const newpage){
	const uInt tableid = PAGEDIR_ADDR2TABLEID(page_addr);
	pagetable_t* ppt = pagedir.entry[tableid];

	if (ppt == NULL){
		void* const ptr = aalloc(sizeof(pagetable_t), 4096);
		ppt = pagedir.entry[tableid] = new (ptr) pagetable_t;
	}

	const uInt pageid = PAGETABLE_ADDR2PAGEID(page_addr);
	ppt->entry[pageid] = newpage;
};
