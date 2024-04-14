objects = main.o

l2l_c : $(objects)
	gcc -o l2l_c $(objects)
main.o : main.c
	gcc -c main.c
clean :
	rm l2l_c $(objects)
