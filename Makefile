.PHONY: clean
.SUFFIXES:
.SUFFIXES: .d .cpp .o

CFLAGS := -pipe -Wall -O3 -s -march=native -fno-rtti --std=gnu++0x -flto
LFLAGS += $(CFLAGS) -Wl,--as-needed

SRC := $(shell find src/ -iname *.cpp)
OBJ := $(patsubst %.cpp, %.o, $(SRC))

all: 
debug: CFLAGS :=$(patsubst -s,-g3,$(CFLAGS))
debug: CFLAGS += -Werror -DDEBUG

%.o: %.cpp Makefile
	g++ $(CFLAGS) -c $*.cpp -o $*.o
	@g++ -MM $(CFLAGS) $*.cpp > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

all: $(OBJ) mipsim
debug: all

mipsim: $(OBJ)
	g++ $(LFLAGS) $(OBJ) -o ./mipsim

clean:
	@rm -f `find src/ -iname *.o` `find src/ -iname *.d` ./mipsim

ifneq "$(MAKECMDGOALS)" "clean"
-include $(SRC:.cpp=.d)
endif
