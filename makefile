




.PHONY:all run clean test


all : 
	cd build && make -j4
run:
	./build/lox.out
test:
	./build/lox.out sample.lox
clean:
	cd build && make clean
