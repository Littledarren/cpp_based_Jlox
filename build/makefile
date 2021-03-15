CC      = g++ 
CFLAGS  = -Wall -O -g -std=c++11
INCLUDEFLAGS = -I ../includes/
LDFLAGS = 
OBJS    = Object.o main.o  Value.o Lexer.o Parser.o  RecursiveDescentParser.o Environment.o Interpreter.o
TARGETS = lox.out

vpath %.cpp ../src
vpath %.h ../includes


.PHONY:all 

all : $(TARGETS)

$(TARGETS): $(OBJS)
	    $(CC) -o $@ $^ $(LDFLAGS)

%.o:%.cpp
	    $(CC) -o $@ -c $< $(CFLAGS) $(INCLUDEFLAGS)

%.d:%.cpp
	    @set -e; rm -f $@; $(CC) -MM $< $(INCLUDEFLAGS) > $@.$$$$; \
			    sed 's,\($*\)\.o[ : ]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
				    rm -f $@.$$$$

-include $(OBJS:.o=.d)

.PHONY:clean 
clean:
	rm -f $(TARGETS) *.o *.d *.d.*

