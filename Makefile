# paths to build
src = $(wildcard *.c)
obj = $(src:.c=.o)

# libs to link against
libs = -lm -lpigpio -lpthread

# default build command
predator: $(obj)
	gcc -g $(libs) -o $@ $^ -I.

clean:
	rm *.o predator
