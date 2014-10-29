quoridojo: quoridojo.o duchess.o
  gcc -O0 -o quoridojo quoridojo.o duchess.o

quoridojo.o: quoridojo.c
	gcc -O0 -c quoridojo.c

duchess.o: duchess_init.c duchess_init.h choose_move_or_wall.c game_state.c
	gcc -O0 -c duchess_init.c duchess_init.h choose_move_or_wall.c game_state.c

clean:
	rm *.o quoridojo
