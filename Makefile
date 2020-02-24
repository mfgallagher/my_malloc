my-malloc.so: my-malloc.c
	gcc -g -Wall -pedantic -rdynamic -shared -fPIC -o my-malloc.so my-malloc.c

mallocTest: mallocTest.c
	gcc -g -Wall -pedantic -o mallocTest mallocTest.c

.PHONY: clean lib ls gdb gdbls
clean:
	rm mallocTest my-malloc.so

lib:
	LD_PRELOAD=./my-malloc.so ./mallocTest

ls:
	LD_PRELOAD=./my-malloc.so ls

gdb:
	gdb --args env LD_PRELOAD=./my-malloc.so ./mallocTest

gdbls:
	gdb --args env LD_PRELOAD=./my-malloc.so ls
