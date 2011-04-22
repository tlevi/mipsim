.PHONY: clean
.SUFFIXES:
.SUFFIXES: .d .cpp .o

CFLAGS += -pipe -Wall -Ofast -s -march=native -fno-rtti --std=gnu++0x -flto
LFLAGS += $(CFLAGS) -Wl,--as-needed

SRC := $(shell find src/ -iname *.cpp)
OBJ := $(patsubst %.cpp, %.o, $(SRC))

all: $(OBJ) mipsim
profile: all
debug: all

debug: CFLAGS += -Werror -DDEBUG -DVALGRIND
debug: CFLAGS := $(CFLAGS:-O%=-O1)
debug: CFLAGS := $(CFLAGS:-s=-g3)
debug: CFLAGS := $(CFLAGS:-flto=)

profile: CFLAGS += -fno-inline-functions -finline-small-functions -DVALGRIND
profile: CFLAGS := $(CFLAGS:-s=-g3)
profile: CFLAGS := $(CFLAGS:-flto=)

%.o: %.cpp Makefile
	g++ $(CFLAGS) -c $*.cpp -o $*.o
	@g++ -MM $(CFLAGS) $*.cpp > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

mipsim: $(OBJ)
	g++ $(LFLAGS) $(OBJ) -o ./mipsim

clean:
	@rm -f `find src/ -iname *.o` `find src/ -iname *.d` ./mipsim

ifneq "$(MAKECMDGOALS)" "clean"
-include $(SRC:.cpp=.d)
endif
