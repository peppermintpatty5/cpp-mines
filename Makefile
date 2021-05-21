CXXFLAGS	:= -std=c++2a -pedantic -Wall -Werror -Wextra
SOURCES		:= $(wildcard src/*.cpp)
OBJECTS		:= $(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS		:= $(patsubst %.cpp,%.d,$(SOURCES))
LDLIBS		:= -lncurses

.PHONY : all clean

all : a.out

clean :
	$(RM) $(OBJECTS) $(DEPENDS) a.out

a.out : $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDLIBS)

-include $(DEPENDS)

%.o : %.cpp Makefile
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@
