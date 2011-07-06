#ifndef FLATMEMORY_HPP_
#define FLATMEMORY_HPP_


// 4MB alignment for 4GB mem
#define FLAT_MEM_ALIGN (4*1024*1024)
#define MEMSIZE (uIntptr(4096)*1024*1024)


class flatmemory : public MemoryStorage{
private:
	void* const memory;


public:
	flatmemory() : memory(aalloc(MEMSIZE, FLAT_MEM_ALIGN)) {
		if (memory == NULL)
			fatalError("Failed to allocate memory\n");
	};

	~flatmemory(){
		free(memory);
	};

	void* const getsetpage(const uInt page_addr, void* const page){
		if (page != NULL) free(page);
		return reinterpret_cast<void*>(uIntptr(memory) + (page_addr & PAGE_MASK));
	};

	void const setpage(const uInt addr, void* const page){
		if (page != NULL) free(page);
	};

	void* const getpage(const uInt page_addr) const{
		return reinterpret_cast<void*>(uIntptr(memory) + (page_addr & PAGE_MASK));
	};
};


#endif /* FLATMEMORY_HPP_ */
