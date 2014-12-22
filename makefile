quoridojo: bin/quoridojo.o bin/duchess.o
	gcc -O0 -o quoridojo bin/quoridojo.o bin/duchess.o

bin/quoridojo.o: quoridojo/quoridojo.c
	gcc -O0 -o bin/quoridojo.o quoridojo/quoridojo.c

bin/duchess.o: duchess/duchess_init.c duchess/duchess_init.h duchess/choose_move_or_wall.c duchess/game_state.c
	gcc -O0 -o bin/duchess.o duchess/duchess_init.c duchess/duchess_init.h duchess/choose_move_or_wall.c duchess/game_state.c

clean:
	rm bin/*.o quoridojo
