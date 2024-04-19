objects = main.o

l2l_c : $(objects)
	gcc -ansi -o l2l_c $(objects)
main.o : main.c
	gcc -ansi -c main.c
clean :
	rm l2l_c $(objects)
