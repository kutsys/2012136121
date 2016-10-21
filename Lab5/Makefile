mtest : mtest.o foo.o boo.o bar.o
	gcc -g -o mtest mtest.o foo.o boo.o bar.o

mtest.o : mtest.c
	gcc -c mtest.c

foo.o : foo.c
	gcc -c foo.c

boo.o : boo.c
	gcc -c boo.c

bar.o : bar.c
	gcc -c bar.c
