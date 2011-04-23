#ifndef STLMAPMEM_HPP_
#define STLMAPMEM_HPP_

#include "MemoryStorage.hpp"
#include <map>


class stlmapmem : public MemoryStorage{
public:
	stlmapmem(){};

	~stlmapmem(){
		for (pair<uInt,void*> elm : pages)
			FREE(elm.second);

		pages.clear();
	};

	void* const getsetpage(const uInt addr, void* const page){
		return pages[addr] = page;
	};

	void const setpage(const uInt addr, void* const page){
		pages[addr] = page;
	};

	void* const getpage(const uInt addr) const{
		auto const it = pages.find(addr);
		return (it != pages.end()) ? (*it).second : NULL;
	};


private:
	map<uInt,void*> pages;
};


#endif /* STLMAPMEM_HPP_ */
