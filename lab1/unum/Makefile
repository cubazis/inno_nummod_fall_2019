FLAGS = -O2 -g 

LIB_OBJ = src/posit.o src/pack.o src/util.o src/op1.o src/op2.o
LIB_TARGET = src/libbfp.a

TARGET=tests/test_dot_product_posit 
OBJ = tests/test_dot_product_posit.o $(LIB_TARGET)

all: $(TARGET)
	rm -f src/*.o 
	rm -f tests/*.o 
	rm -f *.o 
	

run_tests: 
	./$(TARGET)

%: %.cpp
	$(CXX) -std=c++11 -o $@  $^ -lboost_unit_test_framework

# run-%: %
# 	-./$^ --output_format=XML --log_level=test_suite > $(^)-report.xml

clean:
	rm -f src/*.o 
	rm -f *.o 
	rm $(LIB_TARGET)
	rm $(TARGET) # *-report.xml
	

$(LIB_TARGET): $(LIB_OBJ)
	ar rcs $@ $^

$(TARGET): $(OBJ)
	$(CXX) -std=c++11 -o $@  $^ -lboost_unit_test_framework

%.o: %.c
	$(CC) -o $@ $(FLAGS) -c $^

%.o: %.cpp
	$(CXX) -std=c++11 -o $@ $(FLAGS) -c $^ 

