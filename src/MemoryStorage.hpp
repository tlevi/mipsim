#ifndef MEMORYSTORAGE_HPP_
#define MEMORYSTORAGE_HPP_


#include "shared.h"


class MemoryStorage{
public:
	virtual void* const getsetpage(const uInt page_addr, void* const page) = 0;
	virtual void const setpage(const uInt page_addr, void* const page) = 0;
	virtual void* const getpage(const uInt page_addr) const = 0;

	virtual ~MemoryStorage(){};

protected:
	MemoryStorage(){};
};


#endif /* MEMORYSTORAGE_HPP_ */
