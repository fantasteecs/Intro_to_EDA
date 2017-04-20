CC	= g++
TARGET	= cadb027
cadb027:  main.cpp 
	$(CC) -o $@ $< parser.cpp parser.h

.PHONY: clean host run run-host

run_example: 
	./$(TARGET) example  example_true_path_set 10 7
run_case1: 
	./$(TARGET) case1  case1_true_path_set 45 4
run_case2: 
	./$(TARGET) case2  case2_true_path_set 43 10
run_case3: 
	./$(TARGET) case3 case3_true_path_set 31 6
run_case4: 
	./$(TARGET) case4 case4_true_path_set 45 6
run_case5: 
	./$(TARGET) case5 case5_true_path_set 47 10


