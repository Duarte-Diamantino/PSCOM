CC= gcc 						# compilador a usar
CFLAGS = -g -Wall -ansi -pedantic 			# opções de compilação
LFLAGS = -g 						# opções de linkagem
LIBS = -lm 						# bibliotecas a usar
OBJS = main.o le_ficheiro.o lista_ord.o			# ficheiros objecto
EXEC = pce 						# nome do executável
$(EXEC): $(OBJS)
	$(CC) $(LFLAGS) -o $(EXEC) $(OBJS) $(LIBS)
# gcc -g -o colors green.o blue.o -lm
le_ficheiro.o: le_ficheiro.c empresas.h
	$(CC) $(CFLAGS) -c le_ficheiro.c
lista_ord.o: lista_ord.c empresas.h
	$(CC) $(CFLAGS) -c lista_ord.c
clean:
	echo "Apagar todos os ficheiros objecto e executavel"
	rm -f *.o $(EXEC)
