CC = cc
EXEC = tests/test_dot_product_posit
OBJECT = tests/test_dot_product_posit.o
TARGET = tests/test_dot_product_posit.c

all: $(TARGET)
	rm -f src/*.o
	rm -f *.o
	rm -f tests/*.o

	$(CC) $(CFLAGS) -c src/posit.c -o src/posit.o
	$(CC) $(CFLAGS) -c $(TARGET) -o $(OBJECT)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJECT) src/posit.o -lcheck

run_tests: 
	./$(EXEC)

clean:
	rm -f src/*.o
	rm -f *.o
	rm -f tests/*.o
