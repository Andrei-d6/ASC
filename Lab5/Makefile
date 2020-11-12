CC = gcc

all: constants_optimization BMM ordonarea_buclelor

constants_optimization: constants_optimization.o
	$(CC) -o $@ $^

BMM: BMM.o
	$(CC) -o $@ $^

ordonarea_buclelor: ordonarea_buclelor.o
	$(CC) -o $@ $^

constants_optimization.o: constants_optimization.c
	$(CC) -c $^

BMM.o: BMM.c
	$(CC) -c $^

ordonarea_buclelor.o: ordonarea_buclelor.c
	$(CC) -c $^

clean:
	rm -f *.o constants_optimization ordonarea_buclelor BMM > /dev/null

