FLAGS = -O2 -g

LIB_OBJ = lib/posit.o lib/pack.o lib/util.o lib/op1.o lib/op2.o
LIB_TARGET = lib/libbfp.a

TARGET=tests/test_dot_product_posit
OBJ = tests/test_dot_product_posit.o $(LIB_TARGET)

all: $(TARGET)
	rm -f lib/*.o
	rm -f tests/*.o
	rm -f *.o

run_tests:
	./$(TARGET)

%: %.cpp
	$(CXX) -stdlib=libc++ -std=c++11 -o $@  $^ -lboost_unit_test_framework

# run-%: %
# 	-./$^ --output_format=XML --log_level=test_suite > $(^)-report.xml

clean:
	rm -f lib/*.o
	rm -f *.o
	rm $(LIB_TARGET)
	rm $(TARGET) # *-report.xml


$(LIB_TARGET): $(LIB_OBJ)
	ar rcs $@ $^

$(TARGET): $(OBJ)
	$(CXX) -stdlib=libc++ -std=c++11 -o $@  $^ -lboost_unit_test_framework

%.o: %.c
	$(CC) -o $@ $(FLAGS) -c $^

%.o: %.cpp
	$(CXX) -stdlib=libc++ -std=c++11 -o $@ $(FLAGS) -c $^