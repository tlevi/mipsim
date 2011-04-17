#ifndef MEMORY_H_
#define MEMORY_H_

#include "shared.h"
#include <map>


class Memory{
public:
	uInt get(uInt addr);
	void set(uInt addr, uInt val);

	~Memory();
private:
	map<uInt,uInt*> pages;

	uInt* create_page(uInt addr);
};


#endif /* MEMORY_H_ */
