quoridojo: bin/duchess.o bin/quoridojo.o
	gcc -O0 -g -Wall quoridojo.o common_structs.o choose_move_or_wall.o game_tree.o game_state.o duchess_init.o game_room.o -o quoridojo

bin/quoridojo.o: src/quoridojo/quoridojo.c src/quoridojo/quoridojo.h src/quoridojo/game_room.c src/quoridojo/game_room.h
	gcc -c -g -Wall src/quoridojo/quoridojo.c src/quoridojo/game_room.c 

bin/duchess.o: src/duchess/duchess_init.c src/duchess/duchess_init.h src/duchess/choose_move_or_wall.c src/duchess/game_state.c src/duchess/game_tree.c src/duchess/game_tree.h src/duchess/game_state.h src/duchess/choose_move_or_wall.h src/duchess/common_structs.h src/duchess/common_structs.c
	gcc -c -g -Wall src/duchess/duchess_init.c src/duchess/choose_move_or_wall.c src/duchess/game_state.c src/duchess/game_tree.c src/duchess/common_structs.c

clean:
	rm bin/*.o quoridojo *.o
