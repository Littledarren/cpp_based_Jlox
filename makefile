




.PHONY:all run clean


all : 
	cd build && make -j4
run:
	./build/lox.out
clean:
	cd build && make clean
