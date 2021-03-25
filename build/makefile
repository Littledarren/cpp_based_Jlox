CC      = g++
#clang++
CFLAGS  = -O2 -std=c++11
DEBUG_CFLAGS  = -Wall -O0 -pg -std=c++11
INCLUDEFLAGS = -I ../includes/
LDFLAGS =
OBJS    = Object.o main.o  Value.o Lexer.o Parser.o  RecursiveDescentParser.o Environment.o Interpreter.o LoxCallable.o Resolver.o LoxInstance.o TokenType.o
TARGETS = lox.out

vpath %.cpp ../src
vpath %.h ../includes


.PHONY:all

all : $(TARGETS)

$(TARGETS): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(INCLUDEFLAGS)

%.o:%.cpp
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDEFLAGS)

%.d:%.cpp
	@set -e; rm -f $@; $(CC) -MM $< $(INCLUDEFLAGS) > $@.$$$$; \
		sed 's,\($*\)\.o[ : ]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

-include $(OBJS:.o=.d)

.PHONY:clean
clean:
	rm -f $(TARGETS) *.o *.d *.d.*

