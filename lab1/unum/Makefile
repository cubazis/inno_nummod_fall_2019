CFLAGS = -std=c99 -pedantic -Wall -lcheck

TARGET = tests/test_dot_product_posit

all: $(TARGET) clean

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $^

$(TARGET): $(TARGET).o src/posit.o
	$(CC) $(CFLAGS) -o $@ $^

run_tests: 
	./$(TARGET)

clean:
	rm -f *.o
	rm -f src/*.o
	rm -f tests/*.o
