HEADER = game_logic.h
EXEC = ruletka

all: $(EXEC)

$(EXEC): main.o game_logic.o
	gcc main.o game_logic.o -o $(EXEC)

main.o: main.c $(HEADER)
	gcc -c main.c

game_logic.o: game_logic.c $(HEADER)
	gcc -c game_logic.c

TEST_EXEC = test_game_logic
test: $(TEST_EXEC)
	./$(TEST_EXEC)

$(TEST_EXEC): tests/test_game_logic.o game_logic.o
	gcc tests/test_game_logic.o game_logic.o -o $(TEST_EXEC)

tests/test_game_logic.o: tests/test_game_logic.c $(HEADER)
	gcc -I. -c tests/test_game_logic.c -o tests/test_game_logic.o

SEED_DATA = seed_data
$(SEED_DATA): seed_data.c
	gcc -o $(SEED_DATA) seed_data.c

clean:
	rm -f *.o tests/*.o $(EXEC) $(TEST_EXEC) $(SEED_DATA)
