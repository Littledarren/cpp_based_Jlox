CC      = g++ 
CFLAGS  = -Wall -O -I includes -g
INCLUDEFLAGS = 
LDFLAGS = 
OBJS    = main.o Scanner.o
TARGETS = lox 

vpath %.cpp src
vpath %.h includes


.PHONY:all 
	all : $(TARGETS)

lox: $(OBJS)
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

