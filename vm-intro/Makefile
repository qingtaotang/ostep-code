
all: va memory-user memory-example null memory-uninit

clean:
	rm -f va memory-user memory-example null memory-uninit

va: va.c
	gcc -o va va.c -Wall

memory-user: memory-user.c
	gcc -o memory-user memory-user.c -Wall

memory-example: memory-example.c
	gcc -g -o memory-example memory-example.c -Wall

null: null.c
	gcc -g -o null null.c -Wall

memory-uninit: memory-uninit.c
	gcc -g -o memory-uninit memory-uninit.c -Wall