OBJ := $(patsubst %.c,%.o,$(wildcard *.c))

msh: $(OBJ)
	gcc -o $@ $^

%.o: %.c
	gcc -c $< -o $@

clean:
	rm -f *.o msh